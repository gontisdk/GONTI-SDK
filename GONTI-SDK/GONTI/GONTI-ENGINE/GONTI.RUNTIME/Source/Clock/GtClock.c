#include "GtClock.h"

#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Platform/GtPlatform.h>

void gontiVkClockUpdate(Clock* clock) {
    if (clock->startTime != 0) {
        clock->elapsed = gontiPlatformGetAbsoluteTime() - clock->startTime;
    }
}
void gontiVkClockStart(Clock* clock) {
    clock->startTime = gontiPlatformGetAbsoluteTime();
    clock->elapsed = 0;
}
void gontiVkClockStop(Clock* clock) {
    clock->startTime = 0;
}
