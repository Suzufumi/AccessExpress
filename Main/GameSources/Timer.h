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
	//���Ԑ����̃X�v���C�g
	//------------------------------------------------------------------
	class RemainingTimerSprite : public GameObject {
	protected:
		float m_timer;	// �c�莞��
		int m_places;	// �^�C���̕\����
		vector <Rect2D<float>> m_numRects;
		vector<shared_ptr<Sprite>>m_numbers;
		vector<vector<VertexPositionTexture>> m_vertices;

	public:
		RemainingTimerSprite(const shared_ptr<Stage>& stage, int timer);
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		virtual void OnDraw() override;

	};
}