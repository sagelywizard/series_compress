# series\_compress

This level of bit-twiddling is tricky, even in C. Might as well make it fast.

#### How it works:

series\_compress takes some mod-64-bits binary data (ie. `bit_length(data) % 64 == 0`). It treats each 64 bits as an unsigned integer. The delta between each data point in the series is taken. Starting with the first bit of the delta, all contiguous bits of the same value are removed except one. For example:

    0000000000000000000000000000000000000000000000000000000110101110

would be compressed to:

    0110101110

Then, a six-bit header containing the compressed length is prepended to the compressed delta. Since number of bits in the previous example was 10 (ie. 001010 bits), the fully compressed data point would be `0010100110101110`.

#### API

    compress/1: Given a binary or a list, will return a compressed binary.
    decompress/1: Given a compressed binary, will return an uncompressed binary.
    append/2: Given a compressed binary and an uncompressed binary, will return a compressed binary with uncompressed binary appended.
