/*!
@file GameStage.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------
	void GameStage::CreateViewLight() {
		auto ptrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto ptrCamera = ObjectFactory::Create<TpsCamera>();
		ptrView->SetCamera(ptrCamera);
		//�J��������������ĂȂ��̂ŁA�ʂɔ�����쐬
		AddGameObject<TpsCameraJudgment>();
		//�}���`���C�g�̍쐬
		auto ptrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��x
		ptrMultiLight->SetDefaultLighting();


	}


	//----------------------------------------------------------------
	//���̍쐬
	//----------------------------------------------------------------
	void GameStage::CreateFloor() {
		auto ptr = AddGameObject<GameObject>();
		auto ptrTrans = ptr->GetComponent<Transform>();
		//�l�p�`�̃R���W�����Z�b�g
		auto col = ptr->AddComponent<CollisionObb>();
		//�����Ȃ��悤�ɂ���
		col->SetAfterCollision(AfterCollision::None);
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, 0, 0));
		ptrTrans->SetScale(100.0f, 0.5f, 100.0f);
		ptrTrans->SetQuaternion(Qt);
		ptrTrans->SetWorldPosition(Vec3(0.0f, -0.25f, 0.0f));
		//�`��R���|�[�l���g�̒ǉ�
		auto drawComp = ptr->AddComponent<BcPNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		drawComp->SetMeshResource(L"DEFAULT_CUBE");
		drawComp->SetFogEnabled(true);
		//�����ɉe���f�肱�ނ悤�ɂ���
		drawComp->SetOwnShadowActive(false);
		Col4 Color(1.0f, 1.0f, 1.0f, 0.7f);
		drawComp->SetColorAndAlpha(Color);
	}
	//----------------------------------------------------------------
	//�v���C���[�֌W�̍쐬
	//----------------------------------------------------------------
	void GameStage::CreatePlayerRelationship() {
		GameObjecttXMLBuilder builder;
		// �v���C���[��Xml�ɓo�^
		builder.Register<Player>(L"Player");
		// Xml�t�@�C������L"Player"��T��
		builder.Build(GetThis<Stage>(), m_stageXmlPath, L"GameStage/Player");

		//�v���C���[�֘A
		AddGameObject<ViewChainLetter>();
		auto chainNum = AddGameObject<ViewChainNum>();
		chainNum->GetComponent<Transform>()->SetPosition(Vec3(780, -480, 0));
		AddGameObject<RayRangeViewObj>();
	}

	void GameStage::CreateBill()
	{
		GameObjecttXMLBuilder builder;
		// Wall��Xml�ɓo�^
		builder.Register<Wall>(L"Wall");
		// Xml�t�@�C������Wall��T��
		builder.Build(GetThis<Stage>(), m_stageXmlPath, L"GameStage/Wall");
	}

	void GameStage::CreateLinkObject()
	{
		GameObjecttXMLBuilder builder;
		// LinkObject��Xml�ɓo�^
		builder.Register<LinkObject>(L"RadioTower");
		// Xml�t�@�C������RadioTower��T��
		builder.Build(GetThis<Stage>(), m_stageXmlPath, L"GameStage/RadioTower");
	}

	void GameStage::CreateDrone()
	{
		GameObjecttXMLBuilder builder;
		// Enemy��Xml�ɓo�^
		builder.Register<Drone>(L"Enemy");
		// Xml�t�@�C������Enemy��T��
		builder.Build(GetThis<Stage>(), m_stageXmlPath, L"GameStage/Enemy");
	}

	void GameStage::CreateMail()
	{
		GameObjecttXMLBuilder builder;
		// Main��Xml�ɓo�^
		builder.Register<MailObject>(L"Mail");
		// Xml�t�@�C������Mails��T��
		builder.Build(GetThis<Stage>(), m_stageXmlPath, L"GameStage/Mails");
	}

	void GameStage::CreateEffect()
	{
		auto multiEfkPtr = AddGameObject<GetEffect>();
		//���L�I�u�W�F�N�g�ɃG�t�F�N�g��o�^
		SetSharedGameObject(L"GetEffect", multiEfkPtr);
		auto bonusEfkPtr = AddGameObject<BonusEffect>();
		SetSharedGameObject(L"BonusEffect", bonusEfkPtr);
		auto multiSparkPtr = AddGameObject<SparkEffect>();
		SetSharedGameObject(L"SparkEffect", multiSparkPtr);
	}


	void GameStage::OnCreate() {
		try {
			wstring dataDir;
			auto scenePtr = App::GetApp()->GetScene<Scene>();
			App::GetApp()->GetDataDirectory(dataDir);
			switch (scenePtr->GetStageNum())
			{
			case 1:
				m_stageXmlPath = dataDir + L"Stage\\" + L"1.xml";
				break;
			case 2:
				m_stageXmlPath = dataDir + L"Stage\\" + L"2.xml";
				break;
			default:
				break;
			}
			auto& gm = GameManager::GetInstance();

			XmlDocReader xmlReader(m_stageXmlPath);
			//�Q�[�����n�܂��Ă���t���O��؂�
			gm.SetGameStart(false);
			//�X���[�t���O���I�t�ɂ��Ă���
			gm.SetOnSlow(false);
			//�X�R�A������������
			gm.ResetNowScore();
			//�}�b�N�X�`�F�C��������
			gm.ResetMaxChain();
			//���[����������
			gm.ResetMail();
			// �ő僁�[�����̏�����
			gm.ResetMaxMail();
			CreateSharedObjectGroup(L"Link");
			CreateSharedObjectGroup(L"Drone");
			CreateSharedObjectGroup(L"Mails");
			//�����v�Z�L��
			SetPhysicsActive(true);
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			//��
			//�v���C���[�֌W
			CreatePlayerRelationship();
			// �����̔z�u
			CreateBill();
			CreateLinkObject();

			CreateDrone();
			CreateMail();
			//���[������\������UI
			AddGameObject<MailNumUI>();
			//�������Ԃ��쐬�i������j
			auto timer = AddGameObject<RemainingTimerSprite>(40);
			timer->GetComponent<Transform>()->SetPosition(1280 - 64 * 10, 0, 0);
			//�X�R�AUI���쐬
			auto score = AddGameObject<ScoreUI>();
			score->GetComponent<Transform>()->SetPosition(1280 - 64 * 6, 0, 0);

			AddGameObject<SkyBox>();
			auto scoreSprite = AddGameObject<Sprite>(L"SCORE_TX", Vec2(100, 40));
			scoreSprite->SetPosition(Vec2(1120, 30));
			auto timeSprite = AddGameObject<Sprite>(L"TIME_TX", Vec2(100, 35));
			timeSprite->SetPosition(Vec2(640, 27));
			CreateEffect();
			auto timeOverTx = AddGameObject<Sprite>(L"TIMEUP_TX", Vec2(512, 256));
			timeOverTx->SetDrawActive(false);
			timeOverTx->SetPosition(Vec2(640, 240));
			SetSharedGameObject(L"timeUp", timeOverTx);
		}
		catch (...) {
			throw;
		}
	}
	void GameStage::OnUpdate() {
		//�}�l�[�W���[�����p�b�h�����X�V
		GameManager::GetInstance().UpDatePadData();

		if (GameManager::GetInstance().GetTimeUp()) {
			auto timeOverTx = GetSharedGameObject<Sprite>(L"timeUp");
			if (m_timeOver == 0) {
				timeOverTx->SetDrawActive(true);
				App::GetApp()->GetScene<Scene>()->MusicOnceStart(L"TIMEUP_SE", 1.0f);
			}
			m_timeOver += App::GetApp()->GetElapsedTime();
			if (m_timeOver >= 1.0f) {
				timeOverTx->SetDrawActive(false);
			}
		}
	}
}
//end basecross
