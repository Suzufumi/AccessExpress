#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void TitleStage::OnCreate() {
		Col4 Col;
		Col.set(0.0f / 255.0f, 200.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
		App::GetApp()->GetScene<Scene>()->SetClearColor(Col);

		CreateViewLight();
		CreatePlayer();
		auto titleBack = AddGameObject<Sprite>(L"SKY_TX", Vec2(1280, 1080));
		titleBack->SetPosition(Vec2(640, 480));
		auto titleSprite = AddGameObject<Sprite>(L"TITLE_TX", Vec2(819, 205));
		titleSprite->SetPosition(Vec2(640, 140));
		auto buttonSprite = AddGameObject<AnimSprite>(L"Title_BUTTON_TX", true,  Vec2(1000, 100), Vec2(0, -300));
		//auto obb = AddGameObject<OBBObject>(Vec3(0, 0, 0), Vec3(10, 8, 1));
		//obb->GetComponent<PNTStaticDraw>()->SetTextureResource(L"Title_TX");
	}
	void TitleStage::OnUpdate() {
		auto& gm = GameManager::GetInstance();
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].wPressedButtons) {
			auto scenePtr = App::GetApp()->GetScene<Scene>();
			scenePtr->MusicOnceStart(L"Decision_SE", 1.0f);
			auto fade = AddGameObject<FadeInOut>(Vec2(640, 400), Vec2(1280, 800));
			fade->SetIsFadeOut(true);
		}
		// �t�F�[�h�����ǂ���
		if (!gm.GetIsFade())
		{
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
		}
		float delta = App::GetApp()->GetElapsedTime();
		m_player.lock()->GetComponent<BcPNTBoneModelDraw>()->UpdateAnimation(delta);

	}
	void TitleStage::CreateViewLight(){
		auto ptrView = CreateView<SingleView>();
		//�}���`���C�g�̍쐬
		auto ptrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		ptrMultiLight->SetDefaultLighting();
	}

	void TitleStage::CreatePlayer()
	{
		m_player = AddGameObject<GameObject>();
		m_player.lock()->SetDrawLayer(100);
		auto transComp = m_player.lock()->GetComponent<Transform>();
		transComp->SetPosition(0.0f, -4.5f, -5.0f);
		transComp->SetScale(1.5f, 1.5f, 1.5f);
		auto drawComp = m_player.lock()->AddComponent<BcPNTBoneModelDraw>();
		drawComp->SetMultiMeshResource(L"PLAYER_MODEL");
		drawComp->SetTextureResource(L"PLAYER_TX");
		drawComp->SetLightingEnabled(false);
		// �A�j���[�V������ǉ�����
		drawComp->AddAnimation(L"Default", 0, 110, true, 60.0f);
		// �A�j���[�V�����̐ݒ�
		drawComp->ChangeCurrentAnimation(L"Default");
		drawComp->SetMultiMeshIsDraw(0, false); // ��ԏ�̐�
		drawComp->SetMultiMeshIsDraw(1, false);	// �^�񒆂̐�
		drawComp->SetMultiMeshIsDraw(2, false);	// ��ԉ��̐�
		drawComp->SetMultiMeshIsDraw(3, true);	// �ʏ��
		drawComp->SetMultiMeshIsDraw(4, false);	// ���ꂵ��
		drawComp->SetMultiMeshIsDraw(5, true); // ��
		drawComp->SetMultiMeshIsDraw(6, false);	// �Q�Ă��
		drawComp->SetMultiMeshIsDraw(7, false);	// �߂�����
	}
}