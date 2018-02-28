#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Poke.h"

struct CARDSTYLE
{

	int max;//最大值
	int min;//最小值
	int m_value;//权值
	int m_ID;
	bool m_ISprimary;//主次之分,true为主牌,false为次牌 
	CARDSTYLE()
	{
		m_ISprimary =false;
	}
};

struct ZONGCARDS
{
	std::vector <CARDSTYLE>  s_rocket;//火箭
	std::vector <CARDSTYLE>  s_bosb;//炸弹
	std::vector <CARDSTYLE>  s_three;//3条
	std::vector <CARDSTYLE>  s_plane;//飞机
	std::vector <CARDSTYLE>  s_link;//连牌
	std::vector <CARDSTYLE>  s_doubleLink;//双顺
	std::vector <CARDSTYLE>  s_double;//对子
	std::vector <CARDSTYLE>  s_single;//单张
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
		//removeAllChildrenWithCleanup(true);

		//添加精灵
		for (int i=0; i<arrayIndexs.size(); ++i)
		{
			auto card = Poke::create(arrayIndexs.at(i));
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

class GameScene;

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

	void FaPai(GameScene* scene, PokeInfo info);
	void ChuPai(GameScene* scene);
	void BuChu();
	void ChaiPai();

	void JudeLink();//判断连牌
	void JudeDoubleLink(std::vector<CARDSTYLE> &,std::vector<CARDSTYLE> &,bool);//判断连队
	void JudeFly(std::vector<CARDSTYLE> &,std::vector<CARDSTYLE>&,bool);//判断飞机

protected:

	bool _isHero;
	bool _isDiZhu;

	cocos2d::Label*	_labelPlayerName;
	cocos2d::Label*	_labelPlayerIdentity;
	cocos2d::Label*	_labelPokeCount;

	std::vector<PokeInfo>	_vecPokeInfo;

	PokeExhibitionZone* _exhibitionZone;

	std::vector<CARDSTYLE>  m_rocket;//火箭
	std::vector<CARDSTYLE>  m_bomb;//炸弹
	std::vector<CARDSTYLE>  m_three;//3条
	std::vector<CARDSTYLE>  m_plane;//飞机
	std::vector<CARDSTYLE>  m_link;//连牌
	std::vector<CARDSTYLE>  m_doubleLink;//双顺
	std::vector<CARDSTYLE>  m_double;//对子
	std::vector<CARDSTYLE>  m_single;//单张
	std::vector<ZONGCARDS>  m_handNum;//牌的手数

	int m_intArray[15];          //记录每张牌出现的次数3----大鬼
};

#endif // __PLAYER_H__
