#ifndef MATRICESTYPES_INL
#define MATRICESTYPES_INL

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../../../Defines/Defines.h"
        #include "../Vectors/VectorsTypes.inl"
        #include <stdalign.h>
        #include <xmmintrin.h>

        typedef struct {
			unsigned int rows;
			unsigned int cols;
			float** elements;
		}mat;

        typedef union mat4_u {
            #if defined(KUSE_SIMD)
                alignas(16) vec4 rows[4];
            #endif

            alignas(16) f32 data[16];
        } mat4;

#ifdef __cplusplus
    }
#endif

#endif