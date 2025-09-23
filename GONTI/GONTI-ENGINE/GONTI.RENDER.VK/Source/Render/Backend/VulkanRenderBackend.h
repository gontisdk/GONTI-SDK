#ifndef VULKANRENDERERBACKEND_H
#define VULKANRENDERERBACKEND_H

#ifdef __cplusplus 
    extern "C" {
#endif

        #include "../../Types/VulkanContextTypes.inl"
        #include "../../Platform/VulkanPlatform.h"

        //#include <stdint.h>
        #ifdef UINT64_MAX
            #undef UINT64_MAX
        #endif
        #ifndef UINT64_MAX
            #define UINT64_MAX 18446744073709551615ULL 
        #endif
    
        KAPI b8 gontiVkRendererBackendInitialize(const char* appName, GontiVulkanPlatformState* platState);
        KAPI b8 gontiVkRendererBackendBeginFrame(f32 deltaTime);
        KAPI b8 gontiVkRendererBackendEndFrame(f32 deltaTime);

        KAPI void gontiVkRendererBackendShutdown();
        KAPI void gontiVkRendererBackendOnResized(u16 width, u16 height);

#ifdef __cplusplus 
    }
#endif

#endif