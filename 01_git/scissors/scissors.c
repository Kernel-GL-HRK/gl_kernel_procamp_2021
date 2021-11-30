#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static char *options[] = {
    "rock",
    "paper",
    "scissors"
};

static char *options_user[] = {
    ['r'] = "rock",
    ['p'] = "paper",
    ['s'] = "scissors"
};

enum {
    ROCK = 0,
    PAPER,
    SCISSORS,
};

static char *game_logic[][3] = {
    ['r'] = {
        [ROCK] = "Draw",
        [PAPER] = "I win: paper beats rock",
        [SCISSORS] = "You win: rock beats scissors"
    },
    ['p'] = {
        [ROCK] = "You win: paper beats rock",
        [PAPER] = "Draw",
        [SCISSORS] = "I win: scissors beats paper"
    },
    ['s'] = {
        [ROCK] = "I win: rock beats scissors",
        [PAPER] = "You win: scissors beats paper",
        [SCISSORS] = "Draw"
    },
};

int main()
{
    int ch;
    char ch_user;

    // Use current time as seed
    srand(time(0));   

    printf("Please choose: rock (r) - paper (p) - scissors (s). q - exit.\n");

    while(1) {

        // wait for user choise
        scanf("%1s", &ch_user);

        // quit if user requested
        if ('q' == ch_user)
            break;

        if (('r' == ch_user)
            || ('p' == ch_user)
            || ('s' == ch_user)) {

            ch = rand() % 3; // 0 - rock, 1 - paper, 2 - scissors

            printf("You choose %s, I choose %s\n", options_user[ch_user], options[ch]);
            printf("%s\n", game_logic[ch_user][ch]);
        } else {
            printf("Coudn't understand your choise. Please try again.\n");
        }
    }
}
