#include "Object.h"
#include <Novice.h>

Object::Object(Vector2 initialPosition) {
	position = initialPosition;
	height = 20.0f;
	width = 20.0f;
	isActive = false;


	bird.radius = 30.0f;

	bird.leftOrRight = rand() %2;

	if (bird.leftOrRight == 0) {
		bird.skyPos = { 0.0f - bird.radius ,0.0f };
		bird.screenPos = { 0.0f - bird.radius ,0.0f };
		bird.moveSpeedX = float(rand() % 5 + 5);

	}

	if (bird.leftOrRight == 1) {
		bird.skyPos = { 1280.0f + bird.radius,0.0f };
		bird.screenPos = { 1280.0f + bird.radius,0.0f };
		bird.moveSpeedX = -float(rand() % 5 + 5);
	}

	birdGH = Novice::LoadTexture("./Resources/images/bird.png");;
	bird.isActive = false;
}

Object::~Object() {};

void Object::Draw() {
	// 画面内なら描画
	// 画面外かどうかは全体管理クラスで行う
	if (isActive) {
		Novice::DrawEllipse(
			(int)position.x,
			(int)position.y,
			(int)width,
			(int)height,
			1.0f, 0xffffffff, kFillModeSolid
		);
	}
}



void Object::BirdInitialize() {

}

void Object::BirdUpdate() {
	if (bird.isActive) {
		bird.screenPos.x += bird.moveSpeedX;

		if (bird.leftOrRight == 0) {
			if (bird.screenPos.x  >= 1280.0f + bird.radius) {
				bird.isActive = false;

			}
		}

		if (bird.leftOrRight == 1) {
			if (bird.screenPos.x <= 0.0f - bird.radius) {
				bird.isActive = false;

			}
		}
	}


	BirdDraw();
}

void Object::BirdDraw() {

	Novice::DrawEllipse(
		static_cast<int>(bird.screenPos.x),
		static_cast<int>(bird.screenPos.y),
		static_cast<int>(bird.radius),
		static_cast<int>(bird.radius),
		0.0f, 0x0000FFFF,
		kFillModeSolid

	);

	Novice::DrawSprite(
		static_cast<int>(bird.screenPos.x - bird.radius),
		static_cast<int>(bird.screenPos.y - bird.radius),
		birdGH,
		1.0f, 1.0f,
		0.0f,
		0xFFFFFFFF
	);

}
