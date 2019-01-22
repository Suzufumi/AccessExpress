#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void ResultStage::OnCreate() {
		GameManager::GetInstance().SetIsFade(true);
		CreateViewLight();
		CreateCollectedMail();
		CreateMaximumChain();
		CreateScore();
		auto clearSprite = AddGameObject<Sprite>(L"ResultStage_TX", Vec2(800.0f, 120.0f));
		clearSprite->SetPosition(Vec2(640.0f, 100.0f));
		m_push = AddGameObject<Sprite>(L"Title_BUTTON_TX", Vec2(1000, 100));
		m_push.lock()->SetPosition(Vec2(640.0f, 700.0f));
		//auto obb = AddGameObject<OBBObject>(Vec3(0, 0, 0), Vec3(10, 8, 1));
		//obb->GetComponent<PNTStaticDraw>()->SetTextureResource(L"ResultStage_TX");
	}

	void ResultStage::OnUpdate() {
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		auto& gm = GameManager::GetInstance();
		switch (m_progress){
		case progress::START:
			if (CntlVec[0].wPressedButtons) {
				m_progress = progress::ADD_SCORE;
				m_push.lock()->SetDrawActive(false);
			}
			break;
		case progress::ADD_SCORE :
			AddScore();
			break;
		case progress::SCORE_COUNTUP :
			if (m_scoreNum.lock()->GetFinishCountUp()) {
				m_push.lock()->SetDrawActive(true);
				m_mailText.lock()->SetDrawActive(false);
				m_mailNum.lock()->SetDrawActive(false);
				m_maxChainText.lock()->SetDrawActive(false);
				m_maxChainNum.lock()->SetDrawActive(false);
				m_scoreText.lock()->SetPosition(Vec2(720.0f, 340.0f));
				//m_scoreNum.lock()->GetComponent<Transform>()->SetPosition()
				CreatePlayer();
				m_progress = progress::END;
			}
			break;
		case progress::END :
			if (CntlVec[0].wPressedButtons) {
				auto scenePtr = App::GetApp()->GetScene<Scene>();
				scenePtr->MusicOnceStart(L"Decision_SE", 1.0f);
				auto fade = AddGameObject<FadeInOut>(Vec2(640, 400), Vec2(1280, 800));
				fade->SetIsFadeOut(true);
				//m_progress = progress::FADE;
			}
			//�t�F�[�h�����ǂ���
			if (!gm.GetIsFade())
			{
				gm.SetIsFade(true);
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
			}
			m_player.lock()->GetComponent<BcPNTBoneModelDraw>()->UpdateAnimation(App::GetApp()->GetElapsedTime());
			break;
		//case progress::FADE:
		//	FadeProcess();
		//	break;
		default:
			break;
		}
	}
	void ResultStage::CreateViewLight() {
		auto ptrView = CreateView<SingleView>();
		//�}���`���C�g�̍쐬
		auto ptrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		ptrMultiLight->SetDefaultLighting();
	}
	///-----------------------------------------------------------------------------
	//���[�����̕\���쐬
	///-----------------------------------------------------------------------------
	void ResultStage::CreateCollectedMail() {
		m_mailText = AddGameObject<Sprite>(L"CollectedMail_TX", Vec2(516, 256));
		m_mailText.lock()->SetPosition(Vec2(320.0f, 250.0f));
		//����
		m_mailNum = AddGameObject<NumberSprite>(2, (GameManager::GetInstance().GetMail()));
		m_mailNum.lock()->GetComponent<Transform>()->SetPosition(880, -160, 0);
		m_mailP1 = m_mailNum.lock()->GetComponent<Transform>()->GetPosition();
	};
	///-----------------------------------------------------------------------------
	//�}�b�N�X�`�F�C���̕\���쐬
	///-----------------------------------------------------------------------------
	void ResultStage::CreateMaximumChain() {
		//����
		m_maxChainText = AddGameObject<Sprite>(L"MaxChainText_TX", Vec2(461, 230));
		m_maxChainText.lock()->SetPosition(Vec2(345.0f, 400.0f));
		//����
		m_maxChainNum = AddGameObject<NumberSprite>(2, GameManager::GetInstance().GetMaxChain());
		m_maxChainNum.lock()->GetComponent<Transform>()->SetPosition(880, -320, 0);
		m_maxChainP1 = m_maxChainNum.lock()->GetComponent<Transform>()->GetPosition();
	};
	///-----------------------------------------------------------------------------
	//�X�R�A�̕\���쐬
	///-----------------------------------------------------------------------------
	void ResultStage::CreateScore() {
		//����
		m_scoreText = AddGameObject<Sprite>(L"ScoreText_TX", Vec2(410, 205));
		m_scoreText.lock()->SetPosition(Vec2(370.0f, 540.0f));
		//����
		m_scoreNum = AddGameObject<ScoreUI>();
		m_scoreNum.lock()->GetComponent<Transform>()->SetPosition(750, -450, 0);
		m_scoreNum.lock()->CountSkip();//�\���X�R�A������X�R�A�܂ŏグ�Ă���
		m_scoreP2 = m_scoreNum.lock()->GetComponent<Transform>()->GetPosition() + Vec3(50.0f,0,0);
	};
	///-----------------------------------------------------------------------------
	//�X�R�A�����o��ɑ��₷
	///-----------------------------------------------------------------------------
	void ResultStage::AddScore() {
		m_leap += App::GetApp()->GetElapsedTime();
		if (m_leap >= 1.0f) {
			auto& gm = GameManager::GetInstance();
			m_leap = 1.0f;
			GameManager::GetInstance().AddScore(((5 - gm.GetCheckPointNum()) * gm.GetMaxChain()) * 10);
			m_progress = progress::SCORE_COUNTUP;
			m_mailNum.lock()->SetDrawActive(false);
			m_maxChainNum.lock()->SetDrawActive(false);
		}
		Vec3 chackPos = (1 - m_leap) * m_mailP1 + m_leap * m_scoreP2;
		m_mailNum.lock()->GetComponent<Transform>()->SetPosition(chackPos);
		Vec3 chainPos = (1 - m_leap) * m_maxChainP1 + m_leap * m_scoreP2;
		m_maxChainNum.lock()->GetComponent<Transform>()->SetPosition(chainPos);
	}
	///-----------------------------------------------------------------------------
	//�v���C���[����ʂɏo��
	///-----------------------------------------------------------------------------
	void ResultStage::CreatePlayer(){
		m_player = AddGameObject<GameObject>();
		m_player.lock()->SetDrawLayer(100);
		auto transComp = m_player.lock()->GetComponent<Transform>();
		transComp->SetPosition(-4.0f, -4.0f, -5.0f);
		transComp->SetScale(1.5f, 1.5f, 1.5f);
		auto drawComp = m_player.lock()->AddComponent<BcPNTBoneModelDraw>();
		drawComp->SetMultiMeshResource(L"PLAYER_MODEL");
		drawComp->SetTextureResource(L"PLAYER_TX");
		drawComp->SetLightingEnabled(false);
		// �A�j���[�V������ǉ�����
		drawComp->AddAnimation(L"Clear", 400, 100, true, 30.0f);
		// �A�j���[�V�����̐ݒ�
		drawComp->ChangeCurrentAnimation(L"Clear");
		drawComp->SetMultiMeshIsDraw(0, false); // ��ԏ�̐�
		drawComp->SetMultiMeshIsDraw(1, false);	// �^�񒆂̐�
		drawComp->SetMultiMeshIsDraw(2, false);	// ��ԉ��̐�
		drawComp->SetMultiMeshIsDraw(3, false);	// �ʏ��
		drawComp->SetMultiMeshIsDraw(4, true);	// ���ꂵ��
		drawComp->SetMultiMeshIsDraw(5, true); // ��
		drawComp->SetMultiMeshIsDraw(6, false);	// �Q�Ă��
		drawComp->SetMultiMeshIsDraw(7, false);	// �߂�����
	}

	void ResultStage::FadeProcess()
	{
		auto fade = AddGameObject<FadeInOut>(Vec2(640, 400), Vec2(1280, 800));
		fade->SetIsFadeOut(true);
		auto& gm = GameManager::GetInstance();
		//�t�F�[�h�����ǂ���
		if (!gm.GetIsFade())
		{
			gm.SetIsFade(true);
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
		}
	}
}