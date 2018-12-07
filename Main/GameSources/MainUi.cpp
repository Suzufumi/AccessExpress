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
		GaugeParam gauges[] = {
			{L"RED_GAUGE_TX", Vec2(32, 32),Vec2(116,192)},
			{L"RED_GAUGE_TX", Vec2(32, 32),Vec2(116,160)},
			{L"RED_GAUGE_TX", Vec2(32, 32),Vec2(116,128)},
			{L"YELLOW_GAUGE_TX", Vec2(32, 32),Vec2(168,192)},
			{L"YELLOW_GAUGE_TX", Vec2(32, 32),Vec2(168,160)},
			{L"YELLOW_GAUGE_TX", Vec2(32, 32),Vec2(168,128)},
			{L"YELLOW_GAUGE_TX", Vec2(32, 32),Vec2(168,96)},
			{L"GREEN_GAUGE_TX", Vec2(32, 32),Vec2(220,192)},
			{L"GREEN_GAUGE_TX", Vec2(32, 32),Vec2(220,160)},
			{L"GREEN_GAUGE_TX", Vec2(32, 32),Vec2(220,128)},
			{L"GREEN_GAUGE_TX", Vec2(32, 32),Vec2(220,96)},
			{L"GREEN_GAUGE_TX", Vec2(32, 32),Vec2(220,64)}
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