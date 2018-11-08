#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	class SearchObject : public AccessObject
	{
	private:
		Vec3 m_position;
		Quat m_quat;
		Vec3 m_scale;
	public:
		SearchObject(const shared_ptr<Stage>& stagePtr, const Vec3 pos, const Quat quat, const Vec3 scale);
		~SearchObject(){}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void Access(AppType type) override;

	};
}