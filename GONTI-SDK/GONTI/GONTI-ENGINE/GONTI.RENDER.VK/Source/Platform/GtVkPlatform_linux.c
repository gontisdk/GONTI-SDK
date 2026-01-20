#include "GtVkPlatform.h"

#if GTPLATFORM_LINUX

#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Memory/GtMemory.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/GtLogger.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Containers/DynamicArray/GtDArray.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Platform/GtPlatform.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/String/GtString.h>

#include <xcb/xcb.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h> // sudo apt-get install libx11-dev
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h> // sudo apt-get install libxkbcommon-x11-dev
#include <sys/time.h>
#include <vulkan/vulkan.h>

#if _POSIX_C_SOURCE >= 199309L
    #include <time.h>
#else
    #include <unistd.h>
#endif

#include <stdlib.h>
#include <stdio.h>

#define VK_USE_PLATFORM_XCB_KHR

GtB8 gontiVkPlatformStartup(GtVkPlatformState* platState, const char* className, const char* appName, GtI32 x, GtI32 y, GtI32 width, GtI32 height) {
    platState->internalState = gt_allocate(sizeof(GtVkInternalStateLINUX), GT_MEM_TAG_WINDOW);
    platState->internalState = &platState->gontiVkInternalStateLinux;

    GtVkInternalStateLINUX* state = (GtVkInternalStateLINUX*)platState->internalState;
    state->display = XOpenDisplay(NULL);

    XAutoRepeatOff(state->display);

    GtB32 screenP = 0;
    state->connection = XGetXCBConnection(state->display);

    if (xcb_connection_has_error(state->connection)) {
        GTFATAL("Failed to connect to X server via XCB");
        return GtFalse;
    }

    const struct xcb_setup_t* setup = xcb_get_setup(state->connection);
    xcb_screen_iterator_t it = xcb_setup_roots_iterator(setup);

    for (GtI32 s = screenP; s > 0; s--) {
        xcb_screen_next(&it);
    }

    state->screen = it.data;
    state->window = xcb_generate_id(state->connection);

    GtU32 eventMask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    GtU32 eventValues = XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE | 
                        XCB_EVENT_MASK_GT_KEY_PRESS | XCB_EVENT_MASK_GT_KEY_RELEASE | 
                        XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_POINTER_MOTION | 
                        XCB_EVENT_MASK_STRUCTURE_NOTIFY;

    GtU32 valueList[] = {state->screen->black_pixel, eventValues};

    xcb_void_cookie_t cookie = xcb_create_window(
        state->connection,
        XCB_COPY_FROM_PARENT,
        state->window,
        state->screen->root,
        x,
        y,
        width,
        height,
        0,
        XCB_WINDOW_CLASS_INPUT_OUTPUT,
        state->screen->root_visual,
        eventMask,
        valueList
    );

    xcb_change_property(
        state->connection,
        XCB_PROP_MODE_REPLACE,
        state->window,
        XCB_ATOM_WM_NAME,
        XCB_ATOM_STRING,
        8,
        gontiStringLength(appName),
        appName
    );

    xcb_intern_atom_cookie_t wmDeleteCookie = xcb_intern_atom(
        state->connection,
        0,
        gontiStringLength("WM_DELETE_WINDOW"),
        "WM_DELETE_WINDOW"
    );

    xcb_intern_atom_cookie_t wmProtocolsCookie = xcb_intern_atom(
        state->connection,
        0,
        gontiStringLength("WM_PROTOCOLS"),
        "WM_PROTOCOLS"
    );

    xcb_intern_atom_reply_t* wmDeleteReply = xcb_intern_atom_reply(
        state->connection,
        wmDeleteCookie,
        NULL
    );

    xcb_intern_atom_reply_t* wmProtocolsReply = xcb_intern_atom_reply(
        state->connection,
        wmProtocolsCookie,
        NULL
    );

    state->wmDeleteWin = wmDeleteReply->atom;
    state->wmProtocols = wmProtocolsReply->atom;

    xcb_change_property(
        state->connection,
        XCB_PROP_MODE_REPLACE,
        state->window,
        wmProtocolsReply->atom,
        4,
        32,
        1,
        &wmDeleteReply->atom
    );

    xcb_map_window(state->connection, state->window);

    GtI32 streamResult = xcb_flush(state->connection);
    if (streamResult <= 0) {
        GTFATAL("An error occurred when flushing the stream: %d", streamResult);
        return GtFalse;
    }

    return GtTrue;
}
GtB8 gontiVkPlatformPumpMessage(GtVkPlatformState* platState) {
    GtVkInternalStateLINUX* state = (GtVkInternalStateLINUX*)platState->internalState;

    if (state->linux_process_message == NULL) {
        GTFATAL("linux_process_message must be set.");
        gontiVkPlatformShutdown();
        return GtFalse;
    }

    GtB8 quitFlagged = GtFalse;

    while ((state->event = xcb_poll_for_event(state->connection)) != NULL) {
        if ((state->event->response_type & ~0x80) == XCB_CLIENT_MESSAGE) {
            state->clientMessage = (xcb_client_message_event_t*)state->event;
        } else {
            state->clientMessage = NULL;
        }

        if (!state->linux_process_message(state)) {
            quitFlagged = GtTrue;
        }

        free(state->event);
    }

    return !quitFlagged;
}

GtB8 gontiVkPlatformCreateVulkanSurface(GtVkPlatformState* platState, GtVkContext* context) {
    GtVkInternalStateLINUX* state = (GtVkInternalStateLINUX*)platState->internalState;

    VkXcbSurfaceCreateInfoKHR createInfo = {VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR};
    createInfo.connection = state->connection;
    createInfo.window = state->window;

    VkResult result = vkCreateXcbSurfaceKHR(
        context->instance,
        &createInfo,
        context->allocator,
        &platState->vkInternalState->surface
    ); if (result != VK_SUCCESS) {
        GTFATAL("Vulkan surface creation failed!");
        return GtFalse;
    }

    context->surface = platState->vkInternalState->surface;
    return GtTrue;
}

void gontiVkPlatformShutdown(GtVkPlatformState* platState) {
    GtVkInternalStateLINUX* state = (GtVkInternalStateLINUX*)platState->internalState;

    XAutoRepeatOn(state->display);
    xcb_destroy_window(state->connection, state->window);

    gt_free(platState->internalState);
}

void gontiVkPlatformGetRequiredExtensionNames(const char*** namesDarray) {
    gontiDarrayPush(*namesDarray, &"VK_KHR_xcb_surface"); // "VK_KHR_xlib_surface" or "VK_KHR_xcb_surface"?
}

#endif