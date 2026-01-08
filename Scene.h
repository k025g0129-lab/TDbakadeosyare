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

	// ゲームシーン用
	void TitleUpdate();
	void TitleDraw();

	void TutorialUpdate();
	void TutorialDraw();

	void MainGameUpdate();
	void MainGameDraw();

	void ResultUpdate();
	void ResultDraw();
	
	
	// フェーズ用
	void PhaseUpdate();
	void ChargeUpdate();
	void ChargeDraw();
	void RiseUpdate();
	void RiseDraw();



};

