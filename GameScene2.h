#ifndef GAME_SCENE2_H
#define GAME_SCENE2_H

#include "GameChapter.h"
#include "StoryManager.h"
#include "PasswordPuzzle.h" // 引用密碼鎖

class ChapterScene2 : public GameChapter
{
public:
    ChapterScene2();
    ~ChapterScene2();
    void load() override;
    void handleEvent(SDL_Event& e) override;
    void update() override;
    void render(int w, int h) override;
    void clean() override;
    bool isFinished() override;

private:
    StoryManager mStoryManager;
    PasswordPuzzle mPasswordPuzzle;
    bool mIsFinished;
};
#endif