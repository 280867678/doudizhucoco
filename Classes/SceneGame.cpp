#include "SceneGame.h"

USING_NS_CC;

Scene* SceneGame::createScene()
{
    auto scene = Scene::create();
    auto layer = SceneGame::create();
    scene->addChild(layer);

    return scene;
}

bool SceneGame::init()
{
    // 初始化基类----------------------------------------
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// 创建主菜单---------------------------------------------
	auto itemZhunbei = MenuItemFont::create("ZhunBei", CC_CALLBACK_1(SceneGame::menuZhunbeiCallback, this));
	itemZhunbei->setPosition(750, 50);

	auto itemBack = MenuItemFont::create("Quit", CC_CALLBACK_1(SceneGame::menuBackCallback, this));
	itemBack->setPosition(850, 50);

    auto menuBack = Menu::create(itemZhunbei, itemBack, NULL);
	menuBack->setPosition(origin);
    this->addChild(menuBack, 1);

	// 创建游戏菜单---------------------------------------------
	auto itemBuchu = MenuItemFont::create("BuChu", CC_CALLBACK_1(SceneGame::menuBuchuCallback, this));
	itemBuchu->setPosition(-200 , 30);

	auto itemChupai = MenuItemFont::create("ChuPai", CC_CALLBACK_1(SceneGame::menuChuPaiCallback, this));
	itemChupai->setPosition(0 , 30);

	_menuGame = Menu::create(itemBuchu, itemChupai, NULL);
	//_menuGame->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	this->addChild(_menuGame, 1);
	_menuGame->setVisible(false);

	// 玩家头像----------------------------------------------
	auto player1 = Label::createWithSystemFont("D", "arial", 24);
	player1->setColor(Color3B(255,0,0));
	player1->setPosition(100, 300);
	this->addChild(player1, 0);

	auto player2 = Label::createWithSystemFont("N", "arial", 24);
	player2->setColor(Color3B(255,0,0));
	player2->setPosition(100, 500);
	this->addChild(player2, 0);

	auto player3 = Label::createWithSystemFont("N", "arial", 24);
	player3->setColor(Color3B(255,0,0));
	player3->setPosition(100, 400);
	this->addChild(player3, 0);

	// 牌堆-----------------------------------------------
	_cardManager1 = CardManager::create();
	_cardManager1->setPosition(480, 250);
	this->addChild(_cardManager1, 1);

	_cardManager2 = ICardManager::create();
	_cardManager2->setPosition(200,500);
	this->addChild(_cardManager2, 1);

	_cardManager3 = ICardManager::create();
	_cardManager3->setPosition(200,400);
	this->addChild(_cardManager3, 1);

	_bottomCardZone = BottomCardZone::create();
	_bottomCardZone->setPosition(450, 50);
	this->addChild(_bottomCardZone, 1);

	initCards();

	_i =0 ;
    
    return true;
}

void SceneGame::menuBackCallback(Ref* pSender)
{
	Director::getInstance()->end();
}

void SceneGame::menuZhunbeiCallback(Ref* pSender)
{
	((MenuItem*)pSender)->setVisible(false);
    _menuGame->setVisible(true);
	faPai();
}

void SceneGame::menuBuchuCallback(Ref* pSender)
{

}

void SceneGame::menuChuPaiCallback(Ref* pSender)
{
	_cardManager3->clearCards();

	_cardManager1->chuPai();
	auto delay = DelayTime::create(1);
	auto callback = CallFuncN::create(CC_CALLBACK_1(SceneGame::callbackChuPai2,this));
	auto seq = Sequence::createWithTwoActions(delay, callback);
	this->runAction(seq);
}

void SceneGame::callbackChuPai2(cocos2d::Node* node)
{
	_cardManager1->clearCards();

	_cardManager2->chuPai();
	auto delay = DelayTime::create(1);
	auto callback = CallFuncN::create(CC_CALLBACK_1(SceneGame::callbackChuPai3,this));
	auto seq = Sequence::createWithTwoActions(delay, callback);
	this->runAction(seq);
}

void SceneGame::callbackChuPai3(cocos2d::Node* node)
{
	_cardManager2->clearCards();

	_cardManager3->chuPai();
}

void SceneGame::callbackFaPai(cocos2d::Node* node)
{
	if (_i >= 51)
	{
		//底牌
		_bottomCardZone->Show(_cardInfo.at(_i));
	}
	else
	{
		if (_i%3==0)
		{
			// 角色1
			_cardManager1->dealer(_cardInfo.at(_i));
		} else if (_i%3==1)
		{
			// 角色2
			_cardManager2->dealer(_cardInfo.at(_i));
		} else if (_i%3==2)
		{
			// 角色3
			_cardManager3->dealer(_cardInfo.at(_i));
		}
	}

	_i++;

	if (_i<54)
	{
		auto delay = DelayTime::create(0.04);
		auto callback = CallFuncN::create(CC_CALLBACK_1(SceneGame::callbackFaPai,this));
		auto seq = Sequence::createWithTwoActions(delay, callback);
		this->runAction(seq);
	}	
}

void SceneGame::initCards()
{
	for (int i=0; i<13; i++)
	{
		for (int j=0; j<4; j++)
		{
			CardInfo info;
			info._num = (CardNum)i;
			info._tag = (CardTag)j;
			_cardInfo.push_back(info);
		}
	}

	CardInfo info;
	info._num = (CardNum)13;
	info._tag = (CardTag)0;
	_cardInfo.push_back(info);
	info._num = (CardNum)14;
	_cardInfo.push_back(info);
}

void SceneGame::faPai()
{
	std::random_shuffle(_cardInfo.begin(), _cardInfo.end());

	auto delay = DelayTime::create(0.04);
	auto callback = CallFuncN::create(CC_CALLBACK_1(SceneGame::callbackFaPai,this));
	auto seq = Sequence::createWithTwoActions(delay, callback);
	this->runAction(seq);

	//	for (int i=0; i<_cardInfo.size(); i++)
	//	{
	//		if (i<=16)
	//		{
	//			// 角色1
	//			_cardManager1->dealer(_cardInfo.at(i));
	//		} else if (i>16 &&i<=33)
	//		{
	//			// 角色2
	//			_cardManager2->dealer(_cardInfo.at(i));
	//		} else if (i>33 &&i<=50)
	//		{
	//			// 角色3
	//			_cardManager3->dealer(_cardInfo.at(i));
	//		}else
	//		{
	//			//底牌
	//			_bottomCardZone->Show(_cardInfo.at(i));
	//		}
	//	}
}