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

	void GameStage::SetBackGround()
	{
		struct backGRParam
		{
			Vec3 m_rot;
			Vec3 m_pos;
		};

		const int BACK_GROUND = 4;
		backGRParam backParam[BACK_GROUND] =
		{
			{ Vec3(0.0f), Vec3(0.0f, 37.5f, 50.0f)},
			{ Vec3(0.0f, Deg2Rad(90), 0.0f), Vec3(-50.0f, 37.5f, 0.0f)},
			{ Vec3(0.0f, Deg2Rad(-90), 0.0f), Vec3(50.0f, 37.5f, 0.0f)},
			{ Vec3(0.0f), Vec3(0.0f, 37.5f, -50.0f)}
		};

		for (auto back : backParam)
		{
			AddGameObject<SkySprite>(back.m_rot, back.m_pos);

		}
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

		//�v���C���[
		//auto player = AddGameObject<Player>(Vec3(pos), Quat(qt), Vec3(scale));
		//auto dev = AddGameObject<SightingDevice>();
		//player->SetSightingDevice(dev);
		//SetSharedGameObject(L"Player", player);
		//AddGameObject<ViewChainLetter>();
		//AddGameObject<ViewChainNum>();
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
		struct DroneParam
		{
			Vec3 m_pos;
			Drone::DroneMotion m_dir;
			int m_needChains;
		};

		DroneParam  drones[] = {
			{ Vec3(-25.0f, 25.0f, -30.0f), Drone::DroneMotion::CounterClockwise, 2},
			{ Vec3(-20.0f, 25.0f, -30.0f), Drone::DroneMotion::ClockWise, 4},
			{ Vec3(-10.0f, 21.0f, 28.0f), Drone::DroneMotion::VerticalMotion, 1},
			{ Vec3(-15.0f, 20.0f, 40.0f), Drone::DroneMotion::VerticalMotion, 2},
			{ Vec3(55.0f, 25.0f, 50.0f), Drone::DroneMotion::CounterClockwise, 3 },
			{ Vec3(5.0f, 18.0f, -23.0f), Drone::DroneMotion::Wave, 2 },
			{ Vec3(37.0f, 15.0f, -10.0f), Drone::DroneMotion::VerticalMotion, 4 },
			{ Vec3(5.0f, 18.0f, 5.0f), Drone::DroneMotion::Wave, 2 },
			{ Vec3(50.0f, 25.0f, 35.0f), Drone::DroneMotion::ClockWise, 2},
		};

		for (auto drone : drones)	{
			auto ptrDrone = AddGameObject<Drone>(drone.m_pos, drone.m_dir, drone.m_needChains);
		}

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
			//�����v�Z�L��
			SetPhysicsActive(true);
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			//��
			//CreateFloor();
			//�v���C���[�֌W
			CreatePlayerRelationship();
			auto goal = AddGameObject<Goal>(Vec3(65.0f, 13.0f, 50.0f), Vec3(1.0f, 30.0f, 1.0f));
			// �����̔z�u
			CreateBill();
			CreateLinkObject();

			//SetBackGround();

			//Quat qt;
			//AddGameObject<SearchObject>(Vec3(-3.0f, 1.0f, 5.0f), Quat(qt.identity()), Vec3(1.0f, 1.0f, 1.0f));
						
			//auto drone = AddGameObject<Drone>(Vec3(-35, 25, -35),Drone::RoopDirection::ClockWise);
			//drone->SetDeadChain(1);
			//auto drone = AddGameObject<Drone>(Vec3(-35, 25, -35));
			//drone->SetDeadChain(1);
			//AddGameObject<Drone>(Vec3(-35, 25, -35));
			CreateDrone();
			
			//�������Ԃ��쐬�i������j
			auto timer = AddGameObject<RemainingTimerSprite>(60);
			timer->GetComponent<Transform>()->SetPosition(1280 - 64 * 10, 0, 0);
			//�X�R�AUI���쐬
			auto score = AddGameObject<ScoreUI>();
			score->GetComponent<Transform>()->SetPosition(1280 - 64 * 3, 0, 0);

			AddGameObject<SkySphere>();
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
