#pragma once
#ifndef CALC_H 
#define CALC_H

#include "Novice.h"
#include"Vector2.h"
#include<math.h>

/// <summary>
/// 円同士の当たり判定
/// </summary>
bool IsCollision(const Vector2& selfPosition, const Vector2& targetPosition, const float& selfRadius, const float& targetRadius);

/// <summary>
/// ベクトルを正規化
/// </summary>
void NormalizeDirection(Vector2& selfPosition, Vector2& selfDirection, Vector2& targetPosition, float& length);

/// <summary>
/// 予測線の描画
/// </summary>
void DrawBulletLine(
	Vector2 startPos,       // 弾の現在位置 (回転の中心)
	Vector2 direction,      // 弾の方向ベクトル
	float length,           // 描画する長さ
	float thickness,        // 予測線の太さ
	int bulletLineGH,
	int alpha
);

/// <summary>
/// 円同士の当たり判定(押し戻し)
/// </summary>
void ResolveCollision(Vector2& playerPosition, float& playerRadius, Vector2& bossPosition, float& bossRadius);

/// <summary>
/// 円同士の当たり判定(押し戻し)
/// </summary>
Vector2 Vector2Add(Vector2 Ve1,Vector2 Ve2);

/// <summary>
/// 円と回転した矩形が当たってるかどうか
/// </summary>
bool IsCollisionCircleRect(
	const Vector2& selfPosition, const float& selfRadius,
	const Vector2& targetCenter, const float& targetHalfWidth, const float& targetHalfHeight
);

/// <summary>
/// フェードイン
/// </summary>
unsigned int FadeIn(int& alphaValue, unsigned int rgbColor);

/// <summary>
/// フェードアウト
/// </summary>
unsigned int FadeOut(int& alphaValue, unsigned int rgbColor);

/// <summary>
/// 引数に設定した透明度を連結
/// </summary>
unsigned int MakeColorWithAlpha(int& alphaValue, unsigned int rgbColor);
#endif // CALC_H