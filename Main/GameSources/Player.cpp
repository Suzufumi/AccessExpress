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
		auto camera = GetStage()->GetView()->GetTargetCamera();
		m_tpsCamera = dynamic_pointer_cast<TpsCamera>(camera);
		m_angleX = m_tpsCamera.lock()->GetCameraAngleX();
		m_angleY = m_tpsCamera.lock()->GetCameraAngleY();
		m_maxAngleSpeed = m_tpsCamera.lock()->GetMaxAngleSpeed();
		m_cameraHeight = m_tpsCamera.lock()->GetCameraHeight();
		m_cameraDistance = m_tpsCamera.lock()->GetCameraDistance();

	}
	//-------------------------------------------------------------------------------------------------
	//������
	//-------------------------------------------------------------------------------------------------
	void Player::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();

		ptrTrans->SetWorldPosition(m_position);
		ptrTrans->SetQuaternion(m_quaternion);
		ptrTrans->SetScale(m_scale);

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
		ptrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));

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
		drawComp->SetMultiMeshResource(L"PLAYER_MODEL");
		drawComp->SetTextureResource(L"PLAYER_TX");
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
		m_energy += m_changeEnergy * 5.0f * App::GetApp()->GetElapsedTime();
		if (m_energy >= m_maxEnergy) {
			m_energy = m_maxEnergy;
		}
		
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
		
		auto goal = dynamic_pointer_cast<Goal>(Other);
		if (goal)
		{
			goal->ArriveGoal();
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
		}
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
			//�����ƈړ��X�s�[�h���|���Z����positon��ύX����
			playerPos.x += m_nowWalkSpeed * m_forward.x * App::GetApp()->GetElapsedTime();
			playerPos.z += m_nowWalkSpeed * m_forward.z * App::GetApp()->GetElapsedTime();
		}
		//���X�e�B�b�N�����͂���ĂȂ�������
		//else {
		//	//Velocity�l�����񂾂񏬂�������
		//	m_velocity.x = m_velocity.x * 0.9f;
		//	m_velocity.z = m_velocity.x * 0.9f;
		//}
		playerTrans->SetWorldPosition(playerPos);
	}
	//---------------------------------------------------------------------------------------------
	//Y�����̈ړ�����
	//---------------------------------------------------------------------------------------------
	void Player::Fall() {
		auto playerTrans = GetComponent<Transform>();
		auto playerPos = playerTrans->GetWorldPosition();
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

		m_angleY += -m_pad.fThumbRX * m_maxAngleSpeed * delta; // �J��������]������
		m_cameraHeight += -m_pad.fThumbRY * m_maxAngleSpeed * 1.5f * delta; // �J���������~������
		//360�x���z������0�ɂ���
		if (m_angleX > 360) {
			m_angleX = 0;
		}
		//0�x�����������Ȃ�����
		if (m_angleX < 0) {
			m_angleX = 360;
		}

		if (m_padDir.length() != 0.0f) {
			// �X�e�B�b�N�̊p�x�ɃJ�����̊p�x��������
			padRad += m_angleY + XM_PIDIV2;  // �J�����̊p�x�ɂX�O�x������
			// �p�x��XZ�����ɕ������ăx�N�g������蒼��
			m_padDir.x = cosf(padRad); // �V�����p�x�� X �����ɕ�������
			m_padDir.z = sinf(padRad); // �V�����p�x�� Z �����ɕ�������

			m_forward = m_padDir;
		}
	}
	//---------------------------------------------------------------------------------------------
	//�����N���烊���N�֔�ԏ���
	//---------------------------------------------------------------------------------------------
	void Player::LinkGo() {
		auto pos = GetComponent<Transform>()->GetWorldPosition();
		//�v�Z�̂��߂̎��ԉ��Z
		m_Lerp += App::GetApp()->GetElapsedTime();
		if (m_Lerp >= 1.0f) {
			m_Lerp = 1.0f;
			//��яI�������X�e�[�g���f�[�^�̂ɂ���
			m_StateMachine->ChangeState(DateState::Instance());
		}
		//�x�W�G�Ȑ��̌v�Z
		pos.x = (1 - m_Lerp) * (1 - m_Lerp) * p0.x + 2 * (1 - m_Lerp) * m_Lerp * p1.x + m_Lerp * m_Lerp * p2.x;
		pos.y = (1 - m_Lerp) * (1 - m_Lerp) * p0.y + 2 * (1 - m_Lerp) * m_Lerp * p1.y + m_Lerp * m_Lerp * p2.y;
		pos.z = (1 - m_Lerp) * (1 - m_Lerp) * p0.z + 2 * (1 - m_Lerp) * m_Lerp * p1.z + m_Lerp * m_Lerp * p2.z;
		GetComponent<Transform>()->SetWorldPosition(pos);
	}
	//�x�W�G�Ȑ��̐���_�ݒ�
	void Player::SetBezierPoint(Vec3 point) {
		p0 = GetComponent<Transform>()->GetWorldPosition();
		p1 = point + Vec3(0, 10, 0);
		p2 = point + Vec3(0,1,0);
	}
	//---------------------------------------------------------------------------------------------
	//�Ə��̈ʒu���J�����ƃv���C���[�̈ʒu���狁�ߕύX����
	//---------------------------------------------------------------------------------------------
	void Player::SightingDeviceChangePosition() {
		auto pos = GetComponent<Transform>()->GetWorldPosition();
		auto m_cameraPos = GetStage()->GetView()->GetTargetCamera()->GetEye();
		//player�ƃJ�����̈ʒu����Ə���������������߂�
		//player�̏�����̂ق��ɒu������player��position�ɒ���������
		auto dir = (pos + Vec3(0.0f, 5.0f, 0.0f)) - m_cameraPos;
		dir = dir.normalize();

		auto sightingDevice = m_SightingDevice.lock();
		//player�̓��ӂ�ɁA���Ȃ��悤�J��������̕������������Ēu��
		sightingDevice->GetComponent<Transform>()->SetWorldPosition((pos + Vec3(0.0f, 3.0f, 0.0f)) + (dir * 2.0f));

		Quat rot;
		rot.rotationRollPitchYawFromVector(Vec3(0.0f,atan2f(dir.x, dir.z), 0.0f));
		
		sightingDevice->GetComponent<Transform>()->SetQuaternion(rot);
	}
	//---------------------------------------------------------------------------------------------
	//Ray��Link�I�u�W�F�N�g���������Ă��邩�𒲂ׂ�
	//---------------------------------------------------------------------------------------------
	void Player::RayHitLink() {
		auto sightingDevice = m_SightingDevice.lock();
		//�����N�I�u�W�F�N�g�ɓ������Ă���t���O��false�ɖ߂�
		sightingDevice->ResetCaptureLink();

		auto pos = sightingDevice->GetComponent<Transform>()->GetWorldPosition();
		auto m_cameraPos = GetStage()->GetView()->GetTargetCamera()->GetEye();
		//player�ƃJ�����̈ʒu�����΂����������߂�
		auto dir = pos - m_cameraPos;
		dir = dir.normalize();
		//�����N�I�u�W�F�N�g�̓����Ă���O���[�v�������Ă���
		auto& linkGroup = GetStage()->GetSharedObjectGroup(L"Link");
		//������o��
		for (auto& link : linkGroup->GetGroupVector()) {
			auto linkObj = link.lock();
			auto linkTrans = linkObj->GetComponent<Transform>();
			//�����N�I�u�W�F�N�g��OBB�����
			OBB obb(linkTrans->GetScale() * 3, linkTrans->GetWorldMatrix());
			//�v���C���[����ł�Ray��OBB�Ŕ���
			bool hit = HitTest::SEGMENT_OBB(pos, pos + dir * 30.0f, obb);
			//�Ō�Ƀx�W�G�Ȑ��Ŕ�񂾃����N�I�u�W�F�N�g����Ȃ����̂ɓ������Ă�����
			if (hit && p2 + Vec3(0, -1, 0) != linkTrans->GetWorldPosition()) {
				//�Ə��ɓ������Ă��邱�Ƃ�������
				sightingDevice->SetCaptureLink(true);
				if (m_pad.wPressedButtons & XINPUT_GAMEPAD_B) {
					SetBezierPoint(linkTrans->GetWorldPosition());
					m_Lerp = 0;
					m_StateMachine->ChangeState(LinkState::Instance());
					break;
				}
			}
		}
	}
	//---------------------------------------------------------------------------------------------
	//�����o���̔���
	//---------------------------------------------------------------------------------------------
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

	void Player::CameraControll()
	{
		auto pos = GetComponent<Transform>()->GetWorldPosition();
		auto camera = GetStage()->GetView()->GetTargetCamera();
		camera->SetAt(pos + Vec3(0.0f, 1.0f, 0.0f));
		auto eye = pos + Vec3(cos(m_angleY) * m_cameraDistance,
			m_cameraHeight, sin(m_angleY) * m_cameraDistance);
		camera->SetEye(eye);
	}
	//---------------------------------------------------------------------------------------------
	//A�{�^���������ꂽ���ǂ�����Ԃ�
	//---------------------------------------------------------------------------------------------
	bool Player::CheckAButton() {
		if (m_pad.wPressedButtons & XINPUT_GAMEPAD_A) {
			return true;
		}
		return false;
	}

	//---------------------------------------------------------------------------------------------
	//�R���{���ɉ����ă{�[�i�X��^����
	//---------------------------------------------------------------------------------------------
	void Player::ComboBonus(int nowChains)
	{
		m_energy += nowChains * 5.0f;
	}

	//---------------------------------------------------------------------------------------------
	//���̕\��
	//---------------------------------------------------------------------------------------------
	void Player::DrawStrings()
	{
		// FPS�̎擾
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring strFps(L"FPS: ");
		strFps += Util::UintToWStr(fps);
		strFps += L"\n";
		auto cameraPos = GetStage()->GetView()->GetTargetCamera();
		wstring cameraStr(L"Camera :");
		cameraStr += L"X:" + Util::FloatToWStr(cameraPos->GetEye().x, 6, Util::FloatModify::Fixed) + L"\t";
		cameraStr += L"Y:" + Util::FloatToWStr(cameraPos->GetEye().y, 6, Util::FloatModify::Fixed) + L"\t";
		cameraStr += L"Z:" + Util::FloatToWStr(cameraPos->GetEye().z, 6, Util::FloatModify::Fixed) + L"\n";
		cameraStr += L"X:" + Util::FloatToWStr(cameraPos->GetAt().x, 6, Util::FloatModify::Fixed) + L"\t";
		cameraStr += L"Y:" + Util::FloatToWStr(cameraPos->GetAt().y, 6, Util::FloatModify::Fixed) + L"\t";
		cameraStr += L"Z:" + Util::FloatToWStr(cameraPos->GetAt().z, 6, Util::FloatModify::Fixed) + L"\n";
		wstring energy(L"Energy : ");
		energy += Util::FloatToWStr(m_energy) + L"\n";
		wstring combo(L"Combo : ");
		combo += Util::IntToWStr(m_chain) + L"\n";
		wstring timeLimit(L"Limit : ");
		timeLimit += Util::IntToWStr(m_chainTimeLimit) + L"\n";
		//�����������
		wstring str = strFps + cameraStr + energy + combo + timeLimit;
		//wstring str = energy;
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
		Obj->ChengeEnergyPur();
	}
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void WalkState::Execute(const shared_ptr<Player>& Obj) {
		Obj->Walk();
		Obj->Fall();
		Obj->CameraControll();
		Obj->SightingDeviceChangePosition();
		if (Obj->CheckAButton()) {
			Obj->GetStateMachine()->ChangeState(DateState::Instance());
			Obj->SightingDeviceDrawActiveFlag(true);
		}
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

	const int CHAIN_TIMELIMIT = 180;	// �R���{���r�؂�鎞��

	//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
	void LinkState::Enter(const shared_ptr<Player>& Obj) {
		//auto pos = Obj->GetComponent<Transform>()->GetWorldPosition();
		//auto camera = Obj->GetStage()->GetView()->GetTargetCamera();
		//camera->SetAt(pos + Vec3(0.0f, 1.0f, 0.0f));
		//camera->SetEye(pos + Vec3(0.0f, 1.5f, -10.0f));
		
		// �R���{�����Z����
		Obj->AddCombo();
		Obj->ComboBonus(Obj->GetChain());
	}
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void LinkState::Execute(const shared_ptr<Player>& Obj) {
		Obj->LinkGo();
		Obj->CameraControll();
		Obj->SightingDeviceChangePosition();
		if (Obj->GetEnergy() <= 0.0f) {
			Obj->GetStateMachine()->ChangeState(WalkState::Instance());
			Obj->SightingDeviceDrawActiveFlag(false);
		}
	}
	//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
	void LinkState::Exit(const shared_ptr<Player>& Obj) {
	}
	//--------------------------------------------------------------------------------------
	//	class DateState : public ObjState<Player>;
	//	�p�r: �f�[�^�̏��
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	shared_ptr<DateState> DateState::Instance() {
		static shared_ptr<DateState> instance(new DateState);
		return instance;
	}
	//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
	void DateState::Enter(const shared_ptr<Player>& Obj) {
		Obj->ChengeEnergyMai();
		// �ȑO�̃X�e�[�g��LinkState��������
		if (Obj->GetStateMachine()->GetPreviousState() == LinkState::Instance())
		{
			// �^�C���`�F�b�J�[�t���O���I���ɂ���
			Obj->SetAdvanceTimeActive(true);
		}
	}
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void DateState::Execute(const shared_ptr<Player>& Obj) {
		// �R���{�Ԃ̎��Ԃ�i�߂邩�ǂ���
		if (Obj->GetAdvanceTimeActive())
		{
			// ���Ԃ����Z����
			Obj->AddChainTimeLimit();
		}
		// ���݂̎��Ԃ��擾
		int timeLimit = Obj->GetChainTimeLim();
		// �������ԂɒB������
		if (timeLimit >= CHAIN_TIMELIMIT)
		{
			// �R���{�̃��Z�b�g
			Obj->ResetCombo();
			// �������Ԃ����Z�b�g
			Obj->ResetTimeLim();
			Obj->SetAdvanceTimeActive(false);
		}
		Obj->Walk();
		Obj->RayHitLink();
		Obj->CameraControll();
		Obj->SightingDeviceChangePosition();
		if (Obj->CheckAButton() || Obj->GetEnergy() <= 0.0f) {
			Obj->GetStateMachine()->ChangeState(WalkState::Instance());
			Obj->SightingDeviceDrawActiveFlag(false);
		}
	}
	//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
	void DateState::Exit(const shared_ptr<Player>& Obj) {
		Obj->ResetTimeLim();
	}

}
//end basecross

