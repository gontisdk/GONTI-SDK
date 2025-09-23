#ifndef VECTORSTYPES_INL
#define VECTORSTYPES_INL

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../../../Defines/Defines.h"
        #include <stdalign.h>
        #include <xmmintrin.h>

        typedef struct {
			unsigned int dim;
			float* elements;
		}vec;

        typedef union vec2_u {
            f32 elements[2];
            struct {
                union {
                    f32 x, r, s, u;
                };
                union {
                    f32 y, g, t, v;
                };
            };
        } vec2;

        typedef union vec3_u {
            f32 elements[3];
            struct {
                union {
                    f32 x, r, s, u;
                };
                union {
                    f32 y, g, t, v;
                };
                union {
                    f32 z, b, p, w;
                };
            };
        } vec3;

        typedef union vec4_u {
            #if defined(KUSE_SIMD)
                alignas(16) __m128 data;
            #endif

            alignas(16) f32 elements[4];
            struct {
                union {
                    f32 x, r, s;
                };
                union {
                    f32 y, g, t;
                };
                union {
                    f32 z, b, p;
                };
                union {
                    f32 w, a, q;
                };
            };
        } vec4; 
        typedef vec4 quat;

#ifdef __cplusplus
    }
#endif

#endif