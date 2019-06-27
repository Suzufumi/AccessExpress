#pragma once
#include "stdafx.h"

namespace basecross
{
	class TpsCamera : public Camera
	{
	private:
		/*プレイヤーのY軸基準回転
		270度から始めることでXの+方向を右、Zの+方向を奥にする*/
		float m_angleY;
		float m_angleX;						//プレイヤーのX軸基準回転
		float m_angeleXMax = 80.0f;				//縦方向を管理する変数のマックス値
		float m_maxAngleSpeed;		//カメラが回転するスピード
		float m_cameraDistance;			//カメラのプレイヤーからの距離
		float m_cameraLookUp;		//カメラが見上げる高さ
		bool m_isLink;				//リンク状態の処理を行う
		Vec3 p0, p1, p2;			//ベジェ曲線の制御点
		float m_leap;				//補正の経過
		//回り込む際に使う角度の数値
		float m_aroundStartAngleY;
		float m_aroundStartAngleX;
		float m_aroundEndAngleY;
		float m_aroundEndAngleX;
	public:
		TpsCamera();
		virtual ~TpsCamera();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		//カメラの角度を変更する
		void CameraControll();
		//照準の方を向きながら値に従い移動する
		void CameraRoll(Vec3 sightPos);
		//飛んでいるときに後ろから追う動きを設定
		void SetBezier(weak_ptr<GameObject> playerPtr,Vec3 p2Player);
		//設定した動きで動く
		void BezierMove(float leap,Vec3 playerPos);
		//タイムアップしたときの回り込む処理の準備
		void SetGoingAround(float aroundEndAngleX, float aroundEndAngleY) ;
		//回り込む
		bool GoingAround();
		//ロックオンしている対象がいる際のカメラ処理
		void RockonCameraMove(Vec3 sightPos,Vec3 linkPos);

		void SetOnLink(bool f) { m_isLink = f; };
		bool GetOnLink() { return m_isLink; };

		float GetCameraAngleX() const
		{ return m_angleX; }
		float GetCameraAngleY() const 
		{ return m_angleY; }
		void SetCameraAngleX(float anglex) {
			m_angleX = anglex;
		}
		void SetCameraAngleY(float angleY){
			m_angleY = angleY;
		}
		float GetMaxAngleSpeed() const
		{ return m_maxAngleSpeed; }
		float GetCameraDistance() const
		{ return m_cameraDistance; }
		float GetCameraLookUp() const
		{ return m_cameraLookUp; }
	};

	class TpsCameraJudgment : public GameObject {
	public:
		TpsCameraJudgment(const shared_ptr<Stage>& StagePtr);
		virtual ~TpsCameraJudgment() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		//衝突したとき
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		//衝突が解除されたとき
		virtual void OnCollisionExit(shared_ptr<GameObject>& Other) override;
	};
}