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

	// チャージ時間
	maxChargeTime = 1200; 
	propellerEndTime = 700;

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


/*------------
   更新処理
--------------*/
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

	}

}

void Scene::ChargeUpdate() {

	if (chargeTimer < maxChargeTime) {
		chargeTimer++;
	}
	else if (chargeTimer <= maxChargeTime) {
		phase = RISE;
	}

	if (chargeTimer < propellerEndTime) {
		player->Update_charge_propeller();
	}
	else if (chargeTimer < maxChargeTime) {
		player->Update_charge_boost();
	}

}


void Scene::RiseUpdate() {
	// プレイヤーの移動更新
	player->Update_play();

	// スクロール処理 (カメラの制御)
	float screenYLimit = 500.0f;
	float currentScroll = screenYLimit - player->position.y;

	if (currentScroll > scrollY) {
		scrollY = currentScroll;
	}

	// 背景の更新
	for (int i = 0; i < 150; i++) {
		backGround[i].skyPos.y = backGround[i].skyOriginalPos.y + scrollY;
	}

	// プレイヤーの描画座標計算（上昇中は中央固定、落下中は自由移動）
	if (player->position.y + scrollY < 500.0f) {
		player->playerScreenY = 500.0f;
	}
	else {
		player->playerScreenY = player->position.y + scrollY;
	}

	// 進捗（どれだけ上に進んだか）の計算
	progressY = playerStartY - player->position.y;

	// チェックポイント（着地判定）
	if (progressY >= checkPoint.triggerProgressY) {

		// 着地：完全停止
		player->velocity.y = 0.0f;

		// 次のチェックポイント準備
		checkPoint.lv++;
		checkPoint.triggerProgressY =
			float(checkPoint.lv) * checkPoint.distance;

		// 次の上昇基準点をここにする
		playerStartY = player->position.y;

		// チャージ時間を半分にする
		maxChargeTime /= 2;
		propellerEndTime /= 2;

		// チャージ時間の制限


		// チャージへ戻る
		chargeTimer = 0;
		player->ResetForCharge();
		phase = CHARGE;
	}

}

void Scene::ResultUpdate() {
	// Bボタンでタイトルへ
	if (IsTriggerB()) {
		gameScene = TITLE;
	}

}


/*------------
   描画処理
--------------*/
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

		player->Draw(player->playerScreenY);
		break;

	}
}

void Scene::ResultDraw() {
	Novice::DrawBox(540, 320, 200, 80, 0.0f, 0xffffffff, kFillModeSolid);
}

void Scene::ChargeDraw() {
	if (chargeTimer < propellerEndTime) {
		Novice::DrawBox(0, 0, 1280, 720, 0.0f, 0x203744ff, kFillModeSolid);
	}
	else if (chargeTimer < maxChargeTime) {
		Novice::DrawBox(0, 0, 1280, 720, 0.0f, 0x522f60ff, kFillModeSolid);
	}

	Novice::ScreenPrintf(300, 0, "charge Timer = %d", chargeTimer);
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

