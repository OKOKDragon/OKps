#pragma once
#include <atomic>
#include <thread>
namespace OKps
{

    /*
    最简自旋锁
    用单个标准库的原子变量实现的自旋锁
    不做任何安全检查，使用者自行考虑死锁问题
    */
    class simple_spin_lock final
    {
    private:
        std::atomic_bool MEMBER_lock;
        std::thread::id MEMBER_thread_id;
    public:
        simple_spin_lock()noexcept;

        simple_spin_lock(const simple_spin_lock &) = delete;
        simple_spin_lock(simple_spin_lock &&) = delete;

        void operator=(const simple_spin_lock &) = delete;
        void operator=(simple_spin_lock &&) = delete;
        /*
        如果锁处于上锁状态，则析构函数会一直阻塞，直到解锁
        */
        ~simple_spin_lock()noexcept;

        void lock()noexcept;
        void unlock() noexcept;
        std::atomic_bool const & is_locked()const noexcept;

        /*
        返回锁的所有者线程ID
        如果没有上锁，则返回 std::thread::id默认构造函数构造的ID
        */
        std::thread::id const & owner_thread() const noexcept;
    };

}