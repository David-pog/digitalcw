#include <iostream>
#include <string>
#include <thread> // For std::this_thread::sleep_for
#include <chrono> // For std::chrono::milliseconds
#include <termios.h> // For terminal I/O settings
#include <unistd.h>  // For STDIN_FILENO, read()
#include <cstdio>    // For getchar() - though read() is used in final version
#include <cstdlib>   // For atexit()

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
    // In raw mode, we need to manage cursor and output carefully.
    // \r moves cursor to beginning of line. Spaces clear previous output.
    std::cout << "\rDOT                     " << std::flush;
    std::cout << '\a' << std::flush; // Attempt to make a sound (ASCII BEL)
    std::this_thread::sleep_for(std::chrono::milliseconds(calculate_dot_length_ms(current_wpm)));
}

// Function to play a dash
void play_dash() {
    std::cout << "\rDASH                    " << std::flush;
    std::cout << '\a' << std::flush; // Attempt to make a sound (ASCII BEL)
    std::this_thread::sleep_for(std::chrono::milliseconds(3 * calculate_dot_length_ms(current_wpm)));
}

// Function for the space between dots/dashes of the same character
void inter_element_space() {
    // Standard space between dots/dashes of the same character is 1 dot length
    // This is silent.
    std::this_thread::sleep_for(std::chrono::milliseconds(calculate_dot_length_ms(current_wpm)));
    // Clear the DOT/DASH message after inter-element space
    std::cout << "\r                        " << std::flush;
}

struct termios orig_termios; // To store original terminal settings

void disable_raw_mode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) < 0) {
        // perror("tcsetattr"); // Error disabling raw mode
    }
    // std::cout << "Terminal settings restored." << std::endl; // Avoid cout in atexit if possible
}

void enable_raw_mode() {
    if (!isatty(STDIN_FILENO)) {
        // std::cerr << "Not a terminal. Cannot enable raw mode." << std::endl;
        // exit(EXIT_FAILURE); // Or handle gracefully
        return; // Do not try to enable raw mode if not a TTY
    }
    if (tcgetattr(STDIN_FILENO, &orig_termios) < 0) {
        // perror("tcgetattr"); // Error getting terminal attributes
        // exit(EXIT_FAILURE);
        return;
    }
    std::atexit(disable_raw_mode);

    struct termios raw = orig_termios;
    // Input flags: no break processing, no CR to NL, no parity check, no strip char, no start/stop output control.
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    // Output flags: no output processing.
    raw.c_oflag &= ~(OPOST);
    // Control flags: set 8 bit chars.
    raw.c_cflag |= (CS8);
    // Local flags: no echo, no canonical processing, no extended functions, no signal chars (^Z, ^C)
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    // For blocking read of a single character
    raw.c_cc[VMIN] = 1; // read() will block until at least 1 byte is available
    raw.c_cc[VTIME] = 0; // no timeout for read()

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) < 0) {
        // perror("tcsetattr"); // Error setting raw mode
        // exit(EXIT_FAILURE); // Or handle gracefully
    }
    // std::cout << "Raw keyboard mode enabled." << std::endl; // Cleaner UI without this
}

void display_wpm() {
    // \r moves to beginning of line, K clears line from cursor to end
    std::cout << "\r\033[KCurrent WPM: " << current_wpm << " ('z' dot, 'x' dash, +/- WPM, 'q' quit) " << std::flush;
}

int main() {
    enable_raw_mode();

    std::cout << "CW Paddle Emulator (Raw Mode with BEL)" << std::endl;
    display_wpm();
    // std::cout << std::endl; // Manage newline carefully, next output will overwrite or go to next line
                           // The display_wpm itself handles \r and clearing.
                           // A DOT/DASH will print on its own line after this initial display.
                           // Let's ensure the first DOT/DASH appears on a new line,
                           // and subsequent ones overwrite that line.
                           // The current play_dot/dash use \r so they will overwrite.
                           // The main loop adds endl after display_wpm call following a dot/dash.

    char input_char;
    // Initial new line for the status bar
    std::cout << std::endl;

    while (true) {
        display_wpm(); // Display status at the start of the line where input is expected or after an action

        if (read(STDIN_FILENO, &input_char, 1) == 1) {
            if (input_char == 'z') {
                play_dot();
                inter_element_space();
                // display_wpm already called at the start of the loop, let play_dot/inter_element_space manage their line
            } else if (input_char == 'x') {
                play_dash();
                inter_element_space();
            } else if (input_char == '+') {
                current_wpm++;
                if (current_wpm > 60) current_wpm = 60;
                // display_wpm(); // Will be called at loop start
            } else if (input_char == '-') {
                if (current_wpm > 1) {
                    current_wpm--;
                }
                // display_wpm(); // Will be called at loop start
            } else if (input_char == 'q') {
                std::cout << "\r\033[KExiting." << std::endl;
                break;
            }
             // After an action, clear the line and redraw WPM status for next input
            std::cout << "\r\033[K"; // Clear line from cursor
            // display_wpm(); // Redraw WPM status on the now cleared line
        } else {
            // read() failed
            break;
        }
    }
    return 0;
}
