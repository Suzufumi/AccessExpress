#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	LinkObject::LinkObject(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode)
		: AccessObject(stagePtr)
	{
		auto stage = GetTypeStage<GameStage>();
		if (stage)
		{
			// LinkObject�̏����擾
			stage->LoadXmlParam(pNode, m_position, m_quat, m_scale);
		}

	}

	void LinkObject::OnCreate()
	{
		m_accessType = AppType::Link;
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetWorldPosition(m_position);
		ptrTrans->SetQuaternion(m_quat);
		ptrTrans->SetScale(m_scale);

		auto col = AddComponent<CollisionObb>();
		col->SetDrawActive(false);
		col->SetAfterCollision(AfterCollision::None);

		SetAlphaActive(true);
		SetDrawLayer(-2);
		GetStage()->GetSharedObjectGroup(L"Link")->IntoGroup(GetThis<GameObject>());
		//�v���C���[�ƂԂ���Ȃ��悤�ɂ���
		AddTag(L"PlayerUse");

	}

	void LinkObject::OnUpdate()
	{

	}

	void LinkObject::OnDraw()
	{
		auto stage = GetTypeStage<GameStage>();
		// ���f���̕ϊ��s��
		auto spanMat = stage->SetModelMatrix(Vec3(0.9f), Vec3(0.0f), Vec3(0.0f), Vec3(0.0f, -0.5f, 0.0f));

		//�`��R���|�[�l���g�̒ǉ�
		auto drawComp = AddComponent<BcPNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		drawComp->SetMeshResource(L"TOWER_MODEL");
		drawComp->SetTextureResource(L"TOWER_TX");
		// ���C�e�B���O���Ȃ��ɂ���
		drawComp->SetLightingEnabled(false);
		drawComp->SetMeshToTransformMatrix(spanMat);
		drawComp->SetSamplerState(SamplerState::LinearClamp);

	}


}