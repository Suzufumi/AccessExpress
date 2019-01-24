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
		//for (auto ptrParticle : GetParticleVec()) {
		//	for (auto& rParticleSprite : ptrParticle->GetParticleSpriteVec()) {
		//		if (rParticleSprite.m_Active) {
		//			rParticleSprite.m_Color.z += 0.05f;
		//			if (rParticleSprite.m_Color.z >= 1.0f) {
		//				rParticleSprite.m_Color.z = 1.0f;
		//			}
		//		}
		//	}
		//}
		//親クラスのOnUpdate()を呼ぶ
		MultiParticle::OnUpdate();
	}

	void FireEffect::InsertFire(const Vec3& pos)
	{
		auto ptrParticle = InsertParticle(4);
		ptrParticle->SetEmitterPos(pos);
		// エフェクト画像の読み込み
		ptrParticle->SetTextureResource(L"FIRE_TX");
		// 生存時間の設定
		ptrParticle->SetMaxTime(0.5f);
		// 読み込んだエフェクトをスプライトにして設定
		for (auto& rParticleSprite : ptrParticle->GetParticleSpriteVec()) {
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * 4.0f - 2.0f;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * 1.0f - 0.5f;
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne() * 4.0f - 2.0f;

			if (rParticleSprite.m_LocalPos.y < 0) {
				rParticleSprite.m_LocalScale = Vec2(-6.0f, -3.0f);
			}
			else {
				rParticleSprite.m_LocalScale = Vec2(6.0f, 3.0f);
			}

			//各パーティクルの移動速度を指定
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 10.0f,
				rParticleSprite.m_LocalPos.y * 10.0f,
				rParticleSprite.m_LocalPos.z * 10.0f
			);
			//色の指定
			rParticleSprite.m_Color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		}

	}

	//-----------------------------------------------------------------------------------
	/// 雷エフェクト
	//-----------------------------------------------------------------------------------
	SparkEffect::SparkEffect(const shared_ptr<Stage>& stagePtr)
		: MultiParticle(stagePtr)
	{}
	SparkEffect::~SparkEffect()
	{}

	void SparkEffect::OnCreate()
	{
		// 加算処理を設定
		SetAddType(true);
		SetDrawLayer(-2);

	}

	void SparkEffect::OnUpdate()
	{
		MultiParticle::OnUpdate();
	}

	void SparkEffect::InsertSpark(const Vec3& pos)
	{
		// 複数のパーティクルを設定(ここでは2)
		auto ptrParticle = InsertParticle(2);
		ptrParticle->SetEmitterPos(pos);
		// エフェクト画像の読み込み
		ptrParticle->SetTextureResource(L"SPARK_TX");
		// 生存時間の設定
		ptrParticle->SetMaxTime(0.1f);
		// 読み込んだエフェクトをスプライトにして設定
		for (auto& rParticleSprite : ptrParticle->GetParticleSpriteVec()) {
			// Spawnさせる場所の設定
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * 0.5f;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * 1.0f;
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne() * 0.5f;

			if (rParticleSprite.m_LocalPos.y < 0) {
				rParticleSprite.m_LocalScale = Vec2(-1.0f, -1.0f);
			}
			else {
				rParticleSprite.m_LocalScale = Vec2(1.0f, 1.0f);
			}

			//各パーティクルの移動速度を指定
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 3.0f,
				rParticleSprite.m_LocalPos.y * 1.0f,
				rParticleSprite.m_LocalPos.z * 3.0f
			);
			//色の指定
			rParticleSprite.m_Color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		}

	}
}