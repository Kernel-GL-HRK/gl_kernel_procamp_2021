#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
  char com;
  int r;
  
  srand(time(NULL));
  
  printf("Please choose: rock (r) - paper (p) - scissors (s)");
  com = getchar();
  r = rand()%3;
  
  if (com == 'r') printf("You choose rock, I choose " );
  if (com == 'p') printf("You choose paper, I choose " );
  if (com == 's') printf("You choose scissors, I choose ");
  
  if (r  == 0) printf("rock.\n" );
  if (r  == 1) printf("paper.\n" );
  if (r  == 2) printf("scissors.\n");
  
  if (com == 'r' &&  r  == 0) printf ("Draw game.\n" );
  if (com == 'p' &&  r  == 1) printf ("Draw game.\n" );
  if (com == 's' &&  r  == 2) printf ("Draw game.\n" );
  
  if (com == 'r' &&  r  == 1) printf ("I win: paper beats rock.\n" );
  if (com == 'p' &&  r  == 2) printf ("I win: scissors beats paper.\n" );
  if (com == 's' &&  r  == 0) printf ("I win: rock beats scissors.\n" );
  
  if (com == 'r' &&  r  == 2) printf ("You win: rock beats scissors.\n" );
  if (com == 'p' &&  r  == 0) printf ("You win: paper beats rock.\n" );
  if (com == 's' &&  r  == 1) printf ("You win: scissors beats paper.\n" );
  
  getchar();
  return 0;
}
