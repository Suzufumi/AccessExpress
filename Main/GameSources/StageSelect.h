#pragma once
#include "stdafx.h"

namespace basecross {
	class StageSelect : public Stage {
		//�r���[�̍쐬
		void CreateViewLight();
	public:
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};
}