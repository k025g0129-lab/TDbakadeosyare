#include "Scene.h"
#include "Vector2.h"
#include "Object.h"
#include "Function.h"
#include"Easing.h"

#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <Xinput.h>




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
	checkPoint.lv = 1;
	checkPoint.isPreparingForLanding = false;
	checkPoint.triggerProgressY = float(checkPoint.lv) * checkPoint.distance;

	isClear = false;

	whiteTextureHandle = Novice::LoadTexture("./NoviceResources/white1x1.png");

	// チャージ時間
	maxChargeTime = 1200;
	propellerEndTime = 700;

	// チャージ演出初期化
	chargeSubPhase = SHOW_PROPELLER_TEXT;
	chargeTimer = 0;
	chargeTextT = 0.0f;
	chargeTextPos = { 0.0f, TEXT_START_Y };

	// プレイヤー生成
	player = new Player();
	playerStartY = player->position.y;

	// 難易度設定
	difficulty = NORMAL;
	ApplyDifficulty();
	checkPoint.triggerProgressY = float(checkPoint.lv) * checkPoint.distance;

	Vector2 a = { 0.0f,0.0f };
	for (int i = 0; i < maxBird; i++) {
		bird[i] = new Object(a);
	}
	birdOccurrences = 1;
	preCheckPointPosY = 0.0f;
	// 難易度設定
	difficulty = NORMAL;
	ApplyDifficulty();

	PtitlePos = { 0.0f,0.0f };
	titleButton = GAME_PLAY_BUTTON;
	//titleT = 0.0f;

	amplitude = 100.0f;
	theta = 0.0f;

	//GH
	//タイトル
	titleBGGH = Novice::LoadTexture("./Resources/images/skyBG.png");
	pressAGH = Novice::LoadTexture("./Resources/images/pressA.png");
	playChoiceGH = Novice::LoadTexture("./Resources/images/play_choice.png");
	tutorialChoiceGH = Novice::LoadTexture("./Resources/images/tutorial_choice.png");
	titleLogoGH = Novice::LoadTexture("./Resources/images/titleLogo.png");
	PtitleLogoGH = Novice::LoadTexture("./Resources/images/P.png");

	//チュートリアル
	pressAexitGH = Novice::LoadTexture("./Resources/images/pressAexit.png");
	LeftArrowGH = Novice::LoadTexture("./Resources/images/LeftArrow.png");
	RightArrowGH = Novice::LoadTexture("./Resources/images/RightArrow.png");
	asobikataGH = Novice::LoadTexture("./Resources/images/asobikata.png");
}




void Scene::ApplyDifficulty() {
	switch (difficulty) {
	case EASY:
		checkPoint.distance = 1500.0f;
		maxChargeTime = 1000;
		propellerEndTime = 500;
		break;

	case NORMAL:
		checkPoint.distance = 2000.0f;
		maxChargeTime = 1000;
		propellerEndTime = 500;
		break;

	case HARD:
		checkPoint.distance = 3000.0f;
		maxChargeTime = 800;
		propellerEndTime = 400;
		break;
	}
	checkPoint.triggerProgressY = float(checkPoint.lv) * checkPoint.distance;
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

	case DIFFICULTY_SELECT:
		DifficultySelectUpdate();
		
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

	case DIFFICULTY_SELECT:
		DifficultySelectDraw();
		
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

// 入力処理
bool Scene::IsPressA() const {
	return (padState.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;
}

// Aボタンが押された瞬間
bool Scene::IsTriggerA() const {
	return (padState.Gamepad.wButtons & XINPUT_GAMEPAD_A) &&
		!(prevPadState.Gamepad.wButtons & XINPUT_GAMEPAD_A);
}


/*------------
   更新処理ee
--------------*/
void Scene::TitleUpdate() {
	// 左右でメニューを選択
	/*if ((padState.Gamepad.sThumbLX < -10000 && prevPadState.Gamepad.sThumbLX >= -10000) ||
		(padState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT && !(prevPadState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT))) {
		selectedTitleMenu = 0; // 左：START
		Novice::PlayAudio(soundHandleSelect, false, 1.0f);
	}
	if ((padState.Gamepad.sThumbLX > 10000 && prevPadState.Gamepad.sThumbLX <= 10000) ||
		(padState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT && !(prevPadState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT))) {
		selectedTitleMenu = 1; // 右：TUTORIAL
	}*/

	player->oldLeftStickPos.x = player->currentLeftStickPos.x;
	Novice::GetAnalogInputLeft(0, &player->currentLeftStickPos.x, &player->currentLeftStickPos.y);
	theta += float(M_PI) / 120.0f;
	PtitlePos.y = sinf(theta) * amplitude;


	//titleT += 0.005f;

	/*if (titleT > 1.0f) {
		titleT = 0.0f;
	}*/
	Novice::ScreenPrintf(900, 100, "%f", player->currentLeftStickPos.x);
	Novice::ScreenPrintf(900, 120, "%f", player->oldLeftStickPos.x);
	if (player->currentLeftStickPos.x > 0.0f && player->oldLeftStickPos.x <= 0.0f) {
		switch (titleButton) {

		case Scene::GAME_PLAY_BUTTON:
			titleButton = TUTORIAL_BUTTON;
			break;

		case Scene::TUTORIAL_BUTTON:
			titleButton = GAME_PLAY_BUTTON;
			break;
		}
	}

	if (player->currentLeftStickPos.x < 0.0f && player->oldLeftStickPos.x >= 0.0f) {
		switch (titleButton) {

		case Scene::GAME_PLAY_BUTTON:
			titleButton = TUTORIAL_BUTTON;
			break;

		case Scene::TUTORIAL_BUTTON:
			titleButton = GAME_PLAY_BUTTON;
			break;
		}
	}

	// Bボタンで進む
	if (IsTriggerB()) {
		Novice::PlayAudio(soundHandleDecide, false, 1.0f);
		if (selectedTitleMenu == 0) {
			switch (titleButton) {

			case Scene::GAME_PLAY_BUTTON:
				gameScene = DIFFICULTY_SELECT;
				break;

			case Scene::TUTORIAL_BUTTON:
				gameScene = TUTORIAL;
				break;
			}


			/*if (selectedTitleMenu == 0) {
				gameScene = DIFFICULTY_SELECT;
			}
			else {
				gameScene = TUTORIAL;
			}*/
		}
	}
}

void Scene::TutorialUpdate() {
	// Bボタンでメインゲームへ
	if (IsTriggerB()) {
		gameScene = TITLE;
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

	switch (chargeSubPhase) {

	// プロペラ案内表示
	case SHOW_PROPELLER_TEXT:
	{
		chargeTextT += 0.01f; // 進行
		float t = chargeTextT;

		// t を 0→2 に拡張
		if (t < 1.0f) {
			// 下 → 真ん中
			chargeTextPos.y = EaseOutBack(t, TEXT_START_Y, TEXT_END_Y);
		}
		else if (t < 2.0f) {
			// 真ん中 → 下
			float t2 = t - 1.0f; // 0〜1
			chargeTextPos.y = EaseInBack(t2, TEXT_END_Y, TEXT_START_Y);
		}
		else {
			// 完全に終了
			chargeTextT = 0.0f;
			chargeTimer = 0;
			chargeSubPhase = PROPELLER_CHARGE;
			chargeTextPos.y = TEXT_START_Y;
		}
	}
	return;

	// プロペラチャージ
	case PROPELLER_CHARGE:
		chargeTimer++;

		player->Update_charge_propeller();


		if (chargeTimer >= propellerEndTime) {
			chargeTextT = 0.0f;
			player->maxPropellerPower = player->leftPropellerPower + player->rightPropellerPower;

			if (checkPoint.lv >= 2) {
				chargeSubPhase = BOOST_CHARGE;
			}
			else {
				chargeSubPhase = SHOW_BOOST_TEXT;
			}
		}
	if (chargeTimer < maxChargeTime) {
		chargeTimer++;
	}
	else if (chargeTimer <= maxChargeTime) {
		player->maxPropellerPower = player->leftPropellerPower + player->rightPropellerPower;


		//プロコン忘れたデバック用絶対消す
		player->leftPropellerPower = 0.0f;
		player->rightPropellerPower = 0.0f;

		//何体鳥を配置するか
		birdOccurrences = checkPoint.lv;

		if (birdOccurrences <= 0) {
			birdOccurrences = 1;
		}

		if (birdOccurrences > 10) {
			birdOccurrences = 10;
		}

		for (int i = 0; i < birdOccurrences; i++) {
			bird[i]->BirdInitialize();
			bird[i]->bird.isActive = false;
		}

		phase = RISE;
	}

	return;

	// ブースト案内表示
	case SHOW_BOOST_TEXT:
	{
		chargeTextT += 0.01f; // 進行
		float t = chargeTextT;

		// t を 0→2 に拡張
		if (t < 1.0f) {
			// 下 → 真ん中
			chargeTextPos.y = EaseOutBack(t, TEXT_START_Y, TEXT_END_Y);
		}
		else if (t < 2.0f) {
			// 真ん中 → 下
			float t2 = t - 1.0f; // 0〜1
			chargeTextPos.y = EaseInBack(t2, TEXT_END_Y, TEXT_START_Y);
		}
		else {
			// 完全に終了
			chargeTextT = 0.0f;
			chargeSubPhase = BOOST_CHARGE;
			chargeTextPos.y = TEXT_START_Y;
		}
	}
	return;

	// ブーストチャージ
	case BOOST_CHARGE:
		chargeTimer++;

		player->Update_charge_boost();

		if (chargeTimer >= maxChargeTime) {

			//何体鳥を配置するか
			birdOccurrences = checkPoint.lv;

			if (birdOccurrences <= 0) {
				birdOccurrences = 1;
			}

			if (birdOccurrences > 10) {
				birdOccurrences = 10;
			}

			for (int i = 0; i < birdOccurrences; i++) {
				bird[i]->BirdInitialize();
				bird[i]->bird.isActive = false;
			}

			//上昇へ
			phase = RISE;
		}

		return;
	}
}


void Scene::RiseUpdate() {
	// プレイヤーの移動更新
	player->Update_play();
	for (int i = 0; i < maxBird; i++) {
		bird[i]->BirdUpdate();
	}

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


	//鳥出現
	for (int i = 0; i < birdOccurrences; i++) {
		if (progressY >= (checkPoint.triggerProgressY * (float(i + 1) / float(birdOccurrences + 1)))) {
			if (!bird[i]->bird.isAppearance) {
				if (bird[i]->bird.isActive == false) {

					bird[i]->bird.isActive = true;
					bird[i]->bird.isAppearance = true;

					bird[i]->bird.skyPos.y = player->position.y - 500.0f;
				}
			}
		}
	}


	Novice::ScreenPrintf(300, 0, "%d", bird[1]->bird.isActive);
	Novice::ScreenPrintf(300, 20, "%f", bird[1]->bird.screenPos.x);
	Novice::ScreenPrintf(300, 40, "%f", bird[1]->bird.skyPos.y);
	Novice::ScreenPrintf(300, 60, "checkPoint.triggerProgressY = %f", checkPoint.triggerProgressY);
	Novice::ScreenPrintf(300, 80, "progressY = %f", progressY);
	Novice::ScreenPrintf(300, 100, "keisan = %f", (checkPoint.triggerProgressY + playerStartY) / 2.0f);
	Novice::ScreenPrintf(300, 120, "player->position.y = %f", player->position.x);
	Novice::ScreenPrintf(300, 140, "checkPoint.lv = %d", checkPoint.lv);
	Novice::ScreenPrintf(550, 80, " tori= %f", (checkPoint.triggerProgressY * (float(2) / float(birdOccurrences + 1))));
	Novice::ScreenPrintf(550, 100, " nantaideruka %d", birdOccurrences);

	for (int i = 0; i < maxBird; i++) {
		bird[i]->bird.screenPos.y = bird[i]->bird.skyPos.y + scrollY;
	}

	for (int i = 0; i < maxBird; i++) {
		if (IsCollision({ bird[i]->bird.screenPos.x,bird[i]->bird.skyPos.y }, player->position, bird[i]->bird.radius, player->width)) {
			if (bird[i]->bird.isActive) {
				bird[i]->bird.isActive = false;
				bird[i]->bird.screenPos.x += 1000.0f;
				bird[i]->bird.screenPos.y += 1000.0f;
				player->leftPropellerPower /= 2.0f;
				player->rightPropellerPower /= 2.0f;
			}
		}
	}



	// チェックポイント（着地判定）
	if (progressY >= checkPoint.triggerProgressY) {

		// 着地：完全停止
		player->velocity.y = 0.0f;

		//前回のチェックポイント記録
		preCheckPointPosY = checkPoint.triggerProgressY;

		// 次のチェックポイント準備
		checkPoint.lv++;
		checkPoint.triggerProgressY =
			float(checkPoint.lv) * checkPoint.distance;

		// 次の上昇基準点をここにする
		playerStartY = player->position.y;

		// チャージ時間を半分にする
		// チャージ時間の短縮（Lv2になった瞬間＝最初の着地時のみ実行）
		if (checkPoint.lv == 2) {
			float reductionRate = 0.5f; // デフォルト（NORMAL）は半分

			if (difficulty == EASY)   reductionRate = 0.8f;
			if (difficulty == NORMAL) reductionRate = 0.5f;
			if (difficulty == HARD)   reductionRate = 0.5f;

			// 初回の着地時のみ、この倍率で時間を固定する
			maxChargeTime = static_cast<int>(maxChargeTime * reductionRate);
			propellerEndTime = static_cast<int>(propellerEndTime * reductionRate);

			// 下限チェック
			if (maxChargeTime < 60) {
				maxChargeTime = 60;
				propellerEndTime = 30;
			}
		}

		// チャージへ戻る
		chargeTimer = 0;
		chargeTextT = 0.0f;
		player->ResetForCharge();
		phase = CHARGE;

		// Lv2以降（1回着地した後）は演出を飛ばす
		if (checkPoint.lv >= 2) {
			chargeSubPhase = PROPELLER_CHARGE;
		}
		else {
			chargeSubPhase = SHOW_PROPELLER_TEXT;
		}
	}

}

void Scene::DifficultySelectUpdate() {
	// スティックの左右、または十字キーの左右で選択
	if ((padState.Gamepad.sThumbLX < -10000 && prevPadState.Gamepad.sThumbLX >= -10000) ||
		(padState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT && !(prevPadState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT))) {
		selectedDifficulty--;
		Novice::PlayAudio(soundHandleSelect, false, 1.0f);
	}
	if ((padState.Gamepad.sThumbLX > 10000 && prevPadState.Gamepad.sThumbLX <= 10000) ||
		(padState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT && !(prevPadState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT))) {
		selectedDifficulty++;
		Novice::PlayAudio(soundHandleSelect, false, 1.0f);
	}

	// ループさせるか、端で止めるかはお好みで（今回は端で止める）
	if (selectedDifficulty < 0) selectedDifficulty = 0;
	if (selectedDifficulty > 2) selectedDifficulty = 2;

	// Bボタンで決定
	if (IsTriggerB()) {
		Novice::PlayAudio(soundHandleDecide, false, 1.0f);
		difficulty = static_cast<Difficulty>(selectedDifficulty);
		ApplyDifficulty();
		gameScene = MAIN_GAME;
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

	Novice::DrawSprite(0, 0, titleBGGH, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
	Novice::DrawBox(0, 0, 1280, 720, 0.0f, 0x00000077, kFillModeSolid);
	Novice::DrawSprite(0, 0, pressAGH, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);

	Novice::DrawSprite(0, 0, titleLogoGH, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);

	//PtitlePos.y = EaseInOutBack(titleT,0.0f,-720.0f,1.70158f);
	//underPtitlePos.y = EaseInOutBack(titleT,720.0f,0.0f,1.70158f);

	Novice::DrawSprite(static_cast<int>(PtitlePos.x), static_cast<int>(PtitlePos.y), PtitleLogoGH, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
	//Novice::DrawSprite(static_cast<int>(underPtitlePos.x), static_cast<int>(underPtitlePos.y), PtitleLogoGH, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);

	switch (titleButton) {

	case GAME_PLAY_BUTTON:
		Novice::DrawSprite(0, 0, playChoiceGH, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
		break;

	case TUTORIAL_BUTTON:
		Novice::DrawSprite(0, 0, tutorialChoiceGH, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
		break;

	}


}

void Scene::TutorialDraw() {

	Novice::DrawBox(440, 220, 400, 280, 0.0f, RED, kFillModeSolid);
	Novice::DrawSprite(0, 0, titleBGGH, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);


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
	if (isClear) {
		Novice::ScreenPrintf(300, 0, "kuria");
	} else {
		Novice::ScreenPrintf(300, 0, "gemuoba");
	}

}

void Scene::ChargeDraw() {
	// 1. まず背景色を決定して画面全体を塗りつぶす
	if (chargeTimer < propellerEndTime) {
		// プロペラの色（暗い青系）
		Novice::DrawBox(0, 0, 1280, 720, 0.0f, 0x203744ff, kFillModeSolid);
	}
	else {
		// ブーストの色（紫系）
		Novice::DrawBox(0, 0, 1280, 720, 0.0f, 0x522f60ff, kFillModeSolid);
	}

	// 2. その上に演出の案内（箱）を重ねる
	if (chargeSubPhase == SHOW_PROPELLER_TEXT) {
		Novice::DrawBox(240, static_cast<int>(chargeTextPos.y), 800, 120, 0.0f, 0xFAFAD2FF, kFillModeSolid);
	}

	if (chargeSubPhase == SHOW_BOOST_TEXT) {
		Novice::DrawBox(240, static_cast<int>(chargeTextPos.y), 800, 120, 0.0f, 0x006400FF, kFillModeSolid);
	}

	// 3. デバッグ情報の表示
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

void Scene::DifficultySelectDraw() {
	Novice::DrawBox(0, 0, 1280, 720, 0.0f, 0x151515FF, kFillModeSolid);

	for (int i = 0; i < 3; i++) {
		int x = 140 + (i * 360); // X座標を横にずらす
		int y = 320;
		unsigned int color = 0;

		if (i == 0) color = 0x00AA00FF; // EASY
		if (i == 1) color = 0xAAAA00FF; // NORMAL
		if (i == 2) color = 0xAA0000FF; // HARD

		// 選択中の項目を強調（白枠を出す）
		if (selectedDifficulty == i) {
			Novice::DrawBox(x - 5, y - 5, 290, 130, 0.0f, WHITE, kFillModeSolid);
		}

		Novice::DrawBox(x, y, 280, 120, 0.0f, color, kFillModeSolid);
	}
}
