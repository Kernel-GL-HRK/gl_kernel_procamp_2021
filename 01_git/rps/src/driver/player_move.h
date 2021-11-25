/*
* COPYRIGHT (C) 2021 Oleksandr Aleksieiev
* all rights reserved
*/

#ifndef ROCK_PAPER_SCISSORS_PLAYER_MOVE_H
#define ROCK_PAPER_SCISSORS_PLAYER_MOVE_H

#include <random>

#include "../datatypes/enums.h"

namespace rps {

    class PlayerMove {
    public:
        PlayerMove();
        explicit PlayerMove(const char &input);
        Result operator==(const PlayerMove &user);
        [[nodiscard]] Option getChoice() const;
        bool isValid();
        void print(const std::string &prefix) const;

    private:
        static Option makeMove() {
            std::random_device randomDevice_;
            std::default_random_engine e1(randomDevice_());
            std::uniform_int_distribution<int> uniform_dist(0, 70);
            return static_cast<Option>(uniform_dist(e1) % 3);
        }

        Option choice_;
    };
}// namespace rps

#endif//ROCK_PAPER_SCISSORS_PLAYER_MOVE_H
