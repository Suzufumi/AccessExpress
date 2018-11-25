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
		float m_cameraHeight;			//カメラの初期高さ
		float m_cameraDistance;			//カメラのプレイヤーからの距離
	public:
		TpsCamera();
		virtual ~TpsCamera();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		// 引数で入ってきたトランスフォームを取得
		void SetTarget(const shared_ptr<Transform>& target);

		float GetCameraAngleX() const
		{ return m_AngleX; }
		float GetCameraAngleY() const 
		{ return m_AngleY; }
		float GetMaxAngleSpeed() const
		{ return m_maxAngleSpeed; }
		float GetCameraHeight() const
		{ return m_cameraHeight; }
		float GetCameraDistance() const
		{ return m_cameraDistance; }
	};
}