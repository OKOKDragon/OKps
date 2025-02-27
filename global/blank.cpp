#include <memory>

#include ".\blank.hpp"

namespace OKps
{
	blank::blank()noexcept
	{
	}
	blank::blank(blank const &)noexcept
	{
	}
	blank::blank(blank &&)noexcept
	{
	}
	blank::~blank()noexcept
	{
	}
	void blank::operator =(blank const &)noexcept
	{
	}
	void blank::operator =(blank &&)noexcept
	{
	}
	bool blank::operator <(blank const & right)const noexcept
	{
		return this < std::addressof(right);
	}

}