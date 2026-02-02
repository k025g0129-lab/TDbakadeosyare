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
		PAUSE
	};

	enum Phase {
		CHARGE,
		RISE,
	};

	enum Direction {
		RIGHT,
		LEFT,
	};



	enum TitleButton {
		GAME_PLAY_BUTTON,
		TUTORIAL_BUTTON,
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

	// 目標距離
	float goalDistance;

	// ゲーム開始時の初期位置
	float gameStartPlayerY;

	// 傾き
	int tiltDegree = 0;

	// スクロール
	float scrollY = 0.0f;
	int isScroll = false;
	int isTouchCheckpoint = false;


	int whiteTextureHandle = 0;

	// チェックポイント
	CheckPoint checkPoint;

	int birdOccurrences;
	int isClear = false;
	float preCheckPointPosY = 0.0f;

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
	
	void PauseUpdate();
	void PauseDraw();

	
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

	bool IsPressX() const;
	bool IsTriggerX() const;

	bool IsPressY() const;
	bool IsTriggerY() const;

	//　難易度適用
	void ApplyDifficulty();

private:
	// コントローラー
	XINPUT_STATE padState{}; // 今のフレームの入力状態
	XINPUT_STATE prevPadState{};  // 1フレーム前の入力状態

	// ポーズ用
	int selectedPauseMenu = 0;

	// 難易度選択用
	int selectedDifficulty = 1;

	// タイトル用
	int selectedTitleMenu = 0;

	//チュートリアル
	int asobikataPaper = 0;
	int maxAsobikataPaper = 4;

	// プレイヤー
	Player* player;
	float playerStartY;
	static const int maxBird = 20;
	Object* bird[maxBird];

	Vector2 PtitlePos = { 0.0f,0.0f };
	//Vector2 underPtitlePos = {0.0f,720.0f};
	TitleButton titleButton = GAME_PLAY_BUTTON;

	//float titleT = 0.0f;

	float amplitude = 100.0f;
	float theta = 0.0f;

	// チャージ演出用
	ChargeSubPhase chargeSubPhase = SHOW_PROPELLER_TEXT;

	// 文字演出用
	float chargeTextT = 0.0f;
	Vector2 chargeTextPos;
	float pressAT = 0.0f;
	float pressATSpeed = 1.0f / 120.0f;

	Vector2 titleBGPos[2];

	//高度
	int altitude = 0;

	int keta[6];


	//GH
	int titleBGGH = 0;
	int titleBG2GH = 0;
	int pressAGH = 0;
	int pressAstartGH = 0;
	int playChoiceGH = 0;
	int tutorialChoiceGH = 0;
	int titleLogoGH = 0;
	int PtitleLogoGH = 0;
	int pressAbackGH = 0;
	int LeftArrowGH = 0;
	int RightArrowGH = 0;
	int asobikataGH = 0;
	int difficultyGH[3];
	int selectLevelGH = 0;
	int checkPointGH[2];
	int clearGH = 0;
	int failedGH = 0;
	int boostGuidanceGH = 0;
	int propGuidanceGH = 0;
	int mawaseGH = 0;
	int oseGH = 0;

	int suuziGH[10];
	int dotGH;

	//　ポーズ
	int pauseGH[3];
	int pauseFilterGH;

	// 定数
	const float TEXT_START_Y = 800.0f;
	const float TEXT_END_Y = 360.0f;
	
	// サウンド
	int soundHandleSelect; // カーソル移動音
	int soundHandleDecide; // 決定音

	int soundHandleTitleBGM; // 音源データ
	int voiceHandleTitleBGM; // 再生中の管理用ID

	int soundHandleMainBGM;
	int voiceHandleMainBGM;

	int soundHandleClear;
	int soundHandleGameOver;
	int voiceHandleResult;
};
