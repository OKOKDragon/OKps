#pragma once

#include <cstddef>
#include <exception>
#include <queue>
#include <condition_variable>
#include <unordered_map>

#include ".\lock.hpp"
#include ".\base.hpp"

namespace OKps
{
	class message final
	{

	private:

		using TYPE_handler_pool = std::unordered_map<std::uintmax_t, std::shared_ptr<base::handler<true>>>;
		TYPE_handler_pool MEMBER_handlers;
		using TYPE_mutex = lock_proxy<std::binary_semaphore>;
		std::unique_lock<TYPE_mutex> MEMBER_lock;
		TYPE_mutex MEMBER_mutex;
		std::atomic<bool> MEMBER_not_waiting;
		std::condition_variable_any MEMBER_waiter;
		std::thread MEMBER_peeker;
		void peek()noexcept;

		class signal_parameter final
		{
		private:
			std::uintmax_t MEMBER_signal;
			std::shared_ptr<base::blank> MEMBER_parameter;
		public:
			/*
			默认parameter不是空指针
			若违反，则会引发未定义行为。
			*/
			signal_parameter(std::uintmax_t const signal, std::shared_ptr<base::blank> const & parameter)
				noexcept(std::is_nothrow_copy_constructible_v<std::shared_ptr<base::blank>>);
			signal_parameter(signal_parameter const &)
				noexcept(std::is_nothrow_copy_constructible_v<std::shared_ptr<base::blank>>);
			void operator =(signal_parameter const &)
				noexcept(std::is_nothrow_copy_assignable_v<std::shared_ptr<base::blank>>);
			signal_parameter(signal_parameter &&)
				noexcept(std::is_nothrow_move_constructible_v<std::shared_ptr<base::blank>>);
			void operator =(signal_parameter &&)
				noexcept(std::is_nothrow_move_assignable_v<std::shared_ptr<base::blank>>);
			~signal_parameter()
				noexcept(std::is_nothrow_destructible_v<std::shared_ptr<base::blank>>);
			std::uintmax_t & signal()noexcept;
			std::uintmax_t const & signal()const noexcept;
			base::blank & parameter()noexcept;
			base::blank const & parameter()const noexcept;
		};

		std::queue<signal_parameter> MEMBER_signals;
		std::atomic<bool> MEMBER_done;
	public:
		/*
		生成随机的信号值signal，并将handler注册为该信号的处理函数
		返回生成的信号signal
		*/
		std::uintmax_t regist(std::shared_ptr<base::handler<true>> const &);
		/*
		如果signal未注册，则将handler函数注册为信号signal的处理函数
		如果signal已注册，则将其处理函数修改为handler
		注意，regist函数和trigger函数不保证顺序，也就是说，
		如果注册信号signal和引发信号signal在不同的线程，可能会抛出“信号signal未注册”的异常
		*/
		void regist(std::uintmax_t const signal, std::shared_ptr<base::handler<true>> const &);
		/*
		引发信号signal
		信号signal的处理函数会在本类管理的一个线程执行，而不会在调用trigger函数的线程执行
		处理函数执行的顺序是按照信号引发的顺序

		parameter是执行本次处理函数所调用的参数，它不能为空指针。
		*/
		void trigger(std::uintmax_t const signal, std::shared_ptr<base::blank> const & parameter);
		message();
		/*
		此类析构之前，会先处理完所有待处理的信号。
		*/
		~message()
			noexcept(noexcept(std::declval<std::thread>().join())
			and std::is_nothrow_destructible_v<TYPE_handler_pool>
			and std::is_nothrow_destructible_v<TYPE_mutex>
			and std::is_nothrow_destructible_v<std::unique_lock<TYPE_mutex>>
			and std::is_nothrow_destructible_v<std::thread>
			and std::is_nothrow_destructible_v<std::condition_variable_any>
			and std::is_nothrow_destructible_v<std::queue<std::uintmax_t>>);
		message(message const &) = delete;
		message(message &&) = delete;
		void operator =(message const &) = delete;
		void operator =(message &&) = delete;
	};
}