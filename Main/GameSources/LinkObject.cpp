#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	LinkObject::LinkObject(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode)
		: AccessObject(stagePtr)
	{
		auto posStr = XmlDocReader::GetAttribute(pNode, L"Pos");
		auto rotStr = XmlDocReader::GetAttribute(pNode, L"Quat");
		auto scaleStr = XmlDocReader::GetAttribute(pNode, L"Scale");

		auto pos = MyUtil::unityVec3StrToBCVec3(posStr);
		auto quat = MyUtil::unityQuatStrToBCQuat(rotStr);
		auto scale = MyUtil::unityVec3StrToBCVec3(scaleStr);

		m_position = pos;
		//m_position += Vec3(2.8f, 3.7f, -9.5f);
		m_quat = quat;
		m_scale = scale;

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

		Mat4x4 spanMat; // ���f���ƃg�����X�t�H�[���̊Ԃ̍����s��
		spanMat.affineTransformation(
			Vec3(0.9f, 0.9f, 0.9f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
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
		SetDrawLayer(-2);
		GetStage()->GetSharedObjectGroup(L"Link")->IntoGroup(GetThis<GameObject>());

	}

	void LinkObject::OnUpdate()
	{

	}


}