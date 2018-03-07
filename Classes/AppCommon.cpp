#include "AppCommon.h"
#include "Poke.h"
#include "cocos2d/external/win32-specific/icon/include/iconv.h"

USING_NS_CC;

MenuItemSprite* customMenuItem(const std::string& normalImage, const std::string& disabledImage, const ccMenuCallback& callback)
{
	auto image = Sprite::create(normalImage);
	image->setColor(Color3B(100,100,100));
	auto item = MenuItemSprite::create(
		Sprite::create(normalImage),
		image,
		Sprite::create(disabledImage),
		callback);

	return item;
}

MenuItemSprite* customMenuItemWithSpriteFrameName(const std::string& normalImage, const std::string& disabledImage, const ccMenuCallback& callback)
{
	auto image = Sprite::createWithSpriteFrameName(normalImage);
	image->setColor(Color3B(100,100,100));
	auto item = MenuItemSprite::create(
		Sprite::createWithSpriteFrameName(normalImage),
		image,
		Sprite::createWithSpriteFrameName(disabledImage),
		callback);

	return item;
}

// �ж��Ƿ�����

bool IsContinuous(std::vector<int>& vec)
{
	bool ret = true;

	// ����
	std::sort(vec.begin(), vec.end());

	// ������ɺ�Ƚ����ڵ��������ֵĲ�ֵ�����ȫΪ1����Ϊ����
	for (int i=0; i<vec.size()-1; i++)
	{
		if (vec[i+1] - vec[i] != 1)
		{
			ret = false;
			break;
		}
	}

	return ret;
}

bool IsContinuous(CCArray* vecPoke)
{
	unsigned int length = vecPoke->count();
	std::vector<int> vec;

	for (int i=0; i<length; i++)
	{
		Poke* poke = dynamic_cast<Poke*>(vecPoke->objectAtIndex(i));
		if (poke != NULL)
		{
			vec.push_back(poke->_info._num);
		}
	}

	return IsContinuous(vec);
}

// �ж��Ƿ�С��2
bool IsLessTwo(CCArray* vecPoke)
{
	bool ret = false;

	unsigned int length = vecPoke->count();
	for (int i=0; i<length; i++)
	{
		Poke* poke = dynamic_cast<Poke*>(vecPoke->objectAtIndex(i));
		if (poke != NULL)
		{
			ret = poke->_info._num >= 12;
		}
	}

	return !ret;
}

bool IsLessTwo(std::vector<int>& vecPoke)
{
	bool ret = false;

	for (int i=0; i<vecPoke.size(); i++)
	{
		ret = vecPoke[i] >= 12;
	}

	return !ret;
}

//////////////////////////////////////////////////////////////////////////
//��������
//////////////////////////////////////////////////////////////////////////
cocos2d::RenderTexture* ScreenShot(const bool bIsSave, std::function<void(cocos2d::RenderTexture*, const std::string&)> pFuncCallback)

{
	//ʹ����Ļ�ߴ��ʼ��һ���յ���Ⱦ�������
	Size sizeWin = Director::getInstance()->getWinSize();
	cocos2d::RenderTexture* textureScreen =
		cocos2d::RenderTexture::create(sizeWin.width, sizeWin.height);
	//������ݲ���ʼ��ȡ
	textureScreen->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
	//���������ڵ�����������texure��
	cocos2d::Director::getInstance()->getRunningScene()->visit();
	//������ȡ
	textureScreen->end();
	//����ΪPNGͼ
	if (bIsSave)
	{
		static int s_iSerialNumber = 0;
		textureScreen->saveToFile(
			cocos2d::CCString::createWithFormat("ScreenShot_%04d.png", ++s_iSerialNumber)->getCString(),
			cocos2d::Image::Format::PNG,
			true,
			pFuncCallback);
	}
	else
	{
		if (nullptr != pFuncCallback)
		{
			pFuncCallback(textureScreen, "");
		}
	}
	return textureScreen;
}

int code_convert(const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
	iconv_t cd;
	const char *temp = inbuf;
	const char **pin = &temp;
	char **pout = &outbuf;
	memset(outbuf,0,outlen);
	cd = iconv_open(to_charset,from_charset);
	if(cd==0) return -1;
	if(iconv(cd,pin,&inlen,pout,&outlen)==-1) return -1;
	iconv_close(cd);
	return 0;
}

/*UTF8תΪGB2312*/
std::string u2a(const char *inbuf)
{
	size_t inlen = strlen(inbuf);
	char * outbuf = new char[inlen * 2 + 2];
	std::string strRet;
	if(code_convert("utf-8", "gb2312", inbuf, inlen, outbuf, inlen * 2 + 2) == 0)
	{
		strRet = outbuf;
	}
	delete [] outbuf;
	return strRet;
}

/*GB2312תΪUTF8*/
std::string a2u(const char *inbuf)
{
	size_t inlen = strlen(inbuf);
	char * outbuf = new char[inlen * 2 + 2];
	std::string strRet;
	if(code_convert("gb2312", "utf-8", inbuf, inlen, outbuf, inlen * 2 + 2) == 0)
	{
		strRet = outbuf;
	}
	delete [] outbuf;
	return strRet;
}