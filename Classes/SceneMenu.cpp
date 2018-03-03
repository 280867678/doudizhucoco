#include "SceneMenu.h"
#include "SceneGame.h"

USING_NS_CC;

Scene* SceneMenu::createScene()
{
    auto scene = Scene::create();
    auto layer = SceneMenu::create();
    scene->addChild(layer);

    return scene;
}

bool SceneMenu::init()
{
    // 初始化基类----------------------------------------
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 背景----------------------------------------------
	auto bg = Sprite::create("beijing.png");
	//bg->setScale(1/0.8);
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(bg, 0);

	auto logo = Sprite::create("game_logo.png");
	logo->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 3 * 2 + origin.y));
	this->addChild(logo, 0);

	// 创建菜单---------------------------------------------

	auto itemBegin = customMenuItem("item_begin.png", "item_begin.png", CC_CALLBACK_1(SceneMenu::menuBeginCallback, this));
	itemBegin->setPosition(Vec2(-200,-200));
	auto itemExit = customMenuItem("item_exit.png", "item_exit.png", CC_CALLBACK_1(SceneMenu::menuExitCallback, this));
	itemExit->setPosition(Vec2(200,-200));

    auto menu = Menu::create(itemBegin, itemExit, NULL);
    this->addChild(menu, 1);
    
    return true;
}


void SceneMenu::menuBeginCallback(Ref* pSender)
{
	Director::getInstance()->replaceScene(SceneGame::createScene());
}

void SceneMenu::menuExitCallback(Ref* pSender)
{
    Director::getInstance()->end();
}