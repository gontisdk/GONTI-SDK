#ifndef VECTOR_H
#define VECTOR_H

#ifdef __cplusplus
	extern "C" {
#endif

		#include "../../../Maths.h"
		#include "../VectorsTypes.inl"

		#include "stdarg.h"
		#include "stdio.h"
		#include "stdlib.h"
		#include "string.h"
		#include "math.h"

		extern KAPI const vec VEC_UNDEFINED;

		/*VEC*/
		KAPI vec gontiVecAllocate(unsigned int dim);
		KAPI vec gontiVecConstructDefaultgontiVectors(unsigned int dim, float val);
		KAPI vec gontiVecConstructEmptyVector(unsigned int dim);
		KAPI vec gontiVecNewVector(unsigned int dim, ...);
		KAPI vec gontiVecCopy(vec v);
		KAPI vec gontiVecCopyPtr(vec* v);
		KAPI vec gontiVecScalarMultiplication(vec v, float k);
		KAPI vec gontiVecScalarDivision(vec v, float k);
		KAPI vec gontiVecAdd(vec v1, vec v2);
		KAPI vec gontiVecSubtract(vec v1, vec v2);
		KAPI vec gontiVecMultiply(vec v1, vec v2);
		KAPI vec gontiVecDivide(vec v1, vec v2);
		KAPI vec gontiVecCross(vec v1, vec v2);
		KAPI vec gontiVecNormalized(vec v);
		KAPI vec gontiVecPower(vec v, float k);

		/*VOID*/
		KAPI void gontiVecPrint(vec v);
		KAPI void gontiVecScalarMultiplicationBy(vec* v, float k);
		KAPI void gontiVecScalarDivisionBy(vec* v, float k);
		KAPI void gontiVecNormalize(vec* v);
		KAPI void gontiVecPowerOf(vec* v, float k);
		KAPI void gontiVecFree(vec* v);

		/*BOOL*/
		KAPI bool gontiVecEquals(vec v1, vec v2);
		KAPI bool gontiVecAddTo(vec* v1, vec v2);
		KAPI bool gontiVecSubtractFrom(vec* v1, vec v2);
		KAPI bool gontiVecMultiplyBy(vec* v1, vec v2);
		KAPI bool gontiVecDivideBy(vec* v1, vec v2);
		KAPI bool gontiVecOrthogonal(vec v1, vec v2);

		/*FLOAT*/
		KAPI float gontiVecDot(vec v1, vec v2);
		KAPI float gontiVecMagnitude(vec v);
		KAPI float gontiVecMagnitudeSquared(vec v);

		/*DEFINES*/

		#define vector(...) gontiVecNewVector(NUMARGS(float, __VA_ARGS__), ##__VA_ARGS__)

#ifdef __cplusplus
	}
#endif

#endif