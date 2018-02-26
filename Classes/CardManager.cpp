#include "CardManager.h"
#include "Card.h"
#include <algorithm>

USING_NS_CC;

CardManager* CardManager::create()
{
	CardManager *sprite = new (std::nothrow) CardManager();
	if (sprite && sprite->init())
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool CardManager::init()
{
	// 初始化基类--------------------------------------------------------------
    if ( !Sprite::init() )
    {
        return false;
    }

	_exhibitionZone = CardExhibitionZone::create();
	_exhibitionZone->setPosition(50, 250);
	this->addChild(_exhibitionZone, 1);

    return true;
}

bool CardManager::dealer(CardInfo info)
{
	_cardIndex.push_back(info);

	//添加精灵
	auto card = Card::create(info);
	this->addChild(card, 100-info._num);
	card->SetTouchEnabled();

	sortAllChildren();

 	updateCards();

	return true;
}

void CardManager::updateCards()
{
	int count = _children.size();
	int zeroPoint = count/2;
	for (int i=0; i<_children.size(); i++)
	{
		Card* card = dynamic_cast<Card*>(_children.at(i));
		if (card != NULL)
		{
			card->setPosition((i-zeroPoint)*50, card->getPosition().y);
		}
	}
}

void CardManager::chuPai()
{
	if (_cardIndex.empty())
	{
		return ;
	}

	std::vector<CardInfo> arrayIndexToChuPai;
	for (auto it=_children.begin(); it!=_children.end(); it++)
	{
		Card* card = dynamic_cast<Card*>(*it);
		if (card != NULL && card->isSelected())
		{
			arrayIndexToChuPai.push_back(card->getInfo());
		}
	}

	for (int j=0; j<arrayIndexToChuPai.size(); j++)
	{
		for (auto it=_children.begin(); it!=_children.end(); it++)
		{
			Card* card = dynamic_cast<Card*>(*it);
			if (card != NULL && 
				card->getInfo() == arrayIndexToChuPai.at(j))
			{
				removeChild(card, true);
				_cardIndex.erase(
					std::remove(_cardIndex.begin(),_cardIndex.end(),card->getInfo()),_cardIndex.end());
				break;
			}
		}
	}

	_exhibitionZone->chuPai(arrayIndexToChuPai);

	updateCards();
}

void CardManager::clearCards()
{
	_exhibitionZone->removeAllChildrenWithCleanup(true);
}