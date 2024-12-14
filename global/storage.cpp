
#include <fstream>
#include <thread>
#include <atomic>

#include ".\value_cast.hpp"

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
	field::field(std::string const & data)
		noexcept(std::is_nothrow_copy_constructible_v<std::string>)
		:MEMBER_raw_data(data)
	{
	}
	field::field(std::string && data)
		noexcept(std::is_nothrow_move_constructible_v<std::string>)
		:MEMBER_raw_data(std::move(data))
	{
	}
	field::~field()
		noexcept(std::is_nothrow_destructible_v<std::string>)
	{
	}
	field::field(std::vector<std::byte> const & data)
		noexcept(std::is_nothrow_default_constructible_v<std::string>
		and noexcept(std::declval<std::string>().resize(std::declval<std::vector<std::byte> const &>().size())))
		:MEMBER_raw_data()
	{
		this->MEMBER_raw_data.resize(data.size());
		for (std::size_t i = 0;i < this->MEMBER_raw_data.size();++i)
		{
			this->MEMBER_raw_data[i] = value_cast<char>(data[i]);
		}
	}

	std::string const & field::raw_string()const noexcept
	{
		return this->MEMBER_raw_data;
	}
	std::string & field::raw_string()noexcept
	{
		return this->MEMBER_raw_data;
	}
	std::string field::field_string()const
		noexcept(std::is_nothrow_default_constructible_v<std::string>
and noexcept(std::declval<std::string>().resize(std::declval<std::string>().size())))
	{
		std::string result;
		result.resize(this->MEMBER_raw_data.size() + sizeof(std::size_t));
		std::size_t const temp_length = this->MEMBER_raw_data.size();
		for (std::size_t i = 0; i < sizeof(std::size_t); ++i)
		{
			result[i] = (reinterpret_cast<char const *>(&temp_length))[i];
		}
		for (std::size_t i = 0; i < this->MEMBER_raw_data.size(); ++i)
		{
			result[i + sizeof(std::size_t)] = this->MEMBER_raw_data[i];
		}
		return result;
	}
	std::vector<std::byte> field::raw_data()const
		noexcept(noexcept(value_cast<std::vector<std::byte>>(std::declval<std::string const>())))
	{
		return value_cast<std::vector<std::byte>>(this->MEMBER_raw_data);
	}

	std::vector<std::byte> field::field_data()const
		noexcept(std::is_nothrow_default_constructible_v<std::vector<std::byte>>
and noexcept(std::declval<std::vector<std::byte>>().resize(std::declval<std::string>().size())))
	{
		std::vector<std::byte> result;
		result.resize(this->MEMBER_raw_data.size() + sizeof(std::size_t));
		std::size_t const temp_length = this->MEMBER_raw_data.size();
		for (std::size_t i = 0; i < sizeof(std::size_t); ++i)
		{
			result[i] = (reinterpret_cast<std::byte const *>(&temp_length))[i];
		}
		for (std::size_t i = 0; i < this->MEMBER_raw_data.size(); ++i)
		{
			result[i + sizeof(std::size_t)] = value_cast<std::byte>(this->MEMBER_raw_data[i]);
		}
		return result;
	}
	std::vector<field> field::parse(std::string const & data)
	{
		std::vector<field> result;
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
	field::field()
		noexcept(std::is_nothrow_default_constructible_v<std::string>)
		: MEMBER_raw_data()
	{
	}
	std::vector<field> field::parse(std::vector<std::byte> const & data)
	{
		std::vector<field> result;
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
			result.push_back(field(std::move(buffer)));
		}
		return result;
	}
	field::field(field const & origin)
		noexcept(std::is_nothrow_copy_constructible_v<std::string>)
		:MEMBER_raw_data(origin.MEMBER_raw_data)
	{
	}
	void field::operator =(field const & origin)
		noexcept(std::is_nothrow_copy_assignable_v<std::string>)
	{
		if (this != (&origin))
		{
			this->MEMBER_raw_data = origin.MEMBER_raw_data;
		}
	}

	void field::operator =(field && origin)
		noexcept(std::is_nothrow_move_assignable_v<std::string>)
	{
		if (this != (&origin))
		{
			this->MEMBER_raw_data = std::move(origin.MEMBER_raw_data);
		}
	}
	field::field(field && origin)
		noexcept(std::is_nothrow_move_constructible_v<std::string>)
		:MEMBER_raw_data(std::move(origin.MEMBER_raw_data))
	{
	}

	stream_proxy<std::fstream>::stream_proxy(std::fstream & f, std::filesystem::path const & p)noexcept
		:MEMBER_path(std::addressof(p))
		, MEMBER_storage(std::addressof(f))
	{
	}
	stream_proxy<std::fstream>::~stream_proxy()noexcept
	{
	}
	stream_proxy<std::fstream>::stream_proxy(stream_proxy const & origin)noexcept
		:MEMBER_path(origin.MEMBER_path)
		, MEMBER_storage(origin.MEMBER_storage)
	{
	}

	void stream_proxy<std::fstream>::operator =(stream_proxy const & origin) noexcept
	{
		if (this != std::addressof(origin))
		{
			this->MEMBER_path = origin.MEMBER_path;
			this->MEMBER_storage = origin.MEMBER_storage;
		}
	}

	void stream_proxy<std::fstream>::read(char * const buffer, std::streamsize const length)
	{
		if (not this->MEMBER_storage->read(buffer, length))
		{
			this->MEMBER_storage->clear();
			this->MEMBER_storage->seekg(0, std::ios::beg);
			std::string const hint = "读取文件" + this->MEMBER_path->string() + "失败";
			throw std::runtime_error(hint);
		}
	}
	void stream_proxy<std::fstream>::write(char const * const  buffer, std::streamsize const length)
	{
		if (not this->MEMBER_storage->write(buffer, length))
		{
			this->MEMBER_storage->clear();
			this->MEMBER_storage->seekp(0, std::ios::beg);
			std::string const hint = "写入文件" + this->MEMBER_path->string() + "失败";
			throw std::runtime_error(hint);
		}
	}
	void stream_proxy<std::fstream>::seekp(std::streampos const & position)
	{
		if (not this->MEMBER_storage->seekp(position))
		{
			this->MEMBER_storage->clear();
			this->MEMBER_storage->seekp(0, std::ios::beg);
			std::string const hint = "文件 " + this->MEMBER_path->string() + " 的位置" + std::to_string(position) + "无效";
			throw std::runtime_error(hint);
		}
	}
	void stream_proxy<std::fstream>::seekg(std::streampos const & position)
	{
		if (not this->MEMBER_storage->seekg(position))
		{
			this->MEMBER_storage->clear();
			this->MEMBER_storage->seekg(0, std::ios::beg);
			std::string const hint = "文件 " + this->MEMBER_path->string() + " 的位置" + std::to_string(position) + "无效";
			throw std::runtime_error(hint);
		}
	}
	void stream_proxy<std::fstream>::seekp(std::streamoff const offset, std::ios_base::seekdir const & way)
	{
		if (not this->MEMBER_storage->seekp(offset, way))
		{
			this->MEMBER_storage->clear();
			this->MEMBER_storage->seekp(0, std::ios::beg);
			std::string const hint = "文件 " + this->MEMBER_path->string() + " 寻位失败 ";
			throw std::runtime_error(hint);
		}
	}
	void stream_proxy<std::fstream>::seekg(std::streamoff const offset, std::ios_base::seekdir const & way)
	{
		if (not this->MEMBER_storage->seekg(offset, way))
		{
			this->MEMBER_storage->clear();
			this->MEMBER_storage->seekg(0, std::ios::beg);
			std::string const hint = "文件 " + this->MEMBER_path->string() + " 寻位失败 ";
			throw std::runtime_error(hint);
		}
	}

	void storage<field>::INNER_insert_data(std::streampos const & pos, std::string const & data)
	{
		stream_proxy<std::fstream> TEMP_storage(this->MEMBER_storage, this->MEMBER_path);
		TEMP_storage.seekg(0, std::ios::end);
		auto const TEMP_pos = this->MEMBER_storage.tellg();
		std::streampos const TEMP_begin = (stream_position(data.size()) + pos).value();
		this->INNER_move_data(TEMP_begin, pos, TEMP_pos);
		TEMP_storage.seekp(pos);
		TEMP_storage.write(data.c_str(), static_cast<std::streamsize>(stream_position(data.size())));
	}

	template<>
	std::string field::connect(std::vector<field> const & data)
	{
		std::string result;
		for (std::size_t i = 0;i < data.size();++i)
		{
			result += data[i].field_string();
		}
		return result;
	}
	template<>
	std::vector<std::byte> field::connect(std::vector<field> const & data)
	{
		std::vector<std::byte> result;
		for (std::size_t i = 0;i < data.size();++i)
		{
			auto const TEMP_data = data[i].field_data();
			auto const TEMP_before_size = result.size();
			result.resize(TEMP_before_size + TEMP_data.size());
			for (std::size_t j = 0;j < TEMP_data.size();++j)
			{
				result[TEMP_before_size + j] = TEMP_data[j];
			}
		}
		return result;
	}
	stream_position::stream_position(std::streampos const & position)
		noexcept(std::is_nothrow_copy_constructible_v<std::streampos>)
		:MEMBER_position(position)
	{
	}
	stream_position const storage<field>::MEMBER_block_size = stream_position(static_cast<std::size_t>(64) * 1024);
	void storage<field>::INNER_move_data(std::streampos const & another_begin, std::streampos const & begin, std::streampos const & end)
	{
		if (end <= begin)
		{
			return;
		}
		if (another_begin == begin)
		{
			return;
		}
		stream_proxy<std::fstream> TEMP_storage(this->MEMBER_storage, this->MEMBER_path);
		if (another_begin < begin)
		{
			auto buffer = std::make_unique<char[]>(static_cast<std::size_t>(storage<field>::MEMBER_block_size));
			std::streampos TEMP_left = begin;
			std::streampos TEMP_another_left = another_begin;
			while (TEMP_left <= end - storage<field>::MEMBER_block_size.value())
			{
				TEMP_storage.seekg(TEMP_left);
				TEMP_storage.read(buffer.get(), static_cast<std::streamsize>(storage<field>::MEMBER_block_size));
				TEMP_storage.seekp(TEMP_another_left);
				TEMP_storage.write(buffer.get(), static_cast<std::streamsize>(storage<field>::MEMBER_block_size));
				TEMP_left += storage<field>::MEMBER_block_size.value();
				TEMP_another_left += storage<field>::MEMBER_block_size.value();
			}
			if (TEMP_left < end)
			{
				TEMP_storage.seekg(TEMP_left);
				TEMP_storage.read(buffer.get(), static_cast<std::streamsize>(stream_position(end) - TEMP_left));
				TEMP_storage.seekp(TEMP_another_left);
				TEMP_storage.write(buffer.get(), static_cast<std::streamsize>(stream_position(end) - TEMP_left));
			}
		}
		else
		{
			auto buffer = std::make_unique<char[]>(static_cast<std::size_t>(storage<field>::MEMBER_block_size));
			std::streampos TEMP_right = end;
			std::streampos TEMP_another_right = stream_position(another_begin) + (stream_position(end) - begin);
			while (TEMP_right - storage<field>::MEMBER_block_size.value() >= begin)
			{
				TEMP_storage.seekg(TEMP_right - MEMBER_block_size.value());
				TEMP_storage.read(buffer.get(), static_cast<std::streamsize>(storage<field>::MEMBER_block_size));
				TEMP_storage.seekp(TEMP_another_right - storage<field>::MEMBER_block_size.value());
				TEMP_storage.write(buffer.get(), static_cast<std::streamsize>(storage<field>::MEMBER_block_size));
				TEMP_right -= storage<field>::MEMBER_block_size.value();
				TEMP_another_right -= storage<field>::MEMBER_block_size.value();
			}
			if (TEMP_right > begin)
			{
				TEMP_storage.seekg(begin);
				TEMP_storage.read(buffer.get(), static_cast<std::streamsize>(stream_position(TEMP_right) - begin));
				TEMP_storage.seekp(another_begin);
				TEMP_storage.write(buffer.get(), static_cast<std::streamsize>(stream_position(TEMP_right) - begin));
			}
		}
	}
	std::streampos const & stream_position::value()const noexcept
	{
		return this->MEMBER_position;
	}
	stream_position::operator std::streampos const & ()const noexcept
	{
		return this->MEMBER_position;
	}
	void storage<field>::INNER_resize_storage(std::uintmax_t const size)
	{
		this->MEMBER_storage.close();
		std::filesystem::resize_file(this->MEMBER_path, size);
		this->MEMBER_storage.open(this->MEMBER_path, std::ios::in | std::ios::out | std::ios::binary);
		if (not this->MEMBER_storage.is_open())
		{
			std::string const hint = "打开文件 " + this->MEMBER_path.string() + " 失败";
			throw std::runtime_error(hint);
		}
	}
	void storage<field>::INNER_erase_data(std::streampos const & begin, std::streampos const & end)
	{
		if (begin >= end)
		{
			return;
		}
		stream_proxy<std::fstream> TEMP_storage(this->MEMBER_storage, this->MEMBER_path);
		TEMP_storage.seekg(0, std::ios::end);
		auto const TEMP_end = this->MEMBER_storage.tellg();
		this->INNER_move_data(begin, end, TEMP_end);
		this->INNER_resize_storage(static_cast<std::uintmax_t>(stream_position(TEMP_end) - (stream_position(end) - begin)));
	}

	storage<field>::reference::reference(storage const & marker, std::size_t const position)
		noexcept(noexcept(base_type(std::declval<storage<field>::base_type const &>())))
		:base_type(marker)
		, MEMBER_position(position)
	{
	}

	//读取数据
	storage<field>::reference::operator field()const
	{
		return dynamic_cast<storage<field> const &>(this->get()).read(this->MEMBER_position);
	}
	//写入数据
	void storage<field>::reference::operator =(field const & data)
	{
		dynamic_cast<storage<field> &>(this->get()).write(data, this->MEMBER_position);
	}

	storage<field>::reference::~reference()
		noexcept(std::is_nothrow_destructible_v<base_type>)
	{
	}
	storage<field>::reference::reference(reference const & origin)
		noexcept(std::is_nothrow_copy_constructible_v<base_type>)
		:base_type(origin)
		, MEMBER_position(origin.MEMBER_position)
	{
	}
	storage<field>::reference::reference(reference && origin)
		noexcept(std::is_nothrow_move_constructible_v<base_type>)
		:base_type(std::move(origin))
		, MEMBER_position(origin.MEMBER_position)
	{
	}
	void storage<field>::reference::operator =(reference const & origin)
		noexcept(std::is_nothrow_copy_assignable_v<base_type>)
	{
		if (this != std::addressof(origin))
		{
			this->base_type::self() = origin;
			this->MEMBER_position = origin.MEMBER_position;
		}
	}
	void storage<field>::reference::operator =(reference && origin)
		noexcept(std::is_nothrow_move_assignable_v<base_type>)
	{
		if (this != std::addressof(origin))
		{
			this->base_type::self() = std::move(origin);
			this->MEMBER_position = origin.MEMBER_position;
		}
	}

	bool storage<field>::reference::operator <(reference const & right)const noexcept
	{
		return this->MEMBER_position < right.MEMBER_position;
	}


	storage<field>::reference storage<field>::operator [](std::size_t const position)
		noexcept(noexcept(reference(std::declval<storage &>(), std::declval<std::size_t const>())))
	{
		return reference(*this, position);
	}
	field storage<field>::operator [](std::size_t const position)const
	{
		return this->read(position);
	}

	storage<field>::storage(std::filesystem::path const & file_path)
		:base_type()
		, MEMBER_path()
		, MEMBER_storage()
	{
		if (not std::filesystem::exists(file_path))
		{
			this->MEMBER_temporary_file = true;
			std::ofstream TEMP_file_creator(file_path, std::ios::out | std::ios::binary | std::ios::trunc);
			if (not TEMP_file_creator.is_open())
			{
				std::string const hint = "创建文件 " + file_path.string() + " 失败";
				throw std::runtime_error(hint);
			}
		}
		else
		{
			this->MEMBER_temporary_file = false;
		}

		this->MEMBER_storage = std::fstream(file_path, std::ios::in | std::ios::out | std::ios::binary);
		if (not this->MEMBER_storage.is_open())
		{
			std::string const hint = "打开文件 " + file_path.string() + " 失败";
			throw std::runtime_error(hint);
		}
		this->MEMBER_path = file_path;
		stream_proxy<std::fstream> TEMP_storage(this->MEMBER_storage, this->MEMBER_path);

		TEMP_storage.seekg(0, std::ios::beg);
		std::streampos position = this->MEMBER_storage.tellg();
		TEMP_storage.seekg(0, std::ios::end);
		std::streampos const file_end = this->MEMBER_storage.tellg();
		while (true)
		{
			if (position == file_end)
			{
				break;
			}
			if (stream_position(file_end) < stream_position(position) + stream_position(sizeof(std::size_t)))
			{
				throw std::invalid_argument("文件格式错误，无法解析为字段流");
			}
			std::size_t length;
			TEMP_storage.read(reinterpret_cast<char *>(&length), stream_position(sizeof(std::size_t)));
			position = (stream_position(position) + stream_position(sizeof(std::size_t))).value();
			if (stream_position(position) + stream_position(length) > stream_position(file_end))
			{
				throw std::invalid_argument("文件格式错误，无法解析为字段流");
			}
			field_info const TEMP_pos_info(position, stream_position(length));
			this->MEMBER_position_list.push_back(TEMP_pos_info);
			position = (stream_position(position) + stream_position(length)).value();
		}
	}

	storage<field>::storage(storage && origin)
		noexcept(std::is_nothrow_move_constructible_v<std::fstream>
		and std::is_nothrow_move_constructible_v<std::filesystem::path>
		and std::is_nothrow_move_constructible_v<base_type>
		and std::is_nothrow_move_constructible_v<std::vector<field_info>>)
		:base_type(std::move(origin))
		, MEMBER_path(std::move(origin.MEMBER_path))
		, MEMBER_position_list(std::move(origin.MEMBER_position_list))
		, MEMBER_storage(std::move(origin.MEMBER_storage))
	{
	}
	void storage<field>::operator =(storage && origin)
		noexcept(std::is_nothrow_move_assignable_v<std::fstream>
		and std::is_nothrow_move_assignable_v<std::filesystem::path>
		and std::is_nothrow_move_assignable_v<base_type>
		and std::is_nothrow_move_assignable_v<std::vector<field_info>>)
	{
		if (this != std::addressof(origin))
		{
			this->base_type::self() = std::move(origin);
			this->MEMBER_path = std::move(origin.MEMBER_path);
			this->MEMBER_position_list = std::move(origin.MEMBER_position_list);
			this->MEMBER_storage = std::move(origin.MEMBER_storage);
		}
	}
	storage<field>::~storage()
		noexcept(std::is_nothrow_destructible_v<std::filesystem::path>
		and std::is_nothrow_destructible_v<std::fstream>
		and std::is_nothrow_destructible_v<base_type>
		and std::is_nothrow_destructible_v<std::vector<field_info>>
		and noexcept(std::declval<std::fstream &>().close())
		and noexcept(std::filesystem::remove(std::declval<std::filesystem::path &>())))
	{
		if (this->MEMBER_temporary_file)
		{
			this->MEMBER_storage.close();
			std::filesystem::remove(this->MEMBER_path);
		}
	}
	storage<field>::reference & storage<field>::reference::self()noexcept
	{
		return *this;
	}
	storage<field>::reference const & storage<field>::reference::self()const noexcept
	{
		return *this;
	}

	storage<field>::reference * storage<field>::reference::operator &()noexcept
	{
		return this;
	}
	storage<field>::reference const * storage<field>::reference::operator &()const noexcept
	{
		return this;
	}
	storage<field> & storage<field>::self()noexcept
	{
		return *this;
	}
	storage<field> const & storage<field>::self()const noexcept
	{
		return *this;
	}

	storage<field> * storage<field>::operator &()noexcept
	{
		return this;
	}
	storage<field>  const * storage<field>::operator &()const noexcept
	{
		return this;
	}
	storage<field>::field_info::field_info(std::streampos const & ARG_begin, std::streamoff const ARG_length)
		noexcept(std::is_nothrow_copy_constructible_v<std::streampos>)
		:MEMBER_begin(ARG_begin)
		, MEMBER_length(ARG_length)
	{
	}
	storage<field>::field_info::~field_info()
		noexcept(std::is_nothrow_destructible_v<std::streampos>)
	{
	}
	storage<field>::field_info::field_info(field_info && origin)
		noexcept(std::is_nothrow_copy_constructible_v<std::streampos>)
		:field_info(static_cast<field_info const &>(origin))
	{
	}

	void storage<field>::field_info::operator =(field_info && origin)
		noexcept(std::is_nothrow_copy_assignable_v<std::streampos>)
	{
		(*this) = static_cast<field_info const &>(origin);
	}
	storage<field>::field_info::field_info(field_info const & origin)
		noexcept(std::is_nothrow_copy_constructible_v<std::streampos>)
		:MEMBER_begin(origin.MEMBER_begin)
		, MEMBER_length(origin.MEMBER_length)
	{
	}
	void storage<field>::field_info::operator =(field_info const & origin)
		noexcept(std::is_nothrow_copy_assignable_v<std::streampos>)
	{
		this->MEMBER_begin = origin.MEMBER_begin;
		this->MEMBER_length = origin.MEMBER_length;
	}
	field storage<field>::read(std::size_t const position)const
	{
		if (position >= this->MEMBER_position_list.size())
		{
			std::string const hint = "字段位置" + std::to_string(position) + "不存在";
			throw std::out_of_range(hint);
		}
		stream_proxy<std::fstream> TEMP_reader(this->MEMBER_storage, this->MEMBER_path);
		TEMP_reader.seekg(this->MEMBER_position_list[position].begin());
		std::unique_ptr<char[]> buffer = std::make_unique<char[]>(static_cast<std::size_t>(stream_position(this->MEMBER_position_list[position].length())));
		TEMP_reader.read(buffer.get(), static_cast<std::streamsize>(stream_position(this->MEMBER_position_list[position].length())));
		std::string TEMP_data;
		TEMP_data.resize(static_cast<std::size_t>(stream_position(this->MEMBER_position_list[position].length())));
		for (std::size_t i = 0;i < TEMP_data.size();++i)
		{
			TEMP_data[i] = buffer[i];
		}
		return field(TEMP_data);
	}
	template<typename value_type>
	stream_position::stream_position(value_type const position)
		noexcept(safe_convertible<stream_position, value_type>)
		:MEMBER_position(static_cast<base::integer<std::streamoff>>(base::integer<value_type>(position)).value())
	{
	}

	template<typename value_type>
	stream_position::operator value_type()const
		noexcept(safe_convertible<value_type, stream_position>)
	{
		return static_cast<base::integer<value_type>>(base::integer<std::streamoff>(this->MEMBER_position)).value();
	}

	stream_position::stream_position(stream_position const & origin)
		noexcept(std::is_nothrow_copy_constructible_v<std::streampos>)
		:MEMBER_position(origin.MEMBER_position)
	{
	}

	void stream_position::operator =(stream_position const & origin)
		noexcept(std::is_nothrow_copy_assignable_v<std::streampos>)
	{
		if (this != std::addressof(origin))
		{
			this->MEMBER_position = origin.MEMBER_position;
		}
	}

	stream_position::~stream_position()
		noexcept(std::is_nothrow_destructible_v<std::streampos>)
	{
	}

	void storage<field>::write(field const & data, std::size_t const position)
	{
		if (position >= this->MEMBER_position_list.size())
		{
			this->resize((base::integer<std::size_t>(position) + 1).value());
			this->write(data, position);
		}
		else
		{
			std::size_t const before_field_length = stream_position(this->MEMBER_position_list[position].length());
			stream_proxy<std::fstream> TEMP_storage(this->MEMBER_storage, this->MEMBER_path);
			if (data.raw_string().size() > before_field_length)
			{
				stream_position const TEMP_length_diff = stream_position(data.raw_string().size() - before_field_length);
				std::string TEMP_buffer = data.field_string();
				TEMP_buffer.resize((base::integer<std::size_t>(before_field_length) + sizeof(std::size_t)).value());
				TEMP_storage.seekp(this->MEMBER_position_list[position].field_begin());
				TEMP_storage.write(TEMP_buffer.c_str(), stream_position(TEMP_buffer.size()));
				TEMP_buffer.resize(TEMP_length_diff);
				for (std::size_t i = 0;i < static_cast<std::size_t>(TEMP_length_diff);++i)
				{
					TEMP_buffer[i] = data.raw_string()[before_field_length + i];
				}
				this->INNER_insert_data(this->MEMBER_position_list[position].end(), TEMP_buffer);
				this->MEMBER_position_list[position].length() = stream_position(data.raw_string().size());
				for (std::size_t i = (base::integer<std::size_t>(position) + 1).value();i < this->MEMBER_position_list.size();++i)
				{
					this->MEMBER_position_list[i].begin() = (stream_position(this->MEMBER_position_list[i].begin()) + TEMP_length_diff).value();
				}
			}
			else if (data.raw_string().size() < before_field_length)
			{
				TEMP_storage.seekp(this->MEMBER_position_list[position].field_begin());
				std::string const TEMP_data = data.field_string();
				TEMP_storage.write(TEMP_data.c_str(), stream_position(TEMP_data.size()));
				this->MEMBER_position_list[position].length() = stream_position(TEMP_data.size());
				if (base::integer<std::size_t>(position) + 1 < this->MEMBER_position_list.size())
				{
					this->INNER_move_data(this->MEMBER_position_list[position].end()
					, this->MEMBER_position_list[(base::integer<std::size_t>(position) + 1).value()].field_begin()
					 , this->MEMBER_position_list[this->MEMBER_position_list.size() - 1].end());
				}
				stream_position const TEMP_length_diff = stream_position(before_field_length - data.raw_string().size());
				this->INNER_erase_data(stream_position(this->MEMBER_position_list[this->MEMBER_position_list.size() - 1].end()) - TEMP_length_diff
					, this->MEMBER_position_list[this->MEMBER_position_list.size() - 1].end());
				for (std::size_t i = (base::integer<std::size_t>(position) + 1).value();i < this->MEMBER_position_list.size();++i)
				{
					this->MEMBER_position_list[i].begin() = (stream_position(this->MEMBER_position_list[i].begin()) - TEMP_length_diff).value();
				}
			}
			else
			{
				TEMP_storage.seekp(this->MEMBER_position_list[position].begin());
				std::string const & TEMP_data = data.raw_string();
				TEMP_storage.write(TEMP_data.c_str(), stream_position(TEMP_data.size()));
			}
		}
	}
	std::streampos const & storage<field>::field_info::begin()const noexcept
	{
		return this->MEMBER_begin;
	}
	std::streamoff const & storage<field>::field_info::length()const noexcept
	{
		return this->MEMBER_length;
	}
	std::streampos & storage<field>::field_info::begin()noexcept
	{
		return this->MEMBER_begin;
	}
	std::streamoff & storage<field>::field_info::length()noexcept
	{
		return this->MEMBER_length;
	}
	std::streampos storage<field>::field_info::field_begin()const
		noexcept(noexcept(stream_position(std::declval<std::streampos const &>()) - stream_position(std::declval<std::size_t>())))
	{
		return stream_position(this->MEMBER_begin) - stream_position(sizeof(std::size_t));
	}
	std::streampos storage<field>::field_info::end()const
		noexcept(noexcept(stream_position(std::declval<std::streampos const &>()) + stream_position(std::declval<std::streamoff const &>())))
	{
		return stream_position(this->MEMBER_begin) + stream_position(this->MEMBER_length);
	}
	storage<field>::field_info::field_info()
		noexcept(noexcept(std::streampos(0)))
		:MEMBER_begin(0)
		, MEMBER_length(0)
	{
	}
	bool stream_position::operator ==(stream_position const & right)const
		noexcept(noexcept(std::declval<std::streampos const &>() == std::declval<std::streampos const &>()))
	{
		return this->MEMBER_position == right.MEMBER_position;
	}
	bool stream_position::operator !=(stream_position const & right)const
		noexcept(noexcept(std::declval<std::streampos const &>() != std::declval<std::streampos const &>()))
	{
		return this->MEMBER_position != right.MEMBER_position;
	}
	bool stream_position::operator <=(stream_position const & right)const
		noexcept(noexcept(std::declval<std::streampos const &>() <= std::declval<std::streampos const &>()))
	{
		return this->MEMBER_position <= right.MEMBER_position;
	}
	bool stream_position::operator >=(stream_position const & right)const
		noexcept(noexcept(std::declval<std::streampos const &>() >= std::declval<std::streampos const &>()))
	{
		return this->MEMBER_position >= right.MEMBER_position;
	}
	bool stream_position::operator <(stream_position const & right)const
		noexcept(noexcept(std::declval<std::streampos const &>() < std::declval<std::streampos const &>()))
	{
		return this->MEMBER_position < right.MEMBER_position;
	}
	bool stream_position::operator >(stream_position const & right)const
		noexcept(noexcept(std::declval<std::streampos const &>() > std::declval<std::streampos const &>()))
	{
		return this->MEMBER_position > right.MEMBER_position;
	}
	stream_position stream_position::operator +(stream_position const & right)const
		noexcept(noexcept(stream_position((base::integer<std::streamoff>(std::declval<std::streampos const &>()) + base::integer<std::streamoff>(std::declval<std::streampos const &>())).value())))
	{
		return stream_position((base::integer<std::streamoff>(this->MEMBER_position) + base::integer<std::streamoff>(right.MEMBER_position)).value());
	}
	stream_position stream_position::operator -(stream_position const & right)const
		noexcept(noexcept(stream_position((base::integer<std::streamoff>(std::declval<std::streampos const &>()) - base::integer<std::streamoff>(std::declval<std::streampos const &>())).value())))
	{
		return stream_position((base::integer<std::streamoff>(this->MEMBER_position) - base::integer<std::streamoff>(right.MEMBER_position)).value());
	}
	void stream_position::operator +=(stream_position const & right)
		noexcept(noexcept(std::declval<std::streampos &>() = (base::integer<std::streamoff>(std::declval<std::streampos &>()) + base::integer<std::streamoff>(std::declval<std::streampos const &>())).value()))
	{
		this->MEMBER_position = (base::integer<std::streamoff>(this->MEMBER_position) + base::integer<std::streamoff>(right.MEMBER_position)).value();
	}
	void stream_position::operator -=(stream_position const & right)
		noexcept(noexcept(std::declval<std::streampos &>() = (base::integer<std::streamoff>(std::declval<std::streampos &>()) - base::integer<std::streamoff>(std::declval<std::streampos const &>())).value()))
	{
		this->MEMBER_position = (base::integer<std::streamoff>(this->MEMBER_position) - base::integer<std::streamoff>(right.MEMBER_position)).value();
	}
	void storage<field>::resize(std::size_t const size)
	{
		if (size == this->MEMBER_position_list.size())
		{
			return;
		}
		else if (size < this->MEMBER_position_list.size())
		{
			if (size == 0)
			{
				this->INNER_resize_storage(0);
				this->MEMBER_position_list.resize(0);
			}
			else
			{
				this->INNER_resize_storage(static_cast<std::uintmax_t>(stream_position(this->MEMBER_position_list[size - 1].end())));
				this->MEMBER_position_list.resize(size);
			}
		}
		else
		{
			//要增加的字段数
			std::size_t const size_diff = size - this->MEMBER_position_list.size();
			//此前字段总数
			std::size_t const before_field_count = this->MEMBER_position_list.size();
			//此前文件的长度
			std::size_t before_file_size;
			stream_proxy<std::fstream> TEMP_storage(this->MEMBER_storage, this->MEMBER_path);
			TEMP_storage.seekg(0, std::ios::end);
			//此前文件的尾位置
			stream_position const before_file_end = this->MEMBER_storage.tellg();
			if (this->MEMBER_position_list.size() == 0)
			{
				before_file_size = 0;
			}
			else
			{
				before_file_size = static_cast<std::size_t>(stream_position(this->MEMBER_position_list[this->MEMBER_position_list.size() - 1].end()));
			}
			//新文件长度
			std::uintmax_t const TEMP_size = static_cast<base::integer<std::uintmax_t>>(base::integer<std::size_t>(size_diff) * sizeof(std::size_t) + before_file_size).value();
			this->INNER_resize_storage(TEMP_size);
			this->MEMBER_position_list.resize(size);
			stream_position TEMP_end = before_file_end;
			TEMP_storage.seekp(TEMP_end);
			for (std::size_t i = before_field_count;i < this->MEMBER_position_list.size();++i)
			{
				TEMP_end += stream_position(sizeof(std::size_t));
				std::uintmax_t const TEMP_field_length = 0;
				TEMP_storage.write(reinterpret_cast<char const *>(&TEMP_field_length), stream_position(sizeof(std::size_t)));
				this->MEMBER_position_list[i] = static_cast<field_info const &>(field_info(TEMP_end, 0));
			}
		}
	}

	std::size_t storage<field>::size()const
		noexcept(noexcept(std::declval<std::vector<field_info> const &>().size()))
	{
		return this->MEMBER_position_list.size();
	}

}