#ifndef BIGINTEGER_H
#define BIGINTEGER_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../../../Defines/Defines.h"

        #include <stdarg.h>
        #include <stdio.h>

        #include "../../Maths.h"

        typedef struct {
            bool sign;
            unsigned int capacity;
            unsigned int noDigits;
            int* digits;
            bool heapAllocated;
        }bigint;

        /*BIGINT*/
        KAPI bigint gontiBigintAllocate(unsigned int capacity);
        KAPI bigint gontiBigintZero();
        KAPI bigint gontiBigintOne();
        KAPI bigint gontiBigintNegOne();
        KAPI bigint gontiBigintFromStr(char* str);
        KAPI bigint gontiBigintFromInt(int i);
        KAPI bigint gontiBigintFromLL(long long ll);
        KAPI bigint gontiBigintFromUnsignedInt(unsigned int ui);
        KAPI bigint gontiBigintFromUnsignedLL(unsigned long long ull);
        KAPI bigint gontiBigintAdd(bigint bi1, bigint bi2);
        KAPI bigint gontiBigintSubtract(bigint bi1, bigint bi2);
        KAPI bigint gontiBigintMultiplyUnsignedInt(unsigned int i1, unsigned int i2);
        KAPI bigint gontiBigintMultiplyUnsignedLL(unsigned long long ll1, unsigned long long ll2);
        KAPI bigint gontiBigintMultiply(bigint bi1, bigint bi2);
        KAPI bigint gontiBigintCopyIntArr(int* arr, unsigned int n, bool sign);
        KAPI bigint gontiBigintLongMultiply(bigint bi1, bigint bi2);
        KAPI bigint gontiBigintKaratsubaMultiply(bigint bi1, bigint bi2);
        KAPI bigint gontiBigintBaseDivide(bigint bi, int d);
        KAPI bigint gontiBigintBaseMult(bigint bi, int d);

        /* CHAR* */
        KAPI char* gontiBigintPtrToString(bigint* bi);
        KAPI char* gontiBigintToString(bigint bi);

        /*CHAR*/
        KAPI char gontiBigintCompare(bigint bi1, bigint bi2);

        /*VOID*/
        KAPI void gontiBigintFree(bigint* bi);
        KAPI void gontiBigintFreeString(char* str);
        KAPI void gontiBigintTrim(bigint* bi);
        KAPI void gontiBigintDebugPrint(const char* name, bigint* bi);

        /* INT* */
        KAPI int* gontiBigintRightShiftArr(int* i, int n, int d, int* newSize);
        KAPI int* gontiBigintLeftShiftArr(int* i, int n, int d, int* newSize);

        /*DEFINE*/
        #define BIGINT_BASE 1000000000
        #define BIGINT_NO_BASE_DIGITS 9

#ifdef __cplusplus
    }
#endif

#endif