# C++ CW Paddle Emulator

This is a simple C++ console application that emulates a CW (Morse code) paddle using keyboard input. It features adjustable WPM (Words Per Minute) and uses system-specific methods for immediate key response and sound generation.

## Features

*   **Dot Key:** 'z'
*   **Dash Key:** 'x'
*   **WPM Control:**
    *   '+': Increase WPM
    *   '-': Decrease WPM
*   **Quit:** 'q'
*   **Audible Feedback:**
    *   **Linux/macOS:** ASCII BEL character (system beep).
    *   **Windows:** Windows `Beep()` API for direct tone generation (default 750 Hz).
*   **Direct Keyboard Input (no need to press Enter):**
    *   **Linux/macOS:** Uses raw terminal mode via `<termios.h>`.
    *   **Windows:** Uses `<conio.h>` functions (`_kbhit()`, `_getch()`).

## Compilation

You will need a C++ compiler that supports C++17.

### Linux/macOS

This version uses `<termios.h>` for raw keyboard input and outputs an ASCII BEL character for sound. The threading library (`-pthread` or equivalent) is needed for `std::this_thread::sleep_for`.
To compile, navigate to the directory containing `main.cpp` and run:

```bash
g++ main.cpp -o cw_paddle -std=c++17 -pthread
```

### Windows

This version uses `<conio.h>` for keyboard input and the Windows `Beep()` API (from `<windows.h>`) for sound generation. The threading library support for `std::this_thread::sleep_for` is also needed.
You can compile it using MinGW g++ or Microsoft Visual C++ compiler.

**Using MinGW g++:**
```bash
g++ main.cpp -o cw_paddle_win.exe -std=c++17 -static-libgcc -static-libstdc++
```
(The `-static-libgcc` and `-static-libstdc++` flags can help create a more portable executable. `-pthread` is often implicitly supported by MinGW for `std::thread`. `kernel32.lib`, which provides `Beep`, is linked by default.)

**Using Microsoft Visual C++ (cl.exe):**
Open a Developer Command Prompt for Visual Studio and run:
```bash
cl main.cpp /EHsc /std:c++17 /Fe:cw_paddle_win.exe
```
(`/Fe:` specifies the output executable name. `kernel32.lib` is linked by default.)

## Running the Application

After successful compilation:

*   **Linux/macOS:** `./cw_paddle`
*   **Windows:** `cw_paddle_win.exe`

## Usage

*   Press 'z' to send a dot.
*   Press 'x' to send a dash.
*   Press '+' to increase the WPM speed.
*   Press '-' to decrease the WPM speed.
*   The current WPM is displayed on screen.
*   Press 'q' to quit the application.

The application will take over your terminal input for the specified keys.

## Sound

*   **Linux/macOS:** The sound generated is a simple ASCII BEL character (``), which usually triggers a system beep.
*   **Windows:** The sound is generated using the Windows `Beep(frequency, duration)` API. This provides a more direct and typically more reliable tone. The default frequency is set to 750 Hz in the application.

For more advanced or pleasant sound customization beyond this, especially on Linux/macOS, you would need to integrate a proper audio library.

## Important Notes

### For Linux/macOS (Terminal Raw Mode)

The Linux/macOS version puts the terminal into "raw" mode. This means normal terminal behaviors (like echoing characters, processing Ctrl+C) are disabled to allow for direct key reads. The application attempts to restore normal terminal settings upon exit (via 'q' or normal termination).

**If the application crashes or is forcibly closed on Linux/macOS, your terminal might remain in raw mode.**
Symptoms include:
*   Typed characters not appearing.
*   Enter key not working as expected.

**To fix a terminal stuck in raw mode (Linux/macOS):**
Blindly type `reset` and press Enter. This usually restores terminal sanity.

### For Windows
The Windows version uses `<conio.h>` which handles input differently and generally does not leave the console in a problematic state if the application crashes. Standard console behavior should be restored automatically.
