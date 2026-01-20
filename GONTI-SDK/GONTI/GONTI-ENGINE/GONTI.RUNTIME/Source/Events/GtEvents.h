#ifndef EVENTS_H
#define EVENTS_H

#ifdef __cplusplus
    extern "C" {
#endif

        #include "GtEventTypes.inl"

        /*BOOL*/
        GTAPI GtB8 gontiEventInitialize();
        GTAPI GtB8 gontiEventRegister(GtU16 code, void* listener, GtOnEventFN onEvent);
        GTAPI GtB8 gontiEventUnregister(GtU16 code, void* listener, GtOnEventFN onEvent);
        GTAPI GtB8 gontiEventFire(GtU16 code, void* sender, GtEventContext context);

        /*VOID*/
        GTAPI void gontiEventShutdown();

#ifdef __cplusplus
    }
#endif

#endif