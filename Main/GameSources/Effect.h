#pragma once
#include "stdafx.h"

namespace basecross
{
	//-----------------------------------------------------------------------------------
	/// 炎エフェクト
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

	//-----------------------------------------------------------------------------------
	/// 雷エフェクト
	//-----------------------------------------------------------------------------------
	class SparkEffect : public MultiParticle
	{
	public:
		SparkEffect(const shared_ptr<Stage>& stagePtr);
		virtual ~SparkEffect();
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void InsertSpark(const Vec3& pos);
	};
}