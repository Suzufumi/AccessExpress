#pragma once
#include "stdafx.h"

namespace basecross {
	class GameOverStage : public Stage {
		//ビューの作成
		void CreateViewLight();
	public:
		virtual ~GameOverStage() {};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;

	};
}