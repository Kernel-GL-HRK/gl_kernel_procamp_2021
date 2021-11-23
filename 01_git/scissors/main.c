#include <stdlib.h>
#include <stdio.h>

enum Type {
	Rock=1,
	Paper,
	Scissors
};

// 0 - winner is first elt second
char get_winner(enum Type first, enum Type second) {
	if (first == Rock && second == Paper) return 0;
	if (first == Paper && second == Rock) return 1;
	if (first == Rock && second == Scissors) return 0;
	if (first == Scissors && second == Rock) return 1;
	if (first == Scissors && second == Paper) return 1;
	if (first == Paper && second == Scissors) return 1;
}

char *to_string(enum Type t)
{
   switch (t)
   {
   case Rock: return "rock";
   case Paper: return "paper";
   case Scissors: return "scissors";
   }

   return "";
}

int main(void) {
	printf("Please choose: rock (r) - paper (p) - scissors (s)\r\n");
	
	enum Type	choose_user;
	switch (getchar()) {
		case 'r': choose_user = Rock; break;
		case 'p': choose_user = Paper; break;
		case 's': choose_user = Scissors; break;
		default: {
			printf("Wrong\r\n");
			return -1;
		}
	}
	
	enum Type	choose_comp;
	int r = rand();
	if (r%Scissors == 0) choose_comp = Scissors;
	else if (r%Paper == 0) choose_comp = Paper;
	else choose_comp = Rock;
	
	printf("You choose %s, I choose %s\n", to_string(choose_user), to_string(choose_comp));
	
	if (choose_user == choose_comp) {
		printf("Friending is winned)))\r\n");
	}
	else {
		if (get_winner(choose_user, choose_comp)) 
			printf("You win: %s beats %s\n", to_string(choose_user), to_string(choose_comp));
		else 
			printf("I win: %s beats %s\n", to_string(choose_comp), to_string(choose_user));
	}
	

	return 0;
}
