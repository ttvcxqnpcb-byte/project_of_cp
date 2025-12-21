#include "GameScene4_FT.h"

void ChapterScene4_FT::load()
{
    // 把這裡改成你的劇本路徑
    if (!mStoryManager.loadScript("assets/txt/scene4ft.txt")) {
        printf("Load script failed!\n");
    }
}

void ChapterScene4_FT::handleEvent(SDL_Event& e)
{
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

void ChapterScene4_FT::update()
{
    mStoryManager.update();
}

void ChapterScene4_FT::render(int screenW, int screenH)
{
    mStoryManager.render(screenW, screenH);
}

bool ChapterScene4_FT::isFinished()
{
    return mStoryManager.isFinished();
}

void ChapterScene4_FT::clean()
{
}
