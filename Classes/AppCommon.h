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

#endif //__APPCOMMON_H__