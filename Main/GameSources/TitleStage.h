#pragma once
#include "stdafx.h"

namespace basecross {
	class TitleStage : public Stage {
		//ビューの作成
		void CreateViewLight();
	public:
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};
}