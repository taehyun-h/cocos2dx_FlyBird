#include "GameWall.h"

USING_NS_CC;

GameWall::~GameWall()
{
}

GameWall::GameWall()
{
}

GameWall* GameWall::spriteWithFile(const char* pszfileName)
{
	GameWall* sprite = new GameWall();

	if (sprite && sprite->initWithFile(pszfileName))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return NULL;
}