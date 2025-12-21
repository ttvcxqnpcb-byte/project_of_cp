#include "GameTitle.h"
#include "GlobalVars.h"
#include <iostream>
#include <algorithm>

extern SDL_Renderer* gRenderer;
extern TTF_Font* gFont;
extern int currentStep;
extern SDL_Window* gWindow;

GameTitle::GameTitle() : mState(TitleState::MAIN_MENU), mIsFinished(false), mCurrentPage(0) {}
GameTitle::~GameTitle() { clean(); }

void GameTitle::load() {
    mIsFinished = false;
    mState = TitleState::MAIN_MENU;
    mCurrentPage = 0;

    SDL_Color white = {255, 255, 255};

    if (!mBgTexture.loadFromFile("assets/img/background/dian2_entrance_night.png")) {
        printf("Background image not found, using black screen.\n");
    }

    if (!mBGM.playMusic("assets/music/Heather.mp3")) {
        printf("Failed to play title music!\n");
    }

    // 載入點擊音效
    mBGM._sfxManager.load("click", "assets/sound/click.wav");

    // ==========================================
    // 【新增】載入操作說明圖片
    // 請確認副檔名是 .png 還是 .jpg (這裡預設 .png)
    // ==========================================
    if (!mInstructionTexture.loadFromFile("assets/img/puzzle/instruction.png")) {
        printf("Failed to load instruction image!\n");
    }

    // 3. 載入文字
    mTitleText.loadFromRenderedText("台大電機謀殺案", white);
    mBtnStart.loadFromRenderedText("重新開始", white);
    mBtnChapters.loadFromRenderedText("章節選擇", white);
    mBtnHelp.loadFromRenderedText("操作說明", white);
    mBtnBack.loadFromRenderedText("返回主選單", white);

    mBtnNextPage.loadFromRenderedText("下一頁 >", white);
    mBtnPrevPage.loadFromRenderedText("< 上一頁", white);

    // 這行原本是用文字顯示說明的，現在有圖片了，但留著也沒關係，當作備用
    mHelpContent.loadFromRenderedText("按 Space 對話，滑鼠點擊選擇", white);

    // 4. 載入章節按鈕
    auto addChapter = [&](const char* name, int id) {
        LTexture* t = new LTexture();
        t->loadFromRenderedText(name, white);
        mChapterBtnTextures.push_back(t);
        mChapterTargetSteps.push_back(id);
    };

    addChapter("序章", CH_PROLOGUE);
    addChapter("第一章", CH_SCENE1);
    addChapter("第二章", CH_SCENE2);
    addChapter("第三章", CH_SCENE3);
    addChapter("最終章", CH_SCENE4_FF);
}

void GameTitle::clean() {
    mBGM.stop();
    mBgTexture.free();
    mTitleText.free();
    mBtnStart.free();
    mBtnChapters.free();
    mBtnHelp.free();
    mBtnBack.free();
    mBtnNextPage.free();
    mBtnPrevPage.free();
    mHelpContent.free();

    // 【新增】釋放說明圖片
    mInstructionTexture.free();

    for(auto t : mChapterBtnTextures) {
        t->free();
        delete t;
    }
    mChapterBtnTextures.clear();
    mChapterTargetSteps.clear();
    mVisibleChapterRects.clear();
}

bool GameTitle::checkCollision(int x, int y, SDL_Rect rect) {
    return (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h);
}

// 輔助函式：繪製有黑色描邊的文字
void GameTitle::renderTextWithOutline(LTexture* texture, int x, int y, int w, int h, int outlineSize) {
    texture->setColor(0, 0, 0);

    texture->render(x - outlineSize, y, w, h);
    texture->render(x + outlineSize, y, w, h);
    texture->render(x, y - outlineSize, w, h);
    texture->render(x, y + outlineSize, w, h);
    texture->render(x - outlineSize, y - outlineSize, w, h);
    texture->render(x + outlineSize, y + outlineSize, w, h);
    texture->render(x + outlineSize, y - outlineSize, w, h);
    texture->render(x - outlineSize, y + outlineSize, w, h);

    texture->setColor(255, 255, 255);
    texture->render(x, y, w, h);
}

// Minecraft 風格按鈕
SDL_Rect GameTitle::drawMCButton(LTexture* texture, int cx, int cy, int wScale, int hScale, int mx, int my) {
    float textRatio = (float)texture->getWidth() / texture->getHeight();
    int textH = (int)(hScale * 0.6f);
    int textW = (int)(textH * textRatio);
    int boxW = textW + (int)(hScale * 1.5f);
    int boxH = hScale;
    int boxX = cx - boxW / 2;
    int boxY = cy - boxH / 2;
    SDL_Rect btnRect = { boxX, boxY, boxW, boxH };

    bool isHovered = checkCollision(mx, my, btnRect);

    if (isHovered) {
        SDL_SetRenderDrawColor(gRenderer, 100, 100, 100, 255);
        texture->setColor(255, 255, 255);
    } else {
        SDL_SetRenderDrawColor(gRenderer, 220, 220, 220, 255);
        texture->setColor(0, 0, 0);
    }

    SDL_RenderFillRect(gRenderer, &btnRect);

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_Rect borderRect = btnRect;
    SDL_RenderDrawRect(gRenderer, &borderRect);
    borderRect.x += 2; borderRect.y += 2; borderRect.w -= 4; borderRect.h -= 4;
    SDL_RenderDrawRect(gRenderer, &borderRect);

    texture->render(cx - textW / 2, cy - textH / 2, textW, textH);

    return btnRect;
}

void GameTitle::handleEvent(SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        int windowW, windowH;
        SDL_GetWindowSize(gWindow, &windowW, &windowH);
        int drawableW, drawableH;
        SDL_GetRendererOutputSize(gRenderer, &drawableW, &drawableH);

        if (windowW > 0 && windowH > 0) {
            float scaleX = (float)drawableW / windowW;
            float scaleY = (float)drawableH / windowH;
            mx = (int)(mx * scaleX);
            my = (int)(my * scaleY);
        }

        if (mState == TitleState::MAIN_MENU) {
            if (checkCollision(mx, my, mRectStart)) {
                mBGM._sfxManager.play("click");
                resetGameVars();
                gTitleTarget = CH_PROLOGUE;
                mIsFinished = true;
            }
            else if (checkCollision(mx, my, mRectChapters)) {
                mBGM._sfxManager.play("click");
                mState = TitleState::CHAPTER_SELECT;
                mCurrentPage = 0;
            }
            else if (checkCollision(mx, my, mRectHelp)) {
                mBGM._sfxManager.play("click");
                mState = TitleState::INSTRUCTION;
            }
        }
        else if (mState == TitleState::CHAPTER_SELECT) {
            int startIdx = mCurrentPage * ITEMS_PER_PAGE;
            for (size_t i = 0; i < mVisibleChapterRects.size(); i++) {
                if (checkCollision(mx, my, mVisibleChapterRects[i])) {
                    mBGM._sfxManager.play("click");
                    int realIndex = startIdx + i;
                    if (realIndex < mChapterTargetSteps.size()) {
                        gTitleTarget = mChapterTargetSteps[realIndex];
                        mIsFinished = true;
                        return;
                    }
                }
            }
            if (mCurrentPage > 0 && checkCollision(mx, my, mRectPrevPage)) {
                mBGM._sfxManager.play("click");
                mCurrentPage--;
            }
            if ((mCurrentPage + 1) * ITEMS_PER_PAGE < mChapterBtnTextures.size() && checkCollision(mx, my, mRectNextPage)) {
                mBGM._sfxManager.play("click");
                mCurrentPage++;
            }
            if (checkCollision(mx, my, mRectBack)) {
                mBGM._sfxManager.play("click");
                mState = TitleState::MAIN_MENU;
            }
        }
        else if (mState == TitleState::INSTRUCTION) {
            if (checkCollision(mx, my, mRectBack)) {
                mBGM._sfxManager.play("click");
                mState = TitleState::MAIN_MENU;
            }
        }
    }
}

void GameTitle::update() {}

void GameTitle::render(int w, int h) {
    if (mBgTexture.getWidth() > 0) {
        mBgTexture.render(0, 0, w, h);
    } else {
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
        SDL_Rect bg = {0, 0, w, h};
        SDL_RenderFillRect(gRenderer, &bg);
    }

    int centerX = w / 2;
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    int windowW, windowH;
    SDL_GetWindowSize(gWindow, &windowW, &windowH);
    if (windowW > 0) {
        mx = (int)(mx * ((float)w / windowW));
        my = (int)(my * ((float)h / windowH));
    }

    int btnTargetH = (int)(h * 0.12f);
    int titleTargetH = (int)(h * 0.15f);

    if (mState == TitleState::MAIN_MENU) {

        if (mTitleText.getWidth() > 0) {
            float scale = (float)titleTargetH / mTitleText.getHeight();
            int drawW = (int)(mTitleText.getWidth() * scale);
            int outline = (int)(h * 0.005f); if (outline < 2) outline = 2;
            renderTextWithOutline(&mTitleText, centerX - drawW/2, (int)(h * 0.15f), drawW, titleTargetH, outline);
        }

        int startY = (int)(h * 0.5f);
        int gap = (int)(h * 0.15f);

        mRectStart = drawMCButton(&mBtnStart, centerX, startY, btnTargetH, btnTargetH, mx, my);
        mRectChapters = drawMCButton(&mBtnChapters, centerX, startY + gap, btnTargetH, btnTargetH, mx, my);
        mRectHelp = drawMCButton(&mBtnHelp, centerX, startY + gap * 2, btnTargetH, btnTargetH, mx, my);
    }

    else if (mState == TitleState::CHAPTER_SELECT) {

        float tScale = (float)titleTargetH / mBtnChapters.getHeight();
        int tDrawW = (int)(mBtnChapters.getWidth() * tScale);
        int outline = (int)(h * 0.005f); if (outline < 2) outline = 2;
        renderTextWithOutline(&mBtnChapters, centerX - tDrawW/2, (int)(h * 0.1f), tDrawW, titleTargetH, outline);

        int startY = (int)(h * 0.35f);
        int gap = (int)(h * 0.12f);

        mVisibleChapterRects.clear();
        int startIdx = mCurrentPage * ITEMS_PER_PAGE;
        int endIdx = std::min((int)mChapterBtnTextures.size(), startIdx + ITEMS_PER_PAGE);

        for (int i = startIdx; i < endIdx; ++i) {
            LTexture* t = mChapterBtnTextures[i];
            int posY = startY + (i - startIdx) * gap;
            SDL_Rect r = drawMCButton(t, centerX, posY, btnTargetH, btnTargetH, mx, my);
            mVisibleChapterRects.push_back(r);
        }

        int navY = (int)(h * 0.85f);
        int navBtnH = (int)(h * 0.08f);

        if (mCurrentPage > 0) mRectPrevPage = drawMCButton(&mBtnPrevPage, centerX - (int)(w * 0.3f), navY, navBtnH, navBtnH, mx, my);
        else mRectPrevPage = {0, 0, 0, 0};

        mRectBack = drawMCButton(&mBtnBack, centerX, navY, navBtnH, navBtnH, mx, my);

        if ((mCurrentPage + 1) * ITEMS_PER_PAGE < mChapterBtnTextures.size()) mRectNextPage = drawMCButton(&mBtnNextPage, centerX + (int)(w * 0.3f), navY, navBtnH, navBtnH, mx, my);
        else mRectNextPage = {0, 0, 0, 0};
    }

    else if (mState == TitleState::INSTRUCTION) {

        float tScale = (float)titleTargetH / mBtnHelp.getHeight();
        int tDrawW = (int)(mBtnHelp.getWidth() * tScale);
        int outline = (int)(h * 0.005f); if (outline < 2) outline = 2;
        renderTextWithOutline(&mBtnHelp, centerX - tDrawW/2, (int)(h * 0.1f), tDrawW, titleTargetH, outline);

        int navY = (int)(h * 0.85f);

        if (mInstructionTexture.getWidth() > 0) {

            int topY = (int)(h * 0.25f);
            int bottomY = navY - (int)(h * 0.05f);

            int availableH = bottomY - topY;
            int availableW = (int)(w * 0.8f);

            float scaleW = (float)availableW / mInstructionTexture.getWidth();
            float scaleH = (float)availableH / mInstructionTexture.getHeight();
            float finalScale = (scaleW < scaleH) ? scaleW : scaleH;

            finalScale *= 0.85f;

            int drawW = (int)(mInstructionTexture.getWidth() * finalScale);
            int drawH = (int)(mInstructionTexture.getHeight() * finalScale);

            int drawX = centerX - drawW / 2;
            int drawY = topY + (availableH - drawH) / 2;

            mInstructionTexture.render(drawX, drawY, drawW, drawH);
        }
        else {
            int contentH = (int)(h * 0.05f);
            float scale = (float)contentH / mHelpContent.getHeight();
            int drawW = (int)(mHelpContent.getWidth() * scale);
            renderTextWithOutline(&mHelpContent, centerX - drawW/2, (int)(h * 0.4f), drawW, contentH, 2);
        }

        mRectBack = drawMCButton(&mBtnBack, centerX, navY, btnTargetH, btnTargetH, mx, my);
    }
}
bool GameTitle::isFinished() {
    return mIsFinished;
}