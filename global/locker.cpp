#include ".\locker.hpp"
#include <random>

namespace OKps
{
    std::size_t spin_locker::INNER_init_id()
    {
        std::size_t result;
        while (true)
        {
            bool TEMP_flag = false;
            if (spin_locker::MEMBER_id_pool_lock.compare_exchange_strong(TEMP_flag, true))
            {
                std::random_device seed;                                 // 用于生成随机数种子
                std::mt19937 random_engine(seed());                      // 随机数生成器
                std::uniform_int_distribution<std::size_t> distribution; // 指定随机数的分布为均匀分布，这里的范围参数是闭区间

                while (true)
                {
                    result = distribution(random_engine);
                    auto TEMP_finder = spin_locker::MEMBER_id_pool.find(result);
                    if (TEMP_finder == spin_locker::MEMBER_id_pool.end())
                    {
                        spin_locker::MEMBER_id_pool.insert(result);
                        break;
                    }
                }
                spin_locker::MEMBER_id_pool_lock.store(false);
                break;
            }
        }
        return result;
    }
    void spin_locker::lock()
    {

        while (true)
        {
            bool TEMP_flag = false;
            if (this->MEMBER_lock.compare_exchange_strong(TEMP_flag, true))
            {
                /*
                lock没有被占用，现在分配给当前线程
                */

                this->MEMBER_thread_id = std::this_thread::get_id();
                this->MEMBER_graph.add_thread(this->MEMBER_thread_id);
                break;
            }
            else
            {
                /*
                lock被占用
                */
                if (this->MEMBER_thread_id == std::this_thread::get_id())
                {
                    /*
                    占用者是当前线程，则本次申请锁是重复申请
                    */
                    throw std::logic_error("当前线程申请锁，但此锁已经被该线程占用，重复申请会导致死锁");
                }
                else
                {
                    /*
                    占用者不是当前线程，则当前线程继续等待
                    */
                    this->MEMBER_graph.add_relation(std::this_thread::get_id(), this->MEMBER_thread_id, this->MEMBER_lock_id);
                }
            }
        }
    }

    void spin_locker::unlock()
    {

        if (this->MEMBER_lock.load() == true)
        {
            /*
            锁被占用
            */
            if (this->MEMBER_thread_id == std::this_thread::get_id())
            {
                /*
                占用者是当前线程
                释放锁
                */

                /*
                占用此锁的线程ID改为空
                */
                this->MEMBER_graph.erase_relation(this->MEMBER_lock_id);
                this->MEMBER_thread_id = std::thread::id();

                this->MEMBER_lock.store(false);
            }
            else
            {

                throw std::logic_error("此锁的所有者不是当前线程，无法进行解锁操作");
            }
        }
        else
        {
            /*
            锁没有被占用
            */

            throw std::logic_error("此锁没有上锁，无法进行解锁操作");
        }
    }

    bool spin_locker::is_locked() const noexcept
    {
        return this->MEMBER_lock.load();
    }

    const std::thread::id & spin_locker::owner_thread() const noexcept
    {
        return this->MEMBER_thread_id;
    }

    const wait_graph & spin_locker::thread_graph() const noexcept
    {
        return this->MEMBER_graph;
    }
    spin_locker::spin_locker()
        : MEMBER_lock(false), MEMBER_thread_id(), MEMBER_lock_id(spin_locker::INNER_init_id())
    {
    }

    wait_graph::wait_graph()
        : MEMBER_lock(false)
    {
    }

    wait_graph::~wait_graph()
    {
    }

    bool wait_graph::have_dead_lock() const
    {

        /*
        改造拓扑排序，判断图中是否有环
        第1步：创建一个到有向图的映射。
        第2步：在有向图中选一个入度为0的顶点，从映射中删除该顶点和它的所有出边。
        第3步：重复执行第2步，直到映射为空（没有环），或剩余顶点的入度均不为0（有环）。

        这样做可以去掉所有不属于环的顶点，同时照原样保留所有的环
        但原样的环不一定能够揭示死锁产生的原因，因为除了简单环（纯粹的O形环），还存在复杂环
        例如设想一个8字形状的环，如果多个8字环或者日字环交叉拼接在一起，形成复杂结构，
        再假设这个复杂连通子图中顶点总数有成千上万，不能靠人力来研究
        可以用 Johnson 算法找出所有简单环
        */
        while (true)
        {
            bool TEMP_singer = false;
            if (this->MEMBER_lock.compare_exchange_strong(TEMP_singer, true))
            {
                break;
            }
        }
        /*
        映射
        */
        std::set<std::thread::id> TEMP_vertex;
        /*
        记录各顶点的入度
        */
        std::map<std::thread::id, size_t> TEMP_incoming;
        for (auto TEMP_finder = this->MEMBER_graph.begin(); TEMP_finder != this->MEMBER_graph.end(); TEMP_finder++)
        {
            TEMP_vertex.insert(TEMP_finder->first);
            /*
            当前顶点是否在 入度表 中
            如果不在，则将ID插入 入度表
            */
            auto TEMP_itor1 = TEMP_incoming.find(TEMP_finder->first);
            if (TEMP_itor1 == TEMP_incoming.end())
            {
                TEMP_incoming.insert(std::make_pair(TEMP_finder->first, 0));
                TEMP_itor1 = TEMP_incoming.find(TEMP_finder->first);
            }
            /*
            遍历当前顶点的邻边，调整 每个被等待线程ID 的入度
            */
            for (auto TEMP_itor2 = TEMP_finder->second->begin(); TEMP_itor2 != TEMP_finder->second->end(); TEMP_itor2++)
            {
                auto TEMP_itor3 = TEMP_incoming.find(TEMP_itor2->first);
                if (TEMP_itor3 == TEMP_incoming.end())
                {
                    TEMP_incoming.insert(std::make_pair(TEMP_itor2->first, 1));
                    continue;
                }
                else
                {
                    TEMP_itor3->second++;
                }
            }
        }
        /*
        现在，TEMP_vertex 是原图中所有顶点的ID集合，TEMP_incoming是原图中所有顶点的入度
        */

        while (true)
        {
            if (TEMP_vertex.empty())
            {
                /*
                顶点映射为空，没有环
                */
                this->MEMBER_lock.store(false);
                return false;
            }
            bool TEMP_can_find = false;
            for (auto TEMP_finder = TEMP_incoming.begin(); TEMP_finder != TEMP_incoming.end(); TEMP_finder++)
            {
                if (TEMP_finder->second == 0)
                {
                    /*
                    找到1个入度为0的顶点
                    */
                    TEMP_can_find = true;
                    TEMP_vertex.erase(TEMP_finder->first);
                    auto TEMP_itor = this->MEMBER_graph.find(TEMP_finder->first);
                    for (auto TEMP_counter = TEMP_itor->second->begin(); TEMP_counter != TEMP_itor->second->end(); TEMP_counter++)
                    {
                        /*
                        遍历这个顶点的边表，把边表包含的顶点的入度 -1 ，相当于完成了 第2步的操作
                        */
                        auto TEMP_worker = TEMP_incoming.find(TEMP_counter->first);
                        TEMP_worker->second--;
                    }
                    TEMP_incoming.erase(TEMP_finder);
                    break;
                }
            }
            if (not TEMP_can_find)
            {
                /*
                顶点映射不空，但是找不到入度为0的顶点
                说明有环
                */
                this->MEMBER_lock.store(false);

                return true;
            }
        }
    }
    const wait_graph::TYPE_graph & wait_graph::wait_relation() const noexcept
    {
        return this->MEMBER_graph;
    }

    void wait_graph::add_thread(const std::thread::id & PARAMETER_thread_id)
    {

        while (true)
        {
            bool TEMP_singer = false;
            if (this->MEMBER_lock.compare_exchange_strong(TEMP_singer, true))
            {
                break;
            }
        }
        auto TEMP_finder = this->MEMBER_graph.find(PARAMETER_thread_id);

        if (TEMP_finder == this->MEMBER_graph.end())
        {
            auto TEMP_relation = std::make_shared<wait_graph::TYPE_vertex>();

            this->MEMBER_graph.insert(std::make_pair(PARAMETER_thread_id, TEMP_relation));
        }
        this->MEMBER_lock.store(false);
    }

    void wait_graph::erase_thread(const std::thread::id & PARAMETER_thread_id)
    {
        while (true)
        {
            bool TEMP_singer = false;
            if (this->MEMBER_lock.compare_exchange_strong(TEMP_singer, true))
            {
                break;
            }
        }
        auto TEMP_finder = this->MEMBER_graph.find(PARAMETER_thread_id);
        if (TEMP_finder == this->MEMBER_graph.end())
        {
            auto TEMP_error = std::invalid_argument("此线程不存在于图中");
            this->MEMBER_lock.store(false);
            throw TEMP_error;
        }
        else
        {
            this->MEMBER_graph.erase(TEMP_finder);
            /*
            遍历等待关系图，对剩余的每一个顶点，查找其是否保存了被删除的线程ID
            如果有，则删除此ID，避免混乱
            */
            TEMP_finder = this->MEMBER_graph.begin();
            while (TEMP_finder != this->MEMBER_graph.end())
            {
                auto TEMP_itor = TEMP_finder->second->find(PARAMETER_thread_id);
                if (TEMP_itor != TEMP_finder->second->end())
                {
                    TEMP_finder->second->erase(TEMP_itor);
                }
            }
        }
        this->MEMBER_lock.store(false);
    }

    const wait_graph::TYPE_vertex & wait_graph::wait_relation(const std::thread::id & PARAMETER_thread_id) const
    {
        while (true)
        {
            bool TEMP_singer = false;
            if (this->MEMBER_lock.compare_exchange_strong(TEMP_singer, true))
            {
                break;
            }
        }
        auto TEMP_finder = this->MEMBER_graph.find(PARAMETER_thread_id);
        if (TEMP_finder == this->MEMBER_graph.end())
        {
            auto TEMP_error = std::invalid_argument("此线程不存在于图中");
            this->MEMBER_lock.store(false);

            throw TEMP_error;
        }
        else
        {
            this->MEMBER_lock.store(false);

            return *(TEMP_finder->second);
        }
    }
    bool wait_graph::have_dead_lock(const std::thread::id & PARAMETER_thread_id) const
    {
        while (true)
        {
            bool TEMP_singer = false;
            if (this->MEMBER_lock.compare_exchange_strong(TEMP_singer, true))
            {
                break;
            }
        }
        /*
        映射
        */
        std::set<std::thread::id> TEMP_vertex;
        /*
        记录各顶点的入度
        */
        std::map<std::thread::id, size_t> TEMP_incoming;
        for (auto TEMP_finder = this->MEMBER_graph.begin(); TEMP_finder != this->MEMBER_graph.end(); TEMP_finder++)
        {
            TEMP_vertex.insert(TEMP_finder->first);
            /*
            当前顶点是否在 入度表 中
            如果不在，则将ID插入 入度表
            */
            auto TEMP_itor1 = TEMP_incoming.find(TEMP_finder->first);
            if (TEMP_itor1 == TEMP_incoming.end())
            {
                TEMP_incoming.insert(std::make_pair(TEMP_finder->first, 0));
                TEMP_itor1 = TEMP_incoming.find(TEMP_finder->first);
            }
            /*
            遍历当前顶点的邻边，调整 每个被等待线程ID 的入度
            */
            for (auto TEMP_itor2 = TEMP_finder->second->begin(); TEMP_itor2 != TEMP_finder->second->end(); TEMP_itor2++)
            {
                auto TEMP_itor3 = TEMP_incoming.find(TEMP_itor2->first);
                if (TEMP_itor3 == TEMP_incoming.end())
                {
                    TEMP_incoming.insert(std::make_pair(TEMP_itor2->first, 1));
                    continue;
                }
                else
                {
                    TEMP_itor3->second++;
                }
            }
        }
        /*
        现在，TEMP_vertex 是原图中所有顶点的ID集合，TEMP_incoming是原图中所有顶点的入度
        */
        while (true)
        {
            if (TEMP_vertex.empty())
            {
                /*
                顶点映射为空，没有环
                */
                this->MEMBER_lock.store(false);
                return false;
            }
            bool TEMP_can_find = false;
            for (auto TEMP_finder = TEMP_incoming.begin(); TEMP_finder != TEMP_incoming.end(); TEMP_finder++)
            {
                if (TEMP_finder->second == 0)
                {
                    /*
                    找到1个入度为0的顶点
                    */
                    TEMP_can_find = true;
                    TEMP_vertex.erase(TEMP_finder->first);
                    auto TEMP_itor = this->MEMBER_graph.find(TEMP_finder->first);
                    for (auto TEMP_counter = TEMP_itor->second->begin(); TEMP_counter != TEMP_itor->second->end(); TEMP_counter++)
                    {
                        /*
                        遍历这个顶点的边表，把边表包含的顶点的入度 -1 ，相当于完成了 第2步的操作
                        */
                        auto TEMP_worker = TEMP_incoming.find(TEMP_counter->first);
                        TEMP_worker->second--;
                    }
                    TEMP_incoming.erase(TEMP_finder);
                    break;
                }
            }
            if (not TEMP_can_find)
            {
                /*
                顶点映射不空，但是找不到入度为0的顶点
                说明有环
                */
                auto TEMP_result = TEMP_vertex.find(PARAMETER_thread_id);
                if (TEMP_result == TEMP_vertex.end())
                {
                    /*
                    指定线程不在环中，不参与死锁
                    */
                    this->MEMBER_lock.store(false);

                    return false;
                }
                else
                {
                    this->MEMBER_lock.store(false);

                    return true;
                }
            }
        }
    }
    void wait_graph::add_relation(const ::std::thread::id & waiter_id, const ::std::thread::id & waited_id, const size_t lock_id)
    {
        while (true)
        {
            bool TEMP_singer = false;
            if (this->MEMBER_lock.compare_exchange_strong(TEMP_singer, true))
            {
                break;
            }
        }
        auto TEMP_finder = this->MEMBER_graph.find(waiter_id);
        if (TEMP_finder == this->MEMBER_graph.end())
        {
            this->MEMBER_lock.store(false);

            this->add_thread(waiter_id);
            while (true)
            {
                bool TEMP_singer = false;
                if (this->MEMBER_lock.compare_exchange_strong(TEMP_singer, true))
                {
                    break;
                }
            }
            TEMP_finder = this->MEMBER_graph.find(waiter_id);
        }
        auto TEMP_itor = TEMP_finder->second->find(waited_id);
        if (TEMP_itor == TEMP_finder->second->end())
        {
            auto TEMP_lock_id_list = std::make_shared<std::set<size_t>>();
            TEMP_lock_id_list->insert(lock_id);
            TEMP_finder->second->insert(std::make_pair(waited_id, TEMP_lock_id_list));
        }

        this->MEMBER_lock.store(false);
    }
    void wait_graph::erase_relation(const size_t lock_id)
    {
        while (true)
        {
            bool TEMP_singer = false;
            if (this->MEMBER_lock.compare_exchange_strong(TEMP_singer, true))
            {
                break;
            }
        }
        for (auto TEMP_finder = this->MEMBER_graph.begin(); TEMP_finder != this->MEMBER_graph.end(); TEMP_finder++)
        {

            auto TEMP_counter = TEMP_finder->second->begin();
            while (TEMP_counter != TEMP_finder->second->end())
            {
                auto TEMP_itor = TEMP_counter->second->find(lock_id);
                if (TEMP_itor != TEMP_counter->second->end())
                {
                    TEMP_counter->second->erase(TEMP_itor);
                }
                if (TEMP_counter->second->size() == 0)
                {
                    TEMP_finder->second->erase(TEMP_counter);
                    TEMP_counter = TEMP_finder->second->begin();
                }
                else
                {
                    TEMP_counter++;
                }
            }
        }
        this->MEMBER_lock.store(false);
    }

}
