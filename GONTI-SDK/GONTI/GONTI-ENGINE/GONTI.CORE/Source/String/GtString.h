#ifndef GtStringH
#define GtStringH

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../Defines/GtDefines.h"

        GTAPI char* gontiStringDuplicate(const char* str);
        GTAPI GtU64 gontiStringLength(const char* str);
        GTAPI GtB8 gontiStringEqual(const char* str1, const char* str2);

#ifdef __cplusplus
    }
#endif

#endif