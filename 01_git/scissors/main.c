#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <openssl/md5.h>

#define LOSE 0
#define WIN  1
#define DRAW 2

#define RIGHT_CHAR(x) (my_choice == rps_state[(x)].act_ch)

typedef enum{
    ROCK = 0,
    PAPER = 1,
    SCISSORS = 2,
    ALL_STATES = 3
}activity_t;

typedef struct{
    int value;
    char act_ch;
    char *act_str;
    unsigned char results[ALL_STATES];
}__attribute__((packed)) rps_state_t;

static void machine_md5_answ(int *answ);

int main(){
    int i;
    char my_choice;
    int my_choice_val;
    int machine_choice;
    
    rps_state_t rps_state[ALL_STATES] = {{ROCK, 'r', "rock", {DRAW, LOSE, WIN}}, \
                                        {PAPER, 'p', "paper", {WIN, DRAW, LOSE}},\
                                        {SCISSORS, 's', "scissors", {LOSE, WIN, DRAW}}};

    printf("Please choose: rock (r) - paper (p) - scissors (s)\n");
    my_choice = getchar();
    
    if(!(RIGHT_CHAR(ROCK) || RIGHT_CHAR(PAPER) || RIGHT_CHAR(SCISSORS)))
        return -1;
    
    for(i = 0; i < ALL_STATES; i++){
        if(my_choice == rps_state[i].act_ch){
            my_choice_val = rps_state[i].value;
        }
    }
    
    srand(time(NULL));
    machine_choice = rps_state[rand() % 3].value;
    
    printf("You choose %s, I choose %s\n", rps_state[my_choice_val].act_str, rps_state[machine_choice].act_str);    
    
    if(rps_state[my_choice_val].results[machine_choice] == WIN)
        printf("I lose, %s beats %s :(\n", rps_state[my_choice_val].act_str, rps_state[machine_choice].act_str);
    else if(rps_state[my_choice_val].results[machine_choice] == LOSE)
        printf("I win, %s beats %s :)\n", rps_state[machine_choice].act_str, rps_state[my_choice_val].act_str);
    else
        printf("Draw, both %s\n", rps_state[my_choice_val].act_str);
    
    machine_md5_answ(&machine_choice);
    
    return 0;
}

static void machine_md5_answ(int *answ){
    int i;
    MD5_CTX md5_ctx;
    unsigned char md5_key[MD5_DIGEST_LENGTH];

    MD5_Init(&md5_ctx);
    MD5_Update(&md5_ctx, (const void *)answ, sizeof(*answ));
    MD5_Final(md5_key, &md5_ctx);

    printf("MD5 : ");
    for(i = 0; i < MD5_DIGEST_LENGTH; i++){
        printf("%x", md5_key[i]);
    }
    printf("\n");
}

