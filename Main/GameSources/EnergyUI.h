#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class EnergyUI : public Sprite {
		EnergyUI(const shared_ptr<Stage>& stage, const std::wstring& key, Vec2 size);
	};
}