#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <byteswap.h>

const char *USAGE = "Usage info";

// https : // www.w3.org/TR/2003/REC-PNG-20031110/#11IHDR/
// SIGNATURE: 137 80 78 71 13 10 26 10
// DEC   73 72 68 82
// HEX   49 48 44 52
// ASCII IHDR

const char SIGNATURE[] = {137, 80, 78, 71, 13, 10, 26, 10};
const char IHDR[] = {73, 72, 68, 82};

struct BasicChunkInfo
{
    char type[4];
    __uint16_t data_length;
    __uint16_t total_length;
    __uint16_t CRC;
};

struct IHDR
{
    struct BasicChunkInfo base;
    __uint8_t width;
    __uint8_t height;
    __uint8_t depth;
    __uint8_t color_type;
    __uint8_t compression_method;
    __uint8_t filter_method;
    __uint8_t interlace_method;
};

int has_png_signature(FILE *fp)
{
    fseek(fp, 0, SEEK_SET);
    char signature[8];
    for (size_t i = 0; i < 8; i++)
        signature[i] = getc(fp);

    return strncmp(signature, SIGNATURE, 8);
}

int big_endian(char c[4])
{
    return (c[0] << 24) | ((c[1] & 0xFF) << 16) | ((c[2] & 0xFF) << 8) | (c[3] & 0xFF);
}

void read_n_bytes(FILE *fp, uint n, char buffer[n])
{
    char c;
    for (size_t i = 0; i < n; i++)
    {
        c = getc(fp);
        if (c == EOF)
            break;

        buffer[i] = c;
    }
}

void read_base_info(FILE *fp, struct BasicChunkInfo *base)
{
    char c;

    char chunk_length[4];
    read_n_bytes(fp, 4, chunk_length);
    base->data_length = big_endian(chunk_length);

    read_n_bytes(fp, 4, base->type);

    base->total_length = base->data_length + 12; // Chunk length + chunk type + data length + CRC
}

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

    if (has_png_signature(fp) != 0)
    {
        fprintf(stderr, "File is not starting with PNG signature");
        return 1;
    }

    struct BasicChunkInfo base;
    read_base_info(fp, &base);

    return 0;
}
