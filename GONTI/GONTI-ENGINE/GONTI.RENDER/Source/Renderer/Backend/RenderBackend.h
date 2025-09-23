#ifndef RENDERERBACKEND_H
#define RENDERERBACKEND_H

#ifdef __cplusplus 
    extern "C" {
#endif

        #include <GONTI/GONTI-ENGINE/GONTI.RENDER.VK/Source/Types/VulkanContextTypes.inl>
        #include "../Types/RenderTypes.inl"

        struct GontiVulkanPlatformState;

        KAPI b8 gontiRendererBackendCreate(GontiRendererBackendType type, struct GontiVulkanPlatformState* platState, GontiRendererBackend* outRendererBackend);
        KAPI void gontiRendererBackendDestroy(GontiRendererBackend* rendererBackend);

        #endif

#ifdef __cplusplus 
    }
#endif