# Asserts Module - How It Works

This document explains the internal mechanics and design of the **GONTI Assertions** system.

## Overview

The assertion system provides runtime validation for critical assumptions during development. When an assertion fails, detailed diagnostic information is logged, and execution is halted via a platform-specific debug break.

## Macro Architecture

The system is driven entirely by macros. This ensures:

* Zero runtime cost when assertions are disabled.
* File name and line number capture at compile time.
* Expression stringification using `#expr`.

### GTASSERT

```c
#define GTASSERT(expr) { \
    if (expr) { } else { \
        gontiReportAssertionFailure(#expr, "", __FILE__, __LINE__); \
        debugBreak(); \
    } \
}
```

Key behaviors:

* Converts the expression into a string.
* Passes file and line metadata.
* Calls `debugBreak()` to interrupt execution.

### GTASSERT_MSG

Same as `GTASSERT`, but includes a user‑provided message.

### GTASSERT_DEBUG

Enabled only when `_DEBUG` is defined.
Compiled out in release builds.

## Failure Reporting

Failed assertions trigger:

```c
gontiLoggerLogOutput(
    GT_LOG_LEVEL_FATAL,
    "Assertion Failure: %s, Message: '%s', In File: %s, In Line: %d\n",
    expression, message, file, line);
```

This ensures a standardized and high‑visibility log entry.

## Debug Break Handling

The implementation chooses the proper instruction per compiler:

* On MSVC: `__debugbreak()`
* On others: `__builtin_trap()` (note: typo in header appears to be `__bulitin_trap()`, likely meant to be `__builtin_trap()`)

`debugBreak()` halts the debugger at the failure site, enabling direct inspection of state.

## Compile-Time Control

Assertions are globally enabled by:

```c
#define GTASSERTIONS_ENABLED
```

If removed:

* All assertion macros expand to empty blocks.
* No log calls.
* No debug breaks.

This design allows assertions to be entirely stripped out for production builds without modifying code elsewhere.

## Summary

The assertion module provides:

* Safe, zero‑overhead removal in release builds.
* Detailed diagnostic reporting.
* Integration with the Logger system.
* Debugger interruption for immediate inspection.

It is a lightweight but powerful tool for maintaining engine correctness during development.
