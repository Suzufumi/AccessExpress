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
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * 0.1f - 0.05f;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * 0.1f;
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne() * 0.1f - 0.05f;

			if (rParticleSprite.m_LocalPos.y < 0) {
				rParticleSprite.m_LocalScale = Vec2(-13.0f, -9.0f);
			}
			else {
				rParticleSprite.m_LocalScale = Vec2(13.0f, 9.0f);
			}

			//各パーティクルの移動速度を指定
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 5.0f,
				rParticleSprite.m_LocalPos.y * 5.0f,
				rParticleSprite.m_LocalPos.z * 5.0f
			);
			//色の指定
			rParticleSprite.m_Color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		}

	}
}