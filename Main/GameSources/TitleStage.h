#pragma once
#include "stdafx.h"

namespace basecross {
	class TitleStage : public Stage {
		weak_ptr<GameObject> m_player;
		//ƒrƒ…[‚Ìì¬
		void CreateViewLight();
		void CreatePlayer();
	public:
		virtual ~TitleStage() {};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};
}