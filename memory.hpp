#pragma once

#ifdef MACRO_OKps_enable_global_memory_record

#include <type_traits>
#include <mutex>
#include <map>

namespace OKps::memory
{
    namespace implement
    {
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
    class recorder_type final
    {
        friend void * ::operator new(std::size_t const);
        friend void ::operator delete(void * const) noexcept;
    private:
        using pool_type = std::map<void const *, std::size_t, std::less<void const *>, implement::allocator_type<std::pair<void const * const, std::size_t>>>;
        pool_type MEMBER_pool;
        mutable std::mutex MEMBER_lock;
    protected:
        void add_record(void const * const, std::size_t const);
        void erase_record(void const * const);
    public:
        recorder_type(recorder_type const &) = delete;
        recorder_type(recorder_type &&) = delete;
        void operator =(recorder_type const &) = delete;
        void operator =(recorder_type &&) = delete;
        recorder_type()
            noexcept(std::is_nothrow_default_constructible_v<std::mutex>
and std::is_nothrow_default_constructible_v<pool_type>);
        ~recorder_type()
            noexcept(std::is_nothrow_destructible_v<std::mutex>
and std::is_nothrow_destructible_v<pool_type>);
        pool_type track() const;

    private:

        static recorder_type & INNER_global_recorder();
    public:
        static recorder_type const & global_recorder()noexcept;
    };

}

void * operator new(std::size_t const size);
void operator delete(void * const block) noexcept;
void * operator new(std::size_t const size, std::align_val_t const);
void operator delete(void * const block, std::align_val_t const) noexcept;

#endif