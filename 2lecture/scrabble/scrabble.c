/*  Author: Rodrigo Hormazabal
    A simple game of Scrabble for 2 player, the program takes a word and gives points depending the complexity of those words.
*/

#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
char UPPERLETTERS[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

int compute_score(string word);
void make_upper(char *s);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    printf("%d\n", score1);
    printf("%d\n", score2);

    // TODO: Print the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else if (score1 == score2)
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    // TODO: Compute and return score for string
    string originalstring = word;
    int points = 0;

    make_upper(word);

    // Linear search the letter position.
    // This for takes the length of "word".
    for (int i = 0; i < strlen(word); i++)
    {
        // This for search on the array "UPPERLETTERS".
        for (int n = 0; n < strlen(UPPERLETTERS); n++)
        {
            if (word[i] == UPPERLETTERS[n])
            {
                points = POINTS[n] + points;
                //printf("Found, index: %d\n", n);
            }
            else
            {
                //printf("Letter not found, index: %d\n", n);
            }
        }
    }
    return points;
}

// Function that separate all the letters of the word, and uppercase them.
void make_upper(char *s)
{
    for (int i = 0; i < strlen(s); i++)
    {
        s[i] = toupper(s[i]);
    }
}