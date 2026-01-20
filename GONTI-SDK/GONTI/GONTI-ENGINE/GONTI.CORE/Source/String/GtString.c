#include "GtString.h"

#include <string.h>
#include "../Memory/GtMemory.h"

char* gontiStringDuplicate(const char* str) {
    GtU64 length = gontiStringLength(str);
    char* copy = gt_allocate(length + 1, GT_MEM_TAG_STRING);
    gt_copyMemory(copy, str, length + 1);

    return copy;
}
GtU64 gontiStringLength(const char* str) {
    return strlen(str);
}
GtB8 gontiStringEqual(const char* str1, const char* str2) {
    return strcmp(str1, str2) == 0;
}