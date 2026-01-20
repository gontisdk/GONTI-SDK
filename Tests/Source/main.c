#include <stdio.h>
#include <GONTI/GONTI-ENGINE/GONTI.RUNTIME/Source/Application/GtApp.h>
#include "events/events.h"
#include "game/game.h"
#include "core/entry.h"
#include "defines/defines.h"


int main(void) {
       GtEntry entry;
       gontiEntry(&entry);
       gontiApplicationCreate(&entry);
       gontiApplicationRun(30, true);

       return 0;
}