#pragma once
#include "stdafx.h"

namespace basecross {
	class ResultStage : public Stage {
		//�r���[�̍쐬
		void CreateViewLight();
		void CreateRemainingTime();
		void CreateMaximumChain();
		void CreateScore();
		bool m_productionStart = 1.0f;
	public:
		virtual ~ResultStage() {};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};
}