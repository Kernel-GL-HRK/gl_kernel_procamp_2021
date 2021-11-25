#include <stdio.h>

 char A = 'r';
 char B = 'p';
 char C = 's';

int main()
{
    
    printf("Hello I want offer to play game SCISOORS \n");
    printf("for choice: rock- press(r), paper- press(p), scissors- press (s) \n");
    printf ("Please PlayerA doing your choise and press Enter\n");
    scanf ("%s,\n" ,&A);
    
    printf ("Please PlayerB doing your choise and press Enter\n");
    scanf ("%s,\n" ,&B);
    
   
   if (A == 'r' && B == 'r')
    {
        printf ("No Winners try again");
    }
    
    else if(A == 'r' && B == 'p')
    {
        printf ("Win PlayerB: paper covers rock");
    }
    
    else if(A == 'r' && B == 's')
    {
        printf ("Win PlayerA: rock break scissors");
    }
    
    
    
    else if (A == 'p' && B == 'p')
    {
        printf ("No Winners try again");
    }
    
    else if(A == 'p' && B == 'r')
    {
        printf ("Win PlayerA: paper covers rock");
    }
    
    else if(A == 'p' && B == 's')
    {
        printf ("Win PlayerB: scissors cutting paper");
    }
    
    
    
    else if (A == 's' && B == 's')
    {
        printf ("No Winners try again");
    }
    
    else if(A == 's' && B == 'r')
    {
        printf ("Win PlayerA: rock break  scissors");
    }
    
    else if(A == 's' && B == 'p')
    {
        printf ("Win PlayerB: scissors cutting paper");
    }
    
    
    
    else
    {
    printf("your choose is loose, try again");
    }
    
    return 0;
}

