#ifndef _MUTIPLY_H
#define _MULTIPLY_H

void naive_multiply(int *src1, int *src2, int *dst, int src1_w, int src1_h,
                    int src2_w, int src2_h)
{
    memset(dst, 0, sizeof(int) * src1_h * src2_w);
    for (int i = 0; i < src1_h; i++) {
        for (int j = 0; j < src2_w; j++) {
            for (int k = 0; k < src2_h; k++) {
                dst[i * src2_w + j] += src1[i * src1_w + k] * src2[k * src2_w + j];
            }
        }
    }
}

void sub_matrix_multiply(int *src1, int *src2, int *dst, int src1_w, int src1_h,
                         int src2_w, int src2_h)
{
    memset(dst, 0, sizeof(int) * src1_h * src2_w);
    for (int i = 0; i < src1_h; i += 4) {
        for (int j = 0; j < src2_w; j += 4) {
            for (int k = 0; k < src2_h; k += 4) {
                for (int i2 = 0; i2 < 4; i2++) {
                    for (int j2 = 0; j2 < 4; j2++) {
                        for (int k2 = 0; k2 < 4; k2++) {
                            dst[(i + i2) * src2_w + (j + j2 )] += src1[(i + i2) * src1_w + (k + k2)] *
                                                                  src2[(k + k2) * src2_w + (j + j2)];
                        }
                    }
                }
            }
        }
    }
}

void sse_multiply(int *src1, int *src2, int *dst, int src1_w, int src1_h,
                  int src2_w, int src2_h)
{
    for (int i = 0; i < src1_h; i += 4) {
        for (int j = 0; j < src2_w; j += 4) {
            __m128i des0 = _mm_setzero_si128 ();
            __m128i des1 = _mm_setzero_si128 ();
            __m128i des2 = _mm_setzero_si128 ();
            __m128i des3 = _mm_setzero_si128 ();
            for (int k = 0; k < src2_h; k += 4) {
                /* load 128bit in row */
                __m128i I0 = _mm_loadu_si128((__m128i *)(src1 + (i + 0) * src1_w + k));
                __m128i I1 = _mm_loadu_si128((__m128i *)(src1 + (i + 1) * src1_w + k));
                __m128i I2 = _mm_loadu_si128((__m128i *)(src1 + (i + 2) * src1_w + k));
                __m128i I3 = _mm_loadu_si128((__m128i *)(src1 + (i + 3) * src1_w + k));
                /* set src2 col */
                __m128i I4 = _mm_set_epi32 (src2[(k + 3) * src2_w + j],
                                            src2[(k + 2) * src2_w + j],
                                            src2[(k + 1) * src2_w + j],
                                            src2[(k + 0) * src2_w + j]);
                __m128i I5 = _mm_set_epi32 (src2[(k + 3) * src2_w + (j + 1)],
                                            src2[(k + 2) * src2_w + (j + 1)],
                                            src2[(k + 1) * src2_w + (j + 1)],
                                            src2[(k + 0) * src2_w + (j + 1)]);
                __m128i I6 = _mm_set_epi32 (src2[(k + 3) * src2_w + (j + 2)],
                                            src2[(k + 2) * src2_w + (j + 2)],
                                            src2[(k + 1) * src2_w + (j + 2)],
                                            src2[(k + 0) * src2_w + (j + 2)]);
                __m128i I7 = _mm_set_epi32 (src2[(k + 3) * src2_w + (j + 3)],
                                            src2[(k + 2) * src2_w + (j + 3)],
                                            src2[(k + 1) * src2_w + (j + 3)],
                                            src2[(k + 0) * src2_w + (j + 3)]);
                /* src1 mul src2 */
                __m128i T0 = _mm_mullo_epi32(I0, I4);
                __m128i T1 = _mm_mullo_epi32(I0, I5);
                __m128i T2 = _mm_mullo_epi32(I0, I6);
                __m128i T3 = _mm_mullo_epi32(I0, I7);

                __m128i T4 = _mm_mullo_epi32(I1, I4);
                __m128i T5 = _mm_mullo_epi32(I1, I5);
                __m128i T6 = _mm_mullo_epi32(I1, I6);
                __m128i T7 = _mm_mullo_epi32(I1, I7);

                __m128i T8 = _mm_mullo_epi32(I2, I4);
                __m128i T9 = _mm_mullo_epi32(I2, I5);
                __m128i T10 = _mm_mullo_epi32(I2, I6);
                __m128i T11 = _mm_mullo_epi32(I2, I7);

                __m128i T12 = _mm_mullo_epi32(I3, I4);
                __m128i T13 = _mm_mullo_epi32(I3, I5);
                __m128i T14 = _mm_mullo_epi32(I3, I6);
                __m128i T15 = _mm_mullo_epi32(I3, I7);

                /* transpose to add */
                __m128i T16 = _mm_unpacklo_epi32(T0, T1);
                __m128i T17 = _mm_unpacklo_epi32(T2, T3);
                __m128i T18 = _mm_unpackhi_epi32(T0, T1);
                __m128i T19 = _mm_unpackhi_epi32(T2, T3);

                __m128i T20 = _mm_unpacklo_epi64(T16, T17);
                __m128i T21 = _mm_unpackhi_epi64(T16, T17);
                __m128i T22 = _mm_unpacklo_epi64(T18, T19);
                __m128i T23 = _mm_unpackhi_epi64(T18, T19);

                T20 = _mm_add_epi32(T20,T21);
                T20 = _mm_add_epi32(T20,T22);
                T20 = _mm_add_epi32(T20,T23);
                des0 = _mm_add_epi32(T20,des0);

                T16 = _mm_unpacklo_epi32(T4, T5);
                T17 = _mm_unpacklo_epi32(T6, T7);
                T18 = _mm_unpackhi_epi32(T4, T5);
                T19 = _mm_unpackhi_epi32(T6, T7);

                T20 = _mm_unpacklo_epi64(T16, T17);
                T21 = _mm_unpackhi_epi64(T16, T17);
                T22 = _mm_unpacklo_epi64(T18, T19);
                T23 = _mm_unpackhi_epi64(T18, T19);

                T20 = _mm_add_epi32(T20,T21);
                T20 = _mm_add_epi32(T20,T22);
                T20 = _mm_add_epi32(T20,T23);
                des1 = _mm_add_epi32(T20,des1);

                T16 = _mm_unpacklo_epi32(T8, T9);
                T17 = _mm_unpacklo_epi32(T10, T11);
                T18 = _mm_unpackhi_epi32(T8, T9);
                T19 = _mm_unpackhi_epi32(T10, T11);

                T20 = _mm_unpacklo_epi64(T16, T17);
                T21 = _mm_unpackhi_epi64(T16, T17);
                T22 = _mm_unpacklo_epi64(T18, T19);
                T23 = _mm_unpackhi_epi64(T18, T19);

                T20 = _mm_add_epi32(T20,T21);
                T20 = _mm_add_epi32(T20,T22);
                T20 = _mm_add_epi32(T20,T23);
                des2 = _mm_add_epi32(T20,des2);

                T16 = _mm_unpacklo_epi32(T12, T13);
                T17 = _mm_unpacklo_epi32(T14, T15);
                T18 = _mm_unpackhi_epi32(T12, T13);
                T19 = _mm_unpackhi_epi32(T14, T15);

                T20 = _mm_unpacklo_epi64(T16, T17);
                T21 = _mm_unpackhi_epi64(T16, T17);
                T22 = _mm_unpacklo_epi64(T18, T19);
                T23 = _mm_unpackhi_epi64(T18, T19);

                T20 = _mm_add_epi32(T20,T21);
                T20 = _mm_add_epi32(T20,T22);
                T20 = _mm_add_epi32(T20,T23);
                des3 = _mm_add_epi32(T20,des3);
            }
            /* store to dst */
            _mm_storeu_si128((__m128i *)(dst + ((i + 0) * src2_w) + j), des0);
            _mm_storeu_si128((__m128i *)(dst + ((i + 1) * src2_w) + j), des1);
            _mm_storeu_si128((__m128i *)(dst + ((i + 2) * src2_w) + j), des2);
            _mm_storeu_si128((__m128i *)(dst + ((i + 3) * src2_w) + j), des3);
        }
    }
}

void sse_multiply_prefetch(int *src1, int *src2, int *dst, int src1_w, int src1_h,
                           int src2_w, int src2_h)
{
    for (int i = 0; i < src1_h; i += 4) {
        for (int j = 0; j < src2_w; j += 4) {
            __m128i des0 = _mm_setzero_si128 ();
            __m128i des1 = _mm_setzero_si128 ();
            __m128i des2 = _mm_setzero_si128 ();
            __m128i des3 = _mm_setzero_si128 ();
            for (int k = 0; k < src2_h; k += 4) {
#define PFDIST  8
                /* prefetch */
                _mm_prefetch(src1+(i + PFDIST + 0) * src1_w + k, _MM_HINT_T1);
                _mm_prefetch(src1+(i + PFDIST + 1) * src1_w + k, _MM_HINT_T1);
                _mm_prefetch(src1+(i + PFDIST + 2) * src1_w + k, _MM_HINT_T1);
                _mm_prefetch(src1+(i + PFDIST + 3) * src1_w + k, _MM_HINT_T1);
                //_mm_prefetch(src2+(k + PFDIST + 0) * src2_w + j, _MM_HINT_T1);
                //_mm_prefetch(src2+(k + PFDIST + 1) * src2_w + j, _MM_HINT_T1);
                //_mm_prefetch(src2+(k + PFDIST + 2) * src2_w + j, _MM_HINT_T1);
                //_mm_prefetch(src2+(k + PFDIST + 3) * src2_w + j, _MM_HINT_T1);

                /* load 128bit in row */
                __m128i I0 = _mm_loadu_si128((__m128i *)(src1 + (i + 0) * src1_w + k));
                __m128i I1 = _mm_loadu_si128((__m128i *)(src1 + (i + 1) * src1_w + k));
                __m128i I2 = _mm_loadu_si128((__m128i *)(src1 + (i + 2) * src1_w + k));
                __m128i I3 = _mm_loadu_si128((__m128i *)(src1 + (i + 3) * src1_w + k));
                /* set src2 col */
                __m128i I4 = _mm_set_epi32 (src2[(k + 3) * src2_w + j],
                                            src2[(k + 2) * src2_w + j],
                                            src2[(k + 1) * src2_w + j],
                                            src2[(k + 0) * src2_w + j]);
                __m128i I5 = _mm_set_epi32 (src2[(k + 3) * src2_w + (j + 1)],
                                            src2[(k + 2) * src2_w + (j + 1)],
                                            src2[(k + 1) * src2_w + (j + 1)],
                                            src2[(k + 0) * src2_w + (j + 1)]);
                __m128i I6 = _mm_set_epi32 (src2[(k + 3) * src2_w + (j + 2)],
                                            src2[(k + 2) * src2_w + (j + 2)],
                                            src2[(k + 1) * src2_w + (j + 2)],
                                            src2[(k + 0) * src2_w + (j + 2)]);
                __m128i I7 = _mm_set_epi32 (src2[(k + 3) * src2_w + (j + 3)],
                                            src2[(k + 2) * src2_w + (j + 3)],
                                            src2[(k + 1) * src2_w + (j + 3)],
                                            src2[(k + 0) * src2_w + (j + 3)]);
                /* src1 mul src2 */
                __m128i T0 = _mm_mullo_epi32(I0, I4);
                __m128i T1 = _mm_mullo_epi32(I0, I5);
                __m128i T2 = _mm_mullo_epi32(I0, I6);
                __m128i T3 = _mm_mullo_epi32(I0, I7);

                __m128i T4 = _mm_mullo_epi32(I1, I4);
                __m128i T5 = _mm_mullo_epi32(I1, I5);
                __m128i T6 = _mm_mullo_epi32(I1, I6);
                __m128i T7 = _mm_mullo_epi32(I1, I7);

                __m128i T8 = _mm_mullo_epi32(I2, I4);
                __m128i T9 = _mm_mullo_epi32(I2, I5);
                __m128i T10 = _mm_mullo_epi32(I2, I6);
                __m128i T11 = _mm_mullo_epi32(I2, I7);

                __m128i T12 = _mm_mullo_epi32(I3, I4);
                __m128i T13 = _mm_mullo_epi32(I3, I5);
                __m128i T14 = _mm_mullo_epi32(I3, I6);
                __m128i T15 = _mm_mullo_epi32(I3, I7);

                /* transpose to add */
                __m128i T16 = _mm_unpacklo_epi32(T0, T1);
                __m128i T17 = _mm_unpacklo_epi32(T2, T3);
                __m128i T18 = _mm_unpackhi_epi32(T0, T1);
                __m128i T19 = _mm_unpackhi_epi32(T2, T3);

                __m128i T20 = _mm_unpacklo_epi64(T16, T17);
                __m128i T21 = _mm_unpackhi_epi64(T16, T17);
                __m128i T22 = _mm_unpacklo_epi64(T18, T19);
                __m128i T23 = _mm_unpackhi_epi64(T18, T19);

                T20 = _mm_add_epi32(T20,T21);
                T20 = _mm_add_epi32(T20,T22);
                T20 = _mm_add_epi32(T20,T23);
                des0 = _mm_add_epi32(T20,des0);

                T16 = _mm_unpacklo_epi32(T4, T5);
                T17 = _mm_unpacklo_epi32(T6, T7);
                T18 = _mm_unpackhi_epi32(T4, T5);
                T19 = _mm_unpackhi_epi32(T6, T7);

                T20 = _mm_unpacklo_epi64(T16, T17);
                T21 = _mm_unpackhi_epi64(T16, T17);
                T22 = _mm_unpacklo_epi64(T18, T19);
                T23 = _mm_unpackhi_epi64(T18, T19);

                T20 = _mm_add_epi32(T20,T21);
                T20 = _mm_add_epi32(T20,T22);
                T20 = _mm_add_epi32(T20,T23);
                des1 = _mm_add_epi32(T20,des1);

                T16 = _mm_unpacklo_epi32(T8, T9);
                T17 = _mm_unpacklo_epi32(T10, T11);
                T18 = _mm_unpackhi_epi32(T8, T9);
                T19 = _mm_unpackhi_epi32(T10, T11);

                T20 = _mm_unpacklo_epi64(T16, T17);
                T21 = _mm_unpackhi_epi64(T16, T17);
                T22 = _mm_unpacklo_epi64(T18, T19);
                T23 = _mm_unpackhi_epi64(T18, T19);

                T20 = _mm_add_epi32(T20,T21);
                T20 = _mm_add_epi32(T20,T22);
                T20 = _mm_add_epi32(T20,T23);
                des2 = _mm_add_epi32(T20,des2);

                T16 = _mm_unpacklo_epi32(T12, T13);
                T17 = _mm_unpacklo_epi32(T14, T15);
                T18 = _mm_unpackhi_epi32(T12, T13);
                T19 = _mm_unpackhi_epi32(T14, T15);

                T20 = _mm_unpacklo_epi64(T16, T17);
                T21 = _mm_unpackhi_epi64(T16, T17);
                T22 = _mm_unpacklo_epi64(T18, T19);
                T23 = _mm_unpackhi_epi64(T18, T19);

                T20 = _mm_add_epi32(T20,T21);
                T20 = _mm_add_epi32(T20,T22);
                T20 = _mm_add_epi32(T20,T23);
                des3 = _mm_add_epi32(T20,des3);
            }
            /* store to dst */
            _mm_storeu_si128((__m128i *)(dst + ((i + 0) * src2_w) + j), des0);
            _mm_storeu_si128((__m128i *)(dst + ((i + 1) * src2_w) + j), des1);
            _mm_storeu_si128((__m128i *)(dst + ((i + 2) * src2_w) + j), des2);
            _mm_storeu_si128((__m128i *)(dst + ((i + 3) * src2_w) + j), des3);
        }
    }
}

void avx_multiply(int *src1, int *src2, int *dst, int src1_w, int src1_h,
                  int src2_w, int src2_h)
{
    for (int i = 0; i < src1_h; i += 8) {
        for (int j = 0; j < src2_w; j += 8) {
            __m256 des0, des1, des2, des3, des4, des5, des6, des7;
            for (int k = 0; k < src2_h; k += 8) {
                /* laod eight rows from src2 */
                __m256 ymm0 = _mm256_loadu_ps((float *) (src2 + (k + 0) * src2_w + j));
                __m256 ymm1 = _mm256_loadu_ps((float *) (src2 + (k + 1) * src2_w + j));
                __m256 ymm2 = _mm256_loadu_ps((float *) (src2 + (k + 2) * src2_w + j));
                __m256 ymm3 = _mm256_loadu_ps((float *) (src2 + (k + 3) * src2_w + j));
                __m256 ymm4 = _mm256_loadu_ps((float *) (src2 + (k + 4) * src2_w + j));
                __m256 ymm5 = _mm256_loadu_ps((float *) (src2 + (k + 5) * src2_w + j));
                __m256 ymm6 = _mm256_loadu_ps((float *) (src2 + (k + 6) * src2_w + j));
                __m256 ymm7 = _mm256_loadu_ps((float *) (src2 + (k + 7) * src2_w + j));

                /* set each element from src1*/
                __m256 ymm8 = _mm256_set1_ps(src1[(i + 0) * src1_w + k + 0]);
                __m256 ymm9 = _mm256_set1_ps(src1[(i + 0) * src1_w + k + 1]);
                __m256 ymm10 = _mm256_set1_ps(src1[(i + 0) * src1_w + k + 2]);
                __m256 ymm11 = _mm256_set1_ps(src1[(i + 0) * src1_w + k + 3]);
                __m256 ymm12 = _mm256_set1_ps(src1[(i + 0) * src1_w + k + 4]);
                __m256 ymm13 = _mm256_set1_ps(src1[(i + 0) * src1_w + k + 5]);
                __m256 ymm14 = _mm256_set1_ps(src1[(i + 0) * src1_w + k + 6]);
                __m256 ymm15 = _mm256_set1_ps(src1[(i + 0) * src1_w + k + 7]);

                /* vector multiply */
                /* row 1 2 */
                ymm8 = _mm256_mul_ps(ymm8, ymm0);
                ymm9 = _mm256_mul_ps(ymm9, ymm1);
                ymm8 = _mm256_add_ps(ymm8, ymm9);
                /* row 3 4 */
                ymm10 = _mm256_mul_ps(ymm10, ymm2);
                ymm11 = _mm256_mul_ps(ymm11, ymm3);
                ymm10 = _mm256_add_ps(ymm10, ymm11);
                /* row 5 6 */
                ymm12 = _mm256_mul_ps(ymm12, ymm4);
                ymm13 = _mm256_mul_ps(ymm13, ymm5);
                ymm12 = _mm256_add_ps(ymm12, ymm13);
                /* row 7 8 */
                ymm14 = _mm256_mul_ps(ymm14, ymm6);
                ymm15 = _mm256_mul_ps(ymm15, ymm7);
                ymm14 = _mm256_add_ps(ymm14, ymm15);
                /* sum */
                ymm8 = _mm256_add_ps(ymm8, ymm10);
                ymm12 = _mm256_add_ps(ymm12, ymm14);
                ymm8 = _mm256_add_ps(ymm8, ymm12);

                des0 = _mm256_add_ps(des0, ymm8);
                //------------------------------------------------------------------------------//
                /* set each element from src1*/
                ymm8 = _mm256_set1_ps(src1[(i + 1) * src1_w + k + 0]);
                ymm9 = _mm256_set1_ps(src1[(i + 1) * src1_w + k + 1]);
                ymm10 = _mm256_set1_ps(src1[(i + 1) * src1_w + k + 2]);
                ymm11 = _mm256_set1_ps(src1[(i + 1) * src1_w + k + 3]);
                ymm12 = _mm256_set1_ps(src1[(i + 1) * src1_w + k + 4]);
                ymm13 = _mm256_set1_ps(src1[(i + 1) * src1_w + k + 5]);
                ymm14 = _mm256_set1_ps(src1[(i + 1) * src1_w + k + 6]);
                ymm15 = _mm256_set1_ps(src1[(i + 1) * src1_w + k + 7]);

                /* vector multiply */
                /* row 1 2 */
                ymm8 = _mm256_mul_ps(ymm8, ymm0);
                ymm9 = _mm256_mul_ps(ymm9, ymm1);
                ymm8 = _mm256_add_ps(ymm8, ymm9);
                /* row 3 4 */
                ymm10 = _mm256_mul_ps(ymm10, ymm2);
                ymm11 = _mm256_mul_ps(ymm11, ymm3);
                ymm10 = _mm256_add_ps(ymm10, ymm11);
                /* row 5 6 */
                ymm12 = _mm256_mul_ps(ymm12, ymm4);
                ymm13 = _mm256_mul_ps(ymm13, ymm5);
                ymm12 = _mm256_add_ps(ymm12, ymm13);
                /* row 7 8 */
                ymm14 = _mm256_mul_ps(ymm14, ymm6);
                ymm15 = _mm256_mul_ps(ymm15, ymm7);
                ymm14 = _mm256_add_ps(ymm14, ymm15);
                /* sum */
                ymm8 = _mm256_add_ps(ymm8, ymm10);
                ymm12 = _mm256_add_ps(ymm12, ymm14);
                ymm8 = _mm256_add_ps(ymm8, ymm12);

                des1 = _mm256_add_ps(des1, ymm8);
                //------------------------------------------------------------------------------//
                /* set each element from src1*/
                ymm8 = _mm256_set1_ps(src1[(i + 2) * src1_w + k + 0]);
                ymm9 = _mm256_set1_ps(src1[(i + 2) * src1_w + k + 1]);
                ymm10 = _mm256_set1_ps(src1[(i + 2) * src1_w + k + 2]);
                ymm11 = _mm256_set1_ps(src1[(i + 2) * src1_w + k + 3]);
                ymm12 = _mm256_set1_ps(src1[(i + 2) * src1_w + k + 4]);
                ymm13 = _mm256_set1_ps(src1[(i + 2) * src1_w + k + 5]);
                ymm14 = _mm256_set1_ps(src1[(i + 2) * src1_w + k + 6]);
                ymm15 = _mm256_set1_ps(src1[(i + 2) * src1_w + k + 7]);

                /* vector multiply */
                /* row 1 2 */
                ymm8 = _mm256_mul_ps(ymm8, ymm0);
                ymm9 = _mm256_mul_ps(ymm9, ymm1);
                ymm8 = _mm256_add_ps(ymm8, ymm9);
                /* row 3 4 */
                ymm10 = _mm256_mul_ps(ymm10, ymm2);
                ymm11 = _mm256_mul_ps(ymm11, ymm3);
                ymm10 = _mm256_add_ps(ymm10, ymm11);
                /* row 5 6 */
                ymm12 = _mm256_mul_ps(ymm12, ymm4);
                ymm13 = _mm256_mul_ps(ymm13, ymm5);
                ymm12 = _mm256_add_ps(ymm12, ymm13);
                /* row 7 8 */
                ymm14 = _mm256_mul_ps(ymm14, ymm6);
                ymm15 = _mm256_mul_ps(ymm15, ymm7);
                ymm14 = _mm256_add_ps(ymm14, ymm15);
                /* sum */
                ymm8 = _mm256_add_ps(ymm8, ymm10);
                ymm12 = _mm256_add_ps(ymm12, ymm14);
                ymm8 = _mm256_add_ps(ymm8, ymm12);

                des2 = _mm256_add_ps(des2, ymm8);
                //------------------------------------------------------------------------------//
                /* set each element from src1*/
                ymm8 = _mm256_set1_ps(src1[(i + 3) * src1_w + k + 0]);
                ymm9 = _mm256_set1_ps(src1[(i + 3) * src1_w + k + 1]);
                ymm10 = _mm256_set1_ps(src1[(i + 3) * src1_w + k + 2]);
                ymm11 = _mm256_set1_ps(src1[(i + 3 ) * src1_w + k + 3]);
                ymm12 = _mm256_set1_ps(src1[(i + 3) * src1_w + k + 4]);
                ymm13 = _mm256_set1_ps(src1[(i + 3) * src1_w + k + 5]);
                ymm14 = _mm256_set1_ps(src1[(i + 3) * src1_w + k + 6]);
                ymm15 = _mm256_set1_ps(src1[(i + 3) * src1_w + k + 7]);

                /* vector multiply */
                /* row 1 2 */
                ymm8 = _mm256_mul_ps(ymm8, ymm0);
                ymm9 = _mm256_mul_ps(ymm9, ymm1);
                ymm8 = _mm256_add_ps(ymm8, ymm9);
                /* row 3 4 */
                ymm10 = _mm256_mul_ps(ymm10, ymm2);
                ymm11 = _mm256_mul_ps(ymm11, ymm3);
                ymm10 = _mm256_add_ps(ymm10, ymm11);
                /* row 5 6 */
                ymm12 = _mm256_mul_ps(ymm12, ymm4);
                ymm13 = _mm256_mul_ps(ymm13, ymm5);
                ymm12 = _mm256_add_ps(ymm12, ymm13);
                /* row 7 8 */
                ymm14 = _mm256_mul_ps(ymm14, ymm6);
                ymm15 = _mm256_mul_ps(ymm15, ymm7);
                ymm14 = _mm256_add_ps(ymm14, ymm15);
                /* sum */
                ymm8 = _mm256_add_ps(ymm8, ymm10);
                ymm12 = _mm256_add_ps(ymm12, ymm14);
                ymm8 = _mm256_add_ps(ymm8, ymm12);

                des3 = _mm256_add_ps(des3, ymm8);
                //------------------------------------------------------------------------------//
                /* set each element from src1*/
                ymm8 = _mm256_set1_ps(src1[(i + 4) * src1_w + k + 0]);
                ymm9 = _mm256_set1_ps(src1[(i + 4) * src1_w + k + 1]);
                ymm10 = _mm256_set1_ps(src1[(i + 4) * src1_w + k + 2]);
                ymm11 = _mm256_set1_ps(src1[(i + 4) * src1_w + k + 3]);
                ymm12 = _mm256_set1_ps(src1[(i + 4) * src1_w + k + 4]);
                ymm13 = _mm256_set1_ps(src1[(i + 4) * src1_w + k + 5]);
                ymm14 = _mm256_set1_ps(src1[(i + 4) * src1_w + k + 6]);
                ymm15 = _mm256_set1_ps(src1[(i + 4) * src1_w + k + 7]);

                /* vector multiply */
                /* row 1 2 */
                ymm8 = _mm256_mul_ps(ymm8, ymm0);
                ymm9 = _mm256_mul_ps(ymm9, ymm1);
                ymm8 = _mm256_add_ps(ymm8, ymm9);
                /* row 3 4 */
                ymm10 = _mm256_mul_ps(ymm10, ymm2);
                ymm11 = _mm256_mul_ps(ymm11, ymm3);
                ymm10 = _mm256_add_ps(ymm10, ymm11);
                /* row 5 6 */
                ymm12 = _mm256_mul_ps(ymm12, ymm4);
                ymm13 = _mm256_mul_ps(ymm13, ymm5);
                ymm12 = _mm256_add_ps(ymm12, ymm13);
                /* row 7 8 */
                ymm14 = _mm256_mul_ps(ymm14, ymm6);
                ymm15 = _mm256_mul_ps(ymm15, ymm7);
                ymm14 = _mm256_add_ps(ymm14, ymm15);
                /* sum */
                ymm8 = _mm256_add_ps(ymm8, ymm10);
                ymm12 = _mm256_add_ps(ymm12, ymm14);
                ymm8 = _mm256_add_ps(ymm8, ymm12);

                des4 = _mm256_add_ps(des4, ymm8);
                //------------------------------------------------------------------------------//
                /* set each element from src1*/
                ymm8 = _mm256_set1_ps(src1[(i + 5) * src1_w + k + 0]);
                ymm9 = _mm256_set1_ps(src1[(i + 5) * src1_w + k + 1]);
                ymm10 = _mm256_set1_ps(src1[(i + 5) * src1_w + k + 2]);
                ymm11 = _mm256_set1_ps(src1[(i + 5) * src1_w + k + 3]);
                ymm12 = _mm256_set1_ps(src1[(i + 5) * src1_w + k + 4]);
                ymm13 = _mm256_set1_ps(src1[(i + 5) * src1_w + k + 5]);
                ymm14 = _mm256_set1_ps(src1[(i + 5) * src1_w + k + 6]);
                ymm15 = _mm256_set1_ps(src1[(i + 5) * src1_w + k + 7]);

                /* vector multiply */
                /* row 1 2 */
                ymm8 = _mm256_mul_ps(ymm8, ymm0);
                ymm9 = _mm256_mul_ps(ymm9, ymm1);
                ymm8 = _mm256_add_ps(ymm8, ymm9);
                /* row 3 4 */
                ymm10 = _mm256_mul_ps(ymm10, ymm2);
                ymm11 = _mm256_mul_ps(ymm11, ymm3);
                ymm10 = _mm256_add_ps(ymm10, ymm11);
                /* row 5 6 */
                ymm12 = _mm256_mul_ps(ymm12, ymm4);
                ymm13 = _mm256_mul_ps(ymm13, ymm5);
                ymm12 = _mm256_add_ps(ymm12, ymm13);
                /* row 7 8 */
                ymm14 = _mm256_mul_ps(ymm14, ymm6);
                ymm15 = _mm256_mul_ps(ymm15, ymm7);
                ymm14 = _mm256_add_ps(ymm14, ymm15);
                /* sum */
                ymm8 = _mm256_add_ps(ymm8, ymm10);
                ymm12 = _mm256_add_ps(ymm12, ymm14);
                ymm8 = _mm256_add_ps(ymm8, ymm12);

                des5 = _mm256_add_ps(des5, ymm8);
                //------------------------------------------------------------------------------//
                /* set each element from src1*/
                ymm8 = _mm256_set1_ps(src1[(i + 6) * src1_w + k + 0]);
                ymm9 = _mm256_set1_ps(src1[(i + 6) * src1_w + k + 1]);
                ymm10 = _mm256_set1_ps(src1[(i + 6) * src1_w + k + 2]);
                ymm11 = _mm256_set1_ps(src1[(i + 6) * src1_w + k + 3]);
                ymm12 = _mm256_set1_ps(src1[(i + 6) * src1_w + k + 4]);
                ymm13 = _mm256_set1_ps(src1[(i + 6) * src1_w + k + 5]);
                ymm14 = _mm256_set1_ps(src1[(i + 6) * src1_w + k + 6]);
                ymm15 = _mm256_set1_ps(src1[(i + 6) * src1_w + k + 7]);

                /* vector multiply */
                /* row 1 2 */
                ymm8 = _mm256_mul_ps(ymm8, ymm0);
                ymm9 = _mm256_mul_ps(ymm9, ymm1);
                ymm8 = _mm256_add_ps(ymm8, ymm9);
                /* row 3 4 */
                ymm10 = _mm256_mul_ps(ymm10, ymm2);
                ymm11 = _mm256_mul_ps(ymm11, ymm3);
                ymm10 = _mm256_add_ps(ymm10, ymm11);
                /* row 5 6 */
                ymm12 = _mm256_mul_ps(ymm12, ymm4);
                ymm13 = _mm256_mul_ps(ymm13, ymm5);
                ymm12 = _mm256_add_ps(ymm12, ymm13);
                /* row 7 8 */
                ymm14 = _mm256_mul_ps(ymm14, ymm6);
                ymm15 = _mm256_mul_ps(ymm15, ymm7);
                ymm14 = _mm256_add_ps(ymm14, ymm15);
                /* sum */
                ymm8 = _mm256_add_ps(ymm8, ymm10);
                ymm12 = _mm256_add_ps(ymm12, ymm14);
                ymm8 = _mm256_add_ps(ymm8, ymm12);

                des6 = _mm256_add_ps(des6, ymm8);
                //------------------------------------------------------------------------------//
                /* set each element from src1*/
                ymm8 = _mm256_set1_ps(src1[(i + 7) * src1_w + k + 0]);
                ymm9 = _mm256_set1_ps(src1[(i + 7) * src1_w + k + 1]);
                ymm10 = _mm256_set1_ps(src1[(i + 7) * src1_w + k + 2]);
                ymm11 = _mm256_set1_ps(src1[(i + 7) * src1_w + k + 3]);
                ymm12 = _mm256_set1_ps(src1[(i + 7) * src1_w + k + 4]);
                ymm13 = _mm256_set1_ps(src1[(i + 7) * src1_w + k + 5]);
                ymm14 = _mm256_set1_ps(src1[(i + 7) * src1_w + k + 6]);
                ymm15 = _mm256_set1_ps(src1[(i + 7) * src1_w + k + 7]);

                /* vector multiply */
                /* row 1 2 */
                ymm8 = _mm256_mul_ps(ymm8, ymm0);
                ymm9 = _mm256_mul_ps(ymm9, ymm1);
                ymm8 = _mm256_add_ps(ymm8, ymm9);
                /* row 3 4 */
                ymm10 = _mm256_mul_ps(ymm10, ymm2);
                ymm11 = _mm256_mul_ps(ymm11, ymm3);
                ymm10 = _mm256_add_ps(ymm10, ymm11);
                /* row 5 6 */
                ymm12 = _mm256_mul_ps(ymm12, ymm4);
                ymm13 = _mm256_mul_ps(ymm13, ymm5);
                ymm12 = _mm256_add_ps(ymm12, ymm13);
                /* row 7 8 */
                ymm14 = _mm256_mul_ps(ymm14, ymm6);
                ymm15 = _mm256_mul_ps(ymm15, ymm7);
                ymm14 = _mm256_add_ps(ymm14, ymm15);
                /* sum */
                ymm8 = _mm256_add_ps(ymm8, ymm10);
                ymm12 = _mm256_add_ps(ymm12, ymm14);
                ymm8 = _mm256_add_ps(ymm8, ymm12);

                des7 = _mm256_add_ps(des7, ymm8);

            }
            /* store dst */
            _mm256_storeu_ps((float *) (dst + (i + 0) * src2_w + j), des0);
            _mm256_storeu_ps((float *) (dst + (i + 1) * src2_w + j), des1);
            _mm256_storeu_ps((float *) (dst + (i + 2) * src2_w + j), des2);
            _mm256_storeu_ps((float *) (dst + (i + 3) * src2_w + j), des3);
            _mm256_storeu_ps((float *) (dst + (i + 4) * src2_w + j), des4);
            _mm256_storeu_ps((float *) (dst + (i + 5) * src2_w + j), des5);
            _mm256_storeu_ps((float *) (dst + (i + 6) * src2_w + j), des6);
            _mm256_storeu_ps((float *) (dst + (i + 7) * src2_w + j), des7);
        }
    }
}

void avx_multiply_prefetch(int *src1, int *src2, int *dst, int src1_w, int src1_h,
                           int src2_w, int src2_h)
{
    for (int i = 0; i < src1_h; i += 8) {
        for (int j = 0; j < src2_w; j += 8) {
            __m256 des0, des1, des2, des3, des4, des5, des6, des7;
            for (int k = 0; k < src2_h; k += 8) {
#define AVX_PFDIST  8
                _mm_prefetch(src2 + (k + AVX_PFDIST + 0) * src2_w + j, _MM_HINT_T1);
                _mm_prefetch(src2 + (k + AVX_PFDIST + 1) * src2_w + j, _MM_HINT_T1);
                _mm_prefetch(src2 + (k + AVX_PFDIST + 2) * src2_w + j, _MM_HINT_T1);
                _mm_prefetch(src2 + (k + AVX_PFDIST + 3) * src2_w + j, _MM_HINT_T1);
                _mm_prefetch(src2 + (k + AVX_PFDIST + 4) * src2_w + j, _MM_HINT_T1);
                _mm_prefetch(src2 + (k + AVX_PFDIST + 5) * src2_w + j, _MM_HINT_T1);
                _mm_prefetch(src2 + (k + AVX_PFDIST + 6) * src2_w + j, _MM_HINT_T1);
                _mm_prefetch(src2 + (k + AVX_PFDIST + 7) * src2_w + j, _MM_HINT_T1);

                /* laod eight rows from src2 */
                __m256 ymm0 = _mm256_loadu_ps((float *) (src2 + (k + 0) * src2_w + j));
                __m256 ymm1 = _mm256_loadu_ps((float *) (src2 + (k + 1) * src2_w + j));
                __m256 ymm2 = _mm256_loadu_ps((float *) (src2 + (k + 2) * src2_w + j));
                __m256 ymm3 = _mm256_loadu_ps((float *) (src2 + (k + 3) * src2_w + j));
                __m256 ymm4 = _mm256_loadu_ps((float *) (src2 + (k + 4) * src2_w + j));
                __m256 ymm5 = _mm256_loadu_ps((float *) (src2 + (k + 5) * src2_w + j));
                __m256 ymm6 = _mm256_loadu_ps((float *) (src2 + (k + 6) * src2_w + j));
                __m256 ymm7 = _mm256_loadu_ps((float *) (src2 + (k + 7) * src2_w + j));

                /* set each element from src1*/
                __m256 ymm8 = _mm256_set1_ps(src1[(i + 0) * src1_w + k + 0]);
                __m256 ymm9 = _mm256_set1_ps(src1[(i + 0) * src1_w + k + 1]);
                __m256 ymm10 = _mm256_set1_ps(src1[(i + 0) * src1_w + k + 2]);
                __m256 ymm11 = _mm256_set1_ps(src1[(i + 0) * src1_w + k + 3]);
                __m256 ymm12 = _mm256_set1_ps(src1[(i + 0) * src1_w + k + 4]);
                __m256 ymm13 = _mm256_set1_ps(src1[(i + 0) * src1_w + k + 5]);
                __m256 ymm14 = _mm256_set1_ps(src1[(i + 0) * src1_w + k + 6]);
                __m256 ymm15 = _mm256_set1_ps(src1[(i + 0) * src1_w + k + 7]);

                /* vector multiply */
                /* row 1 2 */
                ymm8 = _mm256_mul_ps(ymm8, ymm0);
                ymm9 = _mm256_mul_ps(ymm9, ymm1);
                ymm8 = _mm256_add_ps(ymm8, ymm9);
                /* row 3 4 */
                ymm10 = _mm256_mul_ps(ymm10, ymm2);
                ymm11 = _mm256_mul_ps(ymm11, ymm3);
                ymm10 = _mm256_add_ps(ymm10, ymm11);
                /* row 5 6 */
                ymm12 = _mm256_mul_ps(ymm12, ymm4);
                ymm13 = _mm256_mul_ps(ymm13, ymm5);
                ymm12 = _mm256_add_ps(ymm12, ymm13);
                /* row 7 8 */
                ymm14 = _mm256_mul_ps(ymm14, ymm6);
                ymm15 = _mm256_mul_ps(ymm15, ymm7);
                ymm14 = _mm256_add_ps(ymm14, ymm15);
                /* sum */
                ymm8 = _mm256_add_ps(ymm8, ymm10);
                ymm12 = _mm256_add_ps(ymm12, ymm14);
                ymm8 = _mm256_add_ps(ymm8, ymm12);

                des0 = _mm256_add_ps(des0, ymm8);
                //------------------------------------------------------------------------------//
                /* set each element from src1*/
                ymm8 = _mm256_set1_ps(src1[(i + 1) * src1_w + k + 0]);
                ymm9 = _mm256_set1_ps(src1[(i + 1) * src1_w + k + 1]);
                ymm10 = _mm256_set1_ps(src1[(i + 1) * src1_w + k + 2]);
                ymm11 = _mm256_set1_ps(src1[(i + 1) * src1_w + k + 3]);
                ymm12 = _mm256_set1_ps(src1[(i + 1) * src1_w + k + 4]);
                ymm13 = _mm256_set1_ps(src1[(i + 1) * src1_w + k + 5]);
                ymm14 = _mm256_set1_ps(src1[(i + 1) * src1_w + k + 6]);
                ymm15 = _mm256_set1_ps(src1[(i + 1) * src1_w + k + 7]);

                /* vector multiply */
                /* row 1 2 */
                ymm8 = _mm256_mul_ps(ymm8, ymm0);
                ymm9 = _mm256_mul_ps(ymm9, ymm1);
                ymm8 = _mm256_add_ps(ymm8, ymm9);
                /* row 3 4 */
                ymm10 = _mm256_mul_ps(ymm10, ymm2);
                ymm11 = _mm256_mul_ps(ymm11, ymm3);
                ymm10 = _mm256_add_ps(ymm10, ymm11);
                /* row 5 6 */
                ymm12 = _mm256_mul_ps(ymm12, ymm4);
                ymm13 = _mm256_mul_ps(ymm13, ymm5);
                ymm12 = _mm256_add_ps(ymm12, ymm13);
                /* row 7 8 */
                ymm14 = _mm256_mul_ps(ymm14, ymm6);
                ymm15 = _mm256_mul_ps(ymm15, ymm7);
                ymm14 = _mm256_add_ps(ymm14, ymm15);
                /* sum */
                ymm8 = _mm256_add_ps(ymm8, ymm10);
                ymm12 = _mm256_add_ps(ymm12, ymm14);
                ymm8 = _mm256_add_ps(ymm8, ymm12);

                des1 = _mm256_add_ps(des1, ymm8);
                //------------------------------------------------------------------------------//
                /* set each element from src1*/
                ymm8 = _mm256_set1_ps(src1[(i + 2) * src1_w + k + 0]);
                ymm9 = _mm256_set1_ps(src1[(i + 2) * src1_w + k + 1]);
                ymm10 = _mm256_set1_ps(src1[(i + 2) * src1_w + k + 2]);
                ymm11 = _mm256_set1_ps(src1[(i + 2) * src1_w + k + 3]);
                ymm12 = _mm256_set1_ps(src1[(i + 2) * src1_w + k + 4]);
                ymm13 = _mm256_set1_ps(src1[(i + 2) * src1_w + k + 5]);
                ymm14 = _mm256_set1_ps(src1[(i + 2) * src1_w + k + 6]);
                ymm15 = _mm256_set1_ps(src1[(i + 2) * src1_w + k + 7]);

                /* vector multiply */
                /* row 1 2 */
                ymm8 = _mm256_mul_ps(ymm8, ymm0);
                ymm9 = _mm256_mul_ps(ymm9, ymm1);
                ymm8 = _mm256_add_ps(ymm8, ymm9);
                /* row 3 4 */
                ymm10 = _mm256_mul_ps(ymm10, ymm2);
                ymm11 = _mm256_mul_ps(ymm11, ymm3);
                ymm10 = _mm256_add_ps(ymm10, ymm11);
                /* row 5 6 */
                ymm12 = _mm256_mul_ps(ymm12, ymm4);
                ymm13 = _mm256_mul_ps(ymm13, ymm5);
                ymm12 = _mm256_add_ps(ymm12, ymm13);
                /* row 7 8 */
                ymm14 = _mm256_mul_ps(ymm14, ymm6);
                ymm15 = _mm256_mul_ps(ymm15, ymm7);
                ymm14 = _mm256_add_ps(ymm14, ymm15);
                /* sum */
                ymm8 = _mm256_add_ps(ymm8, ymm10);
                ymm12 = _mm256_add_ps(ymm12, ymm14);
                ymm8 = _mm256_add_ps(ymm8, ymm12);

                des2 = _mm256_add_ps(des2, ymm8);
                //------------------------------------------------------------------------------//
                /* set each element from src1*/
                ymm8 = _mm256_set1_ps(src1[(i + 3) * src1_w + k + 0]);
                ymm9 = _mm256_set1_ps(src1[(i + 3) * src1_w + k + 1]);
                ymm10 = _mm256_set1_ps(src1[(i + 3) * src1_w + k + 2]);
                ymm11 = _mm256_set1_ps(src1[(i + 3 ) * src1_w + k + 3]);
                ymm12 = _mm256_set1_ps(src1[(i + 3) * src1_w + k + 4]);
                ymm13 = _mm256_set1_ps(src1[(i + 3) * src1_w + k + 5]);
                ymm14 = _mm256_set1_ps(src1[(i + 3) * src1_w + k + 6]);
                ymm15 = _mm256_set1_ps(src1[(i + 3) * src1_w + k + 7]);

                /* vector multiply */
                /* row 1 2 */
                ymm8 = _mm256_mul_ps(ymm8, ymm0);
                ymm9 = _mm256_mul_ps(ymm9, ymm1);
                ymm8 = _mm256_add_ps(ymm8, ymm9);
                /* row 3 4 */
                ymm10 = _mm256_mul_ps(ymm10, ymm2);
                ymm11 = _mm256_mul_ps(ymm11, ymm3);
                ymm10 = _mm256_add_ps(ymm10, ymm11);
                /* row 5 6 */
                ymm12 = _mm256_mul_ps(ymm12, ymm4);
                ymm13 = _mm256_mul_ps(ymm13, ymm5);
                ymm12 = _mm256_add_ps(ymm12, ymm13);
                /* row 7 8 */
                ymm14 = _mm256_mul_ps(ymm14, ymm6);
                ymm15 = _mm256_mul_ps(ymm15, ymm7);
                ymm14 = _mm256_add_ps(ymm14, ymm15);
                /* sum */
                ymm8 = _mm256_add_ps(ymm8, ymm10);
                ymm12 = _mm256_add_ps(ymm12, ymm14);
                ymm8 = _mm256_add_ps(ymm8, ymm12);

                des3 = _mm256_add_ps(des3, ymm8);
                //------------------------------------------------------------------------------//
                /* set each element from src1*/
                ymm8 = _mm256_set1_ps(src1[(i + 4) * src1_w + k + 0]);
                ymm9 = _mm256_set1_ps(src1[(i + 4) * src1_w + k + 1]);
                ymm10 = _mm256_set1_ps(src1[(i + 4) * src1_w + k + 2]);
                ymm11 = _mm256_set1_ps(src1[(i + 4) * src1_w + k + 3]);
                ymm12 = _mm256_set1_ps(src1[(i + 4) * src1_w + k + 4]);
                ymm13 = _mm256_set1_ps(src1[(i + 4) * src1_w + k + 5]);
                ymm14 = _mm256_set1_ps(src1[(i + 4) * src1_w + k + 6]);
                ymm15 = _mm256_set1_ps(src1[(i + 4) * src1_w + k + 7]);

                /* vector multiply */
                /* row 1 2 */
                ymm8 = _mm256_mul_ps(ymm8, ymm0);
                ymm9 = _mm256_mul_ps(ymm9, ymm1);
                ymm8 = _mm256_add_ps(ymm8, ymm9);
                /* row 3 4 */
                ymm10 = _mm256_mul_ps(ymm10, ymm2);
                ymm11 = _mm256_mul_ps(ymm11, ymm3);
                ymm10 = _mm256_add_ps(ymm10, ymm11);
                /* row 5 6 */
                ymm12 = _mm256_mul_ps(ymm12, ymm4);
                ymm13 = _mm256_mul_ps(ymm13, ymm5);
                ymm12 = _mm256_add_ps(ymm12, ymm13);
                /* row 7 8 */
                ymm14 = _mm256_mul_ps(ymm14, ymm6);
                ymm15 = _mm256_mul_ps(ymm15, ymm7);
                ymm14 = _mm256_add_ps(ymm14, ymm15);
                /* sum */
                ymm8 = _mm256_add_ps(ymm8, ymm10);
                ymm12 = _mm256_add_ps(ymm12, ymm14);
                ymm8 = _mm256_add_ps(ymm8, ymm12);

                des4 = _mm256_add_ps(des4, ymm8);
                //------------------------------------------------------------------------------//
                /* set each element from src1*/
                ymm8 = _mm256_set1_ps(src1[(i + 5) * src1_w + k + 0]);
                ymm9 = _mm256_set1_ps(src1[(i + 5) * src1_w + k + 1]);
                ymm10 = _mm256_set1_ps(src1[(i + 5) * src1_w + k + 2]);
                ymm11 = _mm256_set1_ps(src1[(i + 5) * src1_w + k + 3]);
                ymm12 = _mm256_set1_ps(src1[(i + 5) * src1_w + k + 4]);
                ymm13 = _mm256_set1_ps(src1[(i + 5) * src1_w + k + 5]);
                ymm14 = _mm256_set1_ps(src1[(i + 5) * src1_w + k + 6]);
                ymm15 = _mm256_set1_ps(src1[(i + 5) * src1_w + k + 7]);

                /* vector multiply */
                /* row 1 2 */
                ymm8 = _mm256_mul_ps(ymm8, ymm0);
                ymm9 = _mm256_mul_ps(ymm9, ymm1);
                ymm8 = _mm256_add_ps(ymm8, ymm9);
                /* row 3 4 */
                ymm10 = _mm256_mul_ps(ymm10, ymm2);
                ymm11 = _mm256_mul_ps(ymm11, ymm3);
                ymm10 = _mm256_add_ps(ymm10, ymm11);
                /* row 5 6 */
                ymm12 = _mm256_mul_ps(ymm12, ymm4);
                ymm13 = _mm256_mul_ps(ymm13, ymm5);
                ymm12 = _mm256_add_ps(ymm12, ymm13);
                /* row 7 8 */
                ymm14 = _mm256_mul_ps(ymm14, ymm6);
                ymm15 = _mm256_mul_ps(ymm15, ymm7);
                ymm14 = _mm256_add_ps(ymm14, ymm15);
                /* sum */
                ymm8 = _mm256_add_ps(ymm8, ymm10);
                ymm12 = _mm256_add_ps(ymm12, ymm14);
                ymm8 = _mm256_add_ps(ymm8, ymm12);

                des5 = _mm256_add_ps(des5, ymm8);
                //------------------------------------------------------------------------------//
                /* set each element from src1*/
                ymm8 = _mm256_set1_ps(src1[(i + 6) * src1_w + k + 0]);
                ymm9 = _mm256_set1_ps(src1[(i + 6) * src1_w + k + 1]);
                ymm10 = _mm256_set1_ps(src1[(i + 6) * src1_w + k + 2]);
                ymm11 = _mm256_set1_ps(src1[(i + 6) * src1_w + k + 3]);
                ymm12 = _mm256_set1_ps(src1[(i + 6) * src1_w + k + 4]);
                ymm13 = _mm256_set1_ps(src1[(i + 6) * src1_w + k + 5]);
                ymm14 = _mm256_set1_ps(src1[(i + 6) * src1_w + k + 6]);
                ymm15 = _mm256_set1_ps(src1[(i + 6) * src1_w + k + 7]);

                /* vector multiply */
                /* row 1 2 */
                ymm8 = _mm256_mul_ps(ymm8, ymm0);
                ymm9 = _mm256_mul_ps(ymm9, ymm1);
                ymm8 = _mm256_add_ps(ymm8, ymm9);
                /* row 3 4 */
                ymm10 = _mm256_mul_ps(ymm10, ymm2);
                ymm11 = _mm256_mul_ps(ymm11, ymm3);
                ymm10 = _mm256_add_ps(ymm10, ymm11);
                /* row 5 6 */
                ymm12 = _mm256_mul_ps(ymm12, ymm4);
                ymm13 = _mm256_mul_ps(ymm13, ymm5);
                ymm12 = _mm256_add_ps(ymm12, ymm13);
                /* row 7 8 */
                ymm14 = _mm256_mul_ps(ymm14, ymm6);
                ymm15 = _mm256_mul_ps(ymm15, ymm7);
                ymm14 = _mm256_add_ps(ymm14, ymm15);
                /* sum */
                ymm8 = _mm256_add_ps(ymm8, ymm10);
                ymm12 = _mm256_add_ps(ymm12, ymm14);
                ymm8 = _mm256_add_ps(ymm8, ymm12);

                des6 = _mm256_add_ps(des6, ymm8);
                //------------------------------------------------------------------------------//
                /* set each element from src1*/
                ymm8 = _mm256_set1_ps(src1[(i + 7) * src1_w + k + 0]);
                ymm9 = _mm256_set1_ps(src1[(i + 7) * src1_w + k + 1]);
                ymm10 = _mm256_set1_ps(src1[(i + 7) * src1_w + k + 2]);
                ymm11 = _mm256_set1_ps(src1[(i + 7) * src1_w + k + 3]);
                ymm12 = _mm256_set1_ps(src1[(i + 7) * src1_w + k + 4]);
                ymm13 = _mm256_set1_ps(src1[(i + 7) * src1_w + k + 5]);
                ymm14 = _mm256_set1_ps(src1[(i + 7) * src1_w + k + 6]);
                ymm15 = _mm256_set1_ps(src1[(i + 7) * src1_w + k + 7]);

                /* vector multiply */
                /* row 1 2 */
                ymm8 = _mm256_mul_ps(ymm8, ymm0);
                ymm9 = _mm256_mul_ps(ymm9, ymm1);
                ymm8 = _mm256_add_ps(ymm8, ymm9);
                /* row 3 4 */
                ymm10 = _mm256_mul_ps(ymm10, ymm2);
                ymm11 = _mm256_mul_ps(ymm11, ymm3);
                ymm10 = _mm256_add_ps(ymm10, ymm11);
                /* row 5 6 */
                ymm12 = _mm256_mul_ps(ymm12, ymm4);
                ymm13 = _mm256_mul_ps(ymm13, ymm5);
                ymm12 = _mm256_add_ps(ymm12, ymm13);
                /* row 7 8 */
                ymm14 = _mm256_mul_ps(ymm14, ymm6);
                ymm15 = _mm256_mul_ps(ymm15, ymm7);
                ymm14 = _mm256_add_ps(ymm14, ymm15);
                /* sum */
                ymm8 = _mm256_add_ps(ymm8, ymm10);
                ymm12 = _mm256_add_ps(ymm12, ymm14);
                ymm8 = _mm256_add_ps(ymm8, ymm12);

                des7 = _mm256_add_ps(des7, ymm8);

            }
            /* store dst */
            _mm256_storeu_ps((float *) (dst + (i + 0) * src2_w + j), des0);
            _mm256_storeu_ps((float *) (dst + (i + 1) * src2_w + j), des1);
            _mm256_storeu_ps((float *) (dst + (i + 2) * src2_w + j), des2);
            _mm256_storeu_ps((float *) (dst + (i + 3) * src2_w + j), des3);
            _mm256_storeu_ps((float *) (dst + (i + 4) * src2_w + j), des4);
            _mm256_storeu_ps((float *) (dst + (i + 5) * src2_w + j), des5);
            _mm256_storeu_ps((float *) (dst + (i + 6) * src2_w + j), des6);
            _mm256_storeu_ps((float *) (dst + (i + 7) * src2_w + j), des7);
        }
    }
}

#endif

