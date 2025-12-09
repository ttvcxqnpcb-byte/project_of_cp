//
// Created by 曾暐勛 on 2025/12/9.
//

#ifndef FONTSOFPROJECT_GAMEPROLOGUE_H
#define FONTSOFPROJECT_GAMEPROLOGUE_H
#include "StoryManager.h"
#include "GameChapter.h"
#include <stdio.h>

class GamePrologue : public GameChapter {
    public:
    GamePrologue();
    ~GamePrologue() override;

    void load() override;

    void handleEvent(SDL_Event& e) override;

    void update() override;

    void render(int w, int h) override;

    bool isFinished() override;

    void clean() override;

    private:
    StoryManager prologue;
};

#endif //FONTSOFPROJECT_GAMEPROLOGUE_H