#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//-----------------------------------------------------------------------------------
	/// 獲得エフェクト
	//-----------------------------------------------------------------------------------
	GetEffect::GetEffect(const shared_ptr<Stage>& stagePtr)
		: MultiParticle(stagePtr)
	{}

	GetEffect::~GetEffect()
	{}

	void GetEffect::OnCreate()
	{
		// 加算描画処理をする
		SetAddType(true);
	}

	void GetEffect::OnUpdate()
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
		// 親クラスのOnUpdate()を呼ぶ
		MultiParticle::OnUpdate();
	}

	void GetEffect::InsertGetEffect(const Vec3& pos)
	{
		auto ptrParticle = InsertParticle(10);
		ptrParticle->SetEmitterPos(pos);
		// エフェクト画像の読み込み
		ptrParticle->SetTextureResource(L"GetEffect_TX");
		// 生存時間の設定
		ptrParticle->SetMaxTime(200.0f);
		// 読み込んだエフェクトをスプライトにして設定
		for (auto& rParticleSprite : ptrParticle->GetParticleSpriteVec()) {
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * 2.0f;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * 1.0f;
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne() * 2.0f;

			if (rParticleSprite.m_LocalPos.y < 0) {
				rParticleSprite.m_LocalScale = Vec2(-1.0f, -1.0f);
			}
			else {
				rParticleSprite.m_LocalScale = Vec2(1.0f, 1.0f);
			}

			//各パーティクルの移動速度を指定
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 5.0f,
				rParticleSprite.m_LocalPos.y * 10.0f,
				rParticleSprite.m_LocalPos.z * 5.0f
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
		//SetAddType(true);

		//auto enemyGroup = GetStage()->GetSharedObjectGroup(L"Drone");
		//for (auto& enemy : enemyGroup->GetGroupVector())
		//{
		//	auto enemyObj = dynamic_pointer_cast<Drone>(enemy.lock());
		//	auto transComp = GetComponent<Transform>();
		//	transComp->SetParent(enemyObj);
		//}
		SetDrawLayer(-2);

	}

	void SparkEffect::OnUpdate()
	{
		MultiParticle::OnUpdate();
	}

	void SparkEffect::InsertSpark(const Vec3& pos)
	{
		// 複数のパーティクルを設定(ここでは2)
		auto ptrParticle = InsertParticle(1);
		ptrParticle->SetEmitterPos(Vec3(pos.x - 0.5f, pos.y + 1.0f, pos.z - 0.5f));
		// エフェクト画像の読み込み
		ptrParticle->SetTextureResource(L"SPARK_TX");
		// 生存時間の設定
		ptrParticle->SetMaxTime(0.1f);
		// 読み込んだエフェクトをスプライトにして設定
		for (auto& rParticleSprite : ptrParticle->GetParticleSpriteVec()) {
			// 拡散させる方向の設定
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * 1.0f;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * 0.5f;
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne() * 0.0f;

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