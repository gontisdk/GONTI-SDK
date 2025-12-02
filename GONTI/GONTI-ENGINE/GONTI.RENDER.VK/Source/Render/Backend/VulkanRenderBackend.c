#include "VulkanRenderBackend.h"

#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/Logger.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/String/String.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Containers/DynamicArray/DynamicArray.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Platform/Platform.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Memory/Memory.h>
#include "../../Types/VulkanContextTypes.inl"
#include "../../Platform/VulkanPlatform.h"
#include "../../Debugger/VulkanDebugger.h"
#include "../../Hardware/Device/VulkanDevice.h"
#include "../../RendererSubsystem/Swapchain/VulkanSwapchain.h"
#include "../../RendererSubsystem/Renderpass/VulkanRenderpass.h"
#include "../../RendererSubsystem/CommandBuffer/VulkanCommandBuffer.h"
#include "../../RendererSubsystem/FrameBuffer/VulkanFrameBuffer.h"
#include "../../RendererSubsystem/SyncObjects/VulkanSyncObjects.h"
#include "../../RendererSubsystem/Fence/VulkanFence.h"
#include "../../Util/VulkanUtil.h"

static GontiVulkanContext context;

/* === [{ VV }==[TEMP VERBS]==[{ VV }] === */

static u32 cachedFramebufferWidth = 0;
static u32 cachedFramebufferHeight = 0;

/* === [{ ^^ }]==[TEMP VERBS]==[{ ^^ }] === */

/*PRIVATE FUNCS*/

i32 gontiVkFindMemoryIndex(u32 typeFilter, u32 propertyFlags) {
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(context.device.physicalDevice, &memoryProperties);

    for (u32 i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if (typeFilter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & propertyFlags) == propertyFlags) {
            return i;
        }
    }

    KWARN("Unable to find suitable memory type!");
    return -1;
}

void gontiVkCreateCommandBuffers() {
    if (!context.graphicsCommandBuffers) {
        context.graphicsCommandBuffers = gontiDarrayReserve(GontiVulkanCommandBuffer, context.swapchain.imageCount);

        for (u32 i = 0; i < context.swapchain.imageCount; i++) {
            k_zeroMemory(&context.graphicsCommandBuffers[i], sizeof(GontiVulkanCommandBuffer));
        }
    }

    for (u32 i = 0; i < context.swapchain.imageCount; i++) {
        if (context.graphicsCommandBuffers[i].handle) {
            gontiVkCommandBufferFree(
                &context,
                context.device.graphicsCommandPool,
                &context.graphicsCommandBuffers[i]
            );
        }
        
        gontiVkCommandBufferAllocate(
            &context,
            context.device.graphicsCommandPool,
            true,
            &context.graphicsCommandBuffers[i]
        );
    }

    KINFO("Vulkan command buffers created");
}

void gontiVkRegenerateFramebuffers(GontiVulkanSwapchain* swapchain, GontiVulkanRenderpass* renderpass) {
    for (u32 i = 0; i < swapchain->imageCount; i++) {
        KINFO("Vulkan creating framebuffers...");

        // TODO: make this dynamic based on the currently configured attachments
        u32 attachmentsCount = 2;
        VkImageView attachments[] = {
            swapchain->views[i],
            swapchain->depthAttachment.view
        };

        gontiVkFramebufferCreate(
            &context,
            renderpass,
            context.framebufferWidth,
            context.framebufferHeight,
            attachmentsCount,
            attachments,
            &context.swapchain.framebuffers[i]
        );
    }
}

b8 gontiVkRecreateSwapchain() {
    if (context.swapchain.recreatingSwapchain) {
        KDEBUG("gontiVkRecreateSwapchain called when already recreating. Booting...");
        return false;
    } 

    if (context.framebufferWidth == 0 || context.framebufferHeight == 0) {
        KDEBUG("gontiVkRecreateSwapchain called when window < 1 in a dimension. Booting...");
        return false;
    }

    context.swapchain.recreatingSwapchain = true;

    vkDeviceWaitIdle(context.device.logicalDevice);

    for (u32 i = 0; i < context.swapchain.imageCount; i++) {
        context.imagesInFlight[i] = 0;
    }

    gontiVkDeviceQuerySwapchainSupport(
        context.device.physicalDevice,
        context.surface,
        &context.device.swapchainSupport
    );
    gontiVkDeviceDetectDepthFormat(&context);
    gontiVkSwapchainRecreate(
        &context,
        cachedFramebufferWidth,
        cachedFramebufferHeight,
        &context.swapchain
    );

    context.framebufferWidth = cachedFramebufferWidth;
    context.framebufferHeight = cachedFramebufferHeight;
    context.mainRenderpass.w = context.framebufferWidth;
    context.mainRenderpass.h = context.framebufferHeight;
    cachedFramebufferWidth = 0;
    cachedFramebufferHeight = 0;

    context.framebufferSizeLastGeneration = context.framebufferSizeGeneration;

    for (u32 i = 0; i < context.swapchain.imageCount; i++) {
        gontiVkCommandBufferFree(&context, context.device.graphicsCommandPool, &context.graphicsCommandBuffers[i]);
    }

    for (u32 i = 0; i < context.swapchain.imageCount; i++) {
        gontiVkFramebufferDestroy(&context, &context.swapchain.framebuffers[i]);
    }

    context.mainRenderpass.x = 0;
    context.mainRenderpass.y = 0;
    context.mainRenderpass.w = context.framebufferWidth;
    context.mainRenderpass.h = context.framebufferHeight;

    gontiVkRegenerateFramebuffers(&context.swapchain, &context.mainRenderpass);
    gontiVkCreateCommandBuffers();

    context.swapchain.recreatingSwapchain = false;

    return true;
}

/*PUBLIC FUNCS*/

b8 gontiVkRendererBackendInitialize(const char* appName, struct GontiVulkanPlatformState* platState) {
    k_zeroMemory(&context, sizeof(GontiVulkanContext));
    
    context.gontiVkFindMemoryIndex = gontiVkFindMemoryIndex;
    context.allocator = 0; // TODO: custom allocator

    if (!platState->get_frame_buffer_size_ptr) {
        KFATAL("get_frame_buffer_size_ptr is NULL ptr");
        return false;
    }

    platState->get_frame_buffer_size_ptr(&cachedFramebufferWidth, &cachedFramebufferHeight);
    context.framebufferWidth = (cachedFramebufferWidth != 0) ? cachedFramebufferWidth : 800;
    context.framebufferHeight = (cachedFramebufferHeight != 0) ? cachedFramebufferHeight : 600;
    cachedFramebufferWidth = 0;
    cachedFramebufferHeight = 0;

    VkApplicationInfo appInfo = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
    appInfo.apiVersion = VK_API_VERSION_1_2;
    appInfo.pApplicationName = appName;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "GONTI ENGINE RENDER";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

    VkInstanceCreateInfo createInfo = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    createInfo.pApplicationInfo = &appInfo;

    const char** requiredExtensions = gontiDarrayCreate(const char*);
    gontiDarrayPush(requiredExtensions, &VK_KHR_SURFACE_EXTENSION_NAME);
    gontiVkPlatformGetRequiredExtensionNames(&requiredExtensions);

    #if defined(_DEBUG)
        gontiDarrayPush(requiredExtensions, &VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        KDEBUG("Required extensions:");
        u32 length = gontiDarrayLength(requiredExtensions);
        for (u32 i = 0; i < length; i++) {
            KDEBUG(requiredExtensions[i]);
        }
    #endif

    createInfo.enabledExtensionCount = gontiDarrayLength(requiredExtensions);
    createInfo.ppEnabledExtensionNames = requiredExtensions;

    const char** requiredValidationLayerNames = 0;
    u32 requiredValidationLayerCount = 0;

    #if defined(_DEBUG)
        KDEBUG("Validation layers enabled. Enumerating...");

        requiredValidationLayerNames = gontiDarrayCreate(const char*);
        gontiDarrayPush(requiredValidationLayerNames, &"VK_LAYER_KHRONOS_validation");
        requiredValidationLayerCount = gontiDarrayLength(requiredValidationLayerNames);

        u32 availableLayerCount = 0;
        VK_CHECK(vkEnumerateInstanceLayerProperties(&availableLayerCount, 0));
        VkLayerProperties* availableLayers = gontiDarrayReserve(VkLayerProperties, availableLayerCount);
        VK_CHECK(vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayers));

        for (u32 i = 0; i < requiredValidationLayerCount; i++) {
            KDEBUG("Searching for layer: %s...", requiredValidationLayerNames[i]);
            b8 found = false;

            for (u32 j = 0; j < availableLayerCount; j++) {
                if (gontiStringEqual(requiredValidationLayerNames[i], availableLayers[j].layerName)) {
                    found = true;
                    KDEBUG("Found Successfully");
                    break;
                }
            }

            if (!found) {
                KFATAL("Required validation layers is missing: %s", requiredValidationLayerNames[i]);
                return false;
            }
        }

        KDEBUG("All requred validation layers are present.");
    #endif

    createInfo.enabledLayerCount = requiredValidationLayerCount;
    createInfo.ppEnabledLayerNames = requiredValidationLayerNames;

    VK_CHECK(vkCreateInstance(&createInfo, context.allocator, &context.instance));
    KINFO("Vulkan Instance created");

    #if defined(_DEBUG)
        KDEBUG("Creating Vulkan debugger...");

        u32 logSecurity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT 
                        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
                        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
        debugCreateInfo.messageSeverity = logSecurity;
        debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                                    | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                                    | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugCreateInfo.pfnUserCallback = __vkDebugCallback;
        debugCreateInfo.pUserData = 0;

        PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context.instance, "vkCreateDebugUtilsMessengerEXT");
        KASSERT_MSG(func, "Failed to create debug messenger!");
        VK_CHECK(func(context.instance, &debugCreateInfo, context.allocator, &context.debugMessenger));

        KDEBUG("Vulkan debugger created.");
    #endif

    KINFO("Creating Vulkan surface...");
    if (!gontiVkPlatformCreateVulkanSurface(platState, &context)) {
        KERROR("Failed to create Vulkan surface!");
        return false;
    } KINFO("Vulkan surface created.");

    if (!gontiVkDeviceCreate(&context)) {
        KERROR("Failed to create device!");
        return false;
    }

    KINFO("Creating Vulkan swapchain...");
    gontiVkSwapchainCreate(&context, context.framebufferWidth, context.framebufferHeight, &context.swapchain);

    if (context.swapchain.maxFramesInFlight == 0) {
        KFATAL("Swapchain maxFramesInFlight is 0 after creation!");
        return false;
    }
    
    KINFO("Creating Vulkan Sync Objects...");
    gontiVkSyncObjectsCreate(&context);

    KINFO("Creating Vulkan renderpass...");
    gontiVkRenderpassCreate(
        &context, &context.mainRenderpass,
        0, 0, context.framebufferWidth, context.framebufferHeight,
        0.0f, 0.0f, 0.2f, 1.0f, 
        1.0f, 
        0
    );

    context.swapchain.framebuffers = gontiDarrayReserve(GontiVulkanFramebuffer, context.swapchain.imageCount);
    gontiVkRegenerateFramebuffers(&context.swapchain, &context.mainRenderpass);

    KINFO("Creating Vulkan command buffers...");
    gontiVkCreateCommandBuffers();

    context.currentFrame = 0;

    for (u8 i = 0; i < context.swapchain.maxFramesInFlight; i++) {
        if (context.inFlightFences[i].handle == VK_NULL_HANDLE) {
            KFATAL("Failed to create in-flight fence %d", i);
            return false;
        }
        if (context.imageAvailableSemaphores[i] == VK_NULL_HANDLE) {
            KFATAL("Failed to create image available semaphore %d", i);
            return false;
        }
        if (context.queueCompleteSemaphore[i] == VK_NULL_HANDLE) {
            KFATAL("Failed to create queue complete semaphore %d", i);
            return false;
        }
        KDEBUG("Sync objects %d created successfully - Fence: %p, ImageSem: %p, QueueSem: %p", 
            i, (void*)context.inFlightFences[i].handle, 
            (void*)context.imageAvailableSemaphores[i], 
            (void*)context.queueCompleteSemaphore[i]);
    }

    KINFO("Vulkan renderer initialized successfully");
    return true;
}
b8 gontiVkRendererBackendBeginFrame(f32 deltaTime) {
    GontiVulkanDevice* device = &context.device;

    if (context.currentFrame >= context.swapchain.maxFramesInFlight) {
        KERROR("currentFrame (%d) is out of bounds (max: %d)", 
            context.currentFrame, context.swapchain.maxFramesInFlight);
        context.currentFrame = 0;
    }

    if (context.swapchain.recreatingSwapchain) {
        VkResult result = vkDeviceWaitIdle(device->logicalDevice);
        if (!gontiVkUtilResultIsSuccess(result)) {
            KERROR("vkDeviceWaitIdle (1) failed: '%s'", gontiVkUtilResultToString(result, true));
            return false;
        }
        KINFO("Recreating swapchain, booting.");
        return false;
    }

    if (context.framebufferSizeGeneration != context.framebufferSizeLastGeneration) {
        VkResult result = vkDeviceWaitIdle(device->logicalDevice);
        if (!gontiVkUtilResultIsSuccess(result)) {
            KERROR("vkDeviceWaitIdle (2) failed: '%s'", gontiVkUtilResultToString(result, true));
            return false;
        }

        if (!gontiVkRecreateSwapchain()) {
            KERROR("Recreating swapchain failed.");
            return false;
        }
        KINFO("Resized, booting...");
        return false;
    }

    GontiVulkanFence* currentFence = &context.inFlightFences[context.currentFrame];
    if (!currentFence || currentFence->handle == VK_NULL_HANDLE) {
        KERROR("Current fence (%d) is invalid!", context.currentFrame);
        return false;
    }

    KDEBUG("BeginFrame: Using fence %d with handle %p", 
        context.currentFrame, (void*)currentFence->handle);

    if (!gontiVkFenceWait(&context, currentFence, 1000000000ULL)) {
        KWARN("In-flight fence wait failure!");
        return false;
    }

    gontiVkFenceReset(&context, currentFence);

    VkResult acquireResult = vkAcquireNextImageKHR(
        context.device.logicalDevice,
        context.swapchain.handle,
        UINT64_MAX,
        context.imageAvailableSemaphores[context.currentFrame],
        VK_NULL_HANDLE,
        &context.imageIndex
    );

    if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR) {
        KINFO("Swapchain out of date, recreating...");
        if (!gontiVkRecreateSwapchain()) {
            KERROR("Failed to recreate swapchain");
            return false;
        }
        return false;
    } else if (acquireResult != VK_SUCCESS && acquireResult != VK_SUBOPTIMAL_KHR) {
        KERROR("Failed to acquire swapchain image! Result: %d", acquireResult);
        return false;
    }

    if (context.imageIndex >= context.swapchain.imageCount) {
        KERROR("Invalid image index: %d (max: %d)", context.imageIndex, context.swapchain.imageCount);
        return false;
    }

    KDEBUG("Acquired image index: %d for frame %d", context.imageIndex, context.currentFrame);

    if (context.imagesInFlight[context.imageIndex] != 0) {
        if (!gontiVkFenceWait(&context, context.imagesInFlight[context.imageIndex], 1000000000ULL)) {
            KWARN("Image in flight fence wait failed");
        }
    }
    context.imagesInFlight[context.imageIndex] = currentFence;

    GontiVulkanCommandBuffer* commandBuffer = &context.graphicsCommandBuffers[context.imageIndex];
    if (!commandBuffer || commandBuffer->handle == VK_NULL_HANDLE) {
        KERROR("Invalid command buffer for image %d", context.imageIndex);
        return false;
    }

    gontiVkCommandBufferReset(commandBuffer);
    gontiVkCommandBufferBegin(commandBuffer, false, false, false);

    VkViewport viewport;
    viewport.x = 0.0f;
    viewport.y = (f32)context.framebufferHeight;
    viewport.width = (f32)context.framebufferWidth;
    viewport.height = -(f32)context.framebufferHeight;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor;
    scissor.offset.x = scissor.offset.y = 0;
    scissor.extent.width = context.framebufferWidth;
    scissor.extent.height = context.framebufferHeight;

    vkCmdSetViewport(commandBuffer->handle, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer->handle, 0, 1, &scissor);

    context.mainRenderpass.w = context.framebufferWidth;
    context.mainRenderpass.h = context.framebufferHeight;

    if (context.swapchain.framebuffers[context.imageIndex].handle == VK_NULL_HANDLE) {
        KERROR("Invalid framebuffer for image %d", context.imageIndex);
        return false;
    }

    gontiVkRenderpassBegin(
        commandBuffer,
        &context.mainRenderpass,
        context.swapchain.framebuffers[context.imageIndex].handle
    );

    return true;
}

b8 gontiVkRendererBackendEndFrame(f32 deltaTime) {
    if (context.imageIndex >= context.swapchain.imageCount) {
        KERROR("Invalid image index in EndFrame: %d", context.imageIndex);
        return false;
    }

    GontiVulkanCommandBuffer* commandBuffer = &context.graphicsCommandBuffers[context.imageIndex];
    if (!commandBuffer || commandBuffer->handle == VK_NULL_HANDLE) {
        KERROR("Invalid command buffer in EndFrame");
        return false;
    }

    gontiVkRenderpassEnd(commandBuffer, &context.mainRenderpass);
    gontiVkCommandBufferEnd(commandBuffer);

    if (context.currentFrame >= context.swapchain.maxFramesInFlight) {
        KERROR("Invalid currentFrame in EndFrame: %d", context.currentFrame);
        return false;
    }

    VkSubmitInfo submitInfo = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer->handle;
    
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &context.imageAvailableSemaphores[context.currentFrame];
    
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &context.queueCompleteSemaphore[context.imageIndex];

    VkPipelineStageFlags flags[1] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.pWaitDstStageMask = flags;

    GontiVulkanFence* currentFence = &context.inFlightFences[context.currentFrame];
    if (!currentFence || currentFence->handle == VK_NULL_HANDLE) {
        KERROR("Invalid fence for submit in EndFrame");
        return false;
    }

    KDEBUG("Submitting frame %d, image %d: wait_sem=%p, signal_sem=%p", 
        context.currentFrame, context.imageIndex,
        (void*)context.imageAvailableSemaphores[context.currentFrame],
        (void*)context.queueCompleteSemaphore[context.imageIndex]);

    VkResult result = vkQueueSubmit(
        context.device.graphicsQueue,
        1,
        &submitInfo,
        currentFence->handle
    );

    if (result != VK_SUCCESS) {
        KERROR("vkQueueSubmit failed with result: %s", gontiVkUtilResultToString(result, true));
        return false;
    }

    gontiVkCommandBufferUpdateSubmitted(commandBuffer);
    
    VkResult presentResult = gontiVkSwapchainPresent(
        &context,
        &context.swapchain,
        context.device.graphicsQueue,
        context.device.presentQueue,
        context.queueCompleteSemaphore[context.imageIndex],
        context.imageIndex
    );

    context.currentFrame = (context.currentFrame + 1) % context.swapchain.maxFramesInFlight;

    return presentResult == VK_SUCCESS || presentResult == VK_SUBOPTIMAL_KHR;
}

void gontiVkRendererBackendShutdown() {
    #if defined(_DEBUG)
    KDEBUG("Destroying Vulkan debugger...");
    if (context.debugMessenger) {
        PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context.instance, "vkDestroyDebugUtilsMessengerEXT");
        func(context.instance, context.debugMessenger, context.allocator);
    }
    #endif

    KINFO("Destroying Vulkan Sync Objects...");
    gontiVkSyncObjectsDestroy(&context);

    KINFO("Destroying Vulkan command buffers...");
    for (u32 i = 0; i < context.swapchain.imageCount; i++) {
        if (context.graphicsCommandBuffers[i].handle) {
            gontiVkCommandBufferFree(
                &context,
                context.device.graphicsCommandPool,
                &context.graphicsCommandBuffers[i]
            );
            context.graphicsCommandBuffers[i].handle = 0;
        }
    } gontiDarrayDestroy(context.graphicsCommandBuffers);
    context.graphicsCommandBuffers = 0;

    KINFO("Destroying Vulkan framebuffers...");
    for (u32 i = 0; i < context.swapchain.imageCount; i++) {
        gontiVkFramebufferDestroy(&context, &context.swapchain.framebuffers[i]);
    }

    KINFO("Destroying Vulkan renderpass...");
    gontiVkRenderpassDestroy(&context, &context.mainRenderpass);

    KINFO("Destroying Vulkan swapchain...");
    gontiVkSwapchainDestroy(&context, &context.swapchain);
    
    KINFO("Destroying handle to device...");
    gontiVkDeviceRelease(&context);
    
    KINFO("Destroying Vulkan surface...");
    if (context.surface) {
        vkDestroySurfaceKHR(context.instance, context.surface, context.allocator);
        context.surface = 0;
    }
    
    KINFO("Destroying Vulkan Instance...");
    vkDestroyInstance(context.instance, context.allocator);
    context.instance = 0;

    KINFO("All destroyed successfully");
}
void gontiVkRendererBackendOnResized(u16 width, u16 height) {
    cachedFramebufferWidth = width;
    cachedFramebufferHeight = height;
    context.framebufferSizeGeneration++;

    KINFO("Vulkan renderer backend->resized. Width: %i, Height: %i, Gen: %llu", width, height, context.framebufferSizeGeneration);
}
