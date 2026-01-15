#include <Novice.h>
#include "Vector2.h"
#include "Scene.h"
#include "Player.h"

const char kWindowTitle[] = "1330_タイトル";


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

		Vector2_int currentLeftStickPos;

	Scene scene;
	Player player;

	Player* p = new Player();

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		Novice::GetAnalogInputLeft(0, &currentLeftStickPos.x, &currentLeftStickPos.y);

		scene.Update();
		player.Update(3);

		
		p->Update(p->scene_trial);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		p->Draw();
		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
