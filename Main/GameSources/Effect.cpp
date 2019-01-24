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
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * 0.1f - 0.05f;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * 0.1f;
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne() * 0.1f - 0.05f;

			if (rParticleSprite.m_LocalPos.y < 0) {
				rParticleSprite.m_LocalScale = Vec2(-13.0f, -9.0f);
			}
			else {
				rParticleSprite.m_LocalScale = Vec2(13.0f, 9.0f);
			}

			//�e�p�[�e�B�N���̈ړ����x���w��
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 5.0f,
				rParticleSprite.m_LocalPos.y * 5.0f,
				rParticleSprite.m_LocalPos.z * 5.0f
			);
			//�F�̎w��
			rParticleSprite.m_Color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		}

	}
}