#include "StoryManager.h"
#include <iostream>

StoryManager::StoryManager() : mCurrentLineIndex(-1), mIsTyping(false), mByteIndex(0), mTypeSpeed(50), mLastUpdateTime(0), isDiaVisible(true) {
    if (!mMusicPlayer.init()) {
        printf("Warning: Audio system failed to initialize!\n");
    }
}

StoryManager::~StoryManager() {}

bool StoryManager::loadScript(std::string path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        printf("Unable to open the txt file: %s\n", path.c_str());
        return false;
    }

    mLines.clear();
    std::string line;
    while (std::getline(file, line))
    {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (!line.empty()) {
            mLines.push_back(line);
        }
    }
    file.close();

    isDiaVisible = true;
    mCurrentLineIndex = -1;
    mBackgroundTexture.free();
    mCharacter.free();
    mNameTexture.free();
    mCharacter.setScale(0.9f);

    handleContinue();

    return true;
}

void StoryManager::handleContinue()
{
    if (mIsTyping)
    {
        mShowText = mTargetText;
        mIsTyping = false;
        updateTexture();
    }
    else
    {
        mCurrentLineIndex++;
        while (!isFinished())
        {
            std::string line = mLines[mCurrentLineIndex];
            if (parseTag(line)) {
                mCurrentLineIndex++;
            }
            else {
                break;
            }
        }

        if (!isFinished())
        {
            parseLine(mLines[mCurrentLineIndex]);
            mShowText = "";
            mByteIndex = 0;
            mIsTyping = true;
            mLastUpdateTime = SDL_GetTicks();
        }
    }
}

void StoryManager::parseLine(std::string rawLine)
{

    if (!rawLine.empty() && rawLine[0] == '(')
    {
        std::string content = rawLine.substr(1);

        if (!content.empty() && content.back() == ')') {
            content.pop_back();
        }

        mTargetText = content;
        mNameTexture.free();
        return;
    }

    if (rawLine.find("（") == 0)
    {
        std::string content = rawLine.substr(3);

        if (content.size() >= 3 && content.substr(content.size() - 3) == "）") {
            content = content.substr(0, content.size() - 3);
        }

        mTargetText = content;
        mNameTexture.free();
        return;
    }


    std::string name = "";
    std::string content = rawLine;
    size_t colonPos = std::string::npos;
    int skipLength = 0;

    size_t fullColon = rawLine.find("：");
    if (fullColon != std::string::npos) {
        colonPos = fullColon;
        skipLength = 3;
    }
    else {
        size_t halfColon = rawLine.find(":");
        if (halfColon != std::string::npos) {
            colonPos = halfColon;
            skipLength = 1;
        }
    }

    // 名字長度限制 (防呆)
    if (colonPos != std::string::npos && colonPos < 18)
    {
        name = rawLine.substr(0, colonPos);
        content = rawLine.substr(colonPos + skipLength);
    }
    else
    {
        name = "";
        content = rawLine;
    }

    mTargetText = content;
    mNameTexture.free();

    if (!name.empty()) {
        SDL_Color nameColor = { 255, 255, 0 }; // 黃色名字
        mNameTexture.loadFromRenderedText(name, nameColor);
    }
}

bool StoryManager::parseTag(std::string line)
{
    if (line.size() > 2 && line.front() == '[' && line.back() == ']')
    {
        std::string tag = line.substr(1, line.size() - 2);

        if (tag.find("BG_") == 0)
        {
            std::string name = tag.substr(3);
            std::string path = "assets/img/background/" + name + ".png";
            if (!mBackgroundTexture.loadFromFile(path)) {
                printf("Failed to load BG: %s\n", path.c_str());
            }
            return true;
        }
        if (tag.find("CHAR_") == 0)
        {
            std::string fileName = tag.substr(5);
            std::string folderName = fileName;
            size_t underscorePos = fileName.find('_');
            if (underscorePos != std::string::npos) {
                folderName = fileName.substr(0, underscorePos);
            }

            if (folderName == "xiaoxun" || folderName == "xiaode") {
                mCharacter.setScale(0.85f);
            }
            else if (folderName == "senpai") {
                mCharacter.setScale(0.95f);
            }
            else {
                mCharacter.setScale(0.9f);
            }

            std::string path = "assets/img/" + folderName + "/" + fileName + ".png";
            if (!mCharacter.loadFromFile(path)) {
                printf("Failed to load CHAR: %s\n", path.c_str());
            }
            return true;
        }
        if (tag.find("MUSIC_") == 0)
        {
            // [MUSIC_bgm01] -> assets/music/bgm01.mp3
            std::string name = tag.substr(6);
            if (name == "stop") {
                mMusicPlayer.stop();
                return true;
            }

            std::string path = "assets/music/" + name + ".mp3";

            // 呼叫音樂
            mMusicPlayer.playMusic(path);

            return true;
        }
        if (tag.find("SFX_") == 0) {
            std::string name = tag.substr(4);
            std::string path = "assets/sound/" + name + ".wav";

            mMusicPlayer._sfxManager.load(name, path);
            mMusicPlayer._sfxManager.play(name);

            return true;
        }
        if (tag == "CLEAR")
        {
            mCharacter.free();
            return true;
        }
        if (tag == "HIDE") {
            isDiaVisible = false;
            return true;
        }
        if (tag == "SHOW") {
            isDiaVisible = true;
            return true;
        }
    }
    return false;
}

void StoryManager::update()
{
    if (!mIsTyping || isFinished()) return;

    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - mLastUpdateTime > mTypeSpeed)
    {
        if (mByteIndex < mTargetText.length())
        {
            char firstByte = mTargetText[mByteIndex];
            int charLen = getUTF8CharLength(firstByte);
            mShowText += mTargetText.substr(mByteIndex, charLen);
            mByteIndex += charLen;
            updateTexture();
            mLastUpdateTime = currentTime;
        }
        else
        {
            mIsTyping = false;
        }
    }
}

void StoryManager::updateTexture()
{
    if (isFinished()) return;

    std::string text = mShowText;
    if (text.empty()) text = " ";

    SDL_Color textColor = { 255, 255, 255 };

    int w, h;
    SDL_GetRendererOutputSize(gRenderer, &w, &h);

    float scale = (float)h / 960.0f;
    if (scale < 1.0f) scale = 1.0f; // 避免太小

    int boxMargin = w * 0.05;
    int borderThickness = w * 0.01;
    int textPadding = w * 0.03;


    Uint32 wrapLimit = w - (boxMargin * 2) - (borderThickness * 2) - (textPadding * 2);

    wrapLimit = (Uint32)(wrapLimit / scale);

    mDialogueTexture.loadFromRenderedTextWrapped(text, textColor, wrapLimit);
}

void StoryManager::render(int screenW, int screenH)
{
    if (isFinished()) return;

    // 計算全域縮放比例
    float globalScale = (float)screenH / 960.0f;

    // 背景
    if (mBackgroundTexture.getWidth() > 0) {
        int bgW = mBackgroundTexture.getWidth();
        int bgH = mBackgroundTexture.getHeight();
        float scaleX = (float)screenW / bgW;
        float scaleY = (float)screenH / bgH;
        float scale = (scaleX > scaleY) ? scaleX : scaleY;
        int drawW = (int)(bgW * scale);
        int drawH = (int)(bgH * scale);
        int drawX = (screenW - drawW) / 2;
        int drawY = (screenH - drawH) / 2;
        mBackgroundTexture.render(drawX, drawY, drawW, drawH);
    }

    // 人物
    int boxHeight = screenH * 0.35;
    int boxY = screenH - boxHeight - (screenH * 0.05);

    if (mCharacter.getWidth() > 0) {
        int targetHeight = screenH * mCharacter.getScale();
        double ratio = (double)mCharacter.getWidth() / mCharacter.getHeight();
        int targetWidth = targetHeight * ratio;
        int charX = (screenW - targetWidth) / 2;
        int charY = screenH - targetHeight;
        mCharacter.render(charX, charY, targetWidth, targetHeight);
    }

    if (isDiaVisible) {
        int boxMargin = screenW * 0.05;
        int boxWidth = screenW - (boxMargin * 2);
        int borderThickness = screenW * 0.005;
        if (borderThickness < 2) borderThickness = 2;
        int textPadding = screenW * 0.03;

        SDL_Rect rect1 = { boxMargin, boxY, boxWidth, boxHeight };
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        SDL_RenderFillRect(gRenderer, &rect1);

        SDL_Rect rect2 = {
            boxMargin + borderThickness,
            boxY + borderThickness,
            boxWidth - (borderThickness * 2),
            boxHeight - (borderThickness * 2)
        };
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
        SDL_RenderFillRect(gRenderer, &rect2);

        // 名字框
        if (mNameTexture.getWidth() > 0)
        {
            // 名字圖片
            int nameTexW = mNameTexture.getWidth() * globalScale;
            int nameTexH = mNameTexture.getHeight() * globalScale;

            int namePadding = textPadding / 2;
            int nameBoxW = nameTexW + (namePadding * 2);
            int nameBoxH = nameTexH + (namePadding * 2);

            int nameBoxX = boxMargin;
            int nameBoxY = boxY - nameBoxH;

            SDL_Rect nameRect1 = { nameBoxX, nameBoxY, nameBoxW, nameBoxH };
            SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
            SDL_RenderFillRect(gRenderer, &nameRect1);

            SDL_Rect nameRect2 = {
                nameBoxX + borderThickness,
                nameBoxY + borderThickness,
                nameBoxW - (borderThickness * 2),
                nameBoxH - (borderThickness * 2)
            };
            SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
            SDL_RenderFillRect(gRenderer, &nameRect2);

            // 畫名字
            mNameTexture.render(nameBoxX + namePadding, nameBoxY + namePadding, nameTexW, nameTexH);
        }

        // 對話內容
        int textX = boxMargin + borderThickness + textPadding;
        int textY = boxY + borderThickness + textPadding;

        // (原圖大小 * 縮放比例)
        int textDrawW = mDialogueTexture.getWidth() * globalScale;
        int textDrawH = mDialogueTexture.getHeight() * globalScale;

        // 使用 LTexture 的縮放繪圖功能
        mDialogueTexture.render(textX, textY, textDrawW, textDrawH);
    }
}

int StoryManager::getUTF8CharLength(char c)
{
    if ((c & 0x80) == 0) return 1;
    if ((c & 0xE0) == 0xC0) return 2;
    if ((c & 0xF0) == 0xE0) return 3;
    if ((c & 0xF8) == 0xF0) return 4;
    return 1;
}

bool StoryManager::isFinished()
{
    return mCurrentLineIndex >= mLines.size();
}

std::string StoryManager::getCurrentLine()
{
    if (isFinished()) return "";
    return mLines[mCurrentLineIndex];
}