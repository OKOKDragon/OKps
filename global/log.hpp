#pragma once

#include <source_location>
#include <string>
#include <exception>

namespace OKps
{
	/*
	极简代码日志

	此类继承 std::exception，以便于作为异常抛出、捕获。
	std::exception 可以复制，无法移动，此类保持该行为。
	*/
	class log final :public std::exception
	{
	private:
		std::source_location MEMBER_location;
		std::string MEMBER_hint;
	public:
		void operator =(log &&) = delete;
		log(log &&) = delete;
		log(std::string const & hint, std::source_location const & location = std::source_location::current())
			noexcept(std::is_nothrow_default_constructible_v<std::exception>
			and std::is_nothrow_copy_constructible_v<std::string>
			and std::is_nothrow_copy_constructible_v<std::source_location>);
		log(log const & origin)
			noexcept(std::is_nothrow_copy_constructible_v<std::string>
			and std::is_nothrow_copy_constructible_v<std::source_location>
			and std::is_nothrow_copy_constructible_v<std::exception>);
		void operator =(log const & origin)
			noexcept(std::is_nothrow_copy_assignable_v<std::exception>
			and std::is_nothrow_copy_assignable_v<std::string>
			and std::is_nothrow_copy_assignable_v<std::source_location>);
		~log()
			noexcept(std::is_nothrow_destructible_v<std::string>
				and std::is_nothrow_destructible_v<std::source_location>
				and std::is_nothrow_destructible_v<std::exception>)
			override;
		std::string const & hint()const noexcept;
		std::source_location const & location()const noexcept;
		char const * what() const
			noexcept(noexcept(std::declval<std::string const>().c_str()))
			override;
	};
}