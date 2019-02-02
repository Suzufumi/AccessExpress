#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void ResultStage::OnCreate() {
		GameManager::GetInstance().SetIsFade(true);
		CreateViewLight();
		//�w�i
		auto resultBack = AddGameObject<Sprite>(L"ClearBack_TX", Vec2(1441, 811));
		resultBack->SetPosition(Vec2(640, 400));
		resultBack->SetDrawLayer(-3);
		m_antenna = AddGameObject<CheckPoint>(
			Vec3(0.0f, -5.0f, -3.0f), Quat(0,Deg2Rad(-30), 0, 1), Vec3(2.0f, 2.0f, 2.0f));
		m_antenna.lock()->SetDrawLayer(-1);
		CreatePlayer();
		playerP0 = m_player.lock()->GetComponent<Transform>()->GetWorldPosition();
		playerP1 = m_antenna.lock()->GetComponent<Transform>()->GetWorldPosition() + Vec3(0.0f, 5.0f, 0.0f);
		playerP2 = m_antenna.lock()->GetComponent<Transform>()->GetWorldPosition() + Vec3(3.0f, -1.0f, 0.0f);
		auto multiEfkPtr = AddGameObject<ResultEffect>();
		SetSharedGameObject(L"ResultEfk", multiEfkPtr);
		m_fade = AddGameObject<FadeInOut>(Vec2(640, 400), Vec2(1280, 800));
		//auto obb = AddGameObject<OBBObject>(Vec3(0, 0, 0), Vec3(10, 8, 1));
		//obb->GetComponent<PNTStaticDraw>()->SetTextureResource(L"ResultStage_TX");
	}

	void ResultStage::OnUpdate() {
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		auto& gm = GameManager::GetInstance();
		auto elapsedTime = App::GetApp()->GetElapsedTime();
		auto playerTrs = m_player.lock()->GetComponent<Transform>();
		switch (m_progress){
		case progress::ANTENNA:
			if (m_leap < 1.0f) {
				m_leap += elapsedTime;
				auto pos = (1 - m_leap)*(1 - m_leap) * playerP0 + 2 * (1 - m_leap) * m_leap * playerP1 + m_leap * m_leap * playerP2;
				playerTrs->SetWorldPosition(pos);
			}
			else  {
				//�v���C���[�𐳖ʌ�������
				m_playerForward =
					playerTrs->GetWorldPosition() - GetView()->GetTargetCamera()->GetEye();
				m_playerForward = m_playerForward.normalize();
				//�o�߂�������
				m_leap = 0.0f;
				if (gm.GetMail() > 0) {
					//�N���A�A�j���[�V�����ɂ���
					m_player.lock()->GetComponent<BcPNTBoneModelDraw>()->ChangeCurrentAnimation(L"Clear");
				}
				else {
					m_player.lock()->GetComponent<BcPNTBoneModelDraw>()->ChangeCurrentAnimation(L"Default");
				}
				m_progress = progress::MAIL_UP;
			}
			break;
		case progress::MAIL_UP:
			m_player.lock()->GetBehavior<UtilBehavior>()->RotToHead(m_playerForward, 0.1f);
			//���A�j���[�V�������I���������s
			if (m_player.lock()->GetComponent<BcPNTBoneModelDraw>()->IsTargetAnimeEnd() && !m_isMailFly) {
				m_isMailFly = true;
				m_player.lock()->GetComponent<BcPNTBoneModelDraw>()->ChangeCurrentAnimation(L"Default");
				playerP0 = playerTrs->GetWorldPosition();
				playerP1 = playerP0 + Vec3(0.0f, -10.0f, 0.0f);
				//���[�����P�ł��l�����Ă�����
				if (gm.GetMail() > 0) {
					CreateMail(playerP0, Vec3(2.0f, 2.0f, 2.0f));
					mailP0 = playerP0 + Vec3(-3.0f, 0.0f, 0.0f);
					mailP1 = mailP0 + Vec3(0.0f, 20.0f, 0.0f);
				}
			}
			//���[�����łĂ����珈��
			if (m_isMailFly) {
				m_leap += elapsedTime / 2.0f;
				auto playerPos = (1 - m_leap) * playerP0 + m_leap * playerP1;
				playerTrs->SetWorldPosition(playerPos);
				m_antenna.lock()->GetComponent<Transform>()->SetWorldPosition(playerPos + Vec3(-3.0f,0.0f,-2.0f));
				//���[�����P�ł��l�����Ă�����
				if (gm.GetMail() > 0) {
					auto mailPos = (1 - m_leap) * mailP0 + m_leap * mailP1;
					m_mail.lock()->GetComponent<Transform>()->SetWorldPosition(Vec3(mailPos.x, mailPos.y - 2.0f, mailPos.z));

					auto particle = GetSharedGameObject<ResultEffect>(L"ResultEfk", false);
					if (particle)
					{
						//auto mailPos = m_mail.lock()->GetComponent<Transform>()->GetWorldPosition();
						particle->InsertResultEffect(Vec3(mailPos.x, mailPos.y - 3.0f, mailPos.z), Vec2(2.0f, 2.0f));
					}
				}
				if (m_leap >= 1.0f) {
					m_leap = 0.0f;
					CreateResult();
					m_progress = progress::START;
				}
			}
			break;
		case progress::START:
			if (CntlVec[0].wPressedButtons) {
				m_progress = progress::ADD_SCORE;
				m_push.lock()->SetDrawActive(false);
			}
			break;
		case progress::ADD_SCORE :
			m_leap += App::GetApp()->GetElapsedTime();
			AddScore();
			break;
		case progress::SCORE_COUNTUP :
			CountupScore();
			break;
		case progress::END :
			ResultProcess(gm.GetScore());
			break;
		case progress::FADE:
			FadeProcess();
			break;
		default:
			break;
		}
		m_player.lock()->GetComponent<BcPNTBoneModelDraw>()->UpdateAnimation(elapsedTime);

	}
	void ResultStage::CreateViewLight() {
		auto ptrView = CreateView<SingleView>();
		//�}���`���C�g�̍쐬
		auto ptrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		ptrMultiLight->SetDefaultLighting();
	}
	///-----------------------------------------------------------------------------
	//���U���g�֘A�̍쐬
	///-----------------------------------------------------------------------------
	void ResultStage::CreateResult() {
		//�z�B�����̕���
		if (GameManager::GetInstance().GetMail() > 0.0f) {
			m_clearSprite = AddGameObject<Sprite>(L"ResultStage_TX", Vec2(410, 205));
		}
		else {
			m_clearSprite = AddGameObject<Sprite>(L"sippai_TX", Vec2(410, 205));
		}
		m_clearSprite.lock()->SetPosition(Vec2(640.0f, 100.0f));

		//�v�b�V���G�j�[�{�^��
		m_push = AddGameObject<Sprite>(L"Title_BUTTON_TX", Vec2(1000, 100));
		m_push.lock()->SetPosition(Vec2(640.0f, 670.0f));
		CreateCollectedMail();
		CreateMaximumChain();
		CreateScore();
		CreateRank();
	}
	///-----------------------------------------------------------------------------
	//���[�����̕\���쐬
	///-----------------------------------------------------------------------------
	void ResultStage::CreateCollectedMail() {
		m_mailText = AddGameObject<Sprite>(L"CollectedMail_TX", Vec2(516, 256));
		m_mailText.lock()->SetPosition(Vec2(320.0f, 250.0f));
		//����
		m_mailNum = AddGameObject<NumberSprite>(2, (GameManager::GetInstance().GetMail()));
		m_mailNum.lock()->GetComponent<Transform>()->SetPosition(940, -160, 0);
		m_mailP0 = m_mailNum.lock()->GetComponent<Transform>()->GetPosition();
	};
	///-----------------------------------------------------------------------------
	//�}�b�N�X�`�F�C���̕\���쐬
	///-----------------------------------------------------------------------------
	void ResultStage::CreateMaximumChain() {
		//����
		m_maxChainText = AddGameObject<Sprite>(L"MaxChainText_TX", Vec2(461, 230));
		m_maxChainText.lock()->SetPosition(Vec2(345.0f, 400.0f));
		//����
		m_maxChainNum = AddGameObject<NumberSprite>(3, GameManager::GetInstance().GetMaxChain());
		m_maxChainNum.lock()->GetComponent<Transform>()->SetPosition(940, -320, 0);
		m_maxChainP0 = m_maxChainNum.lock()->GetComponent<Transform>()->GetPosition();
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
		m_scoreNum.lock()->GetComponent<Transform>()->SetPosition(590, -450, 0);
		m_scoreNum.lock()->CountSkip();//�\���X�R�A������X�R�A�܂ŏグ�Ă���
		m_scoreP1 = m_scoreNum.lock()->GetComponent<Transform>()->GetPosition() + Vec3(200.0f,0,0);
	};

	///-----------------------------------------------------------------------------
	//�����N�̕\���쐬
	///-----------------------------------------------------------------------------
	void ResultStage::CreateRank()
	{
		float width = static_cast<float>(App::GetApp()->GetGameWidth());
		m_rankText = AddGameObject<Sprite>(L"RANK_TEXT_TX", Vec2(205, 102));
		m_rankText.lock()->SetPosition(Vec2(960, 400));
		m_rankText.lock()->SetDrawActive(false);
		// �e�����N�̐ݒ�
		m_rankResult.push_back(AddGameObject<Sprite>(L"RANK_S_TX", Vec2(205, 205)));
		m_rankResult.push_back(AddGameObject<Sprite>(L"RANK_A_TX", Vec2(205, 205)));
		m_rankResult.push_back(AddGameObject<Sprite>(L"RANK_B_TX", Vec2(205, 205)));
		m_rankResult.push_back(AddGameObject<Sprite>(L"RANK_C_TX", Vec2(205, 205)));
		for (auto rankResult : m_rankResult)
		{
			rankResult.lock()->SetPosition(Vec2(700, 320));
			rankResult.lock()->SetDrawActive(false);
		}
		// �����N�ɉ������\��e�N�X�`���̐ݒ�
		m_rankFace.push_back(AddGameObject<Sprite>(L"RESULT_S_TX", Vec2(307, 307)));
		m_rankFace.push_back(AddGameObject<Sprite>(L"RESULT_A_TX", Vec2(307, 307)));
		m_rankFace.push_back(AddGameObject<Sprite>(L"RESULT_B_TX", Vec2(307, 307)));
		m_rankFace.push_back(AddGameObject<Sprite>(L"RESULT_C_TX", Vec2(307, 307)));
		for (auto rankFace : m_rankFace)
		{
			rankFace.lock()->SetPosition(Vec2(380, 340));
			rankFace.lock()->SetDrawActive(false);
		}
	}

	///-----------------------------------------------------------------------------
	//�X�R�A�����o��ɑ��₷
	///-----------------------------------------------------------------------------
	void ResultStage::AddScore() {
		if (m_leap >= 1.0f) {
			auto& gm = GameManager::GetInstance();
			m_leap = 1.0f;
			GameManager::GetInstance().AddScore((gm.GetMail() * gm.GetMaxChain()) * 10);
			m_progress = progress::SCORE_COUNTUP;
			m_mailNum.lock()->SetDrawActive(false);
			m_maxChainNum.lock()->SetDrawActive(false);
		}
		Vec3 chackPos = (1 - m_leap) * m_mailP0 + m_leap * m_scoreP1;
		m_mailNum.lock()->GetComponent<Transform>()->SetPosition(chackPos);
		Vec3 chainPos = (1 - m_leap) * m_maxChainP0 + m_leap * m_scoreP1;
		m_maxChainNum.lock()->GetComponent<Transform>()->SetPosition(chainPos);
	}
		

	void ResultStage::CountupScore()
	{
		m_time += App::GetApp()->GetElapsedTime();
		if (m_time >= 1.2f)
		{
			auto scenePtr = App::GetApp()->GetScene<Scene>();
			m_audioPtr = scenePtr->MusicOnceStart(L"Countup_SE", 1.0f);
			m_time = 0.0f;
		}
		if (m_scoreNum.lock()->GetFinishCountUp()) {
			//m_scoreNum.lock()->GetComponent<Transform>()->SetPosition(750, -470, 0);
			m_push.lock()->SetDrawActive(true);
			m_mailText.lock()->SetDrawActive(false);
			m_mailNum.lock()->SetDrawActive(false);
			m_maxChainText.lock()->SetDrawActive(false);
			m_maxChainNum.lock()->SetDrawActive(false);
			auto audioMa = App::GetApp()->GetXAudio2Manager();
			audioMa->Stop(m_audioPtr.lock());
			m_isCountUpEnd = true;
			m_scoreText.lock()->SetScale(Vec2(256, 128));
			m_scoreText.lock()->SetPosition(Vec2(480, 585));
			//m_scoreNum.lock()->GetComponent<Transform>()->SetPosition()
			//CreatePlayer();
			m_progress = progress::END;
		}

	}
	///-----------------------------------------------------------------------------
	// �ŏI�X�R�A�\���ƃ����N�\��
	///-----------------------------------------------------------------------------
	void ResultStage::ResultProcess(int resultScore)
	{
		if (m_isCountUpEnd)
		{
			auto balloon = AddGameObject<Sprite>(L"BALLOON_TX", Vec2(338, 293));
			balloon->SetPosition(Vec2(690, 320));
			balloon->SetDrawLayer(-1);
			m_isCountUpEnd = false;
		}
		ShowRank(resultScore);
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].wPressedButtons) {
			auto scenePtr = App::GetApp()->GetScene<Scene>();
			scenePtr->MusicOnceStart(L"Decision_SE", 1.0f);
			m_fade.lock()->SetIsFadeOut(true);
			m_progress = progress::FADE;
		}
	}

	///-----------------------------------------------------------------------------
		//�ŏI�]�������N
	///-----------------------------------------------------------------------------
	void ResultStage::ShowRank(int resultScore)
	{
		m_rankText.lock()->SetDrawActive(true);
		//if (resultScore >= RANK_SS)
		//{
		//	m_rankResult[0].lock()->SetDrawActive(true);
		//	m_rankResult[0].lock()->SetScale(Vec2(154, 154));
		//	//m_rankResult[0].lock()->SetDrawActive(true);
		//	//m_rankResult[0].lock()->SetScale(Vec2(154, 154));

		//}
		if (resultScore >= RANK_S)
		{
			m_rankResult[0].lock()->SetDrawActive(true);
			m_rankFace[0].lock()->SetDrawActive(true);
		}
		else if (resultScore >= RANK_A && resultScore < RANK_S)
		{
			m_rankResult[1].lock()->SetDrawActive(true);
			m_rankFace[1].lock()->SetDrawActive(true);
		}
		else if (resultScore >= RANK_B && resultScore < RANK_A)
		{
			m_rankResult[2].lock()->SetDrawActive(true);
			m_rankFace[2].lock()->SetDrawActive(true);
		}
		else
		{
			m_rankResult[3].lock()->SetDrawActive(true);
			m_rankFace[3].lock()->SetDrawActive(true);
		}

	}


	///-----------------------------------------------------------------------------
	//�v���C���[����ʂɏo��
	///-----------------------------------------------------------------------------
	void ResultStage::CreatePlayer(){
		m_player = AddGameObject<GameObject>();
		m_player.lock()->SetDrawLayer(100);
		auto transComp = m_player.lock()->GetComponent<Transform>();
		transComp->SetPosition(15.0f, -1.0f, -5.0f);
		transComp->SetScale(1.5f, 1.5f, 1.5f);
		transComp->SetQuaternion(Quat(0.0f, Deg2Rad(90.0f), 0.0f, 1));
		auto drawComp = m_player.lock()->AddComponent<BcPNTBoneModelDraw>();
		drawComp->SetMultiMeshResource(L"PLAYER_MODEL");
		drawComp->SetTextureResource(L"PLAYER_TX");
		drawComp->SetLightingEnabled(false);
		// �A�j���[�V������ǉ�����
		drawComp->AddAnimation(L"Default", 0, 110, false, 60.0f);
		drawComp->AddAnimation(L"Fly", 310, 30, false, 17.0f);
		drawComp->AddAnimation(L"Over", 350, 40, false, 30.0f);
		drawComp->AddAnimation(L"Clear", 400, 100, false, 30.0f);
		// �A�j���[�V�����̐ݒ�
		drawComp->ChangeCurrentAnimation(L"Fly");
		if (GameManager::GetInstance().GetMail() > 0) {
			drawComp->SetMultiMeshIsDraw(0, false); // ��ԏ�̐�
			drawComp->SetMultiMeshIsDraw(1, false);	// �^�񒆂̐�
			drawComp->SetMultiMeshIsDraw(2, false);	// ��ԉ��̐�
			drawComp->SetMultiMeshIsDraw(3, false);	// �ʏ��
			drawComp->SetMultiMeshIsDraw(4, true);	// ���ꂵ��
			drawComp->SetMultiMeshIsDraw(5, true); // ��
			drawComp->SetMultiMeshIsDraw(6, false);	// �Q�Ă��
			drawComp->SetMultiMeshIsDraw(7, false);	// �߂�����
		}
		else {
			drawComp->SetMultiMeshIsDraw(0, false); // ��ԏ�̐�
			drawComp->SetMultiMeshIsDraw(1, false);	// �^�񒆂̐�
			drawComp->SetMultiMeshIsDraw(2, false);	// ��ԉ��̐�
			drawComp->SetMultiMeshIsDraw(3, false);	// �ʏ��
			drawComp->SetMultiMeshIsDraw(4, false);	// ���ꂵ��
			drawComp->SetMultiMeshIsDraw(5, true); // ��
			drawComp->SetMultiMeshIsDraw(6, false);	// �Q�Ă��
			drawComp->SetMultiMeshIsDraw(7, true);	// �߂�����
		}
	}
	///-----------------------------------------------------------------------------
	//���[������ʂɏo��
	///-----------------------------------------------------------------------------
	void ResultStage::CreateMail(Vec3 position,Vec3 scale) {
		m_mail = AddGameObject<GameObject>();
		auto ptrTrans = m_mail.lock()->GetComponent<Transform>();
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, 0, 0));
		ptrTrans->SetWorldPosition(position);
		ptrTrans->SetQuaternion(Qt);
		ptrTrans->SetScale(scale);
		m_mail.lock()->SetDrawLayer(1);

		Mat4x4 spanMat; // ���f���ƃg�����X�t�H�[���̊Ԃ̍����s��
		spanMat.affineTransformation(
			Vec3(1.5f, 1.5f, 1.5f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//�`��R���|�[�l���g�̒ǉ�
		auto drawComp = m_mail.lock()->AddComponent<BcPNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		drawComp->SetMeshResource(L"MAIL_MODEL");
		drawComp->SetTextureResource(L"MAIL_TX");
		drawComp->SetMeshToTransformMatrix(spanMat);
		drawComp->SetLightingEnabled(false);

	}

	void ResultStage::FadeProcess()
	{
		auto& gm = GameManager::GetInstance();
		//MessageBox(NULL, L"", L"", MB_OK);
		//�t�F�[�h�����ǂ���
		if (!m_fade.lock()->GetIsFadeOut())
		{
			gm.SetIsFade(true);
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
		}
	}
}