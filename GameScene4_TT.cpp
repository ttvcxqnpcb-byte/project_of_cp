#include "GameScene4_TT.h"

void ChapterScene4_TT::load()
{
    // 把這裡改成你的劇本路徑
    if (!mStoryManager.loadScript("assets/txt/scene4tt.txt")) {
        printf("Load script failed!\n");
    }
}

void ChapterScene4_TT::handleEvent(SDL_Event& e)
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

void ChapterScene4_TT::update()
{
    mStoryManager.update();
}

void ChapterScene4_TT::render(int screenW, int screenH)
{
    mStoryManager.render(screenW, screenH);
}

bool ChapterScene4_TT::isFinished()
{
    return mStoryManager.isFinished();
}

void ChapterScene4_TT::clean()
{
}
