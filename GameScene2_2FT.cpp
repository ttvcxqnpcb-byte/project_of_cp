#include "GameScene2_2FT.h"
#include "GlobalVars.h"

ChapterScene2_2FT::ChapterScene2_2FT() {};

void ChapterScene2_2FT::load()
{
    if (!mStoryManager.loadScript("assets/txt/scene2_2ft.txt")) {
        printf("Load script failed!\n");
    }
}

void ChapterScene2_2FT::handleEvent(SDL_Event& e)
{
    if (e.type == SDL_KEYDOWN) {
        if (mStoryManager.isAtLastLine()) {
            if (e.key.keysym.sym == SDLK_y) {
                isPictureGet = true;
                mStoryManager.handleContinue();
            }
            else if (e.key.keysym.sym == SDLK_n) {
                isPictureGet = false;
                mStoryManager.handleContinue();
            }
        }
        else {
            switch (e.key.keysym.sym) {
                case SDLK_UP:
                    mStoryManager.handleBack();
                    break;
                case SDLK_RETURN:
                case SDLK_DOWN:
                case SDLK_SPACE:
                    mStoryManager.handleContinue();
            }
        }
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        if (!mStoryManager.isFinished()) mStoryManager.handleContinue();
    }

}

void ChapterScene2_2FT::update()
{
    mStoryManager.update();
}

void ChapterScene2_2FT::render(int screenW, int screenH)
{
    mStoryManager.render(screenW, screenH);
}

bool ChapterScene2_2FT::isFinished()
{
    return mStoryManager.isFinished();
}

void ChapterScene2_2FT::clean()
{
}
