#include <iostream>
#include <string>
#include <vector>
#define _USE_MATH_DEFINES // For M_PI with MSVC, place before cmath
#include <cmath>    // For std::sin, M_PI
#include <thread>
#include <chrono>
#include <cstdint> // For std::int16_t
#include <conio.h>  // For _kbhit() and _getch() (Windows)

#include <SFML/Audio.hpp> // For SFML sound

// Global WPM variable
int current_wpm = 12;

// SFML Sound globals
sf::SoundBuffer g_sound_buffer;
sf::Sound g_cw_sound;
const double CW_FREQUENCY = 750.0; // Hz
const unsigned CW_SAMPLE_RATE = 44100; // Samples per second
const std::int16_t CW_AMPLITUDE = 10000; // Changed sf::Int16 to std::int16_t

// Function to calculate dot duration in milliseconds
int calculate_dot_length_ms(int wpm) {
    if (wpm <= 0) {
        return 1200;
    }
    return 1200 / wpm;
}

// Initialize SFML sound buffer and sound object
bool init_sound() {
    std::vector<std::int16_t> samples; // Changed sf::Int16 to std::int16_t

    // Generate 1 second of sine wave data.
    samples.resize(CW_SAMPLE_RATE * 1); // 1 channel (mono), 1 second duration

    for (std::size_t i = 0; i < samples.size(); ++i) {
        double time_s = static_cast<double>(i) / CW_SAMPLE_RATE;
        // Ensure CW_AMPLITUDE is used in calculation for static_cast
        samples[i] = static_cast<std::int16_t>(static_cast<double>(CW_AMPLITUDE) * std::sin(2 * M_PI * CW_FREQUENCY * time_s));
    }

    if (!g_sound_buffer.loadFromSamples(samples.data(), samples.size(), 1, CW_SAMPLE_RATE)) {
        std::cerr << "Error: Could not load audio samples into SFML buffer." << std::endl;
        return false;
    }

    g_cw_sound.setBuffer(g_sound_buffer); // Set the buffer for the sound object
    g_cw_sound.setLooping(true); // Changed from setLoop to setLooping for SFML 3.0
    return true;
}


// Function to play a dot
void play_dot() {
    std::cout << "\rDOT                                                                      " << std::flush;
    // Changed sf::Sound::Playing to sf::SoundSource::Status::Playing for SFML 3.0
    if (g_cw_sound.getStatus() != sf::SoundSource::Status::Playing) {
        g_cw_sound.play();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(calculate_dot_length_ms(current_wpm)));
    g_cw_sound.stop();
}

// Function to play a dash
void play_dash() {
    std::cout << "\rDASH                                                                     " << std::flush;
    // Changed sf::Sound::Playing to sf::SoundSource::Status::Playing for SFML 3.0
    if (g_cw_sound.getStatus() != sf::SoundSource::Status::Playing) {
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
