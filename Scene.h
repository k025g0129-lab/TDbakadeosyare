#pragma once
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

