#ifndef MATHS_H
#define MATHS_H

#ifdef __cplusplus
	extern "C" {
#endif

		#include "../Defines/Defines.h"

		/* DEFINES */
		#define NUMARGS(type, ...) (sizeof((type[]){(type)0, ##__VA_ARGS__}) / sizeof(type) - 1)

		#define K_MAX(i1, i2) ((i1) > (i2) ? (i1) : (i2))
		#define K_MIN(i1, i2) ((i1) < (i2) ? (i1) : (i2))

		#define K_PI 3.14159265358979323846f
		#define K_PI_2 2.0f * K_PI
		#define K_HALF_PI 0.5f * K_PI
		#define K_QUARTER_PI 0.25f * K_PI
		#define K_ONE_OVER_PI 1.0f / K_PI
		#define K_ONE_OVER_TWO_PI 1.0f / K_PI_2

		#define K_SQRT_TWO 1.41421356237309504880f
		#define K_SQRT_THREE 1.73205080756887729352f
		#define K_SQRT_ONE_OVER_TWO 0.70710678118654752440f
		#define K_SQRT_ONE_OVER_THREE 0.57735026918962576450f

		#define K_DEG2RAD_MULTIPLIER K_PI / 180.0f
		#define K_RAD2DEG_MULTIPLIER 180.0f / K_PI

		#define K_SEC_TO_MS_MULTIPLIER 1000.0f
		#define K_MS_TO_SEC_MULTIPLIER 0.001f

		#define K_INFINITY 1e30f
		#define K_FLOAT_EPSILON 1.192092896e-07f

		/* INLINE BOOL / B8 */
		KINLINE b8 gontiMathIsPowerOf2(u64 value) {
			return (value != 0) && ((value & (value - 1)) == 0);
		}

		/*VOID*/
		KAPI void gontiMathSwap(int* a, int* b);
		KAPI void gontiMathSwapFloat(float* a, float* b);
		KAPI void gontiMathPrintUintArray(unsigned int* arr, unsigned int noElements);
		KAPI void gontiMathDivMod(unsigned int num, unsigned int divisor, unsigned int* quotient, unsigned int* remainder);
		KAPI void gontiMathDivModLL(unsigned long long num, unsigned long long divisor, unsigned long long* quotient, unsigned long long* remainder);

		/*BOOL*/
		KAPI bool gontiMathContainsUint(unsigned int* arr, unsigned int noElements, unsigned int target);

		/*UNSIGNED INT*/
		KAPI unsigned int gontiMathNumDigits(unsigned int val, unsigned int base);

		/*UNSIGNED LONG LONG*/
		KAPI unsigned long long gontiMathNumDigitsLL(unsigned long long val, unsigned long long base);

		/* F32 / FLOAT */
		KAPI f32 gontiMathSin(f32 x);
		KAPI f32 gontiMathCos(f32 x);
		KAPI f32 gontiMathTan(f32 x);
		KAPI f32 gontiMathAcos(f32 x);
		KAPI f32 gontiMathSqrt(f32 x);
		KAPI f32 gontiMathAbs(f32 x);
		KAPI f32 gontiMathRandomFloat();
		KAPI f32 gontiMathRandomFloatInRange(f32 min, f32 max);

		/* I32 / INT */
		KAPI i32 gontiMathRandom();
		KAPI i32 gontiMathRandomInRange(i32 min, i32 max);

#ifdef __cplusplus
	}
#endif

#endif