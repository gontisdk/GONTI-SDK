#ifndef VULKANSYNCOBJECTS_H
#define VULKANSYNCOBJECTS_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../../Types/VulkanContextTypes.inl"

        KAPI void gontiVkSyncObjectsCreate(GontiVulkanContext* context);
        KAPI void gontiVkSyncObjectsDestroy(GontiVulkanContext* context);

#ifdef __cplusplus
    }
#endif

#endif