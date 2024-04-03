#include ".\timer.hpp"

namespace OKps
{
    time_monitor::time_monitor()
        : MEMBER_begin_time(std::chrono::high_resolution_clock::now())
    {
    }

    time_monitor::~time_monitor()
    {
        // this->end();
    }
    void time_monitor::begin()
    {
        this->MEMBER_begin_time = std::chrono::high_resolution_clock::now();
    }
    template <typename time_type>
    long long time_monitor::end()
    {
        auto TEMP_end_time = std::chrono::high_resolution_clock::now();
        auto RESULT_begin_time = std::chrono::time_point_cast<time_type>(this->MEMBER_begin_time).time_since_epoch().count();
        auto RESULT_end_time = std::chrono::time_point_cast<time_type>(TEMP_end_time).time_since_epoch().count();
        return (RESULT_end_time - RESULT_begin_time);
    }
}