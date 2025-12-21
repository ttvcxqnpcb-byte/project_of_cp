#ifndef GAME_TITLE_H
#define GAME_TITLE_H

#include "GameChapter.h"
#include "LTexture.h"
#include "BackgroundMusic.h" // 【修改】改用你的音樂管理類別
#include <vector>
#include <string>

enum class TitleState {
    MAIN_MENU,
    CHAPTER_SELECT,
    INSTRUCTION
};

class GameTitle : public GameChapter
{
public:
    GameTitle();
    ~GameTitle();

    void load() override;
    void handleEvent(SDL_Event& e) override;
    void update() override;
    void render(int w, int h) override;
    void clean() override;
    bool isFinished() override;

private:
    TitleState mState;
    bool mIsFinished;
    int mCurrentPage;
    const int ITEMS_PER_PAGE = 4;

    LTexture mBgTexture;
    LTexture mTitleText;
    LTexture mInstructionTexture;
    LTexture mBtnStart;
    LTexture mBtnChapters;
    LTexture mBtnHelp;
    LTexture mBtnBack;
    LTexture mBtnNextPage;
    LTexture mBtnPrevPage;
    LTexture mHelpContent;
    BackgroundMusic mBGM;

    std::vector<LTexture*> mChapterBtnTextures;
    std::vector<int> mChapterTargetSteps;

    SDL_Rect mRectStart;
    SDL_Rect mRectChapters;
    SDL_Rect mRectHelp;
    SDL_Rect mRectBack;
    SDL_Rect mRectNextPage;
    SDL_Rect mRectPrevPage;
    std::vector<SDL_Rect> mVisibleChapterRects;

    bool checkCollision(int x, int y, SDL_Rect rect);

    SDL_Rect drawMCButton(LTexture* texture, int cx, int cy, int wScale, int hScale, int mx, int my);

    void renderTextWithOutline(LTexture* texture, int x, int y, int w, int h, int outlineSize = 2);
};

#endif