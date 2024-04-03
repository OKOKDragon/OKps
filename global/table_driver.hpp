#pragma once

#include ".\base.hpp"

namespace OKps::global
{
	// template <typename return_type, typename... argument_list>

	template <typename key_type,		 /*用来映射函数对象的类型，相当于switch或if语句的条件*/
		typename return_type,		 /*返回值类型*/
		typename... argument_list> /*函数参数表*/
/*
驱动表是优化大量if else或switch case的技术
我们通过可变参数模板和std::map实现
*/
	class table_driver
	{
	public:
		typedef return_type function_type(argument_list &&...); /*函数类型*/
	private:
		/*std::any可以保存任意类型数据。使用std::any_cast作类型转换，保证是类型安全的。但要求类型是可复制的。*/
		std::map<key_type, std::function<function_type>> function_map;

	public:
		/*
		注册器，将函数与条件匹配，并放入驱动表
		register是关键字，只好用regist这个不存在的词表达。这个函数会被程序员多次直接调用，不适合用下划线 “_” 开头
		注意，若函数function有参数，调用时不可以省略本函数的模板参数
		即，对于函数 R f(T t)，必须写 regist<T>(key,f)，否则编译器默认f为无参数函数
		*/
		void regist(const key_type & key,
					function_type && function); /*函数*/
		/*执行函数*/
		return_type execute(const key_type & key, argument_list &&...arguments);
		/*
		执行函数，无参数
		因为用了可变参数模板，在cpp中，无参数的情况必须单独重载
		*/
		// return_type execute(const key_type &key);
	};

	template <typename key_type, typename return_type, typename... argument_list>
	void table_driver<key_type, return_type, argument_list...>::regist(const key_type & key, table_driver<key_type, return_type, argument_list...>::function_type && function)
	{
		/*包装函数
		作为std::map中键值对的值*/

		// auto value = std::function<return_type(argument_list && ...)>(/*值传递*/ [=](argument_list &&.../*可变参数列表*/ arguments)
		//															  {
		/*函数体*/
		//		return function(/*完美转发*/std::forward<argument_list>(arguments)...); });

		/*写入map*/
		this->function_map[key] = function;
	}

	template <typename key_type, typename return_type, typename... argument_list>
	return_type table_driver<key_type, return_type, argument_list...>::execute(const key_type & key, argument_list &&...arguments)
	{
		auto order = this->function_map.find(key);
		if (order == this->function_map.end())
		/*没有找到命令*/
		{
			throw std::invalid_argument("此命令不存在");
		}
		else
		{
			return order->second(std::forward<argument_list>(arguments)...);
		}
	}
	// template <typename key_type, typename return_type, typename... argument_list>
	// return_type table_driver<key_type, return_type, argument_list...>::execute(const key_type &key)
	//{
	// auto order = this->function_map.find(key);
	// if (order == this->function_map.end())
	/*没有找到命令*/
	//{
	// throw OKps::error::information(u8"此命令不存在", OKps::global::string::state::UTF8, false, false, false, OKps::error::information::error_type::internal_debug);
	//}
	// else
	//{
	// return order->second();
	//}
	//}

}
