#include "choice.h"

#include <iostream>
#include <cassert>
#include <cstring>

#include <openssl/md5.h>

namespace
{

const char* const gaItemsStrRepr[]
{
    "scissors", // SCISSORS
    "paper",    // PAPER
    "rock"      // ROCK
};

} //namespace

namespace game
{
// Auxiliary stuff.

CVal decodeChoiceStdin(char rdRawChoice)
{
    switch (std::tolower(rdRawChoice))
    {
    // In-game items.
    case 's':
        return CVal::SCISSORS;
    case 'p':
        return CVal::PAPER;
    case 'r':
        return CVal::ROCK;

    // Control codes.
    case 'q':
    // FALLTHROUGH
    case EOF:
    // FALLTHROUGH
    case 0: // ^D - a corner-case for std::cin! In case ^D is pressed
            // we get into an infinite loop.
        return CVal::QUIT;
    case '\n':
    // FALLTHROUGH
    case '\r':
        return CVal::IGNORE;

    default:
        return CVal::CHOICE_LAST;
    }
}

CVal decodeChoiceInt(int rdRawChoice)
{
    for (auto i = Choice::ToInt(CVal::SCISSORS);
            i < Choice::ToInt(CVal::CHOICE_LAST); ++i)
    {
        if (i == rdRawChoice)
        {
            return static_cast<CVal>(i);
        }
    }

    return CVal::CHOICE_LAST;
}

// Choice class impl.

const char* const Choice::ToString(CVal reValue)
{
    assert(reValue < CVal::CHOICE_LAST);
    return gaItemsStrRepr[ToInt(reValue)];
}

void Choice::PrintMd5FromChoice(CVal reValue)
{
    assert(reValue < CVal::CHOICE_LAST);
    const auto pChoiceString = ToString(reValue);
    const auto dChoiceLength = std::strlen(pChoiceString);

    unsigned char aDigest[MD5_DIGEST_LENGTH] {};
    const auto pDigest = MD5(reinterpret_cast<const unsigned char*>(pChoiceString),
                             dChoiceLength, aDigest);
    std::cout << "MD5(choice): 0x";
    for (size_t i = 0; i < sizeof(aDigest); ++i)
    {
        std::cout << std::hex << short(pDigest[i]);
    }

    std::cout << std::endl;
}

// ChoiceStdin class impl.

CVal ChoiceStdin::Get()
{
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

        auto dInput = '\0';
        std::cin >> dInput;
        const auto eChoice {decodeChoiceStdin(dInput)};

        if (eChoice == CVal::IGNORE)
        {
            // Eat up LFs and CRs, do not repeat the prompt above.
            bSkipHeading = true;
            continue;
        }

        if (eChoice == CVal::CHOICE_LAST)
        {
            std::cout << "Wrong input";
            if (std::isalnum(dInput))
            {
                // Check letters & numbers only, do not overcomplicate things.
                std::cout << ": " << char(dInput);
            }

            std::cout << "!" << std::endl << "Press 'q' or '^C'/'^D' to exit" << std::endl;
            continue;
        }

        return eChoice;
    }
}

// ChoiceRandom class impl.

CVal ChoiceRandom::Get()
{
    const auto eChoice = decodeChoiceInt(mtUniDistr(mtRandSrc));
    assert(eChoice != CVal::CHOICE_LAST); // Double-checking UniDistr range.
    PrintMd5FromChoice(eChoice);
    return eChoice;
}

} // namespace game
