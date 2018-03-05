#include "SceneGame.h"
#include "Player.h"
#include "SceneMenu.h"

USING_NS_CC;

struct CRAD_INDEX
{
	std::vector<int>	single_index;	//记录单张的牌
	std::vector<int>	double_index;	//记录对子的牌
	std::vector<int>	three_index;	//记录3张
	std::vector<int>	four_index;		//记录4张
};

//
// SceneGame
//

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

	// 背景----------------------------------------------
	auto bg = Sprite::create("bg_table.jpg");
	bg->setScale(1/0.8);
	bg->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	this->addChild(bg, 0);

	auto flag = Sprite::create("d_flag.png");
	flag->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y + 100));
	this->addChild(flag, 0);

	// 返回菜单

	auto itemBack = customMenuItem("item_back.png", "item_back.png", CC_CALLBACK_1(SceneGame::menuBackCallback, this));
	itemBack->setPosition(visibleSize.width/2+200, visibleSize.height-50);

	auto menuBack = Menu::create(itemBack, NULL);
	menuBack->setPosition(origin);
	this->addChild(menuBack, 1);
	
	// 准备菜单

	auto itemReady = customMenuItem("item_zhunbei.png", "item_zhunbei.png", CC_CALLBACK_1(SceneGame::menuReadyCallback, this));
	_menuReady = Menu::create(itemReady, NULL);
	this->addChild(_menuReady, 1);

	// 重开菜单

	auto itemRestart = customMenuItemWithSpriteFrameName("ddzsingle_maplvl_btn_restart.png", "ddzsingle_maplvl_btn_restart.png", CC_CALLBACK_1(SceneGame::menuRestartCallback, this));
	itemRestart->setPosition(0, -150);
	_menuRestart = Menu::create(itemRestart, NULL);
	this->addChild(_menuRestart, 101);
	_menuRestart->setVisible(false);

	// 抢地主菜单

	auto itemQiang = customMenuItem("item_qiangdizhu.png", "item_qiangdizhu.png", CC_CALLBACK_1(SceneGame::menuQiangCallback, this));
	itemQiang->setPosition(-80, -50);
	auto itemBuQiang = customMenuItem("item_buqiang.png", "item_buqiang.png", CC_CALLBACK_1(SceneGame::menuBuQiangCallback, this));
	itemBuQiang->setPosition(80, -50);

	_menuQiangDiZhu = Menu::create(itemBuQiang, itemQiang, NULL);
	this->addChild(_menuQiangDiZhu, 1);
	_menuQiangDiZhu->setVisible(false);

    // 出牌菜单

	auto itemBuChu = customMenuItem("item_buchu.png", "item_buchu_d.png", CC_CALLBACK_1(SceneGame::menuBuChuCallback, this));
	itemBuChu->setPosition(-100, -50);
    auto itemChuPai = customMenuItem("item_chupai.png", "item_chupai_d.png", CC_CALLBACK_1(SceneGame::menuChuPaiCallback, this));
	itemChuPai->setPosition(100, -50);

	_menuChuPai = Menu::create();
	_menuChuPai->addChild(itemBuChu,2,0);
	_menuChuPai->addChild(itemChuPai,2,1);
	this->addChild(_menuChuPai, 1);
	_menuChuPai->setVisible(false);

	// 玩家

	_player1 = Player::create("Player1", true);
	_player1->setPosition(70, 300);
	this->addChild(_player1);

	_player2 = Player::create("Robot1", false);
	_player2->setPosition(1000, 600);
	this->addChild(_player2);

	_player3 = Player::create("Robot2", false);
	_player3->setPosition(70, 600);
	this->addChild(_player3);

	// 底牌区
	_bottomCardZone = BottomCardZone::create();
	_bottomCardZone->setPosition(600, 610);
	this->addChild(_bottomCardZone, 1);

	// 初始化牌堆
	initCards();

	// 洗牌
	srand(time(0));
	std::random_shuffle(_pokeInfo.begin(), _pokeInfo.end());
    
	_arrPlayerOut = CCArray::create();
	_arrPlayerOut->retain();

	_state = 1;
	_gameover = false;

    return true;
}

void SceneGame::initCards()
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

void SceneGame::faPai()
{
	// 发牌

	for (int i = 0; i < 54; i++)
	{
		if (i >= 51)
		{
			//底牌
			_bottomCardZone->Show(_pokeInfo.at(i));
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

	// 自动判断牌型
	schedule(schedule_selector(SceneGame::OutCard),0.5);
}

void SceneGame::menuBackCallback(Ref* pSender)
{
	Director::getInstance()->replaceScene(SceneMenu::createScene());
}

void SceneGame::menuReadyCallback(Ref* pSender)
{
	faPai();

	_menuReady->setVisible(false);
	_menuQiangDiZhu->setVisible(true);
}

void SceneGame::menuRestartCallback(Ref* pSender)
{
	Director::getInstance()->replaceScene(SceneGame::createScene());
}

void SceneGame::menuQiangCallback(Ref* pSender)
{
	// 分发底牌
	for (int i=0; i<_vecDiPai.size(); i++)
	{
		_player1->setDiZhu();
		_player1->FaPai(this, _vecDiPai[i]);
	}

	// 切换菜单可见
	_menuQiangDiZhu->setVisible(false);
	_menuChuPai->setVisible(true);
}

void SceneGame::menuBuQiangCallback(Ref* pSender)
{
	// 分发底牌
	for (int i=0; i<_vecDiPai.size(); i++)
	{
		_player2->setDiZhu();
		_player2->FaPai(this, _vecDiPai[i]);
	}

	// 切换菜单可见
	_menuQiangDiZhu->setVisible(false);
	_menuChuPai->setVisible(true);
}

void SceneGame::menuBuChuCallback(Ref* pSender)
{
	_player3->clearCards();
	_player1->BuChu();

	auto delay = DelayTime::create(1);
	auto callback = CallFuncN::create(CC_CALLBACK_1(SceneGame::callbackChuPai2,this));
	auto seq = Sequence::createWithTwoActions(delay, callback);
	this->runAction(seq);

	_menuChuPai->setVisible(false);
}

void SceneGame::menuChuPaiCallback(Ref* pSender)
{
	_player3->clearCards();

	_player1->ChuPai(this, false, ERROR_CARD, 0, 1);
	// 清空牌型判断区
	_arrPlayerOut->removeAllObjects();

	auto delay = DelayTime::create(1);
	auto callback = CallFuncN::create(CC_CALLBACK_1(SceneGame::callbackChuPai2,this));
	auto seq = Sequence::createWithTwoActions(delay, callback);
	this->runAction(seq);

	_menuChuPai->setVisible(false);
}

void SceneGame::callbackChuPai2(cocos2d::Node* node)
{
	// 出牌之前，判断上家的牌型
	CARD_TYPE type_out_cards = PanDuanPaiXing(_vecOutCards);

	// 判断之后出牌

	_player1->clearCards();

	if (_vecOutCards.empty())
	{
		_player2->ChuPai(this, false, type_out_cards, _vecOutCards.size(), 1);// 出牌
	}
	else
	{
		_player2->ChuPai(this, true, type_out_cards, _vecOutCards.size(), 1);// 跟牌
	}

	auto delay = DelayTime::create(1);
	auto callback = CallFuncN::create(CC_CALLBACK_1(SceneGame::callbackChuPai3,this));
	auto seq = Sequence::createWithTwoActions(delay, callback);
	this->runAction(seq);
}

void SceneGame::callbackChuPai3(cocos2d::Node* node)
{
	// 出牌之前，判断上家的牌型
	CARD_TYPE type_out_cards = PanDuanPaiXing(_vecOutCards);

	// 判断之后出牌

	_player2->clearCards();

	if (_vecOutCards.empty())
	{
		_player3->ChuPai(this, false, type_out_cards, _vecOutCards.size(), 1);// 出牌
	}
	else
	{
		_player3->ChuPai(this, true, type_out_cards, _vecOutCards.size(), 1);// 跟牌
	}

	if (!_gameover)
	{
		_menuChuPai->setVisible(true);
	}
}

void SceneGame::update(float delta)
{

}

void SceneGame::OutCard(float delta)
{
	if (_state == 1)
	{
		CARD_TYPE type_card = PanDuanPaiXing(_arrPlayerOut);
		if (type_card != ERROR_CARD)
		{
			((MenuItemFont *)_menuChuPai->getChildByTag(1))->setEnabled(true);
		}
		else
		{
			((MenuItemFont *)_menuChuPai->getChildByTag(1))->setEnabled(false);
		}
	}
}

void SceneGame::saveOutCards(std::vector<PokeInfo>& vec)
{
	_vecOutCards.clear();

	for (int i = 0; i < vec.size(); i++)
	{
		_vecOutCards.push_back(vec[i]._num);
	}
}

void SceneGame::gameover()
{
	_gameover = true;

	unschedule(schedule_selector(SceneGame::OutCard));

	_menuRestart->setVisible(true);
	_menuChuPai->setVisible(false);
	_menuQiangDiZhu->setVisible(false);
	_menuReady->setVisible(false);

	jiesuan();
}

void SceneGame::jiesuan()
{
	// 结算页面

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto jiesuanBg = Sprite::create("gameover/bg_small.png");
	jiesuanBg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(jiesuanBg, 100);

	auto tiaofu = Sprite::create("gameover/gameresult_win_tiaofu.png");
	tiaofu->setPosition(Vec2(350, 400));
	jiesuanBg->addChild(tiaofu, 0);

	auto shengli = Sprite::createWithSpriteFrameName("ddz_result_base_word_win.png");
	shengli->setPosition(Vec2(350, 400));
	jiesuanBg->addChild(shengli, 0);
}

unsigned int SceneGame::GetCardLogicValue(cocos2d::CCArray* cards)
{
	//获取类型  
	CARD_TYPE cbNextType= PanDuanPaiXing(cards);
	std::vector<int> cards_num;
	for (int i=0; i<cards->count(); i++)
	{
		Poke* card = dynamic_cast<Poke*>(cards->getObjectAtIndex(i));
		if (card != NULL)
		{
			cards_num.push_back(card->_info._num);
		}
	}

	std::sort(cards_num.begin(), cards_num.end());
	unsigned int length = cards_num.size();

	// 将数组中的牌数分到结构体CRAD_INDEX中
	CRAD_INDEX card_index;

	for (int i=0; i<length; )
	{
		if (i+1 < length && cards_num[i] == cards_num[i+1])
		{
			if (i+2 < length && cards_num[i+1] == cards_num[i+2])
			{
				if (i+3 < length && cards_num[i+2] == cards_num[i+3])
				{
					card_index.four_index.push_back(cards_num[i]);
					i += 4;
				}
				else
				{
					card_index.three_index.push_back(cards_num[i]);
					i += 3;
				}
			}
			else
			{
				card_index.double_index.push_back(cards_num[i]);
				i += 2;
			}
		}
		else
		{
			card_index.single_index.push_back(cards_num[i]);
			i++;
		}
	}


	//开始对比  
	switch(cbNextType)  
	{  
	case SINGLE_CARD:  
		{
			assert(length == 1);
			return cards_num[0];
		}
	case DOUBLE_CARD:  
		{
			assert(length == 2);
			return cards_num[0];
		}
	case THREE_CARD:  
		{
			assert(length == 3);
			return cards_num[0];
		}
	case CONNECT_CARD:  
		{
			return cards_num[length - 1];
		}
	case COMPANY_CARD: 
		{
			return cards_num[length - 1];
		}
	case AIRCRAFT_CARD:  
		{
			return cards_num[length - 1];
		}
	case MISSILE_CARD:
		{
			assert(length == 2);
			return cards_num[0];
		}
	case BOMB_CARD:  
		{
			assert(length == 4);
			return cards_num[0];
		}
	case THREE_ONE_CARD:  
		{
			assert(card_index.three_index.size() == 1);
			return card_index.three_index[0];
		}
	case THREE_TWO_CARD:  
		{
			assert(card_index.three_index.size() == 1);
			return card_index.three_index[0];
		}
	case BOMB_TWO_CARD:  
		{
			assert(card_index.four_index.size() == 1);
			return card_index.four_index[0];
		}
	case BOMB_TWOOO_CARD:
		{
			assert(card_index.four_index.size() == 1);
			return card_index.four_index[0];
		}
	}  
}

bool SceneGame::CompareCard(CCArray* firstCard, CCArray* nextCard)  
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
	case BOMB_CARD:  
	case THREE_ONE_CARD:  
	case THREE_TWO_CARD:
	case BOMB_TWO_CARD:  
	case BOMB_TWOOO_CARD:
		{  
			//获取数值  
			BYTE cbNextLogicValue=GetCardLogicValue(nextCard);  
			BYTE cbFirstLogicValue=GetCardLogicValue(firstCard);  

			//对比扑克  
			return cbNextLogicValue>cbFirstLogicValue;  
		}  
	case CONNECT_CARD:  
	case COMPANY_CARD:  
	case AIRCRAFT_CARD:  
		{
			if (firstCard->count() == nextCard->count())
			{
				//获取数值  
				BYTE cbNextLogicValue=GetCardLogicValue(nextCard);  
				BYTE cbFirstLogicValue=GetCardLogicValue(firstCard);  

				//对比扑克  
				return cbNextLogicValue>cbFirstLogicValue;  
			}
		}
	}  

	return false;  
}  

CARD_TYPE SceneGame::PanDuanPaiXing(CCArray* cards)
{
	std::vector<int> vec;
	for (int i=0; i<cards->count(); i++)
	{
		Poke* poke = dynamic_cast<Poke*>(cards->objectAtIndex(i));
		if (poke != NULL)
		{
			vec.push_back(poke->_info._num);
		}
	}
	
	return PanDuanPaiXing(vec);
}

CARD_TYPE SceneGame::PanDuanPaiXing(std::vector<int>& cards)
{
	unsigned int length = cards.size();
	std::sort(cards.begin(), cards.end());

	//小于5张牌
	//单牌，对子，3不带,炸弹通用算法
	if(length > 0 && length < 5) 
	{
		// 单牌/对子/三不带/炸弹
		if(cards[0] == cards[length - 1])
		{
			return (CARD_TYPE)length;
		}

		// 火箭
		if (cards[0] == NUM_XW && cards[1] == NUM_DW && length == 2)
		{
			return MISSILE_CARD;
		}

		// 三带一
		if ((cards[0] == cards[length - 2] && length == 4)
			|| (cards[1] == cards[length - 1] && length == 4))
		{
			return THREE_ONE_CARD;
		}
	}

	// 大于等于5张牌
	else if (length >= 5)
	{
		// 连牌
		if (IsContinuous(cards) && IsLessTwo(cards))
		{
			return CONNECT_CARD;
		}

		// 连对
		if (length >= 6 && length % 2 == 0) // 判断是否大于六张，且为双数
		{
			// 判断是否都是对子
			bool is_all_double = true;

			for (int i=0; i<length; i+=2)
			{
				if (cards[i] != cards[i+1])
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
					vec_single.push_back(cards[i]);
				}

				if (IsContinuous(vec_single))
				{
					return COMPANY_CARD;
				}
			}
		}

		// 将数组中的牌数分到结构体CRAD_INDEX中
		CRAD_INDEX card_index;

		for (int i=0; i<length; )
		{
			if (i+1 < length && cards[i] == cards[i+1])
			{
				if (i+2 < length && cards[i+1] == cards[i+2])
				{
					if (i+3 < length && cards[i+2] == cards[i+3])
					{
						card_index.four_index.push_back(cards[i]);
						i += 4;
					}
					else
					{
						card_index.three_index.push_back(cards[i]);
						i += 3;
					}
				}
				else
				{
					card_index.double_index.push_back(cards[i]);
					i += 2;
				}
			}
			else
			{
				card_index.single_index.push_back(cards[i]);
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