#ifndef VULKANDEVICETYPES_INL
#define VULKANDEVICETYPES_INL

#ifdef __cplusplus
    extern "C" {
#endif
        
        #include "VulkanLogicalDeviceTypes.inl"
        #include "VulkanPhysicalDeviceTypes.inl"
        #include "../../RendererSubsystem/Types/VulkanSwapchainTypes.inl"

        typedef struct GontiVulkanDeviceInfo{
            VkPhysicalDeviceProperties properties;
            VkPhysicalDeviceFeatures features;
            VkPhysicalDeviceMemoryProperties memory;
        }GontiVulkanDeviceInfo;

        typedef struct GontiVulkanDevice {
            VkPhysicalDevice physicalDevice;
            VkDevice logicalDevice;

            GontiVulkanSwapchainSupportInfo swapchainSupport;
            GontiVulkanLogicalDevice vkLdDevice;
            GontiVulkanPhysicalDeviceList vkDevices;
            GontiVulkanDeviceInfo* vkDeviceInfo;

            i32 graphicsQueueIndex;
            i32 presentQueueIndex;
            i32 transferQueueIndex;
            i32 computeQueueIndex;

            VkQueue graphicsQueue;
            VkQueue presentQueue;
            VkQueue transferQueue;
            VkQueue computeQueue;

            VkCommandPool graphicsCommandPool;

            VkPhysicalDeviceProperties properties;
            VkPhysicalDeviceFeatures features;
            VkPhysicalDeviceMemoryProperties memory;
            VkFormat depthFormat;
        }GontiVulkanDevice;

#ifdef __cplusplus
    }
#endif

#endif