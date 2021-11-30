
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define CHAR_ROCK         ('r')
#define CHAR_PAPER        ('p')
#define CHAR_SCISSORS     ('s')
#define BUF_LENGTH        (16u)

int main()
{
    enum {
        ROCK,
        PAPER,
        SCISSORS,
        VARS_NUM
    };
    char buf[BUF_LENGTH];
    int player, ai, game_active = 1;
    const char* const option[] = {"rock", "paper", "scissors"};

    while (game_active) {

        printf("Please choose: rock (%c) - paper (%c) - scissors (%c) and press \"Enter\"\n", CHAR_ROCK, CHAR_PAPER, CHAR_SCISSORS);

        fgets(buf, sizeof(buf), stdin);

        switch((char)buf[0]) {
            case CHAR_ROCK:
                player = ROCK;
                break;
            case CHAR_PAPER:
                player = PAPER;
                break;
            case CHAR_SCISSORS:
                player = SCISSORS;
                break;
            default:
                printf("WRONG INPUT!!!\n\n");
                continue;
        }

        ai = rand() % VARS_NUM;
        printf("You choose %s, I choose %s\n", option[player], option[ai]);

        if (player == ai) {
            printf("Draw... Try again!\n\n");
        }

        if ((player==ROCK && ai==SCISSORS) || (player==PAPER && ai==ROCK) || (player==SCISSORS && ai==PAPER)) {
            printf("You win: %s beats %s\n\n", option[player], option[ai]);
        } else {
            printf("I win: %s beats %s\n\n", option[ai], option[player]);
        }
        game_active = 0;
    }
    return 0;
}
