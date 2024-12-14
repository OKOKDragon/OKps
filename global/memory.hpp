#pragma once

#include <map>

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
    此项决定是否在main函数结束后，全局内存分配记录器作为全局变量析构时，检测内存泄漏并抛出异常。
    */
    constexpr bool const enable_global_leak_check = enable_global_recorder;

    namespace implement
    {

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
        friend void * ::operator new(std::size_t const);
        friend void ::operator delete(void * const) noexcept;
    #ifdef MACRO_OKps_enable_global_aligned_alloc
        friend void * ::operator new(std::size_t const size, std::align_val_t const);
        friend void ::operator delete(void * const block, std::align_val_t const) noexcept;
    #endif
    private:
        using pool_type = std::map<void const *, std::size_t, std::less<void const *>, implement::allocator_type<std::pair<void const * const, std::size_t>>>;
        pool_type MEMBER_pool;

        mutable lock_proxy<std::binary_semaphore> MEMBER_lock;

        bool const MEMBER_check;
    public:
        void add_record(void const * const, std::size_t const)
            noexcept(noexcept(std::lock_guard<decltype(recorder_type::MEMBER_lock)>(std::declval<decltype(recorder_type::MEMBER_lock) &>()))
and noexcept(std::declval<pool_type &>().insert(std::make_pair(std::declval<void const * const>(), std::declval<std::size_t const>()))));
        void erase_record(void const * const)
            noexcept(noexcept(std::lock_guard<decltype(recorder_type::MEMBER_lock)>(std::declval<decltype(recorder_type::MEMBER_lock) &>()))
and noexcept(std::declval<pool_type &>().erase(std::declval<void const * const>())));

        recorder_type(recorder_type const &) = delete;
        recorder_type(recorder_type &&) = delete;
        void operator =(recorder_type const &) = delete;
        void operator =(recorder_type &&) = delete;

        recorder_type(bool const check)
            noexcept(std::is_nothrow_default_constructible_v<decltype(recorder_type::MEMBER_lock)>
and std::is_nothrow_default_constructible_v<pool_type>);
        ~recorder_type()
            noexcept(std::is_nothrow_destructible_v<decltype(recorder_type::MEMBER_lock)>
and std::is_nothrow_destructible_v<pool_type>
and false);
        /*
        检查是否有尚未解分配的内存
        */
        bool check()const
            noexcept(noexcept(std::lock_guard<decltype(recorder_type::MEMBER_lock)>(std::declval<decltype(recorder_type::MEMBER_lock) &>()))
and noexcept(std::declval<pool_type const &>().empty()));
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
        static recorder_type const & global_recorder()
            noexcept(enable_global_recorder);
    };

}

