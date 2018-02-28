#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Player.h"

bool IsContinuous(std::vector<int>& vec);
bool IsContinuous(int arr[], int len);

// �ж��Ƿ�����
bool CheckContinuous(cocos2d::CCArray* vecPoke);

// �ж��Ƿ�С��2
bool IsLessTwo(cocos2d::CCArray* vecPoke);

// ����
enum  CARD_TYPE
{
	SINGLE_CARD =1,			//����
	DOUBLE_CARD,			//����
	THREE_CARD,				//3����
	BOMB_CARD,				//ը��
	MISSILE_CARD,			//���
	THREE_ONE_CARD,			//3��1
	THREE_TWO_CARD,			//3��2
	BOMB_TWO_CARD,			//�ĸ���2�ŵ���
	BOMB_TWOOO_CARD,		//�ĸ���2��
	CONNECT_CARD,			//����
	COMPANY_CARD,			//����
	AIRCRAFT_CARD,			//�ɻ�����
	AIRCRAFT_SINGLE_CARD,	//�ɻ�������
	AIRCRAFT_DOUBLE_CARD,	//�ɻ�������
	ERROR_CARD				//���������
};

// ���Ͷ�Ӧ��Ȩֵ
// Ϊÿһ�����Ͷ���Ȩֵ�Ĵ�С��

//	����    1
//
//	����    2
//
//	����    3
//
//	����    4  (ÿ��һ����Ȩֵ+1)
//
//	����    5��ÿ��һ���ƣ�Ȩֵ+2��
//
//	�ɻ�	6��ÿ���Էɻ���Ȩֵ�ڻ�����+3��
//
//	ը��    7��������������

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

	// ��ȡȨֵ
	unsigned int GetCardLogicValue(cocos2d::CCArray* cards);
	// true next��
	bool CompareCard(cocos2d::CCArray* firstCard, cocos2d::CCArray* nextCard);
	CARD_TYPE PanDuanPaiXing(cocos2d::CCArray* cards);//�ж�����
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

	int		_state;	// ��ǰ״̬��1���ƣ�2�ȴ�

	std::vector<PokeInfo> _pokeInfo;

	cocos2d::CCArray* _arrPlayerOut;
};

#endif // __GAME_SCENE_H__
