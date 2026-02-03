#include "Object.h"
#include <Novice.h>

Object::Object(Vector2 initialPosition) {
	position = initialPosition;
	height = 20.0f;
	width = 20.0f;
	isActive = false;

	bird.radius = 30.0f;
	BirdInitialize();
}

Object::~Object() {


};

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
	bird.leftOrRight = rand() % 2;

	if (bird.leftOrRight == 0) {
		bird.skyPos = { 0.0f - bird.radius ,0.0f };
		bird.screenPos = { 0.0f - bird.radius ,0.0f };
		bird.moveSpeedX = float(rand() % 5 + 5);

		birdGH = Novice::LoadTexture("./Resources/images/birdRight.png");
	}

	if (bird.leftOrRight == 1) {
		bird.skyPos = { 1280.0f + bird.radius,0.0f };
		bird.screenPos = { 1280.0f + bird.radius ,0.0f };
		bird.moveSpeedX = -float(rand() % 5 + 5);
		birdGH = Novice::LoadTexture("./Resources/images/birdLeft.png");
	}

	bird.isActive = false;
	bird.isAppearance = false;
	
}


void Object::BirdUpdate() {
	if (bird.isActive) {
		bird.screenPos.x += bird.moveSpeedX;

		if (bird.screenPos.x >= 1780.0f + bird.radius) {
			bird.isActive = false;
		}

		if (bird.screenPos.x <= -500.0f - bird.radius) {
			bird.isActive = false;
		}

	}


	BirdDraw();
}

void Object::BirdDraw() {

	if (bird.isActive) {

		Novice::DrawSprite(
			static_cast<int>(bird.screenPos.x - bird.radius),
			static_cast<int>(bird.screenPos.y - bird.radius),
			birdGH,
			1.0f, 1.0f,
			0.0f,
			0xFFFFFFFF
		);

	}


}