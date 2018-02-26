#include "Card.h"

USING_NS_CC;

Card* Card::create(CardInfo info)
{
	Card *sprite = new (std::nothrow) Card();
	if (sprite && sprite->init(info))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool Card::init(CardInfo info)
{
	// 初始化基类--------------------------------------------------------------
	if ( !Sprite::init() )
	{
		return false;
	}

	_info = info;

	char num[10] = { 0 };
	sprintf(num, "%d", _info._num);

	auto cardFront = Label::createWithSystemFont(num, "arial", 24);
	this->addChild(cardFront);
	_size = cardFront->getContentSize();

	_isSelected = false;

	return true;
}

void Card::SetTouchEnabled()
{
	// 触摸事件
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);

	touchListener->onTouchBegan = CC_CALLBACK_2(Card::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(Card::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(Card::onTouchEnded, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(Card::onTouchCancelled, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

}

Rect Card::getRect()
{
	return Rect(-_size.width / 2, -_size.height / 2, _size.width, _size.height);
}

bool Card::onTouchBegan(Touch* touch, Event* event)
{
	if (getRect().containsPoint(convertTouchToNodeSpaceAR(touch)))
	{
		click();

		return true;
	}

	return false;
}

void Card::onTouchEnded(Touch* touch, Event* event)
{

}

void Card::onTouchCancelled(Touch* touch, Event* event)
{

}

void Card::onTouchMoved(Touch* touch, Event* event)
{

}

void Card::click()
{
	if (_isSelected)
	{
		setPosition(_position.x,_position.y -20);
	} else
	{
		setPosition(_position.x,_position.y +20);
	}
	_isSelected = !_isSelected;
}