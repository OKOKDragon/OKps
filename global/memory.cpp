#include ".\memory.hpp"

#ifdef MACRO_OKps_enable_global_memory_record

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
    recorder_type const & recorder_type::global_recorder()noexcept
    {
        return recorder_type::INNER_global_recorder();
    }

    recorder_type::recorder_type()
        noexcept(std::is_nothrow_default_constructible_v<decltype(recorder_type::MEMBER_lock)>
        and std::is_nothrow_default_constructible_v<pool_type>)
        :MEMBER_pool(pool_type::allocator_type())
    {
    }

    recorder_type::~recorder_type()
        noexcept(std::is_nothrow_destructible_v<decltype(recorder_type::MEMBER_lock)>
        and std::is_nothrow_destructible_v<pool_type>)
    {
    }



    recorder_type::pool_type recorder_type::track()const
    {
        std::lock_guard<decltype(this->MEMBER_lock)> TEMP_lock(this->MEMBER_lock);
        recorder_type::pool_type result = this->MEMBER_pool;
        return result;
    }


    void recorder_type::add_record(void const * const block, std::size_t const size)
    {
        std::lock_guard<decltype(this->MEMBER_lock)> TEMP_lock(this->MEMBER_lock);
        auto const i = this->MEMBER_pool.find(block);
        if (i == this->MEMBER_pool.end())
        {
            this->MEMBER_pool.insert(std::make_pair(block, size));
        }
        else
        {
            throw std::invalid_argument("此地址已存在");
        }
    }
    void recorder_type::erase_record(void const * const block)
    {
        std::lock_guard<decltype(this->MEMBER_lock)> TEMP_lock(this->MEMBER_lock);
        this->MEMBER_pool.erase(block);
    }

    recorder_type & recorder_type::INNER_global_recorder()
    {
        static recorder_type INNER_recorder = recorder_type();
        return INNER_recorder;
    }
}

void * operator new(std::size_t const size)
{
    void * result = std::malloc(size);
    if (not result)
    {
        throw std::bad_alloc();
    }
    OKps::memory::recorder_type::INNER_global_recorder().add_record(result, size);
    return result;
}
void operator delete(void * const block) noexcept
{
    OKps::memory::recorder_type::INNER_global_recorder().erase_record(block);
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

#endif