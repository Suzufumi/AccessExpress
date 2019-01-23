#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//-----------------------------------------------------------------------------------
	/// 炎エフェクト
	//-----------------------------------------------------------------------------------
	class FireEffect : MultiParticle
	{
	public:
		FireEffect(const shared_ptr<Stage>& stagePtr);
		virtual ~FireEffect();
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void InsertEffect();
	};
}