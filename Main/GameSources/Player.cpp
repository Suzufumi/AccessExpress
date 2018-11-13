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
		//�l�p�`�̓����蔻����Z�b�g
		auto col = AddComponent<CollisionObb>();
		col->SetDrawActive(true);
		//�R���W�����̔�������Ȃ�
		col->SetAfterCollision(AfterCollision::None);
		//player�Ɏ������Ďg�����̂ɂ͏Փ˂��Ȃ�
		col->AddExcludeCollisionTag(L"PlayerUse");

		//�����������
		auto ptrString = AddComponent<StringSprite>();
		ptrString->SetText(L"");

		Mat4x4 spanMat; // ���f���ƃg�����X�t�H�[���̊Ԃ̍����s��
		spanMat.affineTransformation(
			Vec3(1.0f, 0.5f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, Deg2Rad(180), 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
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

		m_StateMachine.reset(new StateMachine<Player>(GetThis<Player>()));
		//�ŏ��̃X�e�[�g��WalkState�ɐݒ�
		m_StateMachine->ChangeState(WalkState::Instance());
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

		//�X�e�[�g�}�V���̃A�b�v�f�[�g
		m_StateMachine->Update();
	};

	//--------------------------------------------------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------------------------------------------------
	void Player::OnUpdate2() {
		////�����蔻������o��
		//auto hitJud = m_RadioTowerHitJudgment.lock();
		////�����蔻�肩�瑬�x�������Ă���
		//m_nowWalkSpeed = hitJud->GetAcceleration();
		////�����蔻��̃X�s�[�h��������
		//hitJud->Rset();

		// �g�����X�t�H�[���R���|�[�l���g������W���擾����
		auto pos = GetComponent<Transform>()->GetWorldPosition();

		// �J�����̒����_���v���C���[�̍��W�ɍ��킹��
		auto camera = GetStage()->GetView()->GetTargetCamera();
		camera->SetAt(pos + Vec3(0.0f, 1.0f, 0.0f));
		auto eye = pos + Vec3(cos(m_AngleY) * m_cameraDistance, m_cameraHeight, sin(m_AngleY) * m_cameraDistance);
		camera->SetEye(eye);


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
			if (!addless->GetUnlockGoal())
			{
				addless->FindGoal();
				addless->SetDrawActive(false);
			}
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
			//������Y�����ɗ͂������鏈�����s��Ȃ��悤�ɂ���
			m_isFall = false;
		}

		Extrusion(Other);

		auto access = dynamic_pointer_cast<LinkObject>(Other);
		if (access) {
			if (state != L"Link") {
				if (m_pad.wPressedButtons & XINPUT_GAMEPAD_B) {
					SetBezierPoint(access->GetGoPosition());
					m_Lerp = 0;
					state = L"Link";
					m_StateMachine->ChangeState(LinkState::Instance());
				}
			}
		}
		//�h���C�u�ɕϊ��ł���
		//auto drive = dynamic_pointer_cast<Drive>(Other);
		//if (drive) {
		//	if (m_pad.wPressedButtons & XINPUT_GAMEPAD_B) {
		//		//player�̎����Ă���t�@�C�������o��
		//		auto file = m_File.lock();
		//		//���o������
		//		if (file != nullptr) {
		//			//�t�@�C���������Ă���(�����Ă���)
		//			if (m_isHaveFile) {
		//				//�����Ȃ�����
		//				file->UnLookFile();
		//				//�����ĂȂ�����
		//				m_isHaveFile = false;
		//			}
		//			//�t�@�C���������ĂȂ�(�����Ȃ�)
		//			else {
		//				//������悤�ɂ���
		//				file->LookFile();
		//				//�����Ă���
		//				m_isHaveFile = true;
		//			}
		//		}
		//	}
		//}
	}
	//--------------------------------------------------------------------------------------------------------------
	//�Փ˂��������ꂽ�Ƃ�
	//-------------------------------------------------------------------------------------------------------------
	void Player::OnCollisionExit(shared_ptr<GameObject>& Other) {
		m_isFall = true;
	}


	//--------------------------------------------------------------------------------------------
	//XZ���ʂ̈ړ�����
	//--------------------------------------------------------------------------------------------
	void Player::Walk() {
		auto playerTrans = GetComponent<Transform>();
		auto playerPos = playerTrans->GetWorldPosition();
		//���X�e�B�b�N�ɒl�����͂���Ă�����
		if (m_padDir.x > 0.4f || m_padDir.x < -0.4f ||
			m_padDir.z > 0.4f || m_padDir.z < -0.4f) {
			//�����ƈړ��X�s�[�h���|���Z����Velocity�ɗ^����
			playerPos.x += m_nowWalkSpeed * m_forward.x * App::GetApp()->GetElapsedTime();
			playerPos.z += m_nowWalkSpeed * m_forward.z * App::GetApp()->GetElapsedTime();
		}
		//���X�e�B�b�N�����͂���ĂȂ�������
		//else {
		//	//Velocity�l�����񂾂񏬂�������
		//	m_velocity.x = m_velocity.x * 0.9f;
		//	m_velocity.z = m_velocity.x * 0.9f;
		//}
		if (m_isFall) {
			playerPos.y += -m_nowFallSpeed * App::GetApp()->GetElapsedTime();
		}
		playerTrans->SetWorldPosition(playerPos);
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
	//�����N���烊���N�֔�ԏ���
	void Player::LinkGo() {
		auto pos = GetComponent<Transform>()->GetWorldPosition();
		//�v�Z�̂��߂̎��ԉ��Z
		m_Lerp += App::GetApp()->GetElapsedTime();
		if (m_Lerp >= 1.0f) {
			m_Lerp = 1.0f;
			state = L"walk";
			//��яI�������X�e�[�g������ɂ���
			m_StateMachine->ChangeState(WalkState::Instance());
		}
		//�x�W�G�Ȑ��̌v�Z
		pos.x = (1 - m_Lerp) * (1 - m_Lerp) * p0.x + 2 * (1 - m_Lerp) * m_Lerp * p1.x + m_Lerp * m_Lerp * p2.x;
		pos.y = (1 - m_Lerp) * (1 - m_Lerp) * p0.y + 2 * (1 - m_Lerp) * m_Lerp * p1.y + m_Lerp * m_Lerp * p2.y;
		pos.z = (1 - m_Lerp) * (1 - m_Lerp) * p0.z + 2 * (1 - m_Lerp) * m_Lerp * p1.z + m_Lerp * m_Lerp * p2.z;
		GetComponent<Transform>()->SetWorldPosition(pos);
	}
	void Player::SetBezierPoint(Vec3 point) {
		p0 = GetComponent<Transform>()->GetWorldPosition();
		p1 = point + Vec3(0, 10, 0);
		p2 = point + Vec3(0,1,0);
	}
	void Player::Extrusion(const weak_ptr<GameObject>& Other) {
		//player�̏��
		auto trans = GetComponent<Transform>();
		auto pos = trans->GetWorldPosition();
		auto scale = trans->GetScale();
		auto half = scale * 0.5f;
		//�Փ˂����I�u�W�F�N�g�̏��
		auto other = Other.lock();
		auto otherTrans = other->GetComponent<Transform>();
		auto otherPos = otherTrans->GetWorldPosition();
		auto otherScale = otherTrans->GetScale();
		auto otherHalf = otherScale * 0.5f;
		// �Փ˂��Ă���̂ŉ������s��
		float diff[6] = {
			(otherPos.x + otherHalf.x) - (pos.x - half.x), // �E
			(pos.x + half.x) - (otherPos.x - otherHalf.x), // ��
			(otherPos.y + otherHalf.y) - (pos.y - half.y), // ��
			(pos.y + half.y) - (otherPos.y - otherHalf.y), // ��
			(otherPos.z + otherHalf.z) - (pos.z - half.z), // ��
			(pos.z + half.z) - (otherPos.z - otherHalf.z), // ��O
		};
		// ���ʂ̋������ŏ��ɂȂ��Ă���v�f��������
		int min = 0;
		for (int i = 0; i < 6; i++) {
			if (diff[i] < diff[min]) {
				min = i;
			}
		}
		// �ŏ��ɂȂ��Ă�������ɑ΂��ĉ����o�����s��
		switch (min) {
		case 0:
			pos.x += diff[min];
			break;
		case 1:
			pos.x -= diff[min];
			break;
		case 2:
			pos.y += diff[min];
			break;
		case 3:
			pos.y -= diff[min];
			break;
		case 4:
			pos.z += diff[min];
			break;
		case 5:
			pos.z -= diff[min];
			break;
		default:
			break;
		}
		trans->SetWorldPosition(pos);
	}

	void Player::DrawStrings()
	{
		// FPS�̎擾
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring strFps(L"FPS: ");
		strFps += Util::UintToWStr(fps);
		wstring str = strFps+state;
		//�����������
		auto ptrString = GetComponent<StringSprite>();
		ptrString->SetText(str);

	}

	//--------------------------------------------------------------------------------------
	//	class WalkState : public ObjState<Player>;
	//	�p�r: �������
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	shared_ptr<WalkState> WalkState::Instance() {
		static shared_ptr<WalkState> instance(new WalkState);
		return instance;
	}
	//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
	void WalkState::Enter(const shared_ptr<Player>& Obj) {
	}
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void WalkState::Execute(const shared_ptr<Player>& Obj) {
		Obj->Walk();
	}
	//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
	void WalkState::Exit(const shared_ptr<Player>& Obj) {
	}
	//--------------------------------------------------------------------------------------
	//	class WalkState : public ObjState<Player>;
	//	�p�r: �����N�����ł�����
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	shared_ptr<LinkState> LinkState::Instance() {
		static shared_ptr<LinkState> instance(new LinkState);
		return instance;
	}
	//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
	void LinkState::Enter(const shared_ptr<Player>& Obj) {
	}
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void LinkState::Execute(const shared_ptr<Player>& Obj) {
		Obj->LinkGo();
	}
	//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
	void LinkState::Exit(const shared_ptr<Player>& Obj) {
	}
}
//end basecross

