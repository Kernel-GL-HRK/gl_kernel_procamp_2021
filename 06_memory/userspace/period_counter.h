/*
 * COPYRIGHT (C) 2021 Oleksandr Aleksieiev
 * all rights reserved
*/
#ifndef MEMORY_ALLOCATOR_TEST_BENCH__PERIOD_COUNTER_H
#define MEMORY_ALLOCATOR_TEST_BENCH__PERIOD_COUNTER_H

#include <chrono>
#include <ostream>

using namespace std::chrono;

namespace time_manager {

    using NanoPoint = time_point<system_clock, nanoseconds>;
    using NanoDuration = duration<double, std::nano>;
    class PeriodCounter {
    public:
        explicit PeriodCounter() = default;
        PeriodCounter(const PeriodCounter&) = delete;
        PeriodCounter(const PeriodCounter&&) = delete;
        PeriodCounter& operator=(const PeriodCounter&) = delete;

        friend std::ostream &operator<<(std::ostream &out, const PeriodCounter &t);

        void makeStamp();

    private:
        NanoPoint last_measured_{};
        NanoDuration delta_{std::chrono::nanoseconds::zero()};
        bool is_first_time_{true};
    };
}// namespace time_manager

#endif// MEMORY_ALLOCATOR_TEST_BENCH__PERIOD_COUNTER_H
