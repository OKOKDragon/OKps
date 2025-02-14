
#include <stdexcept>
#include <locale>

#include ".\matcher.hpp"

namespace OKps
{

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
		if (this != std::addressof(origin))
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
	void order_matcher::regist(std::string const & order, handler_pointer && work)
	{
		if (not work)
		{
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
	void order_matcher::execute(std::string const & order, base::blank & para)const
	{
		auto ref = this->MEMBER_orders.find(order);
		if (ref != this->MEMBER_orders.end())
		{
			auto & TEMP_handler = (*(ref->second));
			TEMP_handler(para);
		}
		else
		{
			std::string const hint = std::string("命令“") + order + "”不存在";
			throw std::invalid_argument(hint);
		}
	}
	base::handler<false> const & order_matcher::find(std::string const & order)const
	{
		auto ref = this->MEMBER_orders.find(order);
		if (ref == this->MEMBER_orders.end())
		{
			throw std::invalid_argument("该命令不存在");
		}
		return *(ref->second);
	}
	base::handler<false>& order_matcher::find(std::string const order)
	{
		auto ref = this->MEMBER_orders.find(order);
		if (ref == this->MEMBER_orders.end())
		{
			throw std::invalid_argument("该命令不存在");
		}
		return *(ref->second);
	}
}