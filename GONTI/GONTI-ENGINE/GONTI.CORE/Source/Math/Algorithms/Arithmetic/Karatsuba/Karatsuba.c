#include "Karatsuba.h"

#include <stdlib.h>
#include "../../../../Memory/Memory.h"
#include "../../../Algebra/BigInteger/BigInteger.h"

/* 
* INT* 
*/
int* gontiKaratsubaMultiplyLongIntArr(
    int* arr1, unsigned int arr1I, unsigned int arr1F,
    int* arr2, unsigned int arr2I, unsigned int arr2F,
    unsigned int* outSize
) {
    unsigned int i1range = arr1F - arr1I;
    unsigned int i2range = arr2F - arr2I;
    unsigned int noDigits = i1range + i2range;

    int* ret = k_allocate(noDigits * sizeof(int), GONTI_MEMORY_TAG_ARRAY);
    if (!ret) {
        *outSize = 0;
        return NULL;
    }

    for (unsigned int i = 0; i < noDigits; i++) ret[i] = 0;
    for (unsigned int i = 0; i < i1range; i++) {
        int carry = 0;

        for (unsigned int j = 0; j < i2range; j++) {
            bigint prod = gontiBigintMultiplyUnsignedLL((long long)arr1[arr1I + i], (long long)arr2[arr2I + j]);

            unsigned int addition = 0;
            if (prod.noDigits > 0) addition = carry + prod.digits[0];
            else addition = carry;

            if (addition >= BIGINT_BASE) {
                carry = 1;
                addition -= BIGINT_BASE;
            } else carry = 0;

            ret[i + j] += addition;
            if (ret[i + j] >= BIGINT_BASE) {
                carry++;
                ret[i + j] -= BIGINT_BASE;
            }

            unsigned int factor = 1;
            for (int k = 1; k < prod.noDigits; k++) {
                carry += prod.digits[k] * factor;
                factor *= BIGINT_BASE;
            }
        }

        if (carry) ret[i + i2range] += carry;
    }

    *outSize = noDigits;
    return ret;
}

int* gontiKaratsubaMultiplyIntArr(
    int* arr1, unsigned int arr1Size,
    int* arr2, unsigned int arr2Size,
    unsigned int idxI, unsigned int idxF,
    unsigned int* outSize
) {
    if (idxI >= arr1Size || idxI >= arr2Size || idxI >= idxF) {
        *outSize = 0;
        return NULL;
    }

    unsigned int range = idxF - idxI;
    unsigned int noDigits = range << 1;
    unsigned int half = ((idxF + idxI) >> 1) + (range & 1);
    unsigned int sumTermSize = half - idxI + 1;

    if (range <= KARATSUBA_THRESHOLD) 
    return gontiKaratsubaMultiplyLongIntArr(
        arr1, idxI, K_MIN(arr1Size, idxF), 
        arr2, idxI, K_MIN(arr2Size, idxF),
        outSize
    );

    int* ret = k_allocate(noDigits * sizeof(int), GONTI_MEMORY_TAG_ARRAY);
    int* i1sum = k_allocate(sumTermSize * sizeof(int), GONTI_MEMORY_TAG_ARRAY);
    int* i2sum = k_allocate(sumTermSize * sizeof(int), GONTI_MEMORY_TAG_ARRAY);

    unsigned int leftIdx = idxI;
    unsigned int rightIdx = half;

    bool c1 = false;
    bool c2 = false;

    for (int i = 0; i < sumTermSize; i++, leftIdx++, rightIdx++) {
        i1sum[i] = c1;
        c1 = false;

        if (leftIdx < arr1Size && leftIdx < half) i1sum[i] += arr1[leftIdx];
        if (rightIdx < arr1Size && rightIdx < idxF) i1sum[i] += arr1[rightIdx];

        i2sum[i] = c2;
        c2 = false;

        if (leftIdx < arr2Size && leftIdx < half) i2sum[i] += arr2[leftIdx];
        if (rightIdx < arr2Size && rightIdx < idxF) i2sum[i] += arr2[rightIdx];

        if (i1sum[i] >= BIGINT_BASE) {
            c1 = true;
            i1sum[i] -= BIGINT_BASE;
        }

        if (i2sum[i] >= BIGINT_BASE) {
            c2 = true;
            i2sum[i] -= BIGINT_BASE;
        }
    }

    unsigned int hiSize, loSize, mdSize;
    int* hi = gontiKaratsubaMultiplyIntArr(arr1, arr1Size, arr2, arr2Size, half, idxF, &hiSize);
    int* lo = gontiKaratsubaMultiplyIntArr(arr1, arr1Size, arr2, arr2Size, idxI, half, &loSize);
    int* md = gontiKaratsubaMultiplyIntArr(i1sum, sumTermSize, i2sum, sumTermSize, 0, sumTermSize, &mdSize);

    int carry = 0;
    for (unsigned int i = 0; i < mdSize; i++) {
        md[i] -= ((i < hiSize) ? hi[i] : 0) + ((i < loSize) ? lo[i] : 0);
        if (carry) md[i] += carry;

        carry = 0;
        while (md[i] < 0) {
            carry--;
            md[i] += BIGINT_BASE;
        }
    }

    int loHalf = (half - idxI) << 1;
    int i = 0;

    for (; i < loHalf; i++) ret[i] = (i < loSize) ? lo[i] : 0;

    int hiIdx = 0;
    for (; i < noDigits; i++, hiIdx++) ret[i] = (hiIdx < hiSize) ? hi[hiIdx] : 0;

    carry = false;
    unsigned int retIdx = half - idxI;

    for (i = 0; i < mdSize && retIdx < noDigits; i++, retIdx++) {
        ret[retIdx] += md[i] + carry;

        carry = false;
        if (ret[retIdx] >= BIGINT_BASE) {
            carry = true;
            ret[retIdx] -= BIGINT_BASE;
        }
    }

    if (carry && retIdx < noDigits) ret[retIdx]++;

    k_free(lo);
    k_free(md);
    k_free(hi);
    k_free(i1sum);
    k_free(i2sum);

    *outSize = noDigits;
    return ret;
}