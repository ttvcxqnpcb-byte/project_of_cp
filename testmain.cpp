#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h> // 記得加入這個
#include <stdio.h>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

// 引用
#include "GameChapter.h"
#include "StoryManager.h"
#include "ChapterTemplate.h" 

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// 全域變數
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont = NULL;
GameChapter* gChapter = NULL;

// 為了除錯，把 init 放在這裡簡化
bool init() {
    printf("STEP 01: SDL_Init Start\n"); fflush(stdout);
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 ) {
        printf( "SDL Init Failed! %s\n", SDL_GetError() );
        return false;
    }

    printf("STEP 02: CreateWindow\n"); fflush(stdout);
    // Windows 暫時拿掉 HighDPI 以免座標錯亂
    gWindow = SDL_CreateWindow( "Debug Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if( gWindow == NULL ) {
        printf( "Window Create Failed! %s\n", SDL_GetError() );
        return false;
    }

    printf("STEP 03: CreateRenderer\n"); fflush(stdout);
    gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    if( gRenderer == NULL ) {
        printf( "Renderer Create Failed! %s\n", SDL_GetError() );
        return false;
    }

    printf("STEP 04: IMG_Init\n"); fflush(stdout);
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
        printf( "IMG_Init Failed! %s\n", IMG_GetError() );
        return false;
    }

    printf("STEP 05: TTF_Init\n"); fflush(stdout);
    if( TTF_Init() == -1 ) {
        printf( "TTF_Init Failed! %s\n", TTF_GetError() );
        return false;
    }
    
    printf("STEP 06: Mix_Init\n"); fflush(stdout);
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
        printf( "Mixer Init Failed! %s\n", Mix_GetError() );
        return false;
    }

    return true;
}

bool loadFonts() {
    printf("STEP 07: Loading Font...\n"); fflush(stdout);
    // 這裡請確認路徑！如果不確定，先用 ./assets/...
    gFont = TTF_OpenFont( "assets/font/lazy.ttf", 28 ); 
    if( gFont == NULL ) {
        printf( "ERROR: Failed to load lazy.ttf! SDL_ttf Error: %s\n", TTF_GetError() );
        return false;
    }
    printf("STEP 08: Font Loaded Successfully\n"); fflush(stdout);
    return true;
}

void close() {
    if (gChapter) { delete gChapter; gChapter = NULL; }
    if (gFont) { TTF_CloseFont(gFont); gFont = NULL; }
    if (gRenderer) { SDL_DestroyRenderer(gRenderer); gRenderer = NULL; }
    if (gWindow) { SDL_DestroyWindow(gWindow); gWindow = NULL; }
    
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main( int argc, char* args[] ) {
    // 強制開啟 Console (Windows)
    #ifdef _WIN32
        AllocConsole();
        freopen("CON", "w", stdout);
        freopen("CON", "w", stderr);
    #endif

    printf("DEBUG: Program Started.\n"); fflush(stdout);

    if( !init() ) {
        printf( "Failed to initialize!\n" );
        system("pause");
        return -1;
    }

    if( !loadFonts() ) {
        printf( "Failed to load fonts!\n" );
        system("pause");
        return -1;
    }

    printf("STEP 09: Creating Chapter\n"); fflush(stdout);
    // 這裡會呼叫 StoryManager 建構子
    gChapter = new ChapterTemplate();

    printf("STEP 10: Loading Chapter Script\n"); fflush(stdout);
    // 這裡會呼叫 StoryManager::loadScript
    gChapter->load();

    printf("STEP 11: Entering Main Loop\n"); fflush(stdout);
    
    bool quit = false;
    SDL_Event e;

    while( !quit ) {
        while( SDL_PollEvent( &e ) != 0 ) {
            if( e.type == SDL_QUIT ) quit = true;
            if (gChapter) gChapter->handleEvent(e);
            if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE ) quit = true;
        }

        SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 255 );
        SDL_RenderClear( gRenderer );

        if (gChapter) {
            // 這裡會呼叫 StoryManager::render
            // 如果這裡崩潰，代表 updateTexture 還是有問題
            gChapter->update(); 
            gChapter->render(SCREEN_WIDTH, SCREEN_HEIGHT);
        }

        SDL_RenderPresent( gRenderer );
    }

    close();
    return 0;
}
