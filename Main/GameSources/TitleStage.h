#pragma once
#include "stdafx.h"

namespace basecross {
	class TitleStage : public Stage {
		//�r���[�̍쐬
		void CreateViewLight();
	public:
		virtual ~TitleStage() {};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};
}