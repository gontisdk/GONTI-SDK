#ifndef GtVkRBackendH
#define GtVkRBackendH

#ifdef __cplusplus 
    extern "C" {
#endif

        #include "../../Types/GtVkContextTypes.inl"
        #include "../../Platform/GtVkPlatform.h"
    
        GTAPI GtB8 gontiVkRendererBackendInitialize(const char* appName, GtVkPlatformState* platState);
        GTAPI GtB8 gontiVkRendererBackendBeginFrame(GtF32 deltaTime);
        GTAPI GtB8 gontiVkRendererBackendEndFrame(GtF32 deltaTime);

        GTAPI void gontiVkRendererBackendShutdown();
        GTAPI void gontiVkRendererBackendOnResized(GtU16 width, GtU16 height);

#ifdef __cplusplus 
    }
#endif

#endif