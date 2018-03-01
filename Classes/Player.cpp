#include "Player.h"
#include "SceneGame.h"

USING_NS_CC;

Player* Player::create(std::string name, bool isDiZhu, bool isHero)
{
	Player *sprite = new (std::nothrow) Player();
	if (sprite && sprite->init(name, isDiZhu, isHero))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool Player::init(std::string name, bool isDiZhu, bool isHero)
{
    if ( !Sprite::init() )
    {
        return false;
    }

	_isHero = isHero;

	// 玩家名称
	_labelPlayerName = Label::createWithSystemFont(name, "宋体", 24);
	_labelPlayerName->setColor(Color3B(255, 255, 0));
	_labelPlayerName->setPosition(0,0);
	this->addChild(_labelPlayerName,1);

	// 玩家身份
	_labelPlayerIdentity = Label::createWithSystemFont(isDiZhu ? "D" : "N", "宋体", 24);
	_labelPlayerIdentity->setColor(Color3B(0, 255, 0));
	_labelPlayerIdentity->setPosition(100,0);
	this->addChild(_labelPlayerIdentity,1);

	// 玩家头像
	auto avatorPlayer = Sprite::create(isDiZhu ? "touxiang_dizhu.png" : "touxiang_nongmin.png");
	this->addChild(avatorPlayer, 0);

	// 玩家牌数
	_labelPokeCount = Label::createWithSystemFont("0", "宋体", 24);
	_labelPokeCount->setPosition(200,0);
	this->addChild(_labelPokeCount,1);

	// 出牌区
	_exhibitionZone = PokeExhibitionZone::create();
	_exhibitionZone->setPosition(300, 0);
	this->addChild(_exhibitionZone, 1);

    return true;
}

void Player::FaPai(SceneGame* scene, PokeInfo info)
{
	_vecPokeInfo.push_back(info);

	if (/*_isHero*/true)
	{
		//添加精灵
		auto card = Poke::create(info,!_isHero);
		card->setPosition(0, -150);
		this->addChild(card, info._num);
		card->SetTouchEnabled();
		card->setGameMain(scene);
	}

	sortAllChildren();

	updateCards();
}

void Player::ChuPai(SceneGame* scene, bool isFollow, CARD_TYPE cardType, unsigned int count, unsigned int value)
{
	std::vector<PokeInfo> arrayIndexToChuPai;

	if (_isHero)
	{
		for (auto it=_children.begin(); it!=_children.end(); it++)
		{
			Poke* card = dynamic_cast<Poke*>(*it);
			if (card != NULL && card->isSelected())
			{
				arrayIndexToChuPai.push_back(card->getInfo());
			}
		}

		for (int j=0; j<arrayIndexToChuPai.size(); j++)
		{
			for (auto it=_children.begin(); it!=_children.end(); it++)
			{
				Poke* card = dynamic_cast<Poke*>(*it);
				if (card != NULL && 
					card->getInfo() == arrayIndexToChuPai.at(j))
				{
					removeChild(card, true);
					_vecPokeInfo.erase(
						std::remove(_vecPokeInfo.begin(),_vecPokeInfo.end(),card->getInfo()),_vecPokeInfo.end());
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
				arrayIndexToChuPai.push_back(info);
			}
		}
		else
		{
			arrayIndexToChuPai.push_back(_vecPokeInfo.back());
			_vecPokeInfo.pop_back();
		}

		// 暂时添加，以后删除
		for (int j=0; j<arrayIndexToChuPai.size(); j++)
		{
			for (auto it=_children.begin(); it!=_children.end(); it++)
			{
				Poke* card = dynamic_cast<Poke*>(*it);
				if (card != NULL && 
					card->getInfo() == arrayIndexToChuPai.at(j))
				{
					removeChild(card, true);
					_vecPokeInfo.erase(
						std::remove(_vecPokeInfo.begin(),_vecPokeInfo.end(),card->getInfo()),_vecPokeInfo.end());
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
		// 胜利
		scene->gameover();
	}
}

std::vector<int>& Player::FindFollowCards(CARD_TYPE cardType, unsigned int count)
{
	// 暂时提取最大的

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
	ChaiPai();

	std::stringstream text;
	text << _vecPokeInfo.size();
	_labelPokeCount->setString(text.str());

	int count = _children.size();
	int zeroPoint = count/2;
	for (int i=0; i<count; i++)
	{
		Poke* card = dynamic_cast<Poke*>(_children.at(i));
		if (card != NULL)
		{
			card->setPosition(600+(i-zeroPoint)*50, card->getPosition().y);
		}
	}
}

void Player::clearCards()
{
	_exhibitionZone->removeAllChildrenWithCleanup(true);
}

void Player::BuChu()
{
	for (int i=0; i<_children.size(); i++)
	{
		Poke* card = dynamic_cast<Poke*>(_children.at(i));
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
	for (int i=0; i<_children.size(); i++)
	{
		Poke* card = dynamic_cast<Poke*>(_children.at(i));
		if (card != NULL)
		{
			vec_poke.push_back(card->_info._num);
		}
	}

	std::sort(vec_poke.begin(), vec_poke.end());

	//对牌数组清零
	for(int i=0;i<15;i++) arrayTemp[i]=0;

	for (int i=0;i<vec_poke.size();i++)
	{
		arrayTemp[vec_poke[i]]+=1;
	}

	// 开始拆分

	//火箭
	if (arrayTemp[NUM_XW]==1 && arrayTemp[NUM_DW]==1)
	{
		CARDS_DATA temp;
		temp._cards.push_back(NUM_XW);
		temp._cards.push_back(NUM_DW);
		temp._type = MISSILE_CARD;
		temp._value = 100;

		_allCardGroups.push_back(temp);
	}

	// 将所有的牌压入副牌
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
}
