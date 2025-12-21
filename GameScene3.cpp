#include "GameScene3.h"

void ChapterScene3::load()
{
    // 把這裡改成你的劇本路徑
    if (!mStoryManager.loadScript("assets/txt/scene3.txt")) {
        printf("Load script failed!\n");
    }
}

void ChapterScene3::handleEvent(SDL_Event& e)
{
    // 如果有特殊按鍵邏輯 (如解謎) 寫在這裡
    // 預設只需處理劇情推進
    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_RETURN) {
            mStoryManager.handleContinue();
        }
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        mStoryManager.handleContinue();
    }
}

void ChapterScene3::update()
{
    mStoryManager.update();
}

void ChapterScene3::render(int screenW, int screenH)
{
    mStoryManager.render(screenW, screenH);
}

bool ChapterScene3::isFinished()
{
    return mStoryManager.isFinished();
}

void ChapterScene3::clean()
{
}
