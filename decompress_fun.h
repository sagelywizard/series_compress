#ifndef DECOMPRESS_H
#define DECOMPRESS_H

#include "series_compress.h"
#include "util.h"

int decompress(ErlNifBinary * source0, ErlNifBinary * destination);

#endif
