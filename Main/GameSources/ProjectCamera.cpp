#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	TpsCamera::TpsCamera()
		: Camera(),
		m_AngleX(Deg2Rad(10.0f)),
		m_AngleY(Deg2Rad(270)),
		m_maxAngleSpeed(2.0f),
		m_cameraDistance(13.0f),
		m_cameraLookUp(3.5f)
	{}

	TpsCamera::~TpsCamera()
	{}

	void TpsCamera::OnCreate()
	{
		
	}

	void TpsCamera::OnUpdate()
	{

	}

	void TpsCamera::SetTarget(const shared_ptr<Transform>& target)
	{
	}
	//--------------------------------------------------------------------------------------------
	//ベジェ曲線で飛ばす(playerと一緒に動くためにplayerの現在Leapをもらってくる)
	//--------------------------------------------------------------------------------------------
	void TpsCamera::BezierMove(float leap,Vec3 playerPos) {
		Vec3 pos;
		pos = (1 - leap)*(1 - leap)*p0 + 2 * (1 - leap)*leap*p1 + leap * leap*p2;
		SetEye(pos);
		SetAt(playerPos + Vec3(0.0f,3.0f,0.0f));
	}
	//--------------------------------------------------------------------------------------------
	//制御点の設定
	//--------------------------------------------------------------------------------------------
	void TpsCamera::SetBezier(weak_ptr<GameObject> playerPtr,Vec3 p2Player) {
		auto player = playerPtr.lock();
		//飛ぶ前のプレイヤーとカメラの位置関係を所持
		Vec3 a = player->GetComponent<Transform>()->GetWorldPosition() - GetEye();
		p0 = GetEye();		//始点
		p2 = p2Player - a;	//終点
		//始点と終点の中点からaの方向にずらして与える
		p1 = (p2 - ((p2 - p0) / 2)) - a.normalize() * Vec3(p2 - p0).length();
	}
	//--------------------------------------------------------------------------------------------
	//カメラとともに動く判定のコンストラクタ
	//--------------------------------------------------------------------------------------------
	TpsCameraJudgment::TpsCameraJudgment(const shared_ptr<Stage>& StagePtr)
		:GameObject(StagePtr)
	{
	}
	//--------------------------------------------------------------------------------------------
	//作成
	//--------------------------------------------------------------------------------------------
	void TpsCameraJudgment::OnCreate() {
		GetComponent<Transform>()->SetScale(3.0f, 3.0f, 3.0f);

		AddComponent<CollisionObb>();
	}
	//--------------------------------------------------------------------------------------------
	//更新
	//--------------------------------------------------------------------------------------------
	void TpsCameraJudgment::OnUpdate() {
		auto camera = GetStage()->GetView()->GetTargetCamera();
		auto tpsCamera = dynamic_pointer_cast<TpsCamera>(camera);
		GetComponent<Transform>()->SetWorldPosition(tpsCamera->GetEye());
	}
	//--------------------------------------------------------------------------------------------
	//衝突時判定
	//--------------------------------------------------------------------------------------------
	void TpsCameraJudgment::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		auto wall = dynamic_pointer_cast<Wall>(Other);
		//壁にカメラが当たったら、壁を見えなくする
		if (wall) {
			wall->SetDrawActive(false);
		}
	}
	//--------------------------------------------------------------------------------------------
	//衝突解除時判定
	//--------------------------------------------------------------------------------------------
	void TpsCameraJudgment::OnCollisionExit(shared_ptr<GameObject>& Other) {
		auto wall = dynamic_pointer_cast<Wall>(Other);
		//衝突が解除された壁を見えるようにする
		if (wall) {
			wall->SetDrawActive(true);
		}
	}
}