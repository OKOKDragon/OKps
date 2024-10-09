#include <stdexcept>
#include <locale>

#include ".\matcher.hpp"

namespace OKps
{
	order_matcher::parameter::parameter()noexcept
	{
	}
	order_matcher::parameter::parameter(parameter const &)noexcept
	{
	}
	order_matcher::parameter::parameter(parameter &&)noexcept
	{
	}
	order_matcher::parameter::~parameter()noexcept
	{
	}
	void order_matcher::parameter::operator =(parameter const &)noexcept
	{
	}
	void order_matcher::parameter::operator =(parameter &&)noexcept
	{
	}
	order_matcher::handler::handler()noexcept
	{
	}
	order_matcher::handler::handler(handler const & origin)noexcept
	{
	}
	void order_matcher::handler::operator =(handler const & origin)  noexcept
	{
	}
	order_matcher::handler::handler(handler && origin)noexcept
	{
	}
	void order_matcher::handler::operator =(handler && origin)  noexcept
	{
	}
	order_matcher::handler::~handler()noexcept
	{
	}

	order_matcher::order_matcher()
		noexcept(std::is_nothrow_default_constructible_v<TYPE_pool>)
	{
	}
	order_matcher::order_matcher(order_matcher && origin)
		noexcept(std::is_nothrow_move_constructible_v<TYPE_pool>)
		:MEMBER_orders(std::move(origin.MEMBER_orders))
	{
	}
	void order_matcher::operator =(order_matcher && origin)
		noexcept(std::is_nothrow_move_assignable_v<TYPE_pool>)
	{
		if (this != (&origin))
		{
			this->MEMBER_orders = std::move(origin.MEMBER_orders);
		}
	}
	/*
	order_matcher::order_matcher(order_matcher const & origin)
		noexcept(std::is_nothrow_copy_constructible_v<TYPE_pool>)
		: MEMBER_orders(origin.MEMBER_orders)
	{
	}
	*/
	order_matcher::~order_matcher()
		noexcept(std::is_nothrow_destructible_v<TYPE_pool>)
	{
	}
	void order_matcher::regist(std::string const & order, TYPE_work && work)
	{
		if (not work)
		{
			std::locale::global(std::locale::classic());
			throw std::invalid_argument("禁止输入空指针");
		}
		auto ref = this->MEMBER_orders.find(order);
		if (ref == this->MEMBER_orders.end())
		{
			this->MEMBER_orders.insert(std::make_pair(order, std::move(work)));
		}
		else
		{
			ref->second = std::move(work);
		}
	}
	//删除命令
	void order_matcher::erase(std::string const & order)
	{
		auto ref = this->MEMBER_orders.find(order);
		if (ref != this->MEMBER_orders.end())
		{
			this->MEMBER_orders.erase(ref);
		}
	}
	//执行命令
	void order_matcher::execute(std::string const & order, handler::argument_type para)const
	{
		auto ref = this->MEMBER_orders.find(order);
		if (ref != this->MEMBER_orders.end())
		{
			ref->second->handle(para);
		}
		else
		{
			std::locale::global(std::locale::classic());
			std::string const hint = std::string("命令“") + order + "”不存在";
			throw std::invalid_argument(hint);
		}
	}
	order_matcher::handler const & order_matcher::find(std::string const & order)const
	{
		auto ref = this->MEMBER_orders.find(order);
		if (ref == this->MEMBER_orders.end())
		{
			std::locale::global(std::locale::classic());
			throw std::invalid_argument("该命令不存在");
		}
		return *(ref->second);
	}
	order_matcher::handler & order_matcher::find(std::string const order)
	{
		auto ref = this->MEMBER_orders.find(order);
		if (ref == this->MEMBER_orders.end())
		{
			std::locale::global(std::locale::classic());
			throw std::invalid_argument("该命令不存在");
		}
		return *(ref->second);
	}
}