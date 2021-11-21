#ifndef __ROCK_PAPER_SCISSORS_H_
#define __ROCK_PAPER_SCISSORS_H_

#define RPS_N_VALUABLE_CHOICES 3

typedef enum RPS_CHOICES {
    RPS_CHOICE_ROCK,
    RPS_CHOICE_PAPER,
    RPS_CHOICE_SCISSORS,
    RPS_CHOICE_UNKNOWN
} RPS_CHOICES_E;


typedef struct RPS_CHOICE_ENTRY {
    RPS_CHOICES_E id;
    char long_name[16];
    char short_name;
} RPS_CHOICE_ENTRY_S;


void RPS_run_game(void);

#endif // __ROCK_PAPER_SCISSORS_H_
