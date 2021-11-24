/*
 * Main file of scissors-paper project
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scissors.h"
#include <time.h>

//
// Return name of material chose by gamer
//
char* getmaterial(char gamersymb)
{
  switch(gamersymb)
  {
    case SYMB_ROCK:
      return STR_ROCK;;
    case SYMB_PAPER:
      return STR_PAPER;
    case SYMB_SCISSORS:
      return  STR_SCISSORS;
  }
}

//
// Calculate who is winner of game
//
int getwinner(char gamer1, char gamer2)
{
  int ret = WINNER_NO;

  if(gamer1==gamer2)  return WINNER_NO;
  switch(gamer1)
  {
  case SYMB_PAPER:
    if(SYMB_ROCK==gamer2)  ret = WINNER_1;
    else                  ret = WINNER_2;
    break;
  case SYMB_ROCK:
    if(SYMB_PAPER==gamer2)  ret = WINNER_2;
    else                   ret = WINNER_1;
    break;
  case SYMB_SCISSORS:
    if(SYMB_ROCK==gamer2)  ret = WINNER_2;
    else                  ret = WINNER_1;
    break;
  default:
    ret = WINNER_ERR;
    break;
  }
  return ret;
}

int main()
{
  char sgamer[256],gamer,computer,winner;

  gamer = computer = winner = 0;
  srand(time(NULL));  //Initialisation of random function

  for(;;){
    // Get user choise
    for(;;){
      printf("\n\tPlease, choose rock (%c) - paper(%c) - scissors(%c) exit - (%c)\n\t",\
      SYMB_ROCK,SYMB_PAPER,SYMB_SCISSORS,SYMB_EXIT);

      if(NULL==fgets((char*)sgamer,sizeof(sgamer),stdin)){
        printf("\n\tInput error");
        return 1;
      }
      if(2<strlen((char*)sgamer)){
        printf("\n\t too much symbols (%li) %s",strlen((char*)sgamer)-1,sgamer);
        continue;
      }
      gamer = sgamer[0];
      if(SYMB_EXIT==gamer)  return 0;
      if((SYMB_PAPER==gamer)||(SYMB_ROCK==gamer)||(SYMB_SCISSORS==gamer)) break;
      printf("\n\tWrong choise  - (%c)\n\t",gamer);
    }

    //Get computer choise
    computer = rand()%3;
    switch(computer){
      default:
      case 0:
        computer = SYMB_ROCK;
        break;
      case 1:
        computer = SYMB_PAPER;
        break;
      case 2:
        computer = SYMB_SCISSORS;
        break;
    }

    // Get winner of game
    winner = getwinner(gamer,computer);
    if(WINNER_ERR==winner){
      printf("\n\tgetwinner error\n\t");
      return 1;
    }
    if(WINNER_NO==winner){
      printf("\n\tNo sides!\n\t");
    }
    else{
      if(WINNER_1==winner){
        printf("\n\tYou won: %s beats %s",getmaterial(gamer),getmaterial(computer));
      }
      else{
        printf("\n\tI won: %s beats %s",getmaterial(computer),getmaterial(gamer));
      }
    }
  }
  return 0;
}
