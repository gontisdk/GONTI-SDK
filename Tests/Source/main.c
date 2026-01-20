#include <stdio.h>
#include <GONTI/GONTI-ENGINE/GONTI.RENDER.VK/Source/Platform/GtVkPlatform.h>
#include <GONTI/GONTI-ENGINE/GONTI.RUNTIME/Source/Application/Vulkan/GtVkApp.h>
#include "events/events.h"
#include "game/game.h"
#include "core/entry.h"
#include "defines/defines.h"


int main(void) {
       gontiInitializeMemory();

       GtEntry entry;
       gontiEntry(&entry);
       gontiVkApplicationCreate(&entry);
       gontiVkApplicationRun(30, true);

       gontiShutdownMemory();
       return 0;
}