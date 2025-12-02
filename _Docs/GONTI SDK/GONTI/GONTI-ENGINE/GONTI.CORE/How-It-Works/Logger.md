# Logger Module - How It Works

This document explains the internal design and behavior of the **GONTI Logger** module.

## Overview

The logger provides structured, platform-aware console output using configurable log levels. Logging is macro-driven for performance and compile-time filtering.

## Log Level System

The module defines six severity levels:

* `LOG_LEVEL_FATAL`
* `LOG_LEVEL_ERROR`
* `LOG_LEVEL_WARN`
* `LOG_LEVEL_INFO`
* `LOG_LEVEL_DEBUG`
* `LOG_LEVEL_TRACE`

Compile-time flags (`LOG_WARN_ENABLED`, etc.) enable or disable categories. In release builds (`KRELEASE == 1`), `DEBUG` and `TRACE` logs are automatically turned off.

## Macro Dispatching

Each logging macro expands directly to:

```c
gontiLoggerLogOutput(level, message, ...)
```

This minimizes call overhead and ensures consistent formatting.

## Formatting Pipeline

1. A fixed-size buffer (`outMessage`) is allocated on the stack.
2. The buffer is zeroed with `k_setMemory`.
3. `vsnprintf` formats the message from the variadic arguments.
4. A second buffer concatenates the log prefix (`[INFO]:`, `[ERROR]:`, etc.) with the formatted message.

## Error Routing

Messages with level `< LOG_LEVEL_WARN` (i.e., fatal and error) are sent to:

```c
gontiPlatformConsoleWriteError()
```

All others go through:

```c
gontiPlatformConsoleWrite()
```

This separates error output handling between platforms.

## Initialization & Shutdown

The functions `gontiLoggerInitializeLogging()` and `gontiLoggerShutdownLogging()` currently contain stubs for future enhancements such as:

* Log file creation
* Buffered / queued logging
* Graceful flush on shutdown

## Memory & Safety

* Messages are stored on the stack to avoid heap allocation.
* Maximum message length is capped at 32,000 characters to prevent buffer overflow.
* Prefix strings are static constants for fast lookup.

This modular structure allows future expansion without affecting the API exposed to the rest of the engine.
