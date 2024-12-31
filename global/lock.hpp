#pragma once

#include <thread>
#include <semaphore>
#include <mutex>
#include <type_traits>

/*
此文件原本准备实现一个运行期死锁检测机制
但实现时才发现c++标准库有免死锁算法，包括 std::lock 函数和 std::scoped_lock 包装器，不需要自己实现
*/

namespace OKps
{

    /*
    满足标准库中“可基本锁定”要求的代理类
    */
    template<typename>
    class lock_proxy;

    /*
    二元信号量的代理类
    将信号量的用法转化成“可基本锁定”的锁，即使用 lock() 和 unlock() 成员函数。
    */
    template<>
    class lock_proxy<std::binary_semaphore> final
    {
    private:
        std::binary_semaphore MEMBER_lock;
        std::thread::id MEMBER_thread_id;
        bool MEMBER_locked_flag;
    public:
        lock_proxy()
            noexcept(noexcept(std::binary_semaphore(1)));
        ~lock_proxy()
            noexcept(std::is_nothrow_destructible_v<std::binary_semaphore>);
        lock_proxy(lock_proxy const &) = delete;
        lock_proxy(lock_proxy &&) = delete;
        void operator = (lock_proxy const &) = delete;
        void operator =(lock_proxy &&) = delete;
        void lock()
            noexcept(noexcept(std::declval<std::binary_semaphore &>().acquire())
            and noexcept(std::declval<std::thread::id &>() = std::this_thread::get_id()));
        void unlock()
            noexcept(noexcept(std::declval<std::binary_semaphore &>().release())
and noexcept(std::declval<std::thread::id &>() = std::thread::id()));
        /*
        返回锁的所有者线程的编号
        如果没有上锁，则其值是默认构造的std::thread::id
        */
        std::thread::id const & owner_thread() const noexcept;
        bool const & is_locked()const noexcept;
    };

}