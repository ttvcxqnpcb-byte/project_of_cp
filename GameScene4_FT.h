#ifndef GAMESCENE4_FT_H
#define GAMESCENE4_FT_H

#include "GameChapter.h"
#include "StoryManager.h"

class ChapterScene4_FT : public GameChapter
{
public:
    void load() override;
    void handleEvent(SDL_Event& e) override;
    void update() override;
    void render(int screenW, int screenH) override;
    bool isFinished() override;
    void clean() override;

private:
    StoryManager mStoryManager;
};

#endif
