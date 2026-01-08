#include "Scene.h"
#include "Vector2.h"
#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>

Scene::Scene() {
	Initialize();
}

void Scene::Initialize() {
	gameScene = TITLE;
	phase = CHARGE;
	direction = LEFT;

	leftChargeAmount = 0;
	rightChargeAmount = 0;
	chargeTime = 600;
	tiltDegree = 0;
	isScroll = false;

	/*
	planeLocalCenterPos = { 0.0f,0.0f };
	width = 100.0f;
	height = 300.0f;

	planeLocalFourCornersPos[0] = {planeLocalCenterPos.x - width / 2, planeLocalCenterPos.y - height / 2};
	planeLocalFourCornersPos[1] = {planeLocalCenterPos.x + width / 2, planeLocalCenterPos.y - height / 2};
	planeLocalFourCornersPos[2] = {planeLocalCenterPos.x - width / 2, planeLocalCenterPos.y + height / 2};
	planeLocalFourCornersPos[3] = {planeLocalCenterPos.x + width / 2, planeLocalCenterPos.y + height / 2};


	planeWorldPos = { 640.0f,720.0f };
	planeWorldFourCornersPos[0] = { planeLocalFourCornersPos[0]. + planeWorldPos, planeLocalFourCornersPos[0] + planeWorldPos };
	planeWorldFourCornersPos[1] =
	planeWorldFourCornersPos[2] =
	planeWorldFourCornersPos[3] =*/

	whiteTextureHandle = Novice::LoadTexture("./NoviceResources/white1x1.png");
}

void Scene::Update() {
	
}

void Scene::Drow() {

}

void Scene::PhaseUpdate() {


}

void Scene::ChargeUpdate() {
	chargeTime--;

	if (chargeTime <= 0) {

		phase = RISE;
		int difference = leftChargeAmount - rightChargeAmount;
		tiltDegree = difference * difference;

		if (leftChargeAmount > rightChargeAmount) {
			direction = LEFT;
		} else {
			direction = RIGHT;
		}

	}

	ChargeDrow();
}

void Scene::ChargeDrow() {
}

void Scene::RiseUpdate() {
	
	
	
	RiseDrow();
}

void Scene::RiseDrow() {
	
}
