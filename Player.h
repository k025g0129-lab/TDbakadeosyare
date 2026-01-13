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

	float speed;
	float boost;

	// 自機の傾き
	float angle;

	Player();
	~Player();

	void Update(int scene);
	void Draw();


	int timer_trial;
	int scene_trial;
};

