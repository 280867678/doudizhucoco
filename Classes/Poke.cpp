#include "Poke.h"
#include "GameScene.h"

USING_NS_CC;

Poke* Poke::create(PokeInfo info)
{
	Poke *sprite = new (std::nothrow) Poke();
	if (sprite && sprite->init(info))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

char* poke_text[] = {
	"A",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"10",
	"J",
	"Q",
	"K",
	"g",
	"G",
};

bool Poke::init(PokeInfo info)
{
	// 初始化基类--------------------------------------------------------------
	if ( !Sprite::init() )
	{
		return false;
	}

	_info = info;

	auto cardFront = Label::createWithSystemFont(poke_text[_info._num], "arial", 24);
	this->addChild(cardFront);
	_size = cardFront->getContentSize();

	_isSelected = false;

	return true;
}

void Poke::SetTouchEnabled()
{
	// 触摸事件
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);

	touchListener->onTouchBegan = CC_CALLBACK_2(Poke::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(Poke::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(Poke::onTouchEnded, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(Poke::onTouchCancelled, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

}

Rect Poke::getRect()
{
	return Rect(-_size.width / 2, -_size.height / 2, _size.width, _size.height);
}

bool Poke::onTouchBegan(Touch* touch, Event* event)
{
	if (getRect().containsPoint(convertTouchToNodeSpaceAR(touch)))
	{
		click();

		return true;
	}

	return false;
}

void Poke::onTouchEnded(Touch* touch, Event* event)
{

}

void Poke::onTouchCancelled(Touch* touch, Event* event)
{

}

void Poke::onTouchMoved(Touch* touch, Event* event)
{

}

void Poke::click()
{
	GameScene* scene = (GameScene*)Director::getInstance()->getRunningScene();
	CCArray* arr = scene->getArrPlayerOut();

	if (_isSelected)
	{
		setPosition(_position.x,_position.y -20);
		arr->removeObject(this);
	} 
	else
	{
		setPosition(_position.x,_position.y +20);
		arr->addObject(this);
	}
	_isSelected = !_isSelected;
}