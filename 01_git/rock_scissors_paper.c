#include "rock_scissors_paper.h"
#include <stdio.h>
#include <stdlib.h>
#include <openssl/md5.h>

void printf_md5(const char* const message, const enum GameValue value)
{
    unsigned char digest[MD5_DIGEST_LENGTH];

    MD5((unsigned char*)&value, sizeof(value), (unsigned char*)&digest);

    char mdString[33];

    for(int i = 0; i < 16; ++i)
    {
        sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
    }

    printf(message, mdString);
}

const char* const to_string(const enum GameValue value)
{
    switch(value)
    {
    case Rock:
        return "Rock";
    case Paper:
        return "Paper";
    case Scissors:
        return "Scissors";
    }
}

enum GameResult get_game_result(const enum GameValue player, const enum GameValue computer)
{
    if (player == computer)
    {
        return Draw;
    }

    if ((player == Scissors && computer == Paper) || (player == Rock && computer == Scissors) ||
        (player == Paper && computer == Rock))
        return Win;

    return Lose;
}

enum GameValue generate_computer_move()
{
    return (enum GameValue) GameValue_First + (rand() % (GameValue_Last - GameValue_First + 1));
}
