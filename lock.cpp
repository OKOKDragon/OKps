
#include ".\lock.hpp"

namespace OKps
{

    simple_spin_lock::simple_spin_lock()
        noexcept(std::is_nothrow_default_constructible_v<std::thread::id>
and noexcept(std::atomic<bool>(std::declval<bool>())))
    :MEMBER_lock(false)
        , MEMBER_thread_id()
    {
    }

    simple_spin_lock::~simple_spin_lock()
        noexcept(std::is_nothrow_destructible_v<std::atomic<bool>>
and std::is_nothrow_destructible_v<std::thread::id>
and noexcept(std::declval<simple_spin_lock *>()->unlock()))
    {
        this->unlock();
    }

    void simple_spin_lock::lock()
        noexcept(noexcept(std::declval<std::atomic<bool>>().compare_exchange_strong(std::declval<bool &>(), std::declval<bool>()))
        and noexcept(std::declval<std::thread::id>() = std::this_thread::get_id()))
    {
        while (true)
        {
            bool TEMP_flag = false;
            if (this->MEMBER_lock.compare_exchange_strong(TEMP_flag, true))
            {
                //lock没有被占用，现在分配给当前线程                
                this->MEMBER_thread_id = std::this_thread::get_id();
                break;
            }
        }
    }

    void simple_spin_lock::unlock()
        noexcept(noexcept(std::declval<std::thread::id>() = std::thread::id())
and noexcept(std::declval<std::atomic<bool>>() = std::declval<bool>()))
    {
        this->MEMBER_thread_id = std::thread::id();
        this->MEMBER_lock = false;
    }

    std::atomic<bool> const & simple_spin_lock::is_locked() const noexcept
    {
        return this->MEMBER_lock;
    }

    std::thread::id const & simple_spin_lock::owner_thread() const noexcept
    {
        return this->MEMBER_thread_id;
    }

}
