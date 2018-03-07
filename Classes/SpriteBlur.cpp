#include "SpriteBlur.h"

//////////////////////////////////////////////////////////////////////////
//����ģ��������
//////////////////////////////////////////////////////////////////////////
cocos2d::RenderTexture* SpriteBlurer(cocos2d::Sprite* pSprite, const float fRadius, const float fSampleNum)
{
	//ģ��������ʱ����
	auto pSptBlur = SpriteBlur::create(pSprite, fRadius, fSampleNum);
	pSptBlur->setRotationSkewX(180.0f);
	pSptBlur->setPositionX(pSptBlur->getContentSize().width / 2);
	pSptBlur->setPositionY(pSptBlur->getContentSize().height / 2);
	//ʹ�þ���ߴ��ʼ��һ���յ���Ⱦ�������
	cocos2d::RenderTexture* textureScreen =
		cocos2d::RenderTexture::create(pSptBlur->getContentSize().width, pSptBlur->getContentSize().height);
	//��ʼ��ȡ
	textureScreen->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
	//�����ڵ�����������texure��
	pSptBlur->visit();
	//������ȡ
	textureScreen->end();
	return textureScreen;
}

//
// SpriteBlur
//

SpriteBlur::SpriteBlur()
    : fBlurRadius_(0.0f)
    , fBlurSampleNum_(0.0f)
{

}

SpriteBlur::~SpriteBlur()
{

}

SpriteBlur* SpriteBlur::create(cocos2d::Sprite* pSprite, const float fRadius, const float fSampleNum)
{
    SpriteBlur* pRet = new (std::nothrow) SpriteBlur();
    if (nullptr == pRet)
        return nullptr;
    pRet->fBlurRadius_ = fRadius;
    pRet->fBlurSampleNum_ = fSampleNum;
    if (pRet->initWithSpriteFrame(pSprite->getSpriteFrame()))
        pRet->autorelease();
    else
        CC_SAFE_DELETE(pRet);
    return pRet;
}

bool SpriteBlur::initWithTexture(cocos2d::Texture2D* texture, const cocos2d::Rect& rect)
{
    if (Sprite::initWithTexture(texture, rect))
    {
#if CC_ENABLE_CACHE_TEXTURE_DATA
        auto listener = EventListenerCustom::create(EVENT_RENDERER_RECREATED, [this](EventCustom* event){
            setGLProgram(nullptr);
            initGLProgram();
        });

        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
#endif
        initGLProgram();
        return true;
    }
    return false;
}

void SpriteBlur::initGLProgram()
{
    GLchar * fragSource = (GLchar*)cocos2d::String::createWithContentsOfFile(
        cocos2d::FileUtils::getInstance()->fullPathForFilename("shaders/example_Blur.fsh").c_str())->getCString();
    auto program = cocos2d::GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource);

    auto glProgramState = cocos2d::GLProgramState::getOrCreateWithGLProgram(program);
    setGLProgramState(glProgramState);

    auto size = getTexture()->getContentSizeInPixels();
    getGLProgramState()->setUniformVec2("resolution", size);
    getGLProgramState()->setUniformFloat("blurRadius", fBlurRadius_);
    getGLProgramState()->setUniformFloat("sampleNum", fBlurSampleNum_);
}


void SpriteBlur::setBlurRadius(float radius)
{
    fBlurRadius_ = radius;
    getGLProgramState()->setUniformFloat("blurRadius", fBlurRadius_);
}

void SpriteBlur::setBlurSampleNum(float num)
{
    fBlurSampleNum_ = num;
    getGLProgramState()->setUniformFloat("sampleNum", fBlurSampleNum_);
}