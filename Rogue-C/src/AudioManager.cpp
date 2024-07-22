#include "AudioManager.h"

std::vector<Sound> AudioManager::_sounds;
std::vector<Music> AudioManager::_music;

SoundClip AudioManager::RegisterSound(const char* path) {
    _sounds.emplace_back(LoadSound(path));
    return _sounds.size() - 1;
}

MusicClip AudioManager::RegisterMusic(const char* path) {
    _music.emplace_back(LoadMusicStream(path));
    return _music.size() - 1;
}

void AudioManager::Play(const SoundClip& sound) {
    PlaySound(_sounds[sound]);
}

void AudioManager::PlayMusic(const MusicClip& music) {
    PlayMusicStream(_music[music]);
}

void AudioManager::UnloadAll() {
    for(auto const& sound : _sounds) {
        UnloadSound(sound);
    }

    for(auto const& music : _music) {
        UnloadMusicStream(music);
    }
}
