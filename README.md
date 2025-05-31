# C++ CW Paddle Emulator

This is a simple C++ console application that emulates a CW (Morse code) paddle using keyboard input. It features adjustable WPM (Words Per Minute) and uses raw terminal input for immediate key response.

## Features

*   **Dot Key:** 'z'
*   **Dash Key:** 'x'
*   **WPM Control:**
    *   '+': Increase WPM
    *   '-': Decrease WPM
*   **Quit:** 'q'
*   Audible feedback via ASCII BEL character (system beep).
*   Direct keyboard input (no need to press Enter) via raw terminal mode (for Linux/macOS).

## Compilation

This program is designed for Linux or macOS systems due to its use of `<termios.h>` for raw keyboard input.

You'll need a C++ compiler that supports C++17 and pthreads. To compile, navigate to the directory containing `main.cpp` and run:

```bash
g++ main.cpp -o cw_paddle -std=c++17 -pthread
```

If you are on a system that does not support `termios.h` (e.g., Windows without a compatibility layer like WSL or Cygwin), the raw keyboard input functionality will not work, and the code would need to be adapted using platform-specific APIs like `<conio.h>`.

## Running the Application

After successful compilation, you can run the application from your terminal:

```bash
./cw_paddle
```

## Usage

*   Press 'z' to send a dot.
*   Press 'x' to send a dash.
*   Press '+' to increase the WPM speed.
*   Press '-' to decrease the WPM speed.
*   The current WPM is displayed on screen.
*   Press 'q' to quit the application.

The application will take over your terminal input.

## Sound

The sound generated is a simple ASCII BEL character (`\a`), which usually triggers a system beep. For more advanced or pleasant sound, you would need to integrate a proper audio library (e.g., SFML, SDL_mixer, OpenAL) or use platform-specific audio APIs.

## Important: Terminal Raw Mode

This application puts the terminal into "raw" mode. This means normal terminal behaviors (like echoing characters, processing Ctrl+C) are disabled to allow for direct key reads.
The application attempts to restore normal terminal settings upon exit (via 'q' or normal termination).

**If the application crashes or is forcibly closed, your terminal might remain in raw mode.**
Symptoms include:
*   Typed characters not appearing.
*   Enter key not working as expected.

**To fix a terminal stuck in raw mode:**
Blindly type `reset` and press Enter. This usually restores terminal sanity.
