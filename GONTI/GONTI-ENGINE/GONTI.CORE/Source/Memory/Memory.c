#include "Memory.h"

#include <stdio.h>
#include "../Platform/Platform.h"
#include "../Logging/Logger.h"
#include "../String/String.h"

static const char* memoryTagStrings[GONTI_MEMORY_TAG_MAX_TAGS] = {
    "UNKNOWN:.................................",
    "ARRAY:...................................",
    "ARRAYEX:.................................",
    "DARRAY:..................................",
    "DARRAYEX:................................",
    "DICT:....................................",
    "RING_QUEUE:..............................",
    "BST:.....................................",
    "STRING:..................................",
    "APPLICATION:.............................",
    "JOB:.....................................",
    "TEXTURE:.................................",
    "MAT_INST:................................",
    "WINDOW:..................................",
    "RENDERER:................................",
    "ENTRY:...................................",
    "TRANSFORM:...............................",
    "ENTITY:..................................",
    "ENTITY_MODE:.............................",
    "SCENE:...................................",
    "MATH:....................................",
    "VECTOR:..................................",
    "MATRIX:..................................",
    "BIGINTEGER..............................."
};

struct GontiMemoryStats {
    u64 totalAllocated;
    u64 taggedAllocations[GONTI_MEMORY_TAG_MAX_TAGS];
}; static struct GontiMemoryStats gontiMemoryStats;

typedef struct GontiAllocationHeader {
    u64 size;
    GontiMemoryTag tag;
} GontiAllocationHeader;

/* CHAR* */
char* k_getMemoryUsageStr() {
    const u64 gib = 1024 * 1024 * 1024;
    const u64 mib = 1024 * 1024;
    const u64 kib = 1024;

    char buffer[8000] = "System memory use (tagged):\n";
    u64 offset = gontiStringLength(buffer);

    for (u32 i = 0; i < GONTI_MEMORY_TAG_MAX_TAGS; i++) {
        char unit[4] = "XiB";
        float amount = 1.0f;

        if (gontiMemoryStats.taggedAllocations[i] >= gib) {
            unit[0] = 'G';
            amount = gontiMemoryStats.taggedAllocations[i] / (float)gib;
        } else if (gontiMemoryStats.taggedAllocations[i] >= mib) {
            unit[0] = 'M';
            amount = gontiMemoryStats.taggedAllocations[i] / (float)mib;
        } else if (gontiMemoryStats.taggedAllocations[i] >= kib) {
            unit[0] = 'K';
            amount = gontiMemoryStats.taggedAllocations[i] / (float)kib;
        } else {
            unit[0] = 'B';
            unit[1] = 0;
            amount = (float)gontiMemoryStats.taggedAllocations[i];
        }

        i32 length = snprintf(buffer + offset, 8000, "  %s-> %.2f%s\n", memoryTagStrings[i], amount, unit);
        offset += length;
    }

    char* outString = gontiStringDuplicate(buffer);
    return outString;
}

/*VOID*/
void gontiInitializeMemory() {
    gontiPlatformZeroMemory(&gontiMemoryStats, sizeof(struct GontiMemoryStats));
}
void gontiShutdownMemory() {
    gontiPlatformFree(&gontiMemoryStats, false);
}
void k_free(void* block) {
    GontiAllocationHeader* header = ((GontiAllocationHeader*)block) - 1;

    gontiMemoryStats.totalAllocated -= 1;
    gontiMemoryStats.taggedAllocations[header->tag] -= header->size;

    gontiPlatformFree(header, false);
}

/* VOID* */
void* k_allocate(u64 size, GontiMemoryTag memTag) {
    if (memTag == GONTI_MEMORY_TAG_UNKOWN)
        KWARN("k_allocate() called with UNKNOWN tag");

    gontiMemoryStats.totalAllocated += 1;

    u64 total = size + sizeof(GontiAllocationHeader);
    GontiAllocationHeader* header = gontiPlatformAllocate(total, false);

    header->size = size;
    header->tag = memTag;

    gontiMemoryStats.taggedAllocations[memTag] += size;

    void* block = (void*)(header + 1);
    gontiPlatformZeroMemory(block, size);
    return block;
}
void* k_zeroMemory(void* block, u64 size) {
    return gontiPlatformZeroMemory(block, size);
}
void* k_copyMemory(void* dest, const void* source, u64 size) {
    return gontiPlatformCopyMemory(dest, source, size);
}
void* k_setMemory(void* dest, i32 value, u64 size) {
    return gontiPlatformSetMemory(dest, value, size);
}
void* k_reallocate(void* block, u64 newSize) {
    GontiAllocationHeader* oldHeader = ((GontiAllocationHeader*)block) - 1;

    gontiMemoryStats.taggedAllocations[oldHeader->tag] -= oldHeader->size;
    gontiMemoryStats.taggedAllocations[oldHeader->tag] += newSize;

    u64 total = newSize + sizeof(GontiAllocationHeader);
    GontiAllocationHeader* newHeader = gontiPlatformReallocate(oldHeader, total);

    newHeader->size = newSize;
    return (void*)(newHeader + 1);
}