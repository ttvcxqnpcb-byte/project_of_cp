#ifndef BACKGROUNDMUSIC_H
#define BACKGROUNDMUSIC_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#include "SoundEffect.h"

class BackgroundMusic {
public:
    BackgroundMusic();
    ~BackgroundMusic();

    bool init();

    // 播放音樂 (自動循環)
    bool playMusic(const std::string& path);

    void stop();

    void close();

    SoundEffect _sfxManager;

private:
    Mix_Music* _bgm;
};

#endif