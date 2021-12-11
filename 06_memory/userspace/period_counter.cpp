/*
 * COPYRIGHT (C) 2021 Oleksandr Aleksieiev
 * all rights reserved
*/
#include "period_counter.h"

#include <iostream>

namespace time_manager {

    void PeriodCounter::makeStamp() {
        static NanoPoint now;
        now = system_clock::now();
        if (is_first_time_) {
            last_measured_ = now;
            is_first_time_ = false;
        }
        delta_ = now - last_measured_;
        last_measured_ = now;
    }

    std::ostream &operator<<(std::ostream &out, const PeriodCounter &t) {
        std::cout << t.delta_.count() << " ns.";
        return out;
    }

}// namespace time_manager
