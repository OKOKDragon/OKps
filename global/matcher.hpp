#pragma once

#include <type_traits>
#include <string>
#include <unordered_map>
#include <memory>

namespace OKps
{
	//命令匹配器
	class order_matcher final
	{
	public:
		class handler;
		/*
		参数基类
		*/
		class parameter
		{
			friend class order_matcher::handler;
		protected:
			parameter()noexcept;
			parameter(parameter const &)noexcept;
			parameter(parameter &&)noexcept;
		public:
			virtual ~parameter()noexcept;
		protected:
			virtual void operator =(parameter const &)noexcept;
			virtual void operator =(parameter &&)noexcept;

		};
		/*
		处理器基类
		*/
		class handler
		{
			friend class order_matcher;
		protected:
			handler()noexcept;
			handler(handler const &)noexcept;
			virtual void operator =(handler const &)noexcept;
			handler(handler &&)noexcept;
			virtual void operator =(handler &&)noexcept;
		public:
			virtual ~handler()noexcept;
		protected:
			using argument_type = std::unique_ptr<parameter> const &;
			virtual void handle(argument_type) = 0;
		};

	private:
		using TYPE_work = std::unique_ptr<handler>;
		using TYPE_pool = std::unordered_map<std::string, TYPE_work>;

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
		void regist(std::string const & order, TYPE_work && work);
		//删除命令
		void erase(std::string const & order);
		//使用参数para执行命令order
		void execute(std::string const & order, handler::argument_type para)const;

		handler const & find(std::string const & order)const;
		handler & find(std::string const order);
	};

}
