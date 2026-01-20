#ifndef GtAppH
#define GtAppH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.h>
        #include "../EntryPoint/GtEntry.h"

        /* B8 */
        GTAPI GtB8 gontiApplicationCreate(GtEntry* entry);
        GTAPI GtB8 gontiApplicationRun(GtU64 fpsLimit, GtB8 isFpsLimit);

        /* VOID */
        GTAPI void gontiApplicationPause();
        GTAPI void gontiApplicationResume();
        GTAPI void gontiApplicationSetWidth(GtU32 width);
        GTAPI void gontiApplicationSetHeight(GtU32 height);

        /* GtI16 */
        GTAPI GtI16 gontiApplicationGetWidth();
        GTAPI GtI16 gontiApplicationGetHeight();

        /* BOOL/B8 */
        GTAPI GtB8 gontiApplicationIsPaused();

#ifdef __cplusplus
    }
#endif

#endif