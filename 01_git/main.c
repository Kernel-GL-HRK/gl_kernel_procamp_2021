/**
 * @file main.c
 * @brief rock-paper-scissors game implementation in C programming language
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum eSigns
{
  SIGN_ROCK     = 0,
  SIGN_SCISSORS = 1,
  SIGN_PAPER    = 2,
  SIGN_INVALID  = 3,
};

enum eGameResults
{
  WINNER_COMPUTER = 0,
  WINNER_HUMAN    = 1,
  DRAW            = 2,
};

const char *signs_names[] = { "\"Rock\"", "\"Scissors\"", "\"Paper\"" };
const int  game_result[]  = { SIGN_PAPER, SIGN_ROCK, SIGN_SCISSORS };

int sign_to_num(char sign);
int get_game_result(int sign_human, int sign_computer);

int main(void)
{
  char input    = '\0';
  int  computer = 0;
  int  human    = 0;
  int  result   = 0;
  int  cnt_human_win    = 0;
  int  cnt_computer_win = 0;
  int  cnt_draw         = 0;
 
  srand(time(0));

  puts("*** Rock paper scissors game ***\n");
  puts("To play game enter your sign: r - for \"Rock\", p - for \"Paper\", s - for \"Scissors\"");
  puts("Press Ctrl+D or Ctrl+Z to exit the game\n");

  while ((input = getchar()) != EOF)
  {
    /* Skip Enter button processing */
    if (input == '\n')
    {
      continue;
    }

    if ((human = sign_to_num(input)) == SIGN_INVALID)
    {
      puts("Enter your sign: r - for \"Rock\", p - for \"Paper\", s - for \"Scissors\"");
      continue;
    }

    printf("Your sign is %s\n", signs_names[human]);

    computer = rand() % 3;
    printf("PC sign is %s\n", signs_names[computer]);

    result = get_game_result(human, computer);

    if (result == DRAW)
    {
      ++cnt_draw;
      puts("Draw\n");
    }
    else if (result == WINNER_HUMAN)
    {
      ++cnt_human_win;
      puts("You win!\n");
    }
    else
    {
      ++cnt_computer_win;
      puts("Computer win!\n");
    }
  }
  
  puts("The game is finished!");
  printf("Results: You win %d times, computer win %d times, draw %d times\n", 
        cnt_human_win, 
        cnt_computer_win, 
        cnt_draw);

  return 0;
}

int sign_to_num(char sign)
{
  int retval = 0;

  switch (sign)
  {
  case 'r':
    retval = SIGN_ROCK;
    break;

  case 's':
    retval = SIGN_SCISSORS;
    break;

  case 'p':
    retval = SIGN_PAPER;
    break;

  default:
    retval = SIGN_INVALID;
    break;
  }

  return retval;
}

int get_game_result(int sign_human, int sign_computer)
{
  int retval = 0;

  if (sign_human == sign_computer)
  {
    retval = DRAW;
  }
  else if (game_result[sign_human] == sign_computer)
  {
    retval = WINNER_COMPUTER;
  }
  else
  {
    retval = WINNER_HUMAN;
  }
  
  return retval;
}
