#include "series_compress.h"
#include "bindbg.h"
#include "util.h"

int decompress(ErlNifBinary * source0, ErlNifBinary * dest0) {
    printf("\n");
    uint64_t decompressed_size;
    double extrapolation_points[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
    union conversion source;
    union conversion dest;
    workspace workspace;
    workspace.data = malloc(dest0->size);
    source.as_bytes = source0->data;
    dest.as_bytes = dest0->data;
    workspace.data = source.as_uint64_t;
    workspace.i = 0;
    workspace.bits = 0;
    workspace.total_bits = 0;
    union point_conversion prediction;
    union point_conversion the_d;
    union point_conversion thingg;
    uint64_t delta;

    int i = 0;

    workspace.bits += 32;

    decompressed_size = (source.as_uint64_t[0] >> 32);

    while (workspace.total_bits < decompressed_size) {
        prediction.as_double = predict_data_point(extrapolation_points);
        printf("predictio: ");
        print_int_as_bin(prediction.as_int64_t);
        printf("\n");
        printf("delta out: %d: ", i);
        if (!read_block(&workspace, &delta)) {
            return 1;
        }
        the_d.as_uint64_t = delta;
        thingg.as_int64_t = the_d.as_int64_t + prediction.as_int64_t;
        printf("comes out: %d: ", i);
        print_int_as_bin(thingg.as_int64_t);
        printf("\n");
        dest.as_double[i] = thingg.as_double;

        for (int j=0; j<4; j++) {
            extrapolation_points[j] = extrapolation_points[j+1];
        }
        extrapolation_points[4] = thingg.as_double;
        i += 1;
    }
    return 0;
}

int read_block(workspace * workspace, uint64_t * dest) {
    int64_t code_point;
    union point_conversion cp;
    uint64_t length = (workspace->data[workspace->i] << workspace->bits) >> 58;
    if (workspace->bits > 58) {
        workspace->i += 1;
        length |= workspace->data[workspace->i] >> (64 - ((6 + workspace->bits) % 64));
    }
    length += 1;
    printf("LENGTH: %llu\n", length);
    workspace->bits = (workspace->bits + 6) % 64;
    code_point = (workspace->data[workspace->i] << workspace->bits);
    if ((workspace->bits + length) > 64) {
        workspace->i += 1;
        code_point |= (((workspace->data[workspace->i] >> (64 - workspace->bits)) >> (64 - length)) << (64 - length));
    } else {
        code_point = (code_point >> (64 - length)) << (64 - length);
    }
    cp.as_int64_t = code_point;
    if (code_point >= 0) {
        cp.as_uint64_t = cp.as_uint64_t >> (64 - length);
    } else {
        cp.as_uint64_t = cp.as_uint64_t >> (64 - length);
        if (length != 64) {
            cp.as_uint64_t |= (ULLONG_MAX << length);
        }
    }
    print_uint_as_bin(cp.as_uint64_t);
    printf("\nFUUUU\n");
    print_uint_as_bin((ULLONG_MAX << length));
    printf("\n");
    workspace->bits = (workspace->bits + length) % 64;
    workspace->total_bits += 64;
    (*dest) = cp.as_uint64_t;
    return 1;
}
