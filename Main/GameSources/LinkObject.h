#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	class LinkObject : public AccessObject{
		Vec3 m_position;
		Vec3 m_scale;
		Vec3 m_goPosition;
	public:
		LinkObject(const shared_ptr<Stage>& stagePtr,Vec3 pos,Vec3 scale);
		~LinkObject() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void Access() override {};
		Vec3 GetGoPosition() { return m_goPosition; }
		void SetGoPosition(Vec3 pos) { m_goPosition = pos; }
	};
}