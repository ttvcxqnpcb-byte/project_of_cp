#include "BackgroundMusic.h"
#include <iostream>

BackgroundMusic::BackgroundMusic() : _bgm(NULL) {}

BackgroundMusic::~BackgroundMusic() {
    close();
}

bool BackgroundMusic::init() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Mixer Init Error: " << Mix_GetError() << std::endl;
        return false;
    }
    Mix_VolumeMusic(32);
    return true;
}

bool BackgroundMusic::playMusic(const std::string& path) {
    // 如果有舊音樂，先釋放
    if (_bgm != NULL) {
        Mix_FreeMusic(_bgm);
    }

    _bgm = Mix_LoadMUS(path.c_str());
    if (_bgm == NULL) {
        std::cerr << "Music Load Error: " << Mix_GetError() << std::endl;
        return false;
    }

    // -1 無限循環
    Mix_PlayMusic(_bgm, -1);
    std::cout << "Music Playing: " << path << std::endl;
    return true;
}

void BackgroundMusic::stop() {
    Mix_HaltMusic();
}

void BackgroundMusic::close() {
    if (_bgm) {
        Mix_FreeMusic(_bgm);
        _bgm = NULL;
    }

    _sfxManager.clean();

    Mix_Quit();
}