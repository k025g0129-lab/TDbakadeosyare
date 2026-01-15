#include "Scene.h"
#include "Vector2.h"

#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include<Xinput.h>


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

	for (int i = 0; i < 150; i++) {
		backGround[i].skyPos.x = 0.0f;
		backGround[i].skyPos.y = (-720.0f) * i;
		backGround[i].skyOriginalPos.x = 0.0f;
		backGround[i].skyOriginalPos.y = (-720.0f) * i;
	}
	scrollY = 0.0f;
	isTouchCheckpoint = false;
	

	// チェックポイント
	checkPoint.distance = 1500.0f;
	checkPoint.lv = 1;
	checkPoint.isPreparingForLanding = false;
	checkPoint.checkPointY = float(checkPoint.lv) * checkPoint.distance;
	checkPoint.scrollSpeed = 1.5f;
	
	whiteTextureHandle = Novice::LoadTexture("./NoviceResources/white1x1.png");
	
	// プレイヤー生成
	player = new Player();

	
}

void Scene::Update() {

	// コントローラーの状態を取得
	prevPadState = padState;
	XInputGetState(0, &padState);

	switch (gameScene) {

	case TITLE:

		TitleUpdate();
		
		break;

	case TUTORIAL:

		TutorialUpdate();

		break;

	case MAIN_GAME:

		MainGameUpdate();

		break;

	case RESULT:

		ResultUpdate();
		
		break;
	}
}

void Scene::Draw() {

	switch (gameScene) {
	case TITLE:

		TitleDraw();

		break;
	
	case TUTORIAL:

		TutorialDraw();
	
		break;
	
	case MAIN_GAME:

		MainGameDraw();
	
		break;
	
	case RESULT:

		ResultDraw();
	
		break;
	}
	
}

/*---------
   関数
----------*/

// 入力処理
bool Scene::IsPressB() const {
	return (padState.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0;
}

// Bボタンが押された瞬間
bool Scene::IsTriggerB() const {
	return (padState.Gamepad.wButtons & XINPUT_GAMEPAD_B) &&
		!(prevPadState.Gamepad.wButtons & XINPUT_GAMEPAD_B);
}


// 更新処理
void Scene::TitleUpdate() {

	// Bボタンでチュートリアルへ
	if (IsTriggerB()) {
		gameScene = TUTORIAL;
	}
}

void Scene::TutorialUpdate() {
	// Bボタンでメインゲームへ
	if (IsTriggerB()) {
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

	case LANDING:

		LandingUpdate();

		break;
		
	}

}


void Scene::ChargeUpdate() {

	//ここらへんは一部勝手に作ったので採用するか微妙
	chargeTime--;

	if (chargeTime <= 0) {

		chargeTime = 600;
		phase = RISE;

		isScroll = true;

		// チェックポイント決め
		checkPoint.lv++;
		checkPoint.checkPointY = float(checkPoint.lv) * checkPoint.distance;

	}

}

void Scene::RiseUpdate() {

	//左右の壁に触れた時ゲームオーバーを書く予定


	//チェックポイント
	// 1.プレイヤーを更新
	player->Update_play();

	// 2.スクロール量をプレイヤーの速度分減少させる
	scrollY += checkPoint.scrollSpeed;

	// 3.背景の位置を更新
	for(int i = 0; i < 150; i++){
		backGround[i].skyPos.y = backGround[i].skyOriginalPos.y + scrollY;
	}

	// 4.チェックポイント通過判定
	if (scrollY - checkPoint.checkPointY >= 600.0f) {
		phase = LANDING;
		isScroll = false;
	}

}

//プレイヤーへの真ん中から下の描画用場所
void Scene::LandingUpdate() {


	if (isScroll) {
		scrollY += 1.5f;
		for (int i = 0; i < 150; i++) {
			backGround[i].skyPos.y = backGround[i].skyOriginalPos.y + scrollY;
		}


		if (scrollY - checkPoint.checkPointY >= 600.0f) {
			phase = CHARGE;
			isScroll = false;
		}
		
	}
}

void Scene::ResultUpdate() {
	// Bボタンでタイトルへ
	if (IsTriggerB()) {
		gameScene = TITLE;
	}

}

// ------------------------------------------------------------------------------------

// 描画処理
void Scene::TitleDraw() {
	Novice::DrawBox(540, 320, 200, 80, 0.0f, 0xffffffff, kFillModeSolid);
}

void Scene::TutorialDraw() {
	Novice::DrawBox(440, 220, 400, 280, 0.0f, RED, kFillModeSolid);
}

void Scene::MainGameDraw() {

	for (int i = 0; i < 150; i++) {
		if (i % 2 == 0) {
			Novice::DrawSprite(
				static_cast<int>(backGround[i].skyPos.x),
				static_cast<int>(backGround[i].skyPos.y),
				whiteTextureHandle,
				1280, 720,
				0.0f, 0xFF000044
			);
		}

		if (i % 2 == 1) {
			Novice::DrawSprite(
				static_cast<int>(backGround[i].skyPos.x),
				static_cast<int>(backGround[i].skyPos.y),
				whiteTextureHandle,
				1280, 720,
				0.0f, 0x00FF0044
			);
		}

	}


	switch (phase) {
	case CHARGE:

		ChargeDraw();

		break;

	case RISE:
		
		RiseDraw();

		break;

	case LANDING:

		LandingDraw();

		break;
	}

	player->Draw();
}



void Scene::ChargeDraw() {

	Novice::DrawLine(0, int(checkPoint.checkPointY - scrollY), 1280, int(checkPoint.checkPointY - scrollY), 0xFF0000FF);
}



void Scene::RiseDraw() {

	Novice::ScreenPrintf(0, 20, "%f", backGround[1].skyPos.y);

	Novice::DrawLine(0, -int(checkPoint.checkPointY - scrollY), 1280, -int(checkPoint.checkPointY - scrollY), 0xFF0000FF);
	Novice::ScreenPrintf(0, 80, "%d", int(checkPoint.checkPointY - scrollY));


}



void Scene::LandingDraw() {

	Novice::DrawLine(0, -int(checkPoint.checkPointY - scrollY), 1280, -int(checkPoint.checkPointY - scrollY), 0xFF0000FF);

}


void Scene::ResultDraw() {
	Novice::DrawBox(540, 320, 200, 80, 0.0f, 0xffffffff, kFillModeSolid);
}
