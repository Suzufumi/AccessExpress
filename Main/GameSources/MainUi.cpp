#include "stdafx.h"
#include "Project.h"

namespace basecross{
	EnergyGaugeUI::EnergyGaugeUI(const shared_ptr<Stage>& stagePtr)
		: Sprite(stagePtr, L"UI_BASE_TX", Vec2(256, 256))
	{

	}

	EnergyGaugeUI::~EnergyGaugeUI()
	{}

	void EnergyGaugeUI::OnCreate() {
		Sprite::OnCreate();
		SetPosition(Vec2(133,123));
		struct GaugeParam {
			const wstring key;
			Vec2 size;
			Vec2 pos;
		};
		Vec2 size = Vec2(28,28);
		GaugeParam gauges[] = {
			{L"RED_GAUGE_TX", size,Vec2(117,191)},
			{L"RED_GAUGE_TX", size,Vec2(117,160)},
			{L"RED_GAUGE_TX", size,Vec2(117,129)},
			{L"YELLOW_GAUGE_TX", size,Vec2(168,192)},
			{L"YELLOW_GAUGE_TX", size,Vec2(168,161)},
			{L"YELLOW_GAUGE_TX", size,Vec2(168,130)},
			{L"YELLOW_GAUGE_TX", size,Vec2(168,99)},
			{L"GREEN_GAUGE_TX", size,Vec2(220,192)},
			{L"GREEN_GAUGE_TX", size,Vec2(220,161)},
			{L"GREEN_GAUGE_TX", size,Vec2(220,130)},
			{L"GREEN_GAUGE_TX", size,Vec2(220,99)},
			{L"GREEN_GAUGE_TX", size,Vec2(220,68)}
		};
		int count = 0;
		for (auto gauge : gauges) {
			m_energyGauge[count] = GetStage()->AddGameObject<Sprite>(gauge.key, gauge.size);
			m_energyGauge[count].lock()->SetPosition(gauge.pos);
			m_energyGauge[count].lock()->SetDrawLayer(1);
			count++;
		}
	}
	void EnergyGaugeUI::OnUpdate() {
		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
		//Ô—Ìˆæ
		for (int i = 0; i < 3; i++) {
			if (player->GetEnergy() < (3 + i * 5)) {
				m_energyGauge[i].lock()->SetDrawActive(false);
			}
			else {
				m_energyGauge[i].lock()->SetDrawActive(true);
			}
		}
		//‰©F—Ìˆæ
		for (int i = 0; i < 4; i++) {
			if (player->GetEnergy() < (18 + i * 8)) {
				m_energyGauge[3 + i].lock()->SetDrawActive(false);
			}
			else {
				m_energyGauge[3 + i].lock()->SetDrawActive(true);
			}
		}
		//—Î—Ìˆæ
		for (int i = 0; i < 5; i++) {
			if (player->GetEnergy() < (50 + i * 10)) {
				m_energyGauge[7 + i].lock()->SetDrawActive(false);
			}
			else {
				m_energyGauge[7 + i].lock()->SetDrawActive(true);
			}
		}
	}
}