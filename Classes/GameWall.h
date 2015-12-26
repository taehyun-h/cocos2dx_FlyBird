#ifndef GAMEWALL_H__
#define GAMEWALL_H__

#include "cocos2d.h"

class GameWall : public cocos2d::Sprite
{
private:
	
public:
	enum gameState {
		UP,
		DOWN,
	};

	CC_SYNTHESIZE(float, height, Height);
	CC_SYNTHESIZE(int, _state, State);

	static GameWall* spriteWithFile(const char* pszfileName);

	GameWall(void);
	~GameWall(void);
};

#endif
