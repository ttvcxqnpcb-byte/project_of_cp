//
// Created by 曾暐勛 on 2025/12/21.
//

#ifndef GAMECHAPTER2_2TT_H
#define GAMECHAPTER2_1TT_H
#include "GameChapter.h"
#include "StoryManager.h"

class ChapterScene2_2TT : public GameChapter
{
public:
    ChapterScene2_2TT();
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