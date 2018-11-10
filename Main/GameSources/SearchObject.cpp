#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	SearchObject::SearchObject(const shared_ptr<Stage>& stagePtr, const Vec3 pos, const Quat quat, const Vec3 scale)
		: AccessObject(stagePtr), m_position(pos), m_quat(quat), m_scale(scale)
	{}

	void SearchObject::OnCreate()
	{
		// Transoform‚Ìİ’è
		auto transComp = GetComponent<Transform>();
		transComp->SetWorldPosition(m_position);
		transComp->SetQuaternion(m_quat);
		transComp->SetScale(m_scale);

		// ƒRƒŠƒWƒ‡ƒ“‚Ìİ’è
		auto col = AddComponent<CollisionObb>();
		col->SetAfterCollision(AfterCollision::Auto);
		// “®‚©‚È‚¢‚æ‚¤‚ÉŒÅ’è
		col->SetFixed(true);

		// •`‰æ‚Ìİ’è
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");
		// F‚Ìİ’è
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