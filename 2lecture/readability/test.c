#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int sentencecounter = 0;

int main(void)
{
    float fl = 3.6;
    printf("The float of %f is : %d\n", fl, (int)round(fl));
}

// for (int n = 0; UPPERLETTERS[n] != '\0'; n++)
//         {
//             if (ch[i] != UPPERLETTERS[n])
//             {
//                 printf("Character: %c\n", ch[i]);
//             }
//         }