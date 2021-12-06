#include <stdio.h>
#include <stdlib.h>

char A = 'r';
char B = 'p';
char C = 's';
char D = 'q';

//Seting game "Scissors"
//first Player is real man and second player is PC;
//if PC select (0) is a rock; if select (1) is a paper; if select (2) is a scissors
//if PlayerA select (r) is a rock; if select (p) is a paper; if select (s) is a scissors
 
int main () 
{
    int i;
    static unsigned int z;

    printf("Hello I want offer to play game SCISOORS \n");
    printf("for choice: rock is a press(r), paper is a press(p), scissors is a press (s) \n \n");
    printf ("Please PlayerA doing your choise and press Enter\n");
    scanf ("%s,\n", &A);

    for (i=1; i<=10; i++)
    {
        z = rand() %4-1;                
        //Block1       
        if (A == 'r' && z == 0)
        {
            printf ("PC also choise 'rock' \n\n");
            printf ("No Winners try again \n\n");
            printf ("If you want quit from this game please press 'q' and press Enter \n");
            z %4-1;
            printf ("If you want continue please doing your choise and press Enter\n\n");
            scanf ("%s,\n", &A);
        }        
        else if(A == 's' && z == 0)
        {
            printf ("PC choise 'rock' \n\n");
            printf ("Win PC: rock break scissors \n\n");
            printf ("If you want quit from this game please press 'q' and press Enter \n");
            z %4-1;
            printf ("If you want continue please doing your choise and press Enter\n\n");
            scanf ("%s,\n", &A);
        }
        else if(A == 'p' && z == 0)
        {
            printf ("PC choise 'rock' \n\n");
            printf ("Win PlayerA: paper covers rock \n\n");
            printf ("If you want quit from this game please press 'q' and press Enter \n");
            z %4-1; 
            printf ("If you want continue please doing your choise and press Enter\n\n");
            scanf ("%s,\n", &A);
        }
        //Block2
        else if (A == 'r' && z == 1)
        {
            printf ("PC choise 'paper' \n\n");
            printf ("Win PC: paper covers rock \n\n");
            printf ("If you want quit from this game please press 'q' and press Enter \n");
            z %4-1;
            printf ("If you want continue please doing your choise and press Enter\n\n");
            scanf ("%s,\n", &A);
        }        
        else if(A == 's' && z == 1)
        {
            printf ("PC choise 'paper' \n\n");
            printf ("Win PlayerA: scissors cuter paper \n\n");
            printf ("If you want quit from this game please press 'q' and press Enter \n");
            z %4-1;
            printf ("If you want continue please doing your choise and press Enter\n\n");
            scanf ("%s,\n", &A);
        }
        else if(A == 'p' && z == 1)
        {
            printf ("PC also choise 'paper' \n\n");
            printf ("No Winners try again \n\n");
            printf ("If you want quit from this game please press 'q' and press Enter \n");
            z %4-1;
            printf ("If you want continue please doing your choise and press Enter\n\n");
            scanf ("%s,\n", &A);
        }
        //Block3
        else if (A == 'r' && z == 2)
        {
            printf ("PC choise 'scissors' \n\n");
            printf ("Win PlayerA: rock break scissors \n\n");
            printf ("If you want quit from this game please press 'q' and press Enter \n");
            z %4-1;
            printf ("If you want continue please doing your choise and press Enter\n\n");
            scanf ("%s,\n", &A);
        }        
        else if(A == 's' && z == 2)
        {
            printf ("PC also choise 'scissors' \n\n");
            printf ("No Winners try again \n\n");
            printf ("If you want quit from this game please press 'q' and press Enter \n");
            z %4-1;
            printf ("If you want continue please doing your choise and press Enter\n\n");
            scanf ("%s,\n", &A);
        }
        else if(A == 'p' && z == 2)
        {
            printf ("PC choise 'scissors' \n\n");
            printf ("Win PC: scissors cuter paper \n\n");
            printf ("If you want quit from this game please press 'q' and press Enter \n");
            z %4-1;
            printf ("If you want continue please doing your choise and press Enter\n\n");
            scanf ("%s,\n", &A);
        }
        else if(A == 'q')
        {
            printf ("Game Over, Goodbye \n");
            i = 10;
        }

        else
        {
            printf("your choose is loose, try again\n \n \n");
            printf ("If you want quit from this game please press 'q' and press Enter \n");
            z = 0;
            printf ("If you want continue please doing your choise and press Enter\n\n");
            scanf ("%s,\n", &A);
        }                   
    }    
}
