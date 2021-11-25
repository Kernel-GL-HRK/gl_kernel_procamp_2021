
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

int main()
{
    enum {
        ROCK,
        PAPER,
        SCISSORS,
        VARS_NUM
    };
    const uint8_t buf_lenght = 16;
    char buf[buf_lenght];
    int player, ai, game_active = 1;
    const char* const option[] = {"rock", "paper", "scissors"};

    while(game_active) {
        
        printf("Please choose: rock (r) - paper (p) - scissors (s) and press \"Enter\"\n");
        
        fgets(buf, sizeof(buf), stdin);
        
        switch((char)buf[0]) {
            case 'r':
                player = ROCK;
                break;
            case 'p':
                player = PAPER;
                break;
            case 's':
                player = SCISSORS;
                break;
            default:
                printf("WRONG INPUT!!!\n\n");
                continue;
        }
        
        srand(time(NULL));
        ai = rand() % VARS_NUM;
        printf("You choose %s, I choose %s\n", option[player], option[ai]);
        
        if(player == ai) {
            printf("Draw... Try again!\n\n");
        }
        
        if((player==ROCK && ai==SCISSORS) || (player==PAPER && ai==ROCK) || (player==SCISSORS && ai==PAPER)) {
            printf("You win: %s beats %s\n\n", option[player], option[ai]);
        } else {
            printf("I win: %s beats %s\n\n", option[ai], option[player]);
        }
        game_active = 0;
    }
	return 0;
}
