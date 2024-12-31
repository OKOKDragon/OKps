#include <memory>

#include ".\log.hpp"

namespace OKps
{
	void log::operator =(log && origin)
		noexcept(std::is_nothrow_copy_assignable_v<std::exception>
		and std::is_nothrow_default_constructible_v<std::exception>
		and std::is_nothrow_copy_assignable_v<std::source_location>
		and std::is_nothrow_default_constructible_v<std::source_location>)
	{
		if (this != std::addressof(origin))
		{
			static_cast<std::exception &>(*this) = origin;
			static_cast<std::exception &>(origin) = std::exception();
			this->MEMBER_location = origin.MEMBER_location;
			origin.MEMBER_location = std::source_location();
		}
	}
	log::log(log && origin)
		noexcept(std::is_nothrow_default_constructible_v<std::exception>
		and std::is_nothrow_copy_constructible_v<std::exception>
		and std::is_nothrow_copy_assignable_v<std::exception>
		and std::is_nothrow_move_constructible_v<std::source_location>)
		:std::exception(origin)
		, MEMBER_location(std::move(origin.MEMBER_location))
	{
		static_cast<std::exception &>(origin) = std::exception();
	}

	/*
	c++标准只规定了 std::exception 有默认构造函数和复制构造函数。
	虽然 msvc 的标准库实现中添加了从 std::string 构造 std::exception 的构造函数，但这是标准之外的，故我决定不使用。
	取而代之，我从 std::runtime_error 中转，即先从 std::string 构造 std::runtime_error 对象，再复制到 std::exception 基类对象。
	*/
	log::log(std::string const & hint, std::source_location && location)
		noexcept(noexcept(std::exception(std::runtime_error(std::declval<std::string const &>())))
		and std::is_nothrow_move_constructible_v<std::source_location>)
		:std::exception(std::runtime_error(hint))
		, MEMBER_location(std::move(location))
	{
	}

	/*
	c++标准中 std::exception 只有默认构造和复制构造
	*/
	log::log(std::string const & hint, std::source_location const & location)
		noexcept(noexcept(std::exception(std::runtime_error(std::declval<std::string const &>())))
		and std::is_nothrow_copy_constructible_v<std::source_location>)
		:std::exception(std::runtime_error(hint))
		, MEMBER_location(location)
	{
	}
	log::~log()
		noexcept(std::is_nothrow_destructible_v<std::source_location>
			and std::is_nothrow_destructible_v<std::exception>)
	{
	}
	char const * log::what() const noexcept
	{
		return this->std::exception::what();
	}

	std::source_location const & log::location()const noexcept
	{
		return this->MEMBER_location;
	}
	log::log(log const & origin)
		noexcept(std::is_nothrow_copy_constructible_v<std::source_location>
		and std::is_nothrow_copy_constructible_v<std::exception>)
		:std::exception(origin)
		, MEMBER_location(origin.MEMBER_location)
	{
	}

	void log::operator =(log const & origin)
		noexcept(std::is_nothrow_copy_assignable_v<std::exception>
		and std::is_nothrow_copy_assignable_v<std::source_location>)
	{
		if (this != std::addressof(origin))
		{
			(*static_cast<std::exception *>(this)) = origin;
			this->MEMBER_location = origin.MEMBER_location;
		}
	}
	std::ostream & operator <<(std::ostream & output, log const & hint)
		noexcept(noexcept(std::declval<std::ostream &>() << std::declval<log const &>().what()))
	{
		return output << hint.what();
	}
}