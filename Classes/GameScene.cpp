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

	// �ؿ��˵�

	auto itemRestart = MenuItemFont::create("Game over! Press this button to restart!", CC_CALLBACK_1(GameScene::menuRestartCallback, this));
	itemRestart->setFontSize(28);
	itemRestart->setColor(Color3B(255, 0, 0));

	_restartMenu = Menu::create(itemRestart, NULL);
	this->addChild(_restartMenu, 1);
	_restartMenu->setVisible(false);

	// �������˵�

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

    // ���Ʋ˵�

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

	// ���

	_player1 = Player::create("W1", true, true);
	_player1->setPosition(100, 200);
	this->addChild(_player1);

	_player2 = Player::create("P2", false, false);
	_player2->setPosition(100, 400);
	this->addChild(_player2);

	_player3 = Player::create("P3", false, false);
	_player3->setPosition(100, 600);
	this->addChild(_player3);

	// ������
	_diPai = PokeExhibitionZone::create();
	_diPai->setPosition(300, 50);
	this->addChild(_diPai, 1);

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
			//_bottomCardZone->Show(_pokeInfo.at(_i));
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

	_diPai->chuPai(_vecDiPai);
	
	// �Զ��ж�����
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
	// �ַ�����
	for (int i=0; i<_vecDiPai.size(); i++)
	{
		_player1->FaPai(this, _vecDiPai[i]);
	}

	// �л��˵��ɼ�
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
	// ��������ж���
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
	//��ȡ����  
	CARD_TYPE cbNextType= PanDuanPaiXing(cards);

	//��ʼ�Ա�  
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
	case CONNECT_CARD:  
	case COMPANY_CARD:  
	case AIRCRAFT_CARD:  
	case BOMB_CARD:  
		{  
			//��ȡ��ֵ  
			BYTE cbNextLogicValue=GetCardLogicValue(nextCard);  
			BYTE cbFirstLogicValue=GetCardLogicValue(firstCard);  

			//�Ա��˿�  
			return cbNextLogicValue>cbFirstLogicValue;  
		}  
	case THREE_ONE_CARD:  
	case THREE_TWO_CARD:  
		{  
// 			//�����˿�  
// 			tagAnalyseResult NextResult;  
// 			tagAnalyseResult FirstResult;  
// 			AnalysebCardData(cbNextCard,cbNextCount,NextResult);  
// 			AnalysebCardData(cbFirstCard,cbFirstCount,FirstResult);  
// 
// 			//��ȡ��ֵ  
// 			BYTE cbNextLogicValue=GetCardLogicValue(NextResult.cbThreeCardData[0]);  
// 			BYTE cbFirstLogicValue=GetCardLogicValue(FirstResult.cbThreeCardData[0]);  

			//�Ա��˿�  
//			return cbNextLogicValue>cbFirstLogicValue;  
		}  
	case BOMB_TWO_CARD:  
	case BOMB_TWOOO_CARD:  
		{  
// 			//�����˿�  
// 			tagAnalyseResult NextResult;  
// 			tagAnalyseResult FirstResult;  
// 			AnalysebCardData(cbNextCard,cbNextCount,NextResult);  
// 			AnalysebCardData(cbFirstCard,cbFirstCount,FirstResult);  
// 
// 			//��ȡ��ֵ  
// 			BYTE cbNextLogicValue=GetCardLogicValue(NextResult.cbFourCardData[0]);  
// 			BYTE cbFirstLogicValue=GetCardLogicValue(FirstResult.cbFourCardData[0]);  

			//�Ա��˿�  
//			return cbNextLogicValue>cbFirstLogicValue;  
		}  
	}  

	return false;  
}  

CARD_TYPE GameScene::PanDuanPaiXing(CCArray* cards)
{
	unsigned int length = cards->count();

	//С��5����
	//���ƣ����ӣ�3����,ը��ͨ���㷨
	if(length > 0 && length < 5) 
	{
		Poke* poke1 = dynamic_cast<Poke*>(cards->objectAtIndex(0));
		Poke* poke_1 = dynamic_cast<Poke*>(cards->objectAtIndex(length - 1));

		if(poke1->_info._num == poke_1->_info._num)
		{
			return (CARD_TYPE)length;
		}

		// ���
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

	// ���ڵ���5����
	else if (length >= 5)
	{
		// ����
		if (CheckContinuous(cards) && IsLessTwo(cards))
		{
			return CONNECT_CARD;
		}

		// ����
		if (length >= 6 && length % 2 == 0) // �ж��Ƿ�������ţ���Ϊ˫��
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

			// �ж��Ƿ��Ƕ���
			bool is_all_double = true;

			for (int i=0; i<length; i+=2)
			{
				if (vec[i] != vec[i+1])
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
			std::vector<int>	single_index;	//��¼���ŵ���
			std::vector<int>	double_index;	//��¼���ӵ���
			std::vector<int>	three_index;	//��¼3��
			std::vector<int>	four_index;		//��¼4��
		};

		// ���ƶ��н������õ�������

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

		// �������е������ֵ��ṹ��CRAD_INDEX��
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

bool IsContinuous(int arr[], int len)
{
	int MAX = -1;
	int MIN = -1;
	for (int i = 0; i < len; i++)
	{
		if (arr[i] != 0)
		{
			if (arr[i] > MAX || MAX == -1) // ��ֹ�����еķ�0��������� -1 Сʱ��������ȡ�������ķ�0���ֵ��
				MAX = arr[i];

			if (arr[i] < MIN || MIN == -1) // ��ֹ�����еķ�0��С������ -1 ��ʱ��������ȡ�������ķ�0��Сֵ��
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

// �ж��Ƿ�����
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