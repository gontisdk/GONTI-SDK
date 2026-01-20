#include "GtApp.h"

#include <GONTI/GONTI-ENGINE/GONTI.RENDER/Source/Config/SetupGraphicBackend/GtUsrSetGraphBackend.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Memory/GtMemory.h>

#if defined(GONTI_USE_VK)
    #include "Vulkan/GtVkApp.h"
#elif defined(GONTI_USE_GL)
#elif defined(GONTI_USE_DX)
#endif

/* B8 */
GtB8 gontiApplicationCreate(GtEntry* entry) {
    gontiInitializeMemory();

    #if defined(GONTI_USE_VK)
        return gontiVkApplicationCreate(entry);
    #endif

    return GtFalse;
}
GtB8 gontiApplicationRun(GtU64 fpsLimit, GtB8 isFpsLimit) {
    GtB8 result = 0;

    #if defined(GONTI_USE_VK)
        result = gontiVkApplicationRun(fpsLimit, isFpsLimit);
    #endif

    gontiShutdownMemory();
    return result;
}

/* VOID */
void gontiApplicationPause() {
    #if defined(GONTI_USE_VK)
        gontiVkApplicationPause();
    #endif
}
void gontiApplicationResume() {
    #if defined(GONTI_USE_VK)
        gontiVkApplicationResume();
    #endif
}
void gontiApplicationSetWidth(GtU32 width) {
#if defined(GONTI_USE_VK)
        gontiVkApplicationSetWidth(width);
    #endif
}
void gontiApplicationSetHeight(GtU32 height) {
    #if defined(GONTI_USE_VK)
        gontiVkApplicationSetHeight(height);
    #endif
}

/* GtI16 */
GtI16 gontiApplicationGetWidth() {
    #if defined(GONTI_USE_VK)
        return gontiVkApplicationGetWidth();
    #endif

    return 0;
}
GtI16 gontiApplicationGetHeight() {
    #if defined(GONTI_USE_VK)
        return gontiVkApplicationGetHeight();
    #endif

    return 0;
}

/* BOOL/B8 */
GtB8 gontiApplicationIsPaused() {
    #if defined(GONTI_USE_VK)
        return gontiVkApplicationIsPaused();
    #endif

    return GtFalse;
}