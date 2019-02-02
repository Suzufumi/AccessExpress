#pragma once
#include "stdafx.h"
namespace basecross {
class AnimSprite;

	class StageSelect : public Stage {
		//�r���[�̍쐬
		void CreateViewLight();
		void CreateStageNum();
		//�X�v���C�g��weak_ptr�̔z��
		vector<weak_ptr<AnimSprite>> m_spVec;
		bool m_CntrolLock;
		weak_ptr<FadeInOut> m_fade;
	public:
		StageSelect()
			: Stage(), m_CntrolLock(false), m_spVec(2)
		{}
		virtual ~StageSelect() {};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		void ChangeSelect(int num);
		void Select();
	};
}