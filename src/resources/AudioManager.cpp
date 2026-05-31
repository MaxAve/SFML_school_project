#include "resources/AudioManager.hpp"
#include <iostream>
#include <random>
#define LOG(message) std::cout << message << std::endl

std::unordered_map<std::string, sf::SoundBuffer> AudioManager::soundBuffer;
std::unordered_map<std::string, sf::Sound> AudioManager::stringToSound;

// sf::Music stored as pointers to avoid copying streams
std::unordered_map<std::string, sf::Music*> AudioManager::stringToBackground; // background sound
std::unordered_map<std::string, sf::Music*> AudioManager::stringToMusic;      // game music

sf::Music* AudioManager::curMusic;
sf::Music* AudioManager::curBackground; // background sounds

void AudioManager::init() {
    std::vector<std::string> sounds{
        "click",
        "footstep",
        "pickup_sound",
        "reload_bullet",
        "shot_ar",
        "shot_pistol",
        "shot_shotgun",
        "shot_sniper",
        "zombie_1",
        "zombie_2"};

    std::vector<std::string> musics{
        "Bück Dich Cover (Action)",
        "Cheap House Of The Rising Sun",
        "Untitled 1",
        "Untitled 2",
        "Untitled 3",
        "Untitled 4"};

    std::vector<std::string> backgroundSounds{
        "background_day",
        "background_night"};

    std::string path = "resources/sounds/";

    for (const auto& sound : sounds) {
        LOG("[LOG] Loading " + path + sound + ".wav");
        if (!soundBuffer[sound].loadFromFile(path + sound + ".wav")) {
            LOG("[ERR] Couldn't load the file " + path + sound + ".wav");
            continue;
        }
        stringToSound.insert({sound, sf::Sound(soundBuffer.at(sound))});
        auto it = stringToSound.find(sound);
        it->second.setRelativeToListener(true);
        it->second.setPosition({0.f, 0.f, 0.f});
    }
    // reduce volumes
    stringToSound.at("footstep").setVolume(5.f);
    stringToSound.at("pickup_sound").setVolume(30.f);
    for (const auto& sound : sounds) {
        std::string prefix = "shot_";
        if (sound.substr(0, prefix.length()) == prefix) {
            stringToSound.at(sound).setVolume(25.f);
        }
    }

    for (const auto& sound : backgroundSounds) {
        LOG("[LOG] Loading " + path + sound + ".ogg");
        sf::Music* ptr = new sf::Music;
        if (ptr && !ptr->openFromFile(path + sound + ".ogg")) {
            LOG("[ERR] Couldn't load the file " + path + sound + ".ogg");
            delete ptr;
            continue;
        }
        ptr->setLooping(true);
        ptr->setRelativeToListener(true);
        ptr->setPosition({0.f, 0.f, 0.f});
        stringToBackground.insert({sound, ptr});
    }

    path = "resources/music/";

    for (const auto& music : musics) {
        LOG("[LOG] Loading " + path + music + ".ogg");
        sf::Music* ptr = new sf::Music;
        if (ptr && !ptr->openFromFile(path + music + ".ogg")) {
            LOG("[ERR] Couldn't load the file " + path + music + ".ogg");
            delete ptr;
            continue;
        }
        ptr->setRelativeToListener(true);
        ptr->setPosition({0.f, 0.f, 0.f});
        stringToMusic.insert({music, ptr});
    }
}

// the function from main, sorry for this ugly way
float randomFloat(float, float);

void AudioManager::playSound(const std::string& soundName) {
    auto it = stringToSound.find(soundName);

    if (it == stringToSound.end()) {
        LOG("Sound not found: " + soundName);
        return;
    }

    it->second.setPitch(randomFloat(0.8f, 1.f));

    it->second.play();
}

void AudioManager::playMusic(const std::string& musicName) {
    if (curMusic) {
        curMusic->stop();
        curMusic = nullptr;
    }

    auto it = stringToMusic.find(musicName);
    if (it == stringToMusic.end()) {
        LOG("Music not found: " + musicName);
        return;
    }

    curMusic = it->second;

    curMusic->setVolume(75.0f);
    curMusic->play();
}

void AudioManager::playRandomMusic() {
    if (curMusic) {
        curMusic->stop();
        curMusic = nullptr;
    }

    // bit faster than vector
    std::array<std::string, 6> musicNames{
        "Bück Dich Cover (Action)",
        "Cheap House Of The Rising Sun",
        "Untitled 1",
        "Untitled 2",
        "Untitled 3",
        "Untitled 4"};

    size_t randIdx = static_cast<size_t>(randomFloat(0, musicNames.size())); // sorry about that

    LOG("playRandMusic()");
    playMusic(musicNames[randIdx]);
}

void AudioManager::stopMusic() {
    if (!curMusic) {
        LOG("No music to stop");
        return;
    }

    curMusic->stop();
    curMusic = nullptr;
}

void AudioManager::playBackground(const std::string& backgroundName) {
    if (curBackground) {
        curBackground->stop();
        curBackground = nullptr;
    }

    auto it = stringToBackground.find(backgroundName);
    if (it == stringToBackground.end()) {
        LOG("Background not found: " + backgroundName);
        return;
    }

    curBackground = it->second;

    curBackground->setVolume(40.0f);
    curBackground->play();
}

void AudioManager::stopBackground() {
    if (!curBackground) {
        LOG("No background sounds to stop");
        return;
    }

    curBackground->stop();
    curBackground = nullptr;
}