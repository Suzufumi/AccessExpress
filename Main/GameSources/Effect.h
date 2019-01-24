#pragma once
#include "stdafx.h"

namespace basecross
{
	//-----------------------------------------------------------------------------------
	/// ���G�t�F�N�g
	//-----------------------------------------------------------------------------------
	class FireEffect : public MultiParticle
	{
	public:
		FireEffect(const shared_ptr<Stage>& stagePtr);
		virtual ~FireEffect();
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void InsertFire(const Vec3& pos);
	};
}