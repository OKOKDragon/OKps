#pragma once

#include <thread>
#include <exception>
#include <type_traits>
#include <string>
#include <memory>

/*
此文件在命名空间 OKps::base 定义多种通用的基类

基类都有self虚函数，返回值是到基类对象自身的引用。
要求所有派生类都重写self函数，返回到派生类对象自身的引用，因为有些基类的函数中会虚调用派生类的self函数。

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
		blank(blank const &) = delete;
		/*
		此析构函数本身不会抛出异常
		不声明为noexcept的原因是便于派生类自由选择其析构函数是否声明为noexcept
		*/
		virtual ~blank();
		blank(blank &&) = delete;
		void operator =(blank &&) = delete;
		void operator =(blank const &) = delete;
		virtual blank & self()noexcept;
		virtual blank const & self()const noexcept;

	};

	template<bool error_holder>
	class handler;

	/*
	处理函数基类
	*/
	template<>
	class handler<false>
	{
	public:

		handler()noexcept;
		handler(handler const &) = delete;
		/*
		此析构函数本身不会抛出异常
		不声明为noexcept的原因是便于派生类自由选择其析构函数是否声明为noexcept
		*/
		virtual ~handler();
		handler(handler &&) = delete;
		void operator =(handler &&) = delete;
		void operator =(handler const &) = delete;
		virtual handler & self()noexcept;
		virtual handler const & self()const noexcept;

		//仿函数
		virtual void operator ()() = 0;

	};

	/*
	处理函数基类
	不直接抛出异常，而是将异常存储在此类中，在需要时才抛出
	*/
	template<>
	class handler<true>
	{
	private:
		std::exception_ptr MEMBER_error_holder;
	public:
		handler()
			noexcept(std::is_nothrow_default_constructible<std::exception_ptr>::value);
		handler(handler const &) = delete;
		void operator =(handler const &) = delete;
		handler(handler &&)
			noexcept(std::is_nothrow_copy_constructible<std::exception_ptr>::value
			and std::is_nothrow_default_constructible<std::exception_ptr>::value
			and std::is_nothrow_copy_assignable<std::exception_ptr>::value);
		void operator =(handler &&)
			noexcept(std::is_nothrow_copy_assignable<std::exception_ptr>::value
			and std::is_nothrow_default_constructible<std::exception_ptr>::value);
		virtual ~handler()noexcept(false);

		virtual handler & self()noexcept;
		virtual handler const & self()const noexcept;

		/*
		此函数禁止直接抛出异常，如果需要使用异常，则使用handler基类提供的接口
		*/
		virtual void operator ()() noexcept = 0;
	public:
		/*
		检查handler基类目前是否有异常
		*/
		bool have_error()const noexcept;
	protected:
		/*
		在此基类中存储一个新的异常 error
		如果此基类中还存储着一个旧的异常未处理，则此函数什么都不做，并抛出一个异常（该异常只是报错，不是先前未处理的那个异常）

		c++标准没有规定std::exception_ptr的实现方式，但规定了它在RAII方面的行为如同std::shared_ptr
		故对于std::exception_ptr持有的异常对象的生命周期的管理，应该也如同std::shared_ptr
		*/
		void raise_error(std::exception_ptr const & error);

		void raise_error(std::exception_ptr && error);

	public:
		/*
		释放handler基类持有的异常，并抛出该异常
		*/
		[[noreturn]]
		void release_error()noexcept(false);
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
			void change_owner(marked * const)noexcept;
		};

		std::shared_ptr<marker_type> MEMBER_marker;

		std::shared_ptr<marker_type> const & marker()const noexcept;
	public:
		marked()
			noexcept(std::is_nothrow_default_constructible_v<std::shared_ptr<marker_type>>
				and noexcept(std::make_shared<marker_type>(std::declval<marked *>())));
		virtual ~marked()noexcept(false);
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
	};

	class reference
	{
	private:
		std::weak_ptr<marked::marker_type> MEMBER_marker;
	public:
		reference() = delete;
		reference(marked const &)
			noexcept(noexcept(std::weak_ptr<marked::marker_type>(std::declval<std::shared_ptr<marked::marker_type> const &>())));
		virtual ~reference()noexcept(false);
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

		bool is_valid()const
			noexcept(noexcept(not std::declval<std::weak_ptr<marked::marker_type> const &>().expired()));

		marked & get();
		marked const & get()const;

	};


	/*
	此基类要求派生类实现函数self_copy，其功能为复制派生类对象自身，并包装到指向基类的智能指针中返回。
	这种做法解决的问题是，已知一个基类指针 p 指向派生类对象 o，但在编译期不确定对象 o 的真正类型，如何正确复制对象 o。
	*/
	class self_copier
	{
	public:
		self_copier()noexcept;
		self_copier(self_copier const &) = delete;
		/*
		此析构函数本身不会抛出异常
		不声明为noexcept的原因是便于派生类自由选择其析构函数是否声明为noexcept
		*/
		virtual ~self_copier();

		self_copier(self_copier &&) = delete;
		void operator =(self_copier &&) = delete;
		void operator =(self_copier const &) = delete;
		virtual self_copier & self()noexcept;
		virtual self_copier const & self()const noexcept;
		/*
		因为必然有动态内存分配，所以允许抛出异常
		*/
		virtual std::unique_ptr<self_copier> self_copy() const = 0;
	};

	/*
	此基类仅保存一个标记，指示当前对象是否有效。
	当调用此类的移动构造函数或移动赋值操作符时，将被移动的对象设置为失效。
	失效的对象不能被复制或移动，也不允许进行其他任何可能读取该失效对象所占内存的操作，否则抛出异常。
	*/
	class invalid_mover
	{
	private:
		bool MEMBER_valid;
	protected:
		//检查当前对象是否有效
		bool const & is_valid()const noexcept;
		//如果当前对象失效则抛出异常，否则什么都不做
		void check_valid()const noexcept(false);
	public:
		//默认构造的对象设为有效
		invalid_mover()noexcept;
		invalid_mover(invalid_mover const &);
		void operator =(invalid_mover const &);
		invalid_mover(invalid_mover &&);
		void operator =(invalid_mover &&);
		/*
		此析构函数本身不会抛出异常
		不声明为noexcept的原因是便于派生类自由选择其析构函数是否声明为noexcept
		*/
		virtual ~invalid_mover();
		virtual invalid_mover & self()noexcept;
		virtual invalid_mover const & self()const noexcept;
	};

	/*
	工作线程基类

	基类管理一个线程，该线程的线程函数是work函数，work函数是纯虚函数，必须由派生类实现。
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
		virtual ~worker()noexcept(false);
		virtual worker & self()noexcept;
		virtual worker const & self()const noexcept;
	protected:
		virtual void work()noexcept = 0;

	public:
		bool have_error()const noexcept;
	protected:
		/*
		将异常以 std::exception_ptr 的形式存储于基类中
		*/
		void raise_error(std::exception_ptr const &);
	public:
		/*
		将存储在基类中的异常抛出
		*/
		[[noreturn]]
		void release_error()noexcept(false);

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
			noexcept(noexcept(std::declval<std::thread const &>().get_id()));
	};

	/*
	用法与std::enable_shared_from_this一样
	要求 lower_type 可复制构造
	*/
	template<typename lower_type>
	class enable_self_copy :public self_copier
	{
	public:
		std::unique_ptr<self_copier> self_copy()const
			noexcept(noexcept(std::make_unique<lower_type>(std::declval<lower_type const &>()))
		and noexcept(dynamic_cast<lower_type const *>(std::declval<enable_self_copy const *>())))
			override
		{
			lower_type const * PTR_self = dynamic_cast<lower_type const *>(this);
			return std::make_unique<lower_type>(*PTR_self);
		}
	protected:
		enable_self_copy()noexcept
		{
		}
		enable_self_copy(enable_self_copy const &) = delete;
		enable_self_copy(enable_self_copy &&) = delete;
		void operator =(enable_self_copy const &) = delete;
		void operator =(enable_self_copy &&) = delete;
	public:
		/*
		此析构函数本身不会抛出异常
		不声明为noexcept的原因是便于派生类自由选择其析构函数是否声明为noexcept
		*/
		~enable_self_copy() override
		{
		}
		enable_self_copy & self()noexcept override
		{
			return *this;
		}
		enable_self_copy const & self()const noexcept override
		{
			return *this;
		}

	};

	/*
	满足具名要求“可基本锁定”的锁，即使用 lock() 和 unlock() 成员函数表示上锁和解锁。
	*/
	class basic_lock
	{
	private:
		//当前占用锁的线程
		std::thread::id MEMBER_thread_id;
		//当前是否已锁定
		bool MEMBER_locked_flag;
	public:
		basic_lock()
			noexcept(std::is_nothrow_default_constructible_v<std::thread::id>);
		/*
		此析构函数本身不会抛出异常
		不声明为noexcept的原因是便于派生类自由选择其析构函数是否声明为noexcept
		*/
		virtual ~basic_lock();
		basic_lock(basic_lock const &) = delete;
		basic_lock(basic_lock &&) = delete;
		void operator = (basic_lock const &) = delete;
		void operator =(basic_lock &&) = delete;
		void lock();
		void unlock()noexcept;
		/*
		返回锁的所有者线程的编号
		如果没有上锁，则其值是默认构造的std::thread::id
		*/
		std::thread::id const & owner_thread() const noexcept;
		bool const & is_locked()const noexcept;
	protected:
		/*
		由派生类实现锁定的逻辑
		允许抛出异常，但抛出异常时不能取得锁。
		*/
		virtual void lock_implement() = 0;
		/*
		由派生类实现解锁的逻辑
		不允许抛出异常
		*/
		virtual void unlock_implement() noexcept = 0;
	public:
		basic_lock & self()noexcept;
		basic_lock const & self()const noexcept;

	};
}