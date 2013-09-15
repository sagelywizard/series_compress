all:
	clang -Wall -I./ -o series_compress bindbg.c util.c compress.c decompress.c series_compress.c
