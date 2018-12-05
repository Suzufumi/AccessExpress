#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void TitleStage::OnCreate() {
		Col4 Col;
		Col.set(0.0f / 255.0f, 200.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
		App::GetApp()->GetScene<Scene>()->SetClearColor(Col);

		CreateViewLight();
		CreatePlayer();
		AddGameObject<SkySprite>(Vec3(0.0f), Vec3(0.0f));
		auto titleSprite = AddGameObject<Sprite>(L"TITLE_TX", Vec2(600, 200));
		titleSprite->SetPosition(Vec2(640, 140));
		auto buttonSprite = AddGameObject<Sprite>(L"Title_BUTTON_TX", Vec2(1000, 100));
		buttonSprite->SetPosition(Vec2(640, 700));
		//auto obb = AddGameObject<OBBObject>(Vec3(0, 0, 0), Vec3(10, 8, 1));
		//obb->GetComponent<PNTStaticDraw>()->SetTextureResource(L"Title_TX");
	}
	void TitleStage::OnUpdate() {
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].wPressedButtons) {
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToStageSelect");
		}
		float delta = App::GetApp()->GetElapsedTime();
		m_player.lock()->GetComponent<PNTBoneModelDraw>()->UpdateAnimation(delta);

	}
	void TitleStage::CreateViewLight(){
		auto ptrView = CreateView<SingleView>();
		//マルチライトの作成
		auto ptrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		ptrMultiLight->SetDefaultLighting();
	}

	void TitleStage::CreatePlayer()
	{
		m_player = AddGameObject<GameObject>();
		m_player.lock()->SetDrawLayer(100);
		auto transComp = m_player.lock()->GetComponent<Transform>();
		transComp->SetPosition(0.0f, -4.0f, -5.0f);
		transComp->SetScale(1.5f, 1.5f, 1.5f);
		auto drawComp = m_player.lock()->AddComponent<PNTBoneModelDraw>();
		drawComp->SetMultiMeshResource(L"PLAYER_MODEL");
		drawComp->SetTextureResource(L"PLAYER_TX");
		// アニメーションを追加する
		drawComp->AddAnimation(L"Default", 0, 110, true, 30.0f);
		// アニメーションの設定
		drawComp->ChangeCurrentAnimation(L"Default");

	}
}