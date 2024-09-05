#pragma once

#include <source_location>
#include <string>

namespace OKps
{
	/*
	极简代码日志
	由1个代码位置信息和1个字符串提示信息组成
	*/
	class log final
	{
	private:
		std::source_location MEMBER_location;
		std::string MEMBER_hint;
		bool MEMBER_valid;
	public:
		log(std::string const & hint, std::source_location const & location = std::source_location::current());
		log(log const & origin);
		log(log && origin)
			noexcept(std::is_nothrow_move_constructible_v<std::source_location>
				and std::is_nothrow_move_constructible_v<std::string>);
		~log()
			noexcept(std::is_nothrow_destructible_v<std::string>
				and std::is_nothrow_destructible_v<std::source_location>);
		std::string const & hint()const;
		std::source_location const & location()const;
	};
}