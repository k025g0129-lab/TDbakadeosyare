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
	switch (gameScene) {

	case TITLE:
		TitleUpdate();
		TitleDraw();
		
		break;

	case TUTORIAL:
		TutorialUpdate();
		TutorialDraw();

		break;

	case MAIN_GAME:
		MainGameUpdate();
		MainGameDraw();

		break;

	case RESULT:
		ResultUpdate();
		ResultDraw();
		
		break;
	}
}

/*
bool Scene::IsPressB() const {
	return (padState.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0;
}

bool Scene::IsTriggerB() const {
	return (padState.Gamepad.wButtons & XINPUT_GAMEPAD_B) &&
		!(prevPadState.Gamepad.wButtons & XINPUT_GAMEPAD_B);
}
*/

// 更新処理
void Scene::TitleUpdate() {
	// Bボタンでチュートリアルへ
	if (Novice::IsTriggerButton(0, kPadButton12)) {
		gameScene = TUTORIAL;
	}
}

void Scene::TutorialUpdate() {
	// Bボタンでメインゲームへ
	if (Novice::IsTriggerButton(0, kPadButton12)) {
		gameScene = MAIN_GAME;
	}

}

void Scene::MainGameUpdate() {
	PhaseUpdate();
}

void Scene::PhaseUpdate() {
	switch (phase) {
	case CHARGE:
		ChargeUpdate();
		break;
	case RISE:
		RiseUpdate();
		break;
	}

}

void Scene::ResultUpdate() {
	// Bボタンでタイトルへ
	if (Novice::IsTriggerButton(0, kPadButton12)) {
		gameScene = TITLE;
	}

}

// 描画処理
void Scene::TitleDraw() {
	Novice::DrawBox(540, 320, 200, 80, 0.0f, 0xffffffff, kFillModeSolid);
}

void Scene::TutorialDraw() {
	Novice::DrawBox(440, 220, 400, 280, 0.0f, 0xffffffff, kFillModeSolid);
}

void Scene::MainGameDraw() {
	switch (phase) {
	case CHARGE:
		ChargeDraw();
		break;
	case RISE:
		RiseDraw();
		break;
	}
}

void Scene::ResultDraw() {
	Novice::DrawBox(540, 320, 200, 80, 0.0f, 0xffffffff, kFillModeSolid);
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

}

void Scene::ChargeDraw() {
}

void Scene::RiseUpdate() {
	
	
}

void Scene::RiseDraw() {
}
