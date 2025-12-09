#ifndef SOUNDEFFECT_H
#define SOUNDEFFECT_H

#include <SDL_mixer.h>
#include <string>
#include <map>
#include <iostream>

class SoundEffect {
public:
    SoundEffect();
    ~SoundEffect();

    bool load(const std::string& name, const std::string& path);

    void play(const std::string& name);

    void clean();

private:
    std::map<std::string, Mix_Chunk*> mSoundMap;
    //用Map管理音效，每有一個音效就塞進去且加上名字
};

#endif