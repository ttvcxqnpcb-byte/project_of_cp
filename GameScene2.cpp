#include "GameScene2.h"

ChapterScene2::ChapterScene2() : mIsFinished(false){}
ChapterScene2::~ChapterScene2() { clean(); }

void ChapterScene2::load() {
    mStoryManager.loadScript("assets/txt/scene2.txt");
    mPasswordPuzzle.init("1945"); // 設定密碼
    mIsFinished = false; // 繼承自 GameChapter 的變數
}

void ChapterScene2::clean() {
    // 資源釋放
}

void ChapterScene2::handleEvent(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_UP) {

        if (mPasswordPuzzle.isActive()) {
            mPasswordPuzzle.hide();
        }

        mStoryManager.handleBack();
        return;
    }

    if (mPasswordPuzzle.isActive() && mPasswordPuzzle.isActive()) {
        mPasswordPuzzle.handleEvent(e);
        return;
    }

    if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_RETURN)) {
        mStoryManager.handleContinue();
    }
}

void ChapterScene2::update() {

    if (mStoryManager.checkPasswordRequest()) {

        if (mPasswordPuzzle.isActive()) {
            mPasswordPuzzle.show();
        }
        else {
            mPasswordPuzzle.open();
        }
    }

    if (mPasswordPuzzle.hasAttempted()) {
        mIsFinished = true;
    }

    mStoryManager.update();
}

void ChapterScene2::render(int w, int h) {
    mStoryManager.render(w, h);
    mPasswordPuzzle.render(w, h);
}
bool ChapterScene2::isFinished() {
    return mIsFinished;
}