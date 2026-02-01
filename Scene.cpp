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
	// BGM停止
	if (Novice::IsPlayingAudio(voiceHandleTitleBGM)) {
		Novice::StopAudio(voiceHandleTitleBGM);
	}
	
	if (Novice::IsPlayingAudio(voiceHandleMainBGM)) {
		Novice::StopAudio(voiceHandleMainBGM);
	}

	if (Novice::IsPlayingAudio(voiceHandleResult)) {
		Novice::StopAudio(voiceHandleResult);
	}

	// 変数をリセット
	voiceHandleTitleBGM = -1;
	voiceHandleMainBGM = -1;
	voiceHandleResult = -1;

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

	// 目標距離
	goalDistance = 7000;

	// チャージ演出初期化
	chargeSubPhase = SHOW_PROPELLER_TEXT;
	chargeTimer = 0;
	chargeTextT = 0.0f;
	chargeTextPos = { 0.0f, TEXT_START_Y };

	// プレイヤー生成
	player = new Player();
	playerStartY = player->position.y;
	gameStartPlayerY = player->position.y;

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

	//チュートリアル
	asobikataPaper = 0;
	maxAsobikataPaper = 4;

	// 難易度設定
	difficulty = NORMAL;
	ApplyDifficulty();
	selectedDifficulty = 1;

	PtitlePos = { 0.0f,0.0f };
	titleButton = GAME_PLAY_BUTTON;
	//titleT = 0.0f;

	amplitude = 100.0f;
	theta = 0.0f;
	pressAT = 0.0f;
	pressATSpeed = 1.0f / 120.0f;

	titleBGPos[0] = {0.0f,0.0f};
	titleBGPos[1] = {1280.0f,0.0f};

	altitude = 0.0f;

	//GH
	//タイトル
	titleBGGH = Novice::LoadTexture("./Resources/images/skyBG.png");
	titleBG2GH = Novice::LoadTexture("./Resources/images/skyBG2.png");
	pressAGH = Novice::LoadTexture("./Resources/images/pressA.png");

	playChoiceGH = Novice::LoadTexture("./Resources/images/play_choice.png");
	tutorialChoiceGH = Novice::LoadTexture("./Resources/images/tutorial_choice.png");
	titleLogoGH = Novice::LoadTexture("./Resources/images/titleLogo.png");
	PtitleLogoGH = Novice::LoadTexture("./Resources/images/P.png");

	//チュートリアル
	pressAbackGH = Novice::LoadTexture("./Resources/images/pressAback.png");
	LeftArrowGH = Novice::LoadTexture("./Resources/images/LeftArrow.png");
	RightArrowGH = Novice::LoadTexture("./Resources/images/RightArrow.png");
	asobikataGH = Novice::LoadTexture("./Resources/images/asobikata.png");

	//難易度
	difficultyGH[0] = Novice::LoadTexture("./Resources/images/easy_active.png");
	difficultyGH[1] = Novice::LoadTexture("./Resources/images/normal_active.png");
	difficultyGH[2] = Novice::LoadTexture("./Resources/images/expert_active.png");
	pressAstartGH = Novice::LoadTexture("./Resources/images/pressAstart.png");

	//数字
	suuziGH[0] = Novice::LoadTexture("./Resources/images/0.png");

	// サウンド
	soundHandleSelect = Novice::LoadAudio("./Resources/sound/select.mp3");
	soundHandleDecide = Novice::LoadAudio("./Resources/sound/decide.mp3");

	soundHandleTitleBGM = Novice::LoadAudio("./Resources/sound/titleBGM.mp3");

	soundHandleMainBGM = Novice::LoadAudio("./Resources/sound/mainBGM.mp3");

	soundHandleClear = Novice::LoadAudio("./Resources/sound/clear.mp3");
	soundHandleGameOver = Novice::LoadAudio("./Resources/sound/gameover.mp3");
}




void Scene::ApplyDifficulty() {
	switch (difficulty) {
	case EASY:
		checkPoint.distance = 1500.0f;
		maxChargeTime = 1000;
		propellerEndTime = 500;
		goalDistance = 7000.0f;
		break;

	case NORMAL:
		checkPoint.distance = 2000.0f;
		maxChargeTime = 1000;
		propellerEndTime = 500;
		goalDistance = 7000.0f;
		break;

	case HARD:
		checkPoint.distance = 3000.0f;
		maxChargeTime = 900;
		propellerEndTime = 450;
		goalDistance = 9000.0f;
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

	case PAUSE:
		PauseUpdate();

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

	case PAUSE:
		MainGameDraw();
		PauseDraw();

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

// 入力処理
bool Scene::IsPressX() const {
	return (padState.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0;
}

// Xボタンが押された瞬間
bool Scene::IsTriggerX() const {
	return (padState.Gamepad.wButtons & XINPUT_GAMEPAD_X) &&
		!(prevPadState.Gamepad.wButtons & XINPUT_GAMEPAD_X);
}

// 入力処理
bool Scene::IsPressY() const {
	return (padState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0;
}

// Xボタンが押された瞬間
bool Scene::IsTriggerY() const {
	return (padState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) &&
		!(prevPadState.Gamepad.wButtons & XINPUT_GAMEPAD_Y);
}

/*------------
   更新処理ee
--------------*/
void Scene::TitleUpdate() {
	// タイトルBGM
	if (voiceHandleTitleBGM == -1 || !Novice::IsPlayingAudio(voiceHandleTitleBGM)) {
		voiceHandleTitleBGM = Novice::PlayAudio(soundHandleTitleBGM, true, 0.5f); // ループ再生
	}

	if (voiceHandleMainBGM != -1 && Novice::IsPlayingAudio(voiceHandleMainBGM)) {
		Novice::StopAudio(voiceHandleMainBGM);
		voiceHandleMainBGM = -1;
	}


	// カーソル音を再生
	if ((padState.Gamepad.sThumbLX < -10000 && prevPadState.Gamepad.sThumbLX >= -10000) ||
		(padState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT && !(prevPadState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT))) {
		Novice::PlayAudio(soundHandleSelect, false, 1.0f);
	}
	if ((padState.Gamepad.sThumbLX > 10000 && prevPadState.Gamepad.sThumbLX <= 10000) ||
		(padState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT && !(prevPadState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT))) {
		Novice::PlayAudio(soundHandleSelect, false, 1.0f);
	}

	player->oldLeftStickPos.x = player->currentLeftStickPos.x;
	Novice::GetAnalogInputLeft(0, &player->currentLeftStickPos.x, &player->currentLeftStickPos.y);
	
	//タイトルP上下用
	theta += float(M_PI) / 120.0f;
	PtitlePos.y = sinf(theta) * amplitude;


	//titleT += 0.005f;

	/*if (titleT > 1.0f) {
		titleT = 0.0f;
	}*/

	Novice::ScreenPrintf(900, 100, "%f", player->currentLeftStickPos.x);
	Novice::ScreenPrintf(900, 120, "%f", player->oldLeftStickPos.x);

	// スティック操作
	// 左
	if (player->currentLeftStickPos.x > 0.0f && player->oldLeftStickPos.x <= 0.0f) {
		switch (titleButton) {

		case Scene::GAME_PLAY_BUTTON:
			titleButton = TUTORIAL_BUTTON;
			selectedTitleMenu = 1;
			break;

		case Scene::TUTORIAL_BUTTON:
			titleButton = GAME_PLAY_BUTTON;
			selectedTitleMenu = 0;
			break;
		}

	}

	// 右
	if (player->currentLeftStickPos.x < 0.0f && player->oldLeftStickPos.x >= 0.0f) {
		switch (titleButton) {

		case Scene::GAME_PLAY_BUTTON:
			titleButton = TUTORIAL_BUTTON;
			selectedTitleMenu = 1;
			break;

		case Scene::TUTORIAL_BUTTON:
			titleButton = GAME_PLAY_BUTTON;
			selectedTitleMenu = 0;
			break;
		}
	}

	// Aボタンで決定
	if (IsTriggerA()) {
		Novice::PlayAudio(soundHandleDecide, false, 1.0f);
		
		if (selectedTitleMenu == 0) {
			gameScene = DIFFICULTY_SELECT;
		} else {
			gameScene = TUTORIAL;
		}
	}
}

void Scene::TutorialUpdate() {
	// Xボタンでタイトルへ戻る
	if (IsTriggerX()) {
		Novice::PlayAudio(soundHandleDecide, false, 1.0f);
		gameScene = TITLE;
	}

		/*if (selectedTitleMenu == 0) {
			gameScene = DIFFICULTY_SELECT;
		}
		else {
			gameScene = TUTORIAL;
		}*/

	
		pressAT += pressATSpeed;
		
		if (pressAT >= 1.0f) {
			pressAT = 1.0f;
			pressATSpeed *= -1.0f;
		}

		if (pressAT < 0.0f) {
			pressAT = 0.0f;
			pressATSpeed *= -1.0f;
		}

		for (int i = 0; i < 2; i++) {
			titleBGPos[i].x -= 1.0f;


			if (titleBGPos[i].x <= -1280.0f) {
				titleBGPos[i].x = 1280.0f;
			}
		}

	//コントローラー情報取得
	player->oldLeftStickPos.x = player->currentLeftStickPos.x;
	Novice::GetAnalogInputLeft(0, &player->currentLeftStickPos.x, &player->currentLeftStickPos.y);

	if (player->currentLeftStickPos.x > 0.0f && player->oldLeftStickPos.x <= 0.0f) {
		asobikataPaper++;
	}

	if (player->currentLeftStickPos.x < 0.0f && player->oldLeftStickPos.x >= 0.0f) {
		asobikataPaper--;
	}

	if (asobikataPaper > maxAsobikataPaper - 1) {
		asobikataPaper = maxAsobikataPaper - 1;
	}

	if (asobikataPaper < 0) {
		asobikataPaper = 0;
	}


	//雲背景

	for (int i = 0; i < 2; i++) {
		titleBGPos[i].x -= 1.0f;


		if (titleBGPos[i].x <= -1280.0f) {
			titleBGPos[i].x = 1280.0f;
		}
	}

	// Aボタンで決定
	if (IsTriggerA()) {
		Novice::PlayAudio(soundHandleDecide, false, 1.0f);
		difficulty = static_cast<Difficulty>(selectedDifficulty);
		ApplyDifficulty();
		gameScene = MAIN_GAME;
	}

}


void Scene::DifficultySelectUpdate() {
	if (voiceHandleTitleBGM == -1 || !Novice::IsPlayingAudio(voiceHandleTitleBGM)) {
		voiceHandleTitleBGM = Novice::PlayAudio(soundHandleTitleBGM, true, 0.5f);
	}

	// メインBGMが鳴っていたら止める（念のため）
	if (voiceHandleMainBGM != -1 && Novice::IsPlayingAudio(voiceHandleMainBGM)) {
		Novice::StopAudio(voiceHandleMainBGM);
		voiceHandleMainBGM = -1;
	}

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

	//文字を透明に
	pressAT += pressATSpeed;

	if (pressAT >= 1.0f) {
		pressAT = 1.0f;
		pressATSpeed *= -1.0f;
		// Xボタンでタイトルへ戻る
		if (IsTriggerX()) {
			Novice::PlayAudio(soundHandleDecide, false, 1.0f);

			Initialize();
			gameScene = TITLE;
		}

		if (pressAT < 0.0f) {
			pressAT = 0.0f;
			pressATSpeed *= -1.0f;
		}

		//背景雲移動
		for (int i = 0; i < 2; i++) {
			titleBGPos[i].x -= 1.0f;


			if (titleBGPos[i].x <= -1280.0f) {
				titleBGPos[i].x = 1280.0f;
			}
		}

		// Bボタンで決定
		if (IsTriggerB()) {
			difficulty = static_cast<Difficulty>(selectedDifficulty);
			ApplyDifficulty();

			Initialize();
			gameScene = MAIN_GAME;
			// Aボタンでメインゲームへ
			if (IsTriggerA()) {
				Novice::PlayAudio(soundHandleDecide, false, 1.0f);
				gameScene = MAIN_GAME;
			}
		}
	}
}


void Scene::MainGameUpdate() {
	// ポーズ
	if (IsTriggerY()) {
		Novice::PlayAudio(soundHandleSelect, false, 1.0f);
		gameScene = PAUSE;
		return; // ポーズに入ったらこのフレームのゲーム処理はしない
	}
	
	if (voiceHandleTitleBGM != -1 && Novice::IsPlayingAudio(voiceHandleTitleBGM)) {
		Novice::StopAudio(voiceHandleTitleBGM);
		voiceHandleTitleBGM = -1;
	}

	if (voiceHandleMainBGM == -1 || !Novice::IsPlayingAudio(voiceHandleMainBGM)) {
		voiceHandleMainBGM = Novice::PlayAudio(soundHandleMainBGM, true, 0.6f);
	}


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
	
		if (chargeTimer >= maxChargeTime) {
			player->maxPropellerPower = player->leftPropellerPower + player->rightPropellerPower;

			// デバック用リセット
			player->leftPropellerPower = 0.0f;
			player->rightPropellerPower = 0.0f;

			// 鳥の出現数設定
			birdOccurrences = checkPoint.lv;
			if (birdOccurrences <= 0) birdOccurrences = 1;
			if (birdOccurrences > 10) birdOccurrences = 10;

			for (int i = 0; i < birdOccurrences; i++) {
				bird[i]->BirdInitialize();
				bird[i]->bird.isActive = false;
			}

			// 次のフェーズへ
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

	altitude = -(player->position.y - 600.0f);

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
	progressY = gameStartPlayerY - player->position.y;

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

	//目標に到達したか
	if (progressY >= goalDistance) {
		isClear = true;       // クリア
		gameScene = RESULT;   // リザルト画面へ
		return;
	}

	// 画面外判定
	bool isOut = false;

	// 1. 左右の画面外
	// プレイヤーの幅（player->width）を考慮して、完全に消えたらアウト
	if (player->position.x + (player->width / 2.0f) < 0.0f ||
		player->position.x - (player->width / 2.0f) > 1280.0f) {
		isOut = true;
	}

	// 2. 下側の画面外（落下死）
	// player->playerScreenY は描画用の座標なので、これが 720 を超えたらアウト
	if (player->playerScreenY - (player->height / 2.0f) > 720.0f) {
		isOut = true;
	}

	if (isOut) {
		isClear = false;      // クリアフラグを折る
		gameScene = RESULT;   // リザルト画面へ
		return;               // 以降の処理をスキップ
	}

	// チェックポイント（着地判定）
	float nextCheckPointDistance = float(checkPoint.lv) * checkPoint.distance;

	if (progressY >= nextCheckPointDistance) {

		// 着地：完全停止
		player->velocity.y = 0.0f;

		//前回のチェックポイント記録
		player->position.y = gameStartPlayerY - nextCheckPointDistance;

		// 次のチェックポイント準備
		checkPoint.lv++;
		checkPoint.triggerProgressY = float(checkPoint.lv) * checkPoint.distance;

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

void Scene::PauseUpdate() {
	// 上入力
	if ((padState.Gamepad.sThumbLY > 10000 && prevPadState.Gamepad.sThumbLY <= 10000) ||
		(padState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP && !(prevPadState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP))) {
		selectedPauseMenu--;
		Novice::PlayAudio(soundHandleSelect, false, 1.0f);
	}

	// 下入力
	if ((padState.Gamepad.sThumbLY < -10000 && prevPadState.Gamepad.sThumbLY >= -10000) ||
		(padState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN && !(prevPadState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN))) {
		selectedPauseMenu++;
		Novice::PlayAudio(soundHandleSelect, false, 1.0f);
	}

	if (selectedPauseMenu < 0) selectedPauseMenu = 2;
	if (selectedPauseMenu > 2) selectedPauseMenu = 0;

	// Aボタンで決定
	if (IsTriggerA()) {
		Novice::PlayAudio(soundHandleDecide, false, 1.0f);

		if (selectedPauseMenu == 0) {
			gameScene = MAIN_GAME;
		}
		else if (selectedPauseMenu == 1) {
			Initialize();
			gameScene = MAIN_GAME;
		}
		else if (selectedPauseMenu == 2) {
			// ★ポイント：一度InitializeでメインBGMを止める
			Initialize();
			// Initialize() 内で gameScene = TITLE になっているので、
			// 難易度選択画面にしたい場合はここで上書きする
			gameScene = DIFFICULTY_SELECT;
		}
	}
	// Yボタンでゲームに戻る
	if (IsTriggerY()) {
		gameScene = MAIN_GAME;
	}
}

void Scene::ResultUpdate() {
	if (voiceHandleMainBGM != -1) {
		Novice::StopAudio(voiceHandleMainBGM);
		voiceHandleMainBGM = -1;
	}

	if (voiceHandleResult == -1 || !Novice::IsPlayingAudio(voiceHandleResult)) {
		if (isClear) {
			// クリア時（ループさせる場合は true、1回なら false）
			voiceHandleResult = Novice::PlayAudio(soundHandleClear, false, 0.5f);
		}
		else {
			// ゲームオーバー時
			voiceHandleResult = Novice::PlayAudio(soundHandleGameOver, false, 0.5f);
		}
	}

	// Aボタンでタイトルへ
	if (IsTriggerA()) {
		Novice::PlayAudio(soundHandleDecide, false, 1.0f);
		
		if (Novice::IsPlayingAudio(voiceHandleResult)) {
			Novice::StopAudio(voiceHandleResult);
		}
		
		Initialize(); // 全てをリセットしてタイトルへ
	}

}


/*------------
   描画処理
--------------*/
void Scene::TitleDraw() {

	Novice::DrawSprite(static_cast<int>(titleBGPos[0].x), static_cast<int>(titleBGPos[0].y), titleBGGH, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
	Novice::DrawSprite(static_cast<int>(titleBGPos[1].x), static_cast<int>(titleBGPos[1].y), titleBG2GH, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);

	Novice::DrawSprite(0, 0, pressAGH, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF - int(pressAT * 255.0f));

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
	Novice::ScreenPrintf(300, 300, "%f", pressAT);


}

void Scene::TutorialDraw() {

	Novice::DrawBox(440, 220, 400, 280, 0.0f, RED, kFillModeSolid);
	Novice::DrawSprite(static_cast<int>(titleBGPos[0].x), static_cast<int>(titleBGPos[0].y), titleBGGH, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
	Novice::DrawSprite(static_cast<int>(titleBGPos[1].x), static_cast<int>(titleBGPos[1].y), titleBG2GH, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);

	Novice::DrawSprite(0, 0, pressAbackGH, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
	Novice::DrawSprite(0, 0, asobikataGH, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);

	if (asobikataPaper != 0) {
		Novice::DrawSprite(0, 0, LeftArrowGH, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
	}

	if (asobikataPaper != maxAsobikataPaper-1) {
		Novice::DrawSprite(0, 0, RightArrowGH, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
	}



	for (int i = 0; i < maxAsobikataPaper; i++) {
		Novice::DrawSprite( 200 +(i * 1055) - (asobikataPaper * 1055 ), 150, whiteTextureHandle, 880, 420, 0.0f, 0xFFFFFFFF);
	}

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

	Novice::DrawBox(900, 20, 360, 120, 0.0f, 0xffffffff, kFillModeSolid);
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

	// 目標距離
	Novice::ScreenPrintf(300, 160, "CURRENT: %f / GOAL: %f", progressY, goalDistance);

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



	for (int i = 0; i < 5; i++) {
		Novice::DrawSprite(20 + (50 * i), 20, suuziGH[0], 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
	}

	Novice::DrawSprite(270 + 10, 20, suuziGH[0], 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);


}

void Scene::DifficultySelectDraw() {
	//Novice::DrawBox(0, 0, 1280, 720, 0.0f, 0x151515FF, kFillModeSolid);

	Novice::DrawSprite(static_cast<int>(titleBGPos[0].x), static_cast<int>(titleBGPos[0].y), titleBGGH, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
	Novice::DrawSprite(static_cast<int>(titleBGPos[1].x), static_cast<int>(titleBGPos[1].y), titleBG2GH, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
	Novice::DrawBox(0, 0, 1280, 720, 0.0f, 0x00000077, kFillModeSolid);

	Novice::DrawSprite(0, 0, pressAstartGH, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF - int(pressAT * 255.0f));
	Novice::DrawSprite(0, 0, pressAbackGH, 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
	//Novice::DrawBox(0, 0, 1280, 720, 0.0f, 0x151515FF, kFillModeSolid);

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

void Scene::PauseDraw() {
	// 画面を暗くする半透明のフィルター
	Novice::DrawBox(0, 0, 1280, 720, 0.0f, 0x00000088, kFillModeSolid);

	Novice::ScreenPrintf(600, 200, "PAUSE");

	// メニュー項目
	const char* menuTexts[] = { "RESUME", "RESTART", "DIFFICULTY SELECT" };
	for (int i = 0; i < 3; i++) {
	
		// 選択中の項目を強調（白枠を出す）
		if (selectedDifficulty == i) {
			Novice::DrawSprite(0, 0, difficultyGH[i], 1.0f, 1.0f, 0.0f, 0xFFFFFFFF);
			if (selectedPauseMenu == i) {
				Novice::DrawBox(530, 300 + (i * 40), 10, 10, 0.0f, WHITE, kFillModeSolid);
			}
		}

	}
}