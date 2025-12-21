#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

#include "GlobalVars.h"
#include "GameChapter.h"
#include "LTexture.h"
#include "GameTitle.h"
#include "GamePrologue.h"
#include "GameScene1.h"
#include "GameScene2.h"
#include "GameScene2_1F.h"
#include "GameScene2_1T.h"
#include "GameScene2_2TT.h"
#include "GameScene2_2TF.h"
#include "GameScene2_2FT.h"
#include "GameScene2_2FF.h"
#include "GameScene3.h"
#include "GameScene4_FF.h"
#include "GameScene4_FT.h"
#include "GameScene4_TF.h"
#include "GameScene4_TT.h"


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool isComputerOpen = false;
bool isPictureGet = false;
bool istheStoryFinished = false;
int gTitleTarget = -1;

bool init();
bool loadFonts();
void close();

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont = NULL;
GameChapter* gChapter = NULL;


bool init()
{
    bool success = true;
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 ) // 建議加上 INIT_AUDIO
    {
       printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
       success = false;
    }
    else
    {
       // 設定線性過濾
       if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
       {
          printf( "Warning: Linear texture filtering not enabled!" );
       }

       gWindow = SDL_CreateWindow( "台大電機謀殺案", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI );

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

             // 初始化 PNG
             int imgFlags = IMG_INIT_PNG;
             if( !( IMG_Init( imgFlags ) & imgFlags ) )
             {
                printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                success = false;
             }

             // 初始化 TTF
             if( TTF_Init() == -1 )
             {
                printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                success = false;
             }

             int mixFlags = MIX_INIT_MP3;
             if( (Mix_Init( mixFlags ) & mixFlags) != mixFlags )
             {
                 printf( "SDL_mixer Init Warning: %s\n", Mix_GetError() );
             }

             // 開啟音效裝置 (這行保持你原本的設定)
             if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
             {
                printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
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
    gFont = TTF_OpenFont( "assets/font/zpix.ttf", 56 );
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
   }

   TTF_CloseFont( gFont );
   gFont = NULL;
   SDL_DestroyRenderer( gRenderer );
   SDL_DestroyWindow( gWindow );
   gWindow = NULL;
   gRenderer = NULL;
   gChapter = NULL;

   Mix_Quit();
   TTF_Quit();
   IMG_Quit();
   SDL_Quit();
}
void resetGameVars() {
   isComputerOpen = false;
   isPictureGet = false;
   istheStoryFinished = false;
}

int currentStep = CH_TITLE;

void changeChapter(int step)
{
   if (gChapter != NULL) {
      gChapter->clean();
      delete gChapter;
      gChapter = NULL;
   }

   switch (step) {
         case CH_TITLE:
            gChapter = new GameTitle();
            break;
         case CH_PROLOGUE:
            gChapter = new ChapterPrologue();
            break;
         case CH_SCENE1:
            gChapter = new ChapterScene1();
            break;
         case CH_SCENE2:
            gChapter = new ChapterScene2();
            break;
         case CH_SCENE2_1T:
            gChapter = new ChapterScene2_1T();
            break;
         case CH_SCENE2_1F:
            gChapter = new ChapterScene2_1F();
            break;
         case CH_SCENE2_2TT:
            gChapter = new ChapterScene2_2TT();
            break;
         case CH_SCENE2_2TF:
            gChapter = new ChapterScene2_2TF();
            break;
         case CH_SCENE2_2FT:
            gChapter = new ChapterScene2_2FT();
            break;
         case CH_SCENE2_2FF:
            gChapter = new ChapterScene2_2FF();
            break;
         case CH_SCENE3:
            gChapter = new ChapterScene3();
            break;
         case CH_SCENE4_TT:
            gChapter = new ChapterScene4_TT();
            istheStoryFinished = true;
            break;
         case CH_SCENE4_TF:
            gChapter = new ChapterScene4_TF();
            istheStoryFinished = true;
            break;
         case CH_SCENE4_FT:
            gChapter = new ChapterScene4_FT();
            istheStoryFinished = true;
            break;
         case CH_SCENE4_FF:
            gChapter = new ChapterScene4_FF();
            istheStoryFinished = true;
      default:
         break;
   }

   if (gChapter != NULL) {
      gChapter->load();
   }
}

void ChangeChapter() {
   if (currentStep == CH_TITLE) {}
   if (currentStep == CH_PROLOGUE || currentStep == CH_SCENE1) {
      currentStep++;
   }
   else if (currentStep == CH_SCENE2) {
      if (isComputerOpen) {
         currentStep = CH_SCENE2_1T;
      }
      else {
         currentStep = CH_SCENE2_1F;
      }
   }
   else if (currentStep == CH_SCENE2_1T) {
      if (isPictureGet) {
         currentStep = CH_SCENE2_2TT;
      }
      else {
         currentStep = CH_SCENE2_2TF;
      }
   }
   else if (currentStep == CH_SCENE2_1F) {
      if (isPictureGet) {
         currentStep = CH_SCENE2_2FT;
      }
      else {
         currentStep = CH_SCENE2_2FF;
      }
   }
   else if (currentStep <= CH_SCENE2_2FF && currentStep >= CH_SCENE2_2TT) {
      currentStep = CH_SCENE3;
   }
   else if (currentStep == CH_SCENE3) {
      if (isComputerOpen) {
         if (isPictureGet) {
            currentStep = CH_SCENE4_TT;
         }
         else {
            currentStep = CH_SCENE4_TF;
         }
      }
      else {
         if (isPictureGet) {
            currentStep = CH_SCENE4_FT;
         }
         else {
            currentStep = CH_SCENE4_FF;
         }
      }
   }
   changeChapter(currentStep);
}

int main( int argc, char* args[] )
{
    if( !init() )
    {
       printf( "Failed to initialize!\n" );
    }
    else
    {
       if (!loadFonts()) {
          printf( "Failed to load fonts!\n" );
       }
       else
       {
          bool quit = false;
          SDL_Event e;
          bool isFullscreen = false;
          // 抓取真實的像素寬高
          int drawableW, drawableH;
          changeChapter(currentStep);

          while( !quit )
          {
             while( SDL_PollEvent( &e ) != 0 )
             {
                if (gChapter != NULL) {
                   gChapter->handleEvent(e);
                }
                if( e.type == SDL_QUIT )
                {
                   quit = true;
                }
                if( e.type == SDL_KEYDOWN ) {
                   switch( e.key.keysym.sym ) {
                      case SDLK_f:
                         printf("F Key Pressed!\n");
                         isFullscreen = !isFullscreen; // 切換狀態 (True/False 反轉)

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

             if (gChapter != NULL)
             {
                gChapter->update();
                if (gChapter->isFinished())
                {
                   if (currentStep == CH_TITLE) {
                       if (gTitleTarget != -1) {
                           currentStep = gTitleTarget;
                           changeChapter(currentStep);
                           gTitleTarget = -1;
                       }
                   }
                   else {
                       if (!istheStoryFinished) {
                          ChangeChapter();
                       }
                       else {
                          currentStep = CH_TITLE;
                          changeChapter(currentStep);
                          resetGameVars();
                       }
                   }
                }
             }

             SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 255 );
             SDL_RenderClear( gRenderer );

             SDL_GetRendererOutputSize(gRenderer, &drawableW, &drawableH);

             gChapter->render(drawableW, drawableH);
             
             SDL_RenderPresent( gRenderer );
          }
       }
    }
    close();
    return 0;
}