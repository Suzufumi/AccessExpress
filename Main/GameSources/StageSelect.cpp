#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void StageSelect::OnCreate() {
		CreateViewLight();
		auto stageSel = AddGameObject<Sprite>(L"StageSelect_TX", Vec2(800, 200));
		stageSel->SetPosition(Vec2((float)App::GetApp()->GetGameWidth() / 2.0f, 100));
		CreateStageNum();
	}
	void StageSelect::OnUpdate() {
		auto PtrScene = App::GetApp()->GetScene<Scene>();
		int StageNum = PtrScene->GetStageNum();
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].wPressedButtons) {
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
			//シーンの取得
			//コントローラの取得
			auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
			if (CntlVec[0].bConnected) {
				//ゲームステージへ
				//Aボタン
				if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A) {
					PostEvent(0.0f, GetThis<StageSelect>(), PtrScene, L"ToGameStage");
					return;
				}
				if (!m_CntrolLock) {
					if (CntlVec[0].fThumbLY >= 0.8f) {
						StageNum--;
						if (StageNum < 1) {
							StageNum = 3;
						}
						m_CntrolLock = true;
						PtrScene->SetStageNum(StageNum);
						ChangeSelect(StageNum);
					}
					else if (CntlVec[0].fThumbLY <= -0.8f) {
						StageNum++;
						if (StageNum > 3) {
							StageNum = 1;
						}
						m_CntrolLock = true;
						PtrScene->SetStageNum(StageNum);
						ChangeSelect(StageNum);
					}
				}
				else {
					if (CntlVec[0].fThumbLY == 0.0f) {
						m_CntrolLock = false;
					}
				}
			}
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
		float height = (float)App::GetApp()->GetGameWidth();
		//for (int i = 0; i < 5; i++)
		//{
		//	int num = (i + 1);
		//	auto stageNum = AddGameObject<Sprite>(L"Stage" + num, Vec2(200, 200));
		//	stageNum->SetPosition(Vec2(200.0f * (i + 1), height / 3.0f));
		//}
		auto stageNum = AddGameObject<AnimSprite>(L"Stage1", true, Vec2(256, 256), Vec2(-400.0f, 0.0f));
		stageNum->SetSelect(true);
		m_SpVec[0] = stageNum;
		stageNum = AddGameObject<AnimSprite>(L"Stage2", true, Vec2(256, 256), Vec2(-200.0f, 0.0f));
		m_SpVec[1] = stageNum;
		stageNum = AddGameObject<AnimSprite>(L"Stage3", true, Vec2(256, 256), Vec2(0.0f, 0.0f));
		m_SpVec[2] = stageNum;
		stageNum = AddGameObject<AnimSprite>(L"Stage4", true, Vec2(256, 256), Vec2(200.0f, 0.0f));
		m_SpVec[3] = stageNum;
		stageNum = AddGameObject<AnimSprite>(L"Stage5", true, Vec2(256, 256), Vec2(400.0f, 0.0f));
		m_SpVec[4] = stageNum;
	}

	void StageSelect::StageSel()
	{
	}
	

	void StageSelect::ChangeSelect(int num)
	{
		for (int i = 0; i < 5; i++) {
			shared_ptr<AnimSprite> shptr = m_SpVec[i].lock();
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
	
}