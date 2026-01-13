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

	Player();
	~Player();

	void Update(int scene);
	void Draw();


};

