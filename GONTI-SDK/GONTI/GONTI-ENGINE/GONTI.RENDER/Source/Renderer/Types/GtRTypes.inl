#ifndef GtRTypesINL
#define GtRTypesINL

#ifdef __cplusplus 
    extern "C" {
#endif

        #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.h>

        typedef enum GontiRendererBackendType {
            RENDERER_BACKEND_TYPE_VULKAN,

            RENDERER_BACKEND_TYPE_OPENGL,

            RENDERER_BACKEND_TYPE_DIRECTX_11,
            RENDERER_BACKEND_TYPE_DIRECTX_12
        }GontiRendererBackendType;

        typedef struct GontiRendererBackend {
            struct GtVkPlatformState* GtVkPlatformState;
            GtU64 frameNumber;
            GtB8 (*initialize)(const char* appName, struct GtVkPlatformState* platState);
            void (*shutdown)();
            void (*resized)(GtU16 width, GtU16 height);
            GtB8 (*beginFrame)(GtF32 deltaTime);
            GtB8 (*endFrame)(GtF32 deltaTime);
        }GontiRendererBackend;

        typedef struct GontiRendererPacket {
            GtF32 deltaTime;
        }GontiRendererPacket;

#ifdef __cplusplus 
    }
#endif

#endif