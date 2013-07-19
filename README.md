# series\_compress

This level of bit-twiddling is tricky, even in C. Might as well make it fast.

Example Usage:

    1> TimeSeries = [15,18,19,20,19,20,23,25].
    [15,18,19,20,19,20,23,25]
    2> TimeSeriesBinary = series_compress:int64_list_to_binary(TimeSeries).
    <<0,0,0,0,0,0,0,15,0,0,0,0,0,0,0,18,0,0,0,0,0,0,0,19,0,0,
      0,0,0,...>>
    3> Compressed = series_compress:compress64(TimeSeriesBinary).
    etc, etc, etc
