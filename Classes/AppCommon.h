/**********************************************************************
@file		AppCommon.h
@brief		游戏的宏定义文件
@author		Ray1024
@date		2016-12-21
***********************************************************************/

#ifndef __APPCOMMON_H__
#define __APPCOMMON_H__

#include "cocos2d.h"

// 创建自定义菜单项
cocos2d::MenuItemSprite* customMenuItem(const std::string& normalImage, const std::string& disabledImage, const cocos2d::ccMenuCallback& callback);
cocos2d::MenuItemSprite* customMenuItemWithSpriteFrameName(const std::string& normalImage, const std::string& disabledImage, const cocos2d::ccMenuCallback& callback);

// 判断是否连续
bool IsContinuous(cocos2d::CCArray* vecPoke);
bool IsContinuous(std::vector<int>& vec);

// 判断是否都小于2
bool IsLessTwo(cocos2d::CCArray* vecPoke);
bool IsLessTwo(std::vector<int>& vecPoke);


//////////////////////////////////////////////////////////////////////////
//截屏函数
//////////////////////////////////////////////////////////////////////////
cocos2d::RenderTexture* ScreenShot(const bool bIsSave, std::function<void(cocos2d::RenderTexture*, const std::string&)> pFuncCallback);


int code_convert(const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen);

/*UTF8转为GB2312*/
std::string u2a(const char *inbuf);
/*GB2312转为UTF8*/
std::string a2u(const char *inbuf);

#endif //__APPCOMMON_H__