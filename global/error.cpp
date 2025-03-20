#include <memory>

#include ".\error.hpp"

namespace OKps
{
	void error::operator =(error && origin)
		noexcept(std::is_nothrow_move_assignable_v<std::exception_ptr>
		and std::is_nothrow_move_assignable_v<std::source_location>)
	{
		if (this != (&origin))
		{
			this->MEMBER_error = std::move(origin.MEMBER_error);
			this->MEMBER_location = std::move(origin.MEMBER_location);
		}
	}

	error::error(error && origin)
		noexcept(std::is_nothrow_move_constructible_v<std::exception_ptr>
		and std::is_nothrow_move_constructible_v<std::source_location>)
		:MEMBER_error(std::move(origin.MEMBER_error))
		, MEMBER_location(std::move(origin.MEMBER_location))
	{
	}

	error::error(std::exception_ptr && error_holder, std::source_location && location)
		noexcept(std::is_nothrow_move_constructible_v<std::exception_ptr>
		and std::is_nothrow_move_constructible_v<std::source_location>)
		:MEMBER_error(std::move(error_holder))
		, MEMBER_location(std::move(location))
	{
	}

	error::error(std::exception_ptr const & error_holder, std::source_location && location)
		noexcept(std::is_nothrow_copy_constructible_v<std::exception_ptr>
		and std::is_nothrow_move_constructible_v<std::source_location>)
		:MEMBER_error(error_holder)
		, MEMBER_location(std::move(location))
	{
	}

	error::error(std::exception_ptr && error_holder, std::source_location const & location)
		noexcept(std::is_nothrow_move_constructible_v<std::exception_ptr>
		and std::is_nothrow_copy_constructible_v<std::source_location>)
		:MEMBER_error(std::move(error_holder))
		, MEMBER_location(location)
	{
	}

	error::error(std::exception_ptr const & error_holder, std::source_location const & location)
		noexcept(std::is_nothrow_copy_constructible_v<std::exception_ptr>
		and std::is_nothrow_copy_constructible_v<std::source_location>)
		:MEMBER_error(error_holder)
		, MEMBER_location(location)
	{
	}

	error::error(error const & origin)
		noexcept(std::is_nothrow_copy_constructible_v<std::source_location>
		and std::is_nothrow_copy_constructible_v<std::exception_ptr>)
		:MEMBER_error(origin.MEMBER_error)
		, MEMBER_location(origin.MEMBER_location)
	{
	}

	void error::operator =(error const & origin)
		noexcept(std::is_nothrow_copy_assignable_v<std::exception_ptr>
		and std::is_nothrow_copy_assignable_v<std::source_location>)
	{
		if (this != (&origin))
		{
			this->MEMBER_error = origin.MEMBER_error;
			this->MEMBER_location = origin.MEMBER_location;
		}
	}

	error::~error()
		noexcept(std::is_nothrow_destructible_v<std::source_location>
			and std::is_nothrow_destructible_v<std::exception_ptr>)
	{
	}

	std::source_location const & error::location()const noexcept
	{
		return this->MEMBER_location;
	}

	bool error::have_error()const noexcept
	{
		return static_cast<bool>(this->MEMBER_error);
	}

	void error::release_error() noexcept(false)
	{
		if (this->MEMBER_error)
		{
			std::exception_ptr TEMP_error = std::move(this->MEMBER_error);
			std::rethrow_exception(TEMP_error);
		}
	}

	bool error::operator <(error const & right)const noexcept
	{
		return this < (&right);
	}

}