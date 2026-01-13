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

	struct BackGround {
		Vector2 skyOriginalPos;
		Vector2 skyPos;
	};

public:

	GameScene gameScene = TITLE;
	Phase phase = CHARGE;
	Direction direction = LEFT;

	BackGround backGround[150];

	int leftChargeAmount = 0;
	int rightChargeAmount = 0;
	int chargeTime = 600;

	int tiltDegree = 0;

	float scrollY = 0.0f;
	int isScroll = false;
	int isTouchCheckpoint = false;

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

