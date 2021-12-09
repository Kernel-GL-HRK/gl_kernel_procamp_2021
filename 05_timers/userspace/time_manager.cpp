/*
 * COPYRIGHT (C) 2021 Oleksandr Aleksieiev
 * all rights reserved
*/
#include "time_manager.h"
#include <iomanip>

namespace time_manager {
    const std::map<const clockid_t, std::string> TimeManager::validClockIdsMap = {
            /* Identifier for system-wide realtime clock.  */
            {0, "REALTIME"},
            /* Monotonic system-wide clock.  */
            {1, "MONOTONIC"},
            /* High-resolution timer from the CPU.  */
            {2, "PROCESS_CPUTIME_ID"},
            /* Thread-specific CPU-time clock.  */
            {3, "THREAD_CPUTIME_ID"},
            /* Monotonic system-wide clock, not adjusted for frequency scaling.  */
            {4, "MONOTONIC_RAW"},
            /* Identifier for system-wide realtime clock, updated only on ticks.  */
            {5, "REALTIME_COARSE"},
            /* Monotonic system-wide clock, updated only on ticks.  */
            {6, "MONOTONIC_COARSE"},
            /* Monotonic system-wide clock that includes time spent in suspension.  */
            {7, "BOOTTIME"},
            /* Like CLOCK_REALTIME but also wakes suspended system.  */
            {8, "REALTIME_ALARM"},
            /* Like CLOCK_BOOTTIME but also wakes suspended system.  */
            {9, "BOOTTIME_ALARM"},
            /* Like CLOCK_REALTIME but in International Atomic Time.  */
            {11, "TAI"}};

    void TimeManager::reconfigureTimeManager(TimeManager &m, const clockid_t &type) {
        if (validClockIdsMap.find(type) == validClockIdsMap.end()) {
            throw std::invalid_argument("Can not configure timer");
        }

        if (clock_getres(m.clock_type_, &m.res_) != 0) {
            throw std::invalid_argument("Can not read res");
        }

        if (clock_gettime(m.clock_type_, &m.tp_) != 0) {
            throw std::invalid_argument("Can not read time");
        }
    }

    std::ostream &operator<<(std::ostream &out, const TimeManager &t) {
        auto i = TimeManager::validClockIdsMap.find(t.clock_type_);
        if (i != TimeManager::validClockIdsMap.end()) {
            out << std::setw(20) << i->second << ": ";
        }
        out << "time: " << std::setw(11) << t.tp_.tv_sec << "." << t.tp_.tv_nsec << "| ";
        out << "res: " << t.res_.tv_sec << "." << t.res_.tv_nsec;
        out << std::endl;

        return out;
    }

    size_t TimeManager::getClockIdsNumber() {
        return validClockIdsMap.size();
    }

    TimeManager::TimeManager(const clockid_t &type)
        : clock_type_(type) {
        reconfigureTimeManager(*this, clock_type_);
    }
    clockid_t TimeManager::getClockIdByNumber(size_t n) {
        auto i = validClockIdsMap.begin();
        std::advance(i, n);
        return i->first;
    }
}// namespace time_manager
