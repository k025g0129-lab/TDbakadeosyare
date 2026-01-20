#include "Scene.h"
#include "Vector2.h"

#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include<Xinput.h>


Scene::Scene() {
	Initialize();
}

Scene::~Scene() {
	delete player;
	player = nullptr;
}

void Scene::Initialize() {
	gameScene = TITLE;
	phase = CHARGE;
	direction = LEFT;

	leftChargeAmount = 0;
	rightChargeAmount = 0;
	chargeTimer = 0;
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
	checkPoint.triggerProgressY = float(checkPoint.lv) * checkPoint.distance;

	whiteTextureHandle = Novice::LoadTexture("./NoviceResources/white1x1.png");

	// プレイヤー生成
	player = new Player();

	playerStartY = player->position.y;

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
		// 描画するY座標（skyPos.y は Update 内で skyOriginalPos.y + scrollY と計算されているはず）
		int drawY = static_cast<int>(backGround[i].skyPos.y);

		// 【重要】画面外の背景は描画しない（カリング）
		// 720以上（画面より下）または -720以下（画面より上）ならスキップ
		if (drawY > 720 || drawY < -720) {
			continue;
		}

		// 色の決定（偶数・奇数）
		unsigned int color = (i % 2 == 0) ? 0xFF000044 : 0x00FF0044;

		// 描画実行
		Novice::DrawSprite(
			0, drawY,           // Xは0固定、Yは計算後の座標
			whiteTextureHandle,
			1280, 720,
			0.0f, color
		);
	}


	switch (phase) {
	case CHARGE:
		ChargeDraw();
		player->Draw(player->playerScreenY);

		break;

	case RISE:
		RiseDraw();
		// ★上昇中もチェックポイントの線を表示する
		LandingDraw();
		player->Draw(player->playerScreenY);
		break;

	case LANDING:
		// ★ここを追加！これがないと着地フェーズで線が消える
		LandingDraw();
		player->Draw(player->playerScreenY);
		break;
	}
}

void Scene::ResultDraw() {
	Novice::DrawBox(540, 320, 200, 80, 0.0f, 0xffffffff, kFillModeSolid);
}


void Scene::ChargeUpdate() {

	if (chargeTimer < 1200) {
		chargeTimer++;
	}
	else if (chargeTimer <= 1200) {
		phase = RISE;
	}

	if (chargeTimer < 700) {
		player->Update_charge_propeller();
	}

	if (chargeTimer > 701 && chargeTimer < 1200) {
		player->Update_charge_boost();
	}

	////ここらへんは一部勝手に作ったので採用するか微妙
	//chargeTime--;

	//if (chargeTime <= 0) {

	//	chargeTime = 600;
	//	phase = RISE;

	//	//チェックポイント決め
	//	checkPoint.lv++;
	//	checkPoint.checkPointY = float(checkPoint.lv) * checkPoint.distance;
	//	
	//	//無しにする原因
	//	int difference = leftChargeAmount - rightChargeAmount;
	//	tiltDegree = difference * difference;

	//	if (leftChargeAmount > rightChargeAmount) {
	//		direction = LEFT;
	//	} else {
	//		direction = RIGHT;
	//	}

	//}
	//Novice::DrawLine(0, int(checkPoint.checkPointY - scrollY), 1280, int(checkPoint.checkPointY - scrollY), 0xFF0000FF);
}

void Scene::ChargeDraw() {
	if (chargeTimer < 700) {
		Novice::DrawBox(0, 0, 1280, 720, 0.0f, 0x203744ff, kFillModeSolid);
	}

	if (chargeTimer > 701 && chargeTimer < 1200) {
		Novice::DrawBox(0, 0, 1280, 720, 0.0f, 0x522f60ff, kFillModeSolid);
	}

	Novice::ScreenPrintf(300, 0, "charge Timer = %d", chargeTimer);
}


void Scene::RiseUpdate() {
	player->Update_play();

	// 上昇距離の計算
	float currentScroll = playerStartY - player->position.y;

	if (currentScroll > scrollY) {
		scrollY = currentScroll;
	}

	// 背景更新
	for (int i = 0; i < 150; i++) {
		backGround[i].skyPos.y = backGround[i].skyOriginalPos.y + scrollY;
	}

	// プレイヤー描画用Y
	player->playerScreenY = player->position.y + scrollY;

	// チェックポイント判定（上昇距離）
	if (!checkPoint.isPreparingForLanding &&
		scrollY >= checkPoint.triggerProgressY) {

		checkPoint.isPreparingForLanding = true;
		isScroll = true;
		phase = LANDING;
	}
}


void Scene::RiseDraw() {

	// MainGameDrawから引っ張ってきた
	for (int i = 0; i < 150; i++) {
		// 描画するY座標（skyPos.y は Update 内で skyOriginalPos.y + scrollY と計算されているはず）
		int drawY = static_cast<int>(backGround[i].skyPos.y);

		// 【重要】画面外の背景は描画しない（カリング）
		// 720以上（画面より下）または -720以下（画面より上）ならスキップ
		if (drawY > 720 || drawY < -720) {
			continue;
		}

		// 色の決定（偶数・奇数）
		unsigned int color = (i % 2 == 0) ? 0xFF000044 : 0x00FF0044;

		// 描画実行
		Novice::DrawSprite(
			0, drawY,           // Xは0固定、Yは計算後の座標
			whiteTextureHandle,
			1280, 720,
			0.0f, color
		);

	}

}

//プレイヤーへの真ん中から下の描画用場所
void Scene::LandingUpdate() {

	if (isScroll) {
		scrollY += 1.5f;
		for (int i = 0; i < 150; i++) {
			backGround[i].skyPos.y = backGround[i].skyOriginalPos.y + scrollY;
		}


		if (scrollY - checkPoint.triggerProgressY >= 600.0f) {
			phase = CHARGE;

		}
	}
}

void Scene::LandingDraw() {
	Novice::DrawLine(0, -int(checkPoint.triggerProgressY - scrollY), 1280, -int(checkPoint.triggerProgressY - scrollY), 0xFF0000FF);

}
