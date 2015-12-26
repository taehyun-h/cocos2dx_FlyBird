#include "HelloWorldScene.h"
#define correction 45

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

HelloWorld::~HelloWorld()
{
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    // super init first
    if ( !Layer::init() )
    {
        return false;
    }

	srand(time(NULL));
	gameTime = 0;
	dropY = 0;
	grassH = 0;
	gameOverBool = false;

	gameWalls = Vector<GameWall*>(6);

	this->setTouchEnabled(true);

	EventDispatcher* dispatcher = Director::getInstance()->getEventDispatcher();
	auto positionListener = EventListenerTouchOneByOne::create();

	positionListener->setSwallowTouches(true);
	positionListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(positionListener, this);

	createGameScene();

    return true;
}

void HelloWorld::createGameScene(void)
{
	// get screen size
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	
	// add background srpite
	Sprite* background = Sprite::create("background.png");
	background->setScaleX(visibleSize.width / background->getContentSize().width);
	background->setScaleY(visibleSize.height / background->getContentSize().height);
	background->setPosition(Vec2(origin.x + visibleSize.width / 2,
								 origin.y + visibleSize.height / 2));
	this->addChild(background);

	Sprite* grass = Sprite::create("grass.png");
	grass->setScaleX(visibleSize.width / background->getContentSize().width);
	grass->setPosition(Vec2(origin.x + visibleSize.width / 2,
							origin.y + grass->getContentSize().height / 2));
	grassH = grass->getBoundingBox().size.height;
	this->addChild(grass, 5);

	Sprite* cloud = Sprite::create("cloud.png");
	cloud->setScaleX(visibleSize.width / background->getContentSize().width);
	cloud->setPosition(Vec2(origin.x + visibleSize.width / 2,
							origin.y + cloud->getContentSize().height / 2 + visibleSize.height / 3 * 2));
	this->addChild(cloud, 1);

	// add time
	_timeFont = Label::createWithBMFont("font.fnt", "0", TextHAlignment::CENTER, visibleSize.width * 0.3f);
	_timeFont->setAnchorPoint(Vec2(1, 0.5f));
	_timeFont->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height*0.8f));
	_timeFont->setScale(1.5f);
	this->addChild(_timeFont, 5);

	// add restart menu
	MenuItemImage *restartItem = MenuItemImage::create("start.png", "start.png", CC_CALLBACK_1(HelloWorld::changeScene, this));

	overMenu = Menu::create(restartItem, NULL);
	overMenu->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.3f));
	overMenu->setAnchorPoint(Vec2(0.5f, 0.5f));
	overMenu->setVisible(false);
	this->addChild(overMenu, 5);

	// add gameover spirte
	gameOverSprite = Sprite::create("gameover.png");
	gameOverSprite->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.6f));
	gameOverSprite->setVisible(false);
	this->addChild(gameOverSprite, 5);

	// create bird object
	bird = createGameBird();

	// bird가 떨어지는 schedule 구현
	schedule(schedule_selector(HelloWorld::dropBird), 0.3f);

	// wall 움직이는 schedule 구현
	schedule(schedule_selector(HelloWorld::moveWall), 1 / 60.0f);

	// time schedule 구현
	schedule(schedule_selector(HelloWorld::countTime), 1.0f);
}

GameBird* HelloWorld::createGameBird()
{
	String* name;
	GameBird* gameBird;

	name = String::createWithFormat("bird.png");
	gameBird = GameBird::spriteWithFile(name->getCString());
	gameBird->setVisible(true);
	gameBird->setScale(1);
	gameBird->setPosition(Vec2(origin.x + visibleSize.width / 7 * 2,
							   origin.y + visibleSize.height / 2));
	gameBird->setAnchorPoint(Vec2(0.5f, 0.5f));
	gameBird->setState(GameBird::DOWN);
	birdSize = gameBird->getContentSize();

	this->addChild(gameBird, 4);

	return gameBird;
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
	auto target = event->getCurrentTarget();
	Point location = target->convertToNodeSpace(touch->getLocation());
	
	if (!gameOverBool) birdMove();
	return true;
}

void HelloWorld::birdMove()
{
	dropY = 0;
	bird->setState(GameBird::UP);
	bird->setRotation(-45);

	bird->setPositionY(bird->getPositionY() + bird->getContentSize().height * 1.5f);
}

void HelloWorld::dropBird(float delta)
{
	Vec2 birdPos = bird->getPosition();

	// bird 방향 조절
	if (bird->getState() == GameBird::UP) bird->setRotation(0);
	else bird->setRotation(45);

	bird->setState(GameBird::DOWN);

	// 바닥에 부딪혀서 game over
	if (birdPos.y - birdSize.y / 2 <= grassH) gameOver();
	else
	{
		// 맵 상단을 벗어나는 경우
		if (birdPos.y + birdSize.y / 2 >= visibleSize.height)
		{
			bird->setPositionY(visibleSize.height - birdSize.y / 2);
		}
		else
		{
			dropY += (birdSize.y * 0.7f);

			// 땅보다 더 밑으로 떨어지는 경우
			if (birdPos.y - dropY <= grassH) dropY = birdPos.y - grassH;
			bird->setPositionY(birdPos.y - dropY);
		}
	}
}

void HelloWorld::moveWall(float delta)
{
	float speed;
	GameWall* tmpWall;
	int count = gameWalls.size();

	// 아직 wall이 없을 경우
	if (count == 0 && gameTime >= 1.0f) createWall(-1);
	else if (count == 2 && gameWalls.at(0)->getPositionX() <= visibleSize.width * 0.2f) createWall(-1);
	else
	{
		speed = visibleSize.width / 120.0f;
		for (int i = 0; i < count; i++)
		{
			// 벽이 맵을 벗어났는지 확인
			tmpWall = gameWalls.at(i);
			if (tmpWall->getPositionX() + tmpWall->getContentSize().width / 2 <= (visibleSize.width * -0.6f)) createWall(i);
			
			tmpWall = gameWalls.at(i);
			tmpWall->setPositionX(tmpWall->getPositionX() - speed);
			checkCollision(i);
		}
	}
}

void HelloWorld::createWall(int index)
{
	float height;
	String *name;
	GameWall *tmpWall, *tmpWall2;

	if (index > -1) tmpWall = gameWalls.at(index);
	if (index == -1 || tmpWall->getState() == GameWall::UP)
	{
		height = rand() % (int)(visibleSize.height / 2) + (int)(visibleSize.height / 5);
		name = String::createWithFormat("upblock.png");

		tmpWall2 = GameWall::spriteWithFile(name->getCString());
		tmpWall2->setHeight(height);
		tmpWall2->setVisible(true);
		tmpWall2->setPosition(Vec2(visibleSize.width + tmpWall2->getContentSize().width / 2, grassH + tmpWall2->getContentSize().height / 2 - (tmpWall2->getContentSize().height - height)));
		tmpWall2->setState(GameWall::UP);
		this->addChild(tmpWall2, 3);

		// 새로운 벽을 추가
		if (index == -1) gameWalls.pushBack(tmpWall2);
	}
	if (index == -1 || tmpWall->getState() == GameWall::DOWN)
	{
		if (index == -1) height = visibleSize.height- grassH - height - (birdSize.y * 4);
		else height = visibleSize.height - grassH - gameWalls.at(index - 1)->getHeight() - (birdSize.y * 4);
		name = String::createWithFormat("downblock.png");

		tmpWall2 = GameWall::spriteWithFile(name->getCString());
		tmpWall2->setHeight(height);
		tmpWall2->setVisible(true);
		tmpWall2->setPosition(Vec2(visibleSize.width + tmpWall2->getContentSize().width / 2, visibleSize.height - (tmpWall2->getContentSize().height / 2) + (tmpWall2->getContentSize().height - height)));
		tmpWall2->setState(GameWall::DOWN);
		this->addChild(tmpWall2, 3);

		// 새로운 벽을 추가
		if (index == -1) gameWalls.pushBack(tmpWall2);
	}

	// 벗어난 벽을 삭제
	if (index > -1)
	{
		gameWalls.replace(index, tmpWall2);
		tmpWall->setVisible(false);
		this->removeChild(tmpWall);
	}
}

void HelloWorld::countTime(float delta)
{
	char timeString[100] = { 0 };

	gameTime = gameTime + 1.0f;

	sprintf(timeString, "%g", gameTime);
	_timeFont->setString(timeString);
}

void HelloWorld::checkCollision(int index)
{
	float birdX = bird->getPosition().x;
	float birdY = bird->getPosition().y;
	GameWall* tmpWall;

	tmpWall = gameWalls.at(index);

	if (tmpWall->getBoundingBox().containsPoint(Vec2(birdX - (birdSize.x - correction), birdY - (birdSize.y - correction)))) gameOver();
	if (tmpWall->getBoundingBox().containsPoint(Vec2(birdX - (birdSize.x - correction), birdY + (birdSize.y - correction)))) gameOver();
	if (tmpWall->getBoundingBox().containsPoint(Vec2(birdX + (birdSize.x - correction), birdY - (birdSize.y - correction)))) gameOver();
	if (tmpWall->getBoundingBox().containsPoint(Vec2(birdX + (birdSize.x - correction), birdY + (birdSize.y - correction)))) gameOver();
}

void HelloWorld::gameOver()
{
	this->unschedule(schedule_selector(HelloWorld::dropBird));
	this->unschedule(schedule_selector(HelloWorld::moveWall));
	this->unschedule(schedule_selector(HelloWorld::countTime));
	this->setTouchEnabled(false);

	gameOverBool = true;

	overMenu->setVisible(true);
	gameOverSprite->setVisible(true);
}

void HelloWorld::changeScene(Ref* pSender)
{
	Scene* pScene = Opening::scene();
	TransitionScene* pTran = TransitionFade::create(0.5f, pScene);
	Director::getInstance()->replaceScene(pTran);
}

/*
void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
*/