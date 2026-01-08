#pragma once
#include "Vector2.h"

class Scene {
public:
	enum GameScene {
		TITLE,
		MAIN_GAME,
		RESULT,
	};

	enum Phase {
		CHARGE,
		RISE,
	};

public:
	void Update();
	void Drow();

public:


};

