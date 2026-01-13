#include"Function.h"
#include"Vector2.h"
#include<Novice.h>

// self○○   = 主観している対象
// target○○ = 当たられる対象

// 例) 弾が敵に当たったら      -> 弾 = self○○ , 敵 = target○○
// 例) 敵が障害物にぶつかったら -> 敵 = self○○ , 障害物 = target○○


//　円同士の当たり判定
bool IsCollision(const Vector2& selfPosition, const Vector2& targetPosition, const float& selfRadius, const float& targetRadius) {
    
    /// 衝突判定に使う計算
    float distanceWidth = static_cast<float>(fabsf(targetPosition.x - selfPosition.x));
    float distanceVertical = static_cast<float>(fabsf(targetPosition.y - selfPosition.y));
    float distance = static_cast<float>(sqrtf(distanceWidth * distanceWidth + distanceVertical * distanceVertical));
    float sumRadius = static_cast<float>(targetRadius + selfRadius);

    /// 距離が半径以下ならtrueを、そうでなければfalseを返す
    if (distance <= sumRadius) {
        return true;
    } else {
        return false;
    }
}

// ベクトル正規化
void NormalizeDirection(Vector2& selfPosition, Vector2& selfDirection, Vector2& targetPosition, float& length) {
    // 弾が発射される直前にプレイヤーの位置へ向かうベクトル算出
    Vector2 dir{};
    dir.x = targetPosition.x - selfPosition.x;
    dir.y = targetPosition.y - selfPosition.y;

    // ベクトルの長さを計算
    length = sqrtf(dir.x * dir.x + dir.y * dir.y);

    // ベクトルの正規化
    if (length != 0.0f) {

        // bullet[i] の direction に保存
        selfDirection.x = dir.x / length;
        selfDirection.y = dir.y / length;

    }
}

// 予測線
void DrawBulletLine(
    Vector2 startPos,       // 弾の現在位置 (回転の中心)
    Vector2 direction,      // 弾の方向ベクトル
    float length,           // ★ 描画する長さ (例: 1500.0f)
    float thickness,        // 予測線の太さ
    int bulletLineGH,
    int alpha
) {
    // -------------------------------------------------------------------
    // 1. 回転角（角度）の計算
    // atan2(Y成分, X成分) で、方向ベクトルからラジアン角度を求める
    float angle = atan2f(direction.y, direction.x);

    // -------------------------------------------------------------------
    // 2. 描画始点 (左上隅) の逆算

    // Boxの中心から左上隅までの相対座標を計算
    float halfW = length / 2.0f - length / 2.0f;
    float halfH = thickness / 2.0f - thickness / 2.0f;

    // 回転前の左上隅の相対オフセット (中心から見てX軸負方向が -W/2, Y軸負方向が -H/2)
    float rx = -halfW; // X軸方向のオフセット
    float ry = -halfH; // Y軸方向のオフセット

    // -------------------------------------------------------------------
    // 3. 回転を考慮した描画始点 (左上隅) の計算

    // (rx, ry) を angle だけ回転させた後の座標を計算
    float rotatedX = rx * cosf(angle) - ry * sinf(angle);
    float rotatedY = rx * sinf(angle) + ry * cosf(angle);

    // 弾の中心 (startPos) に回転後のオフセットを加える
    int topLeftX = (int)(startPos.x + rotatedX);
    int topLeftY = (int)(startPos.y + rotatedY);

    Novice::DrawSprite(
        topLeftX,
        topLeftY - 7,
        bulletLineGH,
        1.0f, 1.0f, angle, MakeColorWithAlpha(alpha, 0xffffff00)
    );
}

// 丸同士の判定(押し戻し)
void ResolveCollision(Vector2& playerPosition, float& playerRadius, Vector2& bossPosition, float& bossRadius) {
    // 1. 中心間の距離ベクトルを計算 (ボス -> プレイヤー)
    Vector2 V = {
        playerPosition.x - bossPosition.x,
        playerPosition.y - bossPosition.y
    };

    // 2. 距離 (D) を計算
    float D = sqrtf(V.x * V.x + V.y * V.y);

    // 3. 半径の合計 (R_total)
    float R_total = bossRadius + playerRadius;

    // 4. 衝突判定 (D < R_total)
    if (D < R_total) {

        // 5. めり込み距離 (S) を計算
        float S = R_total - D;

        // 6. 押し戻す方向ベクトル (N) を計算 (正規化)
        Vector2 N{};
        if (D != 0.0f) {
            N.x = V.x / D;
            N.y = V.y / D;
        } else {
            // 完全に重なった場合 (D=0)、Y軸上に分離すると仮定
            N.y = 1.0f;
        }

        // 7. 押し戻し量 (めり込み距離の半分) を計算
        float push_amount = S / 2.0f;

        // 8. プレイヤーを押し戻す
        playerPosition.x += N.x * push_amount;
        playerPosition.y += N.y * push_amount;
    }
}

// 円と矩形の当たり判定 (追加)
// selfPosition: 円の中心座標, selfRadius: 円の半径
// targetCenter: 矩形の中心座標, targetHalfWidth: 矩形の半幅, targetHalfHeight: 矩形の半高
// 円と矩形の当たり判定 (Circle vs AABB)
bool IsCollisionCircleRect(
    const Vector2& selfPosition, const float& selfRadius,
    const Vector2& targetCenter, const float& targetHalfWidth, const float& targetHalfHeight
) {
    // 1. 矩形の境界線を計算 (AABB)
    float boxLeft = targetCenter.x - targetHalfWidth;
    float boxRight = targetCenter.x + targetHalfWidth;
    float boxTop = targetCenter.y - targetHalfHeight;
    float boxBottom = targetCenter.y + targetHalfHeight;

    // 2. プレイヤーの中心から矩形に最も近い点 (Closest Point) を探す
    // fmaxf/fminf は <cmath> に含まれる（または <math.h>）
    float closestX = fmaxf(boxLeft, fminf(selfPosition.x, boxRight));
    float closestY = fmaxf(boxTop, fminf(selfPosition.y, boxBottom));

    // 3. プレイヤーの中心と Closest Point の距離の二乗を計算
    float distX = selfPosition.x - closestX;
    float distY = selfPosition.y - closestY;
    float distanceSquared = distX * distX + distY * distY;

    // 4. 衝突判定 (距離の二乗 < 半径の二乗)
    // 半径の二乗を計算しておくことで、sqrtfの計算を省略でき、高速化できる
    return distanceSquared < (selfRadius * selfRadius);
}

// 円と回転した矩形の当たり判定 (Circle vs Rotated Rect)
bool IsCollisionCircleRotatedRect(
    const Vector2& selfPosition, const float& selfRadius,
    const Vector2& targetCenter, const float& targetHalfWidth, const float& targetHalfHeight, const float& targetAngle
) {
    // 1. プレイヤーの中心座標を、レーザーの回転を打ち消すように逆回転させる

    // プレイヤー中心とレーザー中心との相対位置
    Vector2 relativePos;
    relativePos.x = selfPosition.x - targetCenter.x;
    relativePos.y = selfPosition.y - targetCenter.y;

    // 逆回転行列（-targetAngle）を適用
    float cosA = cosf(-targetAngle);
    float sinA = sinf(-targetAngle);

    // 逆回転後のプレイヤー座標 (レーザーのローカル座標系)
    Vector2 rotatedPos;
    rotatedPos.x = relativePos.x * cosA - relativePos.y * sinA;
    rotatedPos.y = relativePos.x * sinA + relativePos.y * cosA;

    // 2. 軸並行の矩形判定を行う (中心は (0, 0))

    float boxLeft = -targetHalfWidth;
    float boxRight = targetHalfWidth;
    float boxTop = -targetHalfHeight;
    float boxBottom = targetHalfHeight;

    // 3. プレイヤーの中心 (rotatedPos) から矩形に最も近い点 (Closest Point) を探す
    float closestX = fmaxf(boxLeft, fminf(rotatedPos.x, boxRight));
    float closestY = fmaxf(boxTop, fminf(rotatedPos.y, boxBottom));

    // 4. プレイヤーの中心と Closest Point の距離の二乗を計算
    float distX = rotatedPos.x - closestX;
    float distY = rotatedPos.y - closestY;
    float distanceSquared = distX * distX + distY * distY;

    // 5. 衝突判定 (距離の二乗 < 半径の二乗)
    return distanceSquared < (selfRadius * selfRadius);
}



// 透明度を徐々に増やす
unsigned int FadeIn(int& alphaValue, unsigned int rgbColor) {

    // 1. alphaValue の更新 (この関数で値を減らしたい場合)
    if (alphaValue > 0) {
        alphaValue -= 2;
    }

    if (alphaValue <= 0) {
        alphaValue = 0;
    }

    // 2. RGB成分の準備
    // rgbColor は 0xRRGGBBAA の形式で渡されることを前提とするが、
    // 描画関数側ではアルファを制御するため、一旦アルファは 0 になっていると仮定する。

    // 3. アルファ値を最下位バイトに配置 (シフトなし)
    unsigned int alphaComponent = (unsigned int)alphaValue; // 24ビットシフトを削除！

    // rgbColorは 0xRRGGBB00 の形式で渡されていると想定

    // 4. アルファ成分とRGB成分を論理和で合成
    // 0xRRGGBB00 | 0x000000AA = 0xRRGGBBAA
    return rgbColor | alphaComponent;
}

// 透明度を徐々に減らす
unsigned int FadeOut(int& alphaValue, unsigned int rgbColor) {

    // 1. alphaValue の更新 (この関数で値を減らしたい場合)
    if (alphaValue < 255) {
        alphaValue += 2;
    }

    if (alphaValue >= 255) {
        alphaValue = 255;
    }

    // 2. RGB成分の準備
    // rgbColor は 0xRRGGBBAA の形式で渡されることを前提とするが、
    // 描画関数側ではアルファを制御するため、一旦アルファは 0 になっていると仮定する。

    // 3. アルファ値を最下位バイトに配置 (シフトなし)
    unsigned int alphaComponent = (unsigned int)alphaValue; // 24ビットシフトを削除！

    // rgbColorは 0xRRGGBB00 の形式で渡されていると想定

    // 4. アルファ成分とRGB成分を論理和で合成
    // 0xRRGGBB00 | 0x000000AA = 0xRRGGBBAA
    return rgbColor | alphaComponent;
}

// 透明度くっつけるだけ
unsigned int MakeColorWithAlpha(int& alphaValue, unsigned int rgbColor) {

    // アルファ値を0〜255の範囲にクランプ（制限）する
    if (alphaValue < 0) {
        alphaValue = 0;
    }
    if (alphaValue > 255) {
        alphaValue = 255;
    }

    // 1. アルファ成分を24ビット左にシフトし、最上位バイト (A) に配置する
    // 例: alphaValue=255(0xFF) -> 0xFF000000
    unsigned int alphaComponent = (unsigned int)alphaValue;

    // 2. アルファ成分とRGB成分を論理和で合成する
    // 0xAA000000 | 0x00RRGGBB = 0xAARRGGBB
    return rgbColor | alphaComponent;
}