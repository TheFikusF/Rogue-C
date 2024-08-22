#include "./include/core/audio/AudioManager.h"

std::vector<SoundChanel> AudioManager::_soundChanels{ 0 };
std::vector<Sound> AudioManager::_sounds[MAX_CHANELS];
std::vector<Music> AudioManager::_music;

SoundClip AudioManager::RegisterSound(const char* path) {
    for(SoundChanel i = 0; i < MAX_CHANELS; i++) {
        _sounds[i].emplace_back(LoadSound(path));
    }
    return _sounds[0].size() - 1;
}

MusicClip AudioManager::RegisterMusic(const char* path) {
    _music.emplace_back(LoadMusicStream(path));
    return _music.size() - 1;
}

void AudioManager::Play(const SoundClip& sound) {
    if(_soundChanels[sound] == MAX_CHANELS) {
        _soundChanels[sound] = 0;
    }
    PlaySound(_sounds[_soundChanels[sound]++][sound]);
}

void AudioManager::StopSoundOnCh(const SoundClip& sound, const SoundChanel& chanel) {
    if(IsSoundPlaying(_sounds[chanel][sound]) == false) {
        return;
    }

    StopSound(_sounds[chanel][sound]);
}

void AudioManager::PlayMusic(const MusicClip& music) {
    PlayMusicStream(_music[music]);
}

void AudioManager::UnloadAll() {
    for(auto const& chanel : _sounds) {
        for(auto const& sound : chanel) {
            UnloadSound(sound);
        }
    }

    for(auto const& music : _music) {
        UnloadMusicStream(music);
    }
}
