#include "Object.h"
#include <Novice.h>

Object::Object(Vector2 initialPosition) {
	position = initialPosition;
	height = 20.0f;
	width = 20.0f;
	isActive = false;
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