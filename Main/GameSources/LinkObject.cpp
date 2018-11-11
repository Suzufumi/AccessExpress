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
		col->SetDrawActive(true);
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, 0, 0));
		ptrTrans->SetWorldPosition(m_position);
		ptrTrans->SetQuaternion(Qt);
		ptrTrans->SetScale(m_scale);
		//�`��R���|�[�l���g�̒ǉ�
		auto drawComp = AddComponent<PNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		drawComp->SetMeshResource(L"DEFAULT_CUBE");
	}

	void LinkObject::OnUpdate()
	{

	}


}