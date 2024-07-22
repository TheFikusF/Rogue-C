#pragma once
#include "./include/raylib/raylib.h"
#include <cstdint>
#include <vector>

using SoundClip = std::size_t;
using MusicClip = std::size_t;

class AudioManager
{
private:
    static std::vector<Sound> _sounds;
    static std::vector<Music> _music;

public:
    static SoundClip RegisterSound(const char* path);
    static MusicClip RegisterMusic(const char* path);
    static void Play(const SoundClip& sound);
    static void PlayMusic(const MusicClip& music);
    static void UnloadAll();
};

