#pragma once

#include <atomic>
#include <thread>

/*
此文件原本准备实现一个运行期死锁检测机制
但实现时才发现c++标准库有免死锁算法 std::lock 函数和 std::scoped_lock 包装器，不需要自己实现
*/

namespace OKps
{

    /*
    最简自旋锁
    用单个原子变量实现的自旋锁
    不做任何安全检查，使用者自行考虑死锁问题
    */
    class simple_spin_lock final
    {
    private:
        std::atomic<bool> MEMBER_lock;
        std::thread::id MEMBER_thread_id;
    public:
        simple_spin_lock() noexcept;

        /*
        原子对象无法复制或移动，故此类也无法提供复制和移动操作。
        */
        simple_spin_lock(const simple_spin_lock &) = delete;
        simple_spin_lock(simple_spin_lock && origin) = delete;

        void operator=(const simple_spin_lock &) = delete;
        void operator=(simple_spin_lock && origin) = delete;
        /*
        如果锁处于上锁状态，则析构时会解锁
        */
        ~simple_spin_lock()noexcept;

        void lock()noexcept;
        void unlock() noexcept;
        std::atomic<bool> const & is_locked()const noexcept;
        /*
        返回锁的所有者线程ID
        如果没有上锁，则返回默认构造的std::thread::id
        */
        std::thread::id const & owner_thread() const noexcept;
    };



}