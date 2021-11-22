#include <stdio.h>
#include <stdlib.h>
#include <openssl/md5.h>

enum GameValue {Rock, Scissors, Paper};
enum GameResult {Win, Draw, Lose};

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
    return (enum GameValue) rand() % (Paper + 1);
}

// int main()
// {
//     char user_input = '\0';
//     while (user_input != 'q')
//     {
//         printf("Choose your move: Press \'r\' for rock,  \'s\' for scissors, \'p\' for paper or \'q\' for exit\n");
//         user_input = getchar();
//         enum GameValue user_move = Scissors;
//         switch(user_input)
//         {
//         case 'r':
//             user_move = Rock;
//             break;
//         case 's':
//             user_move = Scissors;
//             break;
//         case 'p':
//             user_move = Paper;
//             break;
//         default:
//             continue;
//         }
        
//         enum GameValue computer_move = generate_computer_move();
        
//         switch(get_game_result(user_move, computer_move))
//         {
//         case Win:
//             printf("You won the game: Your move: %s; Computer move: %s\n", to_string(user_move), to_string(computer_move));
//             break;
//         case Draw:
//             printf("You have a draw: Your move: %s; Computer move: %s\n", to_string(user_move), to_string(computer_move));
//             break;
//         case Lose:
//             printf("You lose the game: Your move: %s; Computer move: %s\n", to_string(user_move), to_string(computer_move));
//             break;
//         }
//     }
// }