#include "GamePrologue.h"
#include <stdio.h>

GamePrologue::GamePrologue() {
}

GamePrologue::~GamePrologue() {
}

void GamePrologue::load() {
    if (!prologue.loadScript("assets/txt/test.txt")) {
        printf("Fail to load prologue txt\n");
    }
}

void GamePrologue::handleEvent(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch( e.key.keysym.sym ) {
            case SDLK_SPACE:
            case SDLK_KP_ENTER:
            case SDLK_RETURN:
                prologue.handleContinue();
                break;
        }
    }
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        prologue.handleContinue();
    }
}

void GamePrologue::update() {
    prologue.update();
}

void GamePrologue::render(int w, int h) {
    prologue.render(w, h);
}

bool GamePrologue::isFinished() {
    return prologue.isFinished();
}

void GamePrologue::clean() {
}