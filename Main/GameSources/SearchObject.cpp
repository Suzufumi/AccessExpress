#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	SearchObject::SearchObject(const shared_ptr<Stage>& stagePtr, const Vec3 pos, const Quat quat, const Vec3 scale)
		: AccessObject(stagePtr), m_position(pos), m_quat(quat), m_scale(scale)
	{}

	void SearchObject::OnCreate()
	{
		// Transoform�̐ݒ�
		auto transComp = GetComponent<Transform>();
		transComp->SetWorldPosition(m_position);
		transComp->SetQuaternion(m_quat);
		transComp->SetScale(m_scale);

		// �R���W�����̐ݒ�
		auto col = AddComponent<CollisionObb>();
		col->SetAfterCollision(AfterCollision::Auto);
		// �����Ȃ��悤�ɌŒ�
		col->SetFixed(true);

		// �`��̐ݒ�
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");
		// �F�̐ݒ�
		drawComp->SetDiffuse(Col4(0.0f, 1.0f, 0.0f, 1.0f));
		
		m_accessType = AppType::Search;

		AddTag(L"SearchObject");
	}

	void SearchObject::OnUpdate()
	{

	}

	void SearchObject::Access()
	{

	}
}