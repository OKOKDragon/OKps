#pragma once

#include <thread>
#include <exception>
#include <type_traits>
#include <string>
#include <random>
#include <memory>

/*
此文件在命名空间 OKps::base 定义多种通用的基类

基类都有self虚函数，返回值是到基类对象自身的引用。
要求所有派生类都重写self函数，返回到派生类对象自身的引用，因为有些基类的函数中会虚调用派生类的self函数。

基类都有虚 & 操作符，返回值是基类的this指针。
要求所有派生类都重写 & 操作符，返回派生类的this指针。

基类都有 < 运算符，功能是比较对象的地址，用于 std::set 等有序容器的排序。比较时会虚调用self函数。

注意，有多层继承时须手动在最终派生类的构造函数中调用最底层虚基类的构造函数，否则c++只会调用最底层虚基类的默认构造函数。
例如，设基类A被类B虚继承，类B被类C继承，如果在类B的复制构造函数中，调用了类A的复制构造函数；在类C的复制构造函数中，只调用了类B的复制构造函数，而没有显式调用类A的复制构造函数，
则类C的复制构造函数会调用类A的默认构造函数，和类B的复制构造函数，而在调用类B的复制构造函数的过程中不会执行类A的复制构造函数。
*/

namespace OKps::base
{

	/*
	空基类
	它的赋值运算符什么都不做。
	*/
	class blank
	{
	public:
		blank()noexcept;
		blank(blank const &) noexcept;
		virtual ~blank()noexcept;
		blank(blank &&) noexcept;
		void operator =(blank &&) noexcept;
		void operator =(blank const &) noexcept;
		virtual blank & self()noexcept;
		virtual blank const & self()const noexcept;
		virtual blank * operator &()noexcept;
		virtual blank const * operator &()const noexcept;
		bool operator <(blank const &)const noexcept;
	};

	/*
	处理函数基类
	可以用作回调函数的标准类型，而不用 lambda 配合模板的形式
	例如，想要在函数 f 中使用回调函数 h，则可以写成
	result_type f(handler& h, blank& p);
	则调用h(p)相当于回调函数
	*/
	class handler
	{
	public:

		handler()noexcept;
		handler(handler const &) noexcept;
		virtual ~handler()noexcept;
		handler(handler &&) noexcept;
		void operator =(handler &&)noexcept;
		void operator =(handler const &)noexcept;
		virtual handler & self()noexcept;
		virtual handler const & self()const noexcept;
		virtual handler * operator &()noexcept;
		virtual handler const * operator &()const noexcept;

		//仿函数
		virtual void operator ()(blank &) = 0;

		bool operator <(handler const &)const noexcept;
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
	private:
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
			void change_owner(marked *)noexcept;
		};

		std::shared_ptr<marker_type> MEMBER_marker;

		std::shared_ptr<marker_type> const & marker()const noexcept;
	public:
		marked()
			noexcept(std::is_nothrow_default_constructible_v<std::shared_ptr<marker_type>>
				and noexcept(std::make_shared<marker_type>(std::declval<marked *>())));
		virtual ~marked()
			noexcept(std::is_nothrow_destructible_v<std::shared_ptr<marker_type>>);
		marked(marked const &)
			noexcept(std::is_nothrow_default_constructible_v<std::shared_ptr<marker_type>>
				and noexcept(std::make_shared<marker_type>(std::declval<marked *>())));
		marked(marked &&)
			noexcept(std::is_nothrow_move_constructible_v<std::shared_ptr<marker_type>>
				and noexcept(std::make_shared<marker_type>(std::declval<marked *>())));

		void operator =(marked const &) noexcept;

		void operator =(marked &&)
			noexcept(std::is_nothrow_move_assignable_v<std::shared_ptr<marker_type>>
and noexcept(std::make_shared<marker_type>(std::declval<marked *>())));
		virtual marked & self()noexcept;
		virtual marked const & self()const noexcept;
		virtual marked * operator &()noexcept;
		virtual marked const * operator &()const noexcept;
		bool operator <(marked const &)const noexcept;
	};

	class reference
	{
	private:
		std::weak_ptr<marked::marker_type> MEMBER_marker;
	public:
		reference() = delete;
		reference(marked const &)
			noexcept(noexcept(std::weak_ptr<marked::marker_type>(std::declval<std::shared_ptr<marked::marker_type> const &>())));
		virtual ~reference()
			noexcept(std::is_nothrow_destructible_v<std::weak_ptr<marked::marker_type>>);
		reference(reference const &)
			noexcept(std::is_nothrow_copy_constructible_v<std::weak_ptr<marked::marker_type>>);
		void operator =(reference const &)
			noexcept(std::is_nothrow_copy_assignable_v<std::weak_ptr<marked::marker_type>>);
		reference(reference &&)
			noexcept(std::is_nothrow_move_constructible_v<std::weak_ptr<marked::marker_type>>);
		void operator =(reference &&)
			noexcept(std::is_nothrow_move_assignable_v<std::weak_ptr<marked::marker_type>>);
		virtual reference & self()noexcept;
		virtual reference const & self()const noexcept;
		virtual reference * operator &()noexcept;
		virtual reference const * operator &()const noexcept;

		bool is_valid()const
			noexcept(noexcept(not std::declval<std::weak_ptr<marked::marker_type>>().expired()));

		marked & get();
		marked const & get()const;

		bool operator <(reference const &)const noexcept;
	};


	/*
	此基类要求子类实现函数self_copy，其功能为复制子类对象自身，并包装到指向基类的智能指针中返回。
	这种做法解决的问题是，
	已知一个基类指针 p 指向子类对象 o，但在编译期不知道对象 o 的真正类型，如何正确复制对象 o。
	*/
	class self_copier
	{
	public:
		self_copier()noexcept;
		self_copier(self_copier const &)noexcept;

		virtual ~self_copier()noexcept;

		self_copier(self_copier &&)noexcept;
		void operator =(self_copier &&)noexcept;
		void operator =(self_copier const &)noexcept;
		virtual self_copier & self()noexcept;
		virtual self_copier const & self()const noexcept;
		virtual self_copier * operator &()noexcept;
		virtual self_copier const * operator &()const noexcept;
		/*
		因为必然有动态内存分配，所以允许抛出异常
		*/
		virtual std::unique_ptr<self_copier> self_copy() const = 0;
		bool operator <(self_copier const &)const noexcept;
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
	public:
		worker()
			noexcept(std::is_nothrow_default_constructible<std::thread>::value
				and std::is_nothrow_default_constructible<std::exception_ptr>::value);
		worker(worker const &) = delete;
		void operator =(worker const &) = delete;
		worker(worker &&)
			noexcept(std::is_nothrow_move_constructible<std::thread>::value
				and std::is_nothrow_copy_constructible<std::exception_ptr>::value);
		void operator =(worker &&)
			noexcept(std::is_nothrow_move_assignable<std::thread>::value
				and std::is_nothrow_copy_assignable<std::exception_ptr>::value);
		virtual ~worker()
			noexcept(std::is_nothrow_destructible<std::thread>::value
				and std::is_nothrow_destructible<std::exception_ptr>::value);
		virtual worker & self()noexcept;
		virtual worker const & self()const noexcept;
		virtual worker * operator &()noexcept;
		virtual worker const * operator &()const noexcept;
	protected:
		virtual void work()noexcept = 0;

	public:
		std::exception_ptr const & have_error()const noexcept;
	protected:
		/*
		将异常以 std::exception_ptr 的形式存储于基类中
		*/
		void raise_error(std::exception_ptr const &);
	public:
		/*
		将存储在基类中的异常抛出
		*/
		[[noreturn]] void release_error()noexcept(false);

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
		bool operator <(worker const &)const noexcept;
	};

}