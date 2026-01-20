#include "game.h"
#include "../events/events.h"

// SAMPLE USER GAME INIT FUNC
GtB8 gameInitialize(GtEntry* gameInst) {
    gontiEventRegister(GT_EVENT_CODE_APPLICATION_QUIT, 0, applicationOnEvent);
    gontiEventRegister(GT_EVENT_CODE_APPLICATION_PAUSE, 0, applicationOnEvent);
    gontiEventRegister(GT_EVENT_CODE_APPLICATION_RESUME, 0, applicationOnEvent);
    gontiEventRegister(GT_EVENT_CODE_KEY_PRESSED, 0, applicationOnKey);
    gontiEventRegister(GT_EVENT_CODE_KEY_RELEASED, 0, applicationOnKey);
    gontiEventRegister(GT_EVENT_CODE_BUTTON_PRESSED, 0, applicationOnButton);
    gontiEventRegister(GT_EVENT_CODE_BUTTON_RELEASED, 0, applicationOnButton);
    gontiEventRegister(GT_EVENT_CODE_MOUSE_WHEEL, 0, applicationOnMouseWheel);
    gontiEventRegister(GT_EVENT_CODE_MOUSE_MOVED, 0, applicationOnMouseMove);
    gontiEventRegister(GT_EVENT_CODE_RESIZED, 0, applicationOnResized);

    return GtTrue;
}
// SAMPLE USER UPDATE FUNC
GtB8 gameUpdate(GtEntry* gameInst, GtF32 deltaTime) {
    return GtTrue;
}
// SAMPLE USER RENDER FUNC
GtB8 gameRender(GtEntry* gameInst, GtF32 deltaTime) {
    return GtTrue;
}

// SAMPLE USER ONRESIZE FUNC
void gameOnResize(GtEntry* gameInst, GtU32 width, GtU32 height) {

}

// SAMPLE USER SHUTDOWN FUNC
void gameShutdown(GtEntry* gameInst) {
    gontiEventUnregister(GT_EVENT_CODE_APPLICATION_QUIT, 0, applicationOnEvent);
    gontiEventUnregister(GT_EVENT_CODE_APPLICATION_PAUSE, 0, applicationOnEvent);
    gontiEventUnregister(GT_EVENT_CODE_APPLICATION_RESUME, 0, applicationOnEvent);
    gontiEventUnregister(GT_EVENT_CODE_KEY_PRESSED, 0, applicationOnKey);
    gontiEventUnregister(GT_EVENT_CODE_KEY_RELEASED, 0, applicationOnKey);
    gontiEventUnregister(GT_EVENT_CODE_BUTTON_PRESSED, 0, applicationOnButton);
    gontiEventUnregister(GT_EVENT_CODE_BUTTON_RELEASED, 0, applicationOnButton);
    gontiEventUnregister(GT_EVENT_CODE_MOUSE_WHEEL, 0, applicationOnMouseWheel);
    gontiEventUnregister(GT_EVENT_CODE_MOUSE_MOVED, 0, applicationOnMouseMove);
    gontiEventUnregister(GT_EVENT_CODE_RESIZED, 0, applicationOnResized);
}