/*
    Takes a text and then makes a Coleman test, knowing the Grade of the text typed.

    Coleman equation: index = 0.0588 * L - 0.296 * S - 15.8
    L = letterscount / wordscount * 100
    S = sentences / wordscount * 100
*/

#include <cs50.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

void coleman(string text);
void make_upper(char *s);
void counter(char *text);

// L is for letters, and S is for sentences
double L = 0.0, S = 0.0;
int wordscount = 0, letterscount = 0, sentencescount = 0, roundindex = 0;
double index = 0;

int main(void)
{
    string text;
    text = get_string("Text: ");

    coleman(text);
}

// index = 0.0588 * L - 0.296 * S - 15.8
void coleman(string text)
{
    make_upper(text);
    counter(text);

    L = (double)letterscount / (double)wordscount * 100;
    S = (double)sentencescount / (double)wordscount * 100;
    index = 0.0588 * L - 0.296 * S - 15.8;
    roundindex = (int)round(index);

    printf("\nL: %f\nS: %f\n", L, S);

    if (roundindex >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (roundindex < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %d\n", roundindex);
    }
}

// Function uppercase letter by letter, because we just want one case letter.
void make_upper(char *s)
{
    for (int n = 0; n < strlen(s); n++)
    {
        s[n] = toupper(s[n]);
    }
}

// Makes the difference between a word and a letter and count them.
void counter(char *text)
{
    // For that loops the number total of characteres on the sentence.
    for (int i = 0; text[i] != '\0' ; i++)
    {
        // Words counter.
        if (text[i] == ' ' && text[i + 1] != ' ')
        {
            wordscount++;
        }
        // Recognize if the text variable has or not a sentence.
        else if (wordscount == 0 && letterscount == 0 && text[i - 1] != ' ')
        {
            wordscount++;
            if ((int)text[i] == 39)
            {
                wordscount--;
            }
        }
        // Letter counter.
        if ((int)text[i] >= 65 && (int)text[i] <= 90)
        {
            letterscount++;
        }
        // Sentences count.
        if ((int)text[i] == 46 || (int)text[i] == 63 || (int)text[i] == 33)
        {
            sentencescount++;
        }
    }
}
