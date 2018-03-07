/**********************************************************************
@file		AppCommon.h
@brief		��Ϸ�ĺ궨���ļ�
@author		Ray1024
@date		2016-12-21
***********************************************************************/

#ifndef __APPCOMMON_H__
#define __APPCOMMON_H__

#include "cocos2d.h"

// �����Զ���˵���
cocos2d::MenuItemSprite* customMenuItem(const std::string& normalImage, const std::string& disabledImage, const cocos2d::ccMenuCallback& callback);
cocos2d::MenuItemSprite* customMenuItemWithSpriteFrameName(const std::string& normalImage, const std::string& disabledImage, const cocos2d::ccMenuCallback& callback);

// �ж��Ƿ�����
bool IsContinuous(cocos2d::CCArray* vecPoke);
bool IsContinuous(std::vector<int>& vec);

// �ж��Ƿ�С��2
bool IsLessTwo(cocos2d::CCArray* vecPoke);
bool IsLessTwo(std::vector<int>& vecPoke);


//////////////////////////////////////////////////////////////////////////
//��������
//////////////////////////////////////////////////////////////////////////
cocos2d::RenderTexture* ScreenShot(const bool bIsSave, std::function<void(cocos2d::RenderTexture*, const std::string&)> pFuncCallback);


int code_convert(const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen);

/*UTF8תΪGB2312*/
std::string u2a(const char *inbuf);
/*GB2312תΪUTF8*/
std::string a2u(const char *inbuf);

#endif //__APPCOMMON_H__