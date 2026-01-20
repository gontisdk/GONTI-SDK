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
GTINFO("Engine started successfully.");
GTWARN("Low memory condition detected.");
GTERROR("Failed to load asset: %s", path);
GTDEBUG("Delta time: %f", dt);
GTTRACE("Entering update loop.");
```

## Fatal Logging

Fatal logs indicate unrecoverable errors:

```c
GTFATAL("Critical failure: %s", reason);
```

## Shutdown

Call during engine teardown:

```c
gontiLoggerShutdownLogging();
```

## Notes

* Log levels depend on compile-time flags (`GT_LOG_INFO_ENABLED`, `GT_LOG_DEBUG_ENABLED`, etc.).
* In `KRELEASE` builds, `DEBUG` and `TRACE` logs are disabled automatically.
* Platform-specific printing is handled internally.
