#pragma once
#include "stdafx.h"

namespace basecross {
	class ResultStage : public Stage {
		//�r���[�̍쐬
		void CreateViewLight();
	public:
		virtual ~ResultStage() {};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};
}