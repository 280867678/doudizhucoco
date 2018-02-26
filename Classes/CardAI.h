#ifndef __CARD_AI_H__
#define __CARD_AI_H__

#include "cocos2d.h"
#include "Card.h"

enum  CARDTYPE
{
	SINGLECARD=1,                      //����
	BOTHCARDS,                         //����
	LINKCARDS,                         //˳��
	LINKBOTHCARDS,                     //����
	THREECARDS,                        //3����
	THREECARDSOFONE,                   //����һ
	THREECARDSOFTWO,                   //3��2
	PLANECARDS,                        //�ɻ�����
	PLANECARDSOFONE,                   //�ɻ�������
	PLANECARDSOFTWO,                   //�ɻ���2��
	BOMBCARDS,                         //ը��
	BOMBCARDSOFONE,                    //�ĸ���2��;
	BOMBCARDSOFTWO,                    //4����1��
	ERRORCARDS                         //�������(���Ͳ���ȷ)
};

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

struct ARRAYSENDCARD 
{
	int max;  //��ǰ���Ƶ����ֵ
	CARDTYPE type;//��ǰ���Ƶ�����
	int whoHost;//�Ƿ��ǵ���������
	bool isHost;
	int cardNum;
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

struct NUMBER//���㵥�ţ����ӣ�3�ţ�ը�����ֵĴ���
{
	int first;
	int second;
	int three;
	int four;
	int max;
};

//---------------------------------------------
// ���ܳ�����
//---------------------------------------------
class CardAI
{
public:

	//�ж�����(����:���Ƶ����飬���ֵ)
	CARDTYPE JudgeCardsType(std::vector<CardInfo>& sendCard);

	int		_intArray[15];          //��¼ÿ���Ƴ��ֵĴ���3----���
	std::vector<CardInfo> m_send;//������ʱ����
	NUMBER  m_card;//���ţ����ӣ�3�ţ�4�ų��ֵĴ���
};

#endif // __CARD_AI_H__
