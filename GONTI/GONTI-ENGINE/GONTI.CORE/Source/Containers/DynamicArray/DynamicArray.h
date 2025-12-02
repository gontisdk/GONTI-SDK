#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

    #ifdef __cplusplus
        extern "C" {
    #endif

            #include "../../Defines/Defines.h"

            #define GONTI_DARRAY_DEFAULT_CAPACITY 1
            #define GONTI_DARRAY_RESIZE_FACTOR 2

            #define gontiDarrayCreate(type) \
                _gontiDarrayCreate(GONTI_DARRAY_DEFAULT_CAPACITY, sizeof(type))
                
            #define gontiDarrayReserve(type, capacity) \
                _gontiDarrayCreate(capacity, sizeof(type))

            #define gontiDarrayDestroy(array) \
                _gontiDarrayDestroy(array);

            #define gontiDarrayPush(array, value) { \
                typeof(value) temp = value; \
                array = _gontiDarrayPush(array, &temp); \
            }

            #define gontiDarrayPop(array, valuePtr) \
                _gontiDarrayPop(array, valuePtr)

            #define gontiDarrayInsertAt(array, index, value) { \
                typeof(value) temp = value; \
                array = _gontiDarrayInsertAt(array, index, &temp); \
            }

            #define gontiDarrayPopAt(array, index, valuePtr) \
                _gontiDarrayPopAt(array, index, valuePtr)

            #define gontiDarrayClear(array) \
                _gontiDarrayFieldSet(array, GONTI_DARRAY_LENGTH, 0)

            #define gontiDarrayCapacity(array) \
                _gontiDarrayFieldGet(array, GONTI_DARRAY_CAPACITY)

            #define gontiDarrayLength(array) \
                _gontiDarrayFieldGet(array, GONTI_DARRAY_LENGTH)

            #define gontiDarrayStride(array) \
                _gontiDarrayFieldGet(array, GONTI_DARRAY_STRIDE)

            #define gontiDarrayLengthSet(array, value) \
                _gontiDarrayFieldSet(array, GONTI_DARRAY_LENGTH, value)

            enum {
                GONTI_DARRAY_CAPACITY,
                GONTI_DARRAY_LENGTH,
                GONTI_DARRAY_STRIDE,
                GONTI_DARRAY_FIELD_LENGTH
            };

            KAPI void* _gontiDarrayCreate(u64 length, u64 stride);
            KAPI void _gontiDarrayDestroy(void* array);
            KAPI u64 _gontiDarrayFieldGet(void* array, u64 field);
            KAPI void _gontiDarrayFieldSet(void* array, u64 field, u64 value);
            KAPI void* _gontiDarrayResize(void* array);
            KAPI void* _gontiDarrayPush(void* array, const void* valuePtr);
            KAPI void _gontiDarrayPop(void* array, void* dest);
            KAPI void* _gontiDarrayPopAt(void* array, u64 index, void* dest);
            KAPI void* _gontiDarrayInsertAt(void* array, u64 index, void* valuePtr);

    #ifdef __cplusplus
        }
    #endif

#endif