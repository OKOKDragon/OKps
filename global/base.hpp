#pragma once

#include <thread>
#include <exception>
#include <type_traits>
#include <string>
#include <random>
#include <memory>
#include <map>
#include <mutex>

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
	此基类的对象构造时设为有效对象，被移动后会标识为无效对象。
	适用于不能弃置移动构造函数，但是又要禁止使用 被移动后的对象 的情形。
	例如，有函数 T f()，需要在f的函数体内构造T类的对象t并返回，为了执行c++标准中的复制消除优化，类T需要有移动构造函数，
	但是由于某种原因，假设 T类对象t1、t2，不能出现 {t1 = std::move(t2);} 之后又访问 t1 的情况，
	则可以让类T继承此基类。
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
	此类和空基类一样，但专用做 处理函数类 的参数
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
	其中h.handle(p)相当于调用回调函数
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
	引用 marked 类的包装类
	此类的每个对象都是对一个 marked 对象的引用。
	如果 marked 对象 m 被销毁，则引用 m 的所有此类对象自动无效化
	*/
	class reference;
	/*
	拥有唯一标记的基类
	*/
	class marked
	{
		friend class reference;
	protected:
		/*
		标记类
		*/
		class marker_type final
		{
		private:
			marked * MEMBER_owner;
		public:
			marker_type() = delete;
			marker_type(marked *)noexcept;
			~marker_type()noexcept;
			marker_type(marker_type const &) = delete;
			marker_type(marker_type &&) = delete;
			void operator =(marker_type const &) = delete;
			void operator =(marker_type &&) = delete;
			marked & owner()noexcept;
			marked const & owner()const noexcept;
		};
	private:
		std::shared_ptr<marker_type> MEMBER_marker;
	protected:
		std::shared_ptr<marker_type> const & marker()const noexcept;

		marked()
			noexcept(noexcept(std::make_shared<marker_type>(std::declval<marked *>())));
		virtual ~marked()
			noexcept(std::is_nothrow_destructible_v<std::shared_ptr<marker_type>>);
		marked(marked const &)
			noexcept(noexcept(std::make_shared<marker_type>(std::declval<marked *>())));
		marked(marked &&)
			noexcept(noexcept(std::make_shared<marker_type>(std::declval<marked *>())));
		//什么都不做
		virtual void operator =(marked const &) noexcept;
		//什么都不做
		virtual void operator =(marked &&) noexcept;
		virtual marked & self()noexcept;
		virtual marked const & self()const noexcept;
	};

	class reference
	{
	private:
		std::weak_ptr<marked::marker_type> MEMBER_marker;
	protected:
		reference() = delete;
		reference(std::shared_ptr<marked::marker_type> const &)
			noexcept(noexcept(std::weak_ptr<marked::marker_type>(std::declval<std::shared_ptr<marked::marker_type> const &>())));
		virtual ~reference()
			noexcept(std::is_nothrow_destructible_v<std::weak_ptr<marked::marker_type>>);
		reference(reference const &)
			noexcept(std::is_nothrow_copy_constructible_v<std::weak_ptr<marked::marker_type>>);
		virtual void operator =(reference const &)
			noexcept(std::is_nothrow_copy_assignable_v<std::weak_ptr<marked::marker_type>>);
		reference(reference &&)
			noexcept(std::is_nothrow_move_constructible_v<std::weak_ptr<marked::marker_type>>);
		virtual void operator =(reference &&)
			noexcept(std::is_nothrow_move_assignable_v<std::weak_ptr<marked::marker_type>>);
		virtual reference & self()noexcept;
		virtual reference const & self()const noexcept;

		bool is_valid()const
			noexcept(noexcept(std::declval<std::weak_ptr<marked::marker_type>>().expired()));
		marked & get();
		marked const & get()const;
	};


	/*
	此基类要求子类实现函数self_copy，其功能为复制子类对象自身，并包装到指向基类的智能指针中返回。
	这种做法解决的问题是，
	已知一个基类指针 p 指向子类对象 o，但在编译期不知道对象 o 的真正类型，如何正确复制对象 o。
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
	工作线程基类

	基类管理一个线程，该线程的线程函数是work函数，work函数是纯虚函数，必须由子类实现。
	work函数禁止抛出异常，如果需要使用异常，则使用此基类提供的函数，将异常存储在基类中。
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


		std::exception_ptr const & have_error()const noexcept;
		/*
		将异常以 std::exception_ptr 的形式存储于基类中
		*/
		void raise_error(std::exception_ptr const &);
		/*
		将存储在基类中的异常抛出
		*/
		[[noreturn]] void release_error()noexcept(false);
	public:
		/*
		将底层的线程对象绑定到work函数，并执行
		*/
		void execute();
		/*
		等待底层的线程对象加入主线程
		*/
		void join();
		/*
		获取底层线程的 id
		*/
		std::thread::id id()const
			noexcept(noexcept(std::declval<std::thread>().get_id()));
	};


}