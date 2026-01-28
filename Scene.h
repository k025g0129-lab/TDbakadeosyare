#pragma once
#include <Windows.h> 
#include <Xinput.h> 
#include "Vector2.h"
#include "Player.h"
#include "Easing.h"
#include "Object.h"

class Scene {
public:
	enum GameScene {
		TITLE,
		TUTORIAL,
		DIFFICULTY_SELECT,
		MAIN_GAME,
		RESULT,
	};

	enum Phase {
		CHARGE,
		RISE,
	};

	enum Direction {
		RIGHT,
		LEFT,
	};

	struct BackGround {
		Vector2 skyOriginalPos;
		Vector2 skyPos;
	};

	struct CheckPoint {
		float triggerProgressY; // 着地の高さ
		int isPreparingForLanding; // 通過したかどうか
		int lv;  // 何回目のチェックポイントか
		float distance; // 1区間の長さ
		float scrollSpeed;  // 世界のスクロール速度

	};

	enum ChargeSubPhase {
		SHOW_PROPELLER_TEXT,
		PROPELLER_CHARGE,
		SHOW_BOOST_TEXT,
		BOOST_CHARGE,
	};

	// 難易度
	enum Difficulty {
		EASY,
		NORMAL,
		HARD
	};



public:

	GameScene gameScene = TITLE;
	Phase phase = CHARGE;
	Direction direction = LEFT;

	BackGround backGround[150];

	// チャージ
	int leftChargeAmount = 0;
	int rightChargeAmount = 0;

	// チャージ時間
	int chargeTimer = 600;
	int propellerEndTime = 700;
	int maxChargeTime = 1200;

	// 傾き
	int tiltDegree = 0;

	// スクロール
	float scrollY = 0.0f;
	int isScroll = false;
	int isTouchCheckpoint = false;


	int whiteTextureHandle = 0;

	// チェックポイント
	CheckPoint checkPoint;

	int isClear = false;

	// 今の上昇量
	float progressY = 0.0f;

	// 難易度
	Difficulty difficulty;


public:

	Scene();
	~Scene();

	// 初期化
	void Initialize();
	void Update();
	void Draw();

	// ゲームシーン用
	void TitleUpdate();
	void TitleDraw();

	void TutorialUpdate();
	void TutorialDraw();

	void DifficultySelectUpdate();
	void DifficultySelectDraw();

	void MainGameUpdate();
	void MainGameDraw();

	void ResultUpdate();
	void ResultDraw();
	
	
	// フェーズ用
	void PhaseUpdate();

	void ChargeUpdate();
	void ChargeDraw();

	void RiseUpdate();
	void RiseDraw();


	// 入力
	bool IsPressB() const;
	bool IsTriggerB() const;
	
	bool IsPressA() const;
	bool IsTriggerA() const;

	//　難易度適用
	void ApplyDifficulty();



private:
	// コントローラー
	XINPUT_STATE padState{}; // 今のフレームの入力状態
	XINPUT_STATE prevPadState{};  // 1フレーム前の入力状態

	// 難易度選択用
	int selectedDifficulty = 1;

	// タイトル用
	int selectedTitleMenu = 0;

	// プレイヤー
	Player* player;
	float playerStartY;
};

