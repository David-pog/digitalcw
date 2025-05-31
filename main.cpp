#include <iostream>
#include <string>
#include <vector>
#define _USE_MATH_DEFINES // For M_PI with MSVC, place before cmath
#include <cmath>    // For std::sin, M_PI
#include <thread>
#include <chrono>
#include <cstdint>
#include <conio.h>  // For _kbhit() and _getch() (Windows)

#include <SFML/Audio.hpp>
// For sf::SoundChannel, usually included by Audio.hpp. If issues, uncomment:
// #include <SFML/Audio/SoundChannel.hpp>

// Global WPM variable
int current_wpm = 12;

// SFML Sound globals
sf::SoundBuffer g_sound_buffer;
sf::Sound* g_cw_sound = nullptr; // Changed to pointer, initialized to nullptr
const double CW_FREQUENCY = 750.0; // Hz
const unsigned CW_SAMPLE_RATE = 44100; // Samples per second
const std::int16_t CW_AMPLITUDE = 10000;

// Function to calculate dot duration in milliseconds
int calculate_dot_length_ms(int wpm) {
    if (wpm <= 0) {
        return 1200;
    }
    return 1200 / wpm;
}

// Initialize SFML sound buffer and sound object
bool init_sound() {
    std::vector<std::int16_t> samples;
    samples.resize(CW_SAMPLE_RATE * 1); // 1 channel (mono), 1 second duration

    for (std::size_t i = 0; i < samples.size(); ++i) {
        double time_s = static_cast<double>(i) / CW_SAMPLE_RATE;
        samples[i] = static_cast<std::int16_t>(static_cast<double>(CW_AMPLITUDE) * std::sin(2 * M_PI * CW_FREQUENCY * time_s));
    }

    // Define the channel map for mono sound for SFML 3.0 loadFromSamples
    std::vector<sf::SoundChannel> channelMap = {sf::SoundChannel::Mono};

    // Use 5-argument version of loadFromSamples for SFML 3.0
    if (!g_sound_buffer.loadFromSamples(samples.data(), samples.size(), 1, CW_SAMPLE_RATE, channelMap)) {
        std::cerr << "Error: Could not load audio samples into SFML buffer." << std::endl;
        return false; // g_cw_sound is still nullptr here
    }

    // Dynamically allocate and initialize g_cw_sound using the buffer
    g_cw_sound = new sf::Sound(g_sound_buffer);
    // The constructor sf::Sound(const sf::SoundBuffer&) sets the buffer.
    // So, g_cw_sound->setBuffer(g_sound_buffer); is not needed here.

    g_cw_sound->setLooping(true);
    return true;
}

// Function to play a dot
void play_dot() {
    std::cout << "\rDOT                                                                      " << std::flush;
    if (g_cw_sound && g_cw_sound->getStatus() != sf::SoundSource::Status::Playing) {
        g_cw_sound->play();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(calculate_dot_length_ms(current_wpm)));
    if (g_cw_sound) {
        g_cw_sound->stop();
    }
}

// Function to play a dash
void play_dash() {
    std::cout << "\rDASH                                                                     " << std::flush;
    if (g_cw_sound && g_cw_sound->getStatus() != sf::SoundSource::Status::Playing) {
        g_cw_sound->play();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(3 * calculate_dot_length_ms(current_wpm)));
    if (g_cw_sound) {
        g_cw_sound->stop();
    }
}

// Function for the space between dots/dashes of the same character
void inter_element_space() {
    std::this_thread::sleep_for(std::chrono::milliseconds(calculate_dot_length_ms(current_wpm)));
}

// Display WPM
void display_wpm() {
    std::cout << "\rWPM: " << current_wpm << " ('z' dot, 'x' dash, +/- WPM, 'q' quit)                         " << std::flush;
}

int main() {
    if (!init_sound()) {
        // Error message already printed by init_sound()
        // g_cw_sound would be nullptr if init_sound fails before allocation, so no delete needed here.
        return 1;
    }

    std::cout << "CW Paddle Emulator (Windows Mode with SFML Sound)" << std::endl;
    display_wpm();
    std::cout << std::endl;

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

    if (g_cw_sound) { // Clean up the dynamically allocated sound object
        delete g_cw_sound;
        g_cw_sound = nullptr;
    }
    return 0;
}
