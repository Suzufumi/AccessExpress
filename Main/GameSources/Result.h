#pragma once
#include "stdafx.h"

namespace basecross {
	class ResultStage : public Stage {
		//ƒrƒ…[‚Ìì¬
		void CreateViewLight();
	public:
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};
}