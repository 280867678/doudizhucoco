#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Poke.h"

struct CARDSTYLE
{

	int max;//���ֵ
	int min;//��Сֵ
	int m_value;//Ȩֵ
	int m_ID;
	bool m_ISprimary;//����֮��,trueΪ����,falseΪ���� 
	CARDSTYLE()
	{
		m_ISprimary =false;
	}
};

struct ZONGCARDS
{
	std::vector <CARDSTYLE>  s_rocket;//���
	std::vector <CARDSTYLE>  s_bosb;//ը��
	std::vector <CARDSTYLE>  s_three;//3��
	std::vector <CARDSTYLE>  s_plane;//�ɻ�
	std::vector <CARDSTYLE>  s_link;//����
	std::vector <CARDSTYLE>  s_doubleLink;//˫˳
	std::vector <CARDSTYLE>  s_double;//����
	std::vector <CARDSTYLE>  s_single;//����
};

// ������

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
		// ��ʼ������--------------------------------------------------------------
		if ( !Sprite::init() )
		{
			return false;
		}

		return true;
	}

	void chuPai(std::vector<PokeInfo> arrayIndexs)
	{
		//removeAllChildrenWithCleanup(true);

		//��Ӿ���
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
// �����
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

	void JudeLink();//�ж�����
	void JudeDoubleLink(std::vector<CARDSTYLE> &,std::vector<CARDSTYLE> &,bool);//�ж�����
	void JudeFly(std::vector<CARDSTYLE> &,std::vector<CARDSTYLE>&,bool);//�жϷɻ�

protected:

	bool _isHero;
	bool _isDiZhu;

	cocos2d::Label*	_labelPlayerName;
	cocos2d::Label*	_labelPlayerIdentity;
	cocos2d::Label*	_labelPokeCount;

	std::vector<PokeInfo>	_vecPokeInfo;

	PokeExhibitionZone* _exhibitionZone;

	std::vector<CARDSTYLE>  m_rocket;//���
	std::vector<CARDSTYLE>  m_bomb;//ը��
	std::vector<CARDSTYLE>  m_three;//3��
	std::vector<CARDSTYLE>  m_plane;//�ɻ�
	std::vector<CARDSTYLE>  m_link;//����
	std::vector<CARDSTYLE>  m_doubleLink;//˫˳
	std::vector<CARDSTYLE>  m_double;//����
	std::vector<CARDSTYLE>  m_single;//����
	std::vector<ZONGCARDS>  m_handNum;//�Ƶ�����

	int m_intArray[15];          //��¼ÿ���Ƴ��ֵĴ���3----���
};

#endif // __PLAYER_H__
