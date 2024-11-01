
#include ".\log.hpp"

namespace OKps
{
	/*
	c++标准中 std::exception 只有默认构造和复制构造
	*/
	log::log(std::string const & hint, std::source_location const & location)
		noexcept(std::is_nothrow_default_constructible_v<std::exception>
and std::is_nothrow_copy_constructible_v<std::string>
and std::is_nothrow_copy_constructible_v<std::source_location>)
	:std::exception()
		, MEMBER_hint(hint)
		, MEMBER_location(location)
	{
	}
	log::~log()
		noexcept(std::is_nothrow_destructible_v<std::string>
			and std::is_nothrow_destructible_v<std::source_location>
			and std::is_nothrow_destructible_v<std::exception>)
	{
	}
	char const * log::what() const
		noexcept(noexcept(std::declval<std::string const>().c_str()))
	{
		return this->MEMBER_hint.c_str();
	}
	std::string const & log::hint()const noexcept
	{
		return this->MEMBER_hint;
	}
	std::source_location const & log::location()const noexcept
	{
		return this->MEMBER_location;
	}
	log::log(log const & origin)
		noexcept(std::is_nothrow_copy_constructible_v<std::string>
		and std::is_nothrow_copy_constructible_v<std::source_location>
		and std::is_nothrow_copy_constructible_v<std::exception>)
		:std::exception(origin)
		, MEMBER_hint(origin.MEMBER_hint)
		, MEMBER_location(origin.MEMBER_location)
	{
	}

	void log::operator =(log const & origin)
		noexcept(std::is_nothrow_copy_assignable_v<std::exception>
and std::is_nothrow_copy_assignable_v<std::string>
and std::is_nothrow_copy_assignable_v<std::source_location>)
	{
		if (this != std::addressof(origin))
		{
			(*static_cast<std::exception *>(this)) = origin;
			this->MEMBER_hint = origin.MEMBER_hint;
			this->MEMBER_location = origin.MEMBER_location;
		}
	}

}