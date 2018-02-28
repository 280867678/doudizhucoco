#include "Player.h"
#include "GameScene.h"

USING_NS_CC;

Player* Player::create(std::string name, bool isDiZhu, bool isHero)
{
	Player *sprite = new (std::nothrow) Player();
	if (sprite && sprite->init(name, isDiZhu, isHero))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool Player::init(std::string name, bool isDiZhu, bool isHero)
{
    if ( !Sprite::init() )
    {
        return false;
    }

	_isHero = isHero;

	// �������
	_labelPlayerName = Label::createWithSystemFont(name, "����", 24);
	_labelPlayerName->setColor(Color3B(255, 255, 0));
	_labelPlayerName->setPosition(0,0);
	this->addChild(_labelPlayerName,1);

	// ������
	_labelPlayerIdentity = Label::createWithSystemFont(isDiZhu ? "D" : "N", "����", 24);
	_labelPlayerIdentity->setColor(Color3B(0, 255, 0));
	_labelPlayerIdentity->setPosition(100,0);
	this->addChild(_labelPlayerIdentity,1);

	// �������
	_labelPokeCount = Label::createWithSystemFont("0", "����", 24);
	_labelPokeCount->setPosition(200,0);
	this->addChild(_labelPokeCount,1);

	// ������
	_exhibitionZone = PokeExhibitionZone::create();
	_exhibitionZone->setPosition(300, 0);
	this->addChild(_exhibitionZone, 1);

    return true;
}

void Player::FaPai(GameScene* scene, PokeInfo info)
{
	_vecPokeInfo.push_back(info);

	if (/*_isHero*/true)
	{
		//��Ӿ���
		auto card = Poke::create(info);
		card->setPosition(0, -50);
		this->addChild(card, 100-info._num);
		card->SetTouchEnabled();
		card->setGameMain(scene);
	}

	sortAllChildren();

	updateCards();
}

void Player::ChuPai(GameScene* scene)
{
	std::vector<PokeInfo> arrayIndexToChuPai;

	if (_isHero)
	{
		for (auto it=_children.begin(); it!=_children.end(); it++)
		{
			Poke* card = dynamic_cast<Poke*>(*it);
			if (card != NULL && card->isSelected())
			{
				arrayIndexToChuPai.push_back(card->getInfo());
			}
		}

		for (int j=0; j<arrayIndexToChuPai.size(); j++)
		{
			for (auto it=_children.begin(); it!=_children.end(); it++)
			{
				Poke* card = dynamic_cast<Poke*>(*it);
				if (card != NULL && 
					card->getInfo() == arrayIndexToChuPai.at(j))
				{
					removeChild(card, true);
					_vecPokeInfo.erase(
						std::remove(_vecPokeInfo.begin(),_vecPokeInfo.end(),card->getInfo()),_vecPokeInfo.end());
					break;
				}
			}
		}
	}
	else
	{
		arrayIndexToChuPai.push_back(_vecPokeInfo.back());
		_vecPokeInfo.pop_back();

		// ��ʱ��ӣ��Ժ�ɾ��
		for (int j=0; j<arrayIndexToChuPai.size(); j++)
		{
			for (auto it=_children.begin(); it!=_children.end(); it++)
			{
				Poke* card = dynamic_cast<Poke*>(*it);
				if (card != NULL && 
					card->getInfo() == arrayIndexToChuPai.at(j))
				{
					removeChild(card, true);
					_vecPokeInfo.erase(
						std::remove(_vecPokeInfo.begin(),_vecPokeInfo.end(),card->getInfo()),_vecPokeInfo.end());
					break;
				}
			}
		}
	}
	
	_exhibitionZone->chuPai(arrayIndexToChuPai);

	updateCards();

	if (_vecPokeInfo.empty())
	{
		// ʤ��
		scene->gameover();
	}
}

void Player::updateCards()
{
	std::stringstream text;
	text << _vecPokeInfo.size();
	_labelPokeCount->setString(text.str());

	for (int i=0; i<_children.size(); i++)
	{
		Poke* card = dynamic_cast<Poke*>(_children.at(i));
		if (card != NULL)
		{
			card->setPosition(-150+i*40, card->getPosition().y);
		}
	}
}

void Player::clearCards()
{
	_exhibitionZone->removeAllChildrenWithCleanup(true);
}

void Player::BuChu()
{
	for (int i=0; i<_children.size(); i++)
	{
		Poke* card = dynamic_cast<Poke*>(_children.at(i));
		if (card != NULL)
		{
			card->unSelect();
		}
	}
}

CARDSTYLE temp;
int arrayTemp[15];

void Player::ChaiPai()
{
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

	m_bomb.clear();
	m_double.clear();
	m_doubleLink.clear();
	m_link.clear();
	m_plane.clear();
	m_rocket.clear();
	m_single.clear();
	m_three.clear();

	std::vector<int> vec_poke;
	for (int i=0; i<_children.size(); i++)
	{
		Poke* card = dynamic_cast<Poke*>(_children.at(i));
		if (card != NULL)
		{
			vec_poke.push_back(card->_info._num);
		}
	}

	std::sort(vec_poke.begin(), vec_poke.end());

	for(int i=0;i<15;i++) m_intArray[i]=0;//������������

	for (int i=0;i<_vecPokeInfo.size();i++)
	{
		m_intArray[_vecPokeInfo[i]._num]+=1;
	}	
	for (int i=0;i<15;i++)arrayTemp[i]=m_intArray[i];

	//���
	if (m_intArray[13]==1 && m_intArray[14]==1)
	{
		temp.max=15;
		temp.m_ISprimary =true;
		m_rocket.push_back(temp);
	}
	// �����е���ѹ�븱��
	for (int i=0;i<13;i++)
	{
		if (m_intArray[i]==4)
		{
			temp.max = i;
			temp.min =i;
			temp.m_ISprimary =false;
			m_bomb.push_back(temp);
		}
		if (m_intArray[i]>=3)
		{
			temp.max = i;
			temp.min =i;
			temp.m_ISprimary =false;
			m_three.push_back(temp);
		}
		if (m_intArray[i]>=2)
		{
			temp.max = i;
			temp.min =i;
			temp.m_ISprimary =false;
			m_double.push_back(temp);

		}
		if (m_intArray[i]>=1)
		{
			temp.max = i;
			temp.min =i;
			temp.m_ISprimary =false;
			m_single.push_back(temp);
		}
	}
	JudeFly(m_three,m_plane,false);//�ж�3�����Ƿ��зɻ�
	JudeDoubleLink(m_double,m_doubleLink,false);//�ж϶������Ƿ���˫˳
	JudeLink();//�ж�����

	//���Ƽ�������
	//SplitCards();
}

void Player::JudeFly(std::vector<CARDSTYLE> &Three,std::vector<CARDSTYLE> &plane,bool is)//�жϷɻ�
{
	if (Three.size()>=2)
	{
		int index=0;
		int j=0;
		for (int i=0;i<Three.size()-1;i++)
		{
			if (Three[i].max+1==Three[i+1].max&&(Three[i+1].max<12))
			{
				index++;
				j=Three[i].max+1;
			}else 
			{
				if (index>=1)
				{
					temp.max=j;
					temp.min=j-index;
					temp.m_ISprimary=is;
					plane.push_back(temp); 
				}
				index =0;
			}
		}
		if (index>=1)
		{
			temp.max=j;
			temp.min=j-index;
			temp.m_ISprimary=is;
			temp.m_value=temp.max-temp.min+5;
			plane.push_back(temp); 
		}
	}
}
void Player::JudeDoubleLink(std::vector<CARDSTYLE> &Double,std::vector<CARDSTYLE> &linkDouble,bool is)//�ж�����
{
	if (Double.size()>=3)
	{
		int index=0;
		int j=0;
		for (int i=0;i<Double.size()-1;i++)
		{
			if (Double[i].max+1==Double[i+1].max&&(Double[i+1].max<12))
			{
				index++;
				j=Double[i].max+1;
			}else 
			{
				if (index>=2)
				{
					temp.max=j;
					temp.min=j-index;
					temp.m_ISprimary=is;
					linkDouble.push_back(temp); 
				}
				index =0;
			}
		}
		if (index>=2)
		{
			temp.max=j;
			temp.min=j-index;
			temp.m_ISprimary=is;
			temp.m_value=temp.max-temp.min+4;
			linkDouble.push_back(temp); 
		}
	}
}

void Player::JudeLink()//�ж�����
{
	int index=0;
	int j=0;
	for (int i=0;i<11;i++)
	{
		if (m_intArray[i]>0&&m_intArray[i+1]>0)
		{

			index++;
			j=i+1;
		}
		else
		{
			if (index>=4)
			{
				temp.max=j;
				temp.min=j-index;
				temp.m_ISprimary=false;
				m_link.push_back(temp);
			}
			index=0;
		}
	} 
	if (index>=4)
	{
		temp.max=j;
		temp.min=j-index;
		temp.m_ISprimary=false;
		m_link.push_back(temp);
	}
}
