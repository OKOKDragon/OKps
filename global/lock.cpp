#include <cstddef>
#include <set>
#include <random>

#include ".\lock.hpp"

namespace OKps
{
    class lock_id_pool final
    {
    private:
        static inline std::set<std::uintmax_t> MEMBER_id_pool;
        static inline std::atomic_bool MEMBER_lock = std::atomic_bool(false);
    public:
        //为锁生成唯一的id
        static std::uintmax_t generate_id();

    };

    std::uintmax_t lock_id_pool::generate_id()
    {
        std::uintmax_t result;
        while (true)
        {
            bool TEMP_flag = false;
            if (lock_id_pool::MEMBER_lock.compare_exchange_strong(TEMP_flag, true))
            {
                std::random_device seed;                                 // 用于生成随机数种子
                std::mt19937 random_engine(seed());                      // 随机数生成器
                std::uniform_int_distribution<std::uintmax_t> distribution(std::numeric_limits<std::uintmax_t>::min(), std::numeric_limits<std::uintmax_t>::max()); // 指定随机数的分布为均匀分布，这里的范围参数是闭区间

                while (true)
                {
                    result = distribution(random_engine);
                    auto TEMP_finder = lock_id_pool::MEMBER_id_pool.find(result);
                    if (TEMP_finder == lock_id_pool::MEMBER_id_pool.end())
                    {
                        lock_id_pool::MEMBER_id_pool.insert(result);
                        break;
                    }
                }
                lock_id_pool::MEMBER_lock.store(false);
                break;
            }
        }
        return result;
    }

    simple_spin_lock::simple_spin_lock() noexcept
        :MEMBER_lock(false)
        , MEMBER_thread_id()
    {
    }

    simple_spin_lock::~simple_spin_lock() noexcept
    {
        while (this->MEMBER_lock)
        {
        }
    }

    void simple_spin_lock::lock() noexcept
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

    void simple_spin_lock::unlock() noexcept
    {
        this->MEMBER_thread_id = std::thread::id();
        this->MEMBER_lock.store(false);
    }

    std::atomic_bool const & simple_spin_lock::is_locked() const noexcept
    {
        return this->MEMBER_lock;
    }

    std::thread::id const & simple_spin_lock::owner_thread() const noexcept
    {
        return this->MEMBER_thread_id;
    }

}
