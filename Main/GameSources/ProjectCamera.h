#pragma once
#include "stdafx.h"

namespace basecross
{
	class TpsCamera : public Camera
	{
	private:
		/*プレイヤーのY軸基準回転
		270度から始めることでXの+方向を右、Zの+方向を奥にする*/
		float m_AngleY;
		float m_AngleX;						//プレイヤーのX軸基準回転
		float m_maxAngleSpeed;		//カメラが回転するスピード
		float m_cameraDistance;			//カメラのプレイヤーからの距離
		float m_cameraLookUp;	//カメラが見上げる高さ
		bool m_isLink;				//リンク状態の処理を行う
		Vec3 p0, p1, p2;
	public:
		TpsCamera();
		virtual ~TpsCamera();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		// 引数で入ってきたトランスフォームを取得
		void SetTarget(const shared_ptr<Transform>& target);
		void SetBezier(weak_ptr<GameObject> playerPtr,Vec3 p2Player);
		void BezierMove(float leap,Vec3 playerPos);

		void SetOnLink(bool f) { m_isLink = f; };
		bool GetOnLink() { return m_isLink; };

		float GetCameraAngleX() const
		{ return m_AngleX; }
		float GetCameraAngleY() const 
		{ return m_AngleY; }
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