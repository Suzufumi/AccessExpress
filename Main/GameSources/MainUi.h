#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	class EnergyGaugeUI : public Sprite{
		weak_ptr<Sprite> m_energyGauge[12];
	public:
		EnergyGaugeUI(const shared_ptr<Stage>& stagePtr);
		virtual ~EnergyGaugeUI();
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}