#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <openssl/md5.h>

#include "rock_paper_scissors.h"

static RPS_CHOICES_E get_human_choice(void);
static RPS_CHOICES_E get_random_choice(void);
static void RPS_show_game_result(RPS_CHOICES_E comp_choice, RPS_CHOICES_E human_choice);
static void RPS_print_md5(char value);

static RPS_CHOICE_ENTRY_S choices_arr[] = {{RPS_CHOICE_ROCK, "rock", 'r'},
                                           {RPS_CHOICE_PAPER, "paper", 'p'},
                                           {RPS_CHOICE_SCISSORS, "scissors", 's'},
                                           {RPS_CHOICE_UNKNOWN, "unknown", 'u'}};

static RPS_CHOICES_E RPS_get_random_choice(void)
{
    unsigned char buff;
    RPS_CHOICES_E choice = RPS_CHOICE_UNKNOWN;

    int fd = open("/dev/urandom", O_RDONLY);

    if (!fd)
    {
        printf("Error opening /dev/urandom\n");
        exit(1);
    }

    read(fd, &buff, sizeof buff);
    close(fd);
    choice = buff % RPS_N_VALUABLE_CHOICES;

    return choice;
}


static RPS_CHOICES_E RPS_get_human_choice(void)
{
    RPS_CHOICES_E choice = RPS_CHOICE_UNKNOWN;
    char read_value = '?';

    printf("Please choose: rock (r) - paper (p) - scissors (s)\n");
    scanf(" %c", &read_value);

    switch (read_value)
    {
        case 'r':
            {
                choice = RPS_CHOICE_ROCK;
                break;
            }

        case 'p':
            {
                choice = RPS_CHOICE_PAPER;
                break;
            }
        case 's':
            {
                choice = RPS_CHOICE_SCISSORS;
                break;
            }
        default:
            {
                choice = RPS_CHOICE_UNKNOWN;
                break;
            }
    }

    return choice;
}


static void RPS_print_md5(char value)
{
    MD5_CTX ctx;
    unsigned char md5digest[MD5_DIGEST_LENGTH];

    MD5_Init(&ctx);
    MD5_Update(&ctx, (const void *)&value, sizeof value);
    MD5_Final(md5digest, &ctx);

    printf("Random choice: %c md5: ", value);

    for ( int i=0;i < MD5_DIGEST_LENGTH; i++ ) {
        printf("%02x", md5digest[i]);
    };
    printf("\n");
}


static void RPS_show_game_result(RPS_CHOICES_E comp_choice, RPS_CHOICES_E human_choice)
{
    RPS_CHOICE_ENTRY_S *p_human_entry;
    RPS_CHOICE_ENTRY_S *p_comp_entry;
    int game_result;

    p_comp_entry = &choices_arr[comp_choice];
    p_human_entry = &choices_arr[human_choice];

    printf("You choose %s, I choose %s\n", p_human_entry->long_name, p_comp_entry->long_name);

    game_result = comp_choice - human_choice;

    switch (game_result)
    {
        case 0:
            {
                printf("Draw: %s and %s\n", p_human_entry->long_name, p_comp_entry->long_name);
                break;
            }

        case 1:
            {
                printf("I win: %s beats %s\n", p_comp_entry->long_name, p_human_entry->long_name);
                break;
            }
        case 2:
            {
                printf("You win: %s beats %s\n", p_human_entry->long_name, p_comp_entry->long_name);
                break;
            }
        case -1:
            {
                printf("You win: %s beats %s\n", p_human_entry->long_name, p_comp_entry->long_name);
                break;
            }
        case -2:
            {
                printf("I win: %s beats %s\n", p_comp_entry->long_name, p_human_entry->long_name);
                break;
            }
        default:
            {
                printf("Error game result: %i\n", game_result);
                exit(1);
                break;
            }
    }
}


void RPS_run_game(void)
{
    RPS_CHOICES_E human_choice = RPS_CHOICE_UNKNOWN;
    RPS_CHOICES_E comp_choice = RPS_CHOICE_UNKNOWN;

    comp_choice = RPS_get_random_choice();
    // Optional task: add printing md5 checksum of computer's choice before human's move.
    RPS_print_md5(choices_arr[comp_choice].short_name);

    for ( ; human_choice == RPS_CHOICE_UNKNOWN; )
    {
        human_choice = RPS_get_human_choice();
    }

    RPS_show_game_result(comp_choice, human_choice);
}
