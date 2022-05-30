#include <stdio.h>
#include <string.h>

const char *USAGE = "Usage info";

// https : // www.w3.org/TR/2003/REC-PNG-20031110/#11IHDR/
// SIGNATURE: 137 80 78 71 13 10 26 10
// DEC   73 72 68 82
// HEX   49 48 44 52
// ASCII IHDR

const char SIGNATURE[] = {137, 80, 78, 71, 13, 10, 26, 10};
const char IHDR[] = {73, 72, 68, 82};

struct IHDR
{
    __uint16_t data_length;
    __uint16_t type;
    __uint16_t CRC;
    __uint8_t width;
    __uint8_t height;
    __uint8_t depth;
    __uint8_t color_type;
    __uint8_t compression_method;
    __uint8_t filter_method;
    __uint8_t interlace_method;
};

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("%s\n", USAGE);
        return 1;
    }

    char *path = argv[1];

    FILE *fp = fopen(path, "r");

    if (fp == NULL)
    {
        fprintf(stderr, "Failed to open file: '%s'", path);
        return 1;
    }

    int counter = 0;
    char signature[8];

    for (size_t i = 0; i < 8; i++)
        signature[i] = getc(fp);

    int x = strcmp(signature, SIGNATURE);

    if (strncmp(signature, SIGNATURE, 8) != 0)
    {
        fprintf(stderr, "File is not starting with PNG signature");
        return 1;
    }

    char c;
    while ((c = getc(fp)) != EOF)
    {
    }

    return 0;
}
