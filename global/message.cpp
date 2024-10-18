#include <string>
#include <random>

#include ".\message.hpp"

namespace OKps
{
	message::handler::handler()
		noexcept(std::is_nothrow_default_constructible<std::exception_ptr>::value)
		:MEMBER_exception_holder()
	{
	}
	message::handler::handler(handler const & origin)
		noexcept(std::is_nothrow_copy_constructible<std::exception_ptr>::value)
		:MEMBER_exception_holder(origin.MEMBER_exception_holder)
	{
	}
	void message::handler::operator =(handler const & origin)
		noexcept(std::is_nothrow_copy_assignable<std::exception_ptr>::value)
	{
		this->MEMBER_exception_holder = origin.MEMBER_exception_holder;
	}
	message::handler::handler(handler && origin)
		noexcept(std::is_nothrow_move_constructible<std::exception_ptr>::value)
		:MEMBER_exception_holder(std::move(origin.MEMBER_exception_holder))
	{
	}
	void message::handler::operator =(handler && origin)
		noexcept(std::is_nothrow_move_assignable<std::exception_ptr>::value)
	{
		this->MEMBER_exception_holder = std::move(origin.MEMBER_exception_holder);
	}
	message::handler::~handler()
		noexcept(std::is_nothrow_destructible<std::exception_ptr>::value)
	{
	}
	std::exception_ptr const & message::handler::have_exception()const noexcept
	{
		return this->MEMBER_exception_holder;
	}
	void message::handler::raise_exception(std::exception_ptr const & p)
	{
		if (this->MEMBER_exception_holder)
		{
			throw std::logic_error("已经存储了一个异常，尚未得到处理");
		}
		this->MEMBER_exception_holder = p;
	}
	void message::handler::release_exception()noexcept(false)
	{
		if (not this->MEMBER_exception_holder)
		{
			throw std::logic_error("没有存储异常");
		}
		auto const p = this->MEMBER_exception_holder;
		this->MEMBER_exception_holder = nullptr;
		std::rethrow_exception(p);
	}
	std::uintmax_t message::regist(std::unique_ptr<handler> && h)
	{
		if (not h)
		{
			throw std::invalid_argument("处理函数不能为空");
		}
		std::random_device seed;
		std::mt19937_64 engine(seed());
		std::uniform_int_distribution<std::uintmax_t> distribution(std::numeric_limits<std::uintmax_t>::min(), std::numeric_limits<std::uintmax_t>::max());

		std::uintmax_t number;
		bool found = false;
		while (not found)
		{
			number = distribution(engine);
			this->MEMBER_mutex.lock();
			auto finder = this->MEMBER_handlers.find(number);
			if (finder == this->MEMBER_handlers.end())
			{
				this->MEMBER_handlers.insert(std::make_pair(number, std::move(h)));
				found = true;
			}
			this->MEMBER_mutex.unlock();
		}

		return number;
	}
	void message::regist(std::uintmax_t const signal, std::unique_ptr<handler> && h)
	{
		if (not h)
		{
			throw std::invalid_argument("处理函数不能为空");
		}
		this->MEMBER_mutex.lock();
		auto finder = this->MEMBER_handlers.find(signal);
		if (finder == this->MEMBER_handlers.end())
		{
			this->MEMBER_handlers.insert(std::make_pair(signal, std::move(h)));
		}
		else
		{
			(*finder).second = std::move(h);
		}
		this->MEMBER_mutex.unlock();
	}

	void message::trigger(std::uintmax_t const signal)
	{
		if (this->MEMBER_done)
		{
			throw std::runtime_error("message对象正在析构，无法再引发新信号");
		}
		this->MEMBER_mutex.lock();
		auto finder = this->MEMBER_handlers.find(signal);
		if (finder != this->MEMBER_handlers.end())
		{
			this->MEMBER_signals.push(signal);
			this->MEMBER_not_waiting = true;
			this->MEMBER_waiter.notify_one();
			this->MEMBER_mutex.unlock();
		}
		else
		{
			this->MEMBER_mutex.unlock();
			std::string hint = "信号"
				+ std::to_string(signal)
				+ "未注册";
			throw std::invalid_argument(hint);
		}

	}
	message::message()
		: MEMBER_done(false)
		, MEMBER_lock()
		, MEMBER_mutex()
		, MEMBER_waiter()
		, MEMBER_peeker()
		, MEMBER_not_waiting(false)
	{
		using TYPE_lock = decltype(message::MEMBER_lock);
		this->MEMBER_lock = TYPE_lock(this->MEMBER_mutex, std::defer_lock);
		this->MEMBER_peeker = std::thread(&message::peek, this);
	}
	message::~message()
		noexcept(noexcept(std::declval<std::thread>().join()))
	{

		this->MEMBER_done = true;
		this->MEMBER_not_waiting = true;

		this->MEMBER_peeker.join();
	}

	void message::peek()noexcept
	{

		while (not this->MEMBER_done)
		{
			this->MEMBER_lock.lock();
			if (not this->MEMBER_signals.empty())
			{
				this->MEMBER_not_waiting = true;
				auto finder = this->MEMBER_handlers.find(this->MEMBER_signals.front());
				//如果信号不存在，则在引发信号（即调用trigger函数）时就会抛出异常，所以信号队列里不会有未注册的信号
				(*finder).second->handle();
				this->MEMBER_signals.pop();
			}
			else
			{
				this->MEMBER_not_waiting = false;
				using TYPE_waiting = std::atomic<bool> const &;
				class judge_waiting_status final
				{
				private:
					TYPE_waiting MEMBER_waiting_status;
				public:
					judge_waiting_status(TYPE_waiting waiting_status)noexcept
						:MEMBER_waiting_status(waiting_status)
					{
					}
					~judge_waiting_status()noexcept
					{
					}
					judge_waiting_status(judge_waiting_status const & origin)noexcept
						:MEMBER_waiting_status(origin.MEMBER_waiting_status)
					{
					}
					judge_waiting_status(judge_waiting_status &&) = delete;
					void operator =(judge_waiting_status const & origin)noexcept
					{
						if (this != std::addressof(origin))
						{
							(*this) = origin;
						}
					}
					void operator =(judge_waiting_status &&) = delete;
					TYPE_waiting operator ()()const noexcept
					{
						return this->MEMBER_waiting_status;
					}
				};
				auto const is_waiting = judge_waiting_status(this->MEMBER_not_waiting);
				this->MEMBER_waiter.wait(this->MEMBER_lock, is_waiting);
			}
			this->MEMBER_lock.unlock();
		}
		bool done = false;
		this->MEMBER_lock.lock();
		while (not done)
		{
			if (not this->MEMBER_signals.empty())
			{
				auto finder = this->MEMBER_handlers.find(this->MEMBER_signals.front());
				//如果信号不存在，则在引发信号（即调用trigger函数）时就会抛出异常，所以信号队列里不会有未注册的信号
				(*finder).second->handle();
				this->MEMBER_signals.pop();
			}
			else
			{
				done = true;
			}
		}
		this->MEMBER_lock.unlock();
	}
}