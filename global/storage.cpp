#include <fstream>
#include <thread>
#include <atomic>

#include ".\bits.hpp"
#include ".\storage.hpp"


namespace OKps
{
	bool compare(std::filesystem::path const & left, std::filesystem::path const & right, std::uintmax_t const threads)
	{
		namespace fs = std::filesystem;

		if (not left.has_filename())
		{
			std::string const hint = "路径 " + left.string() + " 不是文件";
			throw std::invalid_argument(hint);
		}
		if (not right.has_filename())
		{
			std::string const hint = "路径 " + right.string() + " 不是文件";
			throw std::invalid_argument(hint);
		}
		if (not fs::exists(left))
		{
			std::string const hint = "路径 " + left.string() + " 不存在";
			throw std::invalid_argument(hint);
		}
		if (not fs::exists(right))
		{
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
				std::atomic_bool & MEMBER_result;
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
				compare_worker(std::atomic_bool & result, std::uintmax_t const buffer_length, std::uintmax_t const begin, fs::path const & left, fs::path const & right)noexcept
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
						std::string const hint = std::string("文件 ")
							+ left.string()
							+ " 打开失败";
						throw std::runtime_error(hint);
					}
					if (not this->MEMBER_right.is_open())
					{
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
			auto temp_result = std::atomic_bool(true);
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
			result[i] = value_cast(this->MEMBER_data[i]);
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
			result[i + sizeof(std::size_t)] = value_cast(this->MEMBER_data[i]);
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

}