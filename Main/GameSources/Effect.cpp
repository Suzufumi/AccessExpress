#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//-----------------------------------------------------------------------------------
	/// �l���G�t�F�N�g
	//-----------------------------------------------------------------------------------
	GetEffect::GetEffect(const shared_ptr<Stage>& stagePtr)
		: MultiParticle(stagePtr)
	{}

	GetEffect::~GetEffect()
	{}

	void GetEffect::OnCreate()
	{
		// ���Z�`�揈��������
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
		// �e�N���X��OnUpdate()���Ă�
		MultiParticle::OnUpdate();
	}

	void GetEffect::InsertGetEffect(const Vec3& pos)
	{
		auto ptrParticle = InsertParticle(10);
		ptrParticle->SetEmitterPos(pos);
		// �G�t�F�N�g�摜�̓ǂݍ���
		ptrParticle->SetTextureResource(L"GetEffect_TX");
		// �������Ԃ̐ݒ�
		ptrParticle->SetMaxTime(200.0f);
		// �ǂݍ��񂾃G�t�F�N�g���X�v���C�g�ɂ��Đݒ�
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

			//�e�p�[�e�B�N���̈ړ����x���w��
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * 5.0f,
				rParticleSprite.m_LocalPos.y * 10.0f,
				rParticleSprite.m_LocalPos.z * 5.0f
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
		// �����̃p�[�e�B�N����ݒ�(�����ł�2)
		auto ptrParticle = InsertParticle(1);
		ptrParticle->SetEmitterPos(Vec3(pos.x - 0.5f, pos.y + 1.0f, pos.z - 0.5f));
		// �G�t�F�N�g�摜�̓ǂݍ���
		ptrParticle->SetTextureResource(L"SPARK_TX");
		// �������Ԃ̐ݒ�
		ptrParticle->SetMaxTime(0.1f);
		// �ǂݍ��񂾃G�t�F�N�g���X�v���C�g�ɂ��Đݒ�
		for (auto& rParticleSprite : ptrParticle->GetParticleSpriteVec()) {
			// �g�U����������̐ݒ�
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * 1.0f;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * 0.5f;
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne() * 0.0f;

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