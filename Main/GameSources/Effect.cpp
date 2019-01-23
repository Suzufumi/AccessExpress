#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//-----------------------------------------------------------------------------------
	/// 炎エフェクト
	//-----------------------------------------------------------------------------------
	FireEffect::FireEffect(const shared_ptr<Stage>& stagePtr)
		: MultiParticle(stagePtr)
	{}

	FireEffect::~FireEffect()
	{}

	void FireEffect::OnCreate()
	{
		// 加算描画処理をする
		SetAddType(true);
	}

	void FireEffect::OnUpdate()
	{

	}
}