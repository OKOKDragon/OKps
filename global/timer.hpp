#pragma once

#include ".\base.hpp"

namespace OKps
{
    class time_monitor : public base
    {
    private:
        std::chrono::steady_clock::time_point MEMBER_begin_time;

    public:
        time_monitor();
        ~time_monitor();
        time_monitor(time_monitor const &) = delete;
        time_monitor(time_monitor &&) = delete;
        void operator=(time_monitor const &) = delete;
        void operator=(time_monitor &&) = delete;

        void begin();
        template <typename time_type>
        long long end();

    };

    template long long time_monitor::end<std::chrono::microseconds>();
    template long long time_monitor::end<std::chrono::nanoseconds>();
    template long long time_monitor::end<std::chrono::milliseconds>();
    template long long time_monitor::end<std::chrono::seconds>();
    template long long time_monitor::end<std::chrono::minutes>();
    template long long time_monitor::end<std::chrono::hours>();

}