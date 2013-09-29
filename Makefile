all:
	clang -Wall -I./ -o series_compress bindbg.c util.c compress_fun.c decompress_fun.c series_compress.c
