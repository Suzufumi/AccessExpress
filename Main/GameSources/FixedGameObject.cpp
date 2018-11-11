#include "stdafx.h"
#include "Project.h"

namespace basecross {
	Wall::Wall(const shared_ptr<Stage>& StagePtr, Vec3 pos, Vec3 scale)
		: GameObject(StagePtr), m_position(pos), m_scale(scale)
	{
	}
	//--------------------------------------------------------------------------------------
	//������
	//-------------------------------------------------------------------------------------
	void Wall::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		auto col = AddComponent<CollisionObb>();
		col->SetAfterCollision(AfterCollision::None);
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, 0, 0));
		ptrTrans->SetWorldPosition(m_position);
		ptrTrans->SetQuaternion(Qt);
		ptrTrans->SetScale(m_scale);
		//�`��R���|�[�l���g�̒ǉ�
		auto drawComp = AddComponent<PNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		drawComp->SetMeshResource(L"DEFAULT_CUBE");
		Col4 Color(0.4f, 0.4f, 0.4f, 1.0f);
		drawComp->SetDiffuse(Color);
	}

	Goal::Goal(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale)
		: GameObject(stagePtr), m_position(pos), m_scale(scale)
	{
	}

	void Goal::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		auto col = AddComponent<CollisionObb>();
		col->SetAfterCollision(AfterCollision::None);
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, 0, 0));
		ptrTrans->SetWorldPosition(m_position);
		ptrTrans->SetQuaternion(Qt);
		ptrTrans->SetScale(m_scale);
		//�`��R���|�[�l���g�̒ǉ�
		auto drawComp = AddComponent<PNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		drawComp->SetMeshResource(L"DEFAULT_CUBE");
		Col4 Color(0.4f, 0.4f, 0.4f, 1.0f);
		drawComp->SetDiffuse(Color);
	}
	void Goal::OnUpdate() {

	}
	void Goal::OpenGoal() {
		MessageBox(NULL, L"�S�[���̃A�h���X�����", L"", MB_OK);
		Col4 color(1.0f, 1.0f, 0.0f, 0.7f);
		GetComponent<PNTStaticDraw>()->SetDiffuse(color);
	}

	//---------------------------------------------------------------------------------------------
	//�d�g���̃R���X�g���N�^
	//-------------------------------------------------------------------------------------------
	RadioTower::RadioTower(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale,float acceleration) 
		:GameObject(stagePtr),m_position(pos),m_scale(scale),m_acceleration(acceleration)
	{
	}
	void RadioTower::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		auto col = AddComponent<CollisionObb>();
		col->SetAfterCollision(AfterCollision::None);
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, 0, 0));
		ptrTrans->SetWorldPosition(m_position);
		ptrTrans->SetQuaternion(Qt);
		ptrTrans->SetScale(m_scale);
		//�`��R���|�[�l���g�̒ǉ�
		auto drawComp = AddComponent<PNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		drawComp->SetMeshResource(L"DEFAULT_TETRAHEDRON");
		//drawComp->SetFogEnabled(true);
		//�����ɉe���f�肱�ނ悤�ɂ���
		Col4 Color(1.0f, 1.0f, 1.0f, 0.7f);
		drawComp->SetDiffuse(Color);
	}
	void RadioTower::OnUpdate() {

	}
	float RadioTower::GetAcceleration() {
		return m_acceleration;
	}

	//-------------------------------------------------------------------------------------------
	//�h���C�u
	//-------------------------------------------------------------------------------------------
	Drive::Drive(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale)
		:GameObject(stagePtr), m_position(pos), m_scale(scale)
	{
	}
	void Drive::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		auto col = AddComponent<CollisionObb>();
		col->SetAfterCollision(AfterCollision::None);
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, 0, 0));
		ptrTrans->SetWorldPosition(m_position);
		ptrTrans->SetQuaternion(Qt);
		ptrTrans->SetScale(m_scale);
		//�`��R���|�[�l���g�̒ǉ�
		auto drawComp = AddComponent<PNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		drawComp->SetMeshResource(L"DEFAULT_CUBE");
		//drawComp->SetFogEnabled(true);
		//�����ɉe���f�肱�ނ悤�ɂ���
		drawComp->SetOwnShadowActive(true);
		Col4 Color(1.0f, 1.0f, 0.4f, 0.7f);
		drawComp->SetDiffuse(Color);
		//drawComp->SetColorAndAlpha(Color);
	}
}
