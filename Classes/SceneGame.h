#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "cocos2d.h"
#include "Player.h"
#include "BottomCardZone.h"

// 判断是否连续
bool IsContinuous(cocos2d::CCArray* vecPoke);
bool IsContinuous(std::vector<int>& vec);

// 判断是否都小于2
bool IsLessTwo(cocos2d::CCArray* vecPoke);
bool IsLessTwo(std::vector<int>& vecPoke);

//---------------------------------------------
// 游戏场景
//---------------------------------------------
class SceneGame : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

    virtual bool init();

    // 菜单回调
	void menuBackCallback(cocos2d::Ref* pSender);
	void menuRestartCallback(cocos2d::Ref* pSender);
	void menuBuQiangCallback(cocos2d::Ref* pSender);
    void menuBuChuCallback(cocos2d::Ref* pSender);
	void menuChuPaiCallback(cocos2d::Ref* pSender);

	void callbackChuPai2(cocos2d::Node* node);
	void callbackChuPai3(cocos2d::Node* node);

	void update(float delta);
	void OutCard(float delta);

	void gameover();

	void initCards();

	// 获取权值
	unsigned int GetCardLogicValue(cocos2d::CCArray* cards);
	// true next大
	bool CompareCard(cocos2d::CCArray* firstCard, cocos2d::CCArray* nextCard);
	CARD_TYPE PanDuanPaiXing(cocos2d::CCArray* cards);//判断牌型
	CARD_TYPE PanDuanPaiXing(std::vector<int>& cards);//判断牌型
	cocos2d::CCArray* getArrPlayerOut() { return _arrPlayerOut; }

	void saveOutCards(std::vector<PokeInfo>& vec);
    
    CREATE_FUNC(SceneGame);

protected:

	std::vector<PokeInfo> _vecDiPai;

	Player* _player1;
	Player* _player2;
	Player* _player3;

	BottomCardZone* _bottomCardZone;

	cocos2d::Menu*	_chuPaiMenu;
	cocos2d::Menu*	_qiangDiZhuMenu;
	cocos2d::Menu*	_restartMenu;

	int		_state;	// 当前状态，1出牌，2等待

	std::vector<PokeInfo> _pokeInfo;

	cocos2d::CCArray*	_arrPlayerOut;	// 玩家选中的牌数

	std::vector<int>	_vecOutCards;	// 出牌存储区
};

#endif // __SCENE_GAME_H__
