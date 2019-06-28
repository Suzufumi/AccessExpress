#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	TpsCamera::TpsCamera()
		: Camera(),
		m_angleX(Deg2Rad(10.0f)),
		m_angleY(Deg2Rad(270)),
		m_maxAngleSpeed(2.0f),
		m_cameraDistance(15.0f),
		m_cameraLookUp(3.5f)
	{}

	TpsCamera::~TpsCamera()
	{}

	void TpsCamera::OnCreate(){
		
	}

	void TpsCamera::OnUpdate(){

	}

	//--------------------------------------------------------------------------------------------
	//カメラの値変更
	//--------------------------------------------------------------------------------------------
	void TpsCamera::CameraControll() {
		float delta = App::GetApp()->GetElapsedTime();
		auto m_pad = GameManager::GetInstance().GetPad();

		//右スティックに値が入力されていたら
		if (m_pad.fThumbRX > 0.7f || m_pad.fThumbRX < -0.7f ||
			m_pad.fThumbRY > 0.7f || m_pad.fThumbRY < -0.7f) {
			m_angleY += -m_pad.fThumbRX * m_maxAngleSpeed * 2 * delta; // カメラを回転させる
			m_angleX += -m_pad.fThumbRY * m_maxAngleSpeed * 2 * delta; // カメラを昇降させる
		}
		else if (m_pad.fThumbRX > 0.2f || m_pad.fThumbRX < -0.2f ||
			m_pad.fThumbRY > 0.2f || m_pad.fThumbRY < -0.2f) {
			m_angleY += -m_pad.fThumbRX * m_maxAngleSpeed * delta; // カメラを回転させる
			m_angleX += -m_pad.fThumbRY * m_maxAngleSpeed * delta; // カメラを昇降させる
		}
		//左スティックに値が入力されていたら
		if (m_pad.fThumbLX > 0.7f || m_pad.fThumbLX < -0.7f ||
			m_pad.fThumbLY > 0.7f || m_pad.fThumbLY < -0.7f) {
			m_angleY += -m_pad.fThumbLX * m_maxAngleSpeed * 2 * delta; // カメラを回転させる
			m_angleX += -m_pad.fThumbLY * m_maxAngleSpeed * 2 * delta; // カメラを昇降させる
		}
		else if (m_pad.fThumbLX > 0.2f || m_pad.fThumbLX < -0.2f ||
			m_pad.fThumbLY > 0.2f || m_pad.fThumbLY < -0.2f) {
			m_angleY += -m_pad.fThumbLX * m_maxAngleSpeed * delta; // カメラを回転させる
			m_angleX += -m_pad.fThumbLY * m_maxAngleSpeed * delta; // カメラを昇降させる
		}

		//Y軸基準角度の丸め(-360<360)
		if (m_angleY > Deg2Rad(360.0f)) {
			m_angleY = Deg2Rad(0.0f);
		}
		if (m_angleY < Deg2Rad(0.0f)) {
			m_angleY = Deg2Rad(360.0f);
		}
		//X軸基準角度の丸め(-70<70)
		if (m_angleX > Deg2Rad(m_angeleXMax)) {
			m_angleX = Deg2Rad(m_angeleXMax);
		}
		if (m_angleX < -Deg2Rad(m_angeleXMax)) {
			m_angleX = -Deg2Rad(m_angeleXMax);
		}

	}
	//---------------------------------------------------------------------------------------------
	//カメラのプレイヤー追従処理
	//---------------------------------------------------------------------------------------------
	void TpsCamera::CameraRoll(Vec3 sightPos) {
		//照準を見る
		SetAt(sightPos);
		auto eye = sightPos + Vec3(cos(m_angleY) * m_cameraDistance,
			sin(m_angleX) * m_cameraDistance, sin(m_angleY) * m_cameraDistance);
		SetEye(eye);
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
	//ベジェ曲線で飛ばす(playerと一緒に動くためにplayerの現在Leapをもらってくる)
	//--------------------------------------------------------------------------------------------
	void TpsCamera::BezierMove(float leap, Vec3 playerPos) {
		Vec3 pos;
		pos = (1 - leap)*(1 - leap)*p0 + 2 * (1 - leap)*leap*p1 + leap * leap*p2;
		SetEye(pos);
		SetAt(playerPos + Vec3(0.0f, 3.0f, 0.0f));
	}
	//--------------------------------------------------------------------------------------------
	//回り込む処理の前準備（in：デグリー角）
	//--------------------------------------------------------------------------------------------
	void TpsCamera::SetGoingAround(float aroundEndAngleX, float aroundEndAngleY) {
		m_aroundStartAngleX = m_angleX;
		m_aroundStartAngleY = m_angleY;
		m_aroundEndAngleX = aroundEndAngleX;
		m_aroundEndAngleY = aroundEndAngleY;
		m_leap = 0.0f;
	}
	//--------------------------------------------------------------------------------------------
	//回り込む処理（out：回り込み終わったらtrue）
	//--------------------------------------------------------------------------------------------
	bool TpsCamera::GoingAround() {
		if (m_leap < 1.0f) {
			m_leap += App::GetApp()->GetElapsedTime() * 2;
			m_angleX = (1 - m_leap) * m_aroundStartAngleX + m_leap * m_aroundEndAngleX;
			m_angleY = (1 - m_leap) * m_aroundStartAngleY + m_leap * m_aroundEndAngleY;
			return false;
		}
		else {
			m_leap = 1.0f;
			return true;
		}
	}
	///---------------------------------------------------------------------------------------------
	//ロックオンしている対象がいる際のカメラ処理
	///---------------------------------------------------------------------------------------------
	void TpsCamera::RockonCameraMove(Vec3 sightPos,Vec3 linkPos) {
		float deltaX = sightPos.x - linkPos.x;
		float deltaZ = sightPos.z - linkPos.z;
		float deltaY = sightPos.y - linkPos.y;
		//横のカメラ位置を制御する角度
		m_angleY = atan2f(deltaZ, deltaX);
		if (m_angleY < 0.0f) {
			m_angleY += Deg2Rad(360.0f);
		}
		//縦のカメラ位置をだすための辺出す
		float syahen = hypotf(deltaX, deltaZ);
		//縦のカメラ位置を制御する角度
		m_angleX = atan2f(deltaY, syahen);
	}

	///--------------------------------------------------------------------------------------------
	//カメラとともに動く判定のコンストラクタ
	///--------------------------------------------------------------------------------------------
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