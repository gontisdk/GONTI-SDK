# Platform System ― How It Works

This document explains how the platform abstraction layer functions across Windows and Linux, based on the provided `Platform.h`, `Platform_win32.c`, and `Platform_linux.c`.

---

## 1. Purpose of the Platform Layer

The platform layer provides OS‑dependent functionality behind a uniform API. The engine calls functions such as console output, memory management, or timers without knowing which OS implementation is behind them.

The compiler selects the correct implementation using:

```c
#if KPLATFORM_WINDOWS
#if KPLATFORM_LINUX
```

---

## 2. Console Output

### Windows

Uses WinAPI:

* `GetStdHandle`, `GetConsoleScreenBufferInfo`, `SetConsoleTextAttribute`, and `WriteConsoleA`.
* Colour values are mapped to WinAPI attribute constants.
* Restores original console attributes after printing.

### Linux

Uses ANSI escape sequences:

```c
printf("\033[%sm%s\033[0m", colourString, message);
```

No need for WinAPI handles.

---

## 3. Sleep / Timing

### Windows

Uses:

* `Sleep(ms)` — millisecond precision
* High‑resolution timer:

  * `QueryPerformanceCounter` for actual time
  * Requires engine‑set `clockFrequency` (set via `gontiPlatformSetClockFrequency`)

### Linux

Uses POSIX timers:

* If available: `nanosleep` with full nanosecond resolution
* Fallback: `sleep` + `usleep`
* Absolute time from `clock_gettime(CLOCK_MONOTONIC)`

---

## 4. Memory Functions

Both platforms use standard C library:

* `malloc`, `realloc`, `free`
* `memcpy`, `memset`
* Alignment flag (`aligned`) is currently unused on both platforms but exists for future improvements.

---

## 5. Differences Between Platforms

| Feature           | Windows                   | Linux                   |
| ----------------- | ------------------------- | ----------------------- |
| Console colours   | WinAPI attributes         | ANSI codes              |
| Sleep             | `Sleep(ms)`               | `nanosleep` or fallback |
| High‑res timer    | `QueryPerformanceCounter` | `clock_gettime`         |
| Alignment support | Not implemented           | Not implemented         |
| Dependencies      | `<Windows.h>`             | X11 + POSIX libs        |

---

## 6. Summary

The platform layer ensures the engine compiles and runs on both Windows and Linux without changes in the higher-level modules. Each platform implements the same API differently, depending on what the OS provides.

If new features are required (windowing, input, files, threads), they will continue this pattern: **shared header + per‑platform implementation**.
