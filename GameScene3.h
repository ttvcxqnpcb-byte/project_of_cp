#ifndef GAMESCENE3_H
#define GAMESCENE3_H

#include "GameChapter.h"
#include "StoryManager.h"

class ChapterScene3 : public GameChapter
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
