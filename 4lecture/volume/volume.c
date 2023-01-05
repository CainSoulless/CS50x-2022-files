// Modifies the volume of an audio file
#include <cs50.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;
// I prefered use typedef just for learn.
typedef int16_t     WORD;
typedef uint8_t     BYTE;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file
    BYTE bytes[HEADER_SIZE];

    fread(bytes, sizeof(bytes), 1, input);
    fwrite(bytes, sizeof(bytes), 1, output);

    // TODO: Read samples from input file and write updated data to output file
    WORD temp;
    // Loop ends when all the file was read.
    while (fread(&temp, sizeof(temp), 1, input))
    {
        temp *= factor;
        // Writes all the file to the output.
        fwrite(&temp, sizeof(temp), 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);
}