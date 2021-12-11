#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define STR_WIN_RESULT "> %s win: %s beats %s.\n"

enum shape_type {
    SHAPE_TYPE_ROCK,
    SHAPE_TYPE_PAPER,
    SHAPE_TYPE_SCISSORS
};

static const char *shape_str(enum shape_type type)
{
    switch( type )
    {
        case SHAPE_TYPE_ROCK: return "rock";
        case SHAPE_TYPE_PAPER: return "paper";
        case SHAPE_TYPE_SCISSORS: return "scissors";
        default: return NULL;
    }

}

static enum shape_type get_random_shape(void)
{
    srand(time(NULL));
    return rand() % 3;
}

int main(void)
{
    do {
        printf("> Please choose: rock (r) - paper (p) - scissors (s): \n");
        char choose_str[80];
        scanf("\n%s", choose_str);
        char choose = choose_str[0];
        if ((strlen(choose_str) != 1) || (choose != 'r' && choose != 'p' && choose != 's')) {
            fprintf(stderr, "Invalid argument: %s\n", choose_str);
            return -EINVAL;
        }

        enum shape_type player_choose = SHAPE_TYPE_ROCK;
        switch(choose)
        {
            case 'r':
                player_choose = SHAPE_TYPE_ROCK;
                break;

            case 'p':
                player_choose = SHAPE_TYPE_PAPER;
                break;

            case 's':
                player_choose = SHAPE_TYPE_SCISSORS;
                break;
        }
        enum shape_type computer_choose = get_random_shape();
        printf("> You choose %s, I choose %s\n", shape_str(player_choose), shape_str(computer_choose));

        bool is_dead_head = player_choose == computer_choose;
        if (is_dead_head) {
            printf(">  Dead head\n");
            continue;
        }

        bool is_player_win = false;
        switch(player_choose)    {
            case SHAPE_TYPE_ROCK:
                is_player_win = computer_choose == SHAPE_TYPE_SCISSORS;
                break;

            case SHAPE_TYPE_PAPER:
                is_player_win = computer_choose == SHAPE_TYPE_ROCK;
                break;

            case SHAPE_TYPE_SCISSORS:
                is_player_win = computer_choose == SHAPE_TYPE_PAPER;
                break;
        }

        printf(STR_WIN_RESULT, is_player_win ? "You" : "I",
            is_player_win ? shape_str(player_choose) : shape_str(computer_choose),
            is_player_win ? shape_str(computer_choose) : shape_str(player_choose));
    } while(true);

    return 0;
}
