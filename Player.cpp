#include<Novice.h>
#include<cmath>
#include "Player.h"
#include "Function.h"

Player::Player() {
	position = { 200.0f,300.0f };
	accerelation = { 0.0f,0.0f };
	velocity = { 0.0f,0.0f };

	oldLeftStickPos = { 0,0 };
	currentLeftStickPos = { 0,0 };

	oldRightStickPos = { 0,0 };
	currentRightStickPos = { 0,0 };

	oldLeftAngle = 0.0f;
	currentLeftAngle = 0.0f;

	oldRightAngle = 0.0f;
	currentRightAngle = 0.0f;

	totalLeftRotation = 0.0f;
	totalRightRotation = 0.0f;

	leftPropellerPower = 0.0f;
	rightPropellerPower = 0.0f;

	speed = 5.0f;
	boost = 0.0f;

	angle = 0.0f;


	//見た目
	planeLocalCenterPos = { 0.0f,0.0f };
	width = 100.0f;
	height = 300.0f;

	planeLocalFourCornersPos[0] = { planeLocalCenterPos.x - width / 2, planeLocalCenterPos.y - height / 2 };
	planeLocalFourCornersPos[1] = { planeLocalCenterPos.x + width / 2, planeLocalCenterPos.y - height / 2 };
	planeLocalFourCornersPos[2] = { planeLocalCenterPos.x - width / 2, planeLocalCenterPos.y + height / 2 };
	planeLocalFourCornersPos[3] = { planeLocalCenterPos.x + width / 2, planeLocalCenterPos.y + height / 2 };


	planeWorldPos = { 640.0f,720.0f };

	planeWorldFourCornersPos[0] = Vector2Add(planeLocalFourCornersPos[0], planeWorldPos);
	planeWorldFourCornersPos[1] = Vector2Add(planeLocalFourCornersPos[1], planeWorldPos);
	planeWorldFourCornersPos[2] = Vector2Add(planeLocalFourCornersPos[2], planeWorldPos);
	planeWorldFourCornersPos[3] = Vector2Add(planeLocalFourCornersPos[3], planeWorldPos);

	whiteTextureHandle = Novice::LoadTexture("./NoviceResources/white1x1.png");
}

Player::~Player() {};

void Player::Update(int scene) {

	switch (scene) {
	case 0:
		// タイトル
		break;

	case 1:

		// プロペラチャージ
#pragma region Left
		// 更新する前の座標を保存
		oldRightStickPos = currentRightStickPos;
		// 左スティック操作
		Novice::GetAnalogInputLeft(0, &currentLeftStickPos.x, &currentLeftStickPos.y);

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
				if (totalLeftRotation >= 6.28318f) {
					leftPropellerPower += 1.0f;
					totalLeftRotation -= 6.28318f;
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
				if (totalRightRotation >= 6.28318f) {
					rightPropellerPower += 1.0f;
					totalRightRotation -= 6.28318f;
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

		Novice::GetAnalogInputLeft(0, &currentLeftStickPos.x, &currentLeftStickPos.y);

		if (currentLeftStickPos.x > 0) {
			planeWorldPos.x += 1.0f;
		}

		if (currentLeftStickPos.x < 0) {
			planeWorldPos.x -= 1.0f;
		}

		planeWorldFourCornersPos[0] = Vector2Add(planeLocalFourCornersPos[0], planeWorldPos);
		planeWorldFourCornersPos[1] = Vector2Add(planeLocalFourCornersPos[1], planeWorldPos);
		planeWorldFourCornersPos[2] = Vector2Add(planeLocalFourCornersPos[2], planeWorldPos);
		planeWorldFourCornersPos[3] = Vector2Add(planeLocalFourCornersPos[3], planeWorldPos);

		Draw();

		break;
	}
}

void Player::Draw() {
	Novice::DrawQuad(
		static_cast<int>(planeWorldFourCornersPos[0].x), static_cast<int>(planeWorldFourCornersPos[0].y),
		static_cast<int>(planeWorldFourCornersPos[1].x), static_cast<int>(planeWorldFourCornersPos[1].y),
		static_cast<int>(planeWorldFourCornersPos[2].x), static_cast<int>(planeWorldFourCornersPos[2].y),
		static_cast<int>(planeWorldFourCornersPos[3].x), static_cast<int>(planeWorldFourCornersPos[3].y),


		static_cast<int>(planeWorldFourCornersPos[0].x), static_cast<int>(planeWorldFourCornersPos[0].y),
		static_cast<int>(width), static_cast<int>(height),

		whiteTextureHandle,
		0xFFFFFFFF
	);

	Novice::ScreenPrintf(0, 0, "%f", planeWorldFourCornersPos[0].x);
}
