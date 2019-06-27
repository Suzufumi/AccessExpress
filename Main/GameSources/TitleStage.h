#pragma once
#include "stdafx.h"

namespace basecross {
	class TitleStage : public Stage {
		weak_ptr<GameObject> m_player;
		//�r���[�̍쐬
		void CreateViewLight();
		// �v���C���[�̍쐬
		void CreateCharacter();
		// �^�C�g�����S���̍쐬
		void CreateTitleResources();
		// 
		void FadePlay();
	public:
		virtual ~TitleStage() {};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};
}