#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class RemainingTimer : public GameObject {
		float m_remainingTime = 40.0f;
	public:
		RemainingTimer(const shared_ptr<Stage>& stage);
		virtual ~RemainingTimer() {}
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	//------------------------------------------------------------------
	//時間制限のスプライト
	//------------------------------------------------------------------
	class RemainingTimerSprite : public GameObject {
	protected:
		const int NOTICE_TIME = 5;	//
		float m_timer;	// 残り時間
		int m_places;	// タイムの表示桁
		bool m_isSetTimer = false;	// 既定の時間になったらオンにする
		vector <Rect2D<float>> m_numRects;
		vector<shared_ptr<Sprite>>m_numbers;
		vector<vector<VertexPositionTexture>> m_vertices;

	public:
		RemainingTimerSprite(const shared_ptr<Stage>& stage, int timer);
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnDraw() override;
		void SpriteColorChange();
	};
}