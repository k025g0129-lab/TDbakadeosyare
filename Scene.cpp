#include "Scene.h"
#include "Vector2.h"
#include <Novice.h>

Scene::Scene() {
	Initialize();
}

void Scene::Initialize() {
	gameScene = TITLE;
	phase = CHARGE;
}

void Scene::Update() {
	switch (gameScene) {

	case TITLE:
		TitleUpdate();
		
		break;

	case TUTORIAL:
		TutorialUpdate();
		
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

	case MAIN_GAME:
		MainGameUpdate();

		break;


	case RESULT:
		ResultDraw();

		break;
	}

}

// 更新処理
void Scene::TitleUpdate() {
	if(Novice::CheckHitKey(DIK_SPACE)){
		gameScene = TUTORIAL;
	}
}

void Scene::TutorialUpdate() {
	if (Novice::CheckHitKey(DIK_SPACE)) {
		gameScene = MAIN_GAME;
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

void Scene::ResultUpdate() {
	if (Novice::CheckHitKey(DIK_SPACE)) {
		gameScene = TITLE;
	}
}

// 描画処理
void Scene::TitleDraw() {
	Novice::DrawBox(540, 320, 200, 80, 0.0f, 0xffffffff, kFillModeSolid);
}

void Scene::TutorialDraw() {
	Novice::DrawBox(440, 220, 400, 280, 0.0f, 0xffffffff, kFillModeSolid);
}

void Scene::MainGameDraw() {
	switch (phase) {
	case CHARGE:
		ChargeDraw();
		break;
	case RISE:
		RiseDraw();
		break;
	}
}

void Scene::ResultDraw() {
	Novice::DrawBox(540, 320, 200, 80, 0.0f, 0xffffffff, kFillModeSolid);
}


void Scene::ChargeUpdate() {
}

void Scene::ChargeDraw() {
}

void Scene::RiseUpdate() {
}

void Scene::RiseDraw() {
}
