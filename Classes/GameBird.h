#ifndef GAMEBIRD_H__
#define GAMEBIRD_H__

#include "cocos2d.h"

class GameBird : public cocos2d::Sprite
{
private:

public:
	enum gameState {
		UP,
		DOWN,
	};

	CC_SYNTHESIZE(int, _state, State);

	static GameBird* spriteWithFile(const char* pszfileName);

	GameBird(void);
	~GameBird(void);
};

#endif
