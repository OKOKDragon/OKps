#pragma once

#include <string>
#include <vector>
#include <memory>
#include <type_traits>
#include <set>
#include <filesystem>

namespace OKps
{
	/*
	按字节比较两个文件的内容是否完全相同
	thread 参数表示建议使用的线程数
	*/
	bool compare(std::filesystem::path const & left, std::filesystem::path const & right, std::uintmax_t const thread = 0);
	/*
	深度遍历目录 directory，递归地获取此目录下所有文件、子目录、子目录下的文件和子目录
	result_types 参数决定此函数返回的结果中排除哪些类型的文件
	*/
	std::vector<std::filesystem::path> totally_traverse_directory(std::filesystem::path const & directory, std::set<std::filesystem::file_type> const & result_types = std::set<std::filesystem::file_type>());

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
