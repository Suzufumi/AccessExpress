#pragma once
#include "stdafx.h"

namespace basecross
{
	//-----------------------------------------------------------------------------------
	/// �l���G�t�F�N�g
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
	/// �{�[�i�X�G�t�F�N�g
	//-----------------------------------------------------------------------------------
	class BonusEffect : public MultiParticle
	{
	public:
		BonusEffect(const shared_ptr<Stage>& stagePtr);
		virtual ~BonusEffect();
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void InsertBounusEffect(const Vec3& pos, const Vec2& scale);
	};

	//-----------------------------------------------------------------------------------
	/// ���G�t�F�N�g
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