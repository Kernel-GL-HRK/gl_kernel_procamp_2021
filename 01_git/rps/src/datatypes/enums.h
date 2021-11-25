/*
* COPYRIGHT (C) 2021 Oleksandr Aleksieiev
* all rights reserved
*/


#ifndef ROCK_PAPER_SCISSORS_ENUMS_H
#define ROCK_PAPER_SCISSORS_ENUMS_H

#include <cstdint>

namespace rps {
    enum class Option : uint8_t {
        Invalid = 0xFF,
        Rock = 0x00,
        Paper = 0x01,
        Scissors = 0x02
    };

    enum class Result : uint8_t {
        Draft = 0x00,
        Lose = 0x01,
        Win = 0x02
    };
}// namespace rps

#endif//ROCK_PAPER_SCISSORS_ENUMS_H
