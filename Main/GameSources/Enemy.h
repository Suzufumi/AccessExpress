#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class Drone : public OBBObject {
		struct BezierPoint {
			Vec3 p0; //n“_
			Vec3 p1; 
			Vec3 p2; //I“_
		};
		BezierPoint m_roop[4];
		float m_Lerp = 0.0f;
		int m_roopNum = 0;
		Vec3 m_position;
		int m_deadChain;
		bool m_isDead = false;
		//–WŠQ“d”g‚Ì”ÍˆÍ
		float m_JammerRang = 15.0f;
	public:
		enum RoopDirection {
			ClockWise = 0,			//Œv‰ñ‚è
			CounterClockwise = 1	//‹tŒv‰ñ‚è
		};
		RoopDirection m_roopDir;

		Drone(const shared_ptr<Stage>& stage, Vec3 pos,RoopDirection dir);
		virtual ~Drone(){};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		void SetDeadChain(int chain) { m_deadChain = chain; };
		int GetDeadChain() { return m_deadChain; };
		void Die() { m_isDead = true; };
		//–WŠQ“d”g‚Ìˆ—
		void Jammer();
	};

	class ViewDeadChain : public GameObject {
		int m_deadChain;
	public:
		ViewDeadChain(const shared_ptr<Stage>& stageptr,int chain);
		virtual ~ViewDeadChain() {};
		virtual void OnCreate()override;
		virtual void OnUpdate()override {};
	};
}