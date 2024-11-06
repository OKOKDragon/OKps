#pragma once

#include <type_traits>
#include <mutex>
#include <map>
#include <semaphore>

#include ".\lock.hpp"

void * operator new(std::size_t const size);

void operator delete(void * const block) noexcept;

//微软 C 库未实现 std::aligned_alloc函数，没有此函数则无法以标准方式覆盖全局 operator new(std::size_t,std::align_val_t)
#ifdef MACRO_OKps_enable_global_aligned_alloc
void * operator new(std::size_t const size, std::align_val_t const);
void operator delete(void * const block, std::align_val_t const) noexcept;
#endif

/*
此文件定义内存分配记录器

经测试，在windows11上，启用此内存分配记录器将使每次内存分配比原来慢6倍左右。
不要在最终发布的程序中启用，仅作调试用途。
*/
namespace OKps::memory
{
    /*
    若此项为 true ，则启用全局内存分配记录器。
    */
    constexpr bool const enable_global_recorder = false;

    /*
    标识内存分配记录器使用的锁的类型。

    经测试，信号量略微比互斥锁耗时更短，而且信号量在各种情况下的耗时非常稳定，而互斥锁耗时不是很稳定。
    自旋锁则在绝大多数情况下耗时都非常长。
    推荐在所有情况下都使用信号量。
    */
    enum class lock_type
    {
        mutex,//互斥锁
        spin,//自旋锁
        semaphore//信号量
    };
    /*
    此项决定全局内存分配记录器使用何种锁。
    */
    constexpr lock_type const global_recorder_lock_type = lock_type::semaphore;

    /*
    此项决定是否在main函数结束后，全局内存分配记录器作为全局变量析构时，检测内存泄漏并抛出异常。
    */
    constexpr bool const enable_global_leak_check = enable_global_recorder;

    namespace implement
    {
        template<typename>
        class lock_proxy;

        /*
        信号量的代理类，使得信号量表现得如同满足“可基本锁定”的锁一样。

        “可基本锁定”是c++标准的具名要求，在此类中用于 std::lock_guard。
        */
        template<>
        class lock_proxy<std::binary_semaphore> final
        {
        private:
            std::binary_semaphore MEMBER_lock;
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
                noexcept(noexcept(std::declval<std::binary_semaphore &>().acquire()));
            void unlock()
                noexcept(noexcept(std::declval<std::binary_semaphore &>().release()));
        };

        template<lock_type TYPE_lock>
        class get_lock_type;

        template<>
        class get_lock_type<lock_type::mutex>
        {
        public:
            using type = std::mutex;
            get_lock_type() = delete;
            get_lock_type(get_lock_type const &) = delete;
            get_lock_type(get_lock_type &&) = delete;
            ~get_lock_type() = delete;
            void operator =(get_lock_type const &) = delete;
            void operator =(get_lock_type &&) = delete;
        };

        template<>
        class get_lock_type<lock_type::spin>
        {
        public:
            using type = simple_spin_lock;
            get_lock_type() = delete;
            get_lock_type(get_lock_type const &) = delete;
            get_lock_type(get_lock_type &&) = delete;
            ~get_lock_type() = delete;
            void operator =(get_lock_type const &) = delete;
            void operator =(get_lock_type &&) = delete;
        };

        template<>
        class get_lock_type<lock_type::semaphore>
        {
        public:
            using type = lock_proxy<std::binary_semaphore>;
            get_lock_type() = delete;
            get_lock_type(get_lock_type const &) = delete;
            get_lock_type(get_lock_type &&) = delete;
            ~get_lock_type() = delete;
            void operator =(get_lock_type const &) = delete;
            void operator =(get_lock_type &&) = delete;
        };

        /*
        给内存分配记录器中的std::map专用的内存分配器

        直接调用std::malloc和std::free函数，不做其他任何事。
        */
        template <typename needed_type>
        class allocator_type
        {
        public:
            // 类型定义
            using value_type = needed_type;
            using propagate_on_container_move_assignment = std::true_type;
            using is_always_equal = std::true_type;

            // 构造函数
            allocator_type() noexcept;

            ~allocator_type()noexcept;

            // 从其他分配器构造
            template <typename any_type>
            allocator_type(allocator_type<any_type> const &) noexcept;


              // 分配内存
            value_type * allocate(std::size_t const count);

            // 释放内存
            void deallocate(value_type * const block, std::size_t const count)noexcept;

        };

    }

    /*
    内存分配记录器
    */
    class recorder_type final
    {
        friend class implement::get_lock_type<lock_type::semaphore>;
        friend void * ::operator new(std::size_t const);
        friend void ::operator delete(void * const) noexcept;
    #ifdef MACRO_OKps_enable_global_aligned_alloc
        friend void * ::operator new(std::size_t const size, std::align_val_t const);
        friend void ::operator delete(void * const block, std::align_val_t const) noexcept;
    #endif
    private:
        using pool_type = std::map<void const *, std::size_t, std::less<void const *>, implement::allocator_type<std::pair<void const * const, std::size_t>>>;
        pool_type MEMBER_pool;

        mutable implement::get_lock_type<global_recorder_lock_type>::type MEMBER_lock;

    protected:
        void add_record(void const * const, std::size_t const)
            noexcept(noexcept(std::lock_guard<decltype(recorder_type::MEMBER_lock)>(std::declval<decltype(recorder_type::MEMBER_lock) &>()))
and noexcept(std::declval<pool_type &>().insert(std::make_pair(std::declval<void const * const>(), std::declval<std::size_t const>()))));
        void erase_record(void const * const)
            noexcept(noexcept(std::lock_guard<decltype(recorder_type::MEMBER_lock)>(std::declval<decltype(recorder_type::MEMBER_lock) &>()))
and noexcept(std::declval<pool_type &>().erase(std::declval<void const * const>())));
    public:
        recorder_type(recorder_type const &) = delete;
        recorder_type(recorder_type &&) = delete;
        void operator =(recorder_type const &) = delete;
        void operator =(recorder_type &&) = delete;

        recorder_type()
            noexcept(std::is_nothrow_default_constructible_v<decltype(recorder_type::MEMBER_lock)>
and std::is_nothrow_default_constructible_v<pool_type>);
        ~recorder_type()
            noexcept(std::is_nothrow_destructible_v<decltype(recorder_type::MEMBER_lock)>
and std::is_nothrow_destructible_v<pool_type>
and (not enable_global_leak_check));

        pool_type track() const
            noexcept(noexcept(std::lock_guard<decltype(recorder_type::MEMBER_lock)>(std::declval<decltype(recorder_type::MEMBER_lock) &>()))
and std::is_nothrow_copy_constructible_v<pool_type>);

    private:

        static recorder_type & INNER_global_recorder()
            noexcept(std::is_nothrow_default_constructible_v<recorder_type>);
    public:
        /*
        获取全局内存分配记录器
        */
        static recorder_type const & global_recorder()noexcept;
    };

}

