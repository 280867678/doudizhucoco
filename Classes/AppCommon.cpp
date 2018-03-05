#include "AppCommon.h"
#include "Poke.h"

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

// 判断是否连续

bool IsContinuous(std::vector<int>& vec)
{
	bool ret = true;

	// 排序
	std::sort(vec.begin(), vec.end());

	// 排序完成后比较相邻的两个数字的差值，如果全为1，则为连续
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

// 判断是否都小于2
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