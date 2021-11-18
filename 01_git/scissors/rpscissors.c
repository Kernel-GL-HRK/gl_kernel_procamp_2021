// SPDX-License-Identifier: 0BSD
/**
 * Description: Console Game Rock-Paper-Scissors
 * Author:      Nazarii Kurylko
 * Created:     19.11.2021
 **/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <openssl/md5.h>

/* enumerations */
enum WINNER { PC_WINNER = -1, NO_WINNER, USER_WINNER };
enum STATUS { ERROR = -1, SUCCESS };

/*function declaration*/
static char read_user_choice(void);
static int chek_choice(char choice);
static const char *get_full_choice(char choice);
static int compare_results(char pc_choice, char user_choice);
static void print_results(char pc_choice, char user_choice, int result);
static int calc_md5(const void *in_data, unsigned long in_len,
		    unsigned char *out_data, unsigned long p_out_len);
static void print_md5_of_choise(char choice);

/*main algorithm*/
int main(void)
{
	char user_choice = 'u'; /*unknown*/
	char pc_choice = 'u'; /*unknown*/
	const char choice_range[] = { 'r', 'p',
				      's' }; /*rock, papaer, scissors*/
	enum WINNER result = NO_WINNER;

	/*initialise rand*/
	srand(time(0));

	/*repeat until winner found*/
	while (result == NO_WINNER) {
		printf("> Please choose: rock (r) - paper (p) - scissors (s)\n> ");

		/*pc choice*/
		pc_choice = choice_range[rand() % (sizeof(choice_range) /
						   sizeof(choice_range[0]))];

		/*md5 checksum for pc choise*/
		print_md5_of_choise(pc_choice);

		user_choice = read_user_choice();

		/*verify if user choice is in range*/
		if (chek_choice(user_choice) == ERROR)
			continue;

		/*compare results*/
		result = compare_results(pc_choice, user_choice);

		/*print result*/
		print_results(pc_choice, user_choice, result);
	}
	return 0;
}

/*read user choices*/
static char read_user_choice(void)
{
	/*read user choice*/
	char line[256];

	if ((fgets(line, sizeof(line), stdin) == NULL) || (line[1] != '\n'))
		return 'u'; /*unknown - choice is unclear*/

	return line[0]; /*return first character*/
}

/*comare the pc and user choices*/
static enum WINNER compare_results(char pc_choice, char user_choice)
{
	if (pc_choice == user_choice)
		return NO_WINNER; /*nobody wins*/
	else if ((pc_choice == 'r' && user_choice == 's') ||
		 (pc_choice == 'p' && user_choice == 'r') ||
		 (pc_choice == 's' && user_choice == 'p'))
		return PC_WINNER; /*pc win*/
	else
		return USER_WINNER; /*ps lose*/
}

/*convet short to full description of user choice*/
static const char *get_full_choice(char choice)
{
	if ('r' == choice)
		return "rock";
	else if ('p' == choice)
		return "paper";
	else if ('s' == choice)
		return "scissors";
	else
		return "unknown choice";
}

/*verify if choice is in range*/
static enum STATUS chek_choice(char choice)
{
	if (choice != 'r' && choice != 'p' && choice != 's') {
		//choice is not in range
		return ERROR;
	}
	return SUCCESS;
}

/*print results to comad line*/
static void print_results(char pc_choice, char user_choice, int result)
{
	printf("> You choose %s, I choose %s\n", get_full_choice(user_choice),
	       get_full_choice(pc_choice));

	if (result == USER_WINNER) {
		printf("> I lose: %s beats %s\n", get_full_choice(user_choice),
		       get_full_choice(pc_choice));
	} else if (result == PC_WINNER) {
		printf("> I win: %s beats %s\n", get_full_choice(pc_choice),
		       get_full_choice(user_choice));
	}
}

/*calculates md5 checksum return 0-SUCCESS or -1-ERROR */
static int calc_md5(const void *in_data, unsigned long in_len,
		    unsigned char *out_data, unsigned long p_out_len)
{
	MD5_CTX mdContext;

	/*chekc if argumens on and in range*/
	if (!in_data || !out_data || p_out_len < MD5_DIGEST_LENGTH)
		return ERROR;

	MD5_Init(&mdContext);
	MD5_Update(&mdContext, in_data, in_len);
	MD5_Final(out_data, &mdContext);

	return SUCCESS;
}

/*print md5 checksum of choise*/
static void print_md5_of_choise(char choice)
{
	unsigned char md5_checksum[MD5_DIGEST_LENGTH];
	int i;

	if (calc_md5(get_full_choice(choice), strlen(get_full_choice(choice)),
		     md5_checksum, sizeof(md5_checksum)) == SUCCESS) {
		printf("pc choise checksum is: ");
		for (i = 0; i < MD5_DIGEST_LENGTH; i++)
			printf("%02x", (unsigned int)md5_checksum[i]);

		printf("\n> ");
	}
}
