/*!
@file Player.cpp
@brief �v���C���[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"


namespace basecross {
	///-------------------------------------------------------------------------------
	//player�̃R���X�g���N�^
	///-------------------------------------------------------------------------------
	Player::Player(const shared_ptr<Stage>& StagePtr, IXMLDOMNodePtr pNode)
		:GameObject(StagePtr)
	{
		auto posStr = XmlDocReader::GetAttribute(pNode, L"Pos");
		auto rotStr = XmlDocReader::GetAttribute(pNode, L"Quat");
		auto scaleStr = XmlDocReader::GetAttribute(pNode, L"Scale");

		auto pos = MyUtil::unityVec3StrToBCVec3(posStr);
		auto quat = MyUtil::unityQuatStrToBCQuat(rotStr);
		auto scale = MyUtil::unityVec3StrToBCVec3(scaleStr);

		m_position = pos;
		m_quaternion = quat;
		m_scale = scale;

		auto camera = GetStage()->GetView()->GetTargetCamera();
		m_tpsCamera = dynamic_pointer_cast<TpsCamera>(camera);
		//m_cameraDistance = m_tpsCamera.lock()->GetCameraDistance();
		m_cameraLookUp = m_tpsCamera.lock()->GetCameraLookUp();
	}
	///-------------------------------------------------------------------------------
	//������
	///-------------------------------------------------------------------------------
	void Player::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();

		ptrTrans->SetWorldPosition(m_position);
		ptrTrans->SetQuaternion(m_quaternion);
		ptrTrans->SetScale(m_scale);

		//�l�p�`�̓����蔻����Z�b�g
		auto col = AddComponent<CollisionObb>();
		//�R���W�����̔�������Ȃ�
		col->SetAfterCollision(AfterCollision::None);
		//player�Ɏ������Ďg�����̂ɂ͏Փ˂��Ȃ�
		col->AddExcludeCollisionTag(L"PlayerUse");

		Mat4x4 spanMat; // ���f���ƃg�����X�t�H�[���̊Ԃ̍����s��
		spanMat.affineTransformation(
			Vec3(0.8f, 0.8f, 0.8f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, Deg2Rad(180), 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		//�`��R���|�[�l���g�̒ǉ�
		auto drawComp = AddComponent<PNTBoneModelDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		drawComp->SetMultiMeshResource(L"PLAYER_MODEL");
		drawComp->SetTextureResource(L"PLAYER_TX");
		drawComp->SetMultiMeshIsDraw(0, false);
		drawComp->SetMultiMeshIsDraw(1, false);
		drawComp->SetMultiMeshIsDraw(2, false);
		drawComp->SetMultiMeshIsDraw(FaceState::Smile, false);
		drawComp->SetMultiMeshIsDraw(FaceState::EyeClose, false);
		drawComp->SetMultiMeshIsDraw(FaceState::Regret, false);
		// �ϊ������s�����
		drawComp->SetMeshToTransformMatrix(spanMat);
		// �A�j���[�V������ǉ�����
		drawComp->AddAnimation(L"Default", 0, 110, true, 60.0f);
		drawComp->AddAnimation(L"Move", 200, 30, true, 30.0f);
		drawComp->AddAnimation(L"Fly", 310, 30, false, 17.0f);
		drawComp->AddAnimation(L"Over", 350, 40, true, 30.0f);
		drawComp->AddAnimation(L"Clear", 400, 100, false, 30.0f);
		// ���C���[�̒���
		SetDrawLayer(-10);

		GetStage()->SetSharedGameObject(L"Player", GetThis<Player>());
		auto dev = GetStage()->AddGameObject<SightingDevice>();
		this->SetSightingDevice(dev);

		m_StateMachine.reset(new StateMachine<Player>(GetThis<Player>()));
		//�ŏ��̃X�e�[�g��DataState�ɐݒ�
		m_StateMachine->ChangeState(DataState::Instance());

		m_animStateMachine.reset(new StateMachine<Player>(GetThis<Player>()));
		m_animStateMachine->ChangeState(PlayerDefaultAnim::Instance());
	}
	///-------------------------------------------------------------------------------
	//Update
	///-------------------------------------------------------------------------------
	void Player::OnUpdate() {
		// �A�v���P�[�V�����擾
		auto& app = App::GetApp();
		// 1�t���[���̎��s�ɂ����������Ԃ��擾
		float delta = app->GetElapsedTime();

		if (GameManager::GetInstance().GetGameStart()) {
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

		m_animStateMachine->Update();

		// �A�j���[�V�������X�V����
		auto drawComp = GetComponent<PNTBoneModelDraw>();
		drawComp->UpdateAnimation(delta);
	};

	//--------------------------------------------------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------------------------------------------------
	void Player::OnUpdate2() {
		Extrusion();
	}
	///-------------------------------------------------------------------------------
	//�Փ˂����Ƃ�
	///-------------------------------------------------------------------------------
	void Player::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		auto trans = GetComponent<Transform>();
		auto otherTrans = Other->GetComponent<Transform>();
		//player�̉��ʂƏՓ˂������̂̏�ʂ��������Ă�����
		if (((otherTrans->GetWorldPosition().y + otherTrans->GetScale().y / 2) - (trans->GetWorldPosition().y - trans->GetScale().y / 2)) < 0.5f) {
			//������Y�����ɗ͂������鏈�����s��Ȃ��悤�ɂ���
			m_isFall = false;
		}

		//�ŏ��̂���Ă��鉉�o�A���n������Q�[�����J�n���đ���ł���悤�ɂ���
		if (!GameManager::GetInstance().GetGameStart()) {
			GameManager::GetInstance().SetGameStart(true);
		}
	}
	///-------------------------------------------------------------------------------
	//�Փ˂������Ă���Ƃ�
	///-------------------------------------------------------------------------------
	void Player::OnCollisionExcute(shared_ptr<GameObject>& Other) {
		auto trans = GetComponent<Transform>();
		auto otherTrans = Other->GetComponent<Transform>();
		//player�̉��ʂƏՓ˂������̂̏�ʂ��������Ă�����
		if (((otherTrans->GetWorldPosition().y + otherTrans->GetScale().y / 2) - (trans->GetWorldPosition().y - trans->GetScale().y / 2)) < 0.5f) {
			//������Y�����ɗ͂������鏈�����s��Ȃ��悤�ɂ���
			m_isFall = false;
		}
		ExtrusionJudgment(Other);

	}
	///-------------------------------------------------------------------------------
	//�p�x�̌������l�������X�e�B�b�N�̊p�x���Z�o����
	///-------------------------------------------------------------------------------
	void Player::Forword() {
		auto camera = GetStage()->GetView()->GetTargetCamera();
		auto tpsCamera = dynamic_pointer_cast<TpsCamera>(camera);
		// �X�e�B�b�N�̌X�����p�x�ɕϊ�����
		float padRad = atan2f(m_padDir.z, m_padDir.x);
		if (m_padDir.length() != 0.0f) {
			// �X�e�B�b�N�̊p�x�ɃJ�����̊p�x��������
			padRad += tpsCamera->GetCameraAngleY() + XM_PIDIV2;  // �J�����̊p�x�ɂX�O�x������
			// �p�x��XZ�����ɕ������ăx�N�g������蒼��
			m_padDir.x = cosf(padRad); // �V�����p�x�� X �����ɕ�������
			m_padDir.z = sinf(padRad); // �V�����p�x�� Z �����ɕ�������

			m_forward = m_padDir.normalize();
		}
	}

	///-------------------------------------------------------------------------------
	//Y�����̈ړ�����
	///-------------------------------------------------------------------------------
	void Player::Fall() {
		if (m_isFall) {
			auto playerTrans = GetComponent<Transform>();
			auto playerPos = playerTrans->GetWorldPosition();
			playerPos.y += -m_nowFallSpeed * App::GetApp()->GetElapsedTime();
			playerTrans->SetWorldPosition(playerPos);
		}
		m_isFall = true;

	}
	///-------------------------------------------------------------------------------
	//�v���C���[�������������ɉ�]������
	///-------------------------------------------------------------------------------
	void Player::PlayerRoll() {
		auto ptrUtil = GetBehavior<UtilBehavior>();
		ptrUtil->RotToHead(m_forward, 0.1f);
	}
	///-------------------------------------------------------------------------------
	//�x�W�G�Ȑ��̐���_�ݒ�
	///-------------------------------------------------------------------------------
	void Player::SetBezierPoint(Vec3 point) {

		p0 = GetComponent<Transform>()->GetWorldPosition();
		p2 = point + Vec3(0, 1.0f, 0);
		//��Ԑ�܂ł̋����ɉ����Ĕ�ԍۂ̃X�s�[�h��ς���
		m_BezierSpeedLeap = Vec3(p2 - p0).length();
		//��Ԑ�܂ł̋����ɉ����Ĕ�ԍۂ̕������̌`��ς���
		if (m_BezierSpeedLeap >= 20.0f) {
			p1 = point + Vec3(0, 6, 0);
		}
		else if (m_BezierSpeedLeap >= 10.0f) {
			p1 = point + Vec3(0, 3, 0);
		}
		else if (m_BezierSpeedLeap < 10.0f) {
			p1 = point - ((point - p0) / 2);
		}
		//�J�����̒Ǐ]���铮����ݒ肷��
		auto camera = GetStage()->GetView()->GetTargetCamera();
		dynamic_pointer_cast<TpsCamera>(camera)->SetBezier(GetThis<Player>(), p2);
		//���̃����N�Ɉڂ�̂ŃX���[������
		GameManager::GetInstance().SetOnSlow(false);
		//�v���C���[�̐��ʌ�����ԕ����ɂ���
		m_forward = Vec3(p2 - p0).normalize();
		//�����N�I�u�W�F�̃��b�N�I��������
		m_islockon = false;
	}
	///-------------------------------------------------------------------------------
	//�Ə��̈ʒu���J�����ƃv���C���[�̈ʒu���狁�ߕύX����
	///-------------------------------------------------------------------------------
	void Player::SightingDeviceChangePosition() {
		auto pos = GetComponent<Transform>()->GetWorldPosition();
		auto m_cameraPos = GetStage()->GetView()->GetTargetCamera()->GetEye();
		//player�ƃJ�����̈ʒu����Ə���������������߂�
		//player�̏�����̂ق��ɒu������player��position�ɒ���������
		auto dir = (pos + Vec3(0.0f, 5.0f, 0.0f)) - m_cameraPos;
		dir = dir.normalize();

		auto sightingDevice = m_SightingDevice.lock();
		//player�̓��ӂ�ɁA���Ȃ��悤�J��������̕������������Ēu��
		sightingDevice->GetComponent<Transform>()->SetWorldPosition((pos + Vec3(0.0f, m_cameraLookUp, 0.0f)));

		float syahen = hypotf(dir.x, dir.z);
		Quat rot;
		rot.rotationRollPitchYawFromVector(Vec3(-atan2f(dir.y, syahen), atan2f(dir.x, dir.z), 0.0f));

		sightingDevice->GetComponent<Transform>()->SetQuaternion(rot);
	}
	///-------------------------------------------------------------------------------
	//Ray���΂�
	///-------------------------------------------------------------------------------
	void Player::RayShot() {
		auto sightingDevice = m_SightingDevice.lock();
		//Ray�����������t���O��false�ɖ߂�
		sightingDevice->ResetCaptureLink();
		//Ray�̎˒���ݒ�A�R���{���ŐL�т�
		m_rayRange = m_rayRangeDefolt + (m_chain * 0.5f);

		auto sightPos = sightingDevice->GetComponent<Transform>()->GetWorldPosition();
		auto m_cameraPos = GetStage()->GetView()->GetTargetCamera()->GetEye();
		//�Ə��ƃJ�����̈ʒu�����΂����������߂�
		auto dir = sightPos - m_cameraPos;
		dir = dir.normalize();
		//���b�N�I���̏���
		RockonObject(sightPos, dir, L"Drone", 1.0f);
		RockonObject(sightPos, dir, L"Link", 4.0f);
		RockonObject(sightPos, dir, L"Mails", 4.0f);
		//���b�N�I���Ώۂ�����
		RockOn();
		//RB�{�^����������Ă������ׂ邩�̔�����s��
		if (m_pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			//�����N�I�u�W�F�N�g�Ƃ̔���
			LinkRayCheck(sightPos, dir);
			//���[���Ƃ̔���
			MailRayCheck(sightPos, dir);
			//�h���[���Ƃ̔���
			DroneRayCheck(sightPos, dir);
		}
	}
	///-------------------------------------------------------------------------------
	//Ray��OBB�̔���
	///-------------------------------------------------------------------------------
	bool Player::RayOBBHit(Vec3 origin, Vec3 originDir, shared_ptr<GameObject> obj) {
		auto trans = obj->GetComponent<Transform>();
		//�I�u�W�F�N�g��OBB�����
		OBB obb(trans->GetScale() * 3.0f, trans->GetWorldMatrix());
		//�v���C���[����ł�Ray��OBB�Ŕ���
		bool hit = HitTest::SEGMENT_OBB(origin, origin + originDir * m_rayRange, obb);
		//Ray�͈͓̔��ɂ���I�u�W�F�N�g��������
		if ((origin - trans->GetWorldPosition()).length() <= m_rayRange + 1.0f) {
			//1�O�Ƀx�W�G�Ȑ��Ŕ�񂾃����N�I�u�W�F�N�g����Ȃ����̂ɓ������Ă�����
			if (hit && p2 + Vec3(0, -1, 0) != trans->GetWorldPosition()) {
				//����͔�ׂ����
				return true;
			}
		}
		return false;
	}
	///-------------------------------------------------------------------------------
	//Ray��Link�I�u�W�F�N�g���������Ă��邩�𒲂ׂ�
	///-------------------------------------------------------------------------------
	void Player::LinkRayCheck(Vec3 origin, Vec3 originDir) {
		//���łɑ��̂��̂ɔ��ł�����
		if (m_isGoLink) {
			return;
		}
		//�����N�I�u�W�F�N�g�̓����Ă���O���[�v�������Ă���
		auto& linkGroup = GetStage()->GetSharedObjectGroup(L"Link");
		//������o��
		for (auto& link : linkGroup->GetGroupVector()) {
			auto linkObj = link.lock();
			//���C�ƍ����Ă���I�u�W�F�N�g���������Ă��邩����
			if (RayOBBHit(origin, originDir, linkObj)) {
				auto sightingDevice = m_SightingDevice.lock();
				//�Ə��ɓ������Ă��邱�Ƃ�������
				sightingDevice->SetCaptureLink(true);
				Vec3 dir = GetComponent<Transform>()->GetWorldPosition() - linkObj->GetComponent<Transform>()->GetWorldPosition();
				dir.y = 0;
				//�I�u�W�F�N�g�ɔ��Ȃ��悤�ɕ������������Ă��炵���l��n��
				SetBezierPoint(linkObj->GetComponent<Transform>()->GetWorldPosition() + dir.normalize());
				//�o�߂����Z�b�g
				m_Lerp = 0;
				//��Ԃ̊m��
				m_isGoLink = true;
				//�X���[�̌o�ߎ��Ԃ����Z�b�g
				GameManager::GetInstance().ResetSloawPassage();
				//�h���[�����������܂܂��Ƃ�����̂ق��Ɍ������Ă��܂��̂�NULL�ɂ���
				m_DroneNo = NULL;
				m_StateMachine->ChangeState(LinkState::Instance());
				m_target = Target::LINK;
				return;
			}
		}
	}
	///-------------------------------------------------------------------------------
	//Ray��Drone�I�u�W�F�N�g���������Ă��邩�𒲂ׂ�
	///-------------------------------------------------------------------------------
	void Player::DroneRayCheck(Vec3 origin, Vec3 originDir) {
		//���łɑ��̂��̂ɔ��ł�����
		if (m_isGoLink) {
			return;
		}
		//�h���[���I�u�W�F�N�g�̓����Ă���O���[�v�������Ă���
		auto& droneGroup = GetStage()->GetSharedObjectGroup(L"Drone");
		//���Ԗڂ�Drone��������
		int count = 0;
		//������o��
		for (auto& drone : droneGroup->GetGroupVector()) {
			auto droneObj = drone.lock();
			if (RayOBBHit(origin, originDir, droneObj)) {
				auto sightingDevice = m_SightingDevice.lock();
				//�Ə��ɓ������Ă��邱�Ƃ�������
				sightingDevice->SetCaptureLink(true);
				SetBezierPoint(droneObj->GetComponent<Transform>()->GetWorldPosition());
				m_DroneNo = count;
				//�o�߂����Z�b�g
				m_Lerp = 0;
				//��Ԃ̊m��
				m_isGoLink = true;
				//�X���[�̌o�ߎ��Ԃ����Z�b�g
				GameManager::GetInstance().ResetSloawPassage();
				m_StateMachine->ChangeState(LinkState::Instance());
				m_target = Target::DRONE;
				return;
			}
			count++;
		}
	}
	///-------------------------------------------------------------------------------
	//Ray��Mail�I�u�W�F�N�g���������Ă��邩�𒲂ׂ�
	///-------------------------------------------------------------------------------
	void Player::MailRayCheck(Vec3 origin, Vec3 originDir) {
		//���łɑ��̂��̂ɔ��ł�����
		if (m_isGoLink) {
			return;
		}
		auto& mailGroup = GetStage()->GetSharedObjectGroup(L"Mails");
		int count = 0;
		for (auto& mail : mailGroup->GetGroupVector()) {
			auto mailObj = mail.lock();
			//�����Ă��郁�[�����擾����Ă��Ȃ����[�����m�F
			if (dynamic_pointer_cast<MailObject>(mailObj)->GetIsArrive() != true) {
				if (RayOBBHit(origin, originDir, mailObj)) {
					auto sightingDevice = m_SightingDevice.lock();
					//�Ə��ɓ������Ă��邱�Ƃ�������
					sightingDevice->SetCaptureLink(true);
					if (m_pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
						Vec3 dir = GetComponent<Transform>()->GetWorldPosition() - mailObj->GetComponent<Transform>()->GetWorldPosition();
						dir.y = 0;
						//�I�u�W�F�N�g�ɔ��Ȃ��悤�ɕ������������Ă��炵���l��n��
						SetBezierPoint(mailObj->GetComponent<Transform>()->GetWorldPosition() + dir.normalize());
						//���Ԃ̃��[���ɃA�N�Z�X���Ă��邩�ۑ�
						m_MailNum = count;
						m_Lerp = 0;
						//��Ԃ̊m��
						m_isGoLink = true;
						//�h���[�����������܂܂��Ƃ�����̂ق��Ɍ������Ă��܂��̂�NULL�ɂ���
						m_DroneNo = NULL;
						m_StateMachine->ChangeState(LinkState::Instance());
						m_target = Target::MAIL;
						return;
					}
				}
			}
			count++;
		}
	}
	///-------------------------------------------------------------------------------
	//�����N�֔�ԏ���
	///-------------------------------------------------------------------------------
	void Player::LinkGo() {
		auto pos = GetComponent<Transform>()->GetWorldPosition();
		auto& gameManager = GameManager::GetInstance();
		//�v�Z�̂��߂̎��ԉ��Z
		//�ړ��̌o�߂Ƀ`�F�C�����ɂ����������ꂽ
		auto addLerp = (App::GetApp()->GetElapsedTime() * (m_BezierSpeed + m_chain)) / (m_BezierSpeedLeap);
		//�X���[��Ԃ��ǂ����ŏ���
		if (gameManager.GetOnSlow()) {
			//20����1�̍X�V���x�ɂ���
			m_Lerp += addLerp / gameManager.GetSlowSpeed();
			//�}�l�[�W���[�ɃX���[�̌o�߂�`����
			gameManager.AddSlowPassage((addLerp / gameManager.GetSlowSpeed()) * 10.0f);
		}
		else {
			m_Lerp += addLerp;
		}
		//���n���O�ŃX���[�ɂȂ��Ă��Ȃ�
		if (m_Lerp >= 0.9f && gameManager.GetOnSlow() == false) {
			//�X���[�ɂ���
			gameManager.SetOnSlow(true);
			if (m_chain % BONUS_CHAIN == 0)
			{
				auto ptrEffect = GetStage()->GetSharedGameObject<BonusEffect>(L"BonusEffect", false);
				if (ptrEffect) {
					auto playerPos = GetComponent<Transform>()->GetWorldPosition();
					ptrEffect->InsertBounusEffect(Vec3(playerPos.x, playerPos.y - 3.0f, playerPos.z), Vec2(3.0f, 3.0f));
				}
			}
		}
		//��ԏ������I�������
		if (gameManager.GetSlowPassage() >= 1.0f) {
			m_Lerp = 1.0f;
			//�X���[���I���
			gameManager.SetOnSlow(false);
			//�X�R�A�𑝂₷
			gameManager.AddScore(GetChain() * 20 + GetChain() * 10);
			//���̃����N�֔�ׂȂ������̂Ń`�F�C�����Z�b�g����
			ChainFailure();
			//��яI�������X�e�[�g���f�[�^�̂ɂ���
			m_StateMachine->ChangeState(DataState::Instance());
		}
		//�x�W�G�Ȑ��̌v�Z
		pos = (1 - m_Lerp) * (1 - m_Lerp) * p0 + 2 * (1 - m_Lerp) * m_Lerp * p1 + m_Lerp * m_Lerp * p2;
		GetComponent<Transform>()->SetWorldPosition(pos);
		if (!gameManager.GetOnSlow()) {
			////�v���C���[�ƈꏏ�ɓ������߂Ƀv���C���[��Leap�ŃJ�����𓮂���
			//auto camera = GetStage()->GetView()->GetTargetCamera();
			//dynamic_pointer_cast<TpsCamera>(camera)->BezierMove(m_Lerp, pos);
		}
	}
	///-------------------------------------------------------------------------------
	//�h���[���֔�ԏ���
	///-------------------------------------------------------------------------------
	void Player::DroneGo() {
		auto& gameManager = GameManager::GetInstance();
		auto pos = GetComponent<Transform>()->GetWorldPosition();
		auto droneGroup = GetStage()->GetSharedObjectGroup(L"Drone");
		auto drone = dynamic_pointer_cast<Drone>(droneGroup->at(m_DroneNo));
		//�v�Z�̂��߂̎��ԉ��Z
		//�ړ��̌o�߂Ƀ`�F�C�����ɂ����������ꂽ
		auto addLerp = (App::GetApp()->GetElapsedTime() * (m_BezierSpeed + m_chain)) / m_BezierSpeedLeap;
		//�o�߂ɉ��Z
		m_Lerp += addLerp;
		//�����Ă���̂ŏI�_�̈ʒu���X�V����
		p2 = drone->GetComponent<Transform>()->GetWorldPosition();
		//��ԏ������I�����
		if (m_Lerp >= 1.0f && m_DroneNo != NULL) {
			m_Lerp = 1.0f;
			//�R���{���Z�b�g����
			ChainFailure();
			//��ԑΏۂ������Ԃ̃I�t
			ResetGoLink();
			//�������Ă���h���[���̏�����
			m_DroneNo = NULL;
			//��яI�������X�e�[�g���f�[�^�̂ɂ���
			m_StateMachine->ChangeState(DataState::Instance());
		}
		//�x�W�G�Ȑ��̌v�Z
		pos = (1 - m_Lerp) * (1 - m_Lerp) * p0 + 2 * (1 - m_Lerp) * m_Lerp * p1 + m_Lerp * m_Lerp * p2;
		GetComponent<Transform>()->SetWorldPosition(pos);
		////�J������ǔ�������
		//auto camera = GetStage()->GetView()->GetTargetCamera();
		//dynamic_pointer_cast<TpsCamera>(camera)->BezierMove(m_Lerp, pos);
	}
	///-------------------------------------------------------------------------------
	//���[���֔�ԏ���
	///-------------------------------------------------------------------------------
	void Player::MailGo() {
		auto& gameManager = GameManager::GetInstance();
		auto pos = GetComponent<Transform>()->GetWorldPosition();
		auto mailGroup = GetStage()->GetSharedObjectGroup(L"Mails");
		auto mail = dynamic_pointer_cast<MailObject>(mailGroup->at(m_MailNum));
		//�v�Z�̂��߂̎��ԉ��Z
		//�ړ��̌o�߂Ƀ`�F�C�����ɂ����������ꂽ
		auto addLerp = (App::GetApp()->GetElapsedTime() * (m_BezierSpeed + m_chain)) / (m_BezierSpeedLeap);
		//�X���[��Ԃ��ǂ����ŏ���
		if (gameManager.GetOnSlow()) {
			//�}�l�[�W���[�ɃX���[�̌o�߂�`����
			gameManager.AddSlowPassage((addLerp * 10 / gameManager.GetSlowSpeed()));
			if (gameManager.GetSlowPassage() >= 1.0f) {
				//�X���[���I���
				gameManager.SetOnSlow(false);
				//���̖ڕW�֔�ׂȂ������̂Ń`�F�C�����Z�b�g
				ChainFailure();
				//�X���[���Ԃ��I���������߃X�e�[�g���f�[�^�̂ɂ���
				m_StateMachine->ChangeState(DataState::Instance());
			}
		}
		else {
			//�o�߂ɉ��Z
			m_Lerp += addLerp;
		}
		//��ԏ������I��������ƂŁA�X���[��ԂłȂ����
		if (m_Lerp >= 1.0f && gameManager.GetOnSlow() == false && mail->GetIsArrive() != true) {
			m_Lerp = 1.0f;
			//���[�����擾��Ԃɂ���
			mail->ArriveMail();
			//���o�Ń`�F�C�������΂�
			auto flyingChain = GetStage()->AddGameObject<FlyingChain>();
			//���o�Ń`�F�C�������΂����߂ɒl��^����
			flyingChain->FlySet(GetChain());
			//�X���[�̌o�ߎ��Ԃ����Z�b�g
			gameManager.ResetSloawPassage();
			//�X���[�ɂ���
			gameManager.SetOnSlow(true);
			if (m_chain % BONUS_CHAIN == 0)
			{
				auto ptrEffect = GetStage()->GetSharedGameObject<BonusEffect>(L"BonusEffect", false);
				if (ptrEffect) {
					auto playerPos = GetComponent<Transform>()->GetWorldPosition();
					ptrEffect->InsertBounusEffect(Vec3(playerPos.x, playerPos.y - 3.0f, playerPos.z), Vec2(3.0f, 3.0f));
				}
			}
		}
		//�x�W�G�Ȑ��̌v�Z
		pos = (1 - m_Lerp) * (1 - m_Lerp) * p0 + 2 * (1 - m_Lerp) * m_Lerp * p1 + m_Lerp * m_Lerp * p2;
		GetComponent<Transform>()->SetWorldPosition(pos);
		//if (!gameManager.GetOnSlow()) {
		//	//�v���C���[�ƈꏏ�ɓ������߂Ƀv���C���[��Leap�ŃJ�����𓮂���
		//	auto camera = GetStage()->GetView()->GetTargetCamera();
		//	dynamic_pointer_cast<TpsCamera>(camera)->BezierMove(m_Lerp, pos);
		//}

	}
	///-------------------------------------------------------------------------------
	//���b�N�I���Ώۂ�������A�����������
	///-------------------------------------------------------------------------------
	void Player::RockOn() {
		//���b�N�I�����Ă���Ƃ�
		if (m_islockon) {
			auto sightPos = m_SightingDevice.lock()->GetComponent<Transform>()->GetWorldPosition();
			auto lockOnPos = m_LockOnObj.lock()->GetComponent<Transform>()->GetWorldPosition();

			auto camera = GetStage()->GetView()->GetTargetCamera();
			auto tpsCamera = dynamic_pointer_cast<TpsCamera>(camera);
			//���b�N�I���I�u�W�F��ΏۂɃJ�����̏���
			tpsCamera->RockonCameraMove(sightPos, lockOnPos);
			//���b�N�I���Ώۂ��߂��ɂ���ꍇ�̓��b�N�I��������
			Vec3 delta = GetComponent<Transform>()->GetWorldPosition() - lockOnPos;
			if (delta.length() <= 4.0f) {
				m_islockon = false;
			}
		}
	}
	///---------------------------------------------------------------------------------------------
	//���b�N�I������I�u�W�F�N�g��ݒ�
	///---------------------------------------------------------------------------------------------
	void Player::RockonObject(Vec3 origin, Vec3 originDir, wstring groupName, float correction){
		//L�{�^�������������Ă���Ƃ�
		if (GameManager::GetInstance().GetPad().wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
			m_islockon = false;
			return;
		}
		//�������łɃ��b�N�I�����Ă���Ώۂ�����
		else if(m_islockon){
			return;
		}

		//�I�u�W�F�N�g�̓����Ă���O���[�v�������Ă���
		auto& objectsGroup = GetStage()->GetSharedObjectGroup(groupName);
		//������o��
		for (auto& object : objectsGroup->GetGroupVector()) {
			auto lockonObj = object.lock();
			auto mail = dynamic_pointer_cast<MailObject>(lockonObj);
			//���[�����擾��Ԃ�������X���[����
			if (mail) {
				if (mail->GetIsArrive()) {
					continue;
				}
			}
			auto objTrans = lockonObj->GetComponent<Transform>();
			//�����N�I�u�W�F�N�g��OBB�����
			OBB obb(objTrans->GetScale() * correction, objTrans->GetWorldMatrix());
			//�Ə�����ł�Ray��OBB�Ŕ���
			bool hit = HitTest::SEGMENT_OBB(origin, origin + originDir * m_rayRange, obb);
			if (hit) {
				Vec3 delta = origin - objTrans->GetWorldPosition();
				float deltaLength = delta.length();
				//�߂��Ƃ��̓��b�N�I�����Ȃ�
				if (deltaLength <= 8.0f) {
					continue;
				}
				//Ray�͈̔͂����������́A�����͓������Ă��Ă���������
				if (deltaLength >= m_rayRange + 1) {
					continue;
				}
				m_LockOnObj = lockonObj;
				m_islockon = true;
				break;
			}
		}

	}
	///-------------------------------------------------------------------------------
	//�����o�����K�v�����肷��
	///-------------------------------------------------------------------------------
	void Player::ExtrusionJudgment(const weak_ptr<GameObject>& Other) {
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
	///-------------------------------------------------------------------------------
	//�����o��������
	///-------------------------------------------------------------------------------
	void Player::Extrusion() {
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
	}

	void Player::FaceChanger(FaceState beforeFace, FaceState afterFace)
	{
		auto drawComp = GetComponent<PNTBoneModelDraw>();
		drawComp->SetMultiMeshIsDraw(beforeFace, false);
		drawComp->SetMultiMeshIsDraw(afterFace, true);
		m_faceNum = afterFace;
	}
	///-------------------------------------------------------------------------------
	// �^�C���A�b�v���ɃA�j���[�V������������
	///-------------------------------------------------------------------------------
	void Player::ShowTimeUpAnime() {
		GetAnimStateMachine()->ChangeState(PlayerClearAnim::Instance());
	}
	///-------------------------------------------------------------------------------
	//�^�C���A�b�v�A�j���[�V�������I������烊�U���g�ɂ���
	///-------------------------------------------------------------------------------
	void Player::TimeUpAnimeFinishToResult() {
		if (GetComponent<PNTBoneModelDraw>()->IsTargetAnimeEnd() && GameManager::GetInstance().GetTimeUp()) {
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToResultStage");
		}
	}
	///-------------------------------------------------------------------------------
	//�v���C���[�̌����Ă��������Ԃ�
	///-------------------------------------------------------------------------------
	Vec2 Player::forwardAngle() {
		Vec2 forwardAngle;
		//���̃J�����ʒu�𐧌䂷��p�x
		forwardAngle.y = atan2f(m_forward.z, m_forward.x);
		if (forwardAngle.y < 0.0f) {
			forwardAngle.y += Deg2Rad(360.0f);
		}
		//�c�̃J�����ʒu���������߂̕ӏo��
		float syahen = hypotf(m_forward.x, m_forward.z);
		//�c�̃J�����ʒu�𐧌䂷��p�x
		forwardAngle.x = atan2f(m_forward.y, syahen);

		return forwardAngle;
	};
	///-------------------------------------------------------------------------------
	// �`�F�C�����𑝂₷
	///-------------------------------------------------------------------------------
	void Player::AddChain()
	{
		m_chain++;
		auto& gm = GameManager::GetInstance();
		//���݃`�F�C�������ő�`�F�C��������������X�V����
		if (gm.GetMaxChain() < m_chain) {
			gm.SetMaxChain(m_chain);
		}

		if (m_chain % BONUS_CHAIN == 0)
		{
			gm.AddBouns();
		}
	}
	///-------------------------------------------------------------------------------
	// �`�F�C�����s���ɐF�X���Z�b�g����
	///-------------------------------------------------------------------------------
	void Player::ChainFailure() {
		m_chain = 0;
		m_rayRange = m_rayRangeDefolt;
		//���b�N�I���Ώۂ�������
		if (m_islockon) {
			Vec3 delta = m_SightingDevice.lock()->GetComponent<Transform>()->GetWorldPosition()
				- m_LockOnObj.lock()->GetComponent<Transform>()->GetWorldPosition();
			float deltaLength = delta.length();
			//Ray�͈̔͂ƃ��b�N�I���I�u�W�F�Ƃ̋������ׂĔ͈͊O�������烍�b�N�I�����O��
			if (deltaLength >= m_rayRange + 1)
			{
				m_islockon = false;
			}
		}

		auto& gm = GameManager::GetInstance();
		gm.ResetBouns();
	}


	//--------------------------------------------------------------------------------------
	//	class LinkState : public ObjState<Player>;
	//	�p�r: �����N�����ł�����
	//--------------------------------------------------------------------------------------
	shared_ptr<LinkState> LinkState::Instance() {
		static shared_ptr<LinkState> instance(new LinkState);
		return instance;
	}
	void LinkState::Enter(const shared_ptr<Player>& Obj) {
		Obj->FaceChanger(Obj->m_faceNum, Player::FaceState::Smile);
		auto scenePtr = App::GetApp()->GetScene<Scene>();
		// ���ʉ���炷
		scenePtr->MusicOnceStart(L"Jump_SE", 1.0f);
		Obj->GetAnimStateMachine()->ChangeState(PlayerFlyAnim::Instance());

		// �R���{�����Z����
		Obj->AddChain();

	}
	void LinkState::Execute(const shared_ptr<Player>& Obj) {
		auto camera = Obj->GetStage()->GetView()->GetTargetCamera();
		auto tpsCamera = dynamic_pointer_cast<TpsCamera>(camera);
		//�J�����𓮂���
		tpsCamera->CameraRoll(Obj->GetSightPos());
		if (!GameManager::GetInstance().GetTimeUp()) {
			//�J��������̂��߂̒l��ύX����
			tpsCamera->CameraControll();

			//�X���[��ԂȂ�A���C���J������
			if (GameManager::GetInstance().GetOnSlow()) {
				Obj->ResetGoLink();
				Obj->RayShot();
			}
			else {
				Obj->PlayerRoll();
			}
		}
		//�ǂ�ɔ��ł��邩�ŏ�����ς���
		if (Obj->GetTarget() == Obj->LINK) {
			Obj->LinkGo();
		}
		if (Obj->GetTarget() == Obj->DRONE) {
			Obj->DroneGo();
		}
		if (Obj->GetTarget() == Obj->MAIL) {
			Obj->MailGo();
		}
		Obj->SightingDeviceChangePosition();
	}
	void LinkState::Exit(const shared_ptr<Player>& Obj) {
		Obj->FaceChanger(Obj->m_faceNum, Player::FaceState::Default);
	}
	//--------------------------------------------------------------------------------------
	//	class DataState : public ObjState<Player>;
	//	�p�r: �f�[�^�̏��
	//--------------------------------------------------------------------------------------
	shared_ptr<DataState> DataState::Instance() {
		static shared_ptr<DataState> instance(new DataState);
		return instance;
	}
	void DataState::Enter(const shared_ptr<Player>& Obj) {

	}
	void DataState::Execute(const shared_ptr<Player>& Obj) {
		auto camera = Obj->GetStage()->GetView()->GetTargetCamera();
		auto tpsCamera = dynamic_pointer_cast<TpsCamera>(camera);

		Obj->Forword();
		Obj->ResetGoLink();
		Obj->RayShot();
		Obj->SightingDeviceChangePosition();
		//�J�����𓮂���
		tpsCamera->CameraRoll(Obj->GetSightPos());
		//�J��������̂��߂̒l��ύX����
		tpsCamera->CameraControll();
		//�v���C���[�̑̂̌�����ς���
		Obj->PlayerRoll();
		//�^�C���A�b�v���Ă�����A�A�j���[�V�����𗬂�
		if (GameManager::GetInstance().GetTimeUp()) {
			Obj->GetStateMachine()->ChangeState(ClearState::Instance());
		}
	}
	void DataState::Exit(const shared_ptr<Player>& Obj) {
		Obj->ResetTimeLim();
	}
	//--------------------------------------------------------------------------------------
	//class ClearState : public ObjState<Player>;
	//�p�r: �N���A�A�j���[�V�������
	//--------------------------------------------------------------------------------------
	shared_ptr<ClearState> ClearState::Instance() {
		static shared_ptr<ClearState> instance(new ClearState);
		return instance;
	}
	void ClearState::Enter(const shared_ptr<Player>& Obj) {
		auto camera = Obj->GetStage()->GetView()->GetTargetCamera();
		auto tpsCamera = dynamic_pointer_cast<TpsCamera>(camera);
		tpsCamera->SetGoingAround(Obj->forwardAngle().x, Obj->forwardAngle().y);
	}
	void ClearState::Execute(const shared_ptr<Player>& Obj) {
		auto camera = Obj->GetStage()->GetView()->GetTargetCamera();
		auto tpsCamera = dynamic_pointer_cast<TpsCamera>(camera);
		Obj->SightingDeviceChangePosition();

		//�J�����̃v���C���[�̐��ʂɉ�鏈�����Ă�
		if (!tpsCamera->GoingAround()) {
			//���ʂɉ������N���A�A�j���[�V����
			Obj->ShowTimeUpAnime();
		}
		tpsCamera->CameraRoll(Obj->GetSightPos());

		Obj->TimeUpAnimeFinishToResult();
	}
	void ClearState::Exit(const shared_ptr<Player>& Obj){
	}
}
//end basecross

