#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "impl.c"


int main(int argc, char *argv[])
{
    int martrix_1[16] = { 0,  1,  2,  3,
                          4,  5,  6,  7,
                          8,  9, 10, 11,
                          12, 13, 14, 15
                        };
    int martrix_2[16] = { 16, 17, 18, 19,
                          20, 21, 22, 23,
                          24, 25, 26, 27,
                          28, 29, 30, 31
                        };
    int ans[16] = { 152,  158,  164,  170,
                    504,  526,  548,  570,
                    856,  894,  932,  970,
                    1208, 1262, 1316, 1370
                  };
    int test_out[16] = { 0, 0, 0, 0,
                         0, 0, 0, 0,
                         0, 0, 0, 0,
                         0, 0, 0, 0
                       };

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++)
            printf(" %2d", martrix_1[y * 4 + x]);
        printf("\n");
    }
    printf("\n");

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++)
            printf(" %2d", martrix_2[y * 4 + x]);
        printf("\n");
    }
    printf("\n");

    naive_multiply(martrix_1, martrix_2, test_out, 4, 4, 4, 4);

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++)
            printf(" %2d", test_out[y * 4 + x]);
        printf("\n");
    }

    assert(0 == memcmp(test_out, ans, 16 * sizeof(int)) &&
           "Verification fails");

    return 0;

}
