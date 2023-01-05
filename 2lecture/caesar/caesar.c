/*
    Caesar ecuation = ci = (pi + K) % 26.

*/

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX_LIMIT 100

void caesar(char *t, int K);

char text[MAX_LIMIT];

int main(int argc, string argv[])
{
    char *p;

    // Check if the user has introduced an arg.
    if (argc <= 1 || argc > 2 || isdigit(*argv[1]) == 0)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    int K = (int)strtol(argv[1], &p, 10);
    if (*p != '\0')
    {
        return 1;
    }
    // Check a correct scanf.
    do
    {
        printf("plaintext: ");
        fgets(text, MAX_LIMIT, stdin);
    }
    while (strlen(text) < 1);
    caesar(text, K);
    return 0;
}

// Caesar function, identify the type con character and do the encryptation.
void caesar(char *t, int K)
{
    char c[MAX_LIMIT];
    int position;

    for (int i = 0; i < strlen(t) - 1; i ++)
    {
        // Uppercase differentiator.
        if ((int)t[i] >= 65 && (int)t[i] <= 90)
        {
            position = 25 - (90 - (int)t[i]);
            c[i] = ((position + K) % 26) + 65;
        }
        // Lowercase differentiator.
        else if ((int)t[i] >= 97 && (int)t[i] <= 122)
        {
            position = 25 - (122 - (int)t[i]);
            c[i] = ((position + K) % 26) + 97;
        }
        // Space differentiator.
        if ((int)t[i] < 65 || (int)t[i] > 122)
        {
            c[i] = t[i];
        }
        else if ((int)t[i] > 90 && (int)t[i] < 97)
        {
            c[i] = t[i];
        }
    }
    printf("ciphertext: %s\n", c);
}

