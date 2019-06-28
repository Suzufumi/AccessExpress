#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//-----------------------------------------------------------------------------------
	/// メール獲得エフェクト
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
	}

	void GetEffect::InsertGetEffect(const Vec3& pos)
	{
		// パーティクルの数
		const size_t ParticleCount = 10;
		// 生存時間
		const float SurvivalTime = 0.5f;
		// 移動速度
		const float VelocityX = 5.0f;
		const float VelocityY = 10.0f;

		auto ptrParticle = InsertParticle(ParticleCount);
		ptrParticle->SetEmitterPos(pos);
		// エフェクト画像の読み込み
		ptrParticle->SetTextureResource(L"GetEffect_TX");
		// 生存時間の設定
		ptrParticle->SetMaxTime(SurvivalTime);
		// 読み込んだエフェクトをスプライトにして設定
		for (auto& rParticleSprite : ptrParticle->GetParticleSpriteVec()) {
			// どの方向に拡散させるか
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
				rParticleSprite.m_LocalPos.x * VelocityX,
				rParticleSprite.m_LocalPos.y * VelocityY,
				rParticleSprite.m_LocalPos.z * VelocityX
			);
			//色の指定
			rParticleSprite.m_Color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		}

	}

	//-----------------------------------------------------------------------------------
	/// ボーナスエフェクト
	//-----------------------------------------------------------------------------------
	BonusEffect::BonusEffect(const shared_ptr<Stage>& stagePtr)
		: MultiParticle(stagePtr)
	{}
	BonusEffect::~BonusEffect()
	{}

	void BonusEffect::OnCreate()
	{
		SetAddType(true);
	}

	void BonusEffect::OnUpdate()
	{
		MultiParticle::OnUpdate();
	}

	void BonusEffect::InsertBounusEffect(const Vec3& pos, const Vec2& scale)
	{
		// パーティクルの数
		const size_t ParticleCount = 15;
		// 生存時間
		const float SurvivalTime = 0.6f;
		// 移動速度
		const float VelocityX = 5.0f;
		const float VelocityY = 15.0f;
		// 色の指定
		const Col4 ParticleColor = Col4(1.0f, 1.0f, 0.0f, 0.6f);

		auto ptrParticle = InsertParticle(ParticleCount);
		ptrParticle->SetEmitterPos(pos);
		// エフェクト画像の読み込み
		ptrParticle->SetTextureResource(L"GetEffect_TX");
		// 生存時間の設定
		ptrParticle->SetMaxTime(SurvivalTime);
		// 読み込んだエフェクトをスプライトにして設定
		for (auto& rParticleSprite : ptrParticle->GetParticleSpriteVec()) {
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne();
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne();
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne();

			if (rParticleSprite.m_LocalPos.y < 0) {
				rParticleSprite.m_LocalScale = Vec2(-scale);
			}
			else {
				rParticleSprite.m_LocalScale = Vec2(scale);
			}

			auto camera = GetStage()->GetView()->GetTargetCamera();
			auto tpsCamera = dynamic_pointer_cast<TpsCamera>(camera);
			auto distance = tpsCamera->GetEye() - pos;
			auto dir = distance.normalize();
			//各パーティクルの移動速度を指定
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * VelocityX,
				rParticleSprite.m_LocalPos.y * VelocityY,
				rParticleSprite.m_LocalPos.z * VelocityX
			);
			//色の指定
			rParticleSprite.m_Color = ParticleColor;
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
		SetDrawLayer(-2);

	}

	void SparkEffect::OnUpdate()
	{
		MultiParticle::OnUpdate();
	}

	void SparkEffect::InsertSpark(const Vec3& pos)
	{
		// パーティクルの数
		const size_t ParticleCount = 2;
		const float UpPos = 1.0f;
		const float HalfUp = 0.5f;
		// 生存時間
		const float SurvivalTime = 0.1f;
		// 位置の調整
		const float PosX = 1.0f;
		const float PosY = 0.5f;
		// 移動速度
		const float VelocityX = 3.0f;
		const float VelocityY = 1.0f;


		auto ptrParticle = InsertParticle(ParticleCount);
		// 元の位置よりも少し上に生成する
		ptrParticle->SetEmitterPos(Vec3(pos.x - HalfUp, pos.y + UpPos, pos.z - HalfUp));
		// エフェクト画像の読み込み
		ptrParticle->SetTextureResource(L"SPARK_TX");
		// 生存時間の設定
		ptrParticle->SetMaxTime(SurvivalTime);
		// 読み込んだエフェクトをスプライトにして設定
		for (auto& rParticleSprite : ptrParticle->GetParticleSpriteVec()) {
			// 拡散させる方向の設定
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * PosX;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * PosY;
			rParticleSprite.m_LocalPos.z = 0.0f;

			if (rParticleSprite.m_LocalPos.y < 0) {
				rParticleSprite.m_LocalScale = Vec2(-1.0f, -1.0f);
			}
			else {
				rParticleSprite.m_LocalScale = Vec2(1.0f, 1.0f);
			}

			//各パーティクルの移動速度を指定
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * VelocityX,
				rParticleSprite.m_LocalPos.y * VelocityY,
				rParticleSprite.m_LocalPos.z * VelocityX
			);
			//色の指定
			rParticleSprite.m_Color = Col4(1.0f);
		}

	}

	//-----------------------------------------------------------------------------------
	/// リザルトエフェクト
	//-----------------------------------------------------------------------------------
	ResultEffect::ResultEffect(const shared_ptr<Stage>& stagePtr)
		: MultiParticle(stagePtr)
	{}
	ResultEffect::~ResultEffect()
	{}
	void ResultEffect::OnCreate()
	{
		SetAddType(true);
	}
	void ResultEffect::OnUpdate()
	{
		MultiParticle::OnUpdate();
	}
	void ResultEffect::InsertResultEffect(const Vec3& pos, const Vec2& scale)
	{
		// パーティクルの数
		const size_t ParticleCount = 5;
		// 生存時間
		const float SurvivalTime = 0.3f;
		// 色の指定
		const Col4 ParticleColor = Col4(1.0f, 1.0f, 1.0f, 0.7f);
		// 移動速度の設定
		const float VelocityX = 3.0f;
		const float VelocityY = 1.0f;

		// 複数のパーティクルを設定
		auto ptrParticle = InsertParticle(ParticleCount);
		ptrParticle->SetEmitterPos(Vec3(pos));
		// エフェクト画像の読み込み
		ptrParticle->SetTextureResource(L"GetEffect_TX");
		// 生存時間の設定
		ptrParticle->SetMaxTime(SurvivalTime);
		// 読み込んだエフェクトをスプライトにして設定
		for (auto& rParticleSprite : ptrParticle->GetParticleSpriteVec()) {
			// 拡散させる方向の設定
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne();
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne();
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne();

			if (rParticleSprite.m_LocalPos.y < 0) {
				rParticleSprite.m_LocalScale = Vec2(-scale);
			}
			else {
				rParticleSprite.m_LocalScale = Vec2(scale);
			}

			//各パーティクルの移動速度を指定
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * VelocityX,
				rParticleSprite.m_LocalPos.y * VelocityY,
				rParticleSprite.m_LocalPos.z * VelocityX
			);
			//色の指定
			rParticleSprite.m_Color = ParticleColor;
		}
	}
}