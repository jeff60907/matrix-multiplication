#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <x86intrin.h>
#include "impl.c"

#define MARTRIX1_H 1024
#define MARTRIX1_W 1024
#define MARTRIX2_H 1024
#define MARTRIX2_W 1024


static long diff_in_us(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec * 1000000.0 + diff.tv_nsec / 1000.0);
}

int main(int argc, char *argv[])
{
    /*
    int martrix_1[64] = { 0,  1,  2,  3,   0,  1,  2,  3,
                              4,  5,  6,  7,   4,  5,  6,  7,
                              8,  9, 10, 11,   8,  9, 10, 11,
                              12, 13, 14, 15, 12, 13, 14, 15,
                              0,  1,  2,  3,   0,  1,  2,  3,
                              4,  5,  6,  7,   4,  5,  6,  7,
                              8,  9, 10, 11,   8,  9, 10, 11,
                              12, 13, 14, 15, 12, 13, 14, 15,
                               };
    int martrix_2[64] = { 16, 17, 18, 19, 16, 17, 18, 19,
                              20, 21, 22, 23, 20, 21, 22, 23,
                              24, 25, 26, 27, 24, 25, 26, 27,
                              28, 29, 30, 31, 28, 29, 30, 31,
                              16, 17, 18, 19, 16, 17, 18, 19,
                              20, 21, 22, 23, 20, 21, 22, 23,
                              24, 25, 26, 27, 24, 25, 26, 27,
                              28, 29, 30, 31, 28, 29, 30, 31,
                              };
    int ans[64] = {   304,  316,  328,  340,  304,  316,  328,  340,
                               1008, 1052, 1096, 1140, 1008, 1052, 1096, 1140,
                               1712, 1788, 1864, 1940, 1712, 1788, 1864, 1940,
                               2416, 2524, 2632, 2740, 2416, 2524, 2632, 2740,
                               304,  316,  328,  340,  304,  316,  328,  340,
                               1008, 1052, 1096, 1140, 1008, 1052, 1096, 1140,
                               1712, 1788, 1864, 1940, 1712, 1788, 1864, 1940,
                               2416, 2524, 2632, 2740, 2416, 2524, 2632, 2740
                               };
    int test_out[64] = {};

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++)
            printf(" %2d", martrix_1[y * 8 + x]);
        printf("\n");
    }
    printf("\n");

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++)
            printf(" %2d", martrix_2[y * 8 + x]);
        printf("\n");
    }
    printf("\n");

    sse_multiply_prefetch(martrix_1, martrix_2, test_out, 8, 8, 8, 8);

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++)
            printf(" %2d", test_out[y * 8 + x]);
        printf("\n");
    }

    assert(0 == memcmp(test_out, ans, 64 * sizeof(int)) &&
           "Verification fails");
    */
    struct timespec start, end;
    int *src1 = (int *) malloc(sizeof(int) * MARTRIX1_W * MARTRIX1_H);
    int *src2 = (int *) malloc(sizeof(int) * MARTRIX2_W * MARTRIX2_H);
    int *out1 = (int *) malloc(sizeof(int) * MARTRIX1_W * MARTRIX2_H);

    srand(time(NULL));
    for (int i = 0; i < MARTRIX1_H; i++) {
        for (int j = 0; j < MARTRIX1_W; j++) {
            src1[i * MARTRIX1_W + j] = rand();
        }
    }
    for (int i = 0; i < MARTRIX2_H; i++) {
        for (int j = 0; j < MARTRIX2_W; j++) {
            src2[i * MARTRIX2_W + j] = rand();
        }
    }

#if defined(naive)
    clock_gettime(CLOCK_REALTIME, &start);
    naive_multiply(src1, src2, out1, MARTRIX1_W, MARTRIX1_H, MARTRIX2_W, MARTRIX2_H);
    clock_gettime(CLOCK_REALTIME, &end);
    printf("naive: \t\t %ld us\n", diff_in_us(start, end));
#endif

#if defined(sub_matrix)
    clock_gettime(CLOCK_REALTIME, &start);
    sub_matrix_multiply(src1, src2, out1, MARTRIX1_W, MARTRIX1_H, MARTRIX2_W, MARTRIX2_H);
    clock_gettime(CLOCK_REALTIME, &end);
    printf("sub_matrix: \t\t %ld us\n", diff_in_us(start, end));
#endif

#if defined(sse)
    clock_gettime(CLOCK_REALTIME, &start);
    sse_multiply(src1, src2, out1, MARTRIX1_W, MARTRIX1_H, MARTRIX2_W, MARTRIX2_H);
    clock_gettime(CLOCK_REALTIME, &end);
    printf("sse: \t\t %ld us\n", diff_in_us(start, end));
#endif

#if defined(sse_prefetch)
    clock_gettime(CLOCK_REALTIME, &start);
    sse_multiply_prefetch(src1, src2, out1, MARTRIX1_W, MARTRIX1_H, MARTRIX2_W, MARTRIX2_H);
    clock_gettime(CLOCK_REALTIME, &end);
    printf("sse_prefetch: \t\t %ld us\n", diff_in_us(start, end));
#endif

    free(src1);
    free(src2);
    free(out1);

    return 0;

}
