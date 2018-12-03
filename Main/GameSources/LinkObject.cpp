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

		Mat4x4 spanMat; // モデルとトランスフォームの間の差分行列
		spanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		//描画コンポーネントの追加
		auto drawComp = AddComponent<BcPNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		//drawComp->SetMeshResource(L"TOWER_MODEL");
		//drawComp->SetTextureResource(L"TOWER_TX");
		drawComp->SetMeshResource(L"DEFAULT_CUBE");
		drawComp->SetMeshToTransformMatrix(spanMat);
		drawComp->SetDiffuse(Col4(0.0f, 1.0f, 0.0f, 1.0f));
		GetStage()->GetSharedObjectGroup(L"Link")->IntoGroup(GetThis<GameObject>());
	}

	void LinkObject::OnUpdate()
	{

	}


}