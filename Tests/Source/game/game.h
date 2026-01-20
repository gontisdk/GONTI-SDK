#pragma once

#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/GtLogger.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Defines/GtDefines.h>
#include <GONTI/GONTI-ENGINE/GONTI.RUNTIME/Source/EntryPoint/GtEntry.h>

typedef struct GameState {
    GtF32 deltaTime;
}GameState;

GtB8 gameInitialize(GtEntry* gameInst);
GtB8 gameUpdate(GtEntry* gameInst, GtF32 deltaTime);
GtB8 gameRender(GtEntry* gameInst, GtF32 deltaTime);
void gameOnResize(GtEntry* gameInst, GtU32 width, GtU32 height);
void gameShutdown(GtEntry* gameInst);