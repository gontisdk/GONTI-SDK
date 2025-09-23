#ifndef VULKANFRAMEBUFFER_H
#define VULKANFRAMEBUFFER_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include "../../Types/VulkanContextTypes.inl"

        KAPI void gontiVkFramebufferCreate(
            GontiVulkanContext* context,
            GontiVulkanRenderpass* renderpass,
            u32 width, u32 height,
            u32 attachmentCount,
            VkImageView* attachments,
            GontiVulkanFramebuffer* outFramebuffer
        );
        KAPI void gontiVkFramebufferDestroy(GontiVulkanContext* context, GontiVulkanFramebuffer* framebuffer);

#ifdef __cplusplus
    }
#endif

#endif