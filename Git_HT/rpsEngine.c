#include <stdio.h>
#include <stdlib.h>
#include "rpsEngine.h"


static enum {Paper = 0, Scissors, Rock}Choises;
static char paperString[] = "Paper";
static char scissorsString[] = "Scissors";
static char rockString[] = "Rock";
static char * chNames[] = {paperString, scissorsString, rockString};

void RPC_Run(void){
    char userChoise = 0, compChoise = 0;
    char chcode = 'p';

    while ((userChoise != 'q') || (userChoise != 'Q'))
    {
        printf("Please choose: rock (r) - paper (p) - scissors (s)\n");
        printf("(enter \'q\' for quit)\n");


        compChoise = rand() & 3;
        scanf("%c", &userChoise);

        if(userChoise == 'q')
            return;

        userChoise -= chcode;

        if(userChoise == compChoise)
            continue;

        printf("%d, %c\n", userChoise, userChoise);
        
	if((userChoise < compChoise) || ((userChoise == Paper) && (compChoise == Rock))){
            printf("You choose %s, I choose %s \n I win!\n", chNames[userChoise], chNames[compChoise]);
        }
        else {
            printf("You choose %s, I choose %s \nYou win!\n", chNames[userChoise], chNames[compChoise]);
        }
    }
}
