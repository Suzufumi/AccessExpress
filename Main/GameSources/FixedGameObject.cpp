#include "stdafx.h"
#include "Project.h"

namespace basecross {
	Wall::Wall(const shared_ptr<Stage>& StagePtr, Vec3 pos, Vec3 scale)
		: GameObject(StagePtr), m_position(pos), m_scale(scale)
	{
	}
	//--------------------------------------------------------------------------------------
	//初期化
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
		//描画コンポーネントの追加
		auto drawComp = AddComponent<PNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
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
		//描画コンポーネントの追加
		auto drawComp = AddComponent<PNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		drawComp->SetMeshResource(L"DEFAULT_CUBE");
		Col4 Color(0.4f, 0.4f, 0.4f, 1.0f);
		drawComp->SetDiffuse(Color);
	}
	void Goal::OnUpdate() {

	}
	void Goal::OpenGoal() {
		MessageBox(NULL, L"ゴールのアドレスを特定", L"", MB_OK);
		Col4 color(1.0f, 1.0f, 0.0f, 0.7f);
		GetComponent<PNTStaticDraw>()->SetDiffuse(color);
	}

	//---------------------------------------------------------------------------------------------
	//電波塔のコンストラクタ
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
		//描画コンポーネントの追加
		auto drawComp = AddComponent<PNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		drawComp->SetMeshResource(L"DEFAULT_TETRAHEDRON");
		//drawComp->SetFogEnabled(true);
		//自分に影が映りこむようにする
		Col4 Color(1.0f, 1.0f, 1.0f, 0.7f);
		drawComp->SetDiffuse(Color);
	}
	void RadioTower::OnUpdate() {

	}
	float RadioTower::GetAcceleration() {
		return m_acceleration;
	}

	//-------------------------------------------------------------------------------------------
	//ドライブ
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
		//描画コンポーネントの追加
		auto drawComp = AddComponent<PNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		drawComp->SetMeshResource(L"DEFAULT_CUBE");
		//drawComp->SetFogEnabled(true);
		//自分に影が映りこむようにする
		drawComp->SetOwnShadowActive(true);
		Col4 Color(1.0f, 1.0f, 0.4f, 0.7f);
		drawComp->SetDiffuse(Color);
		//drawComp->SetColorAndAlpha(Color);
	}
}
