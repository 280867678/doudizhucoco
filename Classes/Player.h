#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Poke.h"

// 牌型
enum  CARD_TYPE
{
	SINGLE_CARD =1,			//单牌
	DOUBLE_CARD,			//对子
	THREE_CARD,				//3不带
	BOMB_CARD,				//炸弹
	MISSILE_CARD,			//火箭
	THREE_ONE_CARD,			//3带1
	THREE_TWO_CARD,			//3带2
	BOMB_TWO_CARD,			//四个带2张单牌
	BOMB_TWOOO_CARD,		//四个带2对
	CONNECT_CARD,			//连牌
	COMPANY_CARD,			//连队
	AIRCRAFT_CARD,			//飞机不带
	AIRCRAFT_SINGLE_CARD,	//飞机带单牌
	AIRCRAFT_DOUBLE_CARD,	//飞机带对子
	ERROR_CARD				//错误的牌型
};

// 牌型对应的权值
// 为每一种牌型定义权值的大小：

//	单张    1
//
//	对子    2
//
//	三带    3
//
//	连牌    4  (每多一张牌权值+1)
//
//	连对    5（每多一对牌，权值+2）
//
//	飞机	6（每对以飞机，权值在基础上+3）
//
//	炸弹    7（包括对王在内

// 每种牌的信息
struct CARDS_DATA
{
	std::vector<int>	_cards;
	CARD_TYPE			_type;
	unsigned int		_value;
};

// 出牌区

class PokeExhibitionZone : public cocos2d::Sprite
{

public:

	static PokeExhibitionZone* create()
	{
		PokeExhibitionZone *sprite = new (std::nothrow) PokeExhibitionZone();
		if (sprite && sprite->init())
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}

	virtual bool init()
	{
		// 初始化基类--------------------------------------------------------------
		if ( !Sprite::init() )
		{
			return false;
		}

		return true;
	}

	void chuPai(std::vector<PokeInfo> arrayIndexs)
	{
		if (arrayIndexs.empty())
		{
			auto label_buchu = cocos2d::Sprite::create("buchu.png");
			this->addChild(label_buchu);
		}

		//removeAllChildrenWithCleanup(true);

		//添加精灵
		for (int i=0; i<arrayIndexs.size(); ++i)
		{
			auto card = Poke::create(arrayIndexs.at(i));
			card->setScale(0.5);
			this->addChild(card, 100-arrayIndexs.at(i)._num);
		}

		sortAllChildren();

		int count = _children.size();
		for (int i=0; i<_children.size(); i++)
		{
			Poke* card = dynamic_cast<Poke*>(_children.at(i));
			if (card != NULL)
			{
				card->setPosition(i*30, 0);
			}
		}
	}
};

class SceneGame;

//
// 玩家类
//

class Player : public cocos2d::Sprite
{
public:

	static Player* create(std::string name, bool isDiZhu, bool isHero);

    virtual bool init(std::string name, bool isDiZhu, bool isHero);

	void updateCards();
	void clearCards();

	void FaPai(SceneGame* scene, PokeInfo info);
	void ChuPai(SceneGame* scene, bool isFollow, CARD_TYPE cardType, unsigned int count, unsigned int value);
	void BuChu();
	void ChaiPai();

	// 找相应的牌型
	std::vector<int>& FindFollowCards(CARD_TYPE cardType, unsigned int count);

protected:

	bool _isHero;
	bool _isDiZhu;

	cocos2d::Sprite*_cardsManager;
	cocos2d::Label*	_labelPlayerName;
	cocos2d::Label*	_labelPokeCount;

	std::vector<PokeInfo>	_vecPokeInfo;

	PokeExhibitionZone* _exhibitionZone;

	std::vector<CARDS_DATA>  _allCardGroups;

	int m_intArray[15];          //记录每张牌出现的次数3----大鬼
};

#endif // __PLAYER_H__
