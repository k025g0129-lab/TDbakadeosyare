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

	/*Vector2 planeLocalCenterPos = {0.0f,0.0f};
	float width = 100.0f;
	float height = 300.0f;

	Vector2 planeLocalFourCornersPos[4] =
	{
		{planeLocalCenterPos.x - width / 2, planeLocalCenterPos.y - height / 2},
		{planeLocalCenterPos.x + width / 2, planeLocalCenterPos.y - height / 2},
		{planeLocalCenterPos.x - width / 2, planeLocalCenterPos.y + height / 2},
		{planeLocalCenterPos.x + width / 2, planeLocalCenterPos.y + height / 2},
	};

	Vector2 planeWorldPos = {640.0f,720.0f};
	Vector2 planeWorldFourCornersPos[4] = {};
*/



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



};

