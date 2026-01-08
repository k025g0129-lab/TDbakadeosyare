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
		PhaseUpdate();
		
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
		switch (phase) {
		case CHARGE:
			ChargeDraw();

			break;

		case RISE:
			RiseDraw();

			break;
		}

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

}

void Scene::TutorialDraw() {
}



void Scene::ResultDraw() {
}


void Scene::ChargeUpdate() {
}

void Scene::ChargeDraw() {
}

void Scene::RiseUpdate() {
}

void Scene::RiseDraw() {
}
