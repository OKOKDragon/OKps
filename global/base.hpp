﻿#pragma once

#include <thread>
#include <exception>
#include <type_traits>
#include <string>
#include <random>
#include <memory>

/*
此文件在命名空间 OKps::base 定义多种通用的基类
基类都有self函数，返回值是到基类对象自身的引用
*/

namespace OKps::base
{
	/*
	空基类
	*/
	class blank
	{
	protected:
		blank()noexcept;
		blank(blank const &)noexcept;
		virtual ~blank()noexcept;
		blank(blank &&)noexcept;
		virtual void operator =(blank &&)noexcept;
		virtual void operator =(blank const &)noexcept;
		virtual blank & self()noexcept;
		virtual blank const & self()const noexcept;

	};
	/*
	此基类有一个bool 类型标志，true代表对象有效，false代表对象无效
	对象构造时设为有效对象，被移动后会标识为无效对象
	*/
	class move_invalid
	{
	private:
		bool MEMBER_valid;
	protected:
		bool const & is_valid()const noexcept;
		/*
		检查当前对象是否有效，
		如果有效则什么都不做，如果无效则抛出异常
		*/
		void raise_invalid_error()const noexcept(false);
		move_invalid()noexcept;
		move_invalid(move_invalid const &)noexcept;
		virtual void operator =(move_invalid const &)noexcept;
		move_invalid(move_invalid &&)noexcept;
		virtual void operator =(move_invalid &&)noexcept;
		virtual ~move_invalid()noexcept;
		virtual move_invalid & self()noexcept;
		virtual move_invalid const & self()const noexcept;

	};
	/*
	具有一个无符号整数作为编号的基类
	*/
	class numbered
	{
	protected:
		/*
		随机生成编号的辅助类
		*/
		class id_generator final
		{
		private:
			std::random_device MEMBER_seed;
			std::unique_ptr<std::mt19937_64> MEMBER_engine;
			std::uniform_int_distribution<std::uintmax_t> MEMBER_generator;
		public:
			id_generator();
			~id_generator()
				noexcept(std::is_nothrow_destructible_v<std::random_device>
					and std::is_nothrow_destructible_v<std::unique_ptr<std::mt19937_64>>
					and std::is_nothrow_destructible_v<std::uniform_int_distribution<std::uintmax_t>>);
			id_generator(id_generator const &) = delete;
			id_generator(id_generator &&) = delete;
			void operator =(id_generator const &) = delete;
			void operator =(id_generator &&) = delete;

			std::uintmax_t generate()
				noexcept(noexcept(std::declval<std::uniform_int_distribution<std::uintmax_t>>()(*std::declval<std::unique_ptr<std::mt19937_64>>())));
		};

	private:
		std::uintmax_t MEMBER_id;
	protected:
		numbered(std::uintmax_t const id = 0)noexcept;
		numbered(numbered const &)noexcept;
		virtual void operator =(numbered const &)noexcept;
		numbered(numbered &&)noexcept = delete;
		virtual void operator =(numbered &&)noexcept = delete;
		virtual ~numbered()noexcept;
		std::uintmax_t & id()noexcept;
		std::uintmax_t const & id()const noexcept;
		virtual numbered & self()noexcept;
		virtual numbered const & self()const noexcept;
	};
	/*
	具有一个字符串作为标识的基类
	*/
	class named
	{
	private:
		std::string MEMBER_name;
	protected:
		named(std::string const &)
			noexcept(std::is_nothrow_copy_constructible<std::string>::value);
		named(std::string && = std::string())
			noexcept(std::is_nothrow_move_constructible<std::string>::value);
		named(named const &)
			noexcept(std::is_nothrow_copy_constructible<std::string>::value);
		virtual void operator =(named const &)
			noexcept(std::is_nothrow_copy_assignable<std::string>::value);
		named(named &&)
			noexcept(std::is_nothrow_move_constructible<std::string>::value);
		virtual void operator =(named &&)
			noexcept(std::is_nothrow_move_assignable<std::string>::value);
		virtual ~named()
			noexcept(std::is_nothrow_destructible<std::string>::value);
		std::string const & name()const noexcept;
		std::string & name()noexcept;
		virtual named & self()noexcept;
		virtual named const & self()const noexcept;
	};
	/*
	参数基类
	和空基类一样，但专用做处理函数类的参数
	*/
	class parameter
	{
	protected:
		parameter()noexcept;
		parameter(parameter const &)noexcept;
		virtual ~parameter()noexcept;
		parameter(parameter &&)noexcept;
		virtual void operator =(parameter &&)noexcept;
		virtual void operator =(parameter const &)noexcept;
		virtual parameter & self()noexcept;
		virtual parameter const & self()const noexcept;
	};
	/*
	处理函数基类
	可以用作回调函数的标准类型，而不用 lambda 配合模板的形式
	例如，想要在函数 f 中使用回调函数 h，则可以写成
	result_type f(handler& h, parameter& p);
	其中h.handle()函数相当于回调函数，p是其参数
	*/
	class handler
	{
	protected:
		handler()noexcept;
		handler(handler const &)noexcept;
		virtual ~handler()noexcept;
		handler(handler &&)noexcept;
		virtual void operator =(handler &&)noexcept;
		virtual void operator =(handler const &)noexcept;
		virtual handler & self()noexcept;
		virtual handler const & self()const noexcept;
		/*
		用智能指针的形式接收参数，是因为有可能想要用空指针代表无参数；
		但是又不希望不小心写出错误的 delete 或其他错误的指针操作，所以避免使用裸指针。
		*/
		virtual void handle(std::unique_ptr<parameter> const &) = 0;
	};
	/*
	此基类要求子类实现函数self_copy，其功能为复制子类对象自身，并包装到指向基类的智能指针中返回。
	这种做法解决的问题是，已知一个基类指针 p 指向子类对象 o，如何在编译期不知道对象 o 真正类型的情况下正确复制对象 o
	*/
	class copier
	{
	protected:
		copier()noexcept;
		copier(copier const &)noexcept;
		virtual ~copier()noexcept;
		copier(copier &&)noexcept;
		virtual void operator =(copier &&)noexcept;
		virtual void operator =(copier const &)noexcept;
		virtual copier & self()noexcept;
		virtual copier const & self()const noexcept;
		/*
		因为必然有动态内存分配，所以允许抛出异常
		*/
		virtual std::unique_ptr<copier> self_copy() const = 0;
	};
	/*
	线程工作者基类

	基类管理一个线程，该线程的线程函数是work函数，work函数是纯虚函数，必须由子类实现
	work函数禁止抛出异常，如果需要使用异常，则使用此基类提供的函数，将异常存储在基类中
	*/
	class worker
	{
	private:
		std::thread MEMBER_thread;
		std::exception_ptr MEMBER_error;
	protected:
		worker()
			noexcept(std::is_nothrow_default_constructible<std::thread>::value
				and std::is_nothrow_default_constructible<std::exception_ptr>::value);
		worker(worker const &) = delete;
		virtual void operator =(worker const &) = delete;
		worker(worker &&)
			noexcept(std::is_nothrow_move_constructible<std::thread>::value
				and std::is_nothrow_copy_constructible<std::exception_ptr>::value);
		virtual void operator =(worker &&)
			noexcept(std::is_nothrow_move_assignable<std::thread>::value
				and std::is_nothrow_copy_assignable<std::exception_ptr>::value);
		virtual ~worker()
			noexcept(std::is_nothrow_destructible<std::thread>::value
				and std::is_nothrow_destructible<std::exception_ptr>::value);
		virtual worker & self()noexcept;
		virtual worker const & self()const noexcept;
		virtual void work()noexcept = 0;
		std::thread & thread()noexcept;
		std::thread const & thread()const noexcept;
		std::exception_ptr const & have_error()const noexcept;
		/*
		将异常以 std::exception_ptr 的形式存储于基类中
		*/
		void raise_error(std::exception_ptr const &);
		/*
		将存储在基类中的异常抛出
		*/
		[[noreturn]] void release_error()noexcept(false);
	};
}