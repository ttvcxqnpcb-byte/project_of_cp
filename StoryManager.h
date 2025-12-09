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
    void update();
    void render(int screenW, int screenH);
    bool isFinished();
    std::string getCurrentLine();

protected:
    void updateTexture(); // 更新對話內容文字

    void parseLine(std::string rawLine);

    bool parseTag(std::string line);
    int getUTF8CharLength(char c);

    std::vector<std::string> mLines;
    int mCurrentLineIndex;

    LTexture mDialogueTexture;   // 對話內容圖片
    LTexture mNameTexture;       // 名字圖片
    LTexture mBackgroundTexture;

    Character mCharacter;
    BackgroundMusic mMusicPlayer;

    bool mIsTyping;
    bool isDiaVisible;
    std::string mTargetText;
    std::string mShowText;
    size_t mByteIndex;
    Uint32 mLastUpdateTime;
    Uint32 mTypeSpeed;
};

#endif