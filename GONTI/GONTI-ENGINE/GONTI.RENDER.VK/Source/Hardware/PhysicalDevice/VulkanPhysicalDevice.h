#ifndef VULKANPHYSICALDEVICE_H
#define VULKANPHYSICALDEVICE_H

#ifdef __cplusplus
    extern "C" {
#endif
        
        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/Defines.h>

        #include "../Types/VulkanPhysicalDeviceTypes.inl"
        #include "../../Types/VulkanContextTypes.inl"

        extern const GontiVulkanPhysicalDeviceList ZERO_PHYSICAL_DEVICES;

        KAPI b8 gontiVkSelectPhysicalDevice(GontiVulkanContext* context);
        KAPI b8 gontiVkFindPhysicalDevices(GontiVulkanContext* context);
        KAPI b8 gontiVkPhysicalDeviceMeetsRequirements(
            VkPhysicalDevice device,
            VkSurfaceKHR surface,
            const VkPhysicalDeviceProperties* properties,
            const VkPhysicalDeviceFeatures* features,
            const GontiVulkanPhysicalDeviceRequirements* requirements,
            GontiVulkanPhysicalDeviceQueueFamilyInfo* outQueueInfo,
            GontiVulkanSwapchainSupportInfo* outSwapchainSupport
        );
        KAPI b8 gontiVkNumeratePhysicalDevices(GontiVulkanContext* context);

        KAPI void gontiVkPhysicalDeviceRelease(GontiVulkanContext* context);

#ifdef __cplusplus
    }
#endif

#endif