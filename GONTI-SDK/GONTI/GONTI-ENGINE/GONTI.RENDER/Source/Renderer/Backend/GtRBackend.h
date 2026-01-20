#ifndef GtRBackendH
#define GtRBackendH

#ifdef __cplusplus 
    extern "C" {
#endif

        #include <GONTI/GONTI-ENGINE/GONTI.RENDER.VK/Source/Types/GtVkContextTypes.inl>
        #include "../Types/GtRTypes.inl"

        struct GtVkPlatformState;

        GTAPI GtB8 gontiRendererBackendCreate(GontiRendererBackendType type, struct GtVkPlatformState* platState, GontiRendererBackend* outRendererBackend);
        GTAPI void gontiRendererBackendDestroy(GontiRendererBackend* rendererBackend);

        #endif

#ifdef __cplusplus 
    }
#endif