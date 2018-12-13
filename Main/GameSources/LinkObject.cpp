#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	LinkObject::LinkObject(const shared_ptr<Stage>& stagePtr,Vec3 pos,Vec3 scale)
		: AccessObject(stagePtr),m_position(pos),m_scale(scale)
	{}

	void LinkObject::OnCreate()
	{
		m_accessType = AppType::Link;
		auto ptrTrans = GetComponent<Transform>();
		auto col = AddComponent<CollisionObb>();
		col->SetAfterCollision(AfterCollision::None);
		//col->SetDrawActive(true);
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, 0, 0));
		ptrTrans->SetWorldPosition(m_position);
		ptrTrans->SetQuaternion(Qt);
		ptrTrans->SetScale(m_scale);

		Mat4x4 spanMat; // ���f���ƃg�����X�t�H�[���̊Ԃ̍����s��
		spanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -0.2f, 0.0f)
		);
		//�v���C���[�ƂԂ���Ȃ��悤�ɂ���
		AddTag(L"PlayerUse");
		//�`��R���|�[�l���g�̒ǉ�
		auto drawComp = AddComponent<BcPNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		drawComp->SetMeshResource(L"TOWER_MODEL");
		drawComp->SetTextureResource(L"TOWER_TX");
		// ���C�e�B���O���Ȃ��ɂ���
		drawComp->SetLightingEnabled(false);
		drawComp->SetMeshToTransformMatrix(spanMat);
		drawComp->SetSamplerState(SamplerState::LinearClamp);
		SetAlphaActive(true);
		GetStage()->GetSharedObjectGroup(L"Link")->IntoGroup(GetThis<GameObject>());
	}

	void LinkObject::OnUpdate()
	{

	}


}