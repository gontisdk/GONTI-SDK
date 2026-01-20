#include "entry.h"
#include "../game/game.h"

GtB8 gontiEntry(GtEntry* outEntry) {
    outEntry->windowConfig.windowName = "GONTI GAME ENGINE TESTBED";
    outEntry->windowConfig.className = "0xGONTIENGINE";
    outEntry->windowConfig.startPosX = 100;
    outEntry->windowConfig.startPosY = 100;
    outEntry->windowConfig.startWidth = 800;
    outEntry->windowConfig.startHeight = 600;

    // GtEntry functions (USER IMPLEMENTATIONS)
    outEntry->GtUpdateFN = gameUpdate;
    outEntry->GtRenderFN = gameRender;
    outEntry->GtInitializeFN = gameInitialize;
    outEntry->GtOnResizeFN = gameOnResize;
    outEntry->GtShutdownFN = gameShutdown;

    // GONTI ENGINE FUNCTION
    outEntry->GtState = gt_allocate(sizeof(GameState), GT_MEM_TAG_ENTRY);

    return GtTrue;
}