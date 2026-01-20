#ifndef GtEntryTypesH
#define GtEntryTypesH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.h>

        typedef struct GtEntryWindowConfig {
            char* windowName;
            char* className;

            GtI16 startPosX;
            GtI16 startPosY;
            GtI16 startWidth;
            GtI16 startHeight;
        } GtEntryWindowConfig;

        typedef struct GtEntry {
            GtEntryWindowConfig windowConfig;

            GtB8 (*GtInitializeFN) (struct GtEntry* entryInst);
            GtB8 (*GtUpdateFN) (struct GtEntry* entryInst, GtF32 deltaTime);
            GtB8 (*GtRenderFN) (struct GtEntry* entryInst, GtF32 deltaTime);

            void (*GtOnResizeFN) (struct GtEntry* entryInst, GtU32 width, GtU32 height);
            void (*GtShutdownFN) (struct GtEntry* entryInst);
            void* GtState;
        } GtEntry;

#ifdef __cplusplus
    }
#endif

#endif