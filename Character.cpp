#include "Character.h"

Character::Character() : mScale(0.9f), LTexture()
{}

Character::~Character()
{
    free();
}

bool Character::loadExpression(std::string path)
{
    return loadFromFile(path);
}

void Character::setScale(float scale)
{
    mScale = scale;
}

float Character::getScale()
{
    return mScale;
}