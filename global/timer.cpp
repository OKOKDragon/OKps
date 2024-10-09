#include ".\timer.hpp"

namespace OKps
{

    time_monitor::time_monitor()noexcept
        : MEMBER_begin_time(std::chrono::high_resolution_clock::now())
    {
    }

    time_monitor::~time_monitor()noexcept
    {
        // this->end();
    }
    void time_monitor::begin() noexcept
    {
        this->MEMBER_begin_time = std::chrono::high_resolution_clock::now();
    }
    template <typename time_type>
    time_monitor::end_result_type<time_type> time_monitor::end() noexcept
    {
        auto TEMP_end_time = std::chrono::high_resolution_clock::now();
        auto const RESULT_begin_time = std::chrono::time_point_cast<time_type>(this->MEMBER_begin_time).time_since_epoch().count();
        auto const RESULT_end_time = std::chrono::time_point_cast<time_type>(TEMP_end_time).time_since_epoch().count();
        auto const result = RESULT_end_time - RESULT_begin_time;
        static_assert(std::is_same_v<std::remove_const_t<decltype(result)>, end_result_type<time_type>>);
        return result;
    }

}