#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>

// 核心引擎
#include "GameChapter.h"
#include "LTexture.h"
#include "StoryManager.h"

// 【重點】這裡只引用模板章節，方便組員測試
#include "ChapterTemplate.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();
bool loadFonts();
void close();

// 全域變數
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont = NULL;
GameChapter* gChapter = NULL;

bool init()
{
    bool success = true;
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
       printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
       success = false;
    }
    else
    {
       // 設定線性過濾 (抗鋸齒)
       if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
       {
          printf( "Warning: Linear texture filtering not enabled!" );
       }

       // 開啟 HighDPI 支援 Retina
       gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI );

       if( gWindow == NULL )
       {
          printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
          success = false;
       }
       else
       {
          gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
          if( gRenderer == NULL )
          {
             printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
             success = false;
          }
          else
          {
             SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
             int imgFlags = IMG_INIT_PNG;
             if( !( IMG_Init( imgFlags ) & imgFlags ) )
             {
                printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                success = false;
             }
             if( TTF_Init() == -1 )
             {
                printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                success = false;
             }
          }
       }
    }

    return success;
}

bool loadFonts()
{
    bool success = true;
    // 載入高清字體
    gFont = TTF_OpenFont( "assets/font/zpix.ttf", 64 );
    if( gFont == NULL )
    {
       printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
       success = false;
    }
    return success;
}

void close()
{
    if (gChapter != NULL) {
       gChapter->clean();
       delete gChapter;
       gChapter = NULL;
    }

    TTF_CloseFont( gFont );
    gFont = NULL;

    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main( int argc, char* args[] )
{
    if( !init() )
    {
       printf( "Failed to initialize!\n" );
       return -1;
    }

    if (!loadFonts()) {
        printf( "Failed to load fonts!\n" );
        return -1;
    }

    // ==========================================
    // 【測試重點】直接建立模板章節
    // 組員寫完他們的 ChapterTemplate 後，按執行就會跑這裡
    // ==========================================
    gChapter = new ChapterTemplate();
    gChapter->load();

    bool quit = false;
    SDL_Event e;
    bool isFullscreen = false;
    int drawableW, drawableH;

    while( !quit )
    {
        while( SDL_PollEvent( &e ) != 0 )
        {
            if( e.type == SDL_QUIT )
            {
               quit = true;
            }

            // 讓章節處理事件
            if (gChapter != NULL) {
               gChapter->handleEvent(e);
            }

            if( e.type == SDL_KEYDOWN ) {
               switch( e.key.keysym.sym ) {
                  case SDLK_f:
                     isFullscreen = !isFullscreen;
                     if( isFullscreen ) {
                        SDL_SetWindowFullscreen( gWindow, SDL_WINDOW_FULLSCREEN_DESKTOP );
                     }
                     else {
                        SDL_SetWindowFullscreen( gWindow, 0 );
                     }
                     break;

                  case SDLK_ESCAPE:
                     quit = true;
                     break;
               }
            }
        }

        // 更新邏輯
        if (gChapter != NULL)
        {
            gChapter->update();

            // 測試模式下，如果結束了就直接退出
            if (gChapter->isFinished())
            {
                printf("--- 測試結束：本章節已完成 ---\n");
                quit = true;
            }
        }

        // 繪圖
        SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 255 );
        SDL_RenderClear( gRenderer );

        if (gChapter != NULL) {
            // 自動抓取 Retina / 全螢幕 解析度
            SDL_GetRendererOutputSize(gRenderer, &drawableW, &drawableH);
            gChapter->render(drawableW, drawableH);
        }

        SDL_RenderPresent( gRenderer );
    }

    close();
    return 0;
}