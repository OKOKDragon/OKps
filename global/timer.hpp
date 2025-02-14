#pragma once

#include <chrono>

namespace OKps
{

    class time_monitor final
    {
    private:
        std::chrono::steady_clock::time_point MEMBER_begin_time;

        template <typename time_type>
        using end_result_type = decltype(std::chrono::time_point_cast<time_type>(std::chrono::high_resolution_clock::now()).time_since_epoch().count());
    public:
        time_monitor()noexcept;
        ~time_monitor()noexcept;
        time_monitor(time_monitor const &) = delete;
        time_monitor(time_monitor &&) = delete;
        void operator=(time_monitor const &) = delete;
        void operator=(time_monitor &&) = delete;

        void begin() noexcept;

        template <typename time_type>
        end_result_type<time_type> end() noexcept;

    };

    template time_monitor::end_result_type<std::chrono::microseconds> time_monitor::end<std::chrono::microseconds>();
    template time_monitor::end_result_type<std::chrono::nanoseconds> time_monitor::end<std::chrono::nanoseconds>();
    template time_monitor::end_result_type<std::chrono::milliseconds> time_monitor::end<std::chrono::milliseconds>();
    template time_monitor::end_result_type<std::chrono::seconds> time_monitor::end<std::chrono::seconds>();
    template time_monitor::end_result_type<std::chrono::minutes> time_monitor::end<std::chrono::minutes>();
    template time_monitor::end_result_type<std::chrono::hours> time_monitor::end<std::chrono::hours>();

}