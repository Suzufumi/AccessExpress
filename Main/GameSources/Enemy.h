#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class Drone : public OBBObject {
		struct BezierPoint {
			Vec3 p1; //始点
			Vec3 p2; 
			Vec3 p3; //終点
		};
		BezierPoint m_roop[4];
		Vec3 m_position;
	public:
		Drone(const shared_ptr<Stage>& stage, Vec3 pos);
		virtual ~Drone(){};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};
}