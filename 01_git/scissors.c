#include <stdio.h>
#include <stdlib.h>
#include<time.h>

const char arr[3] = {'r', 'p', 's'};

int machine_rand(void) {
    srand(time(0));
    int random = rand() % 3;
    return random;
}

int start_game(char input_user, char *machine_choice_c){
    int machine_choice_int  = machine_rand();
    *machine_choice_c = arr[machine_choice_int];
    int result;
    if(input_user == arr[machine_choice_int])
    {
        result = -1;
    }
    else if (input_user == 'r' && arr[machine_choice_int] == 's')
    {
        result = 1;
    }
    else if (arr[machine_choice_int] == 'r' && input_user == 's')
    {
        result = 0;
    }
    else if (input_user == 'p' && arr[machine_choice_int] == 'r')
    {
        result = 1;
    }
    else if (arr[machine_choice_int] == 'p' && input_user == 'r')
    {
        result = 0;
    }
    else if (input_user == 's' && arr[machine_choice_int] == 'p')
    {
        result =  1;
    }
    else if (arr[machine_choice_int] == 's' && input_user == 'p')
    {
        result =  0;
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
    else
    {
        printf("\nYou chose %c and I choose %c, You win !\n\n", input_from_user, machine_chose);
    }
}