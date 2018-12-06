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
		//�v���C���[
		auto player = AddGameObject<Player>(Vec3(-35.0f, 35.0f, -37.0f), Quat(0, 0, 0, 1), Vec3(1, 2, 1));
		auto dev = AddGameObject<SightingDevice>();
		player->SetSightingDevice(dev);
		SetSharedGameObject(L"Player", player);
	}

	void GameStage::CreateBill()
	{
		struct BillTrans
		{
			Vec3 m_pos;
			Vec3 m_scale;
		};

		BillTrans billParam[] = 
		{
			{ Vec3(-35.0f, 1.0f, -35.0f), Vec3(20.0f, 8.0f, 20.0f) }, // 0
			{ Vec3(-30.0f, 1.0f, -10.0f), Vec3(5.0f, 7.0f, 5.0f) }, // 1
			{ Vec3(-40.0f, 1.0f, -10.0f), Vec3(5.0f, 7.0f, 5.0f) },	// 2
			{ Vec3(-38.0f, 1.0f, 10.0f), Vec3(5.0f, 7.0f, 5.0f) },	// 3
			{ Vec3(-25.0f, 1.0f, 10.0f), Vec3(5.0f, 6.0f, 5.0f) },	// 3
			{ Vec3(-35.0f, 1.0f, 35.0f),   Vec3(25.0f, 5.0f, 25.0f) }, // 4
			{ Vec3(-5.0f, 1.0f, -23.0f),  Vec3(10.0f, 8.0f, 30.0f) }, // 5
			{ Vec3(10.0f, 1.0f, -5.0f),  Vec3(10.0f, 5.0f, 15.0f) }, // 5
			{ Vec3(5.0f, 1.0f, 25.0f),  Vec3(10.0f, 8.0f, 10.0f) }, // 6
			{ Vec3(10.0f, 1.0f, 50.0f),  Vec3(10.0f, 7.0f, 10.0f) }, // 7
			{ Vec3(-5.0f, 1.0f, 40.0f),  Vec3(10.0f, 6.0f, 10.0f) }, // 7
			{ Vec3(35.0f, 1.0f, -25.0f),  Vec3(20.0f, 4.0f, 20.0f) }, // 8
			{ Vec3(38.0f, 1.0f, 5.0f),  Vec3(15.0f, 6.0f, 10.0f) }, // 9
			{ Vec3(35.0f, 1.0f, 35.0f),  Vec3(30.0f, 8.0f, 30.0f) }, // 10	
			{ Vec3(65.0f, 1.0f, 50.0f),  Vec3(20.0f, 3.0f, 20.0f) }, // 11	
		};

		int count = 0;
		for (auto bill : billParam)
		{
			//bill.m_scale.x *= 2.5f;
			bill.m_scale.y *= 3.0f;
			//bill.m_scale.z *= 2.5f;

			bill.m_pos.y = bill.m_scale.y / 2;
			AddGameObject<Wall>(bill.m_pos, bill.m_scale);
			//if (count % 2 == 0)
			//{
			AddGameObject<LinkObject>(
				Vec3(bill.m_pos.x, bill.m_pos.y * 2.0f, bill.m_pos.z),
				Vec3(1.0f, 1.0f, 1.0f));
			//}
			//count++;
		}
	}

	void GameStage::CreateLinkObject()
	{
		AddGameObject<LinkObject>(Vec3(-35.0f, 12.5f, -33.0f), Vec3(1.0f, 1.0f, 1.0f));
		AddGameObject<LinkObject>(Vec3(-35.0f, 12.5f, -20.0f), Vec3(1.0f, 1.0f, 1.0f));
		AddGameObject<LinkObject>(Vec3(-35.0f, 8.5f, -10.0f), Vec3(1.0f, 1.0f, 1.0f));
		AddGameObject<LinkObject>(Vec3(-23.0f, 11.5f, 5.0f), Vec3(1.0f, 1.0f, 1.0f));
		AddGameObject<LinkObject>(Vec3(-23.0f, 11.5f, -25.0f), Vec3(1.0f, 1.0f, 1.0f));
		AddGameObject<LinkObject>(Vec3(-10.0f, 8.5f, -25.0f), Vec3(1.0f, 1.0f, 1.0f));
		AddGameObject<LinkObject>(Vec3(-23.0f, 11.5f, -5.0f), Vec3(1.0f, 1.0f, 1.0f));
		AddGameObject<LinkObject>(Vec3(-10.0f, 11.5f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		AddGameObject<LinkObject>(Vec3(-10.0f, 12.5f, -10.0f), Vec3(1.0f, 1.0f, 1.0f));
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
			//�Q�[�����n�܂��Ă���t���O��؂�
			App::GetApp()->GetScene<Scene>()->SetGameStart(false);
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

			//CreateLinkObject();

			//SetBackGround();

			//Quat qt;
			//AddGameObject<SearchObject>(Vec3(-3.0f, 1.0f, 5.0f), Quat(qt.identity()), Vec3(1.0f, 1.0f, 1.0f));
						
			//auto drone = AddGameObject<Drone>(Vec3(-35, 25, -35),Drone::RoopDirection::ClockWise);
			//drone->SetDeadChain(1);
			//auto drone = AddGameObject<Drone>(Vec3(-35, 25, -35));
			//drone->SetDeadChain(1);
			//AddGameObject<Drone>(Vec3(-35, 25, -35));
			CreateDrone();
			
			auto score = AddGameObject<RemainingTimerSprite>(60);
			score->GetComponent<Transform>()->SetPosition(1280 - 64 * 10, 0, 0);
			AddGameObject<SkySphere>();
			AddGameObject<EnergyGaugeUI>();
		}
		catch (...) {
			throw;
		}
	}
	void GameStage::OnUpdate() {
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		//�X�^�[�g�{�^�����������ƂŃ��U���g�ɍs��
		if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_START) {
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToResultStage");
			auto fade = AddGameObject<FadeInOut>(Vec2(1.0f, 1.0f));
			fade->SetIsAction(true);
		}
	}
}
//end basecross
