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

		//auto ptrCamera = ObjectFactory::Create<Camera>();
		// �f�o�b�O�p�A���O��(�X�e�[�W�S�̂�����)
		//ptrView->SetCamera(ptrCamera);
		//ptrCamera->SetEye(Vec3(0.0f, 60.0f, -100.0f));
		//ptrCamera->SetAt(Vec3(0.0f, -30.0f, 50.0f));

		//�r���[�̃J�����̐ݒ�
		auto ptrCamera = ObjectFactory::Create<TpsCamera>();
		ptrView->SetCamera(ptrCamera);
		//ptrCamera->SetEye(Vec3(0.0f, 10.0f, -5.0f));
		//ptrCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
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
		AddGameObject<SlowTimeUI>();
		AddGameObject<RayRangeViewObj>();
	}

	void GameStage::CreateBill()
	{
		GameObjecttXMLBuilder builder;
		builder.Register<Wall>(L"Wall");
		builder.Build(GetThis<Stage>(), m_stageXmlPath, L"GameStage/Wall");
	}

	void GameStage::CreateLinkObject()
	{
		GameObjecttXMLBuilder builder;
		builder.Register<LinkObject>(L"RadioTower");
		builder.Build(GetThis<Stage>(), m_stageXmlPath, L"GameStage/RadioTower");
	}

	void GameStage::CreateDrone()
	{
		GameObjecttXMLBuilder builder;
		builder.Register<Drone>(L"Enemy");
		builder.Build(GetThis<Stage>(), m_stageXmlPath, L"GameStage/Enemy");
	}

	void GameStage::CreateGoal()
	{
		GameObjecttXMLBuilder builder;
		builder.Register<Goal>(L"Goal");
		builder.Build(GetThis<Stage>(), m_stageXmlPath, L"GameStage/Goal");
	}

	void GameStage::OnCreate() {
		try {
			wstring dataDir;
			App::GetApp()->GetDataDirectory(dataDir);
			m_stageXmlPath = dataDir + L"Stage\\" + L"1.xml";

			XmlDocReader xmlReader(m_stageXmlPath);
			//�Q�[�����n�܂��Ă���t���O��؂�
			App::GetApp()->GetScene<Scene>()->SetGameStart(false);
			//�X���[�t���O���I�t�ɂ��Ă���
			GameManager::GetInstance().SetOnSlow(false);
			//�X�R�A������������
			GameManager::GetInstance().ResetNowScore();
			CreateSharedObjectGroup(L"Link");
			CreateSharedObjectGroup(L"Drone");
			CreateSharedObjectGroup(L"CheckPoints");
			//�����v�Z�L��
			SetPhysicsActive(true);
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			//��
			//CreateFloor();
			//�v���C���[�֌W
			CreatePlayerRelationship();
			// �����̔z�u
			CreateBill();
			CreateLinkObject();

			CreateDrone();
			
			//�������Ԃ��쐬�i������j
			auto timer = AddGameObject<RemainingTimerSprite>(60);
			timer->GetComponent<Transform>()->SetPosition(1280 - 64 * 10, 0, 0);
			//�X�R�AUI���쐬
			auto score = AddGameObject<ScoreUI>();
			score->GetComponent<Transform>()->SetPosition(1280 - 64 * 3, 0, 0);

			CreateGoal();
			AddGameObject<SkyBox>();
			AddGameObject<EnergyGaugeUI>();
		}
		catch (...) {
			throw;
		}
	}
	void GameStage::OnUpdate() {
		//�}�l�[�W���[�����p�b�h�����X�V
		GameManager::GetInstance().UpDatePadData();

		//�X�^�[�g�{�^�����������ƂŃ��U���g�ɍs��
		if (GameManager::GetInstance().GetPad().wPressedButtons & XINPUT_GAMEPAD_START
			&& App::GetApp()->GetScene<Scene>()->GetGameStart()) {
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToResultStage");
			auto fade = AddGameObject<FadeInOut>(Vec2(1.0f, 1.0f));
			fade->SetIsAction(true);
		}
	}
}
//end basecross
