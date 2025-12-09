#include "LTexture.h"
#include <SDL_image.h>

// Initialize LTexture
LTexture::LTexture() : mTexture(NULL), mWidth(0), mHeight(0)
{}

// 解構子
LTexture::~LTexture()
{
    free();
}

// 載入圖片 (使用全域 gRenderer)
bool LTexture::loadFromFile( std::string path )
{
    // 先清除舊的紋理
    free();

    // 最終要產生的紋理
    SDL_Texture* newTexture = NULL;

    // 1. 使用 SDL_image 載入圖片到 Surface (此時還是 CPU 記憶體)
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );

    if( loadedSurface == NULL )
    {
        printf( "無法載入圖片 %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );

        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s  SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        else
        {
            // 儲存圖片的寬高
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        SDL_FreeSurface( loadedSurface );
    }

    mTexture = newTexture;
    return mTexture != NULL;
}

bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
    free();

    SDL_Surface* textSurface = TTF_RenderUTF8_Blended( gFont, textureText.c_str(), textColor );
    if( textSurface == NULL )
    {
       printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
    {
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
       if( mTexture == NULL )
       {
          printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
       }
       else
       {
          mWidth = textSurface->w;
          mHeight = textSurface->h;
       }

       SDL_FreeSurface( textSurface );
    }

    return mTexture != NULL;
}

// 載入自動換行文字
bool LTexture::loadFromRenderedTextWrapped(std::string textureText, SDL_Color textColor, Uint32 wrapLength) {
    free();

    SDL_Surface* textSurface = TTF_RenderUTF8_Blended_Wrapped( gFont, textureText.c_str(), textColor, wrapLength);
    if( textSurface == NULL ) {
       printf( "Unable to render texture text! SDL Error: %s\n", TTF_GetError() );
    }
    else {
       mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
       if( mTexture == NULL )
       {
          printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
       }
       else
       {
          mWidth = textSurface->w;
          mHeight = textSurface->h;
       }

       SDL_FreeSurface( textSurface );
    }
    return mTexture != NULL;
}

// 釋放記憶體
void LTexture::free()
{
    if( mTexture != NULL )
    {
       SDL_DestroyTexture( mTexture );
       mTexture = NULL;
       mWidth = 0;
       mHeight = 0;
    }
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
    SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
    SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
    SDL_SetTextureAlphaMod( mTexture, alpha );
}

// 原始繪圖函式 (不縮放)
void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };

    if( clip != NULL )
    {
       renderQuad.w = clip->w;
       renderQuad.h = clip->h;
    }

    SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

// 縮放繪圖函式 (指定寬高 w, h) overloading
void LTexture::render( int x, int y, int w, int h )
{
    SDL_Rect renderQuad = { x, y, w, h };
    SDL_RenderCopy( gRenderer, mTexture, NULL, &renderQuad );
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}