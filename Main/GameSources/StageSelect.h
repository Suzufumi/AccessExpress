#pragma once
#include "stdafx.h"
namespace basecross {
class AnimSprite;

	class StageSelect : public Stage {
		//�r���[�̍쐬
		void CreateViewLight();
		void CreateStageNum();
		//�X�v���C�g��weak_ptr�̔z��
		vector<weak_ptr<AnimSprite>> m_SpVec;
		bool m_CntrolLock;
	public:
		StageSelect()
			: Stage(), m_CntrolLock(false), m_SpVec(5)
		{}
		virtual ~StageSelect() {};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		void StageSel();
		void ChangeSelect(int num);
	};
}