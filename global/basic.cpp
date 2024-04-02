#include <iostream>
#include ".\basic.hpp"

namespace OKps
{

    template <void * (*allocator)(std::size_t)>
    void * basic::INNER_proxy_new(const std::size_t size)
    {

        void * result = nullptr;

        while (true)
        {
            bool TEMP_flag = false;
            if (basic::MEMBER_lock.compare_exchange_strong(TEMP_flag, true))
            {
                result = allocator(size);

                basic::MEMBER_pool.insert(std::make_pair(result, size));

                basic::MEMBER_lock.store(false);

                break;
            }
        }

        return result;
    }
    void * basic::operator new(const size_t size)
    {

        return basic::INNER_proxy_new <::operator new>(size);
    }

    void basic::pool_output(void * block) noexcept
    {
        while (true)
        {
            bool TEMP_flag = false;
            if (basic::MEMBER_lock.compare_exchange_strong(TEMP_flag, true))
            {
                basic::MEMBER_pool.erase(block);

                basic::MEMBER_lock.store(false);

                break;
            }
        }
    }

    void basic::operator delete(void * block) noexcept
    {
        basic::pool_output(block);

        ::operator delete(block);
    }

    void * basic::operator new[](const size_t size)
        {
            return basic::INNER_proxy_new <::operator new[]>(size);
        }

        void basic::operator delete[](void * block) noexcept
            {

                basic::pool_output(block);
                ::operator delete[](block);
            }



            basic::basic() noexcept
            {

            }
            basic::~basic() noexcept
            {
            }
            basic::basic(const basic &) noexcept
            {
            }
            basic::basic(basic &&) noexcept
            {
            }
            void basic::operator=(const basic &) noexcept
            {
            }
            void basic::operator=(basic &&) noexcept
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
