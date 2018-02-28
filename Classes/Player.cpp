#include "Player.h"
#include "GameScene.h"

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

void Player::FaPai(GameScene* scene, PokeInfo info)
{
	_vecPokeInfo.push_back(info);

	if (_isHero)
	{
		//添加精灵
		auto card = Poke::create(info);
		card->setPosition(0, -50);
		this->addChild(card, 100-info._num);
		card->SetTouchEnabled();
		card->setGameMain(scene);
	}

	sortAllChildren();

	updateCards();
}

void Player::ChuPai()
{
	if (_vecPokeInfo.empty())
	{
		return ;
	}

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
		arrayIndexToChuPai.push_back(_vecPokeInfo.back());
		_vecPokeInfo.pop_back();
	}
	
	_exhibitionZone->chuPai(arrayIndexToChuPai);

	updateCards();
}

void Player::updateCards()
{
	std::stringstream text;
	text << _vecPokeInfo.size();
	_labelPokeCount->setString(text.str());

	for (int i=0; i<_children.size(); i++)
	{
		Poke* card = dynamic_cast<Poke*>(_children.at(i));
		if (card != NULL)
		{
			card->setPosition(-50+i*40, card->getPosition().y);
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