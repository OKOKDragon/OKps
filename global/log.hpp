#pragma once

#include <source_location>
#include <string>
#include <exception>
#include <iostream>

namespace OKps
{

	/*
	极简代码日志
	包含提示字符串和源码信息

	此类继承 std::exception，以便于作为异常抛出、捕获。
	std::exception 可以复制，无法移动，此类保持该行为。
	*/
	class log final :public std::exception
	{
	private:
		std::source_location MEMBER_location;
		//std::string MEMBER_hint;
	public:
		void operator =(log && origin)
			noexcept(std::is_nothrow_copy_assignable_v<std::exception>
			and std::is_nothrow_default_constructible_v<std::exception>
			and std::is_nothrow_copy_assignable_v<std::source_location>
			and std::is_nothrow_default_constructible_v<std::source_location>);
		log(log && origin)
			noexcept(std::is_nothrow_default_constructible_v<std::exception>
			and std::is_nothrow_copy_constructible_v<std::exception>
			and std::is_nothrow_copy_assignable_v<std::exception>
			and std::is_nothrow_move_constructible_v<std::source_location>);

		log(std::string const & hint, std::source_location && location = std::source_location::current())
			noexcept(noexcept(std::exception(std::runtime_error(std::declval<std::string const &>())))
			and std::is_nothrow_move_constructible_v<std::source_location>);

		log(std::string const & hint, std::source_location const & location)
			noexcept(noexcept(std::exception(std::runtime_error(std::declval<std::string const &>())))
			and std::is_nothrow_copy_constructible_v<std::source_location>);

		log(log const & origin)
			noexcept(std::is_nothrow_copy_constructible_v<std::source_location>
			and std::is_nothrow_copy_constructible_v<std::exception>);
		void operator =(log const & origin)
			noexcept(std::is_nothrow_copy_assignable_v<std::exception>
			and std::is_nothrow_copy_assignable_v<std::source_location>);
		~log()
			noexcept(std::is_nothrow_destructible_v<std::source_location>
				and std::is_nothrow_destructible_v<std::exception>)
			override;
		//获取日志对应的代码源文件位置
		std::source_location const & location()const noexcept;
		//获取提示字符串
		char const * what() const noexcept override;
	};

	/*
	向标准输出流输出 log 对象的提示字符串
	*/
	std::ostream & operator <<(std::ostream &, log const &)
		noexcept(noexcept(std::declval<std::ostream &>() << std::declval<log const &>().what()));
}