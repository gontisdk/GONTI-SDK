#include "Maths.h"

#include <stdio.h>
#include <stdlib.h>

/*
* VOID
*/
void gontiMathSwap(int* a, int* b) {
	*a ^= *b;
	*b = *a ^ *b;
	*a ^= *b;
}
void gontiMathSwapFloat(float* a, float* b) {
	gontiMathSwap((int*)a, (int*)b);
}
void gontiMathPrintUintArray(unsigned int* arr, unsigned int noElements) {
	printf("\n[ ");

	for (unsigned int i = 0; i < noElements; i++) {
		printf("%d ", arr[i]);
	}

	printf("]\n");
}
void gontiMathDivMod(unsigned int num, unsigned int divisor, unsigned int* quotient, unsigned int* remainder) {
	div_t res = div(num, divisor);
	*quotient = res.quot;
	*remainder = res.rem;
}
void gontiMathDivModLL(unsigned long long num, unsigned long long divisor, unsigned long long* quotient, unsigned long long* remainder) {
	lldiv_t res = lldiv(num, divisor);
	*quotient = res.quot;
	*remainder = res.rem;
}

/*
* BOOL
*/
bool gontiMathContainsUint(unsigned int* arr, unsigned int noElements, unsigned int target) {
	for (unsigned int i = 0; i < noElements; i++) {
		if (arr[i] == target) return true;
	}

	return false;
}

/*
* UNSIGNED INT
*/
unsigned int gontiMathNumDigits(unsigned int val, unsigned int base) {
	if (base <= 1) return 0;

	unsigned int ret = 0;

	while (val) {
		ret++;
		val /= base;
	}

	return ret;
}

/*
* UNSIGNED LONG LONG
*/
unsigned long long gontiMathNumDigitsLL(unsigned long long val, unsigned long long base) {
	if (base <= 1LL) return 0;

	unsigned long long ret = 0;

	while (val) {
		ret++;
		val /= base;
	}

	return ret;
}

/* 
* F32 / FLOAT 
*/
f32 gontiMathSin(f32 x) {
	return 0;
}
f32 gontiMathCos(f32 x) {
	return 0;
}
f32 gontiMathTan(f32 x) {
	return 0;
}
f32 gontiMathAcos(f32 x) {
	return 0;
}
f32 gontiMathSqrt(f32 x) {
	return 0;
}
f32 gontiMathAbs(f32 x) {
	return 0;
}
f32 gontiMathRandomFloat() {
	return 0;
}
f32 gontiMathRandomFloatInRange(f32 min, f32 max) {
	return 0;
}

/* 
* I32 / INT 
*/
i32 gontiMathRandom() {
	return 0;
}
i32 gontiMathRandomInRange(i32 min, i32 max) {
	return 0;
}