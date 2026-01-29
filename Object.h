#pragma once
#include"Vector2.h"

class Object {
public:
	Vector2 position;
	float height;
	float width;
	bool isActive;


	struct Bird {
		Vector2 skyPos;
		Vector2 screenPos;
		float radius;
		float moveSpeedX;
		int isActive;
		int leftOrRight;
	};

	Bird bird;
	int birdGH = 0;

	Object(Vector2 initialPosition);
	~Object();

	void Draw();


	void BirdInitialize();
	void BirdUpdate();
	void BirdDraw();


};