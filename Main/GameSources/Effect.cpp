#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//-----------------------------------------------------------------------------------
	/// ���G�t�F�N�g
	//-----------------------------------------------------------------------------------
	FireEffect::FireEffect(const shared_ptr<Stage>& stagePtr)
		: MultiParticle(stagePtr)
	{}

	FireEffect::~FireEffect()
	{}

	void FireEffect::OnCreate()
	{
		// ���Z�`�揈��������
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
		//�e�N���X��OnUpdate()���Ă�
		MultiParticle::OnUpdate();
	}

	void FireEffect::InsertFire(const Vec3& pos)
	{
		auto ptrParticle = InsertParticle(4);
		ptrParticle->SetEmitterPos(pos);
		// �G�t�F�N�g�摜�̓ǂݍ���
		ptrParticle->SetTextureResource(L"FIRE_TX");
		// �������Ԃ̐ݒ�
		ptrParticle->SetMaxTime(0.5f);
		// �ǂݍ��񂾃G�t�F�N�g���X�v���C�g�ɂ��Đݒ�
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

			//�e�p�[�e�B�N���̈ړ����x���w��
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 10.0f,
				rParticleSprite.m_LocalPos.y * 10.0f,
				rParticleSprite.m_LocalPos.z * 10.0f
			);
			//�F�̎w��
			rParticleSprite.m_Color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		}

	}

	//-----------------------------------------------------------------------------------
	/// ���G�t�F�N�g
	//-----------------------------------------------------------------------------------
	SparkEffect::SparkEffect(const shared_ptr<Stage>& stagePtr)
		: MultiParticle(stagePtr)
	{}
	SparkEffect::~SparkEffect()
	{}

	void SparkEffect::OnCreate()
	{
		// ���Z������ݒ�
		SetAddType(true);
		SetDrawLayer(-2);

	}

	void SparkEffect::OnUpdate()
	{
		MultiParticle::OnUpdate();
	}

	void SparkEffect::InsertSpark(const Vec3& pos)
	{
		// �����̃p�[�e�B�N����ݒ�(�����ł�2)
		auto ptrParticle = InsertParticle(2);
		ptrParticle->SetEmitterPos(pos);
		// �G�t�F�N�g�摜�̓ǂݍ���
		ptrParticle->SetTextureResource(L"SPARK_TX");
		// �������Ԃ̐ݒ�
		ptrParticle->SetMaxTime(0.1f);
		// �ǂݍ��񂾃G�t�F�N�g���X�v���C�g�ɂ��Đݒ�
		for (auto& rParticleSprite : ptrParticle->GetParticleSpriteVec()) {
			// Spawn������ꏊ�̐ݒ�
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * 0.5f;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * 1.0f;
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne() * 0.5f;

			if (rParticleSprite.m_LocalPos.y < 0) {
				rParticleSprite.m_LocalScale = Vec2(-1.0f, -1.0f);
			}
			else {
				rParticleSprite.m_LocalScale = Vec2(1.0f, 1.0f);
			}

			//�e�p�[�e�B�N���̈ړ����x���w��
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 3.0f,
				rParticleSprite.m_LocalPos.y * 1.0f,
				rParticleSprite.m_LocalPos.z * 3.0f
			);
			//�F�̎w��
			rParticleSprite.m_Color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		}

	}
}