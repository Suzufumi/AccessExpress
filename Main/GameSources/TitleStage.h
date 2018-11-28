#pragma once
#include "stdafx.h"

namespace basecross {
	class TitleStage : public Stage {
		//ƒrƒ…[‚Ìì¬
		void CreateViewLight();
	public:
		virtual ~TitleStage() {};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};
}