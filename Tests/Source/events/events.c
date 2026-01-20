#include "events.h"

static GtB8 isPaused = GtFalse;

GtB8 applicationOnEvent(GtU16 code, void* sender, void* listenerInst, GtEventContext context) {
    switch (code) {
        case GT_EVENT_CODE_APPLICATION_QUIT: {
            GTINFO("EVENT_CODE_APPLICATION_QUIT recived, shutting down.\n");
            gontiVkApplicationShutDown();

            return GtTrue;
        }
        case GT_EVENT_CODE_APPLICATION_PAUSE: {
            GTINFO("EVENT_CODE_APPLICATION_PAUSE recived, paused\n");
            gontiVkApplicationPause();

            return GtTrue;
        }
        case GT_EVENT_CODE_APPLICATION_RESUME: {
            GTINFO("EVENT_CODE_APPLICATION_PAUSE recived, resumed\n");
            gontiVkApplicationResume();

            return GtTrue;
        }
    }

    return GtFalse;
}
GtB8 applicationOnKey(GtU16 code, void* sender, void* listenerInst, GtEventContext context) {
    if (code == GT_EVENT_CODE_KEY_PRESSED) {
        GtU16 keyCode = context.data.GtU16[0];

        if (keyCode == GT_KEY_ESCAPE) {
            if (!isPaused) {
                GtEventContext data = {};
                gontiEventFire(GT_EVENT_CODE_APPLICATION_PAUSE, 0, data);
                isPaused = GtTrue;
            } else if (isPaused) {
                GtEventContext data = {};
                gontiEventFire(GT_EVENT_CODE_APPLICATION_RESUME, 0, data);
                isPaused = GtFalse;
            }
            
            return GtTrue;
        } else {
            GTDEBUG("\"%c\" KEY PRESSED", keyCode);
        }
    } else if (code == GT_EVENT_CODE_KEY_RELEASED) {
        GtU16 keyCode = context.data.GtU16[0];

        GTDEBUG("\"%c\" KEY RELEASED", keyCode);
    }

    return GtFalse;
}
GtB8 applicationOnButton(GtU16 code, void* sender, void* listenerInst, GtEventContext context) {
    if (code == GT_EVENT_CODE_BUTTON_PRESSED) {
        GtU16 buttonCode = context.data.GtU16[0];

        if (buttonCode == GT_BTN_LEFT) {
            GTDEBUG("LEFT BUTTON PRESSED");
        } else if (buttonCode == GT_BTN_MIDDLE) {
            GTDEBUG("MIDDLE BUTTON PRESSED");
        } else if (buttonCode == GT_BTN_RIGHT) {
            GTDEBUG("RIGHT BUTTON PRESSED");
        }
    } else if (code == GT_EVENT_CODE_BUTTON_RELEASED) {
        GtU16 buttonCode = context.data.GtU16[0];

        if (buttonCode == GT_BTN_LEFT) {
            GTDEBUG("LEFT BUTTON RELEASED");
        } else if (buttonCode == GT_BTN_MIDDLE) {
            GTDEBUG("MIDDLE BUTTON RELEASED");
        } else if (buttonCode == GT_BTN_RIGHT) {
            GTDEBUG("RIGHT BUTTON RELEASED");
        }
    }

    return GtFalse;
}
GtB8 applicationOnMouseWheel(GtU16 code, void* sender, void* listenerInst, GtEventContext context) {
    if (code == GT_EVENT_CODE_MOUSE_WHEEL) {
        GtU16 wheelCode = context.data.GtU16[0];

        if (wheelCode == 1) {
            GTDEBUG("WHEEL MOVED UP");
        } else if (wheelCode == 255) {
            GTDEBUG("WHEEL MOVED DOWN");
        }
    }

    return GtFalse;
}
GtB8 applicationOnMouseMove(GtU16 code, void* sender, void* listenerInst, GtEventContext context) {
    if (code == GT_EVENT_CODE_MOUSE_MOVED) {
        GtU16 mousePosX = context.data.GtU16[0];
        GtU16 mousePosY = context.data.GtU16[1];

        GTDEBUG("Mouse position X: %i Y: %i", mousePosX, mousePosY);
    }

    return GtFalse;
}
GtB8 applicationOnResized(GtU16 code, void* sender, void* listenerInst, GtEventContext context) {
    if (code == GT_EVENT_CODE_RESIZED) {
        GtU16 width = context.data.GtU16[0];
        GtU16 height = context.data.GtU16[1];

        if (width != gontiVkApplicationGetWidth() || height != gontiVkApplicationGetHeight()) {
            gontiVkApplicationSetWidth(width);
            gontiVkApplicationSetHeight(height);

            GTDEBUG("Window resized %i:%i", width, height);

            if (width == 0 || height == 0) {
                GTDEBUG("Window minimalized, suspending application");
                gontiVkApplicationPause();

                return GtTrue;
            } else {
                if (gontiVkApplicationIsPaused()) {
                    GTDEBUG("Window restored, resuming application");
                    gontiVkApplicationResume();
                }

                gontiVkApplicationOnResized(width, height);
                gontiRendererOnResized(width, height);
            }
        }
    }

    return GtFalse;
}