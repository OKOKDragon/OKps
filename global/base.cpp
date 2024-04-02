#include <iostream>
#include ".\base.hpp"

namespace OKps
{

    template <void * (*allocator)(std::size_t)>
    void * base::INNER_proxy_new(const std::size_t size)
    {

        void * result = nullptr;

        while (true)
        {
            bool TEMP_flag = false;
            if (base::MEMBER_lock.compare_exchange_strong(TEMP_flag, true))
            {
                result = allocator(size);

                base::MEMBER_pool.insert(std::make_pair(result, size));

                base::MEMBER_lock.store(false);

                break;
            }
        }

        return result;
    }
    void * base::operator new(const size_t size)
    {

        return base::INNER_proxy_new <::operator new>(size);
    }

    void base::pool_output(void * block) noexcept
    {
        while (true)
        {
            bool TEMP_flag = false;
            if (base::MEMBER_lock.compare_exchange_strong(TEMP_flag, true))
            {
                base::MEMBER_pool.erase(block);

                base::MEMBER_lock.store(false);

                break;
            }
        }
    }

    void base::operator delete(void * block) noexcept
    {
        base::pool_output(block);

        ::operator delete(block);
    }

    void * base::operator new[](const size_t size)
    {
        return base::INNER_proxy_new <::operator new[]>(size);
    }

        void base::operator delete[](void * block) noexcept
    {

        base::pool_output(block);
        ::operator delete[](block);
    }

        base::base() noexcept
    {
    }
    base::~base() noexcept
    {
    }
    base::base(const base &) noexcept
    {
    }
    base::base(base &&) noexcept
    {
    }
    void base::operator=(const base &) noexcept
    {
    }
    void base::operator=(base &&) noexcept
    {
    }

    basic_no_heap::basic_no_heap() noexcept
    {
    }
    basic_no_heap::~basic_no_heap() noexcept
    {
    }
    basic_no_heap::basic_no_heap(const basic_no_heap &) noexcept
    {
    }
    basic_no_heap::basic_no_heap(basic_no_heap &&) noexcept
    {
    }
    void basic_no_heap::operator=(const basic_no_heap &) noexcept
    {
    }
    void basic_no_heap::operator=(basic_no_heap &&) noexcept
    {
    }


}
