//#include "erl_nif.h"
#include "series_compress.h"
#include "compress.h"
#include "decompress.h"
#include "bindbg.h"
#include <math.h>

#define NUMS 20

int main(void) {
    ErlNifBinary source;
    ErlNifBinary compressed;
    ErlNifBinary dest;
    size_t compressed_bits;
    size_t doubles;
    source.data = malloc(sizeof(char) * 8 * NUMS);
    source.size = 8 * NUMS;
    compressed.data = malloc(sizeof(char) * 8 * NUMS);
    compressed.size = 8 * NUMS;
    dest.data = malloc(sizeof(char) * 8 * NUMS);
    dest.size = 8 * NUMS;
    double j=0.0;
    union {
        char * aschar;
        double * asdub;
    } asdf;
    asdf.aschar = source.data;
    for (int i=0; i<NUMS; i++) {
        asdf.asdub[i] = cos(j);
        j+=.05;
    }
    union conversion conv;
    compressed_bits = compress(&source, &compressed);
    //printf("Compressed size in bits: %lu\n", compressed_bits);
    if (compressed_bits % 8 > 0) {
        doubles = (compressed_bits / 64) + 1;
    } else {
        doubles = compressed_bits / 64;
    }
    //printf("Compressed size in doubles: %lu\n", doubles);
    conv.as_bytes = compressed.data;
    for (size_t i=0; i<doubles; i++) {
        print_int_as_bin(conv.as_int64_t[i]);
    }
    decompress(&compressed, &dest);
    union {
        char * aschar;
        double * asdub;
    } asdf2;
    asdf2.aschar = dest.data;
    printf("\n");
    for (int i=0; i<NUMS; i++) {
        printf("%i: %f\n", i, asdf2.asdub[i]);
    }

    return 0;
}

//static ERL_NIF_TERM compress64(ErlNifTerm * env, int argc, const ERL_NIF_TERM argv[]) {
//    ErlNifBinary * destination;
//    ErlNifBinary source;
//    int word_size;
//
//    if (argc != 1) {
//        return enif_make_badarg(env);
//    } else if (!enif_inspect_binary(env, argv[0], &source)) {
//        return enif_make_badarg(env);
//    } else if (source.size % 8 != 0) {
//        return enif_make_badarg(env);
//    }
//
//    if (!compress(source, word_size, destination)) {
//        // return error
//    }
//    return enif_make_binary(env, destination);
//}
//
//static ErlNifFunc funcs[] = {
//    {"compress64", 1, compress64},
//    {"decompress64", 1, decompress64},
//    {"int64_list_to_binary", 1, int64_list_to_binary},
//    {"double_list_to_binary", 1, double_list_to_binary},
//};
//
//int reload(ErlNifEnv* env, void** priv, ERL_NIF_TERM info) {
//    return 0;
//}
//
//int upgrade(ErlNifEnv* env, void** priv, void** old_priv, ERL_NIF_TERM info) {
//    return 0;
//}
//
//ERL_NIF_INIT(series_compress, funcs, NULL, &reload, &upgrade, NULL)
