#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	SearchObject::SearchObject(const shared_ptr<Stage>& stagePtr, const Vec3 pos, const Quat quat, const Vec3 scale)
		: AccessObject(stagePtr), m_position(pos), m_quat(quat), m_scale(scale)
	{}

	void SearchObject::OnCreate()
	{
		// Transoformの設定
		auto transComp = GetComponent<Transform>();
		transComp->SetWorldPosition(m_position);
		transComp->SetQuaternion(m_quat);
		transComp->SetScale(m_scale);

		// コリジョンの設定
		auto col = AddComponent<CollisionObb>();
		col->SetAfterCollision(AfterCollision::Auto);
		// 動かないように固定
		col->SetFixed(true);

		// 描画の設定
		auto drawComp = AddComponent<PNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_CUBE");
		// 色の設定
		drawComp->SetDiffuse(Col4(0.0f, 1.0f, 0.0f, 1.0f));
		
		m_accessType = AppType::Search;

		AddTag(L"SearchObject");
	}

	void SearchObject::OnUpdate()
	{

	}

	void SearchObject::Access(AppType type)
	{

	}
}