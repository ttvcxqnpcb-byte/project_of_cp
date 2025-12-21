#ifndef FONTSOFPROJECT_GLOBALVARS_H
#define FONTSOFPROJECT_GLOBALVARS_H

extern bool isComputerOpen;
extern bool isPictureGet;
extern bool isStoryFinished;
extern int gTitleTarget;

void resetGameVars();

enum ChapterList {
    CH_TITLE,
    CH_PROLOGUE,
    CH_SCENE1,
    CH_SCENE2,
    CH_SCENE2_1T,
    CH_SCENE2_1F,
    CH_SCENE2_2TT,
    CH_SCENE2_2TF,
    CH_SCENE2_2FT,
    CH_SCENE2_2FF,
    CH_SCENE3,
    CH_SCENE4_TT,//第一個是T是電腦
    CH_SCENE4_TF,
    CH_SCENE4_FT,
    CH_SCENE4_FF,
    CH_TOTAL
 };


#endif //FONTSOFPROJECT_GLOBALVARS_H