#include "SceneGame.h"
#include "Player.h"
#include "SceneMenu.h"

USING_NS_CC;

struct CRAD_INDEX
{
	std::vector<int>	single_index;	//��¼���ŵ���
	std::vector<int>	double_index;	//��¼���ӵ���
	std::vector<int>	three_index;	//��¼3��
	std::vector<int>	four_index;		//��¼4��
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
    // ��ʼ������----------------------------------------
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// ����----------------------------------------------
	auto bg = Sprite::create("bg_table.jpg");
	bg->setScale(1/0.8);
	bg->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	this->addChild(bg, 0);

	// �������ز˵�---------------------------------------------
	auto itemBack = createCustomMenuItem("item_back.png", "item_back.png");
	itemBack->setCallback(CC_CALLBACK_1(SceneGame::menuBackCallback, this));
	itemBack->setPosition(visibleSize.width/2+200, visibleSize.height-50);

	auto menuBack = Menu::create(itemBack, NULL);
	menuBack->setPosition(origin);
	this->addChild(menuBack, 1);

	// �ؿ��˵�

	auto itemRestart = MenuItemFont::create("Game over! Press this button to restart!", CC_CALLBACK_1(SceneGame::menuRestartCallback, this));
	itemRestart->setFontSize(28);
	itemRestart->setColor(Color3B(255, 0, 0));

	_restartMenu = Menu::create(itemRestart, NULL);
	this->addChild(_restartMenu, 1);
	_restartMenu->setVisible(false);

	// �������˵�

	auto itemBuQiang = MenuItemFont::create("BuQiang", CC_CALLBACK_1(SceneGame::menuBuQiangCallback, this));
	itemBuQiang->setFontSize(28);
	itemBuQiang->setPosition(-150, -300);

	auto item1Fen = MenuItemFont::create("1$", CC_CALLBACK_1(SceneGame::menuBuQiangCallback, this));
	item1Fen->setFontSize(28);
	item1Fen->setPosition(-50, -300);

	auto item2Fen = MenuItemFont::create("2$", CC_CALLBACK_1(SceneGame::menuBuQiangCallback, this));
	item2Fen->setFontSize(28);
	item2Fen->setPosition(50, -300);

	auto item3Fen = MenuItemFont::create("3$", CC_CALLBACK_1(SceneGame::menuBuQiangCallback, this));
	item3Fen->setFontSize(28);
	item3Fen->setPosition(150, -300);

	_qiangDiZhuMenu = Menu::create(itemBuQiang, item1Fen, item2Fen, item3Fen, NULL);
	this->addChild(_qiangDiZhuMenu, 1);

    // ���Ʋ˵�

	auto itemBuChu = MenuItemFont::create("BuChu", CC_CALLBACK_1(SceneGame::menuBuChuCallback, this));
	itemBuChu->setFontSize(28);
	itemBuChu->setPosition(-100, -300);

    auto itemChuPai = MenuItemFont::create("ChuPai", CC_CALLBACK_1(SceneGame::menuChuPaiCallback, this));
	itemChuPai->setFontSize(28);
	itemChuPai->setPosition(100, -300);

	_chuPaiMenu = Menu::create();
	_chuPaiMenu->addChild(itemBuChu,2,0);
	_chuPaiMenu->addChild(itemChuPai,2,1);
	this->addChild(_chuPaiMenu, 1);
	_chuPaiMenu->setVisible(false);

	// ���

	_player1 = Player::create("W1", true, true);
	_player1->setPosition(70, 300);
	this->addChild(_player1);

	_player2 = Player::create("P2", false, false);
	_player2->setPosition(1000, 600);
	this->addChild(_player2);

	_player3 = Player::create("P3", false, false);
	_player3->setPosition(70, 600);
	this->addChild(_player3);

	// ������
	_bottomCardZone = BottomCardZone::create();
	_bottomCardZone->setPosition(450, 480);
	this->addChild(_bottomCardZone, 1);


	// ��ʼ���ƶ�
	initCards();

	// ϴ��
	srand(time(0));
	std::random_shuffle(_pokeInfo.begin(), _pokeInfo.end());

	// ����

	for (int i = 0; i < 54; i++)
	{
		if (i >= 51)
		{
			//����
			_bottomCardZone->Show(_pokeInfo.at(i));
			_vecDiPai.push_back(_pokeInfo.at(i));
		}
		else
		{
			if (i%3==0)
			{
				// ��ɫ1
				_player1->FaPai(this, _pokeInfo.at(i));
			} else if (i%3==1)
			{
				// ��ɫ2
				_player2->FaPai(this, _pokeInfo.at(i));
			} else if (i%3==2)
			{
				// ��ɫ3
				_player3->FaPai(this, _pokeInfo.at(i));
			}
		}
	}
	
	// �Զ��ж�����
	schedule(schedule_selector(SceneGame::OutCard),0.5);
    
	_arrPlayerOut = CCArray::create();
	_arrPlayerOut->retain();

	_state = 1;

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

void SceneGame::menuBackCallback(Ref* pSender)
{
	Director::getInstance()->replaceScene(SceneMenu::createScene());
}

void SceneGame::menuRestartCallback(Ref* pSender)
{
	Director::getInstance()->replaceScene(SceneGame::createScene());
}

void SceneGame::menuBuQiangCallback(Ref* pSender)
{
	// �ַ�����
	for (int i=0; i<_vecDiPai.size(); i++)
	{
		_player1->FaPai(this, _vecDiPai[i]);
	}

	// �л��˵��ɼ�
	_qiangDiZhuMenu->setVisible(false);
	_chuPaiMenu->setVisible(true);
}

void SceneGame::menuBuChuCallback(Ref* pSender)
{
	_player3->clearCards();
	_player1->BuChu();

	auto delay = DelayTime::create(1);
	auto callback = CallFuncN::create(CC_CALLBACK_1(SceneGame::callbackChuPai2,this));
	auto seq = Sequence::createWithTwoActions(delay, callback);
	this->runAction(seq);

	_chuPaiMenu->setVisible(false);
}

void SceneGame::menuChuPaiCallback(Ref* pSender)
{
	_player3->clearCards();

	_player1->ChuPai(this, false, ERROR_CARD, 0, 1);
	// ��������ж���
	_arrPlayerOut->removeAllObjects();

	auto delay = DelayTime::create(1);
	auto callback = CallFuncN::create(CC_CALLBACK_1(SceneGame::callbackChuPai2,this));
	auto seq = Sequence::createWithTwoActions(delay, callback);
	this->runAction(seq);

	_chuPaiMenu->setVisible(false);
}

void SceneGame::callbackChuPai2(cocos2d::Node* node)
{
	// ����֮ǰ���ж��ϼҵ�����
	CARD_TYPE type_out_cards = PanDuanPaiXing(_vecOutCards);

	// �ж�֮�����

	_player1->clearCards();

	if (_vecOutCards.empty())
	{
		_player2->ChuPai(this, false, type_out_cards, _vecOutCards.size(), 1);// ����
	}
	else
	{
		_player2->ChuPai(this, true, type_out_cards, _vecOutCards.size(), 1);// ����
	}

	auto delay = DelayTime::create(1);
	auto callback = CallFuncN::create(CC_CALLBACK_1(SceneGame::callbackChuPai3,this));
	auto seq = Sequence::createWithTwoActions(delay, callback);
	this->runAction(seq);
}

void SceneGame::callbackChuPai3(cocos2d::Node* node)
{
	// ����֮ǰ���ж��ϼҵ�����
	CARD_TYPE type_out_cards = PanDuanPaiXing(_vecOutCards);

	// �ж�֮�����

	_player2->clearCards();

	if (_vecOutCards.empty())
	{
		_player3->ChuPai(this, false, type_out_cards, _vecOutCards.size(), 1);// ����
	}
	else
	{
		_player3->ChuPai(this, true, type_out_cards, _vecOutCards.size(), 1);// ����
	}

	_chuPaiMenu->setVisible(true);
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
			((MenuItemFont *)_chuPaiMenu->getChildByTag(1))->setEnabled(true);
		}
		else
		{
			((MenuItemFont *)_chuPaiMenu->getChildByTag(1))->setEnabled(false);
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
	_restartMenu->setVisible(true);
}

unsigned int SceneGame::GetCardLogicValue(cocos2d::CCArray* cards)
{
	//��ȡ����  
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

	// �������е������ֵ��ṹ��CRAD_INDEX��
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


	//��ʼ�Ա�  
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
	//��ȡ����  
	CARD_TYPE cbNextType= PanDuanPaiXing(nextCard);  
	CARD_TYPE cbFirstType= PanDuanPaiXing(firstCard);  

	//�����ж�  
	if(cbNextType==ERROR_CARD) return false;  
	if(cbNextType==MISSILE_CARD) return true;  

	//ը���ж�  
	if((cbFirstType!=BOMB_CARD)&&(cbNextType==BOMB_CARD)) return true;  
	if((cbFirstType==BOMB_CARD)&&(cbNextType!=BOMB_CARD)) return false;  

	//�����ж�  
	if((cbFirstType!=cbNextType)||(firstCard->count()!=nextCard->count())) return false;  

	//��ʼ�Ա�  
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
			//��ȡ��ֵ  
			BYTE cbNextLogicValue=GetCardLogicValue(nextCard);  
			BYTE cbFirstLogicValue=GetCardLogicValue(firstCard);  

			//�Ա��˿�  
			return cbNextLogicValue>cbFirstLogicValue;  
		}  
	case CONNECT_CARD:  
	case COMPANY_CARD:  
	case AIRCRAFT_CARD:  
		{
			if (firstCard->count() == nextCard->count())
			{
				//��ȡ��ֵ  
				BYTE cbNextLogicValue=GetCardLogicValue(nextCard);  
				BYTE cbFirstLogicValue=GetCardLogicValue(firstCard);  

				//�Ա��˿�  
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

	//С��5����
	//���ƣ����ӣ�3����,ը��ͨ���㷨
	if(length > 0 && length < 5) 
	{
		// ����/����/������/ը��
		if(cards[0] == cards[length - 1])
		{
			return (CARD_TYPE)length;
		}

		// ���
		if (cards[0] == NUM_XW && cards[1] == NUM_DW && length == 2)
		{
			return MISSILE_CARD;
		}

		// ����һ
		if ((cards[0] == cards[length - 2] && length == 4)
			|| (cards[1] == cards[length - 1] && length == 4))
		{
			return THREE_ONE_CARD;
		}
	}

	// ���ڵ���5����
	else if (length >= 5)
	{
		// ����
		if (IsContinuous(cards) && IsLessTwo(cards))
		{
			return CONNECT_CARD;
		}

		// ����
		if (length >= 6 && length % 2 == 0) // �ж��Ƿ�������ţ���Ϊ˫��
		{
			// �ж��Ƿ��Ƕ���
			bool is_all_double = true;

			for (int i=0; i<length; i+=2)
			{
				if (cards[i] != cards[i+1])
				{
					is_all_double = false;
					break;
				}
			}

			// �ж϶����Ƿ�����
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

		// �������е������ֵ��ṹ��CRAD_INDEX��
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

		// 3����
		if (card_index.three_index.size() == 1 && card_index.double_index.size() == 1 && card_index.four_index.empty() && card_index.single_index.empty())
		{
			return THREE_TWO_CARD;
		}

		// �ɻ�
		// ǰ�᣺�����������ŵ�
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

		// 4��2
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

// �ж��Ƿ�����

bool IsContinuous(std::vector<int>& vec)
{
	bool ret = true;

	// ����
	std::sort(vec.begin(), vec.end());

	// ������ɺ�Ƚ����ڵ��������ֵĲ�ֵ�����ȫΪ1����Ϊ����
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

// �ж��Ƿ�С��2
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