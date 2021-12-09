/*
 * COPYRIGHT (C) 2021 Oleksandr Aleksieiev
 * all rights reserved
*/
#ifndef INC_05_TIMERS__TIME_MANAGER_H
#define INC_05_TIMERS__TIME_MANAGER_H

#include <cstdint>
#include <ctime>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace time_manager {

    class TimeManager {
    public:
        explicit TimeManager(clockid_t const &type);
        ~TimeManager() = default;

        static size_t getClockIdsNumber();
        static clockid_t getClockIdByNumber(size_t n);

        friend std::ostream &operator<<(std::ostream &out, const TimeManager &t);

    private:
        clockid_t clock_type_;
        timespec res_{};
        timespec tp_{};
        static const std::map<const clockid_t, std::string> validClockIdsMap;

        static void reconfigureTimeManager(TimeManager &m, clockid_t const &type);
    };

}// namespace time_manager


#endif// INC_05_TIMERS__TIME_MANAGER_H
