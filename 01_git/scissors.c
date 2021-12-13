#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROCK 'r'
#define PAPER 'p'
#define SCISSORS 's'

const char arr[] = {ROCK, PAPER, SCISSORS};


int start_game(char input_user, char *machine_choice_c){
    srand(time(0));
    int machine_choice_int  = rand() % 3;
    *machine_choice_c = arr[machine_choice_int];
    int result;
    if(input_user == arr[machine_choice_int])
    {
        result = -1;
    }
    else if (input_user == ROCK && arr[machine_choice_int] == SCISSORS)
    {
        result = 1;
    }
    else if (arr[machine_choice_int] == ROCK && input_user == SCISSORS)
    {
        result = 0;
    }
    else if (input_user == PAPER && arr[machine_choice_int] == ROCK)
    {
        result = 1;
    }
    else if (arr[machine_choice_int] == PAPER && input_user == ROCK)
    {
        result = 0;
    }
    else if (input_user == SCISSORS && arr[machine_choice_int] == PAPER)
    {
        result =  1;
    }
    else if (arr[machine_choice_int] == SCISSORS && input_user == PAPER)
    {
        result =  0;
    }
    else
    {
        result = -2;
    }

    return result;
}


int main () {
    int result;
    char input_from_user, machine_chose;
    printf("Please choose: rock (r) - paper (p) - scissors (s)\n");
    scanf("%c", &input_from_user);
    result = start_game(input_from_user, &machine_chose);
    if(result == -1)
    {
         printf("\nYou chose %c and I choose %c, The Friendship wins ! \n\n", input_from_user, machine_chose);
    }
    else if (result == 0)
    {
        printf("\nYou chose %c and I choose %c, HAHAHAH Machine wins ! \n\n", input_from_user, machine_chose);
    }
    else if(result == 1)
    {
        printf("\nYou chose %c and I choose %c, You win !\n\n", input_from_user, machine_chose);
    }
    else
    {
        printf("\nYour chose :%c: is incorrect\n\n", input_from_user);
    }
}
