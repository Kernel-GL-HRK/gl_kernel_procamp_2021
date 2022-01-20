#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

uint32_t randomize(uint32_t range_min, uint32_t range_max)
{
	uint32_t* ptr1 = (uint32_t*)malloc(4);
	uint32_t* ptr2 = (uint32_t*)malloc(4);
	uint32_t pos_seed = (uint32_t) ptr1;
	uint32_t src_seed = (uint32_t) ptr2;
	free(ptr1);
	free(ptr2);
	uint8_t rshifts_num = pos_seed % (sizeof(src_seed)*8);
	uint32_t random_num = (src_seed >> rshifts_num) % (range_max+1 - range_min) + range_min;
	return random_num;
}

uint8_t get_rps_id(char rps_symbol)
{
	switch(rps_symbol)
	{
		case 'r':
			return 0;	// rock
		case 'p':
			return 1;	// paper
		case 's':
			return 2;	// scissors
		default:
			return -1;	// error-code
	}
}

char get_rps_symbol(uint8_t rps_id)
{
	switch(rps_id)
	{
		case 0:
			return 'r';	// rock
		case 1:
			return 'p';	// paper
		case 2:
			return 's';	// scissors
		default:
			return 'E';	// error-code
	}
}

char* get_rps_string(uint8_t rps_id)
{
	switch(rps_id)
	{
		case 0:
			return "rock";	// rock
		case 1:
			return "paper";	// paper
		case 2:
			return "scissors";	// scissors
		default:
			return "ERROR";	// error-code
	}
}

bool rps_does_first_win(uint8_t first, uint8_t second)
{
	if (first == 0 && second == 2)
	{
		return true;
	}
	else if (first == 2 && second == 0)
	{
		return false;
	}
	else
	{
		if (first >= second)
		{
			return true;
		}
		else if ((first == 0) && (second == 2))	// if (first = 'r' && second == 's')
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

int main()
{
	uint8_t random_rps = (uint8_t) randomize(0, 2);
	
	printf("Welcome to the Rock-Paper-Scissors game.\n");
	printf("Choose:\n r - rock,\n p - paper,\n s - scissors.\n");
	printf("Your option: ");
	char symbol = 0;
	scanf("%c", &symbol);
	uint8_t user_rps = get_rps_id(symbol);
	
	bool does_user_win = rps_does_first_win(user_rps, random_rps);
	printf("My option was: %c.\n", get_rps_symbol(random_rps));
	if (does_user_win)
	{
		printf("You win: %s beats %s\n", get_rps_string(user_rps), get_rps_string(random_rps));
	}
	else
	{
		printf("I win: %s beats %s\n", get_rps_string(random_rps), get_rps_string(user_rps));
	}

	printf("Try again!");
	
	printf("\n\n");
	return 0;	
}

