#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "GameBird.h"
#include "GameWall.h"
#include "OpeningScene.h"

class HelloWorld : public cocos2d::Layer
{
private:
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	cocos2d::Vec2 birdSize;
	cocos2d::Vector<GameWall*> gameWalls;
	cocos2d::Label* _timeFont;
	cocos2d::Menu* overMenu;
	cocos2d::Sprite* gameOverSprite;

	float gameOverBool;             // game이 끝나면 true
	float gameTime;                 // running time
	float dropY;                    // 새가 떨어지는 정도
	float grassH;                   // 땅의 높이
	GameBird* bird;

public:
    static cocos2d::Scene* createScene();

    virtual bool init();

	void createGameScene(void);
	GameBird* createGameBird();

	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

	void birdMove(void);
	void gameOver(void);
	void changeScene(cocos2d::Ref* pSender);

	void createWall(int index);
	void checkCollision(int index);

	void dropBird(float delta);
	void moveWall(float delta);
	void countTime(float delta);
    
    // a selector callback
    /*
	void menuCloseCallback(cocos2d::Ref* pSender);
	*/
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
	~HelloWorld(void);
};

#endif // __HELLOWORLD_SCENE_H__
