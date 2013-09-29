#include "series_compress.h"

char *int2bin(uint64_t a, char *buffer, int buf_size) {
    buffer += (buf_size - 1);

    for (int i = 63; i >= 0; i--) {
        *buffer-- = (a & 1) + '0';

        a >>= 1;
    }

    return buffer;
}

union convertoid {
    int64_t s64;
    uint64_t u64;
};

void print_uint_as_bin(uint64_t a) {
    char buffer[BUF_SIZE];
    buffer[BUF_SIZE - 1] = '\0';

    int2bin(a, buffer, BUF_SIZE - 1);

    printf("%s", buffer);
}

void print_int_as_bin(int64_t a) {
    union convertoid conv;
    conv.s64 = a;
    print_uint_as_bin(conv.u64);
}

void print_shit(char * stuffs, uint64_t chars) {
    uint64_t chars2 = chars/8;
    union {
        char * aschar;
        uint64_t * asint;
    } qwer;
    qwer.aschar = stuffs;
    for (uint64_t i=0; i<chars2; i++) {
        print_int_as_bin(qwer.asint[i]);
    }
}
