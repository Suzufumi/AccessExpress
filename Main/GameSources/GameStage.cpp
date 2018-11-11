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
		auto ptrCamera = ObjectFactory::Create<Camera>();
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
			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < 5; j++)
				{
					AddGameObject<Wall>(Vec3(-30.0 + ((float)i*10.0), 4.5, -30.0 + ((float)j * 10.0f)), Vec3(3, 8, 3));
				}
			}
			Quat qt;

			AddGameObject<SearchObject>(Vec3(-3.0f, 1.0f, 5.0f), Quat(qt.identity()), Vec3(1.0f, 1.0f, 1.0f));

			auto linkA = AddGameObject<LinkObject>(Vec3(-4, 1, 0), Vec3(1, 1, 1));
			auto linkB = AddGameObject<LinkObject>(Vec3(-4, 1, 10), Vec3(1, 1, 1));
			linkA->SetGoPosition(linkB->GetComponent<Transform>()->GetWorldPosition());
			linkB->SetGoPosition(linkA->GetComponent<Transform>()->GetWorldPosition());


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
