#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	class LinkObject : public AccessObject
	{
	public:
		LinkObject(const shared_ptr<Stage>& stagePtr);
		~LinkObject() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void Access(AppType type) override;
	};
}