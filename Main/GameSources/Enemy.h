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
		BezierPoint m_roop[4];
		float m_Lerp = 0.0f;
		int m_roopNum = 0;
		Vec3 m_position;
		int m_deadChain;
		bool m_isDead = false;
	public:
		Drone(const shared_ptr<Stage>& stage, Vec3 pos);
		virtual ~Drone(){};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		void SetDeadChain(int chain) { m_deadChain = chain; };
		int GetDeadChain() { return m_deadChain; };
		void Die() { m_isDead = true; };
	};
}