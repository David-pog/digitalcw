#include <iostream>
#include <string>
#include <vector> // For std::vector with SFML samples
#define _USE_MATH_DEFINES // For M_PI with MSVC, place before cmath
#include <cmath>    // For std::sin, M_PI
#include <thread>
#include <chrono>
#include <conio.h>  // For _kbhit() and _getch() (Windows)
// #include <windows.h> // No longer needed for Beep()
#include <cstdlib>   // For atexit() or other general utilities, kept for now

#include <SFML/Audio.hpp> // For SFML sound

// Global WPM variable
int current_wpm = 12;

// SFML Sound globals
sf::SoundBuffer g_sound_buffer;
sf::Sound g_cw_sound;
const double CW_FREQUENCY = 750.0; // Hz
const unsigned CW_SAMPLE_RATE = 44100; // Samples per second
const sf::Int16 CW_AMPLITUDE = 10000; // Amplitude for sf::Int16 samples

// Function to calculate dot duration in milliseconds
int calculate_dot_length_ms(int wpm) {
    if (wpm <= 0) {
        return 1200;
    }
    return 1200 / wpm;
}

// Initialize SFML sound buffer and sound object
bool init_sound() {
    std::vector<sf::Int16> samples;
    // Generate 1 second of sine wave data.
    // Looping the sound makes the buffer duration somewhat arbitrary,
    // as long as it's not excessively short or long.
    // Using a fixed duration buffer that is looped.
    samples.resize(CW_SAMPLE_RATE * 1); // 1 channel (mono), 1 second duration

    for (std::size_t i = 0; i < samples.size(); ++i) {
        double time_s = static_cast<double>(i) / CW_SAMPLE_RATE;
        samples[i] = static_cast<sf::Int16>(CW_AMPLITUDE * std::sin(2 * M_PI * CW_FREQUENCY * time_s));
    }

    if (!g_sound_buffer.loadFromSamples(samples.data(), samples.size(), 1, CW_SAMPLE_RATE)) {
        std::cerr << "Error: Could not load audio samples into SFML buffer." << std::endl;
        return false;
    }

    g_cw_sound.setBuffer(g_sound_buffer);
    g_cw_sound.setLoop(true); // Loop the sound so it plays continuously until stopped
    return true;
}


// Function to play a dot
void play_dot() {
    std::cout << "\rDOT                                                                      " << std::flush;
    if (g_cw_sound.getStatus() != sf::Sound::Playing) {
        g_cw_sound.play();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(calculate_dot_length_ms(current_wpm)));
    g_cw_sound.stop();
}

// Function to play a dash
void play_dash() {
    std::cout << "\rDASH                                                                     " << std::flush;
    if (g_cw_sound.getStatus() != sf::Sound::Playing) {
        g_cw_sound.play();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(3 * calculate_dot_length_ms(current_wpm)));
    g_cw_sound.stop();
}

// Function for the space between dots/dashes of the same character
void inter_element_space() {
    // This is silent.
    std::this_thread::sleep_for(std::chrono::milliseconds(calculate_dot_length_ms(current_wpm)));
}

// Display WPM
void display_wpm() {
    // \r to return to start of line, then print WPM status. Ensure enough spaces to clear previous "DOT" or "DASH"
    std::cout << "\rWPM: " << current_wpm << " ('z' dot, 'x' dash, +/- WPM, 'q' quit)                         " << std::flush;
}

int main() {
    if (!init_sound()) {
        // Error message already printed by init_sound()
        // std::cerr << "Failed to initialize sound. Exiting." << std::endl;
        return 1;
    }

    std::cout << "CW Paddle Emulator (Windows Mode with SFML Sound)" << std::endl;
    display_wpm();
    std::cout << std::endl; // Move to next line so first DOT/DASH doesn't overwrite initial display_wpm line

    char input_char = 0;

    while (true) {
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
                std::cout << "\rExiting application.                                                        " << std::endl;
                break;
            }
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    return 0;
}
