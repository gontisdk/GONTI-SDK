# Asserts Module - Usage

This document provides guidance on how to use the **GONTI Assertions** system.

## Basic Assertion

Use `KASSERT()` to validate conditions that must always be true.

```c
KASSERT(pointer != NULL);
```

If the expression evaluates to false:

* A fatal log entry is generated.
* The program triggers a debug break.

## Assertion with Custom Message

Use `KASSERT_MSG()` when you want to provide additional context:

```c
KASSERT_MSG(index < count, "Index out of bounds");
```

This helps identify logic errors quickly during debugging.

## Debug-Only Assertions

Use `KASSERT_DEBUG()` for checks that should only run in debug builds:

```c
KASSERT_DEBUG(value >= 0);
```

In release builds these are compiled out.

## When Assertions Are Enabled

Assertions are enabled globally by:

```c
#define KASSERTIONS_ENABLED
```

If this macro is removed, all assertion macros become no-ops.

## Behavior on Failure

On assertion failure:

1. The engine logs a fatal error using `gontiLoggerLogOutput()`.
2. `debugBreak()` halts the program inside the debugger.

Use assertions for invariants and critical assumptions—not for recoverable errors.
