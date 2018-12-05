#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class Drone : public OBBObject {
		struct BezierPoint {
			Vec3 p0; //始点
			Vec3 p1;
			Vec3 p2; //終点
		};
		BezierPoint m_roop[8];
		float m_Lerp = 0.0f;
		int m_joinNum = 0;
		int m_joinNumMax;
		Vec3 m_position;
		int m_deadChain;
		bool m_isDead = false;
		//妨害電波の範囲
		float m_JammerRang = 5.0f;
	public:
		enum DroneMotion {
			ClockWise = 0,			//時計回り
			CounterClockwise = 1,	//逆時計回り
			VerticalMotion = 2,		//上下運動
			Wave = 3				//波の動き、一定進んだら返ってくる
		};
		DroneMotion m_roopDir;

		Drone(const shared_ptr<Stage>& stage, Vec3 pos, DroneMotion dir,int chain);
		virtual ~Drone() {};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		int GetDeadChain() { return m_deadChain; };
		void Die() { m_isDead = true; };
		//妨害電波の処理
		void Jammer();
		//時計回りの動きのベジエ曲線の制御点を作る
		void SetBezierClock();
		//上下運動のベジエ曲線の制御点を作る
		void SetVerticalMotion();
		//波の動きのベジエ曲線の制御点を作る
		void SetWaveMotion();
	};

	class ViewDeadChain : public GameObject {
		weak_ptr<Drone> m_DroneObject;
		//このオブジェクトのみで使用するスクエアメッシュ
		shared_ptr<MeshResource> m_SquareMeshResource;
		//背番号
		size_t m_Number;

		Quat Billboard(const Vec3& Line) {
			Vec3 Temp = Line;
			Mat4x4 RotMatrix;
			Vec3 DefUp(0, 1.0f, 0);
			Vec2 TempVec2(Temp.x, Temp.z);
			if (TempVec2.length() < 0.1f) {
				DefUp = Vec3(0, 0, 1.0f);
			}
			Temp.normalize();
			RotMatrix = XMMatrixLookAtLH(Vec3(0, 0, 0), Temp, DefUp);
			RotMatrix.inverse();
			Quat Qt;
			Qt = RotMatrix.quatInMatrix();
			Qt.normalize();
			return Qt;
		}

	public:
		//構築と破棄
		ViewDeadChain(const shared_ptr<Stage>& StagePtr,
			const shared_ptr<Drone>& DroneObjectPtr, size_t Number);
		virtual ~ViewDeadChain() {};
		//初期化
		virtual void OnCreate() override;
		//変化
		virtual void OnUpdate() override;
	};
}