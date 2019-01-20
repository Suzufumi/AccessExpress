#pragma once
#include "stdafx.h"

namespace basecross {
	class ResultStage : public Stage {
		//ÉrÉÖÅ[ÇÃçÏê¨
		void CreateViewLight();
		void CreateRemainingTime();
		void CreateMaximumChain();
		void CreateScore();
		void AddScore();
		float m_leap = 0.0f;
		weak_ptr<GameObject> mchackNum;
		weak_ptr<GameObject> m_maxChainNum;
		Vec3 m_chackP1,m_maxChainP1,m_scoreP2;
		enum progress {
			START,
			ADD_SCORE,
			END
		};
		progress m_progress = progress::START;
	public:
		virtual ~ResultStage() {};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};
}