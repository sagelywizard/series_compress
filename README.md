# series\_compress

This level of bit-twiddling is tricky, even in C. Might as well make it fast.

#### How it works:

series\_compress takes some mod-64-bits binary data (ie. `bit_length(data) % 64 == 0`). It treats each 64 bits as an unsigned integer. The delta between each data point in the series is taken. Starting with the first bit of the delta, all contiguous bits of the same value are removed except one. For example:

    0000000000000000000000000000000000000000000000000000000110101110

would be compressed to:

    0110101110

Then, a six-bit header containing the compressed length is prepended to the compressed delta. Since number of bits in the previous example was 10 (ie. 001010 bits), the fully compressed data point would be `0010100110101110`.

#### Example Usage:

    1> TimeSeries = [15,18,19,20,19,20,23,25].
    [15,18,19,20,19,20,23,25]
    2> TimeSeriesBinary = series_compress:int64_list_to_binary(TimeSeries).
    <<0,0,0,0,0,0,0,15,0,0,0,0,0,0,0,18,0,0,0,0,0,0,0,19,0,0,
      0,0,0,...>>
    3> Compressed = series_compress:compress64(TimeSeriesBinary).
    etc, etc, etc
