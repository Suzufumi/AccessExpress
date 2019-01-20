#pragma once
#include "stdafx.h"

namespace basecross {
	class ResultStage : public Stage {
		//ƒrƒ…[‚Ìì¬
		void CreateViewLight();
		void CreateRemainingTime();
		void CreateMaximumChain();
		void CreateScore();
	public:
		virtual ~ResultStage() {};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};
}