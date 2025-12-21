#include "PasswordPuzzle.h"
#include "GlobalVars.h"
#include <iostream>

extern SDL_Renderer* gRenderer;
extern TTF_Font* gFont;

PasswordPuzzle::PasswordPuzzle() : mIsActive(false), mHasAttempted(false), mIsVisible(false), mIsFourBitsInput(false) {}

PasswordPuzzle::~PasswordPuzzle() {
    mTextTexture.free();
}

void PasswordPuzzle::init(std::string correctAnswer) {
    mCorrectAnswer = correctAnswer;
    mCurrentInput = "";
    mIsActive = false;
    mHasAttempted = false;
    mIsVisible = false;
}

void PasswordPuzzle::open() {
    mIsActive = true;
    mIsVisible = true;
    mCurrentInput = "";
    mHasAttempted = false;
}

void PasswordPuzzle::handleEvent(SDL_Event& e) {
    if (!mIsActive || !mIsVisible) return;

    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_BACKSPACE && mCurrentInput.length() > 0) {
            mCurrentInput.pop_back();
            mIsFourBitsInput = false;
        }
        else if (mIsFourBitsInput && (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER)) {
            if (mCurrentInput == mCorrectAnswer) {
                printf("Password Correct!\n");
                isComputerOpen = true;
            } else {
                printf("Wrong Password!\n");
                isComputerOpen = false;
            }
            mHasAttempted = true;
            mIsActive = false;
        }
        else if (mCurrentInput.length() < 4) {
            bool isNumber = false;
            char num = ' ';
            if (e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9) num = '0' + (e.key.keysym.sym - SDLK_0);
            else if (e.key.keysym.sym >= SDLK_KP_0 && e.key.keysym.sym <= SDLK_KP_9) num = '0' + (e.key.keysym.sym - SDLK_KP_0);

            if (num != ' ') mCurrentInput += num;
            if (mCurrentInput.length() == 4) {
                mIsFourBitsInput = true;
            }
        }
    }
}

void PasswordPuzzle::render(int screenW, int screenH) {
    if (!mIsActive || !mIsVisible) return;


    int boxW = (int)(screenW * 0.85f);

    int boxH = (int)(screenH * 0.60f);

    int x = (screenW - boxW) / 2;
    int y = (screenH - boxH) / 2;

    SDL_Rect bgRect = { x, y, boxW, boxH };
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderFillRect(gRenderer, &bgRect);


    int borderThickness = (int)(screenW * 0.02f);
    if (borderThickness < 4) borderThickness = 4;

    SDL_Rect innerRect = {
        x + borderThickness,
        y + borderThickness,
        boxW - (borderThickness * 2),
        boxH - (borderThickness * 2)
    };
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderFillRect(gRenderer, &innerRect);

    if (gFont != NULL) {
        std::string displayStr = mCurrentInput;
        while(displayStr.length() < 4) {
            displayStr += "_";
        }

        std::string spacedStr = "";
        for(size_t i = 0; i < displayStr.length(); ++i) {
            spacedStr += displayStr[i];
            if (i < displayStr.length() - 1) {
                spacedStr += " ";
            }
        }

        SDL_Color textColor = { 255, 255, 255 };
        if (mTextTexture.loadFromRenderedText(spacedStr, textColor)) {

            float targetTextH = boxH * 0.4f;
            float scale = targetTextH / mTextTexture.getHeight();

            if (scale < 1.0f) scale = 1.0f;
            scale = floor(scale);

            int drawW = (int)(mTextTexture.getWidth() * scale);
            int drawH = (int)(mTextTexture.getHeight() * scale);

            int textX = x + (boxW - drawW) / 2;
            int textY = y + (boxH - drawH) / 2;

            mTextTexture.render(textX, textY, drawW, drawH);
        }
    }
}