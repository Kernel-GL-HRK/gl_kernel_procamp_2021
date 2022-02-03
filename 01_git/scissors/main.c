#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <openssl/md5.h>

void md5_checksum(char**, int);

int main() {
	srand(time(NULL));
	char userChoice;
    char* choicesArr[3] = {"rock", "paper", "scissors"};
    int compNum, userNum;

    printf("Please choose: rock(r) - paper(p) - scissors(s)\n");
    switch(userChoice = getchar())
    {
        case 'r':
            userNum = 0;
            break;
        case 'p':
            userNum = 1;
            break;
        case 's':
            userNum = 2;
            break;
        default:
            printf("Uncompatible character, try again\n");
            return 1;
    }

    compNum = (rand()%3);

    md5_checksum(choicesArr, compNum);

    printf("You choose %s, I choose %s\n", choicesArr[userNum], choicesArr[compNum]);
    
    if (compNum == userNum)
        printf("Tie!\n");
    else if ((compNum - userNum) == 1 || (compNum - userNum) == -2)
        printf("I won: %s beats %s\n", choicesArr[compNum], choicesArr[userNum]);
    else
        printf("You won: %s beats %s\n", choicesArr[userNum], choicesArr[compNum]);

    return 0;
}

void md5_checksum(char** str_arr, int index)
{
    unsigned char md5digest[MD5_DIGEST_LENGTH];
    MD5(str_arr[index], strlen(str_arr[index]), md5digest);
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++)
        printf("%x", md5digest[i]);
    putchar('\n');
}