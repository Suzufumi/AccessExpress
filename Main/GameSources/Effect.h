#pragma once
#include "stdafx.h"

namespace basecross
{
	//-----------------------------------------------------------------------------------
	/// 獲得エフェクト
	//-----------------------------------------------------------------------------------
	class GetEffect : public MultiParticle
	{
	public:
		GetEffect(const shared_ptr<Stage>& stagePtr);
		virtual ~GetEffect();
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void InsertGetEffect(const Vec3& pos);
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