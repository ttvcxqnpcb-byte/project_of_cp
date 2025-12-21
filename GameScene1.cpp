#include "GameScene1.h"
#include <stdio.h>

ChapterScene1::ChapterScene1() {
}

ChapterScene1::~ChapterScene1() {
}

void ChapterScene1::load() {
    if (!prologue.loadScript("assets/txt/scene1.txt")) {
        printf("Fail to load prologue txt\n");
    }
}

void ChapterScene1::handleEvent(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch( e.key.keysym.sym ) {
            case SDLK_SPACE:
            case SDLK_KP_ENTER:
            case SDLK_RETURN:
                prologue.handleContinue();
                break;
            case SDLK_UP:
                prologue.handleBack();
        }
    }
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        prologue.handleContinue();
    }
}

void ChapterScene1::update() {
    prologue.update();
}

void ChapterScene1::render(int w, int h) {
    prologue.render(w, h);
}

bool ChapterScene1::isFinished() {
    return prologue.isFinished();
}

void ChapterScene1::clean() {
}