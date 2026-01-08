#pragma once
#include "Vector2.h"

class Scene {
public:
	enum GameScene {
		TITLE,
		TUTORIAL,
		MAIN_GAME,
		RESULT,
	};

	enum Phase {
		CHARGE,
		RISE,
	};

public:

	GameScene gameScene = TITLE;
	Phase phase = CHARGE;



public:

	Scene();
	void Initialize();

	void Update();
	void Draw();


	void PhaseUpdate();
	void ChargeUpdate();
	void ChargeDrow();
	void RiseUpdate();
	void RiseDrow();



};

