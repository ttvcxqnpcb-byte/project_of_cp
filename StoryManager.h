#ifndef STORY_MANAGER_H
#define STORY_MANAGER_H

#include <string>
#include <vector>
#include <fstream>
#include "LTexture.h"
#include "Character.h"
#include "BackgroundMusic.h"

extern SDL_Renderer* gRenderer;
extern TTF_Font* gFont;

class StoryManager
{
public:
    StoryManager();
    ~StoryManager();

    bool loadScript(std::string path);
    void handleContinue();
    void handleBack();
    void update();
    void render(int screenW, int screenH);
    bool isFinished();
    std::string getCurrentLine();

    bool checkPasswordRequest() {
        if (mRequestPassword) {
            mRequestPassword = false;
            return true;
        }
        return false;
    }
    bool isAtLastLine();

protected:
    void updateTexture();

    void parseLine(std::string rawLine);

    bool parseTag(std::string line);
    bool parseBackslashTag(std::string line);
    int getUTF8CharLength(char c);

    std::vector<std::string> mLines;
    int mCurrentLineIndex;

    LTexture mDialogueTexture;
    LTexture mNameTexture;
    LTexture mBackgroundTexture;
    LTexture mPuzzleTexture;

    Character mCharacter;
    BackgroundMusic mMusicPlayer;

    bool mRequestPassword;
    bool mIsTyping;
    bool isDiaVisible;
    std::string mTargetText;
    std::string mShowText;
    size_t mByteIndex;
    Uint32 mLastUpdateTime;
    Uint32 mTypeSpeed;

    int mSoundCounter;

    bool mIsTempBG;
    bool mIsTempChar;
    bool mIsTempPuzzle;
};

#endif