#pragma once

#include <map>

#include ".\base.hpp"

namespace OKps
{
	//命令匹配器
	class order_matcher final
	{
	public:

		using handler_pointer = std::unique_ptr<base::handler<false>>;
	private:
		using TYPE_pool = std::map<std::string, handler_pointer>;

		TYPE_pool MEMBER_orders;
	public:
		order_matcher()
			noexcept(std::is_nothrow_default_constructible_v<TYPE_pool>);
		/*
		移动后原对象会彻底清空
		*/
		order_matcher(order_matcher &&)
			noexcept(std::is_nothrow_move_constructible_v<TYPE_pool>);
		void operator =(order_matcher &&)
			noexcept(std::is_nothrow_move_assignable_v<TYPE_pool>);
		order_matcher(order_matcher const &)
			noexcept(std::is_nothrow_copy_constructible_v<TYPE_pool>)
			= delete;
		void operator =(order_matcher const &) = delete;
		~order_matcher()
			noexcept(std::is_nothrow_destructible_v<TYPE_pool>);
		/*
		注册命令order
		work参数是一个函数对象，它指定了使用execute函数执行命令order时调用的函数
		如果order命令已经被注册，则覆盖其对应的函数对象
		work函数对象需要自行解决其参数的运行时多态的类型安全问题
		*/
		void regist(std::string const & order, handler_pointer && work);
		//删除命令
		void erase(std::string const & order);
		//使用参数para执行命令order
		void execute(std::string const & order, base::blank & para)const;

		base::handler<false> const & find(std::string const & order)const;
		base::handler<false> & find(std::string const order);
	};

}
