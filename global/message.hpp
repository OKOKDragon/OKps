#pragma once

#include <cstddef>
#include <type_traits>
#include <exception>
#include <queue>
#include <atomic>
#include <thread>
#include <memory>
#include <condition_variable>
#include <unordered_map>

namespace OKps
{
	class message final
	{
	private:

	public:
		/*
		信号处理器的基类
		*/
		class handler
		{
			friend class message;
		private:
			std::exception_ptr MEMBER_exception_holder;
		protected:
			handler()
				noexcept(std::is_nothrow_default_constructible<std::exception_ptr>::value);
			handler(handler const &)
				noexcept(std::is_nothrow_copy_constructible<std::exception_ptr>::value);
			virtual void operator =(handler const &)
				noexcept(std::is_nothrow_copy_assignable<std::exception_ptr>::value);
			handler(handler &&)
				noexcept(std::is_nothrow_move_constructible<std::exception_ptr>::value);
			virtual void operator =(handler &&)
				noexcept(std::is_nothrow_move_assignable<std::exception_ptr>::value);
		public:
			virtual ~handler()
				noexcept(std::is_nothrow_destructible<std::exception_ptr>::value);
		protected:
			/*
			引发信号时，信号处理器会调用此函数
			此函数禁止直接抛出异常，如果需要使用异常，则使用handler基类提供的接口
			*/
			virtual void handle() noexcept = 0;

			/*
			检查handler基类目前是否有异常
			*/
			std::exception_ptr const & have_exception()const noexcept;
			/*
			用新的异常替换掉handler基类目前保有的异常

			c++标准没有规定std::exception_ptr的实现方式，但规定了它在RAII方面的行为如同std::shared_ptr
			故对于std::exception_ptr持有的异常对象的生命周期的管理，应该也如同std::shared_ptr
			*/
			void raise_exception(std::exception_ptr const &);
			/*
			释放handler基类持有的异常，并抛出该异常
			*/
			[[noreturn]]
			void release_exception()noexcept(false);
		};
	private:

		using TYPE_handler_pool = std::unordered_map<std::uintmax_t, std::unique_ptr<handler>>;
		TYPE_handler_pool MEMBER_handlers;
		std::unique_lock<std::mutex> MEMBER_lock;
		std::mutex MEMBER_mutex;
		std::atomic<bool> MEMBER_not_waiting;
		std::condition_variable MEMBER_waiter;
		std::thread MEMBER_peeker;
		void peek()noexcept;
		std::queue<std::uintmax_t> MEMBER_signals;
		std::atomic<bool> MEMBER_done;
	public:
		/*
		生成随机的信号值signal，并将handler注册为该信号的处理函数
		返回生成的信号signal
		*/
		std::uintmax_t regist(std::unique_ptr<handler> &&);
		/*
		如果signal未注册，则将handler函数注册为信号signal的处理函数
		如果signal已注册，则将其处理函数修改为handler
		注意，regist函数和trigger函数不保证顺序，也就是说，
		如果注册信号signal和引发信号signal在不同的线程，可能会抛出“信号signal未注册”的异常
		*/
		void regist(std::uintmax_t const signal, std::unique_ptr<handler> &&);
		/*
		引发信号signal
		信号signal的处理函数会在本类管理的一个线程执行，而不会在调用trigger函数的线程执行
		处理函数执行的顺序是按照信号引发的顺序
		*/
		void trigger(std::uintmax_t const signal);
		message();
		~message()
			noexcept(noexcept(std::declval<std::thread>().join()));
		message(message const &) = delete;
		message(message &&) = delete;
		void operator =(message const &) = delete;
		void operator =(message &&) = delete;
	};
}