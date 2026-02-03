#include<Novice.h>
#include<cmath>
#include <corecrt_math.h>
#define USE_MATH_DEFINES
#include <corecrt_math_defines.h>
#include "Player.h"
#include "Function.h"
#include<Xinput.h>

Player::Player() {
	// 座標　加速度　速度
	position = { 640.0f,600.0f };
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

	maxPropellerPower = 0.0f;

	speed = { 0.0f,0.0f }; // 速度
	upValue = 0.0f; // プロペラパワー[ ? ]により上昇する量
	boostGauge = 0.0f; // ブースト
	boostPower = 1.0f; // ブースト消費により速度を上げる係数

	angle = 0.0f; // 傾き
	powerDiff = 0.0f; // 左右差
	angleFacter = 0.00007f; // 傾きの係数

	playerScreenY=0.0f;

	// 残量割合を保管
	rightPropellerPercentage=0.0f;
	leftPropellerPercentage = 0.0f;
	boostGaugePercentage = 0.0f;

	// 描画時のYスケールを保存する変数
	rightPropGaugeScaleY = 0.0f;
	leftPropGaugeScaleY = 0.0f;
	boostGaugeScaleY = 0.0f;

	// ----------------  本番では使わず、コードができているか確認するために使う変数  --------------------

	

	// ------------------------------------------------------------------------------------------
	angle = 0.0f;


	//見た目
	planeLocalCenterPos = { 0.0f,0.0f };
	width = 100.0f;
	height = 100.0f;

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

	// 画像読み込み
	// ゲージ関連グラフハンドル
	rightPropBarGH = Novice::LoadTexture("./Resources/images/propBar_R.png");
	leftPropBarGH = Novice::LoadTexture("./Resources/images/propBar_L.png");
	propGaugeGH = Novice::LoadTexture("./Resources/images/propGauge.png");

	boostBarGH = Novice::LoadTexture("./Resources/images/boostBar.png");
	boostGaugeGH = Novice::LoadTexture("./Resources/images/boostGauge.png");

	// プロペラが両方残ってる時
	normalGH[0] = Novice::LoadTexture("./Resources/images/player1.png");
	normalGH[1] = Novice::LoadTexture("./Resources/images/player2.png");
	normalGH[2] = Novice::LoadTexture("./Resources/images/player3.png");
	normalGH[3] = Novice::LoadTexture("./Resources/images/player4.png");
	normalGH[4] = Novice::LoadTexture("./Resources/images/player5.png");
	normalGH[5] = Novice::LoadTexture("./Resources/images/player6.png");

	// 右のみ
	rightOnlyGH[0] = Novice::LoadTexture("./Resources/images/rightOnly_player1.png");
	rightOnlyGH[1] = Novice::LoadTexture("./Resources/images/rightOnly_player2.png");
	rightOnlyGH[2] = Novice::LoadTexture("./Resources/images/rightOnly_player3.png");
	rightOnlyGH[3] = Novice::LoadTexture("./Resources/images/rightOnly_player4.png");
	rightOnlyGH[4] = Novice::LoadTexture("./Resources/images/rightOnly_player5.png");
	rightOnlyGH[5] = Novice::LoadTexture("./Resources/images/rightOnly_player6.png");

	// 左のみ
	leftOnlyGH[0] = Novice::LoadTexture("./Resources/images/leftOnly_player1.png");
	leftOnlyGH[1] = Novice::LoadTexture("./Resources/images/leftOnly_player2.png");
	leftOnlyGH[2] = Novice::LoadTexture("./Resources/images/leftOnly_player3.png");
	leftOnlyGH[3] = Novice::LoadTexture("./Resources/images/leftOnly_player4.png");
	leftOnlyGH[4] = Novice::LoadTexture("./Resources/images/leftOnly_player5.png");
	leftOnlyGH[5] = Novice::LoadTexture("./Resources/images/leftOnly_player6.png");

	// 両方終わったとき
	stopGH[0] = Novice::LoadTexture("./Resources/images/stopProp_player1.png");
	stopGH[1] = Novice::LoadTexture("./Resources/images/stopProp_player2.png");
	stopGH[2] = Novice::LoadTexture("./Resources/images/stopProp_player3.png");
	stopGH[3] = Novice::LoadTexture("./Resources/images/stopProp_player4.png");
	stopGH[4] = Novice::LoadTexture("./Resources/images/stopProp_player5.png");
	stopGH[5] = Novice::LoadTexture("./Resources/images/stopProp_player6.png");

	// アニメカウント
	animCount = 0;

	// 何番目の画像かを指定する変数
	GHindex = 0;

}

Player::~Player() {};

void Player::Update_charge_propeller() {


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
				if (leftPropellerPower < MAX_LEFT_POWER) {
					leftPropellerPower += 1.0f;
				}

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

	// キーボード操作バージョン
	if (keys[DIK_A] && !preKeys[DIK_A]) {
		if (leftPropellerPower < MAX_LEFT_POWER) {
			leftPropellerPower += 0.5f;
		}
	}

	if (keys[DIK_D] && !preKeys[DIK_D]) {
		if (leftPropellerPower < MAX_LEFT_POWER) {
			leftPropellerPower += 0.5f;
		}
	}

	// 上限値を超えるとき上限値で固定
	if (leftPropellerPower >= MAX_LEFT_POWER) {
		leftPropellerPower = MAX_LEFT_POWER;
	}

	// 左プロペラパワーの残量割合を計算
	leftPropellerPercentage = leftPropellerPower / MAX_RIGHT_POWER;

	// ゲージ描画時のYスケールを計算
	leftPropGaugeScaleY = leftPropellerPercentage;

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
			if (totalRightRotation >= 1.0f) {
				if (rightPropellerPower < MAX_RIGHT_POWER) {
					rightPropellerPower += 1.0f;
				}

				totalRightRotation -= 1.0f;
			}

			// 5. 【ここに移動】倒している間だけ、前回の角度を更新する
			oldRightAngle = currentRightAngle;
		} else {
			// スティックを離した時は、次に倒した時に「変なジャンプ」が起きないよう
			// 今の角度を old に逃がしておく処理を入れるとより安全です
			oldRightAngle = currentRightAngle;
		}
	}

	// キーボード操作バージョン
	if (keys[DIK_LEFT] && !preKeys[DIK_LEFT]) {
		if (rightPropellerPower < MAX_RIGHT_POWER) {
			rightPropellerPower += 0.5f;
		}
	}

	if (keys[DIK_RIGHT] && !preKeys[DIK_RIGHT]) {
		if (rightPropellerPower < MAX_RIGHT_POWER) {
			rightPropellerPower += 0.5f;
		}
	}

	// 上限値を超えるとき上限値で固定
	if (rightPropellerPower >= MAX_RIGHT_POWER) {
		rightPropellerPower = MAX_RIGHT_POWER;
	}

	// 右プロペラパワーの残量割合を計算
	rightPropellerPercentage = rightPropellerPower / MAX_LEFT_POWER;

	// ゲージ描画時のYスケールを計算
	rightPropGaugeScaleY = rightPropellerPercentage;

#pragma endregion
}

void Player::Update_charge_boost() {
	

	// ブーストチャージ
	// L2押されたとき
	if (Novice::IsTriggerButton(0, PadButton::kPadButton10)) {
		if (boostGauge < MAX_BOOST_GAUGE) {
			boostGauge += 0.4f;
		}
	}

	// R2押されたとき
	if (Novice::IsTriggerButton(0, PadButton::kPadButton11)) {
		if (boostGauge < MAX_BOOST_GAUGE) {
			boostGauge += 0.4f;
		}
	}

	// キーボード操作バージョン
	if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
		boostGauge += 0.6f;
	}

	// 上限値を超えるとき上限値で固定
	if (boostGauge >= MAX_BOOST_GAUGE) {
		boostGauge = MAX_BOOST_GAUGE;
	}

	// ブーストゲージの残量割合を計算
	boostGaugePercentage = boostGauge / MAX_BOOST_GAUGE;

	// ゲージ描画時のYスケールを計算
	boostGaugeScaleY = boostGaugePercentage;
}

void Player::Update_play() {

	// プレイ中

	// 毎フレーム左右のプロペラパワーを減少
	if (rightPropellerPower > 0.0f) {
		rightPropellerPower -= 0.025f;
	}

	if (leftPropellerPower > 0.0f) {
		leftPropellerPower -= 0.025f;
	}

	// 0以下で0に固定
	if (rightPropellerPower < 0.0f) {
		rightPropellerPower = 0.0f;
	}

	if (leftPropellerPower < 0.0f) {
		leftPropellerPower = 0.0f;
	}

	// 左右のプロペラ残量の差(傾きに使う)
	powerDiff = leftPropellerPower - rightPropellerPower;
	// 上昇量の管理 ------------------------------------------------------------------------

	// 徐々に数値が上がり、上限値で固定(1fあたりの上昇量は全ステージ固定)
	if (upValue < MAX_UP_VALUE) {
		upValue += 0.392f;

		if (upValue >= MAX_UP_VALUE) {
			upValue = MAX_UP_VALUE;
		}
	}

	// *  if  *  左右どちらも死んだとき、上昇量を減らし続ける
	// * else *  左右どちらかのプロペラが死んだとき、上昇量を徐々に減らし、上限値の半分に固定
	if ((rightPropellerPower <= 0.0f) && (leftPropellerPower <= 0.0f)) {
		upValue -= 0.5f;
	} else if ((rightPropellerPower <= 0.0f) || (leftPropellerPower <= 0.0f)) {
		if (upValue > MAX_UP_VALUE / 2.0f) {
			upValue -= 0.188f;

			if (upValue <= MAX_UP_VALUE / 2.0f) {
				upValue = MAX_UP_VALUE / 2.0f;
			}
		}
	}
	
	// ------------------------------------------------------------------------------------

	// 傾く量の管理 --------------------------------------------------------------------------

	// 右のプロペラが尽きた時、右側へ大きく傾き続ける
	if (rightPropellerPower <= 0.0f) {
		angle += (powerDiff) * (angleFacter + 0.0005f);
	}

	// 左のプロペラが尽きた時、左側へ大きく傾き続ける
	if (leftPropellerPower <= 0.0f) {
		angle -= (powerDiff) * (angleFacter + 0.0005f);
	}

	// ------------------------------------------------------------------------------------

#pragma region ブーストの操作
	// ブーストゲージ0超過で実行
	// *  if  * R2 L2同時押しでブーストゲージを消費しboostPowerを設定
	// * else * boostPowerを1に固定
	if (boostGauge > 0.0f) {
		if(leftPropellerPower > 0.0f || rightPropellerPower > 0.0f)
		if ((Novice::IsPressButton(0, PadButton::kPadButton10) && Novice::IsPressButton(0, PadButton::kPadButton11)) || keys[DIK_SPACE]) {
			boostGauge -= 0.12f;

			// 現在の燃料の割合 (1.0 ～ 0.0)
			float fuelRatio = 0.0f;
			if (maxPropellerPower > 0.0f) {
				fuelRatio = (leftPropellerPower + rightPropellerPower) / maxPropellerPower;
			}

			// 1. パワーの計算：fuelRatioを「掛ける」のではなく「少し足す」イメージにする
			// // 残量が多いほど強いのは維持しつつ、最低限のパワー（1.5倍など）を保証する
			float baseBoost = 1.5f;
			boostPower = baseBoost + (2.0f * fuelRatio);

			// 2. コストの計算：もう少しマイルドにする（今のままだと減りが早すぎるかも）
			float penaltyCost = 0.04f * fuelRatio;

			leftPropellerPower -= (0.02f + penaltyCost);
			rightPropellerPower -= (0.02f + penaltyCost);

			Novice::ScreenPrintf(0, 280, "raito = %f", fuelRatio);
			Novice::ScreenPrintf(0, 300, "pena = %f", penaltyCost);

			
			if (boostGauge <= 0.0f) {
				boostGauge = 0.0f;
			}

		} else {
			boostPower = 1.0f;
		}
	}

	if (boostGauge <= 0.0f) {
		boostPower = 1.0f;
	}

#pragma endregion

	// 上昇処理
	if (boostPower >= 1.0f) {
		speed.x = sinf(angle) * upValue * (boostPower / 2.0f);
		speed.y = -cosf(angle) * upValue * boostPower;
	} else if (boostPower < 1.0f){
		speed.x = sinf(angle) * upValue + (4.0f * boostPower);
		speed.y = -cosf(angle) * upValue + (4.0f * boostPower);
	}

	position.x += speed.x;
	position.y += speed.y;

	// 毎フレーム左右の差によって傾きを追加
	angle += powerDiff * angleFacter;

#pragma region 手動移動
	// 左スティックの座標を取得
	Novice::GetAnalogInputLeft(0, &currentLeftStickPos.x, &currentLeftStickPos.y);

	// 右移動
	if (currentLeftStickPos.x > 100) {
		velocity.x += 0.002f;
	} else if (keys[DIK_D] || keys[DIK_RIGHT]) {
		velocity.x += 0.002f;
	}

	// 左
	if (currentLeftStickPos.x < -100) {
		velocity.x -= 0.002f;
	} else if (keys[DIK_A] || keys[DIK_LEFT]) {
		velocity.x -= 0.002f;
	}

	// positionを変動
	position.x += 2.0f * velocity.x;

	// 動いていないとき、velocityが0になるまで徐々に減らす
	if ((currentLeftStickPos.x <= 100 && currentLeftStickPos.x >= -100) && (!keys[DIK_A] && !keys[DIK_LEFT] && !keys[DIK_D] && !keys[DIK_RIGHT])) {
		if (velocity.x != 0.0f) {

			// 0超過のとき
			if (velocity.x > 0.0f) {
				velocity.x -= 0.004f;

				if (velocity.x < 0.0f) {
					velocity.x = 0.0f;
				}
			}

			// 0未満の時
			if (velocity.x < 0.0f) {
				velocity.x += 0.004f;

				if (velocity.x > 0.0f) {
					velocity.x = 0.0f;
				}
			}
		}
	}

#pragma endregion

	// 3. 傾きの更新
	angle += powerDiff * angleFacter;

	// 4. 最後に「集約された position」を使って全頂点を一括計算
	Vector2 rotatedCorners[4]{};
	for (int i = 0; i < 4; ++i) {
		// ローカル座標を回転
		rotatedCorners[i].x = planeLocalFourCornersPos[i].x * cosf(angle) - planeLocalFourCornersPos[i].y * sinf(angle);
		rotatedCorners[i].y = planeLocalFourCornersPos[i].x * sinf(angle) + planeLocalFourCornersPos[i].y * cosf(angle);

		// 回転した頂点に「共通の position」を足してワールド座標にする
		planeWorldFourCornersPos[i] = Vector2Add(rotatedCorners[i], position);
	}

	// アニメカウントの計算
	if (animCount < kMaxAnimCount) {
		animCount++;
	} else {
		animCount = 0;
	}

	// 画像指定変数に代入
	GHindex = (int)(animCount / 40);

	// 各パワーの残量割合を計算
	rightPropellerPercentage = rightPropellerPower / MAX_RIGHT_POWER;
	leftPropellerPercentage = leftPropellerPower / MAX_LEFT_POWER;
	boostGaugePercentage = boostGauge / MAX_BOOST_GAUGE;

	// ゲージ描画時のYスケールを計算
	rightPropGaugeScaleY = rightPropellerPercentage;
	leftPropGaugeScaleY = leftPropellerPercentage;
	boostGaugeScaleY = boostGaugePercentage;
}

void Player::Draw(float finalY) {

	// 画面上の理想の位置(finalY) と 物理的な座標(position.y) の差を出す
	float offsetY = finalY - position.y;

	Novice::DrawQuad(
		static_cast<int>(planeWorldFourCornersPos[0].x),
		static_cast<int>(planeWorldFourCornersPos[0].y + offsetY), // offsetYを足す
		static_cast<int>(planeWorldFourCornersPos[1].x),
		static_cast<int>(planeWorldFourCornersPos[1].y + offsetY),
		static_cast<int>(planeWorldFourCornersPos[2].x),
		static_cast<int>(planeWorldFourCornersPos[2].y + offsetY),
		static_cast<int>(planeWorldFourCornersPos[3].x),
		static_cast<int>(planeWorldFourCornersPos[3].y + offsetY),
		0, 0, (int)width, (int)height,
		whiteTextureHandle, 0xFFFFFFFF
	);

	// 自機描画
	if ((leftPropellerPower > 0.0f) && (rightPropellerPower > 0.0f)) {  // どっちも残ってるとき
		Novice::DrawQuad(
			(int)(planeWorldFourCornersPos[0].x),
			(int)(planeWorldFourCornersPos[0].y + offsetY),
			(int)(planeWorldFourCornersPos[1].x),
			(int)(planeWorldFourCornersPos[1].y + offsetY),
			(int)(planeWorldFourCornersPos[2].x),
			(int)(planeWorldFourCornersPos[2].y + offsetY),
			(int)(planeWorldFourCornersPos[3].x),
			(int)(planeWorldFourCornersPos[3].y + offsetY),
			0, 0, (int)width, (int)height,
			normalGH[GHindex], 0xFFFFFFFF
		);

	} else if ((leftPropellerPower <= 0.0f) && (rightPropellerPower > 0.0f)) {  // 右のみ
		Novice::DrawQuad(
			(int)(planeWorldFourCornersPos[0].x),
			(int)(planeWorldFourCornersPos[0].y + offsetY),
			(int)(planeWorldFourCornersPos[1].x),
			(int)(planeWorldFourCornersPos[1].y + offsetY),
			(int)(planeWorldFourCornersPos[2].x),
			(int)(planeWorldFourCornersPos[2].y + offsetY),
			(int)(planeWorldFourCornersPos[3].x),
			(int)(planeWorldFourCornersPos[3].y + offsetY),
			0, 0, (int)width, (int)height,
			rightOnlyGH[GHindex], 0xFFFFFFFF
		);

	} else if ((leftPropellerPower > 0.0f) && (rightPropellerPower <= 0.0f)) {  // 左のみ
		Novice::DrawQuad(
			(int)(planeWorldFourCornersPos[0].x),
			(int)(planeWorldFourCornersPos[0].y + offsetY),
			(int)(planeWorldFourCornersPos[1].x),
			(int)(planeWorldFourCornersPos[1].y + offsetY),
			(int)(planeWorldFourCornersPos[2].x),
			(int)(planeWorldFourCornersPos[2].y + offsetY),
			(int)(planeWorldFourCornersPos[3].x),
			(int)(planeWorldFourCornersPos[3].y + offsetY),
			0, 0, (int)width, (int)height,
			leftOnlyGH[GHindex], 0xFFFFFFFF
		);

	} else if ((leftPropellerPower <= 0.0f) && (rightPropellerPower <= 0.0f)) {  // どっちもない
		Novice::DrawQuad(
			(int)(planeWorldFourCornersPos[0].x),
			(int)(planeWorldFourCornersPos[0].y + offsetY),
			(int)(planeWorldFourCornersPos[1].x),
			(int)(planeWorldFourCornersPos[1].y + offsetY),
			(int)(planeWorldFourCornersPos[2].x),
			(int)(planeWorldFourCornersPos[2].y + offsetY),
			(int)(planeWorldFourCornersPos[3].x),
			(int)(planeWorldFourCornersPos[3].y + offsetY),
			0, 0, (int)width, (int)height,
			stopGH[GHindex], 0xFFFFFFFF
		);
	}
	
	// ゲージの外枠
	Novice::DrawSprite(60, 340, leftPropBarGH, 1.0f, 1.0f, 0.0f, 0xffffffff);
	Novice::DrawSprite(140, 340, rightPropBarGH, 1.0f, 1.0f, 0.0f, 0xffffffff);
	Novice::DrawSprite(1160, 340, boostBarGH, 1.0f, 1.0f, 0.0f, 0xffffffff);

	// ゲージ内部
	Novice::DrawBox(65, 676, 51, (int)-(268.0f * leftPropellerPercentage), 0.0f, 0xe24848ff, kFillModeSolid);
	Novice::DrawBox(145, 676, 51, (int)-(268.0f * rightPropellerPercentage), 0.0f, 0xe24848ff, kFillModeSolid);
	Novice::DrawBox(1165, 676, 51, (int)-(268.0f * boostGaugePercentage), 0.0f, 0x98bbf9ff, kFillModeSolid);
}

// 着地リセット
void Player::ResetForCharge() {
	// 物理
	velocity = { 0.0f, 0.0f };

	// 上昇関連
	upValue = 0.0f;
	boostPower = 1.0f;

}

