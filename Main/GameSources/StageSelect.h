#pragma once
#include "stdafx.h"
namespace basecross {
class AnimSprite;

	class StageSelect : public Stage {
		//ビューの作成
		void CreateViewLight();
		void CreateStageNum();
		//スプライトのweak_ptrの配列
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