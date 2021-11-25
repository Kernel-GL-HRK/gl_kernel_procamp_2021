/*
* COPYRIGHT (C) 2021 Oleksandr Aleksieiev
* all rights reserved
*/

#include <iostream>

#include "player_move.h"

using namespace rps;

PlayerMove::PlayerMove(const char &input) {
    switch (input) {
        case 'r':
        case 'R':
            choice_ = Option::Rock;
            break;
        case 'p':
        case 'P':
            choice_ = Option::Paper;
            break;
        case 's':
        case 'S':
            choice_ = Option::Scissors;
            break;
        default:
            choice_ = Option::Invalid;
    };
}

PlayerMove::PlayerMove()
    :choice_(PlayerMove::makeMove()){}

Result PlayerMove::operator==(const PlayerMove &user) {
    if (choice_ == user.getChoice()) {
        return Result::Draft;
    } else if (choice_ == Option::Rock && user.getChoice() == Option::Scissors) {
        return Result::Win;
    } else if (choice_ == Option::Scissors && user.getChoice() == Option::Paper) {
        return Result::Win;
    } else if (choice_ == Option::Paper && user.getChoice() == Option::Rock) {
        return Result::Win;
    } else return Result::Lose;
}
Option PlayerMove::getChoice() const {
    return choice_;
}
bool PlayerMove::isValid() {
    return choice_ != Option::Invalid;
}

void PlayerMove::print(const std::string& prefix) const {
    std::cout << prefix << (choice_ == Option::Scissors
                            ? "Scissors"
                            : choice_ == Option::Paper ? "Paper" : "Rock") << std::endl;
}
