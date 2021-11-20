#ifndef GAME_COMMON_H
#define GAME_COMMON_H

#define ROCK_SYMBOL 'r'
#define PAPER_SYMBOL 'p'
#define SCISSORS_SYMBOL 's'

typedef char command_type_t;

typedef enum
{
    k_ai_winner,
    k_user_winner,
    k_draw,
    k_undefined
}winner_enum_t;

#define ERROR_RES 0

#endif