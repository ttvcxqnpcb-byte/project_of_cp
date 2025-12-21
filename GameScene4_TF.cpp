#include "GameScene4_TF.h"

void ChapterScene4_TF::load()
{
    // 把這裡改成你的劇本路徑
    if (!mStoryManager.loadScript("assets/txt/scene4tf.txt")) {
        printf("Load script failed!\n");
    }
}

void ChapterScene4_TF::handleEvent(SDL_Event& e)
{
    // 如果有特殊按鍵邏輯 (如解謎) 寫在這裡
    // 預設只需處理劇情推進
    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_RETURN) {
            mStoryManager.handleContinue();
        }
        if (mStoryManager.isAtLastLine())   mStoryManager.handleContinue();
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        mStoryManager.handleContinue();
    }
}

void ChapterScene4_TF::update()
{
    mStoryManager.update();
}

void ChapterScene4_TF::render(int screenW, int screenH)
{
    mStoryManager.render(screenW, screenH);
}

bool ChapterScene4_TF::isFinished()
{
    return mStoryManager.isFinished();
}

void ChapterScene4_TF::clean()
{
}
