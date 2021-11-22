#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>  

#define UNDEFINED 0
#define ERROR -1

#define ROCK 'r'
#define PAPER 'p'
#define SCISSORS 's'

bool is_command_valid(char command) {
    return command == ROCK || command == PAPER || command == SCISSORS;
}

char get_computer_command() {
    int command = rand() % 3;
    switch (command) {
        case 0: 
            return ROCK;
        case 1:
            return PAPER;
        case 2:
            return SCISSORS;
        default:
            return UNDEFINED;
    }
}

char define_winner(char computer_command, char user_command) {
    if (!is_command_valid(computer_command) || !is_command_valid(user_command)) {
        return ERROR;
    }

    if (computer_command == user_command) {
        return UNDEFINED;    
    } else if (computer_command == ROCK && user_command == SCISSORS) {
        return computer_command;
    } else if (computer_command == PAPER && user_command == ROCK) {
        return computer_command;
    } else if (computer_command == SCISSORS && user_command == PAPER) {
        return computer_command;
    } else {
        return user_command;
    }
}

char* get_command_name(char command) {
    if (command == ROCK) {
        return "rock";
    } else if (command == PAPER) {
        return "paper";
    } else if (command == SCISSORS) {
        return "scissors";
    } else {
        return "";
    }
}

int main() {
    printf("Please choose: rock (r) - paper (p) - scissors (s)\n");
    char computer_command = get_computer_command();
    char user_command = getchar();

    char* computer_command_name = get_command_name(computer_command);
    char* user_command_name = get_command_name(user_command);

    printf("You choose %s, I choose %s\n", user_command_name, computer_command_name);
 
    char winner_command = define_winner(computer_command, user_command);

    if (winner_command == computer_command) {
        printf("I win: %s beats %s\n", computer_command_name, user_command_name);
    } else if (winner_command == user_command) {
        printf("You win: %s beats %s\n", user_command_name, computer_command_name);
    } else {
        printf("Draw\n");
    }

    return 0;
}