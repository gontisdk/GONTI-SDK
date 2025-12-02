#include "BigInteger.h"

#include <stdlib.h>
#include <math.h>

#include "../../../Memory/Memory.h"
#include "../../../String/String.h"
#include "../../Algorithms/Arithmetic/Karatsuba/Karatsuba.h"
#include "../../../Logging/Logger.h"

static int DIG1[1] = {1};
static int DIG0[1] = {0};
static const bigint BIGINT_ZERO = {true, 1, 1, DIG0, false};
static const bigint BIGINT_ONE = {true, 1, 1, DIG1, false};
static const bigint BIGINT_NEG_ONE = {false, 1, 1, DIG1, false};

/*
* BIGINT
*/
bigint gontiBigintAllocate(unsigned int capacity) {
    bigint ret;
    ret.capacity = capacity;
    ret.noDigits = 0;
    ret.sign = true;
    ret.heapAllocated = true;
    ret.digits = k_allocate(capacity * sizeof(int), GONTI_MEMORY_TAG_BIGINT);
    return ret;
}
bigint gontiBigintZero() {
    bigint ret = gontiBigintAllocate(BIGINT_ZERO.capacity);

    ret.sign = BIGINT_ZERO.sign;
    ret.noDigits = BIGINT_ZERO.noDigits;
    k_copyMemory(ret.digits, BIGINT_ZERO.digits, BIGINT_ZERO.noDigits * sizeof(int));

    return ret;
}
bigint gontiBigintOne() {
    bigint ret = gontiBigintAllocate(BIGINT_ONE.capacity);

    ret.sign = BIGINT_ONE.sign;
    ret.noDigits = BIGINT_ONE.noDigits;
    k_copyMemory(ret.digits, BIGINT_ONE.digits, BIGINT_ONE.noDigits * sizeof(int));

    return ret;
}
bigint gontiBigintNegOne() {
    bigint ret = gontiBigintAllocate(BIGINT_NEG_ONE.capacity);

    ret.sign = BIGINT_NEG_ONE.sign;
    ret.noDigits = BIGINT_NEG_ONE.noDigits;
    k_copyMemory(ret.digits, BIGINT_NEG_ONE.digits, BIGINT_NEG_ONE.noDigits * sizeof(int));

    return ret;
}
bigint gontiBigintFromStr(char *str) {
    unsigned int len = gontiStringLength(str);
    unsigned int effectiveLen = len;
    unsigned int firstDigit = 0;

    bool sign = true;
    if (str[firstDigit] == '-') {
        sign = false;
        effectiveLen--;
        firstDigit++;
    }

    while (str[firstDigit] == '0') {
        effectiveLen--;
        firstDigit++;
    }

    if (effectiveLen == 0) return gontiBigintZero();

    unsigned int numDigits, extra;
    gontiMathDivMod(effectiveLen, BIGINT_NO_BASE_DIGITS, &numDigits, &extra);

    if (extra) numDigits++;

    bigint ret = gontiBigintAllocate(numDigits);
    ret.noDigits = numDigits;
    ret.sign = sign;

    unsigned int currentDigit = 0;
    unsigned int digitArrIdx = numDigits - 1;
    char currentDigitCharIdx = extra ? extra : BIGINT_NO_BASE_DIGITS;

    for (unsigned int idx = firstDigit; idx < len; idx++) {
        char c = str[idx];

        if (c >= '0' && c <= '9') {
            currentDigit *= 10;
            currentDigit += c - '0';
            currentDigitCharIdx--;

            if (!currentDigitCharIdx) {
                ret.digits[digitArrIdx--] = currentDigit;

                currentDigit = 0;
                currentDigitCharIdx = BIGINT_NO_BASE_DIGITS;
            }
        } else {
            gontiBigintFree(&ret);
            return gontiBigintZero();
        }
    }

    return ret;
}
bigint gontiBigintFromInt(int i) {
    bool sign = true;
    if (i < 0) {
        sign = false;
        i *= -1;
    }

    bigint ret = gontiBigintFromUnsignedInt(i);
    ret.sign = sign;

    return ret;
}
bigint gontiBigintFromLL(long long ll)
{
    bool sign = true;
    if (ll < 0LL) {
        sign = false;
        ll *= -1LL;
    }

    bigint ret = gontiBigintFromUnsignedLL(ll);
    ret.sign = sign;

    return ret;
}
bigint gontiBigintFromUnsignedInt(unsigned int ui) {
    unsigned int noDigits = gontiMathNumDigits(ui, BIGINT_BASE);

    bigint ret = gontiBigintAllocate(noDigits);
    ret.noDigits = noDigits;
    for (unsigned int idx = 0; idx < noDigits; idx++) {
        unsigned int digit;
        gontiMathDivMod(ui, BIGINT_BASE, &ui, &digit);
        ret.digits[idx] = (int)digit;
    }

    return ret;
}
bigint gontiBigintFromUnsignedLL(unsigned long long ull) {
    unsigned long long noDigits = gontiMathNumDigitsLL(ull, BIGINT_BASE);

    bigint ret = gontiBigintAllocate(noDigits);
    ret.noDigits = noDigits;
    for (unsigned int idx = 0; idx < noDigits; idx++) {
        unsigned long long digit;

        gontiMathDivModLL(ull, BIGINT_BASE, &ull, (unsigned long long *)&digit);
        ret.digits[idx] = digit;
    }

    return ret;
}
bigint gontiBigintAdd(bigint bi1, bigint bi2) {
    if (bi1.sign && !bi2.sign) {
        bi2.sign = true;
        return gontiBigintSubtract(bi1, bi2);
    } else if (!bi1.sign && bi2.sign) {
        bi1.sign = true;
        return gontiBigintSubtract(bi2, bi1);
    }

    unsigned int noDigits = K_MAX(bi1.noDigits, bi2.noDigits) + 1;

    bigint ret = gontiBigintAllocate(noDigits);
    ret.noDigits = noDigits;

    bool carry = false;
    for (unsigned int idx = 0; idx < ret.noDigits; idx++) {
        int res = carry ? 1 : 0;
        if (idx < bi1.noDigits) res += bi1.digits[idx];
        if (idx < bi2.noDigits) res += bi2.digits[idx];

        if (res >= BIGINT_BASE) {
            carry = true;
            res -= BIGINT_BASE;
        } else carry = false;

        ret.digits[idx] = res;
    }

    gontiBigintTrim(&ret);

    if (!bi1.sign && !bi2.sign) ret.sign = false;

    return ret;
}
bigint gontiBigintSubtract(bigint bi1, bigint bi2) {
    char comparison = gontiBigintCompare(bi1, bi2);
    if (!comparison) return BIGINT_ZERO;

    if (bi1.sign && !bi2.sign) {
        bi2.sign = true;
        return gontiBigintAdd(bi1, bi2);
    } else if (!bi1.sign && bi2.sign) {
        bi2.sign = false;
        return gontiBigintAdd(bi1, bi2);
    } else if (!bi1.sign && !bi2.sign) {
        bi1.sign = true;
        bi2.sign = true;
        return gontiBigintSubtract(bi2, bi1);
    } else {
        if (comparison < 0) {
            bigint res = gontiBigintSubtract(bi2, bi1);
            res.sign = !res.sign;
            return res;
        }
    }

    unsigned int noDigits = bi1.noDigits;

    bigint ret = gontiBigintAllocate(noDigits);
    ret.noDigits = noDigits;

    bool carry = false;
    for (unsigned int idx = 0; idx < ret.noDigits; idx++) {
        int res = carry ? -1 : 0;

        if (idx < bi1.noDigits) res += bi1.digits[idx];
        if (idx < bi2.noDigits) res -= bi2.digits[idx];

        if (res < 0) {
            carry = true;
            res += BIGINT_BASE;
        } else carry = false;

        ret.digits[idx] = res;
    }

    gontiBigintTrim(&ret);
    return ret;
}
bigint gontiBigintMultiplyUnsignedInt(unsigned int i1, unsigned int i2) {
    return gontiBigintFromUnsignedInt(i1 * i2);
}
bigint gontiBigintMultiplyUnsignedLL(unsigned long long ll1, unsigned long long ll2) {
    return gontiBigintFromUnsignedLL(ll1 * ll2);
}
bigint gontiBigintMultiply(bigint i1, bigint i2) {
    bool sign = !(i1.sign ^ i2.sign);

    if (!(gontiBigintCompare(i1, BIGINT_ZERO) && gontiBigintCompare(i2, BIGINT_ZERO))) return BIGINT_ZERO;

    if (i1.noDigits == 1 && i1.digits[0] == 1) return gontiBigintCopyIntArr(i2.digits, i2.noDigits, sign);
    else if (i2.noDigits == 1 && i2.digits[0] == 1) return gontiBigintCopyIntArr(i1.digits, i1.noDigits, sign);

    unsigned int minNoDigits, maxNoDigits;
    if (i1.noDigits > i2.noDigits) {
        minNoDigits = i2.noDigits;
        maxNoDigits = i1.noDigits;
    } else {
        minNoDigits = i1.noDigits;
        maxNoDigits = i2.noDigits;
    }

    unsigned int noDigits;
    int* retArr;
    if (maxNoDigits < KARATSUBA_THRESHOLD || minNoDigits <= sqrt(maxNoDigits)) {
        retArr = gontiKaratsubaMultiplyLongIntArr(
            i1.digits, 0, i1.noDigits,
            i2.digits, 0, i2.noDigits,
            &noDigits
        );
    } else {
        retArr = gontiKaratsubaMultiplyIntArr(
            i1.digits, i1.noDigits,
            i2.digits, i2.noDigits,
            0, maxNoDigits,
            &noDigits
        );
    }

    bigint ret = gontiBigintCopyIntArr(retArr, noDigits, sign);
    gontiBigintTrim(&ret);
    if (retArr) free(retArr);

    return ret;
}
bigint gontiBigintCopyIntArr(int *arr, unsigned int n, bool sign) {
    bigint ret = gontiBigintAllocate(n);
    ret.noDigits = n;
    ret.sign = sign;

    k_copyMemory(ret.digits, arr, n * sizeof(int));

    return ret;
}
bigint gontiBigintLongMultiply(bigint bi1, bigint bi2) {
    bool sign = !(bi1.sign ^ bi2.sign);
    
    unsigned int noDigits;
    int* retArr = gontiKaratsubaMultiplyLongIntArr(
        bi1.digits, 0, bi1.noDigits,
        bi2.digits, 0, bi2.noDigits,
        &noDigits
    );

    if (!retArr) {
        return gontiBigintZero();
    }

    bigint ret = gontiBigintCopyIntArr(retArr, noDigits, sign);

    k_free(retArr);
    gontiBigintTrim(&ret);

    return ret;
}
bigint gontiBigintKaratsubaMultiply(bigint bi1, bigint bi2) {
    bool sign = !(bi1.sign ^ bi2.sign);

    unsigned int maxNoDigits = K_MAX(bi1.noDigits, bi2.noDigits);
    unsigned int noDigits;
    int* retArr = gontiKaratsubaMultiplyIntArr(
        bi1.digits, bi1.noDigits,
        bi2.digits, bi2.noDigits,
        0, maxNoDigits,
        &noDigits
    );

    if (!retArr) return gontiBigintZero();

    bigint ret = gontiBigintCopyIntArr(retArr, noDigits, sign);
    k_free(retArr);
    gontiBigintTrim(&ret);

    return ret;
}
bigint gontiBigintBaseDivide(bigint bi, int d) {
    int newSize;
    bi.digits = gontiBigintRightShiftArr(bi.digits, bi.noDigits, d, &newSize);
    bi.noDigits = (unsigned int)newSize;
    return bi;
}

bigint gontiBigintBaseMult(bigint bi, int d) {
    int newSize;
    bi.digits = gontiBigintLeftShiftArr(bi.digits, bi.noDigits, d, &newSize);
    bi.noDigits = (unsigned int)newSize;
    return bi;
}

/*
* CHAR*
*/
char* gontiBigintPtrToString(bigint* bi) {
    if (!bi->noDigits) return "0";

    unsigned int noChars = (bi->noDigits - 1) * BIGINT_NO_BASE_DIGITS;
    unsigned int numDigitsLast = gontiMathNumDigits(bi->digits[bi->noDigits - 1], 10);
    noChars += numDigitsLast;

    if (!bi->sign) noChars++;

    char *ret = k_allocate((noChars + 1) * sizeof(char), GONTI_MEMORY_TAG_STRING);

    if (!ret) return NULL;

    unsigned int strIdx = 0;
    if (!bi->sign) ret[strIdx++] = '-';

    char offset = numDigitsLast;
    unsigned int strDigit;
    for (int digitIdx = bi->noDigits; digitIdx; digitIdx--) {
        char increment = offset;
        unsigned int digit = (unsigned int)bi->digits[digitIdx - 1];

        while (offset--) {
            gontiMathDivMod(digit, 10, &digit, &strDigit);
            ret[strIdx + offset] = '0' + (char)strDigit;
        }

        offset = BIGINT_NO_BASE_DIGITS;
        strIdx += increment;
    }

    ret[noChars] = '\0';
    return ret;
}
char* gontiBigintToString(bigint bi) {
    return gontiBigintPtrToString(&bi);
}

/*
* CHAR
*/
char gontiBigintCompare(bigint bi1, bigint bi2) {
    if (bi1.sign && !bi2.sign) return 1;
    else if (!bi1.sign && bi2.sign) return -1;
    else if (!bi1.sign && !bi2.sign) {
        bi1.sign = true;
        bi2.sign = true;

        char comparison = gontiBigintCompare(bi1, bi2);
        return -comparison;
    }

    if (bi1.noDigits > bi2.noDigits) return 1;
    else if (bi1.noDigits < bi2.noDigits) return -1;

    unsigned int idx = bi1.noDigits;
    while (idx--) {
        if (bi1.digits[idx] > bi2.digits[idx]) return 1;
        else if (bi1.digits[idx] < bi2.digits[idx]) return -1;
    }

    return 0;
}

/*
* VOID
*/
void gontiBigintFree(bigint* bi) {
    if (!bi->heapAllocated) return;

    k_free(bi->digits);
    bi->capacity = 0;
    bi->noDigits = 0;
    bi->sign = 1;
    bi->heapAllocated = false;
}
void gontiBigintFreeString(char* str) {
    if (!str) return;
    
    //unsigned int len = gontiStringLength(str);
    k_free(str);
}
void gontiBigintTrim(bigint* bi) {
    while (!bi->digits[bi->noDigits - 1]) bi->noDigits--;
    int* oldDigits = bi->digits;

    bi->digits = k_allocate(bi->noDigits * sizeof(int), GONTI_MEMORY_TAG_BIGINT);
    k_copyMemory(bi->digits, oldDigits, bi->noDigits * sizeof(int));
    k_free(oldDigits);
}
void gontiBigintDebugPrint(const char* name, bigint* bi) {
    printf("%s: sign=%d, noDigits=%u, capacity=%u\n", 
            name, bi->sign, bi->noDigits, bi->capacity);
    printf("  Digits (base %d): ", BIGINT_BASE);
    for (unsigned int i = 0; i < bi->noDigits; i++) {
        printf("[%u]=%d ", i, bi->digits[i]);
    }
    printf("\n");
    
    bool hasNegative = false;
    for (unsigned int i = 0; i < bi->noDigits; i++) {
        if (bi->digits[i] < 0) {
            KWARN("Negative digit at index %u: %d\n", i, bi->digits[i]);
            hasNegative = true;
        }
        if (bi->digits[i] >= BIGINT_BASE) {
            KWARN("Digit >= BASE at index %u: %d\n", i, bi->digits[i]);
        }
    }
    if (!hasNegative) {
        char* str = gontiBigintToString(*bi);
        printf("  String: %s\n", str);
        k_free(str);
    }
}

/*
* INT*
*/
int* gontiBigintRightShiftArr(int* i, int n, int d, int* newSize) {
    if (d < 0) return gontiBigintLeftShiftArr(i, n, -d, newSize);
    if (!d) {
        *newSize = n;
        return i;
    }
    if (d >= n) {
        free(i);
        *newSize = 0;
        return NULL;
    }

    int noLeadingZeros = 0;
    int idx = n - 1;
    while (idx >= 0 && !i[idx--]) noLeadingZeros++;
    if (noLeadingZeros == n) {
        free(i);
        *newSize = 0;
        return NULL;
    }

    *newSize = n - noLeadingZeros - d;
    int *ret = k_allocate(*newSize * sizeof(int), GONTI_MEMORY_TAG_ARRAY);
    k_copyMemory(ret, i + d, (n - noLeadingZeros - d) * sizeof(int));
    free(i);

    return ret;
}
int* gontiBigintLeftShiftArr(int* i, int n, int d, int* newSize) {
    if (d < 0) return gontiBigintRightShiftArr(i, n, -d, newSize);
    if (!d) {
        *newSize = n;
        return i;
    }

    int noLeadingZeros = 0;
    int idx = n - 1;
    while (idx >= 0 && !i[idx--]) noLeadingZeros++;

    if (noLeadingZeros == n) {
        free(i);
        *newSize = 0;
        return NULL;
    }

    *newSize = n - noLeadingZeros + d;
    int *ret = k_allocate(*newSize * sizeof(int), GONTI_MEMORY_TAG_ARRAY);
    k_copyMemory(ret + d, i, (n - noLeadingZeros) * sizeof(int));
    k_setMemory(ret, 0, d * sizeof(int));
    free(i);

    return ret;
}