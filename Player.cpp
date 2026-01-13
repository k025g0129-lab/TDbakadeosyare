#include<Novice.h>
#include<cmath>
#include <corecrt_math.h>
#define USE_MATH_DEFINES
#include <corecrt_math_defines.h>
#include "Player.h"
#include<Xinput.h>

Player::Player() {
	// 座標　加速度　速度
	position = { 200.0f,300.0f };
	accerelation = { 0.0f,0.0f };
	velocity = { 0.0f,0.0f };

	// 左スティック座標
	oldLeftStickPos = { 0,0 }; // 1f前
	currentLeftStickPos = { 0,0 }; // 現在

	// 右スティック座標
	oldRightStickPos = { 0,0 }; // 1f前
	currentRightStickPos = { 0,0 }; // 現在

	// 左スティックの傾き
	oldLeftAngle = 0.0f; // 1f前
	currentLeftAngle = 0.0f; // 現在

	// 右スティックの傾き
	oldRightAngle = 0.0f; // 1f前
	currentRightAngle = 0.0f; // 現在

	// 左右回転度計測
	totalLeftRotation = 0.0f;
	totalRightRotation = 0.0f;

	// 左右プロペラパワー
	leftPropellerPower = 0.0f;
	rightPropellerPower = 0.0f;

	speed = 5.0f; // 速度
	boost = 0.0f; // ブースト

	angle = 0.0f; // 傾き
	powerDiff = 0.0f; // 左右差
	angleFacter = 0.007f; // 傾きの係数



	// ----------------  本番では使わず、コードができているか確認するために使う変数  --------------------

	timer_trial = 630; // プログラム開始時のラグを考慮した10秒間
	scene_trial = 1; // シーン

	// ------------------------------------------------------------------------------------------
}

Player::~Player() {};

void Player::Update(int scene) {

	if (timer_trial != 0) {
		timer_trial--;
	}

	if (timer_trial <= 0) {
		scene_trial = 3;
		powerDiff = leftPropellerPower - rightPropellerPower; // 正なら右側へ、負なら左側へ傾くことになる
	}

	switch (scene) {
	case 0:
		// タイトル
		break;

	case 1:

		// プロペラチャージ
#pragma region Left
		// 更新する前の座標を保存
		oldLeftStickPos = currentLeftStickPos;
		// 左スティック操作
		Novice::GetAnalogInputLeft(0, &currentLeftStickPos.x, &currentLeftStickPos.y);

		// 1f前から座標が変化しているならtrue
		if ((oldLeftStickPos.x != currentLeftStickPos.x) || (oldLeftStickPos.y != currentLeftStickPos.y)) {
			// 正規化(norm = Normalizedの略)
			float leftNormX = (static_cast<float>(currentLeftStickPos.x) - 32768.0f) / 32768.0f;
			float leftNormY = (static_cast<float>(currentLeftStickPos.y) - 32768.0f) / 32768.0f;

			// 現在の角度
			currentLeftAngle = atan2f(leftNormY, leftNormX);

			// 中心からどれだけスティックを倒しているかの距離
			float length = sqrtf(leftNormX * leftNormX + leftNormY * leftNormY);

			// 一定距離以上で判定開始
			if (length > 0.3f) {
				// 1. 差分を出す
				float diff = currentLeftAngle - oldLeftAngle;

				// 2. 補正
				if (diff > 3.14159f) diff -= 2.0f * 3.14159f;
				if (diff < -3.14159f) diff += 2.0f * 3.14159f;

				// 3. 【修正】左回り（反時計回り）は diff がマイナスになる
				if (diff < 0) {
					// totalLeftRotation はプラスで溜めていきたいので、マイナスの diff を引く（＝プラスにする）
					totalLeftRotation += fabsf(diff);
				}

				// 4. 1周判定
				if (totalLeftRotation >= 1.2f) {
					leftPropellerPower += 1.0f;
					totalLeftRotation -= 1.2f;
				}

				// 5. 【ここに移動】倒している間だけ、前回の角度を更新する
				oldLeftAngle = currentLeftAngle;
			} else {
				// スティックを離した時は、次に倒した時に「変なジャンプ」が起きないよう
				// 今の角度を old に逃がしておく処理を入れるとより安全です
				oldLeftAngle = currentLeftAngle;
			}
		}
		
#pragma endregion

#pragma region Right
		// 更新する前の座標を保存
		oldRightStickPos = currentRightStickPos;

		// 右スティック操作
		Novice::GetAnalogInputRight(0, &currentRightStickPos.x, &currentRightStickPos.y);

		// 1f前から座標が変化しているならtrue
		if ((oldRightStickPos.x != currentRightStickPos.x) || (oldRightStickPos.y != currentRightStickPos.y)) {
			// 正規化(norm = Normalizedの略)
			float rightNormX = (static_cast<float>(currentRightStickPos.x) - 32768.0f) / 32768.0f;
			float rightNormY = (static_cast<float>(currentRightStickPos.y) - 32768.0f) / 32768.0f;

			// 現在の角度
			currentRightAngle = atan2f(rightNormY, rightNormX);

			// 中心からどれだけスティックを倒しているかの距離
			float length = sqrtf(rightNormX * rightNormX + rightNormY * rightNormY);

			// 一定距離以上で判定開始
			if (length > 0.3f) {
				// 1. 差分を出す
				float diff = currentRightAngle - oldRightAngle;

				// 2. 補正
				if (diff > 3.14159f) diff -= 2.0f * 3.14159f;
				if (diff < -3.14159f) diff += 2.0f * 3.14159f;

				// 3. 【修正】右回り
				if (diff > 0) {
					// totalRightRotation はプラスで溜めていきたいので、マイナスの diff を引く（＝プラスにする）
					totalRightRotation += fabsf(diff);
				}

				// 4. 1周判定
				if (totalRightRotation >= 1.2f) {
					rightPropellerPower += 1.0f;
					totalRightRotation -= 1.2f;
				}

				// 5. 【ここに移動】倒している間だけ、前回の角度を更新する
				oldRightAngle = currentRightAngle;
			} else {
				// スティックを離した時は、次に倒した時に「変なジャンプ」が起きないよう
				// 今の角度を old に逃がしておく処理を入れるとより安全です
				oldRightAngle = currentRightAngle;
			}
		}
		
#pragma endregion

		break;

	case 2:
		// ブーストチャージ
		// L2押されたとき
		if (Novice::IsTriggerButton(0, PadButton::kPadButton10)) {
			boost += 0.4f;
		}

		// R2押されたとき
		if (Novice::IsTriggerButton(0, PadButton::kPadButton11)) {
			boost += 0.4f;
		}

		break;

	case 3:
		// プレイ中

		// 毎フレーム左右の差によって傾きを追加
		angle += powerDiff * angleFacter;
		break;
	}
}

void Player::Draw() {
	Novice::ScreenPrintf(0, 0, "currentLeftStickPos.x = %d", currentLeftStickPos.x);
	Novice::ScreenPrintf(0, 20, "currentLeftStickPos.y = %d", currentLeftStickPos.y);

	Novice::ScreenPrintf(0, 60, "currentRightStickPos.x = %d", currentRightStickPos.x);
	Novice::ScreenPrintf(0, 80, "currentRightStickPos.y = %d", currentRightStickPos.y);

	Novice::ScreenPrintf(0, 120, "currentLeftAngle = %f", currentLeftAngle);
	Novice::ScreenPrintf(0, 140, "currentRightAngle = %f", currentRightAngle);

	Novice::ScreenPrintf(0, 180, "totalLeftRotation = %f", totalLeftRotation);
	Novice::ScreenPrintf(0, 200, "totalRightRotation = %f", totalRightRotation);

	Novice::ScreenPrintf(0, 240, "leftPropellerPower = %f", leftPropellerPower);
	Novice::ScreenPrintf(0, 260, "rightPropellerPower = %f", rightPropellerPower);
}