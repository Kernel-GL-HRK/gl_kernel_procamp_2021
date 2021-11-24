#include "choice.h"

#include <iostream>

enum class Result : int
{
    PLAYER_WINS = 0,
    CPU_WINS,
    DRAW
};

using namespace game;

namespace
{

const char* const gaGameResStrRepr[]
{
    "You win", // PLAYER_WINS
    "I win",   // CPU_WINS
    "Draw!"    // DRAW
};

const Result gaResolveTbl[][Choice::ToInt(CVal::CHOICE_LAST)]
{
    //      SCISSORS        |        PAPER         |     ROCK           <- Player's choice
    //
    //                                                                       CPU's choice
    //                                                                             |
    //                                                                             V
    {    Result::DRAW,        Result::PLAYER_WINS,     Result::CPU_WINS    },  // SCISSORS
    {  Result::CPU_WINS,         Result::DRAW,        Result::PLAYER_WINS  },  // PAPER
    {  Result::PLAYER_WINS,    Result::CPU_WINS,         Result::DRAW      }   // ROCK
};

void resolveGame(CVal rePlayerChoice, CVal reCpuChoice)
{
    const auto eResult = gaResolveTbl
        [Choice::ToInt(rePlayerChoice)]
        [Choice::ToInt(reCpuChoice)];

    std::cout << gaGameResStrRepr[static_cast<int>(eResult)];

    if (eResult != Result::DRAW)
    {
        const auto eFirstNoun = (eResult == Result::CPU_WINS) ? reCpuChoice : rePlayerChoice;
        const auto eSecondNoun = (eFirstNoun == reCpuChoice) ? rePlayerChoice : reCpuChoice;
        std::cout << ": " << Choice::ToString(eFirstNoun);
        std::cout << " beat" << (eFirstNoun != CVal::SCISSORS ? "s" : "");
        std::cout << " " << Choice::ToString(eSecondNoun);
    }

    std::cout << std::endl;
}
} // namespace

int main()
{
    ChoiceStdin tPlayerInput;
    ChoiceRandom tCpuInput;

    while (true)
    {

        // Make CPU's move first, we need to play fair.
        const auto eCpuChoice = tCpuInput.Get();
        const auto ePlayerChoice = tPlayerInput.Get();
        if (ePlayerChoice == CVal::QUIT)
        {
            break;
        }

        std::cout << "You chose " << Choice::ToString(ePlayerChoice);
        std::cout << ", I chose " << Choice::ToString(eCpuChoice) << std::endl;
        resolveGame(ePlayerChoice, eCpuChoice);
    }

    std::cout << "Bye!" << std::endl;
}
