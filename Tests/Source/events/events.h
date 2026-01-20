#pragma once

#include <GONTI/GONTI-ENGINE/GONTI.RUNTIME/Source/Events/GtEvents.h>
#include <GONTI/GONTI-ENGINE/GONTI.RUNTIME/Source/Events/InputSystem/GtInputs.h>
#include <GONTI/GONTI-ENGINE/GONTI.CORE/Source/Logging/GtLogger.h>
#include <GONTI/GONTI-ENGINE/GONTI.RUNTIME/Source/Application/Vulkan/GtVkApp.h>
#include <GONTI/GONTI-ENGINE/GONTI.RENDER/Source/Renderer/Frontend/GtRFrontend.h>

GtB8 applicationOnEvent(GtU16 code, void* sender, void* listenerInst, GtEventContext context);
GtB8 applicationOnKey(GtU16 code, void* sender, void* listenerInst, GtEventContext context);
GtB8 applicationOnButton(GtU16 code, void* sender, void* listenerInst, GtEventContext context);
GtB8 applicationOnMouseWheel(GtU16 code, void* sender, void* listenerInst, GtEventContext context);
GtB8 applicationOnMouseMove(GtU16 code, void* sender, void* listenerInst, GtEventContext context);
GtB8 applicationOnResized(GtU16 code, void* sender, void* listenerInst, GtEventContext context);