#include "GtVkApp.h"

#if GONTI_USE_VULKAN

#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/GtLogger.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Memory/GtMemory.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Platform/GtPlatform.h>
#include <GONTI/GONTI-ENGINE/GONTI.RENDER.VK/Source/Platform/GtVkPlatform.h>
#include <GONTI/GONTI-ENGINE/GONTI.RENDER/Source/Renderer/Frontend/GtRFrontend.h>
#include <GONTI/GONTI-ENGINE/GONTI.RENDER.VK/Source/Render/Backend/GtVkRBackend.h>
#include "../../Clock/GtClock.h"
#include "../../Events/Platform/GtPlatformEvents.h"
#include "../../Events/GtEvents.h"
#include "../../Events/InputSystem/GtInputs.h"

typedef struct GtVkAppState {
    GtB8 isRunning;
    GtB8 isSuspended;
    GtVkPlatformState gontiPlatform;
    GtEntry* entryInst;
    GtI16 width;
    GtI16 height;
    Clock clock;
    GtF64 lastTime;
}GtVkAppState;

static GtB8 initialized = GtFalse;
static GtVkAppState appState;

/*
* B8
*/
GtB8 gontiVkApplicationCreate(GtEntry* entryInst) {
    if (initialized) {
        GTERROR("gontiVkApplicationCreate() method called more than once.");
        return GtFalse;
    }

    appState.entryInst = entryInst;

    gontiLoggerInitializeLogging();
    gontiInputInitialize();

    appState.isRunning = GtTrue;
    appState.isSuspended = GtFalse;

    if (!gontiEventInitialize()) {
        GTERROR("Event system failed initialization. Application can not continue");
        return GtFalse;
    }

    #if GTPLATFORM_WINDOWS
        appState.gontiPlatform.gontiVkInternalStateWin32.win32_process_message = WndProc;
    #endif
    #if GTPLATFORM_LINUX
        appState.gontiPlatform.gontiVkInternalStateLinux.linux_process_message = LinuxProc;
    #endif

    if (!appState.gontiPlatform.get_frame_buffer_size_ptr)
        appState.gontiPlatform.get_frame_buffer_size_ptr = gontiVkApplicationGetFramebufferSizePtr;

    if (!gontiVkPlatformStartup(
        &appState.gontiPlatform,
        entryInst->windowConfig.windowName,
        entryInst->windowConfig.className,
        entryInst->windowConfig.startPosX, 
        entryInst->windowConfig.startPosY,
        entryInst->windowConfig.startWidth, 
        entryInst->windowConfig.startHeight
    )) {
        GTFATAL("Window failed to create.");
        return GtFalse;
    }

    if (!gontiRendererInitialize(entryInst->windowConfig.windowName, &appState.gontiPlatform)) {
        GTFATAL("Failed to initialize renderer. Aborting gontiVkApplication...");
        return GtFalse;
    }

    if (!appState.entryInst->GtInitializeFN(appState.entryInst)) {
        GTFATAL("Game failed to initialize");
        return GtFalse;
    }

    if (appState.entryInst->GtOnResizeFN != NULL) appState.entryInst->GtOnResizeFN(appState.entryInst, appState.width, appState.height);

    initialized = GtTrue;
    return GtTrue;
}
GtB8 gontiVkApplicationRun(GtU64 fpsLimit, GtB8 isFpsLimit) {
    if (fpsLimit <= 0) {
        fpsLimit = 1;
        isFpsLimit = GtFalse;
    }

    gontiVkClockStart(&appState.clock);
    gontiVkClockUpdate(&appState.clock);
    appState.lastTime = appState.clock.elapsed;

    GtF64 runningTime = 0;
    GtU8 frameCount = 0;
    GtF64 targetFrameSeconds = 1.0f / fpsLimit;

    char* memUsage = gontiGetMemoryUsageStr();
    GTINFO(memUsage);
    gt_free(memUsage);

    while (appState.isRunning) {
        if (!gontiVkPlatformPumpMessage(&appState.gontiPlatform)) appState.isRunning = GtFalse;

        if (!appState.isSuspended) {
            gontiVkClockUpdate(&appState.clock);

            GtF64 currentTime = appState.clock.elapsed;
            GtF64 delta = (currentTime - appState.lastTime);
            GtF64 frameStartTime = gontiPlatformGetAbsoluteTime();

            if (!appState.entryInst->GtUpdateFN(appState.entryInst, (GtF32)delta)) {
                GTFATAL("Game update failed, shutting down...");
                appState.isRunning = GtFalse;
                break;
            }

            if (!appState.entryInst->GtRenderFN(appState.entryInst, (GtF32)delta)) {
                GTFATAL("Game render failed, shutting down...");
                appState.isRunning = GtFalse;
                break;
            }

            // TODO: refactor packet creation
            GontiRendererPacket packet;
            packet.deltaTime = delta;

            gontiRendererDrawFrame(&packet);

            GtF64 frameEndTime = gontiPlatformGetAbsoluteTime();
            GtF64 frameElapsedTime = frameEndTime - frameStartTime;

            runningTime += frameElapsedTime;
            GtF64 remainingSeconds = targetFrameSeconds - frameElapsedTime;

            if (remainingSeconds > 0) {
                GtU64 remainingMs = (remainingSeconds * 1000);

                if (remainingMs > 0 && isFpsLimit) {
                    gontiPlatformSleep(remainingMs - 1);
                }

                frameCount++;
            }

            gontiInputUpdate(delta);
            appState.lastTime = currentTime;
        }
    }

    appState.isRunning = GtFalse;

    if (appState.entryInst->GtShutdownFN) appState.entryInst->GtShutdownFN(appState.entryInst);
    gontiVkClockStop(&appState.clock);
    gontiEventShutdown();
    gontiInputShutdown();
    gontiRendererShutdown();
    gontiVkPlatformShutdown(&appState.gontiPlatform);
    gt_free(appState.entryInst->GtState);

    return GtTrue;
}

/*
* VOID
*/
void gontiVkApplicationShutDown() {
    if (appState.isRunning)
        appState.isRunning = GtFalse;
}
void gontiVkApplicationPause() {
    if (!appState.isSuspended)
        appState.isSuspended = GtTrue;
}
void gontiVkApplicationResume() {
    if (appState.isSuspended)
        appState.isSuspended = GtFalse;
}
void gontiVkApplicationGetFramebufferSizePtr(GtU32* width, GtU32* height) {
    *width = appState.width;
    *height = appState.height;
}
void gontiVkApplicationGetWidthPtr(GtU32* width) {
    *width = appState.width;
}
void gontiVkApplicationGetHeightPtr(GtU32* height) {
    *height = appState.height;
}
void gontiVkApplicationWidthSet(GtU32 width) {
    appState.width = width;
}
void gontiVkApplicationHeightSet(GtU32 height) {
    appState.height = height;
}
void gontiVkApplicationOnResized(GtU32 width, GtU32 height) {
    appState.entryInst->GtOnResizeFN(appState.entryInst, width, height);
}

/*
* GtI16
*/
GtI16 gontiVkApplicationGetWidth() {
    return appState.width;
}
GtI16 gontiVkApplicationGetHeight() {
    return appState.height;
}

/*
* GtB8
*/
GtB8 gontiVkApplicationIsPaused() {
    return appState.isSuspended;
}

#endif