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
	float boost;
	float upValue;
	const float MAX_UP_VALUE = 1.127f; // 1f当たりに進む上昇量の最大値

	// 自機の傾き
	float angle;
	float powerDiff;
	float angleFacter;

	Player();
	~Player();

	void Update_charge_propeller();
	void Update_charge_boost();
	void Update_play();

	void Draw();
};

