#include <iostream>
#include <string>
#include <thread> // For std::this_thread::sleep_for
#include <chrono> // For std::chrono::milliseconds
#include <conio.h>  // For _kbhit() and _getch() (Windows)
#include <cstdlib>   // For atexit() - though not strictly needed now without raw mode functions to cleanup

// Global WPM variable
int current_wpm = 12;

// Function to calculate dot duration in milliseconds
int calculate_dot_length_ms(int wpm) {
    if (wpm <= 0) {
        return 1200;
    }
    return 1200 / wpm;
}

// Function to play a dot
void play_dot() {
    // For Windows, \r might work, but clearing the line needs a different approach
    // For now, keep it simple, will be addressed in next step
    std::cout << "DOT "; // Simplified output for now
    std::cout << '\a' << std::flush; // Attempt to make a sound (ASCII BEL)
    std::this_thread::sleep_for(std::chrono::milliseconds(calculate_dot_length_ms(current_wpm)));
    std::cout << "    " << std::flush; // Try to clear "DOT "
}

// Function to play a dash
void play_dash() {
    std::cout << "DASH "; // Simplified output for now
    std::cout << '\a' << std::flush; // Attempt to make a sound (ASCII BEL)
    std::this_thread::sleep_for(std::chrono::milliseconds(3 * calculate_dot_length_ms(current_wpm)));
    std::cout << "     " << std::flush; // Try to clear "DASH "
}

// Function for the space between dots/dashes of the same character
void inter_element_space() {
    // This is silent.
    std::this_thread::sleep_for(std::chrono::milliseconds(calculate_dot_length_ms(current_wpm)));
    // No specific clearing here, play_dot/dash clear their own text.
    // A general clear might be needed before display_wpm.
}

// Display WPM
void display_wpm() {
    // ANSI escape codes like \033[K likely won't work in standard Windows cmd.exe
    // This will be addressed in the next "Windows Console Output Refinement" step.
    // For now, print on a new line or use \r if it behaves predictably.
    std::cout << "\rWPM: " << current_wpm << " ('z' dot, 'x' dash, +/- WPM, 'q' quit)            " << std::flush;
}

int main() {
    // enable_raw_mode(); // Removed, was termios specific

    std::cout << "CW Paddle Emulator (Windows Mode)" << std::endl;
    // display_wpm(); // Initial display
    // std::cout << std::endl; // New line after initial messages

    char input_char = 0;

    while (true) {
        display_wpm();

        if (_kbhit()) { // Check if a key has been pressed
            input_char = _getch(); // Get the character (doesn't echo)

            if (input_char == 'z') {
                play_dot();
                inter_element_space();
                // display_wpm(); // Called at the start of the loop
            } else if (input_char == 'x') {
                play_dash();
                inter_element_space();
                // display_wpm();
            } else if (input_char == '+') {
                current_wpm++;
                if (current_wpm > 60) current_wpm = 60;
                // display_wpm();
            } else if (input_char == '-') {
                if (current_wpm > 1) {
                    current_wpm--;
                }
                // display_wpm();
            } else if (input_char == 'q') {
                std::cout << "\rExiting application.                                   " << std::endl;
                break;
            }
            // Special keys (like function keys, arrows) return 0 or 0xE0 (224)
            // then another char. For 'z', 'x', '+', '-', 'q' this is not an issue.
            // If other keys were used, we might need:
            // if (input_char == 0 || input_char == 0xE0) {
            //     input_char = _getch(); // consume second byte of special key
            // }
        } else {
            // No key pressed, prevent busy-waiting and reduce CPU usage
            // A very short sleep is advisable.
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    // No disable_raw_mode() to call via atexit for conio.h
    return 0;
}
