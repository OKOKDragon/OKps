#include <fstream>

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