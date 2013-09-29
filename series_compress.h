#ifndef SERIES_COMPRESS_H
#define SERIES_COMPRESS_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "erl_nif.h"

#define TRUE 1
#define FALSE 0

#define BUF_SIZE 65

union conversion {
    int64_t * as_int64_t;
    uint32_t * as_uint32_t;
    uint64_t * as_uint64_t;
    double * as_double;
    unsigned char * as_bytes;
};

typedef struct {
    int64_t bit_count;
    int64_t datum;
} data_point;

typedef struct {
    int64_t i;
    int64_t bits;
    uint64_t total_bits;
    uint64_t * data;
} workspace;

//typedef struct {
//    unsigned char * data;
//    size_t size;
//} ErlNifBinary;

union point_conversion {
    int64_t as_int64_t;
    uint32_t as_uint32_t;
    uint64_t as_uint64_t;
    double as_double;
    unsigned char as_bytes;
};

int read_block(workspace * workspace, uint64_t * dest);

#endif
