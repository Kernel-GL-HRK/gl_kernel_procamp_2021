/*
* COPYRIGHT (C) 2021 Oleksandr Aleksieiev
* all rights reserved
*/

#include "period_counter.h"

#include <cinttypes>
#include <iomanip>
#include <iostream>

int main() {

    uint64_t pwr = 1;
    uint8_t *mem_block_ = nullptr;
    time_manager::PeriodCounter delta_time;

    while (mem_block_ != nullptr || pwr == 1) {
        std::cout << "Size: " << pwr << std::endl;

        delta_time.makeStamp();
        mem_block_ = new (std::nothrow) uint8_t[pwr];
        delta_time.makeStamp();
        std::cout << "Creation: " << std::setw(20) << delta_time << std::endl;

        delta_time.makeStamp();
        delete[] mem_block_;
        delta_time.makeStamp();
        std::cout << "Removal: " << std::setw(20) << delta_time << std::endl;

        pwr <<= 1;
    }

    return 0;
}
