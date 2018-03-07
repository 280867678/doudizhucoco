#ifndef __SPRITE_BLUR_H__
#define __SPRITE_BLUR_H__

#include "cocos2d.h"

USING_NS_CC;

//////////////////////////////////////////////////////////////////////////
//精灵模糊化函数
//////////////////////////////////////////////////////////////////////////
cocos2d::RenderTexture* SpriteBlurer(cocos2d::Sprite* pSprite, const float fRadius = 8.0f, const float fSampleNum = 8.0f);

//////////////////////////////////////////////////////////////////////////
//精灵模糊类
//////////////////////////////////////////////////////////////////////////
class SpriteBlur : public cocos2d::Sprite
{
public:
    SpriteBlur();
    ~SpriteBlur();
    static SpriteBlur* create(cocos2d::Sprite* pSprite, const float fRadius = 8.0f, const float fSampleNum = 8.0f);
    bool initWithTexture(cocos2d::Texture2D* pTexture, const cocos2d::Rect&  rRect);
    void initGLProgram();

    void setBlurRadius(float fRadius);
    void setBlurSampleNum(float fSampleNum);

protected:
    float fBlurRadius_;
    float fBlurSampleNum_;
};

#endif // __SPRITE_BLUR_H__