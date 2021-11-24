#include "rock_scissors_paper.h"

#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

int main()
{
    srand(time(0));
    
    printf("Please choose: rock (r) - paper (p) - scissors (s)\n");

    enum GameValue computer_move = generate_computer_move();

    printf_md5("MD5 of computer move: %s\n", computer_move);

    char user_input = getchar();
    enum GameValue user_move = GameValue_Undefined;
    switch(user_input)
    {
    case 'r':
        user_move = Rock;
        break;
    case 's':
        user_move = Scissors;
        break;
    case 'p':
        user_move = Paper;
        break;
    default:
        printf("Invalid input\n");
        return -1;
    }
    
    printf("You choose %s, I choose %s\n", to_string(user_move), to_string(computer_move));

    switch(get_game_result(user_move, computer_move))
    {
    case Win:
        printf("You win: %s beats %s\n", to_string(user_move), to_string(computer_move));
        break;
    case Draw:
        printf("It's a draw\n");
        break;
    case Lose:
        printf("I win: %s beats %s\n", to_string(computer_move), to_string(user_move));
        break;
    }
}
