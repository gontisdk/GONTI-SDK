#include "GtEvents.h"

#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Memory/GtMemory.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Containers/DynamicArray/GtDArray.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/GtLogger.h>

typedef struct GtRegisteredEvent {
    void* listener;
    GtOnEventFN callback;
}GtRegisteredEvent;

typedef struct GtEventCodeEntry {
    GtRegisteredEvent* events;
}GtEventCodeEntry;

#define MAX_MESSAGE_CODES 16384

typedef struct GtEventSystemState {
    GtEventCodeEntry registered[MAX_GT_EVENT_CODE];
}GtEventSystemState;

static GtB8 isInitialized = GtFalse;
static GtEventSystemState state;

/*BOOL*/
GtB8 gontiEventInitialize() {
    if (isInitialized == GtTrue) return GtFalse;

    isInitialized = GtFalse;
    gt_zeroMemory(&state, sizeof(GtEventSystemState));

    isInitialized = GtTrue;

    GTINFO("Event system initialized");
    return GtTrue;
}
GtB8 gontiEventRegister(GtU16 code, void* listener, GtOnEventFN onEvent) {
    if (isInitialized == GtFalse) return GtFalse;

    if (state.registered[code].events == 0) state.registered[code].events = gontiDarrayCreate(GtRegisteredEvent);

    GtU64 registeredCount = gontiDarrayLength(state.registered[code].events);

    for (GtU64 i = 0; i < registeredCount; i++) {
        if (state.registered[code].events[i].listener == listener) {
            GTWARN("TODO: WARN MESSAGE HERE");
            return GtFalse;
        }
    }

    GtRegisteredEvent event;
    event.listener = listener;
    event.callback = onEvent;
    gontiDarrayPush(state.registered[code].events, event);

    return GtTrue;
}
GtB8 gontiEventUnregister(GtU16 code, void* listener, GtOnEventFN onEvent) {
    if (isInitialized == GtFalse) return GtFalse;

    if (state.registered[code].events == 0) {
        GTWARN("TODO: WARN MESSAGE HERE");
        return GtFalse;
    }

    GtU64 registeredCount = gontiDarrayLength(state.registered[code].events);

    for (GtU64 i = 0; i < registeredCount; i++) {
        GtRegisteredEvent e = state.registered[code].events[i];

        if (e.listener == listener && e.callback == onEvent) {
            GtRegisteredEvent poppedEvent;
            gontiDarrayPopAt(state.registered[code].events, i, &poppedEvent);

            return GtTrue;
        }
    }

    return GtFalse;
}
GtB8 gontiEventFire(GtU16 code, void* sender, GtEventContext context) {
    if (isInitialized == GtFalse) return GtFalse;

    if (state.registered[code].events == 0) return GtFalse;

    GtU64 registeredCount = gontiDarrayLength(state.registered[code].events);

    for (GtU64 i = 0; i < registeredCount; i++) {
        GtRegisteredEvent e = state.registered[code].events[i];

        if (e.callback == NULL) continue;

        if (e.callback(code, sender, e.listener, context)) {
            return GtTrue;
        }
    }

    return GtFalse;
}

/*VOID*/
void gontiEventShutdown() {
    for (GtU16 i = 0; i < MAX_GT_EVENT_CODE; i++) {
        if (state.registered[i].events != NULL) {
            gontiDarrayDestroy(state.registered[i].events);
            state.registered[i].events = NULL;
        }
    }

    isInitialized = GtFalse;
}