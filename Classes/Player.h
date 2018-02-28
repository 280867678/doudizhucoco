#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Poke.h"

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
	void ChuPai();
	void BuChu();

protected:

	bool _isHero;
	bool _isDiZhu;

	cocos2d::Label*	_labelPlayerName;
	cocos2d::Label*	_labelPlayerIdentity;
	cocos2d::Label*	_labelPokeCount;

	std::vector<PokeInfo>	_vecPokeInfo;

	PokeExhibitionZone* _exhibitionZone;
};

#endif // __PLAYER_H__
