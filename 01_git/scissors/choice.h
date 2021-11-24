#pragma once

#include <random>

namespace game
{

class Choice
{
public:

    enum class Value : int
    {
        // Game-related values.
        SCISSORS = 0,
        PAPER,
        ROCK,

        // Enum size & invalid input marker.
        CHOICE_LAST,
        // Crutch to skip some control chars, like CR & LF
        IGNORE,
        // "game quit" choice.
        QUIT
    };

    virtual Value Get() = 0;

    static constexpr int ToInt(Value reValue)
    {
        return static_cast<int>(reValue);
    }

    static const char* const ToString(Value reValue);
    static void PrintMd5FromChoice(Value reValue);

};

using CVal = Choice::Value;

class ChoiceStdin : public Choice
{
public:

    Value Get() override;
};

class ChoiceRandom : public Choice
{
    using UniDistr = std::uniform_int_distribution<>;

public:

    ChoiceRandom() :
        mtUniDistr(ToInt(CVal::SCISSORS), ToInt(CVal::ROCK))
    {
        std::random_device tDrbg;
        mtRandSrc.seed(tDrbg());
    }

    Value Get() override;

private:

    std::mt19937 mtRandSrc;
    UniDistr mtUniDistr;
};

} // namespace game
