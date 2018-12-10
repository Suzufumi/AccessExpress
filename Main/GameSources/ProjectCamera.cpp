#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	TpsCamera::TpsCamera()
		: Camera(),
		m_AngleX(0.0f),
		m_AngleY(Deg2Rad(270)),
		m_cameraHeight(3.0f),
		m_maxAngleSpeed(2.0f),
		m_cameraDistance(13.0f)
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
		SetAt(playerPos);
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
		p1 = p2 - ((p2 - p0) / 2) - a.normalize() * 5.0f;
	}
}