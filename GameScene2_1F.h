//
// Created by 曾暐勛 on 2025/12/21.
//

#ifndef GAMECHAPTER2_1F_H
#define GAMECHAPTER2_1F_H
#include "GameChapter.h"
#include "StoryManager.h"
#include "GlobalVars.h"

class ChapterScene2_1F : public GameChapter
{
public:
    ChapterScene2_1F();
    void load() override;
    void handleEvent(SDL_Event& e) override;
    void update() override;
    void render(int screenW, int screenH) override;
    bool isFinished() override;
    void clean() override;

private:
    StoryManager mStoryManager;
};


#endif //FONTSOFPROJECT_GAMECHAPTER2_1T_H