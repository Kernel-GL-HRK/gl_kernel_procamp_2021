#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game_common.h"

static void init_random_engine()
{
    srand( time( NULL ) );
}

static command_type_t get_ai_command()
{
    int choice = rand() % 3;
    switch ( choice )
    {
        case 0:
            return ROCK_SYMBOL;
            break;
        case 1:
            return PAPER_SYMBOL;
            break;
        case 2:
            return SCISSORS_SYMBOL;
            break;
        default:
            assert( false );
            return ERROR_RES;
    }
}

static bool is_command_valid( command_type_t command_to_check )
{
    if ( command_to_check == ERROR_RES )
        return false;
    const bool is_valid_range = ( command_to_check == ROCK_SYMBOL ) ||
                                ( command_to_check == PAPER_SYMBOL ) ||
                                ( command_to_check == SCISSORS_SYMBOL );

    return is_valid_range;
}

static command_type_t get_user_command()
{
    do
    {
        char user_input = getchar();
        if ( user_input == EOF )
            EXIT_SUCCESS;
        if (is_command_valid(user_input))
            return user_input;
        else if (user_input == '\n')
            continue;
        else
        {
            printf(
                "Entered value is wrong. Allowed values: rock (r) - paper (p) "
                "- scissors (s)\n" );
        }
    } while ( true );

    return ERROR_RES;
}

static winner_enum_t
get_winner( command_type_t ai_command, command_type_t user_command )
{
    if ( !is_command_valid( ai_command ) || !is_command_valid( user_command ) )
    {
        assert( false );
        return k_undefined;
    }
    const bool is_draw = ai_command == user_command;
    const bool is_ai_winner =
        ( ai_command == ROCK_SYMBOL && user_command == SCISSORS_SYMBOL ) ||
        ( ai_command == PAPER_SYMBOL && user_command == ROCK_SYMBOL ) ||
        ( ai_command == SCISSORS_SYMBOL && user_command == PAPER_SYMBOL );

    if ( is_draw )
        return k_draw;

    return is_ai_winner ? k_ai_winner : k_user_winner;
}

static show_final_winner_message(
    winner_enum_t winner,
    const char* ai_command_name,
    const char* user_command_name )
{
    switch ( winner )
    {
        case k_ai_winner:
            printf(
                "I win: %s, beats %s\n", ai_command_name, user_command_name );
            break;

        case k_user_winner:
            printf(
                "You win: %s beats %s \n", user_command_name, ai_command_name );
            break;
        case k_draw:
            printf( "Draw!\n" );
            break;

        default:
            assert( false );
            printf( "Magic cosmic rays are flipping bits\n" );
            break;
    }
}

const char* get_command_name( command_type_t command_choice )
{
    switch ( command_choice )
    {
        case ROCK_SYMBOL:
            return "rock";
            break;
        case PAPER_SYMBOL:
            return "paper";
            break;
        case SCISSORS_SYMBOL:
            return "scissors";
            break;
        default:
            assert( false );
            return "";
            break;
    }
}
int main()
{
    while ( true )
    {
        printf( "Please choose: rock (r) - paper (p) - scissors (s)\n" );

        command_type_t ai_command = get_ai_command();
        command_type_t user_command = get_user_command();

        const char* ai_command_name = get_command_name( ai_command );
        const char* user_command_name = get_command_name( user_command );

        printf(
            "My choice is %s, your choice is %s\n",
            ai_command_name,
            user_command_name );
        winner_enum_t current_winner = get_winner( ai_command, user_command );

        show_final_winner_message(
            current_winner, ai_command_name, user_command_name );
    }
    return 0;
}