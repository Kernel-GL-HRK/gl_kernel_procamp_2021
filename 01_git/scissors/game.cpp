#include <cstdlib>
#include <iostream>

// Not using enum class since too lazy to
// do explicit casts to size_t everywhere.
enum Choice
{
    SCISSORS = 0,
    PAPER,
    ROCK,
    CHOICE_LAST
};

enum GameResult
{
    PLAYER_WINS = 0,
    CPU_WINS,
    DRAW,
    GR_LAST
};

namespace
{

const char* gaItemsStrRepr[]
{
    "scissors", // SCISSORS
    "paper",    // PAPER
    "rock"      // ROCK
};

const char* gaGameResStrRepr[]
{
    "You win", // PLAYER_WINS
    "I win",   // CPU_WINS
    "Draw!"    // DRAW
};

const GameResult gaResolveTbl[][CHOICE_LAST]
{
    //   SCISSORS    |     PAPER     |     ROCK      <- Player's choice
    //
    //                                                  CPU's choice
    //                                                     |
    //                                                     V
    {    DRAW,          PLAYER_WINS,     CPU_WINS    },  // SCISSORS
    {  CPU_WINS,           DRAW,        PLAYER_WINS  },  // PAPER
    {  PLAYER_WINS,      CPU_WINS,         DRAW      }   // ROCK
};

void resolveGame(Choice rePlayerChoice, Choice reCpuChoice)
{
    const auto eResult = gaResolveTbl[rePlayerChoice][reCpuChoice];
    std::cout << gaGameResStrRepr[eResult];

    if (eResult != DRAW)
    {
        const auto eFirstNoun = (eResult == CPU_WINS) ? reCpuChoice : rePlayerChoice;
        const auto eSecondNoun = (eFirstNoun == reCpuChoice) ? rePlayerChoice : reCpuChoice;
        std::cout << ": " << gaItemsStrRepr[eFirstNoun];
        std::cout << " beat" << (eFirstNoun != SCISSORS ? "s" : "");
        std::cout << " " << gaItemsStrRepr[eSecondNoun];
    }

    std::cout << std::endl;
}

Choice decodeChoice(int rdRawChoice)
{
    switch (rdRawChoice)
    {
    case 's':
        return SCISSORS;
    case 'p':
        return PAPER;
    case 'r':
        return ROCK;
    default:
        return CHOICE_LAST;
    }
}

Choice makeCpuChoice()
{
    while (true)
    {
        const auto dRand = rand() % CHOICE_LAST;
        if (dRand < CHOICE_LAST)
        {
            return Choice(dRand);
        }
    }
}
} // namespace

int main()
{
    std::srand(time(nullptr));

    int cInput = 0;
    bool bSkipHeading = false;

    while (true)
    {
        if (not bSkipHeading)
        {
            std::cout << "Please choose: rock (r) - paper (p) - scissors (s)" << std::endl;
        }
        else
        {
            bSkipHeading = false;
        }

        cInput = std::tolower(std::getchar());
        if (cInput == 'q' or cInput == EOF)
        {
            break;
        }

        // Eat up LFs and CRs, do not repeat the prompt above.
        if (cInput == '\n' or cInput == '\r')
        {
            bSkipHeading = true;
            continue;
        }

        // Make CPU's move first, we need to play fair.
        const auto eCpuChoice = makeCpuChoice();

        const auto ePlayerChoice = decodeChoice(cInput);
        if (ePlayerChoice == CHOICE_LAST)
        {
            std::cout << "Wrong input";
            if (std::isalnum(cInput))
            {
                // Check letters & numbers only, do not overcomplicate things.
                std::cout << ": " << char(cInput);
            }

            std::cout << "!" << std::endl << "Press 'q' or '^D' to exit" << std::endl;
            continue;
        }

        std::cout << "You chose " << gaItemsStrRepr[ePlayerChoice];
        std::cout << ", I chose " << gaItemsStrRepr[eCpuChoice] << std::endl;
        resolveGame(ePlayerChoice, eCpuChoice);
    }

    std::cout << "Bye!" << std::endl;
}
