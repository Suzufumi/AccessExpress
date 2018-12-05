#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	class EnegyGaugeUI : public Sprite
	{
	public:
		EnegyGaugeUI(const shared_ptr<Stage>& stagePtr);
		virtual ~EnegyGaugeUI();
		virtual void OnCreate() override;
	};
}