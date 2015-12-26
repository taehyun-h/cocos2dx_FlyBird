#ifndef OPENINGSCENE_H_
#define OPENINGSCENE_H_

#include "cocos2d.h"
#include "HelloWorldScene.h"

using namespace cocos2d;

class Opening : public cocos2d::Layer {
private:
	Size _screenSize;

public:
	virtual bool init();

	static cocos2d::Scene* scene();

	CREATE_FUNC(Opening);

	void creatGameScene(void);
	void changeScene(void);

	void menuCallBack(Ref* pSender);
};

#endif