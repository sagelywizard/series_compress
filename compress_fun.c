#include "series_compress.h"
#include "bindbg.h"
#include "util.h"

// delta must be 64 bits.
void combine(workspace * workspace, data_point delta) {
    int remainder;
    size_t i = workspace->i;
    union point_conversion datum;
    datum.as_int64_t = delta.datum;
    workspace->total_bits += delta.bit_count + 1;
    remainder = delta.bit_count + workspace->bits + 1;
    workspace->data[i] = workspace->data[i] | (datum.as_uint64_t >> workspace->bits);
    // if there's overflow
    if (remainder > 64) {
        workspace->i += 1;
        workspace->data[workspace->i] = datum.as_uint64_t << (64 - workspace->bits);
    } else if (remainder == 64) {
        workspace->i += 1;
        workspace->data[workspace->i] = 0;
    }
    workspace->bits = (workspace->bits + delta.bit_count + 1) % 64;
}

data_point compress_int(int64_t first, int64_t second) {
    union point_conversion datum;
    data_point delta;
    delta.bit_count = 64;
    datum.as_int64_t = first - second;
    if (datum.as_int64_t == 0) {
        delta.datum = 0;
        delta.bit_count = 0;
        return delta;
    } else if (datum.as_int64_t > 0) {
        while (datum.as_int64_t > 0) {
            datum.as_uint64_t = datum.as_uint64_t << 1;
            delta.bit_count -= 1;
        }
        datum.as_uint64_t = datum.as_uint64_t >> 1;
    } else {
        while (datum.as_int64_t < 0) {
            datum.as_uint64_t = datum.as_uint64_t << 1;
            delta.bit_count -= 1;
        }
        datum.as_uint64_t = datum.as_uint64_t >> 1;
        // flip the leftmost bit to a 1
        datum.as_uint64_t = datum.as_uint64_t ^ LLONG_MIN;
    }
    delta.datum = datum.as_int64_t;
    return delta;
}

int compress(ErlNifBinary * source, ErlNifBinary * dest) {
//int compress(ErlNifBinary * source, ErlNifBinary * dest) {
    uint64_t size = source->size/8;
    uint64_t workspace_data[size];
    double extrapolation_points[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
    data_point delta, header;
    workspace workspace;
    workspace.i = 0;
    workspace.bits = 0;
    workspace.total_bits = 0;
//    workspace.data = enif_alloc(source.size);
    workspace.data = workspace_data;
    workspace.data[0] = 0;

    union conversion destination;
    destination.as_bytes = dest->data;

    union {
        double as_double;
        int64_t as_int64_t;
    } prediction;

    union {
        double * as_double;
        int64_t * as_int64_t;
        unsigned char * as_bytes;
    } source_data;

    source_data.as_bytes = source->data;

    workspace.bits += 32;
    workspace.total_bits += 32;

    for (int i=0; i<size; i++) {
        prediction.as_double = predict_data_point(extrapolation_points);

//        printf("predictio: ");
//        print_int_as_bin(prediction.as_int64_t);
//        printf("\n");
//        printf("goes in  : %d: ", i);
//        print_int_as_bin(source_data.as_int64_t[i]);
//        printf("\n");
//        printf("delta in : %d: ", i);
        delta = compress_int(source_data.as_int64_t[i], prediction.as_int64_t);
//        printf("\n");
        header.bit_count = 5;
        header.datum = delta.bit_count << 58;
        combine(&workspace, header);
        combine(&workspace, delta);

        for (int j=0; j<4; j++) {
            extrapolation_points[j] = extrapolation_points[j+1];
        }
        extrapolation_points[4] = source_data.as_double[i];
    }
//    printf("Herpa derpa\n");
    workspace.data[0] |= ((source->size*8) << 32);
//    printf("Herpa derpa 1\n");
    for (int i=0; i<=workspace.i; i++) {
        destination.as_uint64_t[i] = workspace.data[i];
    }
//    printf("Herpa derpa 2\n");
//    printf("\n\n--\n\n");
    return workspace.total_bits;
}

