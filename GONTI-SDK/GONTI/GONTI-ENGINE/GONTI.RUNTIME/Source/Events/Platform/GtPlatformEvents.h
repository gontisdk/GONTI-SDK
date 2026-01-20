#ifndef GtPlatformEventsH
#define GtPlatformEventsH

#ifdef __cplusplus
    extern "C" {
#endif

        #include <GONTI/GONTI-ENGINE/GONTI.RENDER/Source/Config/SetupGraphicBackend/GtUsrSetGraphBackend.h>

        #if GONTI_USE_VULKAN

            #include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.h>
            #include <GONTI/GONTI-ENGINE/GONTI.RENDER.VK/Source/Platform/GtVkPlatform.h>
            #include "../InputSystem/GtInputs.h"

            #if GTPLATFORM_WINDOWS
                #include <Windows.h>
                
                LRESULT CALLBACK WndProc(HWND hwnd, GtU32 msg, WPARAM wParam, LPARAM lParam);
            #endif

            #if GTPLATFORM_LINUX
                GtB8 LinuxProc(GtVkInternalStateLINUX* state);
                GTAPI GtInputKeyboardKeys translateKeycode(GtU32 xKeyCode);
            #endif

        #endif

#ifdef __cplusplus
    }
#endif

#endif