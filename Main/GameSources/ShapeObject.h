#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	class OBBObject : public GameObject{
		Vec3 m_position;
		Vec3 m_scale;
	public:
		OBBObject(const shared_ptr<Stage>& stage,Vec3 pos,Vec3 scale);
		virtual ~OBBObject(){}
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}