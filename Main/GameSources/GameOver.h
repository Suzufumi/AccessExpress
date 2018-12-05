#pragma once
#include "stdafx.h"

namespace basecross {
	class GameOverStage : public Stage {
		//ƒrƒ…[‚Ìì¬
		void CreateViewLight();
	public:
		virtual ~GameOverStage() {};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;

	};
}