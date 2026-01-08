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

}

void Scene::Draw() {

}

void Scene::PhaseUpdate() {


}

void Scene::ChargeUpdate() {
}

void Scene::ChargeDrow() {
}

void Scene::RiseUpdate() {
}

void Scene::RiseDrow() {
}
