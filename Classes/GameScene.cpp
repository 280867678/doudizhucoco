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
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// 重开菜单

	auto itemRestart = MenuItemFont::create("Game over! Press this button to restart!", CC_CALLBACK_1(GameScene::menuRestartCallback, this));
	itemRestart->setFontSize(28);
	itemRestart->setColor(Color3B(255, 0, 0));

	_restartMenu = Menu::create(itemRestart, NULL);
	this->addChild(_restartMenu, 1);
	_restartMenu->setVisible(false);

	// 抢地主菜单

	auto itemBuQiang = MenuItemFont::create("BuQiang", CC_CALLBACK_1(GameScene::menuBuQiangCallback, this));
	itemBuQiang->setFontSize(28);
	itemBuQiang->setPosition(-150, -300);

	auto item1Fen = MenuItemFont::create("1$", CC_CALLBACK_1(GameScene::menuBuQiangCallback, this));
	item1Fen->setFontSize(28);
	item1Fen->setPosition(-50, -300);

	auto item2Fen = MenuItemFont::create("2$", CC_CALLBACK_1(GameScene::menuBuQiangCallback, this));
	item2Fen->setFontSize(28);
	item2Fen->setPosition(50, -300);

	auto item3Fen = MenuItemFont::create("3$", CC_CALLBACK_1(GameScene::menuBuQiangCallback, this));
	item3Fen->setFontSize(28);
	item3Fen->setPosition(150, -300);

	_qiangDiZhuMenu = Menu::create(itemBuQiang, item1Fen, item2Fen, item3Fen, NULL);
	this->addChild(_qiangDiZhuMenu, 1);

    // 出牌菜单

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
	_chuPaiMenu->setVisible(false);

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

	// 底牌区
	_diPai = PokeExhibitionZone::create();
	_diPai->setPosition(300, 50);
	this->addChild(_diPai, 1);

	// 初始化牌堆
	initCards();

	// 洗牌
	srand(time(0));
	std::random_shuffle(_pokeInfo.begin(), _pokeInfo.end());

	// 发牌

	for (int i = 0; i < 54; i++)
	{
		if (i >= 51)
		{
			//底牌
			//_bottomCardZone->Show(_pokeInfo.at(_i));
			_vecDiPai.push_back(_pokeInfo.at(i));
		}
		else
		{
			if (i%3==0)
			{
				// 角色1
				_player1->FaPai(this, _pokeInfo.at(i));
			} else if (i%3==1)
			{
				// 角色2
				_player2->FaPai(this, _pokeInfo.at(i));
			} else if (i%3==2)
			{
				// 角色3
				_player3->FaPai(this, _pokeInfo.at(i));
			}
		}
	}

	_diPai->chuPai(_vecDiPai);
	
	// 自动判断牌型
	schedule(schedule_selector(GameScene::OutCard),0.5);
    
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

void GameScene::menuRestartCallback(Ref* pSender)
{
	Director::getInstance()->replaceScene(GameScene::createScene());
}

void GameScene::menuBuQiangCallback(Ref* pSender)
{
	// 分发底牌
	for (int i=0; i<_vecDiPai.size(); i++)
	{
		_player1->FaPai(this, _vecDiPai[i]);
	}

	// 切换菜单可见
	_qiangDiZhuMenu->setVisible(false);
	_chuPaiMenu->setVisible(true);
}

void GameScene::menuBuChuCallback(Ref* pSender)
{
	_player3->clearCards();
	_player1->BuChu();

	auto delay = DelayTime::create(1);
	auto callback = CallFuncN::create(CC_CALLBACK_1(GameScene::callbackChuPai2,this));
	auto seq = Sequence::createWithTwoActions(delay, callback);
	this->runAction(seq);

	_chuPaiMenu->setVisible(false);
}

void GameScene::menuChuPaiCallback(Ref* pSender)
{
	_player3->clearCards();

	_player1->ChuPai(this);
	// 清空牌型判断区
	_arrPlayerOut->removeAllObjects();

	auto delay = DelayTime::create(1);
	auto callback = CallFuncN::create(CC_CALLBACK_1(GameScene::callbackChuPai2,this));
	auto seq = Sequence::createWithTwoActions(delay, callback);
	this->runAction(seq);

	_chuPaiMenu->setVisible(false);
}

void GameScene::callbackChuPai2(cocos2d::Node* node)
{
	_player1->clearCards();

	_player2->ChuPai(this);
	auto delay = DelayTime::create(1);
	auto callback = CallFuncN::create(CC_CALLBACK_1(GameScene::callbackChuPai3,this));
	auto seq = Sequence::createWithTwoActions(delay, callback);
	this->runAction(seq);
}

void GameScene::callbackChuPai3(cocos2d::Node* node)
{
	_player2->clearCards();

	_player3->ChuPai(this);

	_chuPaiMenu->setVisible(true);
}

void GameScene::update(float delta)
{

}

void GameScene::OutCard(float delta)
{
	if (_state == 1)
	{
		CARD_TYPE type_card = PanDuanPaiXing(_arrPlayerOut);
		if (type_card != ERROR_CARD)
		{
			((MenuItemFont *)_chuPaiMenu->getChildByTag(1))->setEnabled(true);
		}
		else
		{
			((MenuItemFont *)_chuPaiMenu->getChildByTag(1))->setEnabled(false);
		}
	}
}

void GameScene::gameover()
{
	_restartMenu->setVisible(true);
}

unsigned int GameScene::GetCardLogicValue(cocos2d::CCArray* cards)
{
	//获取类型  
	CARD_TYPE cbNextType= PanDuanPaiXing(cards);

	//开始对比  
	switch(cbNextType)  
	{  
	case SINGLE_CARD:  
		return 1;
	case DOUBLE_CARD:  
		return 7;
	case THREE_CARD:  
		return 7;
	case CONNECT_CARD:  
		return 7;
	case COMPANY_CARD:  
		return 7;
	case AIRCRAFT_CARD:  
		return 7;
	case BOMB_CARD:  
		return 7;
	case THREE_ONE_CARD:  
		return 7;
	case THREE_TWO_CARD:  
		return 7;
	case BOMB_TWO_CARD:  
		return 7;
	case BOMB_TWOOO_CARD:
		return 7;
	}  
}

bool GameScene::CompareCard(CCArray* firstCard, CCArray* nextCard)  
{  
	//获取类型  
	CARD_TYPE cbNextType= PanDuanPaiXing(nextCard);  
	CARD_TYPE cbFirstType= PanDuanPaiXing(firstCard);  

	//类型判断  
	if(cbNextType==ERROR_CARD) return false;  
	if(cbNextType==MISSILE_CARD) return true;  

	//炸弹判断  
	if((cbFirstType!=BOMB_CARD)&&(cbNextType==BOMB_CARD)) return true;  
	if((cbFirstType==BOMB_CARD)&&(cbNextType!=BOMB_CARD)) return false;  

	//规则判断  
	if((cbFirstType!=cbNextType)||(firstCard->count()!=nextCard->count())) return false;  

	//开始对比  
	switch(cbNextType)  
	{  
	case SINGLE_CARD:  
	case DOUBLE_CARD:  
	case THREE_CARD:  
	case CONNECT_CARD:  
	case COMPANY_CARD:  
	case AIRCRAFT_CARD:  
	case BOMB_CARD:  
		{  
			//获取数值  
			BYTE cbNextLogicValue=GetCardLogicValue(nextCard);  
			BYTE cbFirstLogicValue=GetCardLogicValue(firstCard);  

			//对比扑克  
			return cbNextLogicValue>cbFirstLogicValue;  
		}  
	case THREE_ONE_CARD:  
	case THREE_TWO_CARD:  
		{  
// 			//分析扑克  
// 			tagAnalyseResult NextResult;  
// 			tagAnalyseResult FirstResult;  
// 			AnalysebCardData(cbNextCard,cbNextCount,NextResult);  
// 			AnalysebCardData(cbFirstCard,cbFirstCount,FirstResult);  
// 
// 			//获取数值  
// 			BYTE cbNextLogicValue=GetCardLogicValue(NextResult.cbThreeCardData[0]);  
// 			BYTE cbFirstLogicValue=GetCardLogicValue(FirstResult.cbThreeCardData[0]);  

			//对比扑克  
//			return cbNextLogicValue>cbFirstLogicValue;  
		}  
	case BOMB_TWO_CARD:  
	case BOMB_TWOOO_CARD:  
		{  
// 			//分析扑克  
// 			tagAnalyseResult NextResult;  
// 			tagAnalyseResult FirstResult;  
// 			AnalysebCardData(cbNextCard,cbNextCount,NextResult);  
// 			AnalysebCardData(cbFirstCard,cbFirstCount,FirstResult);  
// 
// 			//获取数值  
// 			BYTE cbNextLogicValue=GetCardLogicValue(NextResult.cbFourCardData[0]);  
// 			BYTE cbFirstLogicValue=GetCardLogicValue(FirstResult.cbFourCardData[0]);  

			//对比扑克  
//			return cbNextLogicValue>cbFirstLogicValue;  
		}  
	}  

	return false;  
}  

CARD_TYPE GameScene::PanDuanPaiXing(CCArray* cards)
{
	unsigned int length = cards->count();

	//小于5张牌
	//单牌，对子，3不带,炸弹通用算法
	if(length > 0 && length < 5) 
	{
		Poke* poke1 = dynamic_cast<Poke*>(cards->objectAtIndex(0));
		Poke* poke_1 = dynamic_cast<Poke*>(cards->objectAtIndex(length - 1));

		if(poke1->_info._num == poke_1->_info._num)
		{
			return (CARD_TYPE)length;
		}

		// 火箭
		Poke* poke2 = dynamic_cast<Poke*>(cards->objectAtIndex(1));
		if (poke1->_info._num == NUM_XW && poke2->_info._num == NUM_DW && length == 2)
		{
			return MISSILE_CARD;
		}

		Poke* poke_2 = dynamic_cast<Poke*>(cards->objectAtIndex(length - 2));
		if (poke1->_info._num == poke_2->_info._num && length == 4)
		{
			return THREE_ONE_CARD;
		}
	}

	// 大于等于5张牌
	else if (length >= 5)
	{
		// 连牌
		if (CheckContinuous(cards) && IsLessTwo(cards))
		{
			return CONNECT_CARD;
		}

		// 连对
		if (length >= 6 && length % 2 == 0) // 判断是否大于六张，且为双数
		{
			std::vector<int> vec;
			for (int i=0; i<length; i++)
			{
				Poke* poke = dynamic_cast<Poke*>(cards->objectAtIndex(i));
				if (poke != NULL)
				{
					vec.push_back(poke->_info._num);
				}
			}
			std::sort(vec.begin(), vec.end());

			// 判断是否都是对子
			bool is_all_double = true;

			for (int i=0; i<length; i+=2)
			{
				if (vec[i] != vec[i+1])
				{
					is_all_double = false;
					break;
				}
			}

			// 判断对子是否连续
			if (is_all_double)
			{
				std::vector<int> vec_single;
				for (int i=0; i<length; i+=2)
				{
					vec_single.push_back(vec[i]);
				}

				if (IsContinuous(vec_single))
				{
					return COMPANY_CARD;
				}
			}
		}

		struct CRAD_INDEX
		{
			std::vector<int>	single_index;	//记录单张的牌
			std::vector<int>	double_index;	//记录对子的牌
			std::vector<int>	three_index;	//记录3张
			std::vector<int>	four_index;		//记录4张
		};

		// 从牌堆中将数字拿到数组中

		std::vector<int> vec;
		for (int i=0; i<length; i++)
		{
			Poke* poke = dynamic_cast<Poke*>(cards->objectAtIndex(i));
			if (poke != NULL)
			{
				vec.push_back(poke->_info._num);
			}
		}
		std::sort(vec.begin(), vec.end());

		// 将数组中的牌数分到结构体CRAD_INDEX中
		CRAD_INDEX card_index;

		for (int i=0; i<length; )
		{
			if (i+1 < length && vec[i] == vec[i+1])
			{
				if (i+2 < length && vec[i+1] == vec[i+2])
				{
					if (i+3 < length && vec[i+2] == vec[i+3])
					{
						card_index.four_index.push_back(vec[i]);
						i += 4;
					}
					else
					{
						card_index.three_index.push_back(vec[i]);
						i += 3;
					}
				}
				else
				{
					card_index.double_index.push_back(vec[i]);
					i += 2;
				}
			}
			else
			{
				card_index.single_index.push_back(vec[i]);
				i++;
			}
		}

		// 3带对
		if (card_index.three_index.size() == 1 && card_index.double_index.size() == 1 && card_index.four_index.empty() && card_index.single_index.empty())
		{
			return THREE_TWO_CARD;
		}

		// 飞机
		// 前提：两个连续三张的
		if (card_index.four_index.empty() && card_index.three_index.size() == 2 && card_index.three_index[0] + 1 == card_index.three_index[1])
		{
			// 333444
			if (card_index.single_index.empty() && card_index.double_index.empty())
			{
				return AIRCRAFT_CARD;
			}

			// 33344456
			if (card_index.double_index.empty() && card_index.single_index.size() == 2)
			{
				return AIRCRAFT_SINGLE_CARD;
			}

			// 33344455
			if (card_index.single_index.empty() && card_index.double_index.size() == 1)
			{
				return AIRCRAFT_SINGLE_CARD;
			}

			// 3334445566
			if (card_index.single_index.empty() && card_index.double_index.size() == 2)
			{
				return AIRCRAFT_DOUBLE_CARD;
			}
		}

		// 4带2
		if (card_index.four_index.size() == 1 && length % 2 == 0 && card_index.three_index.empty())
		{

			// 444423
			if (card_index.single_index.size() == 2 && card_index.double_index.empty())
			{
				return BOMB_TWO_CARD;
			}

			// 444422
			if (card_index.double_index.size() == 1 && card_index.single_index.empty())
			{
				return BOMB_TWO_CARD;
			}

			// 44442233
			if (card_index.double_index.size() == 2 && card_index.single_index.empty())
			{
				return BOMB_TWOOO_CARD;
			}
		}
	}

	return ERROR_CARD;
}

bool IsContinuous(int arr[], int len)
{
	int MAX = -1;
	int MIN = -1;
	for (int i = 0; i < len; i++)
	{
		if (arr[i] != 0)
		{
			if (arr[i] > MAX || MAX == -1) // 防止数组中的非0最大数还比 -1 小时，最终能取得真正的非0最大值。
				MAX = arr[i];

			if (arr[i] < MIN || MIN == -1) // 防止数组中的非0最小数还比 -1 大时，最终能取得真正的非0最小值。
				MIN = arr[i];
		}

	}

	if (MAX - MIN <= len - 1)
		return true;
	else
		return false;

}

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

// 判断是否连续
bool CheckContinuous(CCArray* vecPoke)
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