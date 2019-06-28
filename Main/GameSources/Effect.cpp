#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	//-----------------------------------------------------------------------------------
	/// ���[���l���G�t�F�N�g
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
	}

	void GetEffect::InsertGetEffect(const Vec3& pos)
	{
		// �p�[�e�B�N���̐�
		const size_t ParticleCount = 10;
		// ��������
		const float SurvivalTime = 0.5f;
		// �ړ����x
		const float VelocityX = 5.0f;
		const float VelocityY = 10.0f;

		auto ptrParticle = InsertParticle(ParticleCount);
		ptrParticle->SetEmitterPos(pos);
		// �G�t�F�N�g�摜�̓ǂݍ���
		ptrParticle->SetTextureResource(L"GetEffect_TX");
		// �������Ԃ̐ݒ�
		ptrParticle->SetMaxTime(SurvivalTime);
		// �ǂݍ��񂾃G�t�F�N�g���X�v���C�g�ɂ��Đݒ�
		for (auto& rParticleSprite : ptrParticle->GetParticleSpriteVec()) {
			// �ǂ̕����Ɋg�U�����邩
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
				rParticleSprite.m_LocalPos.x * VelocityX,
				rParticleSprite.m_LocalPos.y * VelocityY,
				rParticleSprite.m_LocalPos.z * VelocityX
			);
			//�F�̎w��
			rParticleSprite.m_Color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		}

	}

	//-----------------------------------------------------------------------------------
	/// �{�[�i�X�G�t�F�N�g
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
		// �p�[�e�B�N���̐�
		const size_t ParticleCount = 15;
		// ��������
		const float SurvivalTime = 0.6f;
		// �ړ����x
		const float VelocityX = 5.0f;
		const float VelocityY = 15.0f;
		// �F�̎w��
		const Col4 ParticleColor = Col4(1.0f, 1.0f, 0.0f, 0.6f);

		auto ptrParticle = InsertParticle(ParticleCount);
		ptrParticle->SetEmitterPos(pos);
		// �G�t�F�N�g�摜�̓ǂݍ���
		ptrParticle->SetTextureResource(L"GetEffect_TX");
		// �������Ԃ̐ݒ�
		ptrParticle->SetMaxTime(SurvivalTime);
		// �ǂݍ��񂾃G�t�F�N�g���X�v���C�g�ɂ��Đݒ�
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
			//�e�p�[�e�B�N���̈ړ����x���w��
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * VelocityX,
				rParticleSprite.m_LocalPos.y * VelocityY,
				rParticleSprite.m_LocalPos.z * VelocityX
			);
			//�F�̎w��
			rParticleSprite.m_Color = ParticleColor;
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
		SetDrawLayer(-2);

	}

	void SparkEffect::OnUpdate()
	{
		MultiParticle::OnUpdate();
	}

	void SparkEffect::InsertSpark(const Vec3& pos)
	{
		// �p�[�e�B�N���̐�
		const size_t ParticleCount = 2;
		const float UpPos = 1.0f;
		const float HalfUp = 0.5f;
		// ��������
		const float SurvivalTime = 0.1f;
		// �ʒu�̒���
		const float PosX = 1.0f;
		const float PosY = 0.5f;
		// �ړ����x
		const float VelocityX = 3.0f;
		const float VelocityY = 1.0f;


		auto ptrParticle = InsertParticle(ParticleCount);
		// ���̈ʒu����������ɐ�������
		ptrParticle->SetEmitterPos(Vec3(pos.x - HalfUp, pos.y + UpPos, pos.z - HalfUp));
		// �G�t�F�N�g�摜�̓ǂݍ���
		ptrParticle->SetTextureResource(L"SPARK_TX");
		// �������Ԃ̐ݒ�
		ptrParticle->SetMaxTime(SurvivalTime);
		// �ǂݍ��񂾃G�t�F�N�g���X�v���C�g�ɂ��Đݒ�
		for (auto& rParticleSprite : ptrParticle->GetParticleSpriteVec()) {
			// �g�U����������̐ݒ�
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne() * PosX;
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne() * PosY;
			rParticleSprite.m_LocalPos.z = 0.0f;

			if (rParticleSprite.m_LocalPos.y < 0) {
				rParticleSprite.m_LocalScale = Vec2(-1.0f, -1.0f);
			}
			else {
				rParticleSprite.m_LocalScale = Vec2(1.0f, 1.0f);
			}

			//�e�p�[�e�B�N���̈ړ����x���w��
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * VelocityX,
				rParticleSprite.m_LocalPos.y * VelocityY,
				rParticleSprite.m_LocalPos.z * VelocityX
			);
			//�F�̎w��
			rParticleSprite.m_Color = Col4(1.0f);
		}

	}

	//-----------------------------------------------------------------------------------
	/// ���U���g�G�t�F�N�g
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
		// �p�[�e�B�N���̐�
		const size_t ParticleCount = 5;
		// ��������
		const float SurvivalTime = 0.3f;
		// �F�̎w��
		const Col4 ParticleColor = Col4(1.0f, 1.0f, 1.0f, 0.7f);
		// �ړ����x�̐ݒ�
		const float VelocityX = 3.0f;
		const float VelocityY = 1.0f;

		// �����̃p�[�e�B�N����ݒ�
		auto ptrParticle = InsertParticle(ParticleCount);
		ptrParticle->SetEmitterPos(Vec3(pos));
		// �G�t�F�N�g�摜�̓ǂݍ���
		ptrParticle->SetTextureResource(L"GetEffect_TX");
		// �������Ԃ̐ݒ�
		ptrParticle->SetMaxTime(SurvivalTime);
		// �ǂݍ��񂾃G�t�F�N�g���X�v���C�g�ɂ��Đݒ�
		for (auto& rParticleSprite : ptrParticle->GetParticleSpriteVec()) {
			// �g�U����������̐ݒ�
			rParticleSprite.m_LocalPos.x = Util::RandZeroToOne();
			rParticleSprite.m_LocalPos.y = Util::RandZeroToOne();
			rParticleSprite.m_LocalPos.z = Util::RandZeroToOne();

			if (rParticleSprite.m_LocalPos.y < 0) {
				rParticleSprite.m_LocalScale = Vec2(-scale);
			}
			else {
				rParticleSprite.m_LocalScale = Vec2(scale);
			}

			//�e�p�[�e�B�N���̈ړ����x���w��
			rParticleSprite.m_Velocity = Vec3(
				rParticleSprite.m_LocalPos.x * VelocityX,
				rParticleSprite.m_LocalPos.y * VelocityY,
				rParticleSprite.m_LocalPos.z * VelocityX
			);
			//�F�̎w��
			rParticleSprite.m_Color = ParticleColor;
		}
	}
}