#ifndef VULKANCONTEXTTYPES_INL
#define VULKANCONTEXTTYPES_INL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include <vulkan/vulkan.h>
        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/Defines.h>
        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Asserts/Asserts.h>

        #include "../Hardware/Types/VulkanDeviceTypes.inl"
        #include "../Hardware/Types/VulkanLogicalDeviceTypes.inl"
        #include "../Hardware/Types/VulkanPhysicalDeviceTypes.inl"

        #include "../RendererSubsystem/Types/VulkanCommandBufferTypes.inl"
        #include "../RendererSubsystem/Types/VulkanFenceTypes.inl"
        #include "../RendererSubsystem/Types/VulkanFrameBufferTypes.inl"
        #include "../RendererSubsystem/Types/VulkanImageTypes.inl"
        #include "../RendererSubsystem/Types/VulkanRenderpassTypes.inl"
        #include "../RendererSubsystem/Types/VulkanSwapchainTypes.inl"

        #define VK_CHECK(expr) { \
            KASSERT(expr == VK_SUCCESS); \
        }

        typedef struct GontiVulkanContext {
            VkInstance instance;
            VkAllocationCallbacks* allocator;
            VkSurfaceKHR surface;

            VkSemaphore* imageAvailableSemaphores;
            VkSemaphore* queueCompleteSemaphore;

            GontiVulkanDevice device;
            GontiVulkanSwapchain swapchain;
            GontiVulkanRenderpass mainRenderpass;
            GontiVulkanCommandBuffer* graphicsCommandBuffers;
            GontiVulkanFence* inFlightFences;
            GontiVulkanFence** imagesInFlight;

            u32 framebufferWidth;
            u32 framebufferHeight;
            u32 imageIndex;
            u32 currentFrame;
            u32 inFlightFenceCount;

            u64 framebufferSizeGeneration;
            u64 framebufferSizeLastGeneration;

            i32 (*findMemoryIndex)(u32 typeFilter, u32 propertyFlags);

            #if defined(_DEBUG) 
                VkDebugUtilsMessengerEXT debugMessenger;
            #endif
        } GontiVulkanContext;

#ifdef __cplusplus 
    }
#endif

#endif
