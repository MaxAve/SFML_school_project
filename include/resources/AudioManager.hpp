#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map> // because better optimized than usual map

namespace AudioManager {
extern std::unordered_map<std::string, sf::SoundBuffer> soundBuffer;
extern std::unordered_map<std::string, sf::Sound> stringToSound;

// sf::Music stored as pointers to avoid copying streams
extern std::unordered_map<std::string, sf::Music*> stringToBackground; // background sound
extern std::unordered_map<std::string, sf::Music*> stringToMusic; // game music

extern sf::Music* curMusic;
extern sf::Music* curBackground; // background sounds

void init();

void playSound(const std::string&);

void playMusic(const std::string&);
void playRandomMusic();
void stopMusic();

void playBackground(const std::string&);
void stopBackground();

}; // namespace AudioManager