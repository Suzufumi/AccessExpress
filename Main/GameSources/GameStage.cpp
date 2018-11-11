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

		auto ptrCamera = ObjectFactory::Create<Camera>();
		// �f�o�b�O�p�A���O��(�X�e�[�W�S�̂�����)
		//ptrView->SetCamera(ptrCamera);
		//ptrCamera->SetEye(Vec3(0.0f, 60.0f, -100.0f));
		//ptrCamera->SetAt(Vec3(0.0f, -30.0f, 50.0f));

		//�r���[�̃J�����̐ݒ�
		//auto ptrCamera = ObjectFactory::Create<Camera>();
		ptrView->SetCamera(ptrCamera);
		ptrCamera->SetEye(Vec3(0.0f, 10.0f, -5.0f));
		ptrCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
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
		drawComp->SetOwnShadowActive(true);
		Col4 Color(1.0f, 1.0f, 1.0f, 0.7f);
		drawComp->SetColorAndAlpha(Color);
	}
	//----------------------------------------------------------------
	//�v���C���[�֌W�̍쐬
	//----------------------------------------------------------------
	void GameStage::CreatePlayerRelationship() {
		//�v���C���[
		auto player = AddGameObject<Player>(Vec3(-3.0f, 1.0f, 0.0f), Quat(0, 0, 0, 1), Vec3(1, 2, 1));
		//�v���C���[���g���d�g���Ƃ̔�����̓����蔻��
		auto radioTowerHitJudgment = AddGameObject<RadioTowerHitJudgment>(player);
		//�v���C���[�ɓd�g���Ƃ̓����蔻���F�m������
		player->SetRadioTowerHitJudgment(radioTowerHitJudgment);
	}

	void GameStage::CreateStage()
	{
		struct BillTrans
		{
			Vec3 m_pos;
			Vec3 m_scale;
		};
		// �z��̃T�C�Y(�����̐�)
		const int BILL_COUNT = 36;
		BillTrans billParam[BILL_COUNT] = 
		{
			{ Vec3(-35.0f, 4.0f, -35.0f), Vec3(5.0f, 8.0f, 5.0f) }, //0
			{ Vec3(-35.0f, 4.0f, -20.0f), Vec3(4.0f, 8.0f, 5.0f) }, //1
			{ Vec3(-35.0f, 2.5f, -10.0f), Vec3(4.0f, 5.0f, 3.0f) },	//2
			{ Vec3(-35.0f, 3.0f, 0.0f),   Vec3(4.0f, 6.0f, 4.0f) },	//3
			{ Vec3(-35.0f, 4.0f, 10.0f),  Vec3(4.0f, 8.0f, 5.0f) }, //4
			{ Vec3(-35.0f, 2.5f, 25.0f),  Vec3(4.0f, 5.0f, 4.0f) }, //5

			{ Vec3(-23.0f, 3.0f, -35.0f), Vec3(4.0f, 6.0f, 4.0f) }, //6 
			{ Vec3(-23.0f, 3.5f, -25.0f), Vec3(5.0f, 7.0f, 3.0f) }, //7 
			{ Vec3(-23.0f, 4.0f, -15.0f), Vec3(4.0f, 8.0f, 4.0f) }, //8 
			{ Vec3(-23.0f, 2.5f, -5.0f),  Vec3(4.0f, 5.0f, 3.0f) }, //9 
			{ Vec3(-23.0f, 3.5f, 5.0f),   Vec3(5.0f, 7.0f, 3.0f) }, //10 
			{ Vec3(-23.0f, 3.0f, 15.0f),  Vec3(4.0f, 6.0f, 3.0f) }, //11

			{ Vec3(-10.0f, 3.0f, -35.0f), Vec3(4.0f, 6.0f, 3.0f) }, //13
			{ Vec3(-10.0f, 2.5f, -25.0f), Vec3(5.0f, 5.0f, 4.0f) }, //14
			{ Vec3(-10.0f, 4.0f, -10.0f), Vec3(5.0f, 8.0f, 5.0f) }, //15
			{ Vec3(-10.0f, 3.0f, 0.0f),   Vec3(6.0f, 6.0f, 4.0f) }, //16
			{ Vec3(-10.0f, 3.5f, 10.0f),  Vec3(5.0f, 7.0f, 3.0f) }, //17
			{ Vec3(-10.0f, 3.0f, 20.0f),  Vec3(4.0f, 6.0f, 4.0f) }, //18

			{ Vec3(7.0f, 4.0f, -35.0f), Vec3(5.0f, 8.0f, 5.0f) }, //20
			{ Vec3(7.0f, 3.5f, -25.0f), Vec3(5.0f, 7.0f, 3.0f) }, //21
			{ Vec3(7.0f, 3.0f, -15.0f), Vec3(4.0f, 6.0f, 4.0f) }, //22
			{ Vec3(7.0f, 4.0f, -5.0f),  Vec3(6.0f, 8.0f, 5.0f) }, //23
			{ Vec3(7.0f, 3.0f, 5.0f),   Vec3(4.0f, 6.0f, 6.0f) }, //24
			{ Vec3(7.0f, 2.5f, 20.0f),  Vec3(5.0f, 5.0f, 5.0f) }, //25

			{ Vec3(20.0f, 3.0f, -35.0f),  Vec3(6.0f, 6.0f, 6.0f) }, //26
			{ Vec3(20.0f, 2.5f, -25.0f),  Vec3(6.0f, 5.0f, 4.0f) }, //27
			{ Vec3(20.0f, 4.0f, -15.0f),  Vec3(4.0f, 8.0f, 5.0f) }, //28
			{ Vec3(20.0f, 3.0f, -5.0f),   Vec3(4.0f, 6.0f, 4.0f) }, //29
			{ Vec3(20.0f, 3.5f, 5.0f),    Vec3(6.0f, 7.0f, 5.0f) }, //30
			{ Vec3(20.0f, 3.5f, 15.0f),   Vec3(5.0f, 7.0f, 5.0f) }, //31

			{ Vec3(33.0f, 4.0f, -35.0f), Vec3(6.0f, 8.0f, 5.0f) }, //32
			{ Vec3(33.0f, 4.0f, -25.0f), Vec3(6.0f, 8.0f, 5.0f) }, //33
			{ Vec3(33.0f, 3.0f, -15.0f), Vec3(5.0f, 6.0f, 6.0f) }, //34
			{ Vec3(33.0f, 3.5f, -5.0f),  Vec3(5.0f, 7.0f, 4.0f) }, //35
			{ Vec3(33.0f, 4.0f, 5.0f),   Vec3(4.0f, 8.0f, 5.0f) }, //36
			{ Vec3(33.0f, 3.0f, 15.0f),  Vec3(6.0f, 6.0f, 4.0f) }  //37

			//{ Vec3(20.0f, 4.0f, -15.0f), Vec3(4.0f, 8.0f, 5.0f) } // Goal
		};

		for (auto bill : billParam)
		{
			AddGameObject<Wall>(bill.m_pos, bill.m_scale);
		}
	}

	void GameStage::OnCreate() {
		try {
			//�����v�Z�L��
			SetPhysicsActive(true);
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			//��
			CreateFloor();
			//�v���C���[�֌W
			CreatePlayerRelationship();
			//��
			//for (int i = 0; i < 5; i++)
			//{
			//	for (int j = 0; j < 5; j++)
			//	{
			//		AddGameObject<Wall>(Vec3(-30.0 + ((float)i*10.0), 4.5, -30.0 + ((float)j * 10.0f)), Vec3(3, 8, 3));
			//	}
			//}

			CreateStage();

			Quat qt;

			AddGameObject<SearchObject>(Vec3(-3.0f, 1.0f, 5.0f), Quat(qt.identity()), Vec3(1.0f, 1.0f, 1.0f));

			auto linkA = AddGameObject<LinkObject>(Vec3(-4, 1, 0), Vec3(1, 1, 1));
			auto linkB = AddGameObject<LinkObject>(Vec3(-4, 1, 10), Vec3(1, 1, 1));
			linkA->SetGoPosition(linkB->GetComponent<Transform>()->GetWorldPosition());
			linkB->SetGoPosition(linkA->GetComponent<Transform>()->GetWorldPosition());

			AddGameObject<RemainingTimer>();
			//�d�g��
			//for (int i = 0; i < 3; i++)
			//{
			//	for (int j = 0; j < 3; j++)
			//	{
			//		AddGameObject<RadioTower>(Vec3(-10.0f + ((float)i * 10.0f), 5.0f, -10.0f + ((float)j * 10.0f)), Vec3(1.5, 1.5, 1.5), float(15.0f));
			//	}
			//}

			//�h���C�u
			//AddGameObject<Drive>(Vec3(-3.0f, 1.0f, 2.0f), Vec3(1, 1, 1));
			//AddGameObject<Drive>(Vec3(-6.0f, 1.0f, 10.0f), Vec3(1, 1, 1));
			//AddGameObject<Drive>(Vec3(7.0f, 1.0f, 5.0f), Vec3(1, 1, 1));
			//AddGameObject<Drive>(Vec3(-12.0f, 1.0f, 10.0f), Vec3(1, 1, 1));
			//AddGameObject<Drive>(Vec3(-7.0f, 1.0f, 40.0f), Vec3(1, 1, 1));
			////�t�@�C��
			//AddGameObject<File>(Vec3(-4.0f, 1.0f, 2.0f), Vec3(1, 1, 1));
		}
		catch (...) {
			throw;
		}
	}
}
//end basecross
