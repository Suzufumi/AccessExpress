#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void TitleStage::OnCreate() {

		CreateViewLight();
		// �v���C���[�̍쐬
		CreateCharacter();
		// �^�C�g���̃��S���̔z�u
		CreateTitleResources();
	}
	void TitleStage::OnUpdate() {
		float delta = App::GetApp()->GetElapsedTime();
		m_player.lock()->GetComponent<BcPNTBoneModelDraw>()->UpdateAnimation(delta);
		// �t�F�[�h�̍Đ�
		FadePlay();

	}
	void TitleStage::CreateViewLight(){
		auto ptrView = CreateView<SingleView>();
		//�}���`���C�g�̍쐬
		auto ptrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		ptrMultiLight->SetDefaultLighting();
	}

	void TitleStage::CreateCharacter()
	{
		// �������W�̒�`
		const Vec3 StartPosition = Vec3(0.0f, -4.5f, -5.0f);
		// �傫���̒�`
		const float Player_Scale = 1.5f;
		// �A�j���[�V�����̒������`
		const int Player_AnimLength = 110;
		// �A�j���[�V�����̃t���[�������`
		const float AnimationFlame = 60.0f;

		m_player = AddGameObject<GameObject>();
		// �L�����N�^�[���O�ɕ`�悳���悤�Ƀ��C���[�̐ݒ�
		m_player.lock()->SetDrawLayer(10);
		// Transform�R���|�[�l���g���擾
		auto transComp = m_player.lock()->GetComponent<Transform>();
		// ���W�̐ݒ�
		transComp->SetPosition(StartPosition);
		// �傫���̐ݒ�
		transComp->SetScale(Vec3(Player_Scale));
		// �`��R���|�[�l���g�̒ǉ�
		auto drawComp = m_player.lock()->AddComponent<BcPNTBoneModelDraw>();
		// ���f�����b�V���̓ǂݍ���
		drawComp->SetMultiMeshResource(L"PLAYER_MODEL");
		// �e�N�X�`���̓ǂݍ���
		drawComp->SetTextureResource(L"PLAYER_TX");
		// ���C�e�B���O�����邩�ǂ���
		drawComp->SetLightingEnabled(false);
		// �A�j���[�V������ǉ�����
		drawComp->AddAnimation(L"Default", 0, Player_AnimLength, true, AnimationFlame);
		// �A�j���[�V�����̐ݒ�
		drawComp->ChangeCurrentAnimation(L"Default");
		// �}���`���b�V���̐ݒ�
		drawComp->SetMultiMeshIsDraw(0, false); // ��ԏ�̐�
		drawComp->SetMultiMeshIsDraw(1, false);	// �^�񒆂̐�
		drawComp->SetMultiMeshIsDraw(2, false);	// ��ԉ��̐�
		drawComp->SetMultiMeshIsDraw(3, true);	// �ʏ��
		drawComp->SetMultiMeshIsDraw(4, false);	// ���ꂵ��
		drawComp->SetMultiMeshIsDraw(5, true); // ��
		drawComp->SetMultiMeshIsDraw(6, false);	// �Q�Ă��
		drawComp->SetMultiMeshIsDraw(7, false);	// �߂�����
	}

	void TitleStage::CreateTitleResources()
	{
		// �w�i�摜�̃T�C�Y
		const Vec2 BackGround_Size = Vec2(1300, 800);
		// �w�i�摜�̍��W
		const Vec2 BackGround_Pos = Vec2(640, 400);
		// �^�C�g�����S�̃T�C�Y
		const Vec2 TitleLogo_Size = Vec2(819, 205);
		// �^�C�g�����S�̍��W
		const Vec2 TitleLogo_Pos = Vec2(640, 140);
		// �{�^���̏����T�C�Y
		const Vec2 PushAnyButton_Size = Vec2(1000, 100);
		// �{�^���̏����ʒu
		const Vec2 PushAnyButton_Pos = Vec2(0, -300);

		// �w�i�摜�̒ǉ�
		auto titleBack = AddGameObject<Sprite>(L"SkyBox_Back_TX", BackGround_Size);
		// ���W�̐ݒ�
		titleBack->SetPosition(BackGround_Pos);
		// �^�C�g�����S�̒ǉ�
		auto titleSprite = AddGameObject<Sprite>(L"TITLE_TX", TitleLogo_Size);
		// ���W�̐ݒ�
		titleSprite->SetPosition(TitleLogo_Pos);
		// �_�ł���{�^���̒ǉ�
		auto buttonSprite = AddGameObject<AnimSprite>(L"Title_BUTTON_TX", true, PushAnyButton_Size, PushAnyButton_Pos);
		buttonSprite->SetSelect(true);

	}

	void TitleStage::FadePlay()
	{
		// �t�F�[�h�̍��W���`
		const Vec2 FadePos = Vec2(640, 400);
		// �t�F�[�h�̑傫�����`
		const Vec2 FadeSize = Vec2(1280, 800);
		// SE�̑傫�����`
		const float SE_Volume = 1.0f;
	
		auto& gm = GameManager::GetInstance();
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		// �{�^���������ꂽ��
		if (CntlVec[0].wPressedButtons) {
			auto scenePtr = App::GetApp()->GetScene<Scene>();
			scenePtr->MusicOnceStart(L"Decision_SE", SE_Volume);
			// �t�F�[�h�̍쐬
			auto fade = AddGameObject<FadeInOut>(FadePos, FadeSize);
			fade->SetIsFadeOut(true);
		}
		else
		{
			return;
		}
		// �t�F�[�h�����ǂ���
		if (!gm.GetIsFade())
		{
			gm.SetIsFade(true);
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToStageSelect");
		}

	}
}