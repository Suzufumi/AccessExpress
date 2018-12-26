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
		//col->SetDrawActive(true);
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
		auto drawComp = AddComponent<PNTBoneModelDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		drawComp->SetMultiMeshResource(L"PLAYER_MODEL");
		drawComp->SetTextureResource(L"PLAYER_TX");
		// �ϊ������s�����
		drawComp->SetMeshToTransformMatrix(spanMat);
		// �A�j���[�V������ǉ�����
		drawComp->AddAnimation(L"Default", 0, 110, true, 60.0f);
		// �A�j���[�V�����̐ݒ�
		drawComp->ChangeCurrentAnimation(L"Default");
		//Col4 Color(1.0f, 0.2f, 1.0f, 0.7f);
		//drawComp->SetDiffuse(Color);
		//drawComp->SetColorAndAlpha(Color);
		// ���C���[�̒���
		SetDrawLayer(1);

		m_StateMachine.reset(new StateMachine<Player>(GetThis<Player>()));
		//�ŏ��̃X�e�[�g��DataState�ɐݒ�
		m_StateMachine->ChangeState(DataState::Instance());
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
		if (app->GetScene<Scene>()->GetGameStart()) {
			m_pad = GameManager::GetInstance().GetPad();
		}
		else {
			m_nowFallSpeed = 5.0f;
			Fall();
		}

		//���X�e�B�b�N�̓��͕��������߂�
		m_padDir = Vec3(m_pad.fThumbLX, 0.0f, m_pad.fThumbLY);
		m_padDir = m_padDir.normalize();

		//�X�e�[�g�}�V���̃A�b�v�f�[�g
		m_StateMachine->Update();
		// �A�j���[�V�������X�V����
		auto drawComp = GetComponent<PNTBoneModelDraw>();
		drawComp->UpdateAnimation(delta);
	};

	//--------------------------------------------------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------------------------------------------------
	void Player::OnUpdate2() {
		Vec3 pos = GetComponent<Transform>()->GetWorldPosition();
		// �ŏ��ɂȂ��Ă�������ɑ΂��ĉ����o�����s��
		switch (m_nestingMin) {
		case 0:
			pos.x += m_nesting;
			break;
		case 1:
			pos.x -= m_nesting;
			break;
		case 2:
			pos.y += m_nesting;
			break;
		case 3:
			pos.y -= m_nesting;
			break;
		case 4:
			pos.z += m_nesting;
			break;
		case 5:
			pos.z -= m_nesting;
			break;
		default:
			break;
		}
		GetComponent<Transform>()->SetWorldPosition(pos);
		m_nesting = NULL;
		SetJummer(false);

		// �f�o�b�O�����̕\��
		DrawStrings();
	}
	//--------------------------------------------------------------------------------------------------------------
	//�Փ˂����Ƃ�
	//--------------------------------------------------------------------------------------------------------------
	void Player::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		auto trans = GetComponent<Transform>();
		auto otherTrans = Other->GetComponent<Transform>();
		//player�̉��ʂƏՓ˂������̂̏�ʂ��������Ă�����
		if (((otherTrans->GetWorldPosition().y + otherTrans->GetScale().y / 2) - (trans->GetWorldPosition().y - trans->GetScale().y / 2)) < 0.5f) {
			//������Y�����ɗ͂������鏈�����s��Ȃ��悤�ɂ���
			m_isFall = false;
		}

		auto goal = dynamic_pointer_cast<Goal>(Other);
		if (goal){
			goal->ArriveGoal();
		}
		if (!App::GetApp()->GetScene<Scene>()->GetGameStart()) {
			App::GetApp()->GetScene<Scene>()->SetGameStart(true);
			m_nowFallSpeed = 8.0f;
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

	}
	//--------------------------------------------------------------------------------------------------------------
	//�Փ˂��������ꂽ�Ƃ�
	//-------------------------------------------------------------------------------------------------------------
	void Player::OnCollisionExit(shared_ptr<GameObject>& Other) {
		auto wall = dynamic_pointer_cast<Wall>(Other);
		if (wall) {
			auto trans = GetComponent<Transform>();
			auto wallTrans = wall->GetComponent<Transform>();
			//player�̉��ʂƏՓ˂������̂̏�ʂ��������Ă�����
			if (((wallTrans->GetWorldPosition().y + wallTrans->GetScale().y / 2) - (trans->GetWorldPosition().y - trans->GetScale().y / 2)) < 0.5f) {
				m_response = wallTrans->GetWorldPosition() + Vec3(0.0f, 5.0f, 0.0f);
				m_response.y += wallTrans->GetScale().y / 2;
			}
		}
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
			playerPos.x += m_nowWalkSpeed * m_forward.x * App::GetApp()->GetElapsedTime() * m_JummerSpeed;
			playerPos.z += m_nowWalkSpeed * m_forward.z * App::GetApp()->GetElapsedTime() * m_JummerSpeed;
		}
		playerTrans->SetWorldPosition(playerPos);
	}
	//---------------------------------------------------------------------------------------------
	//Y�����̈ړ�����
	//---------------------------------------------------------------------------------------------
	void Player::Fall() {
		if (m_isFall) {
			auto playerTrans = GetComponent<Transform>();
			auto playerPos = playerTrans->GetWorldPosition();
			playerPos.y += -m_nowFallSpeed * App::GetApp()->GetElapsedTime() * m_JummerSpeed;
			playerTrans->SetWorldPosition(playerPos);
		}
		m_isFall = true;

	}
	//---------------------------------------------------------------------------------------------
	//�v���C���[�������������ɉ�]������
	//---------------------------------------------------------------------------------------------
	void Player::PlayerRoll() {
		auto ptrUtil = GetBehavior<UtilBehavior>();
		ptrUtil->RotToHead(m_forward, 0.1f);
	}
	//--------------------------------------------------------------------------------------------
	//���s�ړ��̑��x���X�e�[�g�ŕ����ĕύX����
	//--------------------------------------------------------------------------------------------
	void Player::ChangeWalkSpeed(State state) {
		switch (state){
		case State::HUMAN:
			m_nowWalkSpeed = m_humanWalkSpeed;
			break;
		case State::DATA:
			m_nowWalkSpeed = m_dataWalkSpeed;
			break;
		default:
			break;
		}
	}
	//--------------------------------------------------------------------------------------------
	//�ݒ荂���ȉ��ɂȂ����烊�X�|�[���ʒu�Ƀ��[�v������
	//--------------------------------------------------------------------------------------------
	void Player::Response() {
		if (GetComponent<Transform>()->GetWorldPosition().y <= m_responseHeght) {
			GetComponent<Transform>()->SetWorldPosition(m_response);
			//���A��ɂ���������悤�ɏ����񕜂�����
			m_changeEnergy = 20.0f;
		}
	}
	//--------------------------------------------------------------------------------------------
	//�J�����̉�]����
	//--------------------------------------------------------------------------------------------
	void Player::CameraRoll() {
		float delta = App::GetApp()->GetElapsedTime();

		//�E�X�e�B�b�N�ɒl�����͂���Ă�����
		if (m_pad.fThumbRX > 0.2f || m_pad.fThumbRX < -0.2f ||
			m_pad.fThumbRY > 0.2f || m_pad.fThumbRY < -0.2f) {
			m_angleY += -m_pad.fThumbRX * m_maxAngleSpeed * delta; // �J��������]������
			m_angleX += -m_pad.fThumbRY * m_maxAngleSpeed * delta; // �J���������~������
			//Y����p�x�̊ۂ�(-360<360)
			if (m_angleY > Deg2Rad(360.0f)) {
				m_angleY = Deg2Rad(0.0f);
			}
			if (m_angleY < Deg2Rad(0.0f)) {
				m_angleY = Deg2Rad(360.0f);
			}
			//X����p�x�̊ۂ�(-60<60)
			if (m_angleX > Deg2Rad(60.0f)) {
				m_angleX = Deg2Rad(60.0f);
			}
			if (m_angleX < -Deg2Rad(60.0f)) {
				m_angleX = -Deg2Rad(60.0f);
			}
		}

		// �X�e�B�b�N�̌X�����p�x�ɕϊ�����
		float padRad = atan2f(m_padDir.z, m_padDir.x);
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
	//�J�����̃v���C���[�Ǐ]����
	//---------------------------------------------------------------------------------------------
	void Player::CameraControll(){
		auto pos = GetComponent<Transform>()->GetWorldPosition();
		auto camera = GetStage()->GetView()->GetTargetCamera();
		camera->SetAt(pos + Vec3(0.0f, 3.0f, 0.0f));
		auto eye = pos + Vec3(cos(m_angleY) * m_cameraDistance,
			sin(m_angleX) * m_cameraDistance, sin(m_angleY) * m_cameraDistance);
		camera->SetEye(eye);
	}
	//---------------------------------------------------------------------------------------------
	//L�{�^�����������Ƃ��Ƀ����N�I�u�W�F���Ə��̋߂��������炻����������
	//---------------------------------------------------------------------------------------------
	void Player::Rock(Vec3 origin, Vec3 originDir) {
		if (GameManager::GetInstance().GetPad().wPressedButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
			auto sightingDevice = m_SightingDevice.lock();
			//�����N�I�u�W�F�N�g�̓����Ă���O���[�v�������Ă���
			auto& linkGroup = GetStage()->GetSharedObjectGroup(L"Link");
			//������o��
			for (auto& link : linkGroup->GetGroupVector()) {
				auto linkObj = link.lock();
				auto linkTrans = linkObj->GetComponent<Transform>();
				//�����N�I�u�W�F�N�g��OBB�����
				OBB obb(linkTrans->GetScale() * 5, linkTrans->GetWorldMatrix());
				//�v���C���[����ł�Ray��OBB�Ŕ���
				bool hit = HitTest::SEGMENT_OBB(origin, origin + originDir * 50.0f, obb);
				if (hit) {
					float deltaX = origin.x - linkTrans->GetWorldPosition().x;
					float deltaZ = origin.z - linkTrans->GetWorldPosition().z;
					float deltaY = origin.y - linkTrans->GetWorldPosition().y;
					//���̃J�����ʒu�𐧌䂷��p�x
					m_angleY = atan2f(deltaZ, deltaX);
					if (m_angleY < 0.0f) {
						m_angleY += Deg2Rad(360.0f);
					}
					//�c�̃J�����ʒu�𐧌䂷��p�x
					static float syahen = hypotf(deltaX,deltaZ);
					m_angleX = atan2f(deltaY, syahen) + (3.0f/m_cameraDistance);
				}
			}
		}
	}
	//---------------------------------------------------------------------------------------------
	//�����N���烊���N�֔�ԏ���
	//---------------------------------------------------------------------------------------------
	void Player::LinkGo() {
		auto pos = GetComponent<Transform>()->GetWorldPosition();
		auto& gameManager = GameManager::GetInstance();
		//�v�Z�̂��߂̎��ԉ��Z
		auto addLerp = (App::GetApp()->GetElapsedTime() * m_JummerSpeed * 30.0f) / (m_BezierSpeedLeap);
		if (gameManager.GetOnSlow()) {
			//�X���[��ԂȂ̂�20����1�̍X�V���x�ɂ���
			m_Lerp += addLerp / gameManager.GetSlowSpeed();
		}
		else {
			m_Lerp += addLerp;
		}
		auto droneGroup = GetStage()->GetSharedObjectGroup(L"Drone");
		auto drone = dynamic_pointer_cast<Drone>(droneGroup->at(m_DroneNo));
		//���n���O�ŃX���[�ɂȂ��Ă��Ȃ�������
		if (m_Lerp >= 0.9f && gameManager.GetOnSlow() == false) {
			//�X���[�ɂ���
			gameManager.SetOnSlow(true);
		}
		//��ԏ������I�������
		if (m_Lerp >= 1.0f) {
			m_Lerp = 1.0f;
			//�X���[���I���
			gameManager.SetOnSlow(false);
			//�h���[���Ɍ������Ĕ��ł�����
			if (drone) {
				//���݃`�F�C�����h���[���̎��Ƀ`�F�C�����𒴂��Ă����ꍇ
				if (drone->GetDeadChain() <= GetChain()) {
					//�����Ȃ�����
					drone->Die();
				}
				//�������Ă���h���[���̏�����
				m_DroneNo = NULL;
			}
			//��яI�������X�e�[�g���f�[�^�̂ɂ���
			m_StateMachine->ChangeState(DataState::Instance());
		}
		//�h���[���Ɍ������Ă�����A�����Ă���̂ŏI�_�̈ʒu���X�V����
		if (m_DroneNo != NULL) {
			p2 = drone->GetComponent<Transform>()->GetWorldPosition();
		}
		//�x�W�G�Ȑ��̌v�Z
		pos = (1 - m_Lerp) * (1 - m_Lerp) * p0 + 2 * (1 - m_Lerp) * m_Lerp * p1 + m_Lerp * m_Lerp * p2;
		GetComponent<Transform>()->SetWorldPosition(pos);
		if (!gameManager.GetOnSlow()) {
			//�v���C���[�ƈꏏ�ɓ������߂Ƀv���C���[��Leap�ŃJ�����𓮂���
			auto camera = GetStage()->GetView()->GetTargetCamera();
			dynamic_pointer_cast<TpsCamera>(camera)->BezierMove(m_Lerp, pos);
		}
	}
	//---------------------------------------------------------------------------------------------
	//�x�W�G�Ȑ��̐���_�ݒ�
	//---------------------------------------------------------------------------------------------
	void Player::SetBezierPoint(Vec3 point) {
		p0 = GetComponent<Transform>()->GetWorldPosition();
		p2 = point + Vec3(0,1.0f,0);
		//��Ԑ�܂ł̋����ɉ����Ĕ�ԍۂ̃X�s�[�h��ς���
		m_BezierSpeedLeap = Vec3(p2 - p0).length();
		//��Ԑ�܂ł̋����ɉ����Ĕ�ԍۂ̕������̌`��ς���
		if (m_BezierSpeedLeap >= 20.0f) {
			p1 = point + Vec3(0, 10, 0);
		}
		else if(m_BezierSpeedLeap >= 10.0f){
			p1 = point + Vec3(0, 5, 0);
		}
		else if(m_BezierSpeedLeap < 10.0f){
			p1 = point - ((point - p0) / 2);
		}
		//��񂾍ۂɃ��X�|�[���ʒu�̍X�V���s��
		m_response = p2;
		//�J�����̒Ǐ]���铮����ݒ肷��
		auto camera = GetStage()->GetView()->GetTargetCamera();
		dynamic_pointer_cast<TpsCamera>(camera)->SetBezier(GetThis<Player>(), p2);
		//���̃����N�Ɉڂ�̂ŃX���[������
		GameManager::GetInstance().SetOnSlow(false);
		//�v���C���[�̐��ʌ�����ԕ����ɂ���
		m_forward = Vec3(p2 - p0).normalize();
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
	//Ray���΂�
	//---------------------------------------------------------------------------------------------
	void Player::RayShot() {
		auto sightingDevice = m_SightingDevice.lock();
		//�����N�I�u�W�F�N�g�ɓ������Ă���t���O��false�ɖ߂�
		sightingDevice->ResetCaptureLink();

		auto pos = sightingDevice->GetComponent<Transform>()->GetWorldPosition();
		auto m_cameraPos = GetStage()->GetView()->GetTargetCamera()->GetEye();
		//�Ə��ƃJ�����̈ʒu�����΂����������߂�
		auto dir = pos - m_cameraPos;
		dir = dir.normalize();
		//�����傫�߂̔�����Ƃ��āAL�������ē������Ă����ꍇ�����N�I�u�W�F�ɃJ����������
		Rock(m_cameraPos, dir);
		//�����N�I�u�W�F�N�g�Ƃ̔���
		LinkRayCheck(pos, dir);
		//�h���[���Ƃ̔���
		DroneRayCheck(pos, dir);
	}
	//---------------------------------------------------------------------------------------------
	//Ray��Link�I�u�W�F�N�g���������Ă��邩�𒲂ׂ�
	//---------------------------------------------------------------------------------------------
	void Player::LinkRayCheck(Vec3 origin,Vec3 originDir) {
		auto sightingDevice = m_SightingDevice.lock();
		//�����N�I�u�W�F�N�g�̓����Ă���O���[�v�������Ă���
		auto& linkGroup = GetStage()->GetSharedObjectGroup(L"Link");
		//������o��
		for (auto& link : linkGroup->GetGroupVector()) {
			auto linkObj = link.lock();
			auto linkTrans = linkObj->GetComponent<Transform>();
			//�����N�I�u�W�F�N�g��OBB�����
			OBB obb(linkTrans->GetScale() * 3, linkTrans->GetWorldMatrix());
			//�v���C���[����ł�Ray��OBB�Ŕ���
			bool hit = HitTest::SEGMENT_OBB(origin, origin + originDir * 30.0f, obb);
			//�Ō�Ƀx�W�G�Ȑ��Ŕ�񂾃����N�I�u�W�F�N�g����Ȃ����̂ɓ������Ă�����
			if (hit && p2 + Vec3(0, -1, 0) != linkTrans->GetWorldPosition()) {
				//�Ə��ɓ������Ă��邱�Ƃ�������
				sightingDevice->SetCaptureLink(true);
				if (m_pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
					Vec3 dir = GetComponent<Transform>()->GetWorldPosition() - linkTrans->GetWorldPosition();
					dir.y = 0;
					SetBezierPoint(linkTrans->GetWorldPosition() + dir.normalize());
					m_Lerp = 0;
					//�h���[�����������܂܂��Ƃ�����̂ق��Ɍ������Ă��܂��̂�NULL�ɂ���
					m_DroneNo = NULL;
					m_StateMachine->ChangeState(LinkState::Instance());
					break;
				}
			}
		}
	}
	//---------------------------------------------------------------------------------------------
	//Ray��Drone�I�u�W�F�N�g���������Ă��邩�𒲂ׂ�
	//---------------------------------------------------------------------------------------------
	void Player::DroneRayCheck(Vec3 origin, Vec3 originDir) {
		auto sightingDevice = m_SightingDevice.lock();
		//�h���[���I�u�W�F�N�g�̓����Ă���O���[�v�������Ă���
		auto& droneGroup = GetStage()->GetSharedObjectGroup(L"Drone");
		//���Ԗڂ�Drone��������
		int count = 0;
		//������o��
		for (auto& drone : droneGroup->GetGroupVector()) {
			auto droneObj = drone.lock();
			auto droneTrans = droneObj->GetComponent<Transform>();
			//�����N�I�u�W�F�N�g��OBB�����
			OBB obb(droneTrans->GetScale() * 3, droneTrans->GetWorldMatrix());
			//�v���C���[����ł�Ray��OBB�Ŕ���
			bool hit = HitTest::SEGMENT_OBB(origin, origin + originDir * 30.0f, obb);
			//�Ō�Ƀx�W�G�Ȑ��Ŕ�񂾃I�u�W�F�N�g����Ȃ����̂ɓ������Ă�����
			if (hit && p2 + Vec3(0, -1, 0) != droneTrans->GetWorldPosition()) {
				//�Ə��ɓ������Ă��邱�Ƃ�������
				sightingDevice->SetCaptureLink(true);
				if (m_pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
					SetBezierPoint(droneTrans->GetWorldPosition());
					m_DroneNo = count;
					m_Lerp = 0;
					m_StateMachine->ChangeState(LinkState::Instance());
					break;
				}
			}
			count++;
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
		if (m_nesting == NULL || m_nesting > diff[min]) {
			m_nesting = diff[min];
			m_nestingMin = min;
		}
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
		m_energy += nowChains * 2.0f;
	}

	//---------------------------------------------------------------------------------------------
	//���̕\��
	//---------------------------------------------------------------------------------------------
	void Player::DrawStrings(){
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
		wstring chainLimit(L"CHAIN_TIME : ");
		chainLimit += Util::IntToWStr(m_chainTime) + L"\n";
		wstring timeLimit(L"Limit : ");
		timeLimit += Util::IntToWStr(m_comboChainLimit) + L"\n";
		//�����������
		//wstring str = strFps + cameraStr + energy + combo + timeLimit;
		wstring AngleY(L"AngleY : ");
		AngleY += Util::FloatToWStr(m_angleY) + L"\n";
		wstring AngleX(L"AngleX : ");
		AngleX += Util::FloatToWStr(m_angleX) + L"\n";

		wstring str = combo + chainLimit + energy + timeLimit + AngleY + AngleX ;
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
		Obj->ChangeWalkSpeed(Player::State::HUMAN);
	}
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void WalkState::Execute(const shared_ptr<Player>& Obj) {
		Obj->Walk();
		Obj->Fall();
		Obj->Response();
		Obj->SightingDeviceChangePosition();
		Obj->CameraControll();
		//�E�X�e�B�b�N�̒l�ŃJ�����̉�]�������s��
		Obj->CameraRoll();
		if (Obj->CheckAButton()) {
			Obj->GetStateMachine()->ChangeState(DataState::Instance());
			Obj->SightingDeviceDrawActive(true);
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
		auto pos = Obj->GetComponent<Transform>()->GetWorldPosition();
		auto camera = Obj->GetStage()->GetView()->GetTargetCamera();
		camera->SetAt(pos + Vec3(0.0f, 1.0f, 0.0f));
		camera->SetEye(pos + Vec3(0.0f, 1.5f, -10.0f));
		
		// �R���{�����Z����
		Obj->AddCombo();
		Obj->ComboBonus(Obj->GetChain());
	}
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void LinkState::Execute(const shared_ptr<Player>& Obj) {
		//�X���[��ԂȂ�A�J�����ƃ��C���J������
		if (GameManager::GetInstance().GetOnSlow()) {
			Obj->RayShot();
			Obj->CameraControll();
			Obj->CameraRoll();
		}
		else {
			Obj->PlayerRoll();
		}
		Obj->LinkGo();
		Obj->SightingDeviceChangePosition();
		if (Obj->GetEnergy() <= 0.0f) {
			Obj->GetStateMachine()->ChangeState(WalkState::Instance());
			Obj->SightingDeviceDrawActive(false);
		}
	}
	//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
	void LinkState::Exit(const shared_ptr<Player>& Obj) {
	}
	//--------------------------------------------------------------------------------------
	//	class DataState : public ObjState<Player>;
	//	�p�r: �f�[�^�̏��
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	shared_ptr<DataState> DataState::Instance() {
		static shared_ptr<DataState> instance(new DataState);
		return instance;
	}
	//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
	void DataState::Enter(const shared_ptr<Player>& Obj) {
		if (Obj->GetChain() >= 4)
		{
			// ���݂̃`�F�C���ɉ����Đ������Ԃ�ݒ�
			Obj->SetComboChainLimit(Obj->GetFirstTime() / (Obj->GetChain() - 2));
		}
		else
		{
			Obj->SetComboChainLimit(Obj->GetFirstTime());
		}
		Obj->ChengeEnergyMai();
		Obj->ChangeWalkSpeed(Player::State::DATA);

		// �ȑO�̃X�e�[�g��LinkState��������
		if (Obj->GetStateMachine()->GetPreviousState() == LinkState::Instance())
		{
			// �^�C���`�F�b�J�[�t���O���I���ɂ���
			Obj->SetAdvanceTimeActive(true);
		}
	}
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void DataState::Execute(const shared_ptr<Player>& Obj) {
		// �R���{�Ԃ̎��Ԃ�i�߂邩�ǂ���
		if (Obj->GetAdvanceTimeActive())
		{
			// ���Ԃ����Z����
			Obj->AddChainTime();
		}
		// ���݂̎��Ԃ��擾
		int timeLimit = Obj->GetComboChainLimit();
		// �������ԂɒB������
		if (Obj->GetChainTimeLim() >= Obj->GetComboChainLimit())
		{
			// �R���{�̃��Z�b�g
			Obj->ResetCombo();
			// �������Ԃ����Z�b�g
			Obj->ResetTimeLim();
			Obj->SetAdvanceTimeActive(false);
		}
		Obj->Walk();
		Obj->RayShot();
		Obj->SightingDeviceChangePosition();
		//�J��������̂��߂̒l��ύX����
		Obj->CameraControll();
		//�E�X�e�B�b�N�̒l�ŃJ�����̉�]�������s��
		Obj->CameraRoll();
		//�v���C���[�̑̂̌�����ς���
		Obj->PlayerRoll();
		if (Obj->CheckAButton() || Obj->GetEnergy() <= 0.0f) {
			Obj->GetStateMachine()->ChangeState(WalkState::Instance());
			Obj->SightingDeviceDrawActive(false);
		}
	}
	//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
	void DataState::Exit(const shared_ptr<Player>& Obj) {
		Obj->ResetTimeLim();
	}

}
//end basecross

