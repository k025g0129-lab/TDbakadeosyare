#pragma once
#include"Vector2.h"

class Player {
public:
	Vector2 position;
	Vector2 accerelation;
	Vector2 velocity;

	Vector2_int oldLeftStickPos;
	Vector2_int currentLeftStickPos;

	Vector2_int oldRightStickPos;

	Vector2_int currentRightStickPos;

	float oldLeftAngle;
	float currentLeftAngle;

	float oldRightAngle;
	float currentRightAngle;

	float totalLeftRotation;
	float totalRightRotation;

	float leftPropellerPower;
	float rightPropellerPower;

	Vector2 speed;
	float boostGauge;
	float boostPower;
	float upValue;
	const float MAX_UP_VALUE = 1.127f; // 1f当たりに進む上昇量の最大値

	// プレイヤー用スクロール変数
	float playerScreenY;

	//見た目
	Vector2 planeLocalCenterPos = { 0.0f,0.0f };
	float width = 100.0f;
	float height = 300.0f;

	Vector2 planeLocalFourCornersPos[4] =
	{
		{planeLocalCenterPos.x - width / 2, planeLocalCenterPos.y - height / 2},
		{planeLocalCenterPos.x + width / 2, planeLocalCenterPos.y - height / 2},
		{planeLocalCenterPos.x - width / 2, planeLocalCenterPos.y + height / 2},
		{planeLocalCenterPos.x + width / 2, planeLocalCenterPos.y + height / 2},
	};

	Vector2 planeWorldPos = { 640.0f,720.0f };
	Vector2 planeWorldFourCornersPos[4] = {};


	int whiteTextureHandle = 0;

	
	// 自機の傾き
	float angle;
	float powerDiff;
	float angleFacter;

	Player();
	~Player();

	void Update_charge_propeller();
	void Update_charge_boost();
	void Update_play();

	void Draw(float scrollY);
};

