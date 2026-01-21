#pragma once
#include"Vector2.h"

class Object {
public:
	Vector2 position;
	float height;
	float width;
	bool isActive;


	struct Bird {
		Vector2 pos;
		float radius;
		float moveSpeedX;
		int isActive;
		int leftOrRight;
	};

	Bird bird;

	Object(Vector2 initialPosition);
	~Object();

	void Draw();


	void BirdInitialize();
	void BirdUpdate();
	void BirdDraw();
	

};

