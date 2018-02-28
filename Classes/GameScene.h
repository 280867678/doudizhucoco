#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Player.h"

bool IsContinuous(std::vector<int>& vec);
bool IsContinuous(int arr[], int len);

// 判断是否连续
bool CheckContinuous(cocos2d::CCArray* vecPoke);

// 判断是否都小于2
bool IsLessTwo(cocos2d::CCArray* vecPoke);

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

class GameScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

	void update(float delta);
	void OutCard(float delta);

	void gameover();
    
	void initCards();

    // a selector callback
    void menuBuChuCallback(cocos2d::Ref* pSender);
	void menuChuPaiCallback(cocos2d::Ref* pSender);

	void menuBuQiangCallback(cocos2d::Ref* pSender);

	void menuRestartCallback(cocos2d::Ref* pSender);

	void callbackChuPai2(cocos2d::Node* node);
	void callbackChuPai3(cocos2d::Node* node);

	// 获取权值
	unsigned int GetCardLogicValue(cocos2d::CCArray* cards);
	// true next大
	bool CompareCard(cocos2d::CCArray* firstCard, cocos2d::CCArray* nextCard);
	CARD_TYPE PanDuanPaiXing(cocos2d::CCArray* cards);//判断牌型
	cocos2d::CCArray* getArrPlayerOut() { return _arrPlayerOut; }
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

protected:

	PokeExhibitionZone* _diPai;
	std::vector<PokeInfo> _vecDiPai;

	Player* _player1;
	Player* _player2;
	Player* _player3;

	cocos2d::Menu*	_chuPaiMenu;
	cocos2d::Menu*	_qiangDiZhuMenu;
	cocos2d::Menu*	_restartMenu;

	int		_state;	// 当前状态，1出牌，2等待

	std::vector<PokeInfo> _pokeInfo;

	cocos2d::CCArray* _arrPlayerOut;
};

#endif // __GAME_SCENE_H__
