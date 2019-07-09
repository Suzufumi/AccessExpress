#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class Drone : public GameObject {
		Vec3 m_pos;
		Quat m_quat;
		Vec3 m_scale;
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
	public:
		enum DroneMotion {
			ClockWise = 0,			//時計回り
			CounterClockwise = 1,	//逆時計回り
			VerticalMotion = 2,		//上下運動
			Wave = 3				//波の動き、一定進んだら返ってくる
		};
		DroneMotion m_roopDir;

		Drone(const shared_ptr<Stage>& stagePtr, IXMLDOMNodePtr pNode);
		virtual ~Drone() {};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		//経過を更新する
		void LeapUpdate();
		//動く
		void BezierMove();
		//時計回りの動きのベジエ曲線の制御点を作る
		void SetBezierClock();
		//上下運動のベジエ曲線の制御点を作る
		void SetVerticalMotion();
		//波の動きのベジエ曲線の制御点を作る
		void SetWaveMotion();
	};

}