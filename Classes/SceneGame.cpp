#include "SceneGame.h"
#include "Player.h"
#include "SceneMenu.h"
#include "SpriteBlur.h"

USING_NS_CC;

struct CRAD_INDEX
{
	std::vector<int>	single_index;	//记录单张的牌
	std::vector<int>	double_index;	//记录对子的牌
	std::vector<int>	three_index;	//记录3张
	std::vector<int>	four_index;		//记录4张
};

//////////////////////////////////////////////////////////////////////////
// 玩家名字列表

char* name_list[] = 
{
	"叶文洁",
	"汪淼",
	"史强",
	"常伟思",
	"杨冬",
	"丁仪",
	"伊文斯",
	"罗辑",
	"章北海",
	"庄颜",
	"东方延绪",
	"云天明",
	"程心",
	"艾AA",
	"关一帆"
};

//////////////////////////////////////////////////////////////////////////
// 运行时玩家信息存储

struct PlayerInfo
{
	std::string _name;
	int			_score;
};

struct RunTimeData
{
	RunTimeData()
	{
		_isReady = false;
	}

	bool		_isReady;	

	PlayerInfo	_playerinfo1;
	PlayerInfo	_playerinfo2;
	PlayerInfo	_playerinfo3;
};

RunTimeData s_runtimeData;

//
// SceneGame
//

Scene* SceneGame::createScene()
{
    auto scene = Scene::create();
    auto layer = SceneGame::create();
    scene->addChild(layer);

    return scene;
}

bool SceneGame::init()
{
    // 初始化基类----------------------------------------
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// 背景----------------------------------------------
	auto bg = Sprite::create("bg_table.jpg");
	bg->setScale(1/0.8);
	bg->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	this->addChild(bg, 0);

	auto flag = Sprite::create("d_flag.png");
	flag->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y + 100));
	this->addChild(flag, 0);

	// 返回菜单

	auto itemBack = customMenuItem("item_back.png", "item_back.png", CC_CALLBACK_1(SceneGame::menuBackCallback, this));
	itemBack->setPosition(visibleSize.width/2+200, visibleSize.height-50);

	auto menuBack = Menu::create(itemBack, NULL);
	menuBack->setPosition(origin);
	this->addChild(menuBack, 1);
	
	// 准备菜单

	auto itemReady = customMenuItem("item_zhunbei.png", "item_zhunbei.png", CC_CALLBACK_1(SceneGame::menuReadyCallback, this));
	_menuReady = Menu::create(itemReady, NULL);
	this->addChild(_menuReady, 1);

	// 抢地主菜单

	auto itemQiang = customMenuItem("item_qiangdizhu.png", "item_qiangdizhu.png", CC_CALLBACK_1(SceneGame::menuQiangCallback, this));
	itemQiang->setPosition(-80, -50);
	auto itemBuQiang = customMenuItem("item_buqiang.png", "item_buqiang.png", CC_CALLBACK_1(SceneGame::menuBuQiangCallback, this));
	itemBuQiang->setPosition(80, -50);

	_menuQiangDiZhu = Menu::create(itemBuQiang, itemQiang, NULL);
	this->addChild(_menuQiangDiZhu, 1);
	_menuQiangDiZhu->setVisible(false);

    // 出牌菜单

	auto itemBuChu = customMenuItem("item_buchu.png", "item_buchu_d.png", CC_CALLBACK_1(SceneGame::menuBuChuCallback, this));
	itemBuChu->setPosition(-100, -50);
    auto itemChuPai = customMenuItem("item_chupai.png", "item_chupai_d.png", CC_CALLBACK_1(SceneGame::menuChuPaiCallback, this));
	itemChuPai->setPosition(100, -50);

	_menuChuPai = Menu::create();
	_menuChuPai->addChild(itemBuChu,2,0);
	_menuChuPai->addChild(itemChuPai,2,1);
	this->addChild(_menuChuPai, 1);
	_menuChuPai->setVisible(false);

	// 玩家信息读取
	srand(time(0));

	// 如果首次进入此处，创建人物信息
	if (!s_runtimeData._isReady)
	{
		int name_index_1 = rand()%(sizeof(name_list)/sizeof(name_list[0]));
		int name_index_2 = rand()%(sizeof(name_list)/sizeof(name_list[0]));
		int name_index_3 = rand()%(sizeof(name_list)/sizeof(name_list[0]));

		while (name_index_2 == name_index_1)
		{
			name_index_2 = rand()%(sizeof(name_list)/sizeof(name_list[0]));
		}

		while (name_index_2 == name_index_1 || name_index_3 == name_index_1)
		{
			name_index_3 = rand()%(sizeof(name_list)/sizeof(name_list[0]));
		}

		s_runtimeData._playerinfo1._name = name_list[name_index_1];
		s_runtimeData._playerinfo1._score = 5000;
		s_runtimeData._playerinfo2._name = name_list[name_index_2];
		s_runtimeData._playerinfo2._score = 5000;
		s_runtimeData._playerinfo3._name = name_list[name_index_3];
		s_runtimeData._playerinfo3._score = 5000;

		s_runtimeData._isReady = true;
	}

	// 玩家

	_player1 = Player::create(s_runtimeData._playerinfo1._name, s_runtimeData._playerinfo1._score, true);
	_player1->setPosition(70, 300);
	_player1->SetPlayerID(1);
	this->addChild(_player1);

	_player2 = Player::create(s_runtimeData._playerinfo2._name, s_runtimeData._playerinfo2._score, false);
	_player2->setPosition(1000, 600);
	_player2->SetPlayerID(2);
	this->addChild(_player2);

	_player3 = Player::create(s_runtimeData._playerinfo3._name, s_runtimeData._playerinfo3._score, false);
	_player3->setPosition(70, 600);
	_player3->SetPlayerID(3);
	this->addChild(_player3);

	// 底牌区
	_bottomCardZone = BottomCardZone::create();
	_bottomCardZone->setPosition(600, 610);
	this->addChild(_bottomCardZone, 1);

	// 初始化牌堆
	initCards();
    
	_arrPlayerOut = CCArray::create();
	_arrPlayerOut->retain();

	_state = 1;
	_gameover = false;
	_begin = false;

	//jiesuan(1);

    return true;
}

void SceneGame::initCards()
{
	for (int i=0; i<13; i++)
	{
		for (int j=0; j<4; j++)
		{
			PokeInfo info;
			info._num = (PokeNum)i;
			info._tag = (PokeTag)j;
			_pokeInfo.push_back(info);
		}
	}

	PokeInfo info;
	info._num = (PokeNum)13;
	info._tag = (PokeTag)0;
	_pokeInfo.push_back(info);
	info._num = (PokeNum)14;
	_pokeInfo.push_back(info);
}

static int index_fapai = 0; //发牌时的索引

void SceneGame::faPai()
{
	// 洗牌
	index_fapai = 0;

	srand(time(0));
	std::random_shuffle(_pokeInfo.begin(), _pokeInfo.end());

	// 发牌
	auto delay = DelayTime::create(0.04);
	auto callback = CallFuncN::create(CC_CALLBACK_1(SceneGame::callbackFaPai,this));
	auto seq = Sequence::createWithTwoActions(delay, callback);
	this->runAction(seq);

	// 自动判断牌型
	schedule(schedule_selector(SceneGame::OutCard),0.1);
}

void SceneGame::callbackFaPai(cocos2d::Node* node)
{
	if (index_fapai < 51)
	{
		if (index_fapai%3==0)
		{
			// 角色1
			_player1->FaPai(this, _pokeInfo.at(index_fapai));
		} else if (index_fapai%3==1)
		{
			// 角色2
			_player2->FaPai(this, _pokeInfo.at(index_fapai));
		} else if (index_fapai%3==2)
		{
			// 角色3
			_player3->FaPai(this, _pokeInfo.at(index_fapai));
		}

		index_fapai++;
	}

	if (index_fapai<51)
	{
		auto delay = DelayTime::create(0.04);
		auto callback = CallFuncN::create(CC_CALLBACK_1(SceneGame::callbackFaPai,this));
		auto seq = Sequence::createWithTwoActions(delay, callback);
		this->runAction(seq);
	}
	else
	{
		_menuQiangDiZhu->setVisible(true);
	}
}

void SceneGame::FaDiPai(Player* player)
{
	for (int i=index_fapai; i<54; i++)
	{
		//底牌
		_bottomCardZone->Show(_pokeInfo.at(i));
		_vecDiPai.push_back(_pokeInfo.at(i));
	}

	player->setDiZhu();
	for (int i=0; i<_vecDiPai.size(); i++)
	{
		player->FaPai(this, _vecDiPai[i]);
	}
}

void SceneGame::menuBackCallback(Ref* pSender)
{
	Director::getInstance()->replaceScene(SceneMenu::createScene());
}

void SceneGame::menuReadyCallback(Ref* pSender)
{
	faPai();

	_menuReady->setVisible(false);
}

void SceneGame::menuRestartCallback(Ref* pSender)
{
	Director::getInstance()->replaceScene(SceneGame::createScene());
}

void SceneGame::menuQiangCallback(Ref* pSender)
{
	// 分发底牌
	FaDiPai(_player1);

	// 切换菜单可见
	_menuQiangDiZhu->setVisible(false);
	_menuChuPai->setVisible(true);
}

void SceneGame::menuBuQiangCallback(Ref* pSender)
{
	// 切换菜单可见
	_menuQiangDiZhu->setVisible(false);
	//_menuChuPai->setVisible(true);

	// 分发底牌
	
	// 如果玩家不抢地主，则由下家随机
	if (_player2->IsQiangDiZhu())
	{
		FaDiPai(_player2);

		if (!_gameover)
		{
			auto delay = DelayTime::create(3);
			auto callback = CallFuncN::create(CC_CALLBACK_1(SceneGame::callbackChuPai2,this));
			auto seq = Sequence::createWithTwoActions(delay, callback);
			this->runAction(seq);
		}
	}
	else
	{
		FaDiPai(_player3);

		if (!_gameover)
		{
			auto delay = DelayTime::create(3);
			auto callback = CallFuncN::create(CC_CALLBACK_1(SceneGame::callbackChuPai3,this));
			auto seq = Sequence::createWithTwoActions(delay, callback);
			this->runAction(seq);
		}
	}

	if (!_begin)
	{
		_begin = true;
	}
}

void SceneGame::menuBuChuCallback(Ref* pSender)
{
	_player3->clearCards();
	_player1->BuChu();

	auto delay = DelayTime::create(1);
	auto callback = CallFuncN::create(CC_CALLBACK_1(SceneGame::callbackChuPai2,this));
	auto seq = Sequence::createWithTwoActions(delay, callback);
	this->runAction(seq);

	_menuChuPai->setVisible(false);
}

void SceneGame::menuChuPaiCallback(Ref* pSender)
{
	if (!_begin)
	{
		_begin = true;
	}

	// 出牌之前，判断上家的牌型
	auto player3_outcards = _player3->GetOutCards();
	CARDS_DATA player3_card_data = PanDuanPaiXing(player3_outcards);

	if (player3_outcards.empty())
	{
		auto player2_outcards = _player2->GetOutCards();
		CARDS_DATA player2_card_data = PanDuanPaiXing(player2_outcards);
		if (player2_outcards.empty())
		{
			_player1->ChuPai(this, false, ERROR_CARD, 0, 1);// 出牌
		}
		else
		{
			_player1->ChuPai(this, true, player2_card_data._type, player2_outcards.size(), player2_card_data._value);// 跟牌
		}
	}
	else
	{
		_player1->ChuPai(this, true, player3_card_data._type, player3_outcards.size(), player3_card_data._value);// 跟牌
	}

	// 清空牌型判断区
	_arrPlayerOut->removeAllObjects();

	if (!_gameover)
	{
		auto delay = DelayTime::create(1);
		auto callback = CallFuncN::create(CC_CALLBACK_1(SceneGame::callbackChuPai2,this));
		auto seq = Sequence::createWithTwoActions(delay, callback);
		this->runAction(seq);
	}

	_menuChuPai->setVisible(false);
}

void SceneGame::callbackChuPai2(cocos2d::Node* node)
{
	// 出牌之前，判断上家的牌型
	auto player1_outcards = _player1->GetOutCards();
	CARDS_DATA player1_card_data = PanDuanPaiXing(player1_outcards);

	if (player1_outcards.empty())
	{
		auto player3_outcards = _player3->GetOutCards();
		CARDS_DATA player3_card_data = PanDuanPaiXing(player3_outcards);
		if (player3_outcards.empty())
		{
			_player2->ChuPai(this, false, ERROR_CARD, 0, 1);// 出牌
		}
		else
		{
			_player2->ChuPai(this, true, player3_card_data._type, player3_outcards.size(), player3_card_data._value);// 跟牌
		}
	}
	else
	{
		_player2->ChuPai(this, true, player1_card_data._type, player1_outcards.size(), player1_card_data._value);// 跟牌
	}

	if (!_gameover)
	{
		auto delay = DelayTime::create(1);
		auto callback = CallFuncN::create(CC_CALLBACK_1(SceneGame::callbackChuPai3,this));
		auto seq = Sequence::createWithTwoActions(delay, callback);
		this->runAction(seq);
	}
}

void SceneGame::callbackChuPai3(cocos2d::Node* node)
{
	// 出牌之前，判断上家的牌型
	auto player2_outcards = _player2->GetOutCards();
	CARDS_DATA player2_card_data = PanDuanPaiXing(player2_outcards);

	if (player2_outcards.empty())
	{
		auto player1_outcards = _player1->GetOutCards();
		CARDS_DATA player1_card_data = PanDuanPaiXing(player1_outcards);
		if (player1_outcards.empty())
		{
			_player3->ChuPai(this, false, ERROR_CARD, 0, 1);// 出牌
		}
		else
		{
			_player3->ChuPai(this, true, player1_card_data._type, player1_outcards.size(), player1_card_data._value);// 跟牌
		}
	}
	else
	{
		_player3->ChuPai(this, true, player2_card_data._type, player2_outcards.size(), player2_card_data._value);// 跟牌
	}

	if (!_gameover)
	{
		_menuChuPai->setVisible(true);
	}
}

void SceneGame::update(float delta)
{

}

void SceneGame::setChuPaiMenuEnabled(bool isChuPai, bool isBuChu)
{
	((MenuItemFont *)_menuChuPai->getChildByTag(1))->setEnabled(isChuPai);//出牌
	((MenuItemFont *)_menuChuPai->getChildByTag(0))->setEnabled(isBuChu);//不出
}

void SceneGame::OutCard(float delta)
{
	if (_state == 1)
	{
		// 开局
		if (!_begin)
		{
			CARDS_DATA card_data = PanDuanPaiXing(_arrPlayerOut);
			setChuPaiMenuEnabled(card_data._type != ERROR_CARD, false);
		}
		// 已经开局
		else
		{
			CARDS_DATA card_data = PanDuanPaiXing(_arrPlayerOut);

			auto player3_outcards = _player3->GetOutCards();
			CARDS_DATA player3_card_data = PanDuanPaiXing(player3_outcards);

			if (player3_outcards.empty())
			{
				auto player2_outcards = _player2->GetOutCards();
				CARDS_DATA player2_card_data = PanDuanPaiXing(player2_outcards);
				if (player2_outcards.empty())
				{
					setChuPaiMenuEnabled(card_data._type != ERROR_CARD, false);
				}
				else
				{
					// 提示部分未完善
					//_player1->ShowTipInfo(true, player2_card_data._type, player2_card_data._cards.size(), player2_card_data._value);

					if (
						// 相同牌型
						(card_data._type == player2_card_data._type && card_data._value > player2_card_data._value) ||
						// 不同牌型用炸弹或火箭
						(card_data._type != player2_card_data._type && player2_card_data._type != MISSILE_CARD && 
						(card_data._type == BOMB_CARD || card_data._type == MISSILE_CARD) && card_data._value > player2_card_data._value))
					{
						setChuPaiMenuEnabled(true, true);
					}
					else
					{
						setChuPaiMenuEnabled(false, true);
					}
				}
			}
			else
			{
				//_player1->ShowTipInfo(true, player3_card_data._type, player3_card_data._cards.size(), player3_card_data._value);

				if (
					// 相同牌型
					(card_data._type == player3_card_data._type && card_data._value > player3_card_data._value) ||
					// 不同牌型用炸弹或火箭
					(card_data._type != player3_card_data._type && player3_card_data._type != MISSILE_CARD && 
					(card_data._type == BOMB_CARD || card_data._type == MISSILE_CARD) && card_data._value > player3_card_data._value))
				{
					setChuPaiMenuEnabled(true, true);
				}
				else
				{
					setChuPaiMenuEnabled(false, true);
				}
			}
		}
	}
}

void SceneGame::gameover(int winID)
{
	_gameover = true;

	unschedule(schedule_selector(SceneGame::OutCard));

	//_menuRestart->setVisible(true);
	_menuChuPai->setVisible(false);
	_menuQiangDiZhu->setVisible(false);
	_menuReady->setVisible(false);

	jiesuan(winID);
}

int player1_delta_score = 0;
int player2_delta_score = 0;
int player3_delta_score = 0;
bool isPlayerWin = false;

void SceneGame::jiesuan(int winID)
{
	// 结算分数

	int base_score = 100;
	player1_delta_score = 0;
	player2_delta_score = 0;
	player3_delta_score = 0;

	if (winID == 1)
	{
		isPlayerWin = true;

		if (_player1->getIsDiZhu())
		{
			player1_delta_score = base_score * 2;
			player2_delta_score = base_score * -1;
			player3_delta_score = base_score * -1;
		}
		else if (_player2->getIsDiZhu())
		{
			player1_delta_score = base_score * 1;
			player2_delta_score = base_score * -1 * 2;
			player3_delta_score = base_score * 1;
		}
		else
		{
			player1_delta_score = base_score * 1;
			player2_delta_score = base_score * 1;
			player3_delta_score = base_score * -1 * 2;
		}
	}
	else if (winID == 2)
	{
		isPlayerWin = false;

		if (_player1->getIsDiZhu())
		{
			player1_delta_score = base_score * -1 * 2;
			player2_delta_score = base_score * 1;
			player3_delta_score = base_score * 1;
		}
		else if (_player2->getIsDiZhu())
		{
			player1_delta_score = base_score * -1;
			player2_delta_score = base_score * 2;
			player3_delta_score = base_score * -1;
		}
		else
		{
			player1_delta_score = base_score * 1;
			player2_delta_score = base_score * 1;
			player3_delta_score = base_score * -1 * 2;
		}
	}
	else if (winID == 3)
	{
		isPlayerWin = false;

		if (_player1->getIsDiZhu())
		{
			player1_delta_score = base_score * -1 * 2;
			player2_delta_score = base_score * 1;
			player3_delta_score = base_score * 1;
		}
		else if (_player2->getIsDiZhu())
		{
			player1_delta_score = base_score * 1;
			player2_delta_score = base_score * -1 * 2;
			player3_delta_score = base_score * 1;
		}
		else
		{
			player1_delta_score = base_score * -1;
			player2_delta_score = base_score * -1;
			player3_delta_score = base_score * 2;
		}
	}

	// 存储分数
	_player1->Score(player1_delta_score);
	_player2->Score(player2_delta_score);
	_player3->Score(player3_delta_score);

	s_runtimeData._playerinfo1._score = _player1->GetScore();
	s_runtimeData._playerinfo2._score = _player2->GetScore();
	s_runtimeData._playerinfo3._score = _player3->GetScore();

	// 结算界面
	capture(this);

// 	// 结算页面
// 	
// 	Size visibleSize = Director::getInstance()->getVisibleSize();
// 	Vec2 origin = Director::getInstance()->getVisibleOrigin();
// 
// 	auto jiesuanBg = Sprite::create("gameover/bg_small.png");
// 	jiesuanBg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
// 	this->addChild(jiesuanBg, 100);
// 
// 	auto tiaofu = Sprite::create("gameover/gameresult_win_tiaofu.png");
// 	tiaofu->setPosition(Vec2(350, 400));
// 	jiesuanBg->addChild(tiaofu, 0);
// 
// 	auto shengli = Sprite::createWithSpriteFrameName("ddz_result_base_word_win.png");
// 	shengli->setPosition(Vec2(350, 400));
// 	jiesuanBg->addChild(shengli, 0);
// 
// 	auto player = Sprite::create("owner1_lose.png");
// 	player->setPosition(Vec2(100, 200));
// 	jiesuanBg->addChild(player, 0);
}

//
//屏幕截图
void SceneGame::capture(Ref* sender)
{
	CCLOG("ScreenShot");
	utils::captureScreen(CC_CALLBACK_2(SceneGame::afterCapture, this), "ScreenShot.png");
}

//截图后执行afterCapture
void SceneGame::afterCapture(bool succeed, const std::string& outputFile)
{
	if (succeed)
	{
		CCLOG("%s", outputFile.c_str());
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		//显示截图
		Sprite* sp = Sprite::create(outputFile);
		SpriteBlur* spriteBlur = SpriteBlur::create(sp, 20.0f, 20.0f);		
		spriteBlur->setPosition(visibleSize / 2);
		this->addChild(spriteBlur,1000);

		// 结算页面

		auto jiesuanBg = Sprite::create("gameover/bg_small.png");
		jiesuanBg->setPosition(Vec2(visibleSize.width * 2 / 3 + origin.x, visibleSize.height / 2 + origin.y));
		this->addChild(jiesuanBg, 1001);

		auto tiaofu = Sprite::create(isPlayerWin ? "gameover/gameresult_win_tiaofu.png" : "gameover/gameresult_lose_tiaofu.png");
		tiaofu->setPosition(Vec2(350, 400));
		jiesuanBg->addChild(tiaofu, 0);

		auto shengli = Sprite::createWithSpriteFrameName(isPlayerWin ? "ddz_result_base_word_win.png" : "ddz_result_base_word_lose.png");
		shengli->setPosition(Vec2(350, 430));
		jiesuanBg->addChild(shengli, 0);

		auto player = Sprite::create(
			isPlayerWin ? 
			(_player1->getIsDiZhu() ? "gameover/owner1_win.png" : "gameover/farmer1_win.png") 
			: 
			(_player1->getIsDiZhu() ? "gameover/owner1_lose.png" : "gameover/farmer1_lose.png"));
		player->setPosition(Vec2(-200, 200));
		jiesuanBg->addChild(player, 0);

		// 结算数据

		// 昵称

		auto label_name = Label::createWithTTF(a2u("昵称"), "fonts/FZCuYuan-M03S.ttf", 24);
		label_name->setColor(Color3B(255, 0, 0));
		label_name->setPosition(Vec2(150,300));
		jiesuanBg->addChild(label_name,1);

		// 地主标志
		auto dizhu_flag = Sprite::createWithSpriteFrameName("ddz_result_base_dizhu_flag.png");
		dizhu_flag->setPosition(Vec2(90, _player1->getIsDiZhu() ? 250 : (_player2->getIsDiZhu() ? 200 : (_player3->getIsDiZhu() ? 150 : 0))));
		jiesuanBg->addChild(dizhu_flag, 0);

		auto label_name_player1 = Label::createWithTTF(a2u(_player1->GetName().c_str()), "fonts/FZCuYuan-M03S.ttf", 24);
		label_name_player1->setColor(Color3B(255, 0, 0));
		label_name_player1->setPosition(Vec2(150,250));
		jiesuanBg->addChild(label_name_player1,1);

		auto label_name_player2 = Label::createWithTTF(a2u(_player2->GetName().c_str()), "fonts/FZCuYuan-M03S.ttf", 24);
		label_name_player2->setColor(Color3B(255, 0, 0));
		label_name_player2->setPosition(Vec2(150,200));
		jiesuanBg->addChild(label_name_player2,1);

		auto label_name_player3 = Label::createWithTTF(a2u(_player3->GetName().c_str()), "fonts/FZCuYuan-M03S.ttf", 24);
		label_name_player3->setColor(Color3B(255, 0, 0));
		label_name_player3->setPosition(Vec2(150,150));
		jiesuanBg->addChild(label_name_player3,1);

		// 欢乐豆

		auto label_huanledou = Label::createWithTTF(a2u("欢乐豆"), "fonts/FZCuYuan-M03S.ttf", 24);
		label_huanledou->setColor(Color3B(255, 0, 0));
		label_huanledou->setPosition(Vec2(350,300));
		jiesuanBg->addChild(label_huanledou,1);

		auto label_huanledou_player1 = Label::createWithTTF(a2u(_player1->GetScoreString().c_str()), "fonts/FZCuYuan-M03S.ttf", 24);
		label_huanledou_player1->setColor(Color3B(255, 0, 0));
		label_huanledou_player1->setPosition(Vec2(350,250));
		jiesuanBg->addChild(label_huanledou_player1,1);

		auto label_huanledou_player2 = Label::createWithTTF(a2u(_player2->GetScoreString().c_str()), "fonts/FZCuYuan-M03S.ttf", 24);
		label_huanledou_player2->setColor(Color3B(255, 0, 0));
		label_huanledou_player2->setPosition(Vec2(350,200));
		jiesuanBg->addChild(label_huanledou_player2,1);

		auto label_huanledou_player3 = Label::createWithTTF(a2u(_player3->GetScoreString().c_str()), "fonts/FZCuYuan-M03S.ttf", 24);
		label_huanledou_player3->setColor(Color3B(255, 0, 0));
		label_huanledou_player3->setPosition(Vec2(350,150));
		jiesuanBg->addChild(label_huanledou_player3,1);

		// 分数加减
		char str_score_delta_player1[255] = {0};
		char str_score_delta_player2[255] = {0};
		char str_score_delta_player3[255] = {0};
		
		sprintf_s(str_score_delta_player1, "%+d", player1_delta_score);
		sprintf_s(str_score_delta_player2, "%+d", player2_delta_score);
		sprintf_s(str_score_delta_player3, "%+d", player3_delta_score);

		auto label_huanledou_d_player1 = Label::createWithTTF(a2u(str_score_delta_player1), "fonts/FZCuYuan-M03S.ttf", 24);
		label_huanledou_d_player1->setColor(Color3B(255, 0, 0));
		label_huanledou_d_player1->setPosition(Vec2(480,250));
		jiesuanBg->addChild(label_huanledou_d_player1,1);

		auto label_huanledou_d_player2 = Label::createWithTTF(a2u(str_score_delta_player2), "fonts/FZCuYuan-M03S.ttf", 24);
		label_huanledou_d_player2->setColor(Color3B(255, 0, 0));
		label_huanledou_d_player2->setPosition(Vec2(480,200));
		jiesuanBg->addChild(label_huanledou_d_player2,1);

		auto label_huanledou_d_player3 = Label::createWithTTF(a2u(str_score_delta_player3), "fonts/FZCuYuan-M03S.ttf", 24);
		label_huanledou_d_player3->setColor(Color3B(255, 0, 0));
		label_huanledou_d_player3->setPosition(Vec2(480,150));
		jiesuanBg->addChild(label_huanledou_d_player3,1);

		// 重开菜单

		auto itemRestart = customMenuItemWithSpriteFrameName("ddzsingle_maplvl_btn_restart.png", "ddzsingle_maplvl_btn_restart.png", CC_CALLBACK_1(SceneGame::menuRestartCallback, this));
		itemRestart->setPosition(200, -150);
		auto menuRestart = Menu::create(itemRestart, NULL);
		this->addChild(menuRestart, 1002);
	}
	else
	{
		CCLOG("Capture screen failed.");
	}
}

CARDS_DATA SceneGame::PanDuanPaiXing(CCArray* cards)
{
	std::vector<int> vec;
	for (int i=0; i<cards->count(); i++)
	{
		Poke* poke = dynamic_cast<Poke*>(cards->objectAtIndex(i));
		if (poke != NULL)
		{
			vec.push_back(poke->_info._num);
		}
	}
	
	return PanDuanPaiXing(vec);
}

CARDS_DATA SceneGame::PanDuanPaiXing(std::vector<PokeInfo>& cards)
{
	std::vector<int> vec;
	for (int i=0; i<cards.size(); i++)
	{
		vec.push_back(cards[i]._num);
	}

	return PanDuanPaiXing(vec);
}

CARDS_DATA SceneGame::PanDuanPaiXing(std::vector<int>& cards)
{
	CARDS_DATA ret;
	unsigned int length = cards.size();
	std::sort(cards.begin(), cards.end());
	
	for (int i = 0; i < length; i++)
	{
		ret._cards.push_back(cards[i]);
	}

	//小于5张牌
	//单牌，对子，3不带,炸弹通用算法
	if(length > 0 && length < 5) 
	{
		// 单牌/对子/三不带/炸弹
		if(cards[0] == cards[length - 1])
		{
			ret._type = (CARD_TYPE)length;
			if (length == 4)
			{
				ret._value = 100 + cards[0];
			}
			else
			{
				ret._value = (length - 1)*20 + cards[0];
			}
			return ret;
		}

		// 火箭
		if (cards[0] == NUM_XW && cards[1] == NUM_DW && length == 2)
		{
			ret._type = MISSILE_CARD;
			ret._value = 120;
			return ret;
		}

		// 三带一
		if ((cards[0] == cards[length - 2] && length == 4)
			|| (cards[1] == cards[length - 1] && length == 4))
		{
			ret._type = THREE_ONE_CARD;
			if ((cards[0] == cards[length - 2] && length == 4))
			{
				ret._value = 40 + cards[0];
			}
			else
			{
				ret._value = 40 + cards[1];
			}
			
			return ret;
		}
	}

	// 大于等于5张牌
	else if (length >= 5)
	{
		// 连牌
		if (IsContinuous(cards) && IsLessTwo(cards))
		{
			ret._type = CONNECT_CARD;
			ret._value = 60 + cards[0];
			return ret;
		}

		// 连对
		if (length >= 6 && length % 2 == 0) // 判断是否大于六张，且为双数
		{
			// 判断是否都是对子
			bool is_all_double = true;

			for (int i=0; i<length; i+=2)
			{
				if (cards[i] != cards[i+1])
				{
					is_all_double = false;
					break;
				}
			}

			// 判断对子是否连续
			if (is_all_double)
			{
				std::vector<int> vec_single;
				for (int i=0; i<length; i+=2)
				{
					vec_single.push_back(cards[i]);
				}

				if (IsContinuous(vec_single))
				{
					ret._type = COMPANY_CARD;
					ret._value = 80 + cards[0];
					return ret;
				}
			}
		}

		// 将数组中的牌数分到结构体CRAD_INDEX中
		CRAD_INDEX card_index;

		for (int i=0; i<length; )
		{
			if (i+1 < length && cards[i] == cards[i+1])
			{
				if (i+2 < length && cards[i+1] == cards[i+2])
				{
					if (i+3 < length && cards[i+2] == cards[i+3])
					{
						card_index.four_index.push_back(cards[i]);
						i += 4;
					}
					else
					{
						card_index.three_index.push_back(cards[i]);
						i += 3;
					}
				}
				else
				{
					card_index.double_index.push_back(cards[i]);
					i += 2;
				}
			}
			else
			{
				card_index.single_index.push_back(cards[i]);
				i++;
			}
		}

		// 3带对
		if (card_index.three_index.size() == 1 && card_index.double_index.size() == 1 && card_index.four_index.empty() && card_index.single_index.empty())
		{
			ret._type = THREE_TWO_CARD;
			ret._value = 40 + card_index.three_index[0];
			return ret;
		}

		// 飞机
		// 前提：两个连续三张的
		if (card_index.four_index.empty() && card_index.three_index.size() == 2 && card_index.three_index[0] + 1 == card_index.three_index[1])
		{
			// 333444
			if (card_index.single_index.empty() && card_index.double_index.empty())
			{
				ret._type = AIRCRAFT_CARD;
				ret._value = 80 + cards[0];
				return ret;
			}

			// 33344456
			if (card_index.double_index.empty() && card_index.single_index.size() == 2)
			{
				ret._type = AIRCRAFT_SINGLE_CARD;
				ret._value = 80 + cards[0];
				return ret;
			}

			// 33344455
			if (card_index.single_index.empty() && card_index.double_index.size() == 1)
			{
				ret._type = AIRCRAFT_SINGLE_CARD;
				ret._value = 80 + cards[0];
				return ret;
			}

			// 3334445566
			if (card_index.single_index.empty() && card_index.double_index.size() == 2)
			{
				ret._type = AIRCRAFT_DOUBLE_CARD;
				ret._value = 80 + cards[0];
				return ret;
			}
		}

		// 4带2
		if (card_index.four_index.size() == 1 && length % 2 == 0 && card_index.three_index.empty())
		{

			// 444423
			if (card_index.single_index.size() == 2 && card_index.double_index.empty())
			{
				ret._type = BOMB_TWO_CARD;
				ret._value = 100 + cards[0];
				return ret;
			}

			// 444422
			if (card_index.double_index.size() == 1 && card_index.single_index.empty())
			{
				ret._type = BOMB_TWO_CARD;
				ret._value = 80 + cards[0];
				return ret;
			}

			// 44442233
			if (card_index.double_index.size() == 2 && card_index.single_index.empty())
			{
				ret._type = BOMB_TWOOO_CARD;
				ret._value = 80 + cards[0];
				return ret;
			}
		}
	}

	ret._type = ERROR_CARD;
	ret._value = 0;
	return ret;
}