# C++ CW Paddle Emulator with SFML Sound

This is a simple C++ console application that emulates a CW (Morse code) paddle using keyboard input. It features adjustable WPM (Words Per Minute) and uses SFML for audio feedback.

**Note:** The current input method uses `<conio.h>`, making this version primarily targeted for Windows. For Linux/macOS, the input handling would need to be reverted to use `termios` (see previous versions if available).

## Features

*   **Dot Key:** 'z'
*   **Dash Key:** 'x'
*   **WPM Control:**
    *   '+': Increase WPM
    *   '-': Decrease WPM
*   **Quit:** 'q'
*   **Audible Feedback:** Uses SFML to generate a sine wave tone (default 750 Hz).
*   **Direct Keyboard Input (Windows):** Uses `<conio.h>` functions (`_kbhit()`, `_getch()`).

## Dependencies

This application now requires the **SFML (Simple and Fast Multimedia Library)** for audio output. You will need to install SFML and ensure your compiler can find its headers and libraries.

### Installing SFML

*   **Windows:**
    1.  Download SFML from the [official website](https://www.sfml-dev.org/download.php) (e.g., "GCC X MinGW (DW2)" for MinGW or "Visual C++ X" for MSVC).
    2.  Extract the archive to a known location (e.g., `C:\SFML`).
    3.  You'll need to configure your compiler/IDE to find SFML's `include` directory (e.g., `C:\SFML\include`) and `lib` directory (e.g., `C:\SFML\lib`).
    4.  Ensure SFML's DLLs (e.g., `sfml-audio-2.dll`, `sfml-system-2.dll`, `openal32.dll`, etc.) are accessible to your executable, either by copying them to your build directory or adding SFML's `bin` directory to your system's PATH.

*   **Linux:**
    SFML is usually available through your distribution's package manager.
    ```bash
    sudo apt-get update
    sudo apt-get install libsfml-dev
    ```
    This typically installs headers, libraries, and dependencies.

*   **macOS:**
    The easiest way is often via Homebrew:
    ```bash
    brew install sfml
    ```
    This will install SFML and its dependencies.

## Compilation

You will need a C++ compiler that supports C++17. The threading library (`-pthread` or equivalent) is needed for `std::this_thread::sleep_for`. You must also link against the SFML libraries.

**Note on SFML Libraries:** You typically need to link `sfml-audio` and `sfml-system`. Depending on whether you use static or dynamic SFML libraries, and how SFML itself was built, you might need to link other libraries too (e.g., `openal32`, `flac`, `vorbis`, `ogg` for audio; SFML's website provides detailed guidance). The examples below assume dynamic linking for simplicity where possible.

### Windows (with conio.h input and SFML sound)

**Using MinGW g++:**
Ensure SFML's `include` and `lib` paths are known to g++.
```bash
# Example: Replace C:\SFML with your SFML path
g++ main.cpp -o cw_paddle_sfml.exe -std=c++17 -IC:\SFML\include -LC:\SFML\lib -lsfml-audio -lsfml-system -static-libgcc -static-libstdc++
```
(You might need to copy SFML DLLs like `sfml-audio-2.dll`, `sfml-system-2.dll`, `openal32.dll` to the same directory as the executable).

**Using Microsoft Visual C++ (cl.exe):**
Configure your project's Linker settings:
    - Additional Include Directories: `C:\SFML\include`
    - Additional Library Directories: `C:\SFML\lib`
    - Additional Dependencies: `sfml-audio.lib`, `sfml-system.lib` (and others like `openal32.lib` if needed, especially for static linking. Use `sfml-audio-s.lib`, `sfml-system-s.lib` etc. for static linking).
Command line (ensure environment variables for VS are set):
```bash
cl main.cpp /EHsc /std:c++17 /Fe:cw_paddle_sfml.exe /I C:\SFML\include /link /LIBPATH:C:\SFML\lib sfml-audio.lib sfml-system.lib
```

### Linux (Example - if input was termios)
This example assumes you'd modify `main.cpp` back to use `termios` for input.
```bash
g++ main.cpp -o cw_paddle_sfml -std=c++17 -pthread -lsfml-audio -lsfml-system
```

### macOS (Example - if input was termios)
This example assumes you'd modify `main.cpp` back to use `termios` for input.
```bash
clang++ main.cpp -o cw_paddle_sfml -std=c++17 -pthread -lsfml-audio -lsfml-system -F/Library/Frameworks -L/usr/local/lib # Adjust paths if SFML installed elsewhere
# Or using pkg-config if available and SFML provides .pc files
# clang++ main.cpp -o cw_paddle_sfml $(pkg-config --cflags --libs sfml-audio sfml-system) -std=c++17 -pthread
```

## Running the Application

After successful compilation (and ensuring SFML DLLs/shared objects are accessible if dynamically linked):

*   **Windows:** `cw_paddle_sfml.exe`
*   **Linux/macOS (if adapted):** `./cw_paddle_sfml`

## Usage
*   Press 'z' to send a dot.
*   Press 'x' to send a dash.
*   Press '+' to increase the WPM speed.
*   Press '-' to decrease the WPM speed.
*   The current WPM is displayed on screen.
*   Press 'q' to quit the application.

## Sound

Sound is generated using the **SFML Audio** module. A sine wave tone (default 750 Hz) is produced. This method provides more reliable and cross-platform capable audio output compared to system beeps.

## Important Notes

### For Linux/macOS (Terminal Raw Mode - if re-enabled)
If you adapt the code back to use `termios` for input on Linux/macOS, the application will put the terminal into "raw" mode. (Details about raw mode and `reset` command remain as before).

### For Windows
The current Windows version uses `<conio.h>` for input, which generally does not leave the console in a problematic state if the application crashes.
