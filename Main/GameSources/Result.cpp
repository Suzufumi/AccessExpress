#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void ResultStage::OnCreate() {
		CreateViewLight();
		CreateRemainingTime();
		CreateMaximumChain();
		CreateScore();
		auto clearSprite = AddGameObject<Sprite>(L"ResultStage_TX", Vec2(800.0f, 120.0f));
		clearSprite->SetPosition(Vec2(App::GetApp()->GetGameWidth() / 2.0f, App::GetApp()->GetGameHeight() / 6.0f));
		//auto obb = AddGameObject<OBBObject>(Vec3(0, 0, 0), Vec3(10, 8, 1));
		//obb->GetComponent<PNTStaticDraw>()->SetTextureResource(L"ResultStage_TX");
	}
	void ResultStage::OnUpdate() {
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].wPressedButtons) {
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
		}
	}
	void ResultStage::CreateViewLight() {
		auto ptrView = CreateView<SingleView>();
		//マルチライトの作成
		auto ptrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		ptrMultiLight->SetDefaultLighting();
	}
	void ResultStage::CreateRemainingTime() {
		auto timeSprite = AddGameObject<Sprite>(L"RemainingTimeText_TX", Vec2(410, 205));
		timeSprite->SetPosition(Vec2((App::GetApp()->GetGameWidth() / 10.0f) * 2.0f, (App::GetApp()->GetGameHeight() / 10.0f)* 4));
	};
	void ResultStage::CreateMaximumChain() {
<<<<<<< HEAD
		auto maxChainText = AddGameObject<Sprite>(L"MaxChainText_TX", Vec2(461, 230));
		maxChainText->SetPosition(Vec2((App::GetApp()->GetGameWidth() / 10.0f) * 2.0f, (App::GetApp()->GetGameHeight() / 10.0f) * 6));
		auto maxChainNum = AddGameObject<NumberSprite>(2, 22);	
		maxChainNum->GetComponent<Transform>()->
			//SetPosition((App::GetApp()->GetGameWidth() / 10.0f) * 5.0f, (App::GetApp()->GetGameHeight() / 10.0f) * 6.0f,0);
			SetPosition(850,-450, 0);
=======
		auto timeSprite = AddGameObject<Sprite>(L"MaxChainText_TX", Vec2(461, 230));
		timeSprite->SetPosition(Vec2((App::GetApp()->GetGameWidth() / 10.0f) * 2.0f, (App::GetApp()->GetGameHeight() / 10.0f) * 6));
>>>>>>> origin/master
	};
	void ResultStage::CreateScore() {
		auto timeSprite = AddGameObject<Sprite>(L"ScoreText_TX", Vec2(410, 205));
		timeSprite->SetPosition(Vec2((App::GetApp()->GetGameWidth() / 10.0f) * 2.0f, (App::GetApp()->GetGameHeight() / 10.0f) * 8));
	};

}