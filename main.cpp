#include <iostream>
#include <string>
#include <thread> // For std::this_thread::sleep_for
#include <chrono> // For std::chrono::milliseconds
#include <conio.h>  // For _kbhit() and _getch() (Windows)
#include <windows.h> // For Beep()
#include <cstdlib>   // For atexit() - though not strictly needed now

// Global WPM variable
int current_wpm = 12;
const DWORD BEEP_FREQUENCY = 750; // Frequency in Hertz

// Function to calculate dot duration in milliseconds
int calculate_dot_length_ms(int wpm) {
    if (wpm <= 0) {
        return 1200;
    }
    return 1200 / wpm;
}

// Function to play a dot
void play_dot() {
    // \r to return to start of line, then print DOT and spaces to clear previous display_wpm
    std::cout << "\rDOT                                                                      " << std::flush;
    Beep(BEEP_FREQUENCY, calculate_dot_length_ms(current_wpm));
}

// Function to play a dash
void play_dash() {
    std::cout << "\rDASH                                                                     " << std::flush;
    Beep(BEEP_FREQUENCY, 3 * calculate_dot_length_ms(current_wpm));
}

// Function for the space between dots/dashes of the same character
void inter_element_space() {
    // This is silent. The Beep() function is synchronous, so it blocks.
    // The sleep here provides the silent space after the beep has finished.
    std::this_thread::sleep_for(std::chrono::milliseconds(calculate_dot_length_ms(current_wpm)));
}

// Display WPM
void display_wpm() {
    // \r to return to start of line, then print WPM status. Ensure enough spaces to clear previous "DOT" or "DASH"
    std::cout << "\rWPM: " << current_wpm << " ('z' dot, 'x' dash, +/- WPM, 'q' quit)                         " << std::flush;
}

int main() {
    std::cout << "CW Paddle Emulator (Windows Mode with Beep API)" << std::endl;
    // Initial display of WPM status on a new line
    display_wpm();
    std::cout << std::endl; // Move to next line so first DOT/DASH doesn't overwrite initial display_wpm line

    char input_char = 0;

    while (true) {
        // At the start of the loop, or after an action, display_wpm will overwrite the previous line.
        // The cursor should be at the beginning of the line due to \r in display_wpm.
        display_wpm();

        if (_kbhit()) {
            input_char = _getch();

            if (input_char == 'z') {
                play_dot();
                inter_element_space();
            } else if (input_char == 'x') {
                play_dash();
                inter_element_space();
            } else if (input_char == '+') {
                current_wpm++;
                if (current_wpm > 60) current_wpm = 60;
            } else if (input_char == '-') {
                if (current_wpm > 1) {
                    current_wpm--;
                }
            } else if (input_char == 'q') {
                // Clear the line and print exiting message
                std::cout << "\rExiting application.                                                        " << std::endl;
                break;
            }
            // After an action (dot, dash, WPM change), the loop restarts and display_wpm() is called,
            // which overwrites the "DOT " or "DASH " line.
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    return 0;
}
