#ifndef FONTSOFPROJECT_GAMECHAPTER_H
#define FONTSOFPROJECT_GAMECHAPTER_H

#include <SDL.h>

class GameChapter {
public:

    virtual ~GameChapter() {}

    virtual void load() = 0;

    virtual void handleEvent(SDL_Event& e) = 0;

    virtual void update() = 0;

    //在render這一塊請注意，因為原本的邏輯是需要取得現在的顯示長寬才可以等比例顯示，所以後面在override新的函式進去時，
    //請務必要記得記得要用到這兩個，而至於現在的長寬如何你們不需要在意，我在主回圈會直接傳入
    virtual void render(int currentWidth, int currentHeight) = 0;

    virtual bool isFinished() = 0;

    virtual void clean() = 0;

};

#endif //FONTSOFPROJECT_GAMECHAPTER_H