#include"stdafx.h"
#include"Project.h"

namespace basecross {
	//---------------------------------------------------------------------------------------------
	//電波塔との衝突判定を行う当たり判定のコンストラクタ
	//---------------------------------------------------------------------------------------------
	RadioTowerHitJudgment::RadioTowerHitJudgment(const shared_ptr<Stage>& StagePtr, weak_ptr<GameObject> player)
		:GameObject(StagePtr), m_player(player)
	{
	}
	void RadioTowerHitJudgment::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(10.0f, 10.0f, 10.0f);

		//球形の当たり判定をセット
		auto col = AddComponent<CollisionSphere>();
		//オブジェクトと当たっても反発しない
		col->SetAfterCollision(AfterCollision::None);
		//当たり判定が見えるようにする
		//col->SetDrawActive(true);
		//Playerとぶつからないようにするためのタグ
		AddTag(L"PlayerUse");
		//コンストラクタで取得したプレイヤーにアクセス
		auto obj = m_player.lock();
		//playerと親子関係になる
		ptrTrans->SetParent(obj);
		//playerと同位置になる
		ptrTrans->SetWorldPosition(obj->GetComponent<Transform>()->GetWorldPosition());
	}
	//-------------------------------------------------------------------------------------------------------------
	//ヒットしている最中
	//-------------------------------------------------------------------------------------------------------------
	void RadioTowerHitJudgment::OnCollisionExcute(shared_ptr<GameObject>& Other) {
		//衝突したものが電波塔か調べる
		auto radioTower = dynamic_pointer_cast<RadioTower>(Other);
		//電波塔だった
		if (radioTower) {
			//Player位置から電波塔の位置を引く
			Vec3 d = this->GetComponent<Transform>()->GetWorldPosition() - radioTower->GetComponent<Transform>()->GetWorldPosition();
			//位置の差から距離を出す
			float leng = d.length();
			//導き出した距離が現在持っている距離よりも近かったら
			if (m_length > leng) {
				//一番近い距離を更新する
				m_length = leng;
				//速度を取得する
				m_Acceleration = radioTower->GetAcceleration();
			}
		}
	}

	//-------------------------------------------------------------------------------------
	//ファイル
	//-------------------------------------------------------------------------------------
	File::File(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale)
		:GameObject(stagePtr), m_position(pos), m_scale(scale)
	{
	}
	//-------------------------------------------------------------------------------------
	//初期化
	//-------------------------------------------------------------------------------------
	void File::OnCreate(){
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
		Col4 Color(0.4f, 1.0f, 0.7f, 0.7f);
		drawComp->SetDiffuse(Color);
		//drawComp->SetColorAndAlpha(Color);
		//タグ付け
		AddTag(L"File");
	}
	void File::SetOnPlayer(weak_ptr<GameObject> obj){
		auto trans = GetComponent<Transform>();
		//オブジェクトにアクセス
		auto player = obj.lock();
		//オブジェクトを親にする
		trans->SetParent(player);
		//親の上に乗る
		trans->SetPosition(Vec3(0, player->GetComponent<Transform>()->GetScale().y/2 + m_scale.y/2, 0));
	}
	void File::LookFile() {
		auto drawComp = GetComponent<DrawComponent>();
		drawComp->SetDrawActive(true);
	}
	void File::UnLookFile() {
		auto drawComp = GetComponent<DrawComponent>();
		drawComp->SetDrawActive(false);
	}

	AddlessCertificate::AddlessCertificate(const shared_ptr<Stage>& stage, Vec3 pos, Vec3 scale) 
	:OBBObject(stage,pos,scale)
	{
	}
	void AddlessCertificate::OnCreate() {
		//親オブジェクトのコンストラクタを呼ぶ、ここで形の形成をす
		OBBObject::OnCreate();
		Col4 Color(1.0f, 0.3f, 0.7f, 0.7f);
		GetComponent<PNTStaticDraw>()->SetDiffuse(Color);
		GetComponent<CollisionObb>()->SetDrawActive(true);
	}
	void AddlessCertificate::FindGoal() {
		auto goal = m_goal.lock();
		//goal->OpenGoal();
		m_isUnlockGoal = true;
	}
	//--------------------------------------------------------------------------------
	//照準
	//--------------------------------------------------------------------------------
	SightingDevice::SightingDevice(const shared_ptr<Stage>& stagePtr)
	:GameObject(stagePtr)
	{
	}
	void SightingDevice::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, 0, 0));
		ptrTrans->SetWorldPosition(Vec3(0,0,0));
		ptrTrans->SetQuaternion(Qt);
		ptrTrans->SetScale(1,1,1);
		//描画コンポーネントの追加
		auto drawComp = AddComponent<PNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		drawComp->SetMeshResource(L"DEFAULT_SQUARE");
		//drawComp->SetMeshResource(L"DEFAULT_CUBE");
		//drawComp->SetFogEnabled(true);
		//影が映りこまない
		drawComp->SetOwnShadowActive(false);
		Col4 Color(0.4f, 1.0f, 0.7f, 0.7f);
		drawComp->SetDiffuse(Color);
	}
	void SightingDevice::OnUpdate() {
		if (m_captureLink) {
			Col4 Color(1.0f, 0.4f, 0.7f, 0.7f);
			GetComponent<PNTStaticDraw>()->SetDiffuse(Color);
		}
		else {
			Col4 Color(0.4f, 1.0f, 0.7f, 0.7f);
			GetComponent<PNTStaticDraw>()->SetDiffuse(Color);
		}
	}
}