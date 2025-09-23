#ifndef VULKANFENCETYPES_INL
#define VULKANFENCETYPES_INL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include <vulkan/vulkan.h>
        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/Defines.h>

        typedef struct GontiVulkanFence {
            VkFence handle;
            b8 isSignaled;
        }GontiVulkanFence;

#ifdef __cplusplus 
    }
#endif

#endif