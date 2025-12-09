#ifndef CHARACTER_H
#define CHARACTER_H

#include "LTexture.h"
#include <string>

class Character : public LTexture
{
public:
    Character();
    ~Character();

    bool loadExpression(std::string path);

    
    // 角色的縮放比例
    void setScale(float scale);
    float getScale();

protected:
    float mScale; // 角色的身高比例
};

#endif