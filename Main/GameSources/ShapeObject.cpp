#include "stdafx.h"
#include "Project.h"

namespace basecross {
	OBBObject::OBBObject(const shared_ptr<Stage>& stage,Vec3 pos,Vec3 scale)
		:GameObject(stage),m_position(pos),m_scale(scale)
	{
	}
	void OBBObject::OnCreate() {
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
		//�����ɉe���f�肱�ނ悤�ɂ���
		drawComp->SetOwnShadowActive(true);
	}
	void OBBObject::OnUpdate(){}
}