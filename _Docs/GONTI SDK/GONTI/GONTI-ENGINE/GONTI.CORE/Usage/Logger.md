# Logger Module - Usage

This document describes how to use the **GONTI Logger** module.

## Initialization

Before using any logging features, initialize the logger:

```c
if (!gontiLoggerInitializeLogging()) {
    // handle initialization failure
}
```

## Basic Logging

Use the provided macros to log messages at different severity levels:

```c
KINFO("Engine started successfully.");
KWARN("Low memory condition detected.");
KERROR("Failed to load asset: %s", path);
KDEBUG("Delta time: %f", dt);
KTRACE("Entering update loop.");
```

## Fatal Logging

Fatal logs indicate unrecoverable errors:

```c
KFATAL("Critical failure: %s", reason);
```

## Shutdown

Call during engine teardown:

```c
gontiLoggerShutdownLogging();
```

## Notes

* Log levels depend on compile-time flags (`LOG_INFO_ENABLED`, `LOG_DEBUG_ENABLED`, etc.).
* In `KRELEASE` builds, `DEBUG` and `TRACE` logs are disabled automatically.
* Platform-specific printing is handled internally.
