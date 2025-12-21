
#ifndef FONTSOFPROJECT_GAMESCENE2_2FT_H
#define FONTSOFPROJECT_GAMESCENE2_2FT_H

#include "GameChapter.h"
#include "StoryManager.h"


class ChapterScene2_2FT : public GameChapter
{
public:
    ChapterScene2_2FT();
    void load() override;
    void handleEvent(SDL_Event& e) override;
    void update() override;
    void render(int screenW, int screenH) override;
    bool isFinished() override;
    void clean() override;

private:
    StoryManager mStoryManager;
};



#endif //FONTSOFPROJECT_GAMESCENE2_2FT_H