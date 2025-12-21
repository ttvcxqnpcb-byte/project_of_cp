#ifndef PASSWORD_PUZZLE_H
#define PASSWORD_PUZZLE_H

#include <string>
#include <SDL.h>
#include "LTexture.h"

class PasswordPuzzle {
public:
    PasswordPuzzle();
    ~PasswordPuzzle();

    void init(std::string correctAnswer);
    void handleEvent(SDL_Event& e);
    void render(int screenW, int screenH);
    
    void open();

    void hide() { mIsVisible = false; }
    void show() { mIsVisible = true; mIsActive = true; }

    bool isActive() { return mIsActive; }
    bool hasAttempted() { return mHasAttempted; }

private:
    std::string mCorrectAnswer;
    std::string mCurrentInput;
    bool mIsActive;
    bool mHasAttempted;

    bool mIsVisible;
    bool mIsFourBitsInput;

    LTexture mTextTexture;
};

#endif