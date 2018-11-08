/*!
@file Player.cpp
@brief �v���C���[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	//------------------------------------------------------------------------------------------------
	//player�̃R���X�g���N�^
	//------------------------------------------------------------------------------------------------
	Player::Player(const shared_ptr<Stage>& StagePtr, Vec3 pos, Quat quat, Vec3 sca)
		:GameObject(StagePtr), m_position(pos), m_quaternion(quat), m_scale(sca)
	{
	}
	//-------------------------------------------------------------------------------------------------
	//������
	//-------------------------------------------------------------------------------------------------
	void Player::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();

		ptrTrans->SetWorldPosition(m_position);
		ptrTrans->SetQuaternion(m_quaternion);
		ptrTrans->SetScale(m_scale);

		//�������g�����߂ɓn��
		PsBoxParam Ps(ptrTrans->GetWorldMatrix(), 1.0f, false, PsMotionType::MotionTypeActive);
		//�����R���|�[�l���g���Z�b�g
		auto psPtr = AddComponent<RigidbodyBox>(Ps);
		//�����̏d�͂��g��Ȃ�
		psPtr->SetAutoGravity(false);
		//�l�p�`�̓����蔻����Z�b�g
		auto col = AddComponent<CollisionObb>();
		//�R���W�����̔�������Ȃ�
		col->SetAfterCollision(AfterCollision::None);
		//player�Ɏ������Ďg�����̂ɂ͏Փ˂��Ȃ�
		col->AddExcludeCollisionTag(L"PlayerUse");

		//�����������
		auto ptrString = AddComponent<StringSprite>();
		ptrString->SetText(L"");

		Mat4x4 spanMat; // ���f���ƃg�����X�t�H�[���̊Ԃ̍����s��
		spanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, Deg2Rad(180), 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//�`��R���|�[�l���g�̒ǉ�
		auto drawComp = AddComponent<PNTStaticModelDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		drawComp->SetMeshResource(L"PLAYER_MODEL");
		// �ϊ������s�����
		drawComp->SetMeshToTransformMatrix(spanMat);
		//Col4 Color(1.0f, 0.2f, 1.0f, 0.7f);
		//drawComp->SetDiffuse(Color);
		//drawComp->SetColorAndAlpha(Color);
		// ���C���[�̒���
		SetDrawLayer(1);
	}
	//-------------------------------------------------------------------------------------------------------------
	//Update
	//-------------------------------------------------------------------------------------------------------------
	void Player::OnUpdate() {
		// �A�v���P�[�V�����擾
		auto& app = App::GetApp();
		// 1�t���[���̎��s�ɂ����������Ԃ��擾
		float delta = app->GetElapsedTime();

		// �Q�[���R���g���[���[�擾
		auto device = app->GetInputDevice();
		m_pad = device.GetControlerVec()[0];
		//���X�e�B�b�N�̓��͕��������߂�
		m_padDir = Vec3(m_pad.fThumbLX, 0.0f, m_pad.fThumbLY);
		m_padDir = m_padDir.normalize();

		//�E�X�e�B�b�N�̒l�ŃJ�����̉�]�������s��
		CameraRoll();

		// �g�����X�t�H�[���R���|�[�l���g������W���擾����
		auto pos = GetComponent<Transform>()->GetWorldPosition();

		// �J�����̒����_���v���C���[�̍��W�ɍ��킹��
		auto camera = GetStage()->GetView()->GetTargetCamera();
		camera->SetAt(pos + Vec3(0.0f, 1.0f, 0.0f));
		auto eye = pos + Vec3(cos(m_AngleY) * m_cameraDistance, m_cameraHeight, sin(m_AngleY) * m_cameraDistance);
		camera->SetEye(eye);
	};

	//--------------------------------------------------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------------------------------------------------
	void Player::OnUpdate2() {
		//���݂�Velocity�l���擾
		m_velocity = GetComponent<RigidbodyBox>()->GetLinearVelocity();

		//�����蔻������o��
		auto hitJud = m_RadioTowerHitJudgment.lock();
		//�����蔻�肩�瑬�x�������Ă���
		m_nowWalkSpeed = hitJud->GetAcceleration();
		//�����蔻��̃X�s�[�h��������
		hitJud->Rset();

		//XZ���ʂ̈ړ��������s��
		Walk();

		//�㏸���~�̏���
		Floating();

		//Velocity�̍X�V
		GetComponent<RigidbodyBox>()->SetLinearVelocity(m_velocity);

		// �f�o�b�O�����̕\��
		DrawStrings();
	}
	//--------------------------------------------------------------------------------------------------------------
	//�Փ˂����Ƃ�
	//--------------------------------------------------------------------------------------------------------------
	void Player::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		//�t�@�C���ɕϊ�����
		auto file = dynamic_pointer_cast<File>(Other);
		if (file) {
			file->SetOnPlayer(GetThis<GameObject>());
			m_File = file;
			m_isHaveFile = true;
		}
		auto addless = dynamic_pointer_cast<AddlessCertificate>(Other);
		if (addless) {
			addless->FindGoal();
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//�Փ˂������Ă���Ƃ�
	//--------------------------------------------------------------------------------------------------------------
	void Player::OnCollisionExcute(shared_ptr<GameObject>& Other) {
		auto trans = GetComponent<Transform>();
		auto otherTrans = Other->GetComponent<Transform>();
		//player�̉��ʂƏՓ˂������̂̏�ʂ��������Ă�����
		if (((otherTrans->GetWorldPosition().y + otherTrans->GetScale().y / 2) - (trans->GetWorldPosition().y - trans->GetScale().y / 2)) < 0.5f) {
			//Y��������velocity��0�ɂ���
			auto vel = GetComponent<RigidbodyBox>()->GetLinearVelocity();
			vel.y = 0;
			GetComponent<RigidbodyBox>()->SetLinearVelocity(vel);
			//�I�u�W�F�N�g�̏�Ƀ|�W�V���������炷
			auto pos = trans->GetWorldPosition();
			pos.y = otherTrans->GetWorldPosition().y + otherTrans->GetScale().y / 2 + trans->GetScale().y / 2;
			trans->SetWorldPosition(pos);
			//������Y�����ɗ͂������鏈�����s��Ȃ��悤�ɂ���
			m_isFall = false;
		}
		//�h���C�u�ɕϊ��ł���
		auto drive = dynamic_pointer_cast<Drive>(Other);
		if (drive) {
			if (m_pad.wPressedButtons & XINPUT_GAMEPAD_B) {
				//player�̎����Ă���t�@�C�������o��
				auto file = m_File.lock();
				//���o������
				if (file != nullptr) {
					//�t�@�C���������Ă���(�����Ă���)
					if (m_isHaveFile) {
						//�����Ȃ�����
						file->UnLookFile();
						//�����ĂȂ�����
						m_isHaveFile = false;
					}
					//�t�@�C���������ĂȂ�(�����Ȃ�)
					else {
						//������悤�ɂ���
						file->LookFile();
						//�����Ă���
						m_isHaveFile = true;
					}
				}
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//�Փ˂��������ꂽ�Ƃ�
	//-------------------------------------------------------------------------------------------------------------
	void Player::OnCollisionExit(shared_ptr<GameObject>& Other) {
		m_isFall = true;
	}

	//-------------------------------------------------------------------------------------------------
	//�㏸���~�̏���
	//------------------------------------------------------------------------------------------------
	void Player::Floating() {
		//A�{�^�������ꂽ
		if (m_pad.wPressedButtons & XINPUT_GAMEPAD_A) {
			//�㏸���s�t���Otrue
			m_isFloa = true;
		}
		//A�{�^�����ꂽ
		if (m_pad.wReleasedButtons & XINPUT_GAMEPAD_A) {
			//�㏸���s�t���Ofalse
			m_isFloa = false;
		}
		//�㏸���Ȃ�
		if (m_isFloa == true) {
			//Y�������̗͂��Œ�l��
			m_velocity.y = m_nowWalkSpeed;
		}
		//�㏸������Ȃ��ċ󒆂ɂ���
		else if (m_isFall == true) {
			//���񂾂񉺕����֗͂�������Ă���
			m_velocity.y += -m_nowWalkSpeed * App::GetApp()->GetElapsedTime();
		}
	}

	//--------------------------------------------------------------------------------------------
	//XZ���ʂ̈ړ�����
	//--------------------------------------------------------------------------------------------
	void Player::Walk() {
		//���X�e�B�b�N�ɒl�����͂���Ă�����
		if (m_padDir.x > 0.4f || m_padDir.x < -0.4f ||
			m_padDir.z > 0.4f || m_padDir.z < -0.4f) {
			//�����ƈړ��X�s�[�h���|���Z����Velocity�ɗ^����
			m_velocity.x = m_nowWalkSpeed * m_forward.x;
			m_velocity.z = m_nowWalkSpeed * m_forward.z;
		}
		//���X�e�B�b�N�����͂���ĂȂ�������
		else {
			//Velocity�l�����񂾂񏬂�������
			m_velocity.x = m_velocity.x * 0.9f;
			m_velocity.z = m_velocity.x * 0.9f;
		}
	}
	//--------------------------------------------------------------------------------------------
	//�J�����̉�]����
	//--------------------------------------------------------------------------------------------
	void Player::CameraRoll() {
		float delta = App::GetApp()->GetElapsedTime();
		// �X�e�B�b�N�̌X�����p�x�ɕϊ�����
		float padRad = atan2f(m_padDir.z, m_padDir.x);

		m_AngleY += -m_pad.fThumbRX * m_maxAngleSpeed * delta; // �J��������]������
		m_cameraHeight += -m_pad.fThumbRY * m_maxAngleSpeed * delta; // �J���������~������

		//360�x���z������0�ɂ���
		if (m_AngleX > 360) {
			m_AngleX = 0;
		}
		//0�x�����������Ȃ�����
		if (m_AngleX < 0) {
			m_AngleX = 360;
		}

		if (m_padDir.length() != 0.0f) {
			// �X�e�B�b�N�̊p�x�ɃJ�����̊p�x��������
			padRad += m_AngleY + XM_PIDIV2;  // �J�����̊p�x�ɂX�O�x������
			// �p�x��XZ�����ɕ������ăx�N�g������蒼��
			m_padDir.x = cosf(padRad); // �V�����p�x�� X �����ɕ�������
			m_padDir.z = sinf(padRad); // �V�����p�x�� Z �����ɕ�������

			m_forward = m_padDir;
		}
	}

	void Player::DrawStrings()
	{
		// FPS�̎擾
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring strFps(L"FPS: ");
		strFps += Util::UintToWStr(fps);
		wstring str = strFps;
		//�����������
		auto ptrString = GetComponent<StringSprite>();
		ptrString->SetText(str);

	}

}
//end basecross

