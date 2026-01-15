#pragma once
#include"Vector2.h"

class Object {
public:
	Vector2 position;
	float height;
	float width;
	bool isActive;

	Object(Vector2 initialPosition);
	~Object();

	void Update();
	void Draw();
};

