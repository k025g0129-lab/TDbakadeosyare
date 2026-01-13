#pragma once
#include <Windows.h> 
#include <Xinput.h> 
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

	enum Direction {
		RIGHT,
		LEFT,
	};

public:

	GameScene gameScene = TITLE;
	Phase phase = CHARGE;
	Direction direction = LEFT;

	int leftChargeAmount = 0;
	int rightChargeAmount = 0;
	int chargeTime = 600;

	int tiltDegree = 0;

	int isScroll = false;




	int whiteTextureHandle = 0;

public:

	Scene();
	void Initialize();

	void Update();

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

	// 入力
	bool IsPressB() const;
	bool IsTriggerB() const;

private:
	XINPUT_STATE padState{};
	XINPUT_STATE prevPadState{};

};

