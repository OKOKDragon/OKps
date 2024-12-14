
#include <random>

#include ".\message.hpp"

namespace OKps
{
	message::signal_parameter::signal_parameter(std::uintmax_t const signal, std::shared_ptr<base::blank> const & parameter)
		noexcept(std::is_nothrow_copy_constructible_v<std::shared_ptr<base::blank>>)
		:MEMBER_signal(signal)
		, MEMBER_parameter(parameter)
	{
	}
	message::signal_parameter::signal_parameter(signal_parameter const & origin)
		noexcept(std::is_nothrow_copy_constructible_v<std::shared_ptr<base::blank>>)
		:MEMBER_signal(origin.MEMBER_signal)
		, MEMBER_parameter(origin.MEMBER_parameter)
	{
	}
	void message::signal_parameter::operator =(signal_parameter const & origin)
		noexcept(std::is_nothrow_copy_assignable_v<std::shared_ptr<base::blank>>)
	{
		if (this != std::addressof(origin))
		{
			this->MEMBER_signal = origin.MEMBER_signal;
			this->MEMBER_parameter = origin.MEMBER_parameter;
		}
	}
	message::signal_parameter::signal_parameter(signal_parameter && origin)
		noexcept(std::is_nothrow_move_constructible_v<std::shared_ptr<base::blank>>)
		:MEMBER_signal(origin.MEMBER_signal)
		, MEMBER_parameter(std::move(origin.MEMBER_parameter))
	{
	}
	void message::signal_parameter::operator =(signal_parameter && origin)
		noexcept(std::is_nothrow_move_assignable_v<std::shared_ptr<base::blank>>)
	{
		if (this != std::addressof(origin))
		{
			this->MEMBER_signal = origin.MEMBER_signal;
			this->MEMBER_parameter = std::move(origin.MEMBER_parameter);
		}
	}
	message::signal_parameter::~signal_parameter()
		noexcept(std::is_nothrow_destructible_v<std::shared_ptr<base::blank>>)
	{
	}
	std::uintmax_t & message::signal_parameter::signal()noexcept
	{
		return this->MEMBER_signal;
	}
	std::uintmax_t const & message::signal_parameter::signal()const noexcept
	{
		return this->MEMBER_signal;
	}
	base::blank & message::signal_parameter::parameter()noexcept
	{
		return *(this->MEMBER_parameter);
	}
	base::blank const & message::signal_parameter::parameter()const noexcept
	{
		return *(this->MEMBER_parameter);
	}

	std::uintmax_t message::regist(std::shared_ptr<base::handler<true>> const & h)
	{
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
				this->MEMBER_handlers.insert(std::make_pair(number, h));
				found = true;
			}
			this->MEMBER_mutex.unlock();
		}

		return number;
	}
	void message::regist(std::uintmax_t const signal, std::shared_ptr<base::handler<true>> const & h)
	{
		this->MEMBER_mutex.lock();
		auto finder = this->MEMBER_handlers.find(signal);
		if (finder == this->MEMBER_handlers.end())
		{
			this->MEMBER_handlers.insert(std::make_pair(signal, h));
		}
		else
		{
			finder->second = h;
		}
		this->MEMBER_mutex.unlock();
	}

	void message::trigger(std::uintmax_t const signal, std::shared_ptr<base::blank> const & parameter)
	{
		if (not parameter)
		{
			throw std::invalid_argument("禁止以空指针为参数");
		}
		if (this->MEMBER_done)
		{
			throw std::runtime_error("message对象正在析构，无法再引发新信号");
		}
		this->MEMBER_mutex.lock();
		auto finder = this->MEMBER_handlers.find(signal);
		if (finder != this->MEMBER_handlers.end())
		{
			this->MEMBER_signals.push(signal_parameter(signal, parameter));
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
		noexcept(noexcept(std::declval<std::thread>().join())
		and std::is_nothrow_destructible_v<TYPE_handler_pool>
		and std::is_nothrow_destructible_v<TYPE_mutex>
		and std::is_nothrow_destructible_v<std::unique_lock<TYPE_mutex>>
		and std::is_nothrow_destructible_v<std::thread>
		and std::is_nothrow_destructible_v<std::condition_variable_any>
		and std::is_nothrow_destructible_v<std::queue<std::uintmax_t>>)
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
				auto finder = this->MEMBER_handlers.find(this->MEMBER_signals.front().signal());
				//如果信号不存在，则在引发信号（即调用trigger函数）时就会抛出异常，所以信号队列里不会有未注册的信号
				auto & TEMP_signal_handler = *(finder->second);
				TEMP_signal_handler(this->MEMBER_signals.front().parameter());
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
				auto finder = this->MEMBER_handlers.find(this->MEMBER_signals.front().signal());
				//如果信号不存在，则在引发信号（即调用trigger函数）时就会抛出异常，所以信号队列里不会有未注册的信号
				auto & TEMP_signal_handler = *(finder->second);
				TEMP_signal_handler(this->MEMBER_signals.front().parameter());
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