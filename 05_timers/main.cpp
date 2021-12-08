#include <iostream>
#include "time_manager.h"

int main() {
    size_t ids = time_manager::TimeManager::getClockIdsNumber();

    while (ids) {
        try {
            time_manager::TimeManager timeManager(time_manager::TimeManager::getClockIdByNumber(ids-1));
            std::cout << timeManager;
        } catch (const std::invalid_argument &ex) {
            std::cout << ids-1 << ": " << ex.what() << std::endl;
        }
        ids--;
    }

    return 0;
}
