#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class Ray : public GameObject {
		Vec3 m_positon;
		Vec3 m_direction;
		float m_length = 10.0f;

		weak_ptr<Player> m_player;
		Vec3 m_cameraPos;
	public:
		Ray(const shared_ptr<Stage>& stage,weak_ptr<Player>& player);
		virtual ~Ray() {}
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}