#pragma once

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
	bool compare(std::filesystem::path const & left, std::filesystem::path const & right);
	/*
	深度遍历文件夹，获取所有文件、子目录、子目录下的文件和子目录
	*/
	std::vector<std::filesystem::path> totally_traverse_directory(std::filesystem::path const & directory, bool save_directory/*是否将目录保存到结果中*/);

	/*
	字段，是一串2进制数据，其结构是：
	开头是以2进制表示的一个无符号整数 l ，l的数值为有效数据的长度；
	后接 l 字节2进制有效数据。
	*/
	class field_stream final
	{
	private:
		std::unique_ptr<std::byte[]> MEMBER_data;
		std::size_t MEMBER_length;
	public:
		/*
		构造空字段
		*/
		field_stream()
			noexcept(std::is_nothrow_default_constructible_v<std::unique_ptr<std::byte[]>>);
		field_stream(std::string const & data);
		~field_stream()
			noexcept(std::is_nothrow_destructible_v<std::unique_ptr<std::byte[]>>);
		field_stream(std::vector<std::byte> const & data);
		std::string raw_string()const;
		std::string field_string()const;
		std::vector<std::byte> raw_data()const;
		std::vector<std::byte> field_data()const;
		/*
		输入的数据是由多个字段连接而成的一串2进制有效数据
		如果格式错误则抛出异常
		此函数将输入的数据按字段切分
		*/
		static std::vector<field_stream> parse(std::string const & data);
		static std::vector<field_stream> parse(std::vector<std::byte> const & data);
		field_stream(field_stream const & origin);
		void operator =(field_stream const & origin);
		/*
		移动后，原对象的数据会被清空
		*/
		field_stream(field_stream && origin)
			noexcept(std::is_nothrow_move_constructible_v<std::unique_ptr<std::byte[]>>);
		void operator =(field_stream && origin)
			noexcept(std::is_nothrow_move_assignable_v<std::unique_ptr<std::byte[]>>);
	};

}
