#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	TpsCamera::TpsCamera()
		: Camera(),
		m_AngleX(0.0f),
		m_AngleY(Deg2Rad(270)),
		m_cameraHeight(3.0f),
		m_maxAngleSpeed(PI),
		m_cameraDistance(10.0f)
	{}

	TpsCamera::~TpsCamera()
	{}

	void TpsCamera::OnCreate()
	{

	}

	void TpsCamera::OnUpdate()
	{

	}

	void TpsCamera::SetTarget(const shared_ptr<Transform>& target)
	{
	}

}