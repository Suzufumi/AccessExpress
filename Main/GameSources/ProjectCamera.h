#pragma once
#include "stdafx.h"

namespace basecross
{
	class ProjectCamera : public Camera
	{
	public:
		ProjectCamera();
		virtual ~ProjectCamera();

		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};
}