#include "ICardManager.h"
#include "Card.h"
#include <algorithm>

USING_NS_CC;

ICardManager* ICardManager::create()
{
	ICardManager *sprite = new (std::nothrow) ICardManager();
	if (sprite && sprite->init())
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool ICardManager::init()
{
	// 初始化基类--------------------------------------------------------------
    if ( !Sprite::init() )
    {
        return false;
    }

	_exhibitionZone = CardExhibitionZone::create();
	_exhibitionZone->setPosition(0, -120);
	this->addChild(_exhibitionZone);

	_cardCounts = Label::createWithSystemFont("0", "宋体", 24);
	_cardCounts->setPosition(0,0);
	this->addChild(_cardCounts,1);

    return true;
}

bool ICardManager::dealer(CardInfo info)
{
	_cardIndex.push_back(info);

	updateCards();

	return true;
}

void ICardManager::updateCards()
{
	std::stringstream text;
	text << _cardIndex.size();
	_cardCounts->setString(text.str());
}

void ICardManager::chuPai()
{
	if (_cardIndex.empty())
	{
		return ;
	}

	std::vector<CardInfo> arrayIndexToChuPai;
	arrayIndexToChuPai.push_back(_cardIndex.back());
	_cardIndex.pop_back();

	_exhibitionZone->chuPai(arrayIndexToChuPai);

	updateCards();
}

void ICardManager::clearCards()
{
	_exhibitionZone->removeAllChildrenWithCleanup(true);
}