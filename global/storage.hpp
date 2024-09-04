#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <type_traits>
#include <filesystem>

namespace OKps
{
	/*
	按字节比较两个文件的内容是否完全相同
	*/
    bool compare(std::filesystem::path const& left, std::filesystem::path const& right);
	/*
	深度遍历文件夹，获取所有文件、子目录、子目录下的文件和子目录
	*/
	std::vector<std::filesystem::path> totally_traverse_directory(std::filesystem::path const& directory,bool save_directory/*是否将目录保存到结果中*/);

	class field_stream final
	{
	private:
		std::unique_ptr<std::byte[]> MEMBER_data;
		std::size_t MEMBER_length;
	public:
		field_stream(std::string const & data);
		~field_stream()
			noexcept(std::is_nothrow_destructible_v<std::unique_ptr<std::byte[]>>);
		field_stream(std::vector<std::byte> const & data);
		std::string raw_string()const;
		std::string field_string()const;
		std::vector<std::byte> raw_data()const;
		std::vector<std::byte> field_data()const;
		static std::vector<field_stream> parse(std::string const & data);
		static std::vector<field_stream> parse(std::vector<std::byte> const & data);
		field_stream(field_stream const & origin);
		field_stream(field_stream && origin)
			noexcept(std::is_nothrow_move_constructible_v<std::unique_ptr<std::byte[]>>);
	};
	class file_holder final
	{
	private:
		using TYPE_path = std::filesystem::path;
		std::unique_ptr<std::byte[]> MEMBER_buffer;
		std::unique_ptr<TYPE_path const> MEMBER_route;
		std::size_t MEMBER_length;
		bool MEMBER_do_write;
	public:
		std::unique_ptr<std::byte[]> const & buffer()const;
		void resize(std::size_t const size);
		std::size_t const & size()const;
		bool const & do_write()const;
		bool & do_write();
		/*
		构造时将文件全部读入缓存
		do_create参数控制当文件不存在时是否创建该文件
		*/
		file_holder(TYPE_path const & file, bool const do_create, bool const = true);
		//析构时将缓存全部写入文件
		~file_holder()noexcept(false);
		file_holder(file_holder const &) = delete;
		file_holder(file_holder &&)
			noexcept(std::is_nothrow_move_constructible_v<std::unique_ptr<std::byte[]>>
			and std::is_nothrow_move_constructible_v<std::unique_ptr<TYPE_path const>>);
	};

}
