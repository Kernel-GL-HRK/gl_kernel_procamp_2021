/*
* COPYRIGHT (C) 2021 Oleksandr Aleksieiev
* all rights reserved
*/

#ifndef ROCK_PAPER_SCISSORS_USER_INTERFACE_H
#define ROCK_PAPER_SCISSORS_USER_INTERFACE_H

#include "../datatypes/enums.h"
#include <iostream>

namespace rps {

    class UserInterface {
    public:
        static void printInviteMessage(std::ostream& out) {
            out << "Please choose: rock (r) - paper (p) - scissors (s)"
                      << std::endl;
            out << "Or press x to leave." << std::endl;
        }

        static void printInvalidInputMessage(std::ostream& out) {
            out << "Oh, I got it. Nice joke." << std::endl;
        }

        static void printOption(std::ostream& out, const std::string& prefix, Option pc) {
            out << prefix << (pc == Option::Scissors
                    ? "Scissors"
                    : pc == Option::Paper ? "Paper" : "Rock") << std::endl;
        }

        static void printExitMessage(std::ostream& out) {
            out << "Bye-bye" << std::endl;
        }

        static void printGameResult(std::ostream& out, Result result) {
            switch (result) {
                case Result::Draft :
                    out << "It's a draft." << std::endl;
                    break;
                case Result::Lose :
                    out << "You win." << std::endl;
                    break;
                case Result::Win :
                    out << "I win." << std::endl;
                    break;
            }
            std::cout << std::endl << std::endl << std::endl;
        }

        static Option makeUserOption(const char &option) {
            Option result;
            switch (option) {
                case 'r':
                case 'R':
                    result = Option::Rock;
                    break;
                case 'p':
                case 'P':
                    result = Option::Paper;
                    break;
                case 's':
                case 'S':
                    result = Option::Scissors;
                    break;
                default:
                    result = Option::Invalid;
            };
            return result;
        }
    };
}// namespace rps

#endif//ROCK_PAPER_SCISSORS_USER_INTERFACE_H
