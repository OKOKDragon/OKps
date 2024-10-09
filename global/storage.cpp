#include <fstream>
#include <thread>
#include <atomic>

#include ".\byte.hpp"
#include ".\storage.hpp"


namespace OKps
{
	bool compare(std::filesystem::path const & left, std::filesystem::path const & right, std::uintmax_t const threads)
	{
		namespace fs = std::filesystem;

		if (not left.has_filename())
		{
			std::locale::global(std::locale::classic());
			std::string const hint = "路径 " + left.string() + " 不是文件";
			throw std::invalid_argument(hint);
		}
		if (not right.has_filename())
		{
			std::locale::global(std::locale::classic());
			std::string const hint = "路径 " + right.string() + " 不是文件";
			throw std::invalid_argument(hint);
		}
		if (not fs::exists(left))
		{
			std::locale::global(std::locale::classic());
			std::string const hint = "路径 " + left.string() + " 不存在";
			throw std::invalid_argument(hint);
		}
		if (not fs::exists(right))
		{
			std::locale::global(std::locale::classic());
			std::string const hint = "路径 " + right.string() + " 不存在";
			throw std::invalid_argument(hint);
		}
		if (left == right)
		{
			return true;
		}
		auto const length = fs::file_size(left);
		bool result;
		if (length != fs::file_size(right))
		{
			result = false;
		}
		else
		{
			std::uintmax_t work_threads = threads;
			if (work_threads == 0)
			{
				work_threads = std::thread::hardware_concurrency();
			}
			if (work_threads == 0)
			{
				work_threads = 1;
			}
			constexpr std::uintmax_t const least_buffer_length = static_cast<std::uintmax_t>(1024) * 64;
			if (length / work_threads < least_buffer_length)
			{
				work_threads = length / least_buffer_length;
			}
			std::uintmax_t const buffer_length = length / work_threads;
			std::uintmax_t const last_buffer_length = length - buffer_length * work_threads;
			class compare_worker final
			{
			private:
				std::atomic<bool> & MEMBER_result;
				std::thread MEMBER_worker;
				std::unique_ptr<char[]> MEMBER_left_buffer;
				std::unique_ptr<char[]> MEMBER_right_buffer;
				std::ifstream MEMBER_left;
				std::ifstream MEMBER_right;
				std::uintmax_t const MEMBER_buffer_length;
				std::uintmax_t const MEMBER_begin;
				fs::path const & MEMBER_left_route;
				fs::path const & MEMBER_right_route;
				std::exception_ptr MEMBER_error;
				void work()
				{
					if (this->MEMBER_result)
					{
						if (not this->MEMBER_left.read(this->MEMBER_left_buffer.get(), this->MEMBER_buffer_length))
						{
							try
							{
								std::locale::global(std::locale::classic());
								std::string const hint = std::string("文件 ")
									+ this->MEMBER_left_route.string()
									+ " 读取失败";
								throw std::runtime_error(hint);
							}
							catch (...)
							{
								this->MEMBER_error = std::current_exception();
								return;
							}
						}
						if (not this->MEMBER_right.read(this->MEMBER_right_buffer.get(), this->MEMBER_buffer_length))
						{
							try
							{
								std::locale::global(std::locale::classic());
								std::string const hint = std::string("文件 ")
									+ this->MEMBER_right_route.string()
									+ " 读取失败";
								throw std::runtime_error(hint);
							}
							catch (...)
							{
								this->MEMBER_error = std::current_exception();
								return;
							}
						}
					}
					for (std::uintmax_t i = 0; this->MEMBER_result and i < this->MEMBER_buffer_length;i++)
					{
						if (this->MEMBER_left_buffer[i] != this->MEMBER_right_buffer[i])
						{
							this->MEMBER_result = false;
						}
					}
				}
			public:
				compare_worker(std::atomic<bool> & result, std::uintmax_t const buffer_length, std::uintmax_t const begin, fs::path const & left, fs::path const & right)noexcept
					:MEMBER_result(result)
					, MEMBER_left_buffer(std::make_unique<char[]>(buffer_length))
					, MEMBER_right_buffer(std::make_unique<char[]>(buffer_length))
					, MEMBER_buffer_length(buffer_length)
					, MEMBER_begin(begin)
					, MEMBER_left(left, std::ios::in | std::ios::binary)
					, MEMBER_right(right, std::ios::in | std::ios::binary)
					, MEMBER_error()
					, MEMBER_left_route(left)
					, MEMBER_right_route(right)
				{
					if (not this->MEMBER_left.is_open())
					{
						std::locale::global(std::locale::classic());
						std::string const hint = std::string("文件 ")
							+ left.string()
							+ " 打开失败";
						throw std::runtime_error(hint);
					}
					if (not this->MEMBER_right.is_open())
					{
						std::locale::global(std::locale::classic());
						std::string const hint = std::string("文件 ")
							+ right.string()
							+ " 打开失败";
						throw std::runtime_error(hint);
					}
					this->MEMBER_worker = std::thread(&compare_worker::work, this);
				}
				void join()
				{
					this->MEMBER_worker.join();
				}
				std::exception_ptr const & error()
				{
					return this->MEMBER_error;
				}
				~compare_worker()noexcept
				{
					if (this->MEMBER_worker.joinable())
					{
						this->MEMBER_worker.join();
					}
				}
				compare_worker(compare_worker const &) = delete;
				compare_worker(compare_worker &&) = delete;
				void operator =(compare_worker const &) = delete;
				void operator =(compare_worker &&) = delete;
			};
			std::uintmax_t position = 0;
			auto temp_result = std::atomic<bool>(true);
			auto temp_workers = std::make_unique<std::unique_ptr<compare_worker>[]>(work_threads + 1);

			for (std::uintmax_t i = 0;i < work_threads;i++)
			{
				temp_workers[i] = std::make_unique<compare_worker>(temp_result, buffer_length, position, left, right);
				position += buffer_length;
			}
			temp_workers[work_threads] = std::make_unique<compare_worker>(temp_result, last_buffer_length, position, left, right);
			for (std::uintmax_t i = 0;i <= work_threads;i++)
			{
				temp_workers[i]->join();
				if (temp_workers[i]->error())
				{
					std::rethrow_exception(temp_workers[i]->error());
				}
			}
			result = temp_result;

			return result;
		}
	}
	std::vector<std::filesystem::path> totally_traverse_directory(std::filesystem::path const & directory, std::set<std::filesystem::file_type> const & result_types)
	{
		namespace fs = std::filesystem;

		if (not fs::is_directory(directory))
		{
			std::locale::global(std::locale::classic());
			std::string const hint = "路径 " + directory.string() + " 不是既存目录";
			throw std::invalid_argument(hint);
		}

		std::vector<std::filesystem::path> result;
		for (auto entry = fs::recursive_directory_iterator(directory, fs::directory_options::skip_permission_denied | fs::directory_options::follow_directory_symlink); entry != fs::end(entry); ++entry)
		{
			if (result_types.find(entry->status().type()) == result_types.end())
			{
				result.push_back(entry->path());
			}
		}
		return result;
	}
	field_stream::field_stream(std::string const & data)
		:MEMBER_data(std::make_unique<std::byte[]>(data.size()))
		, MEMBER_length(data.size())
	{
		for (std::size_t i = 0; i < data.size(); i++)
		{
			this->MEMBER_data[i] = value_cast(data[i]);
		}
	}
	field_stream::~field_stream()
		noexcept(std::is_nothrow_destructible_v<std::unique_ptr<std::byte[]>>)
	{
	}
	field_stream::field_stream(std::vector<std::byte> const & data)
		:MEMBER_data(std::make_unique<std::byte[]>(data.size()))
		, MEMBER_length(data.size())
	{
		for (std::size_t i = 0; i < data.size(); i++)
		{
			this->MEMBER_data[i] = data[i];
		}
	}
	std::string field_stream::raw_string()const
	{
		std::string result;
		result.resize(this->MEMBER_length);
		for (std::size_t i = 0; i < this->MEMBER_length; i++)
		{
			result[i] = value_cast<char>(this->MEMBER_data[i]);
		}
		return result;
	}
	std::string field_stream::field_string()const
	{
		std::string result;
		result.resize(this->MEMBER_length + sizeof(std::size_t));
		for (std::size_t i = 0; i < sizeof(std::size_t); i++)
		{
			result[i] = (reinterpret_cast<char const *>(&(this->MEMBER_length)))[i];
		}
		for (std::size_t i = 0; i < this->MEMBER_length; i++)
		{
			result[i + sizeof(std::size_t)] = value_cast<char>(this->MEMBER_data[i]);
		}
		return result;
	}
	std::vector<std::byte> field_stream::raw_data()const
	{
		std::vector<std::byte> result;
		result.resize(this->MEMBER_length);
		for (std::size_t i = 0; i < this->MEMBER_length; i++)
		{
			result[i] = this->MEMBER_data[i];
		}
		return result;
	}
	std::vector<std::byte> field_stream::field_data()const
	{
		std::vector<std::byte> result;
		result.resize(this->MEMBER_length + sizeof(std::size_t));
		for (std::size_t i = 0; i < sizeof(std::size_t); i++)
		{
			result[i] = (reinterpret_cast<std::byte const *>(&(this->MEMBER_length)))[i];
		}
		for (std::size_t i = 0; i < this->MEMBER_length; i++)
		{
			result[i + sizeof(std::size_t)] = this->MEMBER_data[i];
		}
		return result;
	}
	std::vector<field_stream> field_stream::parse(std::string const & data)
	{
		std::vector<field_stream> result;
		std::size_t position = 0;
		while (true)
		{
			if (position == data.size())
			{
				break;
			}
			if (data.size() < position + sizeof(std::size_t))
			{
				std::locale::global(std::locale::classic());
				throw std::invalid_argument("输入的流格式错误，无法解析为字段流");
			}
			std::size_t length;
			for (std::size_t i = 0; i < sizeof(std::size_t); i++)
			{
				(reinterpret_cast<char *>(&length))[i] = data[position];
				position++;
			}
			if (position + length > data.size())
			{
				std::locale::global(std::locale::classic());
				throw std::invalid_argument("输入的流格式错误，无法解析为字段流");
			}
			std::string buffer;
			buffer.resize(length);
			for (std::size_t i = 0; i < length; i++)
			{
				buffer[i] = data[position];
				position++;
			}
			result.push_back(buffer);
		}
		return result;
	}
	field_stream::field_stream()
		noexcept(std::is_nothrow_default_constructible_v<std::unique_ptr<std::byte[]>>)
		:MEMBER_length(0)
		, MEMBER_data()
	{
	}
	std::vector<field_stream> field_stream::parse(std::vector<std::byte> const & data)
	{
		std::vector<field_stream> result;
		std::size_t position = 0;
		while (true)
		{
			if (position == data.size())
			{
				break;
			}
			if (data.size() < position + sizeof(std::size_t))
			{
				std::locale::global(std::locale::classic());
				throw std::invalid_argument("输入的流格式错误，无法解析为字段流");
			}
			std::size_t length;
			for (std::size_t i = 0; i < sizeof(std::size_t); i++)
			{
				(reinterpret_cast<std::byte *>(&length))[i] = data[position];
				position++;
			}
			if (position + length > data.size())
			{
				std::locale::global(std::locale::classic());
				throw std::invalid_argument("输入的流格式错误，无法解析为字段流");
			}
			std::vector<std::byte> buffer;
			buffer.resize(length);
			for (std::size_t i = 0; i < length; i++)
			{
				buffer[i] = data[position];
				position++;
			}
			result.push_back(buffer);
		}
		return result;
	}
	field_stream::field_stream(field_stream const & origin)
		:MEMBER_data(std::make_unique<std::byte[]>(origin.MEMBER_length))
		, MEMBER_length(origin.MEMBER_length)
	{
		for (std::size_t i = 0; i < this->MEMBER_length; i++)
		{
			this->MEMBER_data[i] = origin.MEMBER_data[i];
		}
	}
	void field_stream::operator =(field_stream const & origin)
	{
		if (this != (&origin))
		{
			this->MEMBER_data = std::make_unique<std::byte[]>(origin.MEMBER_length);
			this->MEMBER_length = origin.MEMBER_length;
			for (std::size_t i = 0; i < this->MEMBER_length; i++)
			{
				this->MEMBER_data[i] = origin.MEMBER_data[i];
			}
		}
	}

	void field_stream::operator =(field_stream && origin)
		noexcept(std::is_nothrow_move_assignable_v<std::unique_ptr<std::byte[]>>)
	{
		if (this != (&origin))
		{
			this->MEMBER_data = std::move(origin.MEMBER_data);
			this->MEMBER_length = origin.MEMBER_length;
			origin.MEMBER_length = 0;
		}
	}
	field_stream::field_stream(field_stream && origin)
		noexcept(std::is_nothrow_move_constructible_v<std::unique_ptr<std::byte[]>>)
		:MEMBER_data(std::move(origin.MEMBER_data))
		, MEMBER_length(origin.MEMBER_length)
	{
		origin.MEMBER_length = 0;
	}



	byte_memory::marker_type::marker_type(byte_memory * owner)noexcept
		:MEMBER_owner(owner)
	{
	}

	byte_memory::marker_type::~marker_type()noexcept
	{
	}
	byte_memory & byte_memory::marker_type::owner()noexcept
	{
		return *(this->MEMBER_owner);
	}
	byte_memory const & byte_memory::marker_type::owner()const noexcept
	{
		return *(this->MEMBER_owner);
	}

	byte_memory::reference::reference(std::shared_ptr<marker_type> const & marker, std::streampos const & position)
		noexcept(std::is_nothrow_copy_constructible_v<std::streampos>
		and noexcept(std::weak_ptr<marker_type>(std::declval<std::shared_ptr<marker_type> const &>())))
		:MEMBER_marker(marker)
		, MEMBER_position(position)
	{
	}

	bool byte_memory::reference::is_valid()const
		noexcept(noexcept(std::declval<std::weak_ptr<marker_type>>().expired()))
	{
		return (not this->MEMBER_marker.expired());
	}
	void byte_memory::reference::raise_invalid_error()const noexcept(false)
	{
		if (not this->is_valid())
		{
			std::locale::global(std::locale::classic());
			throw std::logic_error("此引用已失效");
		}
	}
	byte_memory::reference::operator std::byte()const
	{
		this->raise_invalid_error();
		auto & data = this->MEMBER_marker.lock()->owner();
		data.MEMBER_storage.seekg(this->MEMBER_position);
		std::byte result;
		if (not data.MEMBER_storage.read(reinterpret_cast<char *>(&result), 1))
		{
			data.MEMBER_storage.clear();
			std::locale::global(std::locale::classic());
			std::string const hint = std::string("读取文件 ") + data.MEMBER_path.string() + " 的位置 " + std::to_string(this->MEMBER_position) + " 处失败";
			throw std::runtime_error(hint);
		}
		return result;
	}
	void byte_memory::reference::operator = (std::byte const value)
	{
		this->raise_invalid_error();
		auto & data = this->MEMBER_marker.lock()->owner();
		data.MEMBER_storage.seekp(this->MEMBER_position);
		if (not data.MEMBER_storage.write(reinterpret_cast<char const *>(&value), 1))
		{
			data.MEMBER_storage.clear();
			std::locale::global(std::locale::classic());
			std::string const hint = std::string("写入文件 ") + data.MEMBER_path.string() + " 的位置 " + std::to_string(this->MEMBER_position) + " 处失败";
			throw std::runtime_error(hint);
		}

	}

	byte_memory::reference::~reference()
		noexcept(std::is_nothrow_destructible_v<std::streampos>
		and std::is_nothrow_destructible_v<std::weak_ptr<marker_type>>)
	{
	}
	byte_memory::reference::reference(reference const & origin)
		noexcept(std::is_nothrow_copy_constructible_v<std::streampos>
		and std::is_nothrow_copy_constructible_v<std::weak_ptr<marker_type>>)
		:MEMBER_marker(origin.MEMBER_marker)
		, MEMBER_position(origin.MEMBER_position)
	{
	}
	byte_memory::reference::reference(reference && origin)
		noexcept(std::is_nothrow_copy_constructible_v<std::streampos>
		and std::is_nothrow_default_constructible_v<std::streampos>
		and std::is_nothrow_move_constructible_v<std::weak_ptr<marker_type>>)
		:MEMBER_marker(std::move(origin.MEMBER_marker))
		, MEMBER_position(origin.MEMBER_position)
	{
		origin.MEMBER_position = std::streampos();
	}
	void byte_memory::reference::operator =(reference const & origin)
		noexcept(std::is_nothrow_copy_assignable_v<std::streampos>
		and std::is_nothrow_copy_assignable_v<std::weak_ptr<marker_type>>)
	{
		if (this != std::addressof(origin))
		{
			this->MEMBER_marker = origin.MEMBER_marker;
			this->MEMBER_position = origin.MEMBER_position;
		}
	}
	void byte_memory::reference::operator =(reference && origin)
		noexcept(std::is_nothrow_copy_assignable_v<std::streampos>
		and std::is_nothrow_default_constructible_v<std::streampos>
		and std::is_nothrow_move_assignable_v<std::weak_ptr<marker_type>>)
	{
		if (this != std::addressof(origin))
		{
			this->MEMBER_marker = std::move(origin.MEMBER_marker);
			this->MEMBER_position = origin.MEMBER_position;
			origin.MEMBER_position = std::streampos();
		}
	}
	bool byte_memory::reference::operator <(reference const & right)const noexcept
	{
		return this->MEMBER_position < right.MEMBER_position;
	}
	byte_memory::reference byte_memory::operator [](std::streampos const & position)
	{
		return byte_memory::reference(this->MEMBER_marker, position);
	}
	std::byte byte_memory::operator [](std::streampos const & position)const
	{
		this->MEMBER_storage.seekg(position);
		std::byte result;
		if (not this->MEMBER_storage.read(reinterpret_cast<char *>(&result), 1))
		{
			this->MEMBER_storage.clear();
			std::locale::global(std::locale::classic());
			std::string const hint = std::string("读取文件 ") + this->MEMBER_path.string() + " 的位置 " + std::to_string(position) + " 处失败";
			throw std::runtime_error(hint);
		}
		return result;
	}
	byte_memory::byte_memory(std::filesystem::path const & file_path)
		:MEMBER_path(file_path)
		, MEMBER_marker()
		, MEMBER_storage()
	{
		if (std::filesystem::exists(file_path))
		{
			std::locale::global(std::locale::classic());
			std::string const hint = std::string("路径 ") + file_path.string() + " 已存在";
			throw std::invalid_argument(hint);
		}
		this->MEMBER_storage.open(file_path, std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
		if (not this->MEMBER_storage.is_open())
		{
			std::locale::global(std::locale::classic());
			std::string const hint = std::string("文件 ") + file_path.string() + " 无法打开";
			throw std::invalid_argument(hint);
		}
		this->MEMBER_marker = std::make_shared<marker_type>(this);
	}

	byte_memory::byte_memory(byte_memory && origin)
		noexcept(std::is_nothrow_move_constructible_v<std::fstream>
		and std::is_nothrow_move_constructible_v<std::filesystem::path>
		and std::is_nothrow_move_constructible_v<std::shared_ptr<marker_type>>)
		:MEMBER_marker(std::move(origin.MEMBER_marker))
		, MEMBER_path(std::move(origin.MEMBER_path))
		, MEMBER_storage(std::move(origin.MEMBER_storage))
	{

	}
	byte_memory::~byte_memory()
		noexcept(noexcept(std::declval<std::fstream>().close())
			and noexcept(std::filesystem::remove(std::declval<std::filesystem::path>()))
		and std::is_nothrow_destructible_v<std::filesystem::path>
		and std::is_nothrow_destructible_v<std::fstream>
		and std::is_nothrow_destructible_v<std::shared_ptr<marker_type>>)
	{
		this->MEMBER_storage.close();
		std::filesystem::remove(this->MEMBER_path);
	}
	std::streampos & byte_memory::reference::operator &()
	{
		this->raise_invalid_error();
		return this->MEMBER_position;
	}
	std::streampos const & byte_memory::reference::operator &()const
	{
		this->raise_invalid_error();
		return this->MEMBER_position;
	}
	void byte_memory::operator =(byte_memory && origin)
		noexcept(std::is_nothrow_move_assignable_v<std::fstream>
		and std::is_nothrow_move_assignable_v<std::filesystem::path>
		and std::is_nothrow_move_assignable_v<std::shared_ptr<marker_type>>)
	{
		if (this != (&origin))
		{
			this->MEMBER_marker = std::move(origin.MEMBER_marker);
			this->MEMBER_path = std::move(origin.MEMBER_path);
			this->MEMBER_storage = std::move(origin.MEMBER_storage);
		}
	}

	field_stream byte_memory::read(std::streampos const & position)const
	{
		std::size_t length;
		this->MEMBER_storage.seekg(position);
		if (not this->MEMBER_storage.read(reinterpret_cast<char *>(&length), sizeof(std::size_t)))
		{
			this->MEMBER_storage.clear();
			std::locale::global(std::locale::classic());
			std::string const hint = std::string("读取文件 ") + this->MEMBER_path.string() + " 的位置 " + std::to_string(position) + " 处失败";
			throw std::runtime_error(hint);
		}
		auto buffer = std::make_unique<char[]>(length);
		if (not this->MEMBER_storage.read(buffer.get(), length))
		{
			this->MEMBER_storage.clear();
			std::locale::global(std::locale::classic());
			std::string const hint = std::string("读取文件 ") + this->MEMBER_path.string() + " 的位置 " + std::to_string(position) + " 处失败";
			throw std::runtime_error(hint);
		}
		std::string result;
		result.resize(length);
		for (std::size_t i = 0;i < result.size();++i)
		{
			result[i] = buffer[i];
		}
		return result;
	}

	field_stream byte_memory::read(std::streampos const & position, std::streamoff const length)const
	{
		this->MEMBER_storage.seekg(position);
		auto buffer = std::make_unique<char[]>(length);
		if (not this->MEMBER_storage.read(buffer.get(), length))
		{
			this->MEMBER_storage.clear();
			std::locale::global(std::locale::classic());
			std::string const hint = std::string("读取文件 ") + this->MEMBER_path.string() + " 的位置 " + std::to_string(position) + " 处失败";
			throw std::runtime_error(hint);
		}
		std::string result;
		result.resize(length);
		for (std::size_t i = 0;i < result.size();++i)
		{
			result[i] = buffer[i];
		}
		return result;
	}

	void byte_memory::write(field_stream const & data, bool const raw, std::streampos const & position)
	{
		std::string buffer;
		if (raw)
		{
			buffer = data.raw_string();
		}
		else
		{
			buffer = data.field_string();
		}
		this->MEMBER_storage.seekp(position);

		if (not this->MEMBER_storage.write(buffer.c_str(), buffer.size()))
		{
			this->MEMBER_storage.clear();
			std::locale::global(std::locale::classic());
			std::string const hint = std::string("写入文件 ") + this->MEMBER_path.string() + " 的位置 " + std::to_string(position) + " 处失败";
			throw std::runtime_error(hint);
		}
	}
}