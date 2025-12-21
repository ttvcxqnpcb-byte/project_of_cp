#ifndef GAMESCENE4_TF_H
#define GAMESCENE4_TF_H

#include "GameChapter.h"
#include "StoryManager.h"

class ChapterScene4_TF : public GameChapter
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
