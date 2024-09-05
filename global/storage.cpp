#include <fstream>
#include <random>

#include ".\bits.hpp"
#include ".\storage.hpp"


namespace OKps
{
	bool compare(std::filesystem::path const & left, std::filesystem::path const & right)
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
		auto const length = fs::file_size(left);
		bool result;
		if (length != fs::file_size(right))
		{
			result = false;
		}
		else
		{
			constexpr std::uintmax_t const buffer_length = (std::uintmax_t)1024 * 64;
			std::uintmax_t const read_times = length / buffer_length;
			std::uintmax_t last_read_length = length % buffer_length;
			if (last_read_length == 0 and read_times != 0)
			{
				last_read_length = buffer_length;
			}

			result = true;
			std::ifstream left_reader;
			std::ifstream right_reader;
			left_reader.open(left, std::ios::in | std::ios::binary);
			right_reader.open(right, std::ios::in | std::ios::binary);
			if (not left_reader.is_open())
			{
				std::string const hint = "文件 " + left.string() + " 无法打开";
				throw std::runtime_error(hint);
			}
			if (not right_reader.is_open())
			{
				std::string const hint = "文件 " + right.string() + " 无法打开";
				throw std::runtime_error(hint);
			}
			std::unique_ptr<char[]> left_buffer = std::make_unique<char[]>(buffer_length);
			std::unique_ptr<char[]> right_buffer = std::make_unique<char[]>(buffer_length);
			bool done = false;
			for (std::uintmax_t j = 0; j < read_times and (not done); j++)
			{
				if (not left_reader.read(left_buffer.get(), buffer_length))
				{
					std::string const hint = "读取文件 "
						+ left.string()
						+ " 失败";
					throw std::runtime_error(hint);
				}
				if (not right_reader.read(right_buffer.get(), buffer_length))
				{
					std::string const hint = "读取文件 "
						+ right.string()
						+ " 失败";
					throw std::runtime_error(hint);
				}
				for (std::size_t i = 0; i < buffer_length and (not done); i++)
				{
					if (left_buffer[i] != right_buffer[i])
					{

						result = false;
						done = true;
					}
				}
			}
			if (not done)
			{
				if (not left_reader.read(left_buffer.get(), last_read_length))
				{
					std::string const hint = "读取文件 "
						+ left.string()
						+ " 失败";
					throw std::runtime_error(hint);
				}
				if (not right_reader.read(right_buffer.get(), last_read_length))
				{
					std::string const hint = "读取文件 "
						+ right.string()
						+ " 失败";
					throw std::runtime_error(hint);
				}
				for (std::size_t i = 0; i < last_read_length and (not done); i++)
				{
					if (left_buffer[i] != right_buffer[i])
					{

						result = false;
						done = true;
					}
				}
			}
		}

		return result;
	}
	std::vector<std::filesystem::path> totally_traverse_directory(std::filesystem::path const & directory, bool save_directory)
	{
		namespace fs = std::filesystem;
		if (not fs::is_directory(directory))
		{
			std::string const hint = "路径 " + directory.string() + " 不是目录";
			throw std::invalid_argument(hint);
		}
		if (not fs::exists(directory))
		{
			std::string const hint = "路径 " + directory.string() + " 不存在";
			throw std::invalid_argument(hint);
		}
		std::vector<std::filesystem::path> result;
		for (auto entry = fs::recursive_directory_iterator(directory, fs::directory_options::skip_permission_denied | fs::directory_options::follow_directory_symlink); entry != fs::end(entry); ++entry)
		{
			if (entry->is_directory())
			{
				if (save_directory)
				{
					result.push_back(entry->path());
				}
			}
			else
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
			result[i] = ((char const *)(&(this->MEMBER_length)))[i];
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
			result[i] = ((std::byte const *)(&(this->MEMBER_length)))[i];
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
				((char *)(&length))[i] = data[position];
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
				((std::byte *)(&length))[i] = data[position];
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
	field_stream::field_stream(field_stream && origin)
		noexcept(std::is_nothrow_move_constructible_v<std::unique_ptr<std::byte[]>>)
		:MEMBER_data(std::move(origin.MEMBER_data))
		, MEMBER_length(origin.MEMBER_length)
	{
		origin.MEMBER_length = 0;
	}
	bool const & file_holder::do_write()const
	{
		if (not this->MEMBER_route)
		{
			throw std::runtime_error("此对象已失效");
		}
		return this->MEMBER_do_write;
	}
	bool & file_holder::do_write()
	{
		if (not this->MEMBER_route)
		{
			throw std::runtime_error("此对象已失效");
		}
		return this->MEMBER_do_write;
	}
	std::unique_ptr<std::byte[]> const & file_holder::buffer()const
	{
		if (not this->MEMBER_route)
		{
			throw std::runtime_error("此对象已失效");
		}
		return this->MEMBER_buffer;
	}
	std::size_t const & file_holder::size()const
	{
		if (not this->MEMBER_route)
		{
			throw std::runtime_error("此对象已失效");
		}
		return this->MEMBER_length;
	}
	void file_holder::resize(std::size_t const size)
	{
		if (not this->MEMBER_route)
		{
			throw std::runtime_error("此对象已失效");
		}
		if (size < this->MEMBER_length)
		{
			std::unique_ptr<std::byte[]> temp = std::make_unique<std::byte[]>(size);
			for (std::size_t i = 0; i < size; i++)
			{
				temp[i] = this->MEMBER_buffer[i];
			}
			this->MEMBER_buffer = std::move(temp);
			this->MEMBER_length = size;
		}
		else if (size > this->MEMBER_length)
		{
			std::unique_ptr<std::byte[]> temp = std::make_unique<std::byte[]>(size);
			for (std::size_t i = 0; i < this->MEMBER_length; i++)
			{
				temp[i] = this->MEMBER_buffer[i];
			}
			std::random_device seed;
			auto engine = std::mt19937_64(seed());
			using TYPE_byte = std::underlying_type<std::byte>::type;
			auto distribution = std::uniform_int_distribution<short>(std::numeric_limits<TYPE_byte>::min(), std::numeric_limits<TYPE_byte>::max());
			for (std::size_t i = this->MEMBER_length; i < size; i++)
			{
				temp[i] = (std::byte)((TYPE_byte)(distribution(engine)));
			}
			this->MEMBER_buffer = std::move(temp);
			this->MEMBER_length = size;
		}
	}
	//构造时将文件全部读入缓存
	file_holder::file_holder(TYPE_path const & file, bool const do_create, bool const ARG_do_write)
		:MEMBER_do_write(ARG_do_write)
	{
		namespace fs = std::filesystem;
		std::unique_ptr<TYPE_path const> abs;
		if (file.is_absolute())
		{
			abs = std::make_unique<TYPE_path const>(file);
		}
		else
		{
			abs = std::make_unique<TYPE_path const>(fs::absolute(file));
		}
		if (not abs->has_filename())
		{
			std::string const hint = "路径“"
				+ abs->string()
				+ "”不是文件";
			throw std::invalid_argument(hint);
		}
		if (not fs::exists(*abs))
		{
			if (not do_create)
			{
				std::string const hint = "文件“"
					+ abs->string()
					+ "”不存在";
				throw std::invalid_argument(hint);
			}
			else
			{
				std::ofstream creator;
				creator.open(*abs, std::ios::out | std::ios::binary | std::ios::trunc);
				if (not creator.is_open())
				{
					std::string const hint = "创建文件“"
						+ abs->string()
						+ "”失败";
					throw std::runtime_error(hint);
				}
			}
		}
		this->MEMBER_length = fs::file_size(*abs);
		this->MEMBER_buffer = std::make_unique<std::byte[]>(this->MEMBER_length);
		std::ifstream reader;
		reader.open(*abs, std::ios::in | std::ios::binary);
		if (not reader.is_open())
		{
			std::string const hint = "打开文件“"
				+ abs->string()
				+ "”失败";
			throw std::runtime_error(hint);
		}
		if (not reader.read((char *)(this->MEMBER_buffer.get()), this->MEMBER_length))
		{
			std::string const hint = "读取文件“"
				+ abs->string()
				+ "”失败";
			throw std::runtime_error(hint);
		}
		this->MEMBER_route = std::move(abs);
	}
	//析构时将缓存全部写入文件
	file_holder::~file_holder()noexcept(false)
	{
		if (not this->MEMBER_route)
		{
			return;
		}
		if (not this->MEMBER_do_write)
		{
			return;
		}
		namespace fs = std::filesystem;
		std::ofstream writter;
		writter.open(*(this->MEMBER_route), std::ios::out | std::ios::binary | std::ios::trunc);
		if (not writter.is_open())
		{
			std::string const hint = "打开文件“"
				+ this->MEMBER_route->string()
				+ "”失败";
			throw std::runtime_error(hint);
		}
		if (not writter.write((char *)(this->MEMBER_buffer.get()), this->MEMBER_length))
		{
			std::string const hint = "修改文件“"
				+ this->MEMBER_route->string()
				+ "”失败";
			throw std::runtime_error(hint);
		}

	}
	file_holder::file_holder(file_holder && origin)
		noexcept(std::is_nothrow_move_constructible_v<std::unique_ptr<std::byte[]>>
			and std::is_nothrow_move_constructible_v<std::unique_ptr<TYPE_path const>>)
		:MEMBER_buffer(std::move(origin.MEMBER_buffer))
		, MEMBER_route(std::move(origin.MEMBER_route))
		, MEMBER_length(origin.MEMBER_length)
		, MEMBER_do_write(origin.MEMBER_do_write)
	{
	}

}