#pragma once
#include <Windows.h> 
#include <Xinput.h> 
#include "Vector2.h"
#include "Player.h"

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
		LANDING,
	};

	enum Direction {
		RIGHT,
		LEFT,
	};

	struct BackGround {
		Vector2 skyOriginalPos;
		Vector2 skyPos;
	};

	struct CheckPoint {
		float checkPointY; // 設置する高さ
		int isPreparingForLanding; // 通過したかどうか?
		int lv;
		float distance;
		int isPassed; // 通過したかどうか

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

	CheckPoint checkPoint;


	int whiteTextureHandle = 0;

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
	void LandingUpdate();
	void LandingDraw();

	// 入力
	bool IsPressB() const;
	bool IsTriggerB() const;

private:
	XINPUT_STATE padState{};
	XINPUT_STATE prevPadState{};

	Player* player;

};

