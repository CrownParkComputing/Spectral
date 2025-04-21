// tinyARC4, ARC4 stream cypher. based on code by Mike Shaffer.
// - rlyeh, public domain ~~ listening to Black Belt - Leeds | wtrmrkrlyeh

//#pragma once
#include <string.h>

char* tinyARC4( char *output, const char *input, unsigned inlen, const char *passkey ) {
    if( passkey ) {
        int sbox[256], key[256], passlen = (int)strlen(passkey);
        for( unsigned a = 0; a < 256; a++ ) {
            key[ a ] = passkey[ a % passlen ];
            sbox[ a ] = a;
        }
        for( unsigned a = 0, b = 0; a < 256; a++ ) {
            b = (b + sbox[ a ] + key[ a ]) % 256;
            int swap = sbox[ a ]; sbox[ a ] = sbox[ b ]; sbox[ b ] = swap;
        }
        for( unsigned a = 0, i = 0, j = 0, k; a < inlen; a++ ) {
            i = (i + 1) % 256;
            j = (j + sbox[ i ]) % 256;
            int swap = sbox[ i ]; sbox[ i ] = sbox[ j ]; sbox[ j ] = swap;
            k = sbox[(sbox[ i ] + sbox[j]) % 256];
            output[ a ] = input[ a ] ^ k;
        }
    } else {
        memcpy(output, input, inlen);
    }
    return output;
}

