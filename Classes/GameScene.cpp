#include "GameScene.h"
#include "Player.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	_arrPlayerOut = CCArray::create();
	_arrPlayerOut->retain();
	_arrPlayerOut->retain();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 菜单

	auto itemBuChu = MenuItemFont::create("BuChu", CC_CALLBACK_1(GameScene::menuBuChuCallback, this));
	itemBuChu->setFontSize(28);
	itemBuChu->setPosition(-100, -300);

    auto itemChuPai = MenuItemFont::create("ChuPai", CC_CALLBACK_1(GameScene::menuChuPaiCallback, this));
	itemChuPai->setFontSize(28);
	itemChuPai->setPosition(100, -300);

    _chuPaiMenu = Menu::create();
	_chuPaiMenu->addChild(itemBuChu,2,0);
	_chuPaiMenu->addChild(itemChuPai,2,1);
    this->addChild(_chuPaiMenu, 1);

	// 玩家

	_player1 = Player::create("W1", true, true);
	_player1->setPosition(100, 200);
	this->addChild(_player1);

	_player2 = Player::create("P2", false, false);
	_player2->setPosition(100, 400);
	this->addChild(_player2);

	_player3 = Player::create("P3", false, false);
	_player3->setPosition(100, 600);
	this->addChild(_player3);

	// 初始化牌堆
	initCards();

	// 发牌
	//std::random_shuffle(_pokeInfo.begin(), _pokeInfo.end());

	for (int i = 0; i < 54; i++)
	{
		if (i >= 51)
		{
			//底牌
			//_bottomCardZone->Show(_pokeInfo.at(_i));
		}
		else
		{
			if (i%3==0)
			{
				// 角色1
				_player1->FaPai(_pokeInfo.at(i));
			} else if (i%3==1)
			{
				// 角色2
				_player2->FaPai(_pokeInfo.at(i));
			} else if (i%3==2)
			{
				// 角色3
				_player3->FaPai(_pokeInfo.at(i));
			}
		}
	}
	
	// 自动判断牌型
	scheduleOnce(schedule_selector(GameScene::OutCard),1);
    
	_state = 1;

    return true;
}

void GameScene::initCards()
{
	for (int i=0; i<13; i++)
	{
		for (int j=0; j<4; j++)
		{
			PokeInfo info;
			info._num = (PokeNum)i;
			info._tag = (PokeTag)j;
			_pokeInfo.push_back(info);
		}
	}

	PokeInfo info;
	info._num = (PokeNum)13;
	info._tag = (PokeTag)0;
	_pokeInfo.push_back(info);
	info._num = (PokeNum)14;
	_pokeInfo.push_back(info);
}

void GameScene::menuBuChuCallback(Ref* pSender)
{
	_arrPlayerOut;
}

void GameScene::menuChuPaiCallback(Ref* pSender)
{
	_arrPlayerOut;
	_player3->clearCards();

	_player1->ChuPai();
	auto delay = DelayTime::create(1);
	auto callback = CallFuncN::create(CC_CALLBACK_1(GameScene::callbackChuPai2,this));
	auto seq = Sequence::createWithTwoActions(delay, callback);
	this->runAction(seq);
}

void GameScene::callbackChuPai2(cocos2d::Node* node)
{
	_player1->clearCards();

	_player2->ChuPai();
	auto delay = DelayTime::create(1);
	auto callback = CallFuncN::create(CC_CALLBACK_1(GameScene::callbackChuPai3,this));
	auto seq = Sequence::createWithTwoActions(delay, callback);
	this->runAction(seq);
}

void GameScene::callbackChuPai3(cocos2d::Node* node)
{
	_player2->clearCards();

	_player3->ChuPai();
}

void GameScene::update(float delta)
{

}

void GameScene::OutCard(float delta)
{
	if (_state == 1)
	{
		CARD_TYPE type_card = PanDuanPaiXing();
		if (type_card != ERROR_CARD)
		{
			//((MenuItemFont *)_chuPaiMenu->getChildByTag(1))->setEnabled(true);
		}
		else
		{
			//((MenuItemFont *)_chuPaiMenu->getChildByTag(1))->setEnabled(false);
		}
	}
}

CARD_TYPE GameScene::PanDuanPaiXing()
{
	unsigned int lengh = _arrPlayerOut->count();

	//单牌，对子，3不带,炸弹通用算法
	if(lengh > 0 && lengh < 5) //小于5张牌
	{
		if(_arrPlayerOut->objectAtIndex(0) == _arrPlayerOut->objectAtIndex(lengh-1))
		{
			return (CARD_TYPE)lengh;
		}
		if (_arrPlayerOut->objectAtIndex(0) == _arrPlayerOut->objectAtIndex(lengh-2) && lengh == 4)
		{
			return THREE_ONE_CARD;
		}
	}

	return ERROR_CARD;
}
