//
// Created by 曾暐勛 on 2025/12/9.
//

#ifndef GAMESCENE1_H
#define GAMESCENE1_H
#include "StoryManager.h"
#include "GameChapter.h"
#include <stdio.h>

class ChapterScene1 : public GameChapter {
public:
    ChapterScene1();
    ~ChapterScene1() override;

    void load() override;

    void handleEvent(SDL_Event& e) override;

    void update() override;

    void render(int w, int h) override;

    bool isFinished() override;

    void clean() override;

private:
    StoryManager prologue;
};

#endif