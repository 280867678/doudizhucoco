#include "Player.h"
#include "SceneGame.h"

USING_NS_CC;

Player* Player::create(std::string name, bool isHero)
{
	Player *sprite = new (std::nothrow) Player();
	if (sprite && sprite->init(name, isHero))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool Player::init(std::string name, bool isHero)
{
    if ( !Sprite::init() )
    {
        return false;
    }

	_isHero = isHero;

	// ���ƹ���
	_cardsManager = Sprite::create();
	_cardsManager->setPosition(0, -190);
	this->addChild(_cardsManager, 1);

	// �������
	_labelPlayerName = Label::createWithSystemFont(name, "����", 18);
	_labelPlayerName->setColor(Color3B(255, 255, 0));
	_labelPlayerName->setPosition(0,-70);
	this->addChild(_labelPlayerName,1);

	// ���ͷ��
	auto avatorBg = Sprite::create("item_bg1.png");
	avatorBg->setScale(0.8);
	this->addChild(avatorBg, 0);

	char str_avator_image[255] = { 0 };
	sprintf_s(str_avator_image, "head/vtouxiang_%02d.png", rand() % 14 + 1);
	auto avator = Sprite::create(str_avator_image);
	this->addChild(avator, 0);

	// �������
	auto back = Sprite::createWithSpriteFrameName("b/poker_back.png");
	back->setScale(0.3);
	back->setPosition(100,0);
	this->addChild(back,1);

	_labelPokeCount = Label::createWithSystemFont("0", "����", 130);
	_labelPokeCount->setPosition(70,110);
	back->addChild(_labelPokeCount,1);

	// ������
	_exhibitionZone = PokeExhibitionZone::create();
	if (isHero)
		_exhibitionZone->setPosition(500, 0);
	else
		_exhibitionZone->setPosition(0, -150);
	this->addChild(_exhibitionZone, 1);

    return true;
}

void Player::setDiZhu()
{
	_isDiZhu = true;

	// ������
	if (_isDiZhu)
	{
		auto lord = Sprite::create("flag_lord.png");
		lord->setPosition(160, 0);
		this->addChild(lord, 0);
	}
}

void Player::FaPai(SceneGame* scene, PokeInfo info)
{
	_vecPokeInfo.push_back(info);

	if (/*_isHero*/true)
	{
		//��Ӿ���
		auto card = Poke::create(info,!_isHero);
		_cardsManager->addChild(card, 100-info._num);
		card->SetTouchEnabled();
		card->setGameMain(scene);
	}

	_cardsManager->sortAllChildren();

	updateCards();
}

void Player::ChuPai(SceneGame* scene, bool isFollow, CARD_TYPE cardType, unsigned int count, unsigned int value)
{
	std::vector<PokeInfo> arrayIndexToChuPai;

	if (_isHero)
	{
		for (auto it=_cardsManager->getChildren().begin(); it!=_cardsManager->getChildren().end(); it++)
		{
			Poke* card = dynamic_cast<Poke*>(*it);
			if (card != NULL && card->isSelected())
			{
				arrayIndexToChuPai.push_back(card->getInfo());
			}
		}

		for (int j=0; j<arrayIndexToChuPai.size(); j++)
		{
			for (auto it=_cardsManager->getChildren().begin(); it!=_cardsManager->getChildren().end(); it++)
			{
				Poke* card = dynamic_cast<Poke*>(*it);
				if (card != NULL && card->getInfo() == arrayIndexToChuPai.at(j))
				{
					_vecPokeInfo.erase(std::find(_vecPokeInfo.begin(),_vecPokeInfo.end(),card->getInfo()));
					_cardsManager->removeChild(card, true);
					break;
				}
			}
		}
	}
	else
	{
		if (isFollow)
		{
			std::vector<int>& vec = FindFollowCards(cardType, count);

			if (vec.empty())
			{
				_exhibitionZone->chuPai(arrayIndexToChuPai);
				return;
			}

			for (int i=0; i<vec.size(); i++)
			{
				PokeInfo info;
				info._num = (PokeNum)vec[i];
				info._tag = (PokeTag)0;// ��Ϊ��ɫ��Ӱ����ֵ�Ĵ�С�����Գ���ȫ��Ĭ�ϻ�ɫΪ����
				arrayIndexToChuPai.push_back(info);
			}
		}
		else
		{
			arrayIndexToChuPai.push_back(_vecPokeInfo.back());
			_vecPokeInfo.pop_back();
		}

		// ��ʱ��ӣ��Ժ�ɾ��
		for (int j=0; j<arrayIndexToChuPai.size(); j++)
		{
			for (auto it=_cardsManager->getChildren().begin(); it!=_cardsManager->getChildren().end(); it++)
			{
				Poke* card = dynamic_cast<Poke*>(*it);
				if (card != NULL && card->getInfo() == arrayIndexToChuPai.at(j))
				{
					_vecPokeInfo.erase(std::find(_vecPokeInfo.begin(),_vecPokeInfo.end(),card->getInfo()));
					_cardsManager->removeChild(card, true);
					break;
				}
			}
		}
	}
	
	_exhibitionZone->chuPai(arrayIndexToChuPai);

	scene->saveOutCards(arrayIndexToChuPai);

	updateCards();

	if (_vecPokeInfo.empty())
	{
		// ʤ��
		scene->gameover();
	}
}

std::vector<int>& Player::FindFollowCards(CARD_TYPE cardType, unsigned int count)
{
	// ��ʱ��ȡ����

	for (int i=0; i<_allCardGroups.size(); i++)
	{
		if (cardType == _allCardGroups[i]._type)
		{
			if (cardType == CONNECT_CARD || cardType == COMPANY_CARD || cardType == AIRCRAFT_CARD)
			{
				if (count == _allCardGroups[i]._cards.size())
				{
					return _allCardGroups[i]._cards;
				}
			}
			else
			{
				return _allCardGroups[i]._cards;
			}
		}
	}	

	std::vector<int> tmp;
	return tmp;
}

void Player::updateCards()
{
	this->ChaiPai();

	std::stringstream text;
	text << _vecPokeInfo.size();
	_labelPokeCount->setString(text.str());

	int count = _cardsManager->getChildren().size();
	int zeroPoint = count/2;
	for (int i=0; i<count; i++)
	{
		Poke* card = dynamic_cast<Poke*>(_cardsManager->getChildren().at(i));
		if (card != NULL)
		{
			card->setPosition(550+(i-zeroPoint)*50, card->getPosition().y);
		}
	}
}

void Player::clearCards()
{
	_exhibitionZone->removeAllChildrenWithCleanup(true);
}

void Player::BuChu()
{
	for (int i=0; i<_cardsManager->getChildren().size(); i++)
	{
		Poke* card = dynamic_cast<Poke*>(_cardsManager->getChildren().at(i));
		if (card != NULL)
		{
			card->unSelect();
		}
	}
}

int arrayTemp[15];
void Player::ChaiPai()
{
	_allCardGroups.clear();

	std::vector<int> vec_poke;
	for (int i=0; i<_vecPokeInfo.size(); i++)
	{
		vec_poke.push_back(_vecPokeInfo[i]._num);
	}

	std::sort(vec_poke.begin(), vec_poke.end());

	//������������
	for(int i=0;i<15;i++) arrayTemp[i]=0;

	for (int i=0;i<vec_poke.size();i++)
	{
		arrayTemp[vec_poke[i]]+=1;
	}

	// ��ʼ���

	//���
	if (arrayTemp[NUM_XW]==1 && arrayTemp[NUM_DW]==1)
	{
		CARDS_DATA temp;
		temp._cards.push_back(NUM_XW);
		temp._cards.push_back(NUM_DW);
		temp._type = MISSILE_CARD;
		temp._value = 100;

		_allCardGroups.push_back(temp);
	}

	// �����е���ѹ�븱��
	for (int i=0;i<13;i++)
	{
		if (arrayTemp[i]==4)
		{
			CARDS_DATA temp;
			temp._cards.push_back(i);
			temp._cards.push_back(i);
			temp._cards.push_back(i);
			temp._cards.push_back(i);
			temp._type = BOMB_CARD;
			temp._value = 100;

			_allCardGroups.push_back(temp);
		}
		if (arrayTemp[i]>=3)
		{
			CARDS_DATA temp;
			temp._cards.push_back(i);
			temp._cards.push_back(i);
			temp._cards.push_back(i);
			temp._type = THREE_CARD;
			temp._value = 100;

			_allCardGroups.push_back(temp);
		}
		if (arrayTemp[i]>=2)
		{
			CARDS_DATA temp;
			temp._cards.push_back(i);
			temp._cards.push_back(i);
			temp._type = DOUBLE_CARD;
			temp._value = 100;

			_allCardGroups.push_back(temp);

		}
		if (arrayTemp[i]>=1)
		{
			CARDS_DATA temp;
			temp._cards.push_back(i);
			temp._type = SINGLE_CARD;
			temp._value = 100;

			_allCardGroups.push_back(temp);
		}
	}

	// �µĲ����㷨

	// 4����������
	std::vector<int> card_i;
	std::vector<int> card_ii;
	std::vector<int> card_iii;
	std::vector<int> card_iiii;

	// 1.���������Ʒֵ��ĸ���������
	for (int i=0; i<vec_poke.size(); i++)
	{
		// ��һ������
		auto itor_find_i = std::find(card_i.begin(), card_i.end(), vec_poke[i]);
		// �ҵ���
		if (itor_find_i != card_i.end())
		{
			// �ڶ�������
			auto itor_find_ii = std::find(card_ii.begin(), card_ii.end(), vec_poke[i]);
			// �ҵ���
			if (itor_find_ii != card_ii.end())
			{
				// ����������
				auto itor_find_iii = std::find(card_iii.begin(), card_iii.end(), vec_poke[i]);
				// �ҵ���
				if (itor_find_iii != card_iii.end())
				{
					// ���ĸ�����
					card_iiii.push_back(vec_poke[i]);
				}
				// û�ҵ�
				else
				{
					card_iii.push_back(vec_poke[i]);
				}
			}
			// û�ҵ�
			else
			{
				card_ii.push_back(vec_poke[i]);
			}
		}
		// û�ҵ�
		else
		{
			card_i.push_back(vec_poke[i]);
		}
		vec_poke[i];
	}

	std::sort(card_i.begin(), card_i.end());
	std::sort(card_ii.begin(), card_ii.end());
	std::sort(card_iii.begin(), card_iii.end());
	std::sort(card_iiii.begin(), card_iiii.end());

	// 2.���ĸ������������ж�����

	// ���ĸ������ж���ը��
	for (int i=0; i<card_iiii.size(); i++)
	{
		CARDS_DATA temp;
		temp._cards.push_back(card_iiii[i]);
		temp._cards.push_back(card_iiii[i]);
		temp._cards.push_back(card_iiii[i]);
		temp._cards.push_back(card_iiii[i]);
		temp._type = BOMB_CARD;
		temp._value = 100;

		_allCardGroups.push_back(temp);
	}

	// �����������ж���������ͬ�ģ�������������һ��������
	for (int i=0; i<card_iii.size(); i++)
	{
		CARDS_DATA temp;
		temp._cards.push_back(card_iii[i]);
		temp._cards.push_back(card_iii[i]);
		temp._cards.push_back(card_iii[i]);
		temp._type = THREE_CARD;
		temp._value = 100;

		_allCardGroups.push_back(temp);
	}
	
	// �ҵ���(�ڵ�һ�����飬�����ڵڶ������飬��������ɵ�˳)

	// ��˳
	for (int i=0; i<card_i.size(); i++)
	{
		auto itor = std::find(card_ii.begin(), card_ii.end(), card_i[i]);
		if (itor != card_ii.end())
		{
			for (int j=5; j<card_i.size(); j++)
			{
				if (i+j<card_i.size() && card_i[i+j]-card_i[i]==j)
				{
					CARDS_DATA temp;
					temp._type = CONNECT_CARD;
					temp._value = 100;
					for (int k=0; k<j; k++)
					{
						temp._cards.push_back(card_i[i+k]);
					}
					_allCardGroups.push_back(temp);
				}
			}
		}
	}

}
