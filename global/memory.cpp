#include ".\memory.hpp"



#include <new>
#include <cstdlib>
#include <memory>

namespace OKps::memory
{

    namespace implement
    {

        template <typename needed_type>
        template <typename any_type>
        allocator_type<needed_type>::allocator_type(allocator_type<any_type> const &) noexcept
        {
        }
        template <typename needed_type>
        allocator_type<needed_type>::allocator_type() noexcept
        {
        }
        template <typename needed_type>
        allocator_type<needed_type>::~allocator_type() noexcept
        {
        }
        template <typename needed_type>
        allocator_type<needed_type>::value_type * allocator_type<needed_type>::allocate(std::size_t const count)
        {
            if (count > std::numeric_limits<std::size_t>::max() / sizeof(value_type))
            {
                throw std::bad_alloc();
            }

            void * ptr = std::malloc(count * sizeof(value_type));
            if (not ptr)
            {
                throw std::bad_alloc();
            }
            return static_cast<value_type *>(ptr);
        }
        template <typename needed_type>
        void allocator_type<needed_type>::deallocate(value_type * const block, std::size_t const count)noexcept
        {
            std::free(block);
        }
    }
    recorder_type const & recorder_type::global_recorder()
        noexcept(enable_global_recorder)
    {
        if constexpr (enable_global_recorder)
        {
            return recorder_type::INNER_global_recorder();
        }
        else
        {
            throw std::logic_error("全局内存分配记录器功能没有启用");
        }
    }

    recorder_type::recorder_type(bool const check)
        noexcept(std::is_nothrow_default_constructible_v<decltype(recorder_type::MEMBER_lock)>
and std::is_nothrow_default_constructible_v<pool_type>)
    :MEMBER_pool(pool_type::allocator_type())
        , MEMBER_lock()
        , MEMBER_check(check)
    {
    }
    bool recorder_type::check()const
        noexcept(noexcept(std::lock_guard<decltype(recorder_type::MEMBER_lock)>(std::declval<decltype(recorder_type::MEMBER_lock) &>()))
and noexcept(std::declval<pool_type const &>().empty()))
    {
        std::lock_guard<decltype(this->MEMBER_lock)> TEMP_lock(this->MEMBER_lock);
        return not(this->MEMBER_pool.empty());
    }
    recorder_type::~recorder_type()
        noexcept(std::is_nothrow_destructible_v<decltype(recorder_type::MEMBER_lock)>
and std::is_nothrow_destructible_v<pool_type>
and false)
    {
        if (this->MEMBER_check)
        {
            std::lock_guard<decltype(this->MEMBER_lock)> TEMP_lock(this->MEMBER_lock);
            if (not this->MEMBER_pool.empty())
            {
                throw std::logic_error("存在内存泄漏");
            }
        }
    }

    recorder_type::pool_type recorder_type::track()const
        noexcept(noexcept(std::lock_guard<decltype(recorder_type::MEMBER_lock)>(std::declval<decltype(recorder_type::MEMBER_lock) &>()))
        and std::is_nothrow_copy_constructible_v<pool_type>)
    {
        auto TEMP_lock = std::lock_guard<decltype(this->MEMBER_lock)>(this->MEMBER_lock);
        recorder_type::pool_type result = this->MEMBER_pool;
        return result;
    }

    void recorder_type::add_record(void const * const block, std::size_t const size)
        noexcept(noexcept(std::lock_guard<decltype(recorder_type::MEMBER_lock)>(std::declval<decltype(recorder_type::MEMBER_lock) &>()))
and noexcept(std::declval<pool_type &>().insert(std::make_pair(std::declval<void const * const>(), std::declval<std::size_t const>()))))
    {
        auto TEMP_lock = std::lock_guard<decltype(this->MEMBER_lock)>(this->MEMBER_lock);
        this->MEMBER_pool.insert(std::make_pair(block, size));
    }
    void recorder_type::erase_record(void const * const block)
        noexcept(noexcept(std::lock_guard<decltype(recorder_type::MEMBER_lock)>(std::declval<decltype(recorder_type::MEMBER_lock) &>()))
and noexcept(std::declval<pool_type &>().erase(std::declval<void const * const>())))
    {
        auto TEMP_lock = std::lock_guard<decltype(this->MEMBER_lock)>(this->MEMBER_lock);
        this->MEMBER_pool.erase(block);
    }

    recorder_type & recorder_type::INNER_global_recorder()
        noexcept(std::is_nothrow_default_constructible_v<recorder_type>)
    {
        static recorder_type INNER_recorder = recorder_type(enable_global_leak_check);
        return INNER_recorder;
    }
}

void * operator new(std::size_t const size)
{
    void * result = std::malloc(size);
    if (not result)
    {
        auto TEMP_handler = std::get_new_handler();
        if (not TEMP_handler)
        {
            throw std::bad_alloc();
        }
        else
        {
            TEMP_handler();
        }
    }
    if constexpr (OKps::memory::enable_global_recorder)
    {
        OKps::memory::recorder_type::INNER_global_recorder().add_record(result, size);
    }
    return result;
}
void operator delete(void * const block) noexcept
{
    if constexpr (OKps::memory::enable_global_recorder)
    {
        OKps::memory::recorder_type::INNER_global_recorder().erase_record(block);
    }
    std::free(block);
}

#ifdef MACRO_OKps_enable_global_aligned_alloc
void * operator new(std::size_t const size, std::align_val_t const al)
{
    static_assert(std::is_same_v<std::size_t, std::underlying_type_t<std::align_val_t>>, "c++标准规定std::align_val_t是基于std::size_t的枚举类型，此编译器违反标准。");

    void * result = std::aligned_alloc(static_cast<std::size_t>(al), size);
    if (not result)
    {
        throw std::bad_alloc();
    }
    OKps::memory::recorder_type::INNER_global_recorder().add_record(result, size);
    return result;
}
void operator delete(void * const block, std::align_val_t const) noexcept
{
    OKps::memory::recorder_type::INNER_global_recorder().erase_record(block);
    std::free(block);
}
#endif
