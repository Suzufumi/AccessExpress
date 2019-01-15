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
		transComp->SetPosition(0.0f, -4.0f, -5.0f);
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