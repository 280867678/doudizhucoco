#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "cocos2d.h"
#include "Player.h"
#include "BottomCardZone.h"

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
	void menuReadyCallback(cocos2d::Ref* pSender);
	void menuRestartCallback(cocos2d::Ref* pSender);
	void menuQiangCallback(cocos2d::Ref* pSender);
	void menuBuQiangCallback(cocos2d::Ref* pSender);
    void menuBuChuCallback(cocos2d::Ref* pSender);
	void menuChuPaiCallback(cocos2d::Ref* pSender);

	void callbackChuPai2(cocos2d::Node* node);
	void callbackChuPai3(cocos2d::Node* node);

	void update(float delta);
	void OutCard(float delta);

	// 发牌
	void faPai();
	void callbackFaPai(cocos2d::Node* node);
	void FaDiPai(Player* player);

	void gameover(int winID);
	void jiesuan(int winID);
	void setChuPaiMenuEnabled(bool isChuPai, bool isBuChu);

	void initCards();

	CARDS_DATA PanDuanPaiXing(cocos2d::CCArray* cards);//判断牌型
	CARDS_DATA PanDuanPaiXing(std::vector<PokeInfo>& cards);//判断牌型
	CARDS_DATA PanDuanPaiXing(std::vector<int>& cards);//判断牌型
	cocos2d::CCArray* getArrPlayerOut() { return _arrPlayerOut; }

	//屏幕截图
	void capture(Ref* sender);
	//截图后执行afterCapture
	void afterCapture(bool succeed, const std::string& outputFile);
    
    CREATE_FUNC(SceneGame);

protected:

	std::vector<PokeInfo> _vecDiPai;

	Player* _player1;
	Player* _player2;
	Player* _player3;

	BottomCardZone* _bottomCardZone;

	cocos2d::Menu*	_menuChuPai;
	cocos2d::Menu*	_menuQiangDiZhu;
	cocos2d::Menu*	_menuReady;

	int		_state;	// 当前状态，1出牌，2等待
	bool	_gameover;
	bool	_begin;

	std::vector<PokeInfo> _pokeInfo;

	cocos2d::CCArray*	_arrPlayerOut;	// 玩家选中的牌数
};

#endif // __SCENE_GAME_H__
