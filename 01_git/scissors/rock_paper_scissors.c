#include <stdlib.h>
#include <stdio.h>
#include <time.h>

char *to_word(char c)
{
   switch (c)
   {
   case 'r':
      return "rock";
   case 'p':
      return "paper";
   case 's':
      return "scissors";
   }

   return "";
}

char get_random()
{

   srand(time(NULL));

   int r = rand() % 3;

   char computer;
   switch (r)
   {
   case 0:
      computer = 'r';
      break;
   case 1:
      computer = 'p';
      break;
   case 2:
      computer = 's';
      break;
   }

   return computer;
}

int who_win(char user, char computer)
{
   if (user == 's' && computer == 'p')
      return 1;
   if (user == 'p' && computer == 's')
      return -1;
   if (user == 's' && computer == 'r')
      return -1;
   if (user == 'r' && computer == 's')
      return 1;
   if (user == 'p' && computer == 'r')
      return 1;
   if (user == 'r' && computer == 'p')
      return -1;

   return 0;
}

int main()
{
   char user;

   while (user != 'r' && user != 'p' && user != 's')
   {
      printf("Please choose: rock (r) - paper (p) - scissors (s)\n");
      user = getchar();
   }

   char computer = get_random();
   printf("%c\n", computer);
   printf("You choose %s, I choose %s\n", to_word(user), to_word(computer));

   int result = who_win(user, computer);

   switch (result)
   {
   case 0:
      printf("Game Draw!\n");
      break;
   case 1:
      printf("You win: %s beats %s\n", to_word(user), to_word(computer));
      break;
   case -1:
      printf("I win: %s beats %s\n", to_word(computer), to_word(user));
      break;
   }

   return 0;
}
