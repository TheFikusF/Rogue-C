#pragma once
#include "./include/raylib/raylib.h"
#include <cstdint>
#include <vector>

using SoundChanel = std::uint8_t;
using SoundClip = std::size_t;
using MusicClip = std::size_t;

class AudioManager
{
public:
    static const SoundChanel MAX_CHANELS = 16; 

private:
    static std::vector<SoundChanel> _soundChanels;
    static std::vector<Sound> _sounds[MAX_CHANELS];
    static std::vector<Music> _music;

public:
    static SoundClip RegisterSound(const char* path);
    static MusicClip RegisterMusic(const char* path);
    static void Play(const SoundClip& sound);
    static void StopSoundOnCh(const SoundClip& sound, const SoundChanel& chanel);
    static void PlayMusic(const MusicClip& music);
    static void UnloadAll();
};

