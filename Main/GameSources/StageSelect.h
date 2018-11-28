#pragma once
#include "stdafx.h"

namespace basecross {
	class StageSelect : public Stage {
		//ƒrƒ…[‚Ìì¬
		void CreateViewLight();
	public:
		virtual ~StageSelect() {};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};
}