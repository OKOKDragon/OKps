#pragma once

#include <thread>
#include <exception>
#include <type_traits>
#include <string>
#include <random>
#include <memory>

/*
此文件在命名空间 OKps::base 定义多种通用的基类

基类都有self函数，返回值是到基类对象自身的引用，以方便在派生类的函数中调用基类的重载运算符。
例如，对于基类base_type和它的派生类upper_type，可以在upper_type的赋值运算符中调用
{this->base_type::self() = origin.base_type::self();}，
其中origin是赋值运算符的形参。
要求所有派生类都重写self函数，并且功能与上面描述的一样，因为有些基类的非虚函数中会虚调用派生类的self函数。

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

	此文件定义的其他基类都虚继承自空基类，故使用者在继承这些基类时，须在最终派生类的构造函数中显式调用空基类的构造函数，
	以避免潜在的问题。
	这样做是为了让所有派生类都能由空基类的指针访问，方便将不同派生类的对象放入同一个容器或数组。
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

		bool operator <(blank const &)const noexcept;
	};

	/*
	此基类的对象构造时设为有效对象，被移动后会标识为无效对象。
	适用于不能弃置移动构造函数，但是又要禁止使用 被移动后的对象 的情形。
	例如，有函数 T f()，需要在f的函数体内构造T类的对象t并返回，为了执行c++标准中的复制消除优化，类T需要有移动构造函数，
	但是由于某种原因，假设 T类对象t1、t2，不能出现 {t1 = std::move(t2);} 之后又访问 t1 的情况，
	则可以让类T继承此基类。
	*/
	class move_invalid :virtual public blank
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
		void operator =(move_invalid const &)noexcept;
		move_invalid(move_invalid &&)noexcept;
		void operator =(move_invalid &&)noexcept;
		virtual ~move_invalid()noexcept override;
		virtual move_invalid & self()noexcept override;
		virtual move_invalid const & self()const noexcept override;
		bool operator <(move_invalid const &)const noexcept;
	};
	/*
	具有一个无符号整数作为编号的基类
	*/
	class numbered :virtual public  blank
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
		void operator =(numbered const &)noexcept;
		numbered(numbered &&)noexcept = delete;
		void operator =(numbered &&)noexcept = delete;
		virtual ~numbered()noexcept override;
		std::uintmax_t & id()noexcept;
		std::uintmax_t const & id()const noexcept;
		virtual numbered & self()noexcept override;
		virtual numbered const & self()const noexcept override;
		bool operator <(numbered const &)const noexcept;
	};
	/*
	具有一个字符串作为标识的基类
	*/
	class named :virtual public blank
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
		void operator =(named const &)
			noexcept(std::is_nothrow_copy_assignable<std::string>::value);
		named(named &&)
			noexcept(std::is_nothrow_move_constructible<std::string>::value);
		void operator =(named &&)
			noexcept(std::is_nothrow_move_assignable<std::string>::value);
		virtual ~named()
			noexcept(std::is_nothrow_destructible<std::string>::value)
			override;
		std::string const & name()const noexcept;
		std::string & name()noexcept;
		virtual named & self()noexcept override;
		virtual named const & self()const noexcept override;

		bool operator <(named const &)const noexcept;
	};


	/*
	处理函数基类
	可以用作回调函数的标准类型，而不用 lambda 配合模板的形式
	例如，想要在函数 f 中使用回调函数 h，则可以写成
	result_type f(handler& h, blank& p);
	其中h.handle(p)相当于调用回调函数
	*/
	class handler :virtual public blank
	{
	public:

		handler()noexcept;
		handler(handler const &) noexcept;
		virtual ~handler()noexcept override;
		handler(handler &&) noexcept;
		void operator =(handler &&)noexcept;
		void operator =(handler const &)noexcept;
		virtual handler & self()noexcept override;
		virtual handler const & self()const noexcept override;
		/*
		用智能指针的形式接收参数，是因为有可能想要用空指针代表无参数；
		但是又不希望不小心写出错误的 delete 或其他错误的指针操作，所以避免使用裸指针。
		*/
		virtual void handle(std::unique_ptr<blank> const &) = 0;

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
	class marked :virtual public blank
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
		void operator =(marked const &) noexcept;
		//什么都不做
		void operator =(marked &&) noexcept;
		virtual marked & self()noexcept override;
		virtual marked const & self()const noexcept override;
		bool operator <(marked const &)const noexcept;
	};

	class reference :virtual public blank
	{
	private:
		std::weak_ptr<marked::marker_type> MEMBER_marker;
	public:
		reference() = delete;
		reference(marked const &)
			noexcept(noexcept(std::weak_ptr<marked::marker_type>(std::declval<std::shared_ptr<marked::marker_type> const &>())));
		virtual ~reference()
			noexcept(std::is_nothrow_destructible_v<std::weak_ptr<marked::marker_type>>)
			override;
		reference(reference const &)
			noexcept(std::is_nothrow_copy_constructible_v<std::weak_ptr<marked::marker_type>>);
		void operator =(reference const &)
			noexcept(std::is_nothrow_copy_assignable_v<std::weak_ptr<marked::marker_type>>);
		reference(reference &&)
			noexcept(std::is_nothrow_move_constructible_v<std::weak_ptr<marked::marker_type>>);
		void operator =(reference &&)
			noexcept(std::is_nothrow_move_assignable_v<std::weak_ptr<marked::marker_type>>);
		virtual reference & self()noexcept override;
		virtual reference const & self()const noexcept override;

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
	class self_copier :virtual public blank
	{
	public:
		self_copier()noexcept;
		self_copier(self_copier const &)noexcept;

		virtual ~self_copier()noexcept override;

		self_copier(self_copier &&)noexcept;
		void operator =(self_copier &&)noexcept;
		void operator =(self_copier const &)noexcept;
		virtual self_copier & self()noexcept override;
		virtual self_copier const & self()const noexcept override;
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
	class worker :virtual public blank
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
				and std::is_nothrow_destructible<std::exception_ptr>::value)
			override;
		virtual worker & self()noexcept override;
		virtual worker const & self()const noexcept override;
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