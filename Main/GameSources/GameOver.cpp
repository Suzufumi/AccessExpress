#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void GameOverStage::OnCreate() {
		CreateViewLight();
		auto sprite = AddGameObject<Sprite>(L"GameOver_TX",Vec2(1240.0f,240.0f));
		sprite->SetPosition(Vec2(App::GetApp()->GetGameWidth()/2.0f, App::GetApp()->GetGameHeight()/2.0f));
	}
	void GameOverStage::OnUpdate() {
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].wPressedButtons) {
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
		}
	}
	void GameOverStage::CreateViewLight() {
		auto ptrView = CreateView<SingleView>();
		//マルチライトの作成
		auto ptrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		ptrMultiLight->SetDefaultLighting();
	}
}