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
		weak_ptr<Sprite> m_mailText;
		weak_ptr<NumberSprite> m_mailNum;
		weak_ptr<Sprite> m_maxChainText;
		weak_ptr<NumberSprite> m_maxChainNum;
		weak_ptr<Sprite> m_scoreText;
		weak_ptr<ScoreUI> m_scoreNum;
		weak_ptr<Sprite> m_push;
		Vec3 m_mailP1,m_maxChainP1,m_scoreP2;
		enum progress {
			START,
			ADD_SCORE,
			SCORE_COUNTUP,
			END
			//FADE
		};
		progress m_progress = progress::START;
	public:
		virtual ~ResultStage() {};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		void FadeProcess();
	};
}