#ifndef __CARD_AI_H__
#define __CARD_AI_H__

#include "cocos2d.h"
#include "Card.h"

enum  CARDTYPE
{
	SINGLECARD=1,                      //单张
	BOTHCARDS,                         //对子
	LINKCARDS,                         //顺子
	LINKBOTHCARDS,                     //连队
	THREECARDS,                        //3不带
	THREECARDSOFONE,                   //三带一
	THREECARDSOFTWO,                   //3带2
	PLANECARDS,                        //飞机不带
	PLANECARDSOFONE,                   //飞机带单张
	PLANECARDSOFTWO,                   //飞机带2对
	BOMBCARDS,                         //炸弹
	BOMBCARDSOFONE,                    //四个带2个;
	BOMBCARDSOFTWO,                    //4个带1对
	ERRORCARDS                         //错误的牌(牌型不正确)
};

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

struct ARRAYSENDCARD 
{
	int max;  //当前出牌的最大值
	CARDTYPE type;//当前出牌的类型
	int whoHost;//是否是地主出的牌
	bool isHost;
	int cardNum;
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

struct NUMBER//计算单张，对子，3张，炸弹出现的次数
{
	int first;
	int second;
	int three;
	int four;
	int max;
};

//---------------------------------------------
// 智能出牌器
//---------------------------------------------
class CardAI
{
public:

	//判断牌型(参数:出牌的数组，最大值)
	CARDTYPE JudgeCardsType(std::vector<CardInfo>& sendCard);

	int		_intArray[15];          //记录每张牌出现的次数3----大鬼
	std::vector<CardInfo> m_send;//出牌临时数组
	NUMBER  m_card;//单张，对子，3张，4张出现的次数
};

#endif // __CARD_AI_H__
