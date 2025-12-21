//
// Created by 曾暐勛 on 2025/12/21.
//

#ifndef FONTSOFPROJECT_GAMESCENE2_2FF_H
#define FONTSOFPROJECT_GAMESCENE2_2FF_H


#include "GameChapter.h"
#include "StoryManager.h"


class ChapterScene2_2FF : public GameChapter
{
public:
    ChapterScene2_2FF();
    void load() override;
    void handleEvent(SDL_Event& e) override;
    void update() override;
    void render(int screenW, int screenH) override;
    bool isFinished() override;
    void clean() override;

private:
    StoryManager mStoryManager;
};



#endif //FONTSOFPROJECT_GAMESCENE2_2FF_H