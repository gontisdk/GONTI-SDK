#ifndef VULKANDEVICE_H
#define VULKANDEVICE_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/Defines.h>
        #include "../Types/VulkanDeviceTypes.inl"
        #include "../../Types/VulkanContextTypes.inl"

        /*B8*/
        KAPI b8 gontiVkDeviceCreate(struct GontiVulkanContext* context);
        KAPI b8 gontiVkDeviceDetectDepthFormat(struct GontiVulkanContext* context);

        /*VOID*/
        KAPI void gontiVkDeviceRelease(struct GontiVulkanContext* context);
        KAPI void gontiVkDeviceQuerySwapchainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, GontiVulkanSwapchainSupportInfo* outSupportInfo);

#ifdef __cplusplus
    }
#endif

#endif