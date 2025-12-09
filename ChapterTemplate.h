#ifndef CHAPTER_TEMPLATE_H
#define CHAPTER_TEMPLATE_H

#include "GameChapter.h"
#include "StoryManager.h"

// 給組員的說明：
// 1. 請把類別名稱 ChapterTemplate 改成你的章節名稱 (例如 ChapterInvestigate)
// 2. 記得同時修改 .cpp 裡的名稱
class ChapterTemplate : public GameChapter
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