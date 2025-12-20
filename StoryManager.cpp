#include "StoryManager.h"
#include <iostream>
#include <algorithm> 

StoryManager::StoryManager() : mCurrentLineIndex(-1), mIsTyping(false), mByteIndex(0), mTypeSpeed(50), mLastUpdateTime(0), isDiaVisible(true), mIsTempBG(false), mIsTempChar(false), mIsTempPuzzle(false) {
    if (!mMusicPlayer.init()) {
        printf("Warning: Audio system failed to initialize!\n");
    }
}

StoryManager::~StoryManager() {}

bool StoryManager::loadScript(std::string path)
{
    printf("DEBUG: Loading script from %s...\n", path.c_str());
    fflush(stdout); 

    std::ifstream file(path.c_str());
    if (!file.is_open())
    {
        std::string backupPath = "./" + path;
        file.open(backupPath.c_str());

        if (!file.is_open()) {
            printf("ERROR: Unable to open file: %s\n", path.c_str());
            fflush(stdout);
            return false;
        }
    }

    mLines.clear();
    std::string line;
    bool isFirstLine = true;

    while (std::getline(file, line))
    {
        if (isFirstLine && line.size() >= 3) {
            if ((unsigned char)line[0] == 0xEF && (unsigned char)line[1] == 0xBB && (unsigned char)line[2] == 0xBF) {
                printf("DEBUG: Detected BOM in first line, removing it.\n");
                line.erase(0, 3);
            }
            isFirstLine = false;
        }

        while (!line.empty() && (line.back() == '\r' || line.back() == '\n' || line.back() == ' ')) {
            line.pop_back();
        }
        
        if (!line.empty()) {
            mLines.push_back(line);
        }
    }
    file.close();

    printf("DEBUG: Loaded %d lines.\n", (int)mLines.size());
    fflush(stdout);

    isDiaVisible = true;
    mCurrentLineIndex = -1;
    mBackgroundTexture.free();
    mPuzzleTexture.free();
    mCharacter.free();
    mNameTexture.free();
    mCharacter.setScale(0.9f);

    mIsTempBG = false;
    mIsTempChar = false;
    mIsTempPuzzle = false;

    handleContinue();

    return true;
}

void StoryManager::handleBack()
{
    if (mIsTempBG) { mBackgroundTexture.free(); mIsTempBG = false; }
    if (mIsTempChar) { mCharacter.free(); mIsTempChar = false; }
    if (mIsTempPuzzle) { mPuzzleTexture.free(); mIsTempPuzzle = false; }

    if (mLines.empty() || mCurrentLineIndex <= 0) return;

    int searchIndex = mCurrentLineIndex - 1;
    while (searchIndex >= 0)
    {
        std::string line = mLines[searchIndex];
        bool isSquareTag = (line.size() > 2 && line.front() == '[' && line.back() == ']');
        if (!isSquareTag) break;
        searchIndex--;
    }

    mCurrentLineIndex = (searchIndex < 0) ? -1 : (searchIndex - 1);
    mIsTyping = false;
    mShowText = "";
    mByteIndex = 0;

    handleContinue();
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
        if (mIsTempBG) { mBackgroundTexture.free(); mIsTempBG = false; }
        if (mIsTempChar) { mCharacter.free(); mIsTempChar = false; }
        if (mIsTempPuzzle) { mPuzzleTexture.free(); mIsTempPuzzle = false; }

        mCurrentLineIndex++;
        while (!isFinished())
        {
            std::string line = mLines[mCurrentLineIndex];
            
            if (parseTag(line)) {
                mCurrentLineIndex++;
            }
            else if (parseBackslashTag(line)) {
                mCurrentLineIndex++;
                return;
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
    if (rawLine.empty()) return;

    if (rawLine[0] == '(') {
        std::string content = rawLine.substr(1);
        if (!content.empty() && content.back() == ')') content.pop_back();
        mTargetText = content;
        mNameTexture.free();
        return;
    }
    if (rawLine.size() >= 3 && rawLine.substr(0, 3) == "（") {
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

    if (colonPos != std::string::npos && colonPos < 18) {
        name = rawLine.substr(0, colonPos);
        content = rawLine.substr(colonPos + skipLength);
    }

    mTargetText = content;
    mNameTexture.free();

    if (!name.empty()) {
        SDL_Color nameColor = { 255, 255, 0 }; 
        mNameTexture.loadFromRenderedText(name, nameColor);
    }
}

bool StoryManager::parseTag(std::string line)
{
    if (line.size() > 2 && line.front() == '[' && line.back() == ']')
    {
        std::string tag = line.substr(1, line.size() - 2);

        if (tag.find("BG_") == 0) {
            std::string name = tag.substr(3);
            mBackgroundTexture.loadFromFile("assets/img/background/" + name + ".png");
            return true;
        }
        if (tag.find("CHAR_") == 0) {
            std::string fileName = tag.substr(5);
            std::string folderName = fileName;
            size_t underscorePos = fileName.find('_');
            if (underscorePos != std::string::npos) folderName = fileName.substr(0, underscorePos);

            if (folderName == "xiaoxun" || folderName == "xiaode") mCharacter.setScale(0.85f);
            else if (folderName == "senpai") mCharacter.setScale(0.95f);
            else mCharacter.setScale(0.9f);

            mCharacter.loadFromFile("assets/img/" + folderName + "/" + fileName + ".png");
            return true;
        }
        if (tag.find("MUSIC_") == 0) {
            std::string name = tag.substr(6);
            if (name == "stop") mMusicPlayer.stop();
            else mMusicPlayer.playMusic("assets/music/" + name + ".mp3");
            return true;
        }
        if (tag.find("SFX_") == 0) {
            std::string name = tag.substr(4);
            mMusicPlayer._sfxManager.load(name, "assets/sound/" + name + ".wav");
            mMusicPlayer._sfxManager.play(name);
            return true;
        }
        if (tag == "CLEAR") { mCharacter.free(); mPuzzleTexture.free(); return true; }
        if (tag == "HIDE") { isDiaVisible = false; return true; }
        if (tag == "SHOW") { isDiaVisible = true; return true; }
    }
    return false;
}

bool StoryManager::parseBackslashTag(std::string line)
{
    if (line.size() > 2 && line.front() == '\\' && line.back() == '\\')
    {
        std::string tag = line.substr(1, line.size() - 2);

        if (tag.find("puzzle_") == 0) {
            std::string name = tag.substr(7);
            mPuzzleTexture.loadFromFile("assets/img/puzzle/" + name + ".png");
            mIsTempPuzzle = true;
            return true;
        }
        if (tag.find("BG_") == 0) {
            std::string name = tag.substr(3);
            mBackgroundTexture.loadFromFile("assets/img/background/" + name + ".png");
            mIsTempBG = true;
            return true;
        }
        if (tag.find("CHAR_") == 0) {
            std::string fileName = tag.substr(5);
            std::string folderName = fileName;
            size_t underscorePos = fileName.find('_');
            if (underscorePos != std::string::npos) folderName = fileName.substr(0, underscorePos);

            if (folderName == "xiaoxun" || folderName == "xiaode") mCharacter.setScale(0.85f);
            else if (folderName == "senpai") mCharacter.setScale(0.95f);
            else mCharacter.setScale(0.9f);

            mCharacter.loadFromFile("assets/img/" + folderName + "/" + fileName + ".png");
            mIsTempChar = true;
            return true;
        }
        if (tag.find("MUSIC_") == 0) {
            std::string name = tag.substr(6);
            if (name == "stop") mMusicPlayer.stop();
            else mMusicPlayer.playMusic("assets/music/" + name + ".mp3");
            return true;
        }
        if (tag.find("SFX_") == 0) {
            std::string name = tag.substr(4);
            mMusicPlayer._sfxManager.load(name, "assets/sound/" + name + ".wav");
            mMusicPlayer._sfxManager.play(name);
            return true;
        }
        if (tag == "CLEAR") { mCharacter.free(); mPuzzleTexture.free(); return true; }
        if (tag == "HIDE") { isDiaVisible = false; return true; }
        if (tag == "SHOW") { isDiaVisible = true; return true; }
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

    if (w <= 0 || h <= 0) return;

    float scale = (float)h / 960.0f;
    if (scale <= 0.001f) scale = 0.5f; // 防呆，避免除以零

    int boxMargin = w * 0.05;
    int borderThickness = w * 0.01;
    int textPadding = w * 0.03;

    int availableWidth = w - (boxMargin * 2) - (borderThickness * 2) - (textPadding * 2);
    if (availableWidth <= 0) availableWidth = 100; // 防呆

    Uint32 wrapLimit = (Uint32)(availableWidth / scale);

    mDialogueTexture.loadFromRenderedTextWrapped(text, textColor, wrapLimit);
}

void StoryManager::render(int screenW, int screenH)
{
    if (isFinished()) return;
    
    if (screenW <= 0 || screenH <= 0) return;

    float globalScale = (float)screenH / 960.0f;

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

    if (mPuzzleTexture.getWidth() > 0) {
        int bgW = mPuzzleTexture.getWidth();
        int bgH = mPuzzleTexture.getHeight();
        float scaleX = (float)screenW / bgW;
        float scaleY = (float)screenH / bgH;
        float scale = (scaleX > scaleY) ? scaleX : scaleY;
        int drawW = (int)(bgW * scale);
        int drawH = (int)(bgH * scale);
        int drawX = (screenW - drawW) / 2;
        int drawY = (screenH - drawH) / 2;
        mPuzzleTexture.render(drawX, drawY, drawW, drawH);
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

        if (mNameTexture.getWidth() > 0)
        {
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

            mNameTexture.render(nameBoxX + namePadding, nameBoxY + namePadding, nameTexW, nameTexH);
        }

        int textX = boxMargin + borderThickness + textPadding;
        int textY = boxY + borderThickness + textPadding;

        int textDrawW = mDialogueTexture.getWidth() * globalScale;
        int textDrawH = mDialogueTexture.getHeight() * globalScale;

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
