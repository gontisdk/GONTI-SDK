#ifndef GtVkContextTypesINL
#define GtVkContextTypesINL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include <vulkan/vulkan.h>
        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.h>
        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Asserts/GtAsserts.h>

        #include "../Hardware/Types/GtVkDeviceTypes.inl"
        #include "../Hardware/Types/GtVkLDeviceTypes.inl"
        #include "../Hardware/Types/GtVkPDeviceTypes.inl"

        #include "../RendererSubsystem/Types/GtVkCmdBufferTypes.inl"
        #include "../RendererSubsystem/Types/GtVkFenceTypes.inl"
        #include "../RendererSubsystem/Types/GtVkFrameBufferTypes.inl"
        #include "../RendererSubsystem/Types/GtVkImgTypes.inl"
        #include "../RendererSubsystem/Types/GtVkRenderpassTypes.inl"
        #include "../RendererSubsystem/Types/GtVkSwapchainTypes.inl"

        #define GTVK_CHECK(expr) { \
            GTASSERT(expr == VK_SUCCESS); \
        }

        typedef struct GtVkContext {
            VkInstance instance;
            VkAllocationCallbacks* allocator;
            VkSurfaceKHR surface;

            VkSemaphore* imageAvailableSemaphores;
            VkSemaphore* queueCompleteSemaphore;

            GtVkDevice device;
            GtVkSwapchain swapchain;
            GtVkRenderpass mainRenderpass;
            GtVkCmdBuffer* graphicsCommandBuffers;
            GtVkFence* inFlightFences;
            GtVkFence** imagesInFlight;

            GtU32 framebufferWidth;
            GtU32 framebufferHeight;
            GtU32 imageIndex;
            GtU32 currentFrame;
            GtU32 inFlightFenceCount;

            GtU64 framebufferSizeGeneration;
            GtU64 framebufferSizeLastGeneration;

            GtI32 (*gontiVkFindMemoryIndex)(GtU32 typeFilter, GtU32 propertyFlags);

            #if defined(_DEBUG) 
                VkDebugUtilsMessengerEXT debugMessenger;
            #endif
        } GtVkContext;

#ifdef __cplusplus 
    }
#endif

#endif
