
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
    char input[16];
    int player, ai;
    const char* option[] = {"rock", "paper", "scissors"};

    while(1) {
        
        printf("Please choose: rock (r) - paper (p) - scissors (s) and press \"Enter\"\n");
        
        fgets(input, sizeof(input), stdin);
        
        switch((char)input[0]) {
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
            continue;
        }
        
        if((player==ROCK && ai==SCISSORS) || (player==PAPER && ai==ROCK) || (player==SCISSORS && ai==PAPER)) {
            printf("You win: %s beats %s\n\n", option[player], option[ai]);
        } else {
            printf("I win: %s beats %s\n\n", option[ai], option[player]);
        }
        return 0;
    }
}
