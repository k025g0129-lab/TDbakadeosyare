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
	
	checkPoint.distance = 1500.0f;
	checkPoint.lv = 1;
	checkPoint.isPreparingForLanding = false;
	checkPoint.checkPointY = float(checkPoint.lv) * checkPoint.distance;

	whiteTextureHandle = Novice::LoadTexture("./NoviceResources/white1x1.png");
	// プレイヤー生成
	player = new Player();

	// チェックポイント
	checkpoint.checkPointY = -1500.0f; 
	checkpoint.isPassed = false;

}

void Scene::Update() {

	// コントローラーの状態を取得
	prevPadState = padState;
	XInputGetState(0, &padState);

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
	chargeTime--;

	if (chargeTime <= 0) {

		phase = RISE;
		int difference = leftChargeAmount - rightChargeAmount;
		tiltDegree = difference * difference;

		if (leftChargeAmount > rightChargeAmount) {
			direction = LEFT;
		}
		else {
			direction = RIGHT;
		}

	}

}


void Scene::RiseUpdate() {
	// プレイヤー更新
	player->Update(3);
	
	// チェックポイント判定
	if (!checkpoint.isPassed && player->planeWorldPos.y <= checkpoint.checkPointY) {
		checkpoint.isPassed = true;

		// 【巻き直し】
		phase = CHARGE;
		chargeTime = 600; // 再度チャージへ

		
	}
}

void Scene::ResultUpdate() {
	// Bボタンでタイトルへ
	if (IsTriggerB()) {
		gameScene = TITLE;
	}

}

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
	}
}

void Scene::ResultDraw() {
	Novice::DrawBox(540, 320, 200, 80, 0.0f, 0xffffffff, kFillModeSolid);
}


void Scene::ChargeUpdate() {

	//ここらへんは一部勝手に作ったので採用するか微妙
	chargeTime--;

	if (chargeTime <= 0) {

		chargeTime = 600;
		phase = RISE;

		//チェックポイント決め
		checkPoint.lv++;
		checkPoint.checkPointY = float(checkPoint.lv) * checkPoint.distance;
		
		//無しにする原因
		int difference = leftChargeAmount - rightChargeAmount;
		tiltDegree = difference * difference;

		if (leftChargeAmount > rightChargeAmount) {
			direction = LEFT;
		} else {
			direction = RIGHT;
		}

	}
	Novice::DrawLine(0, int(checkPoint.checkPointY - scrollY), 1280, int(checkPoint.checkPointY - scrollY), 0xFF0000FF);
}

void Scene::ChargeDraw() {
}

void Scene::RiseUpdate() {

	//上昇
	if (isScroll) {
		scrollY += 3.0f;
		for (int i = 0; i < 150; i++) {
			backGround[i].skyPos.y = backGround[i].skyOriginalPos.y + scrollY;
		}
		Novice::ScreenPrintf(0, 20, "%f", backGround[1].skyPos.y);

		Novice::DrawLine(0, -int(checkPoint.checkPointY - scrollY ), 1280, -int(checkPoint.checkPointY - scrollY),0xFF0000FF);
		Novice::ScreenPrintf(0, 80, "%d", int(checkPoint.checkPointY - scrollY));


	}

	//左右の壁に触れた時ゲームオーバーを書く予定
	

	//チェックポイント触れ
	//チェックポイントがスクリーン依存なので変える必要あり
	if (checkPoint.checkPointY <= scrollY - 300.0f ){
		phase = LANDING;
	}
	
}

void Scene::RiseDraw() {
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
			
		}
		Novice::DrawLine(0, -int(checkPoint.checkPointY - scrollY), 1280, -int(checkPoint.checkPointY - scrollY), 0xFF0000FF);
	}
}

void Scene::LandingDraw() {
}
