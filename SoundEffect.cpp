#include "SoundEffect.h"

SoundEffect::SoundEffect() {}

SoundEffect::~SoundEffect() {
    clean();
}

bool SoundEffect::load(const std::string& name, const std::string& path) {
    //把這個map拿來當快取，如果重複播放這個音效那就拿之前用過的來用
    if (mSoundMap.find(name) != mSoundMap.end()) {
        return true;
    }

    Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
    if (chunk == NULL) {
        std::cerr << "Failed to load SFX: " << path << " Error: " << Mix_GetError() << std::endl;
        return false;
    }

    // 存入map，下次再用到 "name" 時就可以直接拿來用
    mSoundMap[name] = chunk;
    std::cout << "Loaded new SFX into memory: " << name << std::endl; // 除錯用，讓你看到它只載入一次
    return true;
}

void SoundEffect::play(const std::string& name) {
    auto it = mSoundMap.find(name);
    if (it != mSoundMap.end()) {
        Mix_PlayChannel(-1, it->second, 0);
    } else {
        std::cerr << "SFX not found: " << name << std::endl;
    }
}

void SoundEffect::clean() {
    // 釋放所有音效
    for (auto const& [name, chunk] : mSoundMap) {
        Mix_FreeChunk(chunk);
    }
    mSoundMap.clear();
}