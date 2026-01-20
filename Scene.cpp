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
	checkPoint.checkPointY = float(checkPoint.lv) * checkPoint.distance;
	checkPoint.scrollSpeed = 1.5f;
	
	whiteTextureHandle = Novice::LoadTexture("./NoviceResources/white1x1.png");
	
	// プレイヤー生成
	player = new Player();

}

Scene::~Scene() {
	delete player;
	player = nullptr;
}

// 更新処理
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

// 描画処理
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


// ------------------------------------------------------------------------------------


/*----------------
   更新処理
-----------------*/
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


void Scene::ChargeUpdate() {

	if (chargeTimer < 1200) {
		chargeTimer++;
	} else if (chargeTimer <= 1200){
		phase = RISE;
	}

	if (chargeTimer < 700 ) {
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



void Scene::RiseUpdate() {

	// 1. プレイヤーの更新
	player->Update_play();

	// 2. しきい値
	float screenYLimit = 500.0f;

	// 3. プレイヤーが限界を超えた分の「現在の計算上のスクロール量」
	// (500 - プレイヤーの現在位置) 
	// プレイヤーが 500 より上にいれば正の値、下にいれば負の値になる
	float currentScroll = screenYLimit - player->position.y;

	// 4. 【重要】スクロール量を「増える方向（上方向への進行）」だけに限定する
	// 今までの scrollY より currentScroll が大きくなった時だけ更新する
	if (currentScroll > scrollY) {
		scrollY = currentScroll;
	}

	// 5. 背景の更新（scrollY は減らないので、プレイヤーが下がっても背景は止まったままになる）
	for (int i = 0; i < 150; i++) {
		backGround[i].skyPos.y = backGround[i].skyOriginalPos.y + scrollY;
	}

	player->playerScreenY = player->position.y + scrollY;

	if (player->position.y + scrollY < 500.0f) {
		// 【上昇中・中央固定モード】
		// プレイヤーが画面中央より上にいこうとする間は、500に固定する
		player->playerScreenY = 500.0f;
	}
	else {
		// 【落下・自由移動モード】
		// 500より下（画面下端側）にいるときは、スクロールの影響をそのまま受けて下に下がる
		player->playerScreenY = player->position.y + scrollY;
	}

	// 6. チェックポイント通過判定
	if (player->position.y <= checkPoint.checkPointY) {
		phase = LANDING; 
	}
}


//プレイヤーへの真ん中から下の描画用場所
void Scene::LandingUpdate() {

	// スクロール開始
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

	// 600ピクセル分くらい余韻で進んだら
	if (scrollY - checkPoint.checkPointY >= 600.0f) {

		// ここでプレイヤーの状態をリセット
		player->leftPropellerPower = 0.0f;
		player->rightPropellerPower = 0.0f;
		player->upValue = 0.0f; // 上昇速度もリセット

		phase = CHARGE;
		isScroll = false;
	}
}




// ------------------------------------------------------------------------------------

/*---------------
    描画処理
-----------------*/
void Scene::TitleDraw() {
	Novice::DrawBox(540, 320, 200, 80, 0.0f, 0xffffffff, kFillModeSolid);
}

void Scene::TutorialDraw() {
	Novice::DrawBox(440, 220, 400, 280, 0.0f, RED, kFillModeSolid);
}

void Scene::MainGameDraw() {
	// --- 1. 背景描画 (全フェーズ共通) ---
	for (int i = 0; i < 150; i++) {
		int drawY = static_cast<int>(backGround[i].skyPos.y);
		// 画面外（上下720px以上外）なら描画スキップ
		if (drawY > 720 || drawY < -720) continue;

		unsigned int color = (i % 2 == 0) ? 0xFF000044 : 0x00FF0044;
		Novice::DrawSprite(0, drawY, whiteTextureHandle, 1280, 720, 0.0f, color);
	}

	// --- 2. フェーズごとの特殊描画 (UIやエフェクト) ---
	switch (phase) {
	case CHARGE:
		ChargeDraw();
		break;
	case RISE:
		// RiseDraw()に背景描画を重複させていたので、必要なければ中身を空にするか削除
		RiseDraw();
		break;
	case LANDING:
		LandingDraw();
		break;
	}

	// --- 3. プレイヤー描画 (全フェーズ共通) ---
	// Updateで計算された playerScreenY を使用
	player->Draw(player->playerScreenY);

	// デバッグ用情報
	Novice::ScreenPrintf(10, 10, "Phase: %d", phase);
	Novice::ScreenPrintf(10, 30, "ScrollY: %f", scrollY);
}

void Scene::ChargeDraw() {
	// チャージ時間に応じて背景の色を変えるなどの演出
	if (chargeTimer < 700) {
		// 暗い青
		Novice::DrawBox(0, 0, 1280, 720, 0.0f, 0x203744ff, kFillModeSolid);
	}
	else if (chargeTimer < 1200) {
		// 少し明るい紫
		Novice::DrawBox(0, 0, 1280, 720, 0.0f, 0x522f60ff, kFillModeSolid);
	}

	// デバッグ用にタイマーを表示
	Novice::ScreenPrintf(300, 10, "charge Timer = %d / 1200", chargeTimer);
}

// 既存のRiseDrawから背景描画を削除（MainGameDrawで一括で行うため）
void Scene::RiseDraw() {
	// 上昇中のみ出したいエフェクト等があればここに書く
}

void Scene::LandingDraw() {
	// チェックポイントのラインを描画
	// 世界座標とスクロール量から描画位置を特定
	int lineY = static_cast<int>(checkPoint.checkPointY + scrollY);
	Novice::DrawLine(0, lineY, 1280, lineY, 0xFF0000FF);
}


void Scene::ResultDraw() {
	Novice::DrawBox(540, 320, 200, 80, 0.0f, 0xffffffff, kFillModeSolid);
}

