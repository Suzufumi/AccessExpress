#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	class AccessObject : public GameObject {
	public:
		enum AppType
		{
			None,
			Search,
			Link
		};
	protected:
		AppType m_accessType = None;
	public:
		AccessObject(const shared_ptr<Stage>& StagePtr)
			: GameObject(StagePtr)
		{}
		virtual ~AccessObject()
		{}
		virtual void Access()
		{}
		virtual void OnCreate() override {}
		virtual void OnUpdate() override {}

		AppType GetAccessType()
		{
			m_accessType;
		}
	};

}