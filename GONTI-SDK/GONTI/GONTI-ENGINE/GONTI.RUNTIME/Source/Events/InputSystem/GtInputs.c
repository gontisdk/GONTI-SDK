#include "GtInputs.h"

#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Memory/GtMemory.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/GtLogger.h>
#include "../GtEvents.h"

typedef struct GtKeyboardState {
    GtB8 keys[256];
}GtKeyboardState;

typedef struct GtMouseState {
    GtI16 x;
    GtI16 y;
    GtB8 buttons[BUTTON_MAX_GT_BTNS];
}GtMouseState;

typedef struct GtInputState {
    GtKeyboardState keyboardCurrent;
    GtKeyboardState keyboardPrevious;
    GtMouseState mouseCurrent;
    GtMouseState mousePrevious;
}GtInputState;

static GtB8 initialized;
static GtInputState state;

void gontiInputInitialize() {
    gt_zeroMemory(&state, sizeof(GtInputState));
    initialized = GtTrue;
    GTINFO("Input subsystem initialized");
}
void gontiInputShutdown() {
    initialized = GtFalse;
}
void gontiInputUpdate(GtF64 deltaTime) {
    if (!initialized) return;

    gt_copyMemory(&state.keyboardPrevious, &state.keyboardCurrent, sizeof(GtKeyboardState));
    gt_copyMemory(&state.mousePrevious, &state.mouseCurrent, sizeof(GtMouseState));
}
void gontiInputProcessKey(GtInputKeyboardKeys key, GtB8 pressed) {
    if (state.keyboardCurrent.keys[key] != pressed) {
        state.keyboardCurrent.keys[key] = pressed;

        GtEventContext context;
        context.data.GtU16[0] = key;
        gontiEventFire(pressed ? GT_EVENT_CODE_KEY_PRESSED : GT_EVENT_CODE_KEY_RELEASED, 0, context);
    }
}
void gontiInputProcessButton(GtInputMouseBtns button, GtB8 pressed) {
    if (state.mouseCurrent.buttons[button] != pressed) {
        state.mouseCurrent.buttons[button] = pressed;

        GtEventContext context;
        context.data.GtU16[0] = button;
        gontiEventFire(pressed ? GT_EVENT_CODE_BUTTON_PRESSED : GT_EVENT_CODE_BUTTON_RELEASED, 0, context);
    }
}
void gontiInputProcessMouseMove(GtI16 x, GtI16 y) {
    if (state.mouseCurrent.x != x || state.mouseCurrent.y != y) {
        state.mouseCurrent.x = x;
        state.mouseCurrent.y = y;

        GtEventContext context;
        context.data.GtU16[0] = x;
        context.data.GtU16[1] = y;
        gontiEventFire(GT_EVENT_CODE_MOUSE_MOVED, 0, context);
    }
}
void gontiInputProcessMouseWheel(GtI8 zDelta) {
    GtEventContext context;
    context.data.GtU8[0] = zDelta;
    gontiEventFire(GT_EVENT_CODE_MOUSE_WHEEL, 0, context);
}
void gontiVkInputGetMousePosition(GtI32* x, GtI32* y) {
    if (!initialized) {
        *x = 0;
        *y = 0;
        return;
    }

    *x = state.mouseCurrent.x;
    *y = state.mouseCurrent.y;
}
void gontiInputGetPreviousMousePosition(GtI32* x, GtI32* y) {
    if (!initialized) {
        *x = 0;
        *y = 0;
        return;
    }

    *x = state.mousePrevious.x;
    *y = state.mousePrevious.y;
}

GtB8 gontiInputIsKeyDown(GtInputKeyboardKeys key) {
    if (!initialized) return GtFalse;
    return state.keyboardCurrent.keys[key] == GtTrue;
}
GtB8 gontiInputIsKeyUp(GtInputKeyboardKeys key) {
    if (!initialized) return GtTrue;
    return state.keyboardCurrent.keys[key] == GtFalse;
}
GtB8 gontiInputWasKeyDown(GtInputKeyboardKeys key) {
    if (!initialized) return GtFalse;
    return state.keyboardPrevious.keys[key] == GtTrue;
}
GtB8 gontiInputWasKeyUp(GtInputKeyboardKeys key) {
    if (!initialized) return GtTrue;
    return state.keyboardPrevious.keys[key] == GtFalse;
}
GtB8 gontiInputIsButtonDown(GtInputMouseBtns button) {
    if (!initialized) return GtFalse;
    return state.mouseCurrent.buttons[button] == GtTrue;
}
GtB8 gontiInputIsButtonUp(GtInputMouseBtns button) {
    if (!initialized) return GtTrue;
    return state.mouseCurrent.buttons[button] == GtFalse;
}
GtB8 gontiInputWasButtonDown(GtInputMouseBtns button) {
    if (!initialized) return GtFalse;
    return state.mousePrevious.buttons[button] == GtTrue;
}
GtB8 gontiInputWasButtonUp(GtInputMouseBtns button) {
    if (!initialized) return GtTrue;
    return state.mousePrevious.buttons[button] == GtFalse;
}