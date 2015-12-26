#include "GameBird.h"

USING_NS_CC;

GameBird::~GameBird()
{
}

GameBird::GameBird()
: _state(GameBird::DOWN)
{
}

GameBird* GameBird::spriteWithFile(const char* pszfileName)
{
	GameBird* sprite = new GameBird();

	if (sprite && sprite->initWithFile(pszfileName))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return NULL;
}