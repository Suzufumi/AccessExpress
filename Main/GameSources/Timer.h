#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class RemainingTimer : public GameObject {
		float m_remainingTime = 40.0f;
	public:
		RemainingTimer(const shared_ptr<Stage>& stage);
		virtual ~RemainingTimer() {}
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}