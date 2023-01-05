#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    string names[] = {"Bill", "George", "Rodrigo", "Catalina"};

    for (int i = 0; i < strlen(*names); i++)
    {
        if (!strcmp(names[i], "Bill"))
        {
            printf("Found\n");
            return 0;
        }
    }
    printf("Not found\n");
    return 1;

}