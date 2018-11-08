/*!
@file Player.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class Player : public GameObject {
		float m_nowWalkSpeed = 5.0f;			//現在の移動のスピード
		float m_standardWalkSpeed = 5.0f;		//基本の移動スピード
		/*プレイヤーのY軸基準回転
		270度から始めることでXの+方向を右、Zの+方向を奥にする*/
		float m_AngleY = Deg2Rad(270);
		float m_AngleX = 0;						//プレイヤーのX軸基準回転
		float m_maxAngleSpeed = 3.141592f;		//カメラが回転するスピード
		float m_cameraHeight = 3.0f;			//カメラの初期高さ
		float m_cameraDistance = 10.0f;			//カメラのプレイヤーからの距離
		bool m_isFall = false;					//Y軸方向の力を加えるかどうか
		bool m_isFloa = false;					//浮遊してる
		bool m_isHaveFile = false;

		CONTROLER_STATE m_pad;					//パッドの全情報

		//コンストラクタで持ってきた引数を入れる
		Vec3 m_position;
		Quat m_quaternion;
		Vec3 m_scale;

		weak_ptr<RadioTowerHitJudgment> m_RadioTowerHitJudgment;
		weak_ptr<File> m_File;

		Vec3 m_padDir;									//左スティックの向きを入れる
		Vec3 m_forward;									//カメラの方向を踏まえたプレイヤーの向いている方向
		Vec3 m_velocity;								//現在のvelocityの値を持つ

	public:
		Player(const shared_ptr<Stage>& StagePtr, Vec3 pos, Quat quat, Vec3 sca);
		virtual ~Player() {};
		virtual void OnCreate() override; // 初期化
		virtual void OnUpdate() override; // 更新
		virtual void OnUpdate2() override; // 後更新
		//衝突したとき
		virtual void OnCollisionEnter(shared_ptr<GameObject>& Other) override;
		//衝突している
		virtual void OnCollisionExcute(shared_ptr<GameObject>& Other) override;
		//衝突が解除されたとき
		virtual void OnCollisionExit(shared_ptr<GameObject>& Other) override;
		void Floating();
		void Walk();
		void CameraRoll();
		//子オブジェクトしてもっている電波塔との当たり判定をプレイヤーのほうでも認知する
		void SetRadioTowerHitJudgment(weak_ptr<RadioTowerHitJudgment> hit) {
			m_RadioTowerHitJudgment = hit;
		}

		void DrawStrings();

	};
}
//end basecross

