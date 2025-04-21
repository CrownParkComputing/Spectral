// arc4 file de/crypt
// - rlyeh, public domain

#include <stdlib.h>
#include <stdio.h>
#include "../../3rd_arc4.h"

unsigned char *readfile(const char *pathfile, int *size) {
    unsigned char *bin = 0;
    if( size ) *size = 0;
    for( FILE *fp = fopen(pathfile,"rb"); fp; fclose(fp), fp = 0) {
        fseek(fp, 0L, SEEK_END);
        size_t len = ftell(fp);
        if(size) *size = (int)len;
        fseek(fp, 0L, SEEK_SET);
        bin = (unsigned char*)malloc(len+1);
        if( bin && fread(bin, 1, len, fp) == len ) bin[len] = '\0';
        else free(bin), bin = 0;
    }
    return bin;
}
int writefile(const char *pathfile, const void *blob, int len) {
    int ok = 0;
    FILE *fp = fopen(pathfile, "wb");
    if( fp ) {
        ok = fwrite(blob, len, 1, fp) == 1;
        fclose(fp);
    }
    return ok;
}

#define die(...) exit((fprintf(stderr, __VA_ARGS__),-1))

int main(int argc, char **argv) {
    if( argc < 4 ) die("Usage: %s infile outfile passkey [offset=0]\n", argv[0]);

    char *infile = argv[1];
    char *outfile = argv[2];
    char *passkey = argv[3];
    int offset = argc > 4 ? atoi(argv[4]) : 0;

    int inlen; char *indata = readfile(infile, &inlen);
    if( indata && inlen ) {
        char *outdata = calloc(1, inlen); // @leak
        if( outdata ) {
            memcpy(outdata, indata, inlen);
            tinyARC4(outdata + offset, indata + offset, inlen - offset, passkey);
            if( writefile(outfile, outdata, inlen) ) {
                return puts("Ok"), 0;
            } else die("cannot write output file `%s`\n", outfile);
        } else die("cannot allocate memory for `%d` bytes\n", inlen);
    } else die("cannot read file `%s`\n", infile);

    return -1;
}
