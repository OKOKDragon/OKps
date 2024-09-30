#pragma once

#include <thread>
#include <exception>
#include <type_traits>
#include <string>
#include <random>
#include <memory>
#include <map>
#include <mutex>
#include <optional>
#include <memory_resource>

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
	public:
		blank()noexcept;
		blank(blank const &) noexcept;
		virtual ~blank()noexcept;
		blank(blank &&) noexcept;
		virtual void operator =(blank &&) noexcept;
		virtual void operator =(blank const &) noexcept;
		virtual blank & self()noexcept;
		virtual blank const & self()const noexcept;
		/*
		比较两对象的地址
		用于标准库容器的排序
		*/
		virtual bool operator <(blank const &)const noexcept;
	};
	/*
	此基类的对象构造时设为有效对象，被移动后会标识为无效对象。
	适用于不能弃置移动构造函数，但是又要禁止使用 被移动后的对象 的情形。
	例如，有函数 T f()，需要在f的函数体内构造T类的对象t并返回，为了执行c++标准中的复制消除优化，类T需要有移动构造函数，
	但是由于某种原因，假设 T类对象t1、t2，不能出现 {t1 = std::move(t2);} 之后又访问 t1 的情况，
	则可以让类T继承此基类。
	*/
	class move_invalid :public blank
	{
	private:
		bool MEMBER_valid;
	public:
		bool const & is_valid()const noexcept;
	protected:
		/*
		检查当前对象是否有效，
		如果有效则什么都不做，如果无效则抛出异常
		*/
		void raise_invalid_error()const noexcept(false);
	public:
		move_invalid()noexcept;
		move_invalid(move_invalid const &)noexcept;
		virtual void operator =(move_invalid const &)noexcept;
		move_invalid(move_invalid &&)noexcept;
		virtual void operator =(move_invalid &&)noexcept;
		virtual ~move_invalid()noexcept override;
		virtual move_invalid & self()noexcept override;
		virtual move_invalid const & self()const noexcept override;
		virtual bool operator <(move_invalid const &)const noexcept;
	};
	/*
	具有一个无符号整数作为编号的基类
	*/
	class numbered :public blank
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
	public:
		numbered(std::uintmax_t const id = 0)noexcept;
		numbered(numbered const &)noexcept;
		virtual void operator =(numbered const &)noexcept;
		numbered(numbered &&)noexcept = delete;
		virtual void operator =(numbered &&)noexcept = delete;
		virtual ~numbered()noexcept override;
		std::uintmax_t & id()noexcept;
		std::uintmax_t const & id()const noexcept;
		virtual numbered & self()noexcept override;
		virtual numbered const & self()const noexcept override;
		virtual bool operator <(numbered const &)const noexcept;
	};
	/*
	具有一个字符串作为标识的基类
	*/
	class named :public blank
	{
	private:
		std::string MEMBER_name;
	public:
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
			noexcept(std::is_nothrow_destructible<std::string>::value)
			override;
		std::string const & name()const noexcept;
		std::string & name()noexcept;
		virtual named & self()noexcept override;
		virtual named const & self()const noexcept override;

		virtual bool operator <(named const &)const noexcept;
	};


	/*
	处理函数基类
	可以用作回调函数的标准类型，而不用 lambda 配合模板的形式
	例如，想要在函数 f 中使用回调函数 h，则可以写成
	result_type f(handler& h, parameter& p);
	其中h.handle(p)相当于调用回调函数
	*/
	class handler :public blank
	{
	public:

		handler()noexcept;
		handler(handler const &) noexcept;
		virtual ~handler()noexcept override;
		handler(handler &&) noexcept;
		virtual void operator =(handler &&)noexcept;
		virtual void operator =(handler const &)noexcept;
		virtual handler & self()noexcept override;
		virtual handler const & self()const noexcept override;
		/*
		用智能指针的形式接收参数，是因为有可能想要用空指针代表无参数；
		但是又不希望不小心写出错误的 delete 或其他错误的指针操作，所以避免使用裸指针。
		*/
		virtual void handle(std::unique_ptr<blank> const &) = 0;

		virtual bool operator <(handler const &)const noexcept;
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
	class marked :public blank
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
		};

		std::shared_ptr<marker_type> MEMBER_marker;

		std::shared_ptr<marker_type> const & marker()const noexcept;
	public:
		marked()
			noexcept(std::is_nothrow_default_constructible_v<std::shared_ptr<marker_type>>
				and noexcept(std::make_shared<marker_type>(std::declval<marked *>())));
		virtual ~marked()
			noexcept(std::is_nothrow_destructible_v<std::shared_ptr<marker_type>>)
			override;
		marked(marked const &)
			noexcept(std::is_nothrow_default_constructible_v<std::shared_ptr<marker_type>>
				and noexcept(std::make_shared<marker_type>(std::declval<marked *>())));
		marked(marked &&)
			noexcept(std::is_nothrow_default_constructible_v<std::shared_ptr<marker_type>>
				and noexcept(std::make_shared<marker_type>(std::declval<marked *>())));
		//什么都不做
		virtual void operator =(marked const &) noexcept;
		//什么都不做
		virtual void operator =(marked &&) noexcept;
		virtual marked & self()noexcept override;
		virtual marked const & self()const noexcept override;
		virtual bool operator <(marked const &)const noexcept;
	};

	class reference :public blank
	{
	private:
		std::weak_ptr<marked::marker_type> MEMBER_marker;
	public:
		reference() = delete;
	private:
		reference(std::shared_ptr<marked::marker_type> const &)
			noexcept(noexcept(std::weak_ptr<marked::marker_type>(std::declval<std::shared_ptr<marked::marker_type> const &>())));
	public:
		reference(marked const &)
			noexcept(noexcept(reference(std::declval<std::shared_ptr<marked::marker_type> const &>())));
		virtual ~reference()
			noexcept(std::is_nothrow_destructible_v<std::weak_ptr<marked::marker_type>>)
			override;
		reference(reference const &)
			noexcept(std::is_nothrow_copy_constructible_v<std::weak_ptr<marked::marker_type>>);
		virtual void operator =(reference const &)
			noexcept(std::is_nothrow_copy_assignable_v<std::weak_ptr<marked::marker_type>>);
		reference(reference &&)
			noexcept(std::is_nothrow_move_constructible_v<std::weak_ptr<marked::marker_type>>);
		virtual void operator =(reference &&)
			noexcept(std::is_nothrow_move_assignable_v<std::weak_ptr<marked::marker_type>>);
		virtual reference & self()noexcept override;
		virtual reference const & self()const noexcept override;

		bool is_valid()const
			noexcept(noexcept(not std::declval<std::weak_ptr<marked::marker_type>>().expired()));
		marked & get();
		marked const & get()const;
		virtual bool operator <(reference const &)const noexcept;
	};


	/*
	此基类要求子类实现函数self_copy，其功能为复制子类对象自身，并包装到指向基类的智能指针中返回。
	这种做法解决的问题是，
	已知一个基类指针 p 指向子类对象 o，但在编译期不知道对象 o 的真正类型，如何正确复制对象 o。
	*/
	class self_copier :public blank
	{
	public:
		self_copier()noexcept;
		self_copier(self_copier const &)noexcept;

		virtual ~self_copier()noexcept override;

		self_copier(self_copier &&)noexcept;
		virtual void operator =(self_copier &&)noexcept;
		virtual void operator =(self_copier const &)noexcept;
		virtual self_copier & self()noexcept override;
		virtual self_copier const & self()const noexcept override;
		/*
		因为必然有动态内存分配，所以允许抛出异常
		*/
		virtual std::unique_ptr<self_copier> self_copy() const = 0;
		virtual bool operator <(self_copier const &)const noexcept;
	};
	/*
	工作线程基类

	基类管理一个线程，该线程的线程函数是work函数，work函数是纯虚函数，必须由子类实现。
	work函数禁止抛出异常，如果需要使用异常，则使用此基类提供的函数，将异常存储在基类中。
	*/
	class worker :public blank
	{
	private:
		std::thread MEMBER_thread;
		std::exception_ptr MEMBER_error;
	public:
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
				and std::is_nothrow_destructible<std::exception_ptr>::value)
			override;
		virtual worker & self()noexcept override;
		virtual worker const & self()const noexcept override;
	protected:
		virtual void work()noexcept = 0;


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
		virtual bool operator <(worker const &)const noexcept;
	};

	/*
	此基类重载了 new 和 delete 操作符，使用标准库中的内存池进行内存分配。
	注意 msvc 的标准库实现中，std::shared_ptr 使用全局 new 和 delete 操作符进行内存管理，故无法使用此类的内存池。

	严格要求继承此基类的派生类，其对齐要求必须都是 alignas(std::max_align_t)，否则将引发未定义行为。
	禁止派生类重载 new 和 delete 操作符。
	*/
	class alignas(std::max_align_t) pool_memory
	{
	private:
		class allocator final
		{
		private:
			std::optional<std::pmr::polymorphic_allocator<std::byte>> MEMBER_allocator;
			std::pmr::synchronized_pool_resource MEMBER_pool;

		public:
			allocator()
				noexcept(noexcept(std::declval<std::optional<std::pmr::polymorphic_allocator<std::byte>>>().emplace(std::declval<std::pmr::synchronized_pool_resource *>()))
				and noexcept(std::pmr::synchronized_pool_resource(std::pmr::pool_options(), std::pmr::new_delete_resource())));
			~allocator()
				noexcept(std::is_nothrow_destructible_v<std::optional<std::pmr::polymorphic_allocator<std::byte>>>
				and std::is_nothrow_destructible_v<std::pmr::synchronized_pool_resource>);
			std::pmr::polymorphic_allocator<std::byte> & get()noexcept;

			allocator(allocator const &) = delete;
			allocator(allocator &&) = delete;
			void operator =(allocator const &) = delete;
			void operator =(allocator &&) = delete;
		};

		static allocator MEMBER_allocator;

	public:
		void * operator new(std::size_t const size);
		void * operator new[](std::size_t const size);
		void operator delete[](void * const block, std::size_t const size);
		void operator delete(void * const block, std::size_t const size);

		pool_memory()noexcept;
		pool_memory(pool_memory const &)noexcept;
		pool_memory(pool_memory &&)noexcept;
		virtual ~pool_memory()noexcept;
		virtual void operator =(pool_memory const &)noexcept;
		virtual void operator =(pool_memory &&)noexcept;
		virtual bool operator <(pool_memory const &)const noexcept;
	private:
		class deleter_type final
		{
			friend class pool_memory;
		private:
			std::size_t MEMBER_size;

		public:
			deleter_type(std::size_t const size = 0);


			deleter_type(deleter_type const & origin)noexcept;
			deleter_type(deleter_type &&) noexcept;
			void operator =(deleter_type const & origin)noexcept;
			void operator =(deleter_type &&) noexcept;
			~deleter_type()noexcept;

			void operator()(void * ptr) const noexcept;
		};
	public:
		/*
		从内存池中分配一段 size 字节的内存，未初始化，以作任何用途。
		用 std::unique_ptr 包装，以防止该内存被以错误的方式释放。
		*/
		static std::unique_ptr<std::byte[], deleter_type> allocate(std::size_t const size);

	};
}