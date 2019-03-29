#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void StageSelect::OnCreate() {
		CreateViewLight();
		auto titleBack = AddGameObject<Sprite>(L"SkyBox_Back_TX", Vec2(1300, 800));
		titleBack->SetPosition(Vec2(640, 400));
		auto selectText = AddGameObject<Sprite>(L"StageSelect_TX", Vec2(1024, 128));
		selectText->SetPosition(Vec2(650, 50));
		m_fade = AddGameObject<FadeInOut>(Vec2(640, 400), Vec2(1280, 800));
		CreateStageNum();
	}
	void StageSelect::OnUpdate() {
		auto ptrScene = App::GetApp()->GetScene<Scene>();
		int stageNum = ptrScene->GetStageNum();
		auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		//シーンの取得
		//コントローラの取得
		if (cntlVec[0].bConnected) {
			//ゲームステージへ
			//Aボタン
			if (cntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A) {
				App::GetApp()->GetScene<Scene>()->MusicOnceStart(L"Decision_SE", 1.0f);
				m_fade.lock()->SetAlpha(0.0f);
				m_fade.lock()->SetIsFadeOut(true);
				return;
			}
			Select();
		}
		auto& gm = GameManager::GetInstance();
		// フェード中かどうか
		if (!gm.GetIsFade())
		{
			gm.SetIsFade(true);
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
		}

	}
	void StageSelect::CreateViewLight() {
		auto ptrView = CreateView<SingleView>();
		//マルチライトの作成
		auto ptrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		ptrMultiLight->SetDefaultLighting();
	}

	void StageSelect::CreateStageNum()
	{
		auto ptrSp = AddGameObject<AnimSprite>(L"NORMAL_TX", true,
			Vec2(410, 205), Vec2(-300.0f, 0.0f));
		ptrSp->SetSelect(true);
		m_spVec[0] = ptrSp;
		ptrSp = AddGameObject<AnimSprite>(L"HARD_TX", true,
			Vec2(410, 205), Vec2(300.0f, 0.0f));
		m_spVec[1] = ptrSp;
	}


	void StageSelect::ChangeSelect(int num)
	{
		for (int i = 0; i < 2; i++) {
			shared_ptr<AnimSprite> shptr = m_spVec[i].lock();
			if (shptr) {
				if ((i + 1) == num) {
					shptr->SetSelect(true);
				}
				else {
					shptr->SetSelect(false);
				}
			}
		}

	}

	void StageSelect::Select()
	{
		auto ptrScene = App::GetApp()->GetScene<Scene>();
		int stageNum = ptrScene->GetStageNum();
		auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (!m_CntrolLock) {
			if (cntlVec[0].fThumbLX >= 0.5f) {
				stageNum++;
				if (stageNum > 2) {
					stageNum = 2;
				}
				m_CntrolLock = true;
				ptrScene->SetStageNum(stageNum);
				ChangeSelect(stageNum);
			}
			else if (cntlVec[0].fThumbLX <= -0.5f) {
				stageNum--;
				if (stageNum < 1) {
					stageNum = 1;
				}
				m_CntrolLock = true;
				ptrScene->SetStageNum(stageNum);
				ChangeSelect(stageNum);
			}
		}
		else {
			if (cntlVec[0].fThumbLX == 0.0f) {
				m_CntrolLock = false;
			}
		}

	}
	
}