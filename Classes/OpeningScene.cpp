#include "OpeningScene.h"
#include "HelloWorldScene.h"

USING_NS_CC;

Scene* Opening::scene()
{
	Scene *scene = Scene::create();
	Opening *layer = Opening::create();

	scene->addChild(layer);

	return scene;
}

bool Opening::init()
{
	if (!Layer::init()) return false;

	creatGameScene();
	
	return true;
}

void Opening::creatGameScene(void)
{
	MenuItemFont::setFontSize(60);

	_screenSize = Director::sharedDirector()->getWinSize();

	Sprite* openingImage = Sprite::create("background.png");
	openingImage->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
	this->addChild(openingImage);

	MenuItem* item_1 = MenuItemFont::create("Play", CC_CALLBACK_1(Opening::menuCallBack, this));
	Menu* menu = Menu::create(item_1, NULL);
	this->addChild(menu);
}

void Opening::changeScene(void)
{
	Director::getInstance()->getEventDispatcher()->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);

	auto hScene = HelloWorld::createScene();
	auto pScene = TransitionFade::create(0.5f, hScene);
	Director::getInstance()->replaceScene(pScene);
}

void Opening::menuCallBack(Ref* pSendeR)
{
	changeScene();
}