#include "Object.h"
#include <Novice.h>

Object::Object(Vector2 initialPosition) {
	position = initialPosition;
	height = 20.0f;
	width = 20.0f;
	isActive = false;


	bird.radius = 50.0f;

	bird.leftOrRight = rand() %2;

	if (bird.leftOrRight == 0) {
		bird.pos = { 0.0f,1280.0f + bird.radius };
		bird.moveSpeedX = -float(rand() % 5 + 5);

	}

	if (bird.leftOrRight == 1) {
		bird.pos = {0.0f,0.0f - bird.radius};
		bird.moveSpeedX = float(rand() % 5 + 5);
	}


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
		bird.pos.x += bird.moveSpeedX;

		if (bird.leftOrRight == 0) {
			if (bird.pos.x  >= 1280.0f + bird.radius) {
				bird.isActive = false;

			}
		}

		if (bird.leftOrRight == 1) {
			if (bird.pos.x <= 0.0f - bird.radius) {
				bird.isActive = false;

			}
		}
	}


	BirdDraw();
}

void Object::BirdDraw() {

	Novice::DrawEllipse(
		static_cast<int>(bird.pos.x),
		static_cast<int>(bird.pos.y),
		static_cast<int>(bird.radius),
		static_cast<int>(bird.radius),
		0.0f, 0x0000FFFF,
		kFillModeSolid

	);
}
