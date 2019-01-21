#pragma once
#include "stdafx.h"

namespace basecross {
	class ResultStage : public Stage {
		//ÉrÉÖÅ[ÇÃçÏê¨
		void CreateViewLight();
		void CreateCollectedMail();
		void CreateMaximumChain();
		void CreateScore();
		void AddScore();
		void CreatePlayer();
		float m_leap = 0.0f;
		weak_ptr<GameObject> m_player;
		weak_ptr<Sprite> m_chackText;
		weak_ptr<NumberSprite> m_chackNum;
		weak_ptr<NumberSprite>m_chackNumParameter;
		weak_ptr<Sprite> m_maxChainText;
		weak_ptr<NumberSprite> m_maxChainNum;
		weak_ptr<Sprite> m_scoreText;
		weak_ptr<ScoreUI> m_scoreNum;
		weak_ptr<Sprite> m_push;
		Vec3 m_chackP1,m_maxChainP1,m_scoreP2;
		enum progress {
			START,
			ADD_SCORE,
			SCORE_COUNTUP,
			END
		};
		progress m_progress = progress::START;
	public:
		virtual ~ResultStage() {};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};
}