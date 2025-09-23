#ifndef VULKANFENCE_H
#define VULKANFENCE_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/Defines.h>
        #include "../../Types/VulkanContextTypes.inl"

        KAPI b8 gontiVkFenceWait(
            GontiVulkanContext* context,
            GontiVulkanFence* fence,
            u64 timeoutNs
        );

        KAPI void gontiVkFenceCreate(
            GontiVulkanContext* context,
            b8 createSignaled,
            GontiVulkanFence* outFence
        );
        KAPI void gontiVkFenceDestroy(GontiVulkanContext* context, GontiVulkanFence* fence);
        KAPI void gontiVkFenceReset(GontiVulkanContext* context, GontiVulkanFence* fence);

#ifdef __cplusplus
    }
#endif

#endif