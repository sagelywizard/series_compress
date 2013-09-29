#include "series_compress.h"
#include "compress_fun.h"
#include "decompress_fun.h"
#include "bindbg.h"
#include <math.h>

//#define NUMS 20
//
//int main(void) {
//    ErlNifBinary source;
//    ErlNifBinary compressed;
//    ErlNifBinary dest;
//    size_t compressed_bits;
//    size_t doubles;
//    source.data = malloc(sizeof(char) * 8 * NUMS);
//    source.size = 8 * NUMS;
//    compressed.data = malloc(sizeof(char) * 8 * NUMS);
//    compressed.size = 8 * NUMS;
//    dest.data = malloc(sizeof(char) * 8 * NUMS);
//    dest.size = 8 * NUMS;
//    double j=0.0;
//    union {
//        unsigned char * aschar;
//        double * asdub;
//    } asdf;
//    asdf.aschar = source.data;
//    for (int i=0; i<NUMS; i++) {
//        asdf.asdub[i] = cos(j);
//        j+=.05;
//    }
//    union conversion conv;
//    compressed_bits = compress(&source, &compressed);
//    //printf("Compressed size in bits: %lu\n", compressed_bits);
//    if (compressed_bits % 8 > 0) {
//        doubles = (compressed_bits / 64) + 1;
//    } else {
//        doubles = compressed_bits / 64;
//    }
//    //printf("Compressed size in doubles: %lu\n", doubles);
//    conv.as_bytes = compressed.data;
//    for (size_t i=0; i<doubles; i++) {
//        print_int_as_bin(conv.as_int64_t[i]);
//    }
//    decompress(&compressed, &dest);
//    union {
//        unsigned char * aschar;
//        double * asdub;
//    } asdf2;
//    asdf2.aschar = dest.data;
//    printf("\n");
//    for (int i=0; i<NUMS; i++) {
//        printf("%i: %f\n", i, asdf2.asdub[i]);
//    }
//
//    return 0;
//}

static ERL_NIF_TERM compress_list_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifBinary destination;
    ErlNifBinary source;
    ERL_NIF_TERM list, head, tail;
    unsigned len;
    int i = 0;
    int total_bits;

    double dp;

    if (argc != 1) {
        return enif_make_badarg(env);
    } else if (!enif_is_list(env, argv[0])) {
        return enif_make_badarg(env);
    }

    enif_get_list_length(env, argv[0], &len);
    enif_alloc_binary(len*8, &source);
    enif_alloc_binary(len*9, &destination);

    union {
        unsigned char * as_bytes;
        double * as_doubles;
    } thingg;

    thingg.as_bytes = source.data;

    list = argv[0];
    while(enif_get_list_cell(env, list, &head, &tail)) {
        if (!enif_get_double(env, head, &dp)) {
            return enif_make_badarg(env);
        }
        thingg.as_doubles[i] = dp;
        list = tail;
        i += 1;
    }

    total_bits = compress(&source, &destination);
    if (total_bits % 8 == 0) {
        enif_realloc_binary(&destination, total_bits/8);
    } else {
        enif_realloc_binary(&destination, (total_bits/8)+1);
    }

    enif_release_binary(&source);
    return enif_make_binary(env, &destination);
}

static ERL_NIF_TERM decompress_list_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifBinary source;
    ErlNifBinary destination;
    ERL_NIF_TERM result;
    union conversion conv;
    unsigned len;

    if (argc != 1) {
        return enif_make_badarg(env);
    } else if (!enif_inspect_binary(env, argv[0], &source)) {
        return enif_make_badarg(env);
    }

    len = decompress(&source, &destination);
    conv.as_bytes = destination.data;

    result = enif_make_list(env, 0);
    for (int i=0; i<len; i++) {
        result = enif_make_list_cell(env, enif_make_double(env, conv.as_double[i]), result);
    }

    enif_release_binary(&destination);
    return result;
}

static ERL_NIF_TERM compress_binary_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifBinary * destination;
    ErlNifBinary * source;

    if (argc != 1) {
        return enif_make_badarg(env);
    } else if (!enif_inspect_binary(env, argv[0], source)) {
        return enif_make_badarg(env);
    } else if (source->size % 8 != 0) {
        return enif_make_badarg(env);
    }

    if (!compress(source, destination)) {
        // return error
    }
    return enif_make_binary(env, destination);
}

static ERL_NIF_TERM decompress_binary_nif(ErlNifEnv * env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifBinary * destination;
    ErlNifBinary * source;

    if (argc != 1) {
        return enif_make_badarg(env);
    } else if (!enif_inspect_binary(env, argv[0], source)) {
        return enif_make_badarg(env);
    }

    decompress(source, destination);
    return enif_make_binary(env, destination);
}

static ErlNifFunc funcs[] = {
    {"compress_binary", 1, compress_binary_nif},
    {"decompress_binary", 1, decompress_binary_nif},
    {"compress", 1, compress_list_nif},
    {"decompress", 1, decompress_list_nif},
};

int reload(ErlNifEnv* env, void** priv, ERL_NIF_TERM info) {
    return 0;
}

int upgrade(ErlNifEnv* env, void** priv, void** old_priv, ERL_NIF_TERM info) {
    return 0;
}

ERL_NIF_INIT(series_compress, funcs, NULL, &reload, &upgrade, NULL)
