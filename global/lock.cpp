
#include ".\lock.hpp"

namespace OKps
{

    lock_proxy<std::binary_semaphore>::lock_proxy()
        noexcept(noexcept(std::binary_semaphore(1)))
        :MEMBER_lock(1)
        , MEMBER_locked_flag(false)
    {
    }
    lock_proxy<std::binary_semaphore>::~lock_proxy()
        noexcept(std::is_nothrow_destructible_v<std::binary_semaphore>)
    {
    }
    std::thread::id const & lock_proxy<std::binary_semaphore>::owner_thread() const noexcept
    {
        return this->MEMBER_thread_id;
    }
    bool const & lock_proxy<std::binary_semaphore>::is_locked()const noexcept
    {
        return this->MEMBER_locked_flag;
    }
    void lock_proxy<std::binary_semaphore>::lock()
        noexcept(noexcept(std::declval<std::binary_semaphore &>().acquire())
        and noexcept(std::declval<std::thread::id &>() = std::this_thread::get_id()))
    {
        this->MEMBER_lock.acquire();
        this->MEMBER_locked_flag = true;
        this->MEMBER_thread_id = std::this_thread::get_id();
    }
    void lock_proxy<std::binary_semaphore>::unlock()
        noexcept(noexcept(std::declval<std::binary_semaphore &>().release())
        and noexcept(std::declval<std::thread::id &>() = std::thread::id()))
    {
        this->MEMBER_thread_id = std::thread::id();
        this->MEMBER_locked_flag = false;
        this->MEMBER_lock.release();
    }
}
