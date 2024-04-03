#pragma once

#include ".\base.hpp"

namespace OKps
{
    /*
    线程等待关系图
    一个有向图，记录线程之间的等待关系
    等待关系是指，线程 a 申请访问 共享资源 m，但 m 此时通过锁被线程 b 占有，称为 a 等待 b
    以等待关系为边，线程为顶点，建立有向图
    该图中有环，等价于存在死锁
    */
    class wait_graph : public base
    {

    private:
        /*
        负责本类成员的锁
        为false则无人占用
        */
        mutable ::std::atomic_bool MEMBER_lock;

    public:
        /*
        存储顶点的类型
        存储的内容是：被该顶点所等待的线程的集合，每个线程对应一个锁列表，因为线程可能同时等待多个锁
        例如，线程A持有2个锁 la、 lb，分别对应资源 da、db，而线程B也需要 da、db，所以要同时等待2个锁
        */
        using TYPE_vertex = ::std::map<::std::thread::id, ::std::shared_ptr<std::set<size_t>>>;
        /*
        存储图的类型
        map中每个顶点用线程id标识，对应的值是该顶点的邻边集合
        这个关系可以是是否有边，也可以是是否能从该顶点到达指定顶点
        */
        using TYPE_graph = ::std::map<::std::thread::id, ::std::shared_ptr<TYPE_vertex>>;

    private:
        /*
        等待关系图
        边的意义是标识本顶点与另一顶点之间是否有邻边
        规定边的方向为：线程a等待线程b，则边从a指向b
        */
        TYPE_graph MEMBER_graph;

    public:
        wait_graph();
        ~wait_graph();
        wait_graph(const wait_graph &) = delete;
        void operator=(const wait_graph &) = delete;
        wait_graph(const wait_graph &&) = delete;
        void operator=(const wait_graph &&) = delete;
        wait_graph(wait_graph &&) = delete;
        void operator=(wait_graph &&) = delete;

        /*
        向图中添加线程
        */
        void add_thread(const ::std::thread::id & PARAMETER_thread_id);
        /*
        在图中删除线程
        当线程被从本图中删除时，会删除所有相关信息
        如果该线程出现了死锁，本类不负责
        */
        void erase_thread(const ::std::thread::id & PARAMETER_thread_id);
        /*
        在图中寻找等待者线程，如果没有则添加该线程ID
        然后建立2个线程之间的等待关系
        */
        void add_relation(const ::std::thread::id & waiter_id, /*进行等待的线程ID*/
                          const ::std::thread::id & waited_id, /*被等待的线程ID*/
                          const size_t lock_id);              /*对应的锁的ID*/
        /*
        在图中寻找并删除一个被等待的锁
        例如，多个线程 A1，A2，···，An都在等待线程B释放锁Lb，则输入Lb，会清除图中包含的全部Lb
        */
        void erase_relation(const size_t lock_id); /*被等待的锁的ID*/
        /*
        返回指定线程的等待关系
        只含有该线程所等待的上游线程，不含有等待该线程的下游线程
        */
        const TYPE_vertex & wait_relation(const ::std::thread::id & PARAMETER_thread_id) const;
        /*
        整个图中是否有死锁
        */
        bool have_dead_lock() const;
        /*
        判断 指定线程 是否参与了死锁
        */
        bool have_dead_lock(const ::std::thread::id & PARAMETER_thread_id) const;
        /*
        返回整个等待关系图
        */
        const TYPE_graph & wait_relation() const noexcept;

    private:
    };

    /*
    用原子变量实现的自旋锁
    */
    class spin_locker : public base
    {
    public:
    private:
        /*
        锁
        */
        ::std::atomic_bool MEMBER_lock;
        /*
        占用此锁的线程ID
        这个变量在上锁和解锁时会被修改，但是如果解锁者不是占用锁的线程，会抛出异常而不会修改这个变量，所以只有锁的占有者线程会修改此变量
        */
        ::std::thread::id MEMBER_thread_id;
        static inline wait_graph MEMBER_graph = wait_graph();
        static inline ::std::set<::std::size_t> MEMBER_id_pool;
        static inline ::std::atomic_bool MEMBER_id_pool_lock = false;
        static ::std::size_t INNER_init_id();
        const ::std::size_t MEMBER_lock_id;

    public:
        /*
        上锁
        */
        void lock();
        /*
        解锁
        */
        void unlock();
        /*
        检测锁的开闭
        */
        bool is_locked() const noexcept;
        /*
        返回锁的所有者线程ID
        如果没有上锁，则返回 std::thread::id默认构造函数构造的ID
        */
        const ::std::thread::id & owner_thread() const noexcept;

        const wait_graph & thread_graph() const noexcept;

        /*
        锁初始为没有上锁的状态
        */
        spin_locker();

        spin_locker(const spin_locker &) = delete;
        spin_locker(spin_locker &&) = delete;

        void operator=(const spin_locker &) = delete;
        void operator=(spin_locker &&) = delete;
    };
}
