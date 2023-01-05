// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Variables
unsigned int word_count;
unsigned int hash_value;
// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    hash_value = hash(word);
    node *newNode = table[hash_value];

    // Traversing while loop.
    while (newNode != 0)
    {
        if (strcasecmp(word, newNode->word) == 0)
        {
            return true;
        }
        newNode = newNode->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    unsigned int value;
    for (int i = 0; i < (int)strlen(word); i++)
    {
        value = tolower(word[i]);
    }
    return value % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *file = fopen(dictionary, "r");

    //  Check if dictionary was able to open it.
    if (file == NULL)
    {
        return false;
    }
    char word[LENGTH + 1];
    //  Read any word into the file.
    while (fscanf(file, "%s", word) != EOF)
    {
        //  Allocate memory for any word.
        node *newNode = (node *)malloc(sizeof(node));
        if (newNode == NULL)
        {
            unload();
            return false;
        }
        // Copies word into malloc
        strcpy(newNode->word, word);

        hash_value = hash(word);
        newNode->next = table[hash_value];
        table[hash_value] = newNode;
        word_count++;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if (word_count > 0)
    {
        return word_count;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        node *n = table[i];

        while (n != NULL)
        {
            node *tmp = n;
            n = n->next;
            free(tmp);
        }

        if (n == NULL && i == N - 1)
        {
            return true;
        }
    }
    return false;
}