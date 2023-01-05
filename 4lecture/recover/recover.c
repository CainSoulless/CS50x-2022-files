#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    FILE *input = fopen(argv[1], "r");
    //  Check usage.
    if (argc == 1 || argc > 2 || !input)
    {
        printf("Usage: %s IMAGE\n", argv[0]);
        return 1;
    }

    // Reading file.
    BYTE bytes[512];
    char filename[8];
    FILE *newJpg = NULL;
    int count = 0;

    //  Reading all the buffer.
    while (fread(bytes, sizeof(bytes), 1, input) == 1)
    {
        //  Comprobe if is jpeg file
        if (bytes[0] == 0xff  && bytes[1] == 0xd8 && bytes[2] == 0xff && (bytes[3] & 0xf0) == 0xe0)
        {
            sprintf(filename, "%03i.jpg", count);
            newJpg = fopen(filename, "w");
            count++;
        }
        // If found a jpg header, write a new JPG file.
        if (!(count == 0))
        {
            fwrite(&bytes, sizeof(bytes), 1, newJpg);
        }
    }
    // Close i/o file.
    fclose(input);
    fclose(newJpg);
}
