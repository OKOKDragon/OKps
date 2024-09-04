#include <stdexcept>

#include ".\log.hpp"

namespace OKps
{
	log::log(std::string const& hint, std::source_location const& location)
		:MEMBER_hint(hint)
		,MEMBER_location(location)
		,MEMBER_valid(true)
	{
	}
	log::~log()
		noexcept(std::is_nothrow_destructible_v<std::string>
			and std::is_nothrow_destructible_v<std::source_location>)
	{}
	std::string const& log::hint()const
	{
		if (not this->MEMBER_valid)
		{
			throw std::logic_error("�˶�����ʧЧ����ֹ����");
		}
		return this->MEMBER_hint;
	}
	std::source_location const& log::location()const
	{
		if (not this->MEMBER_valid)
		{
			throw std::logic_error("�˶�����ʧЧ����ֹ����");
		}
		return this->MEMBER_location;
	}
	log::log(log const& origin)
		:MEMBER_hint(origin.MEMBER_hint)
		, MEMBER_location(origin.MEMBER_location)
		,MEMBER_valid(origin.MEMBER_valid)
	{
	}
	log::log(log&& origin)
		noexcept(std::is_nothrow_move_constructible_v<std::source_location>
			and std::is_nothrow_move_constructible_v<std::string>)
		:MEMBER_hint(std::move(origin.MEMBER_hint))
		,MEMBER_location(std::move(origin.MEMBER_location))
		,MEMBER_valid(origin.MEMBER_valid)
	{
		origin.MEMBER_valid = false;
	}
}