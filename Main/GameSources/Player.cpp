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
		m_angleX = m_tpsCamera.lock()->GetCameraAngleX();
		m_angleY = m_tpsCamera.lock()->GetCameraAngleY();
		m_maxAngleSpeed = m_tpsCamera.lock()->GetMaxAngleSpeed();
		m_cameraDistance = m_tpsCamera.lock()->GetCameraDistance();
		m_cameraLookUp = m_tpsCamera.lock()->GetCameraLookUp();
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
		ptrString->SetTextRect(Rect2D<float>(16.0f, 120.0f, 640.0f, 480.0f));

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
		drawComp->AddAnimation(L"Clear", 400, 50, true, 30.0f);
		//Col4 Color(1.0f, 0.2f, 1.0f, 0.7f);
		//drawComp->SetDiffuse(Color);
		//drawComp->SetColorAndAlpha(Color);
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

		m_animStateMachine->Update();
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

		// �f�o�b�O�����̕\��
		DrawStrings();
		CheckYButton();
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

		auto checkPoint = dynamic_pointer_cast<CheckPoint>(Other);
		if (checkPoint){
			checkPoint->ArriveCheckPoint();
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
			playerPos.x += m_nowWalkSpeed * m_forward.x * App::GetApp()->GetElapsedTime();
			playerPos.z += m_nowWalkSpeed * m_forward.z * App::GetApp()->GetElapsedTime();
			// ���݂̃A�j���X�e�[�g��Default��������
			if (GetAnimStateMachine()->GetCurrentState() == PlayerDefaultAnim::Instance())
			{
				GetAnimStateMachine()->ChangeState(PlayerMoveAnim::Instance());
			}
		}
		// pad�����͂���ĂȂ������݂̃A�j���X�e�[�g��Move��������
		else if(m_padDir.x == 0.0f && m_padDir.z == 0.0f && 
				GetAnimStateMachine()->GetCurrentState() == PlayerMoveAnim::Instance())
		{
			GetAnimStateMachine()->ChangeState(PlayerDefaultAnim::Instance());
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
			playerPos.y += -m_nowFallSpeed * App::GetApp()->GetElapsedTime();
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
		if (m_pad.fThumbRX > 0.7f || m_pad.fThumbRX < -0.7f ||
			m_pad.fThumbRY > 0.7f || m_pad.fThumbRY < -0.7f) {
			m_angleY += -m_pad.fThumbRX * m_maxAngleSpeed*2 * delta; // �J��������]������
			m_angleX += -m_pad.fThumbRY * m_maxAngleSpeed*2 * delta; // �J���������~������
		}
		else if(m_pad.fThumbRX > 0.2f || m_pad.fThumbRX < -0.2f ||
			m_pad.fThumbRY > 0.2f || m_pad.fThumbRY < -0.2f) {
			m_angleY += -m_pad.fThumbRX * m_maxAngleSpeed * delta; // �J��������]������
			m_angleX += -m_pad.fThumbRY * m_maxAngleSpeed * delta; // �J���������~������
		}
		//Y����p�x�̊ۂ�(-360<360)
		if (m_angleY > Deg2Rad(360.0f)) {
			m_angleY = Deg2Rad(0.0f);
		}
		if (m_angleY < Deg2Rad(0.0f)) {
			m_angleY = Deg2Rad(360.0f);
		}
		//X����p�x�̊ۂ�(-70<70)
		if (m_angleX > Deg2Rad(m_angeleXMax)) {
			m_angleX = Deg2Rad(m_angeleXMax);
		}
		if (m_angleX < -Deg2Rad(m_angeleXMax)) {
			m_angleX = -Deg2Rad(m_angeleXMax);
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
		auto sightPos = m_SightingDevice.lock()->GetComponent<Transform>()->GetWorldPosition();
		auto camera = GetStage()->GetView()->GetTargetCamera();
		//�Ə�������
		camera->SetAt(sightPos);
		auto eye = sightPos + Vec3(cos(m_angleY) * m_cameraDistance,
			sin(m_angleX) * m_cameraDistance, sin(m_angleY) * m_cameraDistance);
		camera->SetEye(eye);
	}
	//---------------------------------------------------------------------------------------------
	//L�{�^���������Ă���Ƃ��Ƀ����N�I�u�W�F���Ə��̋߂��������炻����������
	//---------------------------------------------------------------------------------------------
	void Player::Rock(Vec3 origin, Vec3 originDir, wstring groupName, float correction) {
		//L�{�^�������������Ă���Ƃ�
		if (GameManager::GetInstance().GetPad().wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
			if (!m_islockon) {
				RockonObject(origin, originDir, groupName, correction);
			}
		}
		//L�{�^���������Ă��Ȃ��Ƃ�
		else {
			m_islockon = false;
		}
		if (m_islockon) {
			Vec3 delta = GetComponent<Transform>()->GetWorldPosition() -
				m_LockOnObj.lock()->GetComponent<Transform>()->GetWorldPosition();
			if (delta.length() <= 4.0f) {
				m_islockon = false;
			}
		}
		//�����N�I�u�W�F�N�g���Ə��ɂƂ炦�ă��b�N�I�����Ă���Ƃ�
		if (m_islockon) {
			//�J�����̐���������Ȃ�
			auto sightingDevicePos = m_SightingDevice.lock()->GetComponent<Transform>()->GetWorldPosition();
			auto linkTrans = m_LockOnObj.lock()->GetComponent<Transform>();
			float deltaX = sightingDevicePos.x - linkTrans->GetWorldPosition().x;
			float deltaZ = sightingDevicePos.z - linkTrans->GetWorldPosition().z;
			float deltaY = sightingDevicePos.y - linkTrans->GetWorldPosition().y;
			//���̃J�����ʒu�𐧌䂷��p�x
			m_angleY = atan2f(deltaZ, deltaX);
			if (m_angleY < 0.0f) {
				m_angleY += Deg2Rad(360.0f);
			}
			//�c�̃J�����ʒu���������߂̕ӏo��
			float syahen = hypotf(deltaX, deltaZ);
			//�c�̃J�����ʒu�𐧌䂷��p�x
			m_angleX = atan2f(deltaY, syahen);
		}
	}

	///---------------------------------------------------------------------------------------------
	//���b�N�I������I�u�W�F�N�g��ݒ�
	///---------------------------------------------------------------------------------------------
	void Player::RockonObject(Vec3 origin, Vec3 originDir, wstring groupName, float correction)
	{
		//�����N�I�u�W�F�N�g�̓����Ă���O���[�v�������Ă���
		auto& objectsGroup = GetStage()->GetSharedObjectGroup(groupName);
		//������o��
		for (auto& object : objectsGroup->GetGroupVector()) {
			auto lockonObj = object.lock();
			auto mail = dynamic_pointer_cast<MailObject>(lockonObj);
			//���[���Ƃ̔�������Ă��āA���[�����擾��Ԃ�������X���[����
			if (mail) {
				if (mail->GetIsArrive()) {
					break;
				}
			}
			auto objTrans = lockonObj->GetComponent<Transform>();
			//�����N�I�u�W�F�N�g��OBB�����
			OBB obb(objTrans->GetScale() * correction, objTrans->GetWorldMatrix());
			//�Ə�����ł�Ray��OBB�Ŕ���
			bool hit = HitTest::SEGMENT_OBB(origin, origin + originDir * m_rayRange, obb);
			if (hit) {
				Vec3 delta = GetComponent<Transform>()->GetWorldPosition() - objTrans->GetWorldPosition();
				//�߂��Ƃ��̓��b�N�I�����Ȃ�
				float deltaLength = delta.length();
				if (deltaLength <= 4.0f) {
					break;
				}
				m_LockOnObj = lockonObj;
				m_islockon = true;
				break;
			}
		}

	}
	
	//---------------------------------------------------------------------------------------------
	//�����N�֔�ԏ���
	//---------------------------------------------------------------------------------------------
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
		}
		//��ԏ������I�������
		if (gameManager.GetSlowPassage() >= 1.0f) {
			m_Lerp = 1.0f;
			//�X���[���I���
			gameManager.SetOnSlow(false);
			//�X�R�A�𑝂₷
			gameManager.AddScore(GetChain() * 20 + GetChain() * 10);
			//�O�Ղ�����
			m_ActionLine.lock()->SetOnDraw(false);
			//���̃����N�֔�ׂȂ������̂ŃR���{���Z�b�g����
			ResetCombo();
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
	//---------------------------------------------------------------------------------------------
	//�h���[���֔�ԏ���
	//---------------------------------------------------------------------------------------------
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
			ResetCombo();
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
	//---------------------------------------------------------------------------------------------
	//�`�F�b�N�|�C���g�֔�ԏ���
	//---------------------------------------------------------------------------------------------
	void Player::CheckPointGo()
	{
		auto& gameManager = GameManager::GetInstance();
		auto pos = GetComponent<Transform>()->GetWorldPosition();
		auto checkPointGroup = GetStage()->GetSharedObjectGroup(L"CheckPoints");
		auto checkPoint = dynamic_pointer_cast<CheckPoint>(checkPointGroup->at(m_checkPointNum));
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
				//�������Ă���h���[���̏�����
				m_checkPointNum = NULL;
				//���̖ڕW�֔�ׂȂ������̂ŃR���{���Z�b�g
				ResetCombo();
				//�X���[���Ԃ��I���������߃X�e�[�g���f�[�^�̂ɂ���
				m_StateMachine->ChangeState(DataState::Instance());
			}
		}
		else {
			//�o�߂ɉ��Z
			m_Lerp += addLerp;
			//�����Ă���̂ŏI�_�̈ʒu���X�V����
			p2 = checkPoint->GetComponent<Transform>()->GetWorldPosition();
		}
		//��ԏ������I��������ƂŁA�X���[��ԂłȂ����
		if (m_Lerp >= 1.0f && gameManager.GetOnSlow() == false && m_checkPointNum != NULL) {
			m_Lerp = 1.0f;
			// �Ώۂ̃`�F�b�N�|�C���g��false��������
			if (!checkPoint->GetIsArrive()) {
				//���o�Ń`�F�C�������΂����߂ɒl��^����
				GetStage()->GetSharedGameObject<FlyingChain>(L"FlyingChain")->FlySet(GetChain());
				// �t���O�𗧂Ă�
				checkPoint->ArriveCheckPoint();
				//�X���[�̌o�ߎ��Ԃ����Z�b�g
				gameManager.ResetSloawPassage();
				//�X���[�ɂ���
				gameManager.SetOnSlow(true);
			}
			else 
			{
				//�X���[���Ԃ��I���������߃X�e�[�g���f�[�^�̂ɂ���
				m_StateMachine->ChangeState(DataState::Instance());
			}
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
	//���[���֔�ԏ���
	//---------------------------------------------------------------------------------------------
	void Player::MailGo(){
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
				//���̖ڕW�֔�ׂȂ������̂ŃR���{���Z�b�g
				ResetCombo();
				//�O�Ղ�����
				m_ActionLine.lock()->SetDrawActive(false);
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
			//���o�Ń`�F�C�������΂����߂ɒl��^����
			GetStage()->GetSharedGameObject<FlyingChain>(L"FlyingChain")->FlySet(GetChain());
			//�X���[�̌o�ߎ��Ԃ����Z�b�g
			gameManager.ResetSloawPassage();
			//�X���[�ɂ���
			gameManager.SetOnSlow(true);
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
			p1 = point + Vec3(0, 6, 0);
		}
		else if(m_BezierSpeedLeap >= 10.0f){
			p1 = point + Vec3(0, 3, 0);
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
		//�����N�I�u�W�F�̃��b�N�I��������
		m_islockon = false;
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
		sightingDevice->GetComponent<Transform>()->SetWorldPosition((pos + Vec3(0.0f, m_cameraLookUp, 0.0f)));

		float syahen = hypotf(dir.x, dir.z);
		Quat rot;
		rot.rotationRollPitchYawFromVector(Vec3(-atan2f(dir.y,syahen),atan2f(dir.x, dir.z), 0.0f));
		
		sightingDevice->GetComponent<Transform>()->SetQuaternion(rot);
	}
	//---------------------------------------------------------------------------------------------
	//Ray���΂�
	//---------------------------------------------------------------------------------------------
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
		//�����傫�߂̔�����Ƃ��āAL�������ē������Ă����ꍇ�����N�I�u�W�F�ɃJ����������
		Rock(sightPos, dir, L"Link", 3.0f);
		Rock(sightPos, dir, L"CheckPoints", 2.0f);
		Rock(sightPos, dir, L"Mails", 3.0f);
		//�h���[���Ƃ̔���
		DroneRayCheck(sightPos, dir);
		//�����N�I�u�W�F�N�g�Ƃ̔���
		LinkRayCheck(sightPos, dir);
		// �`�F�b�N�|�C���g�Ƃ̔���
		CheckPointsRayCheck(sightPos, dir);
		//���[���Ƃ̔���
		MailRayCheck(sightPos, dir);
	}
	//---------------------------------------------------------------------------------------------
	//Ray����������	
	//---------------------------------------------------------------------------------------------
	void Player::RayView(Vec3 origin, Vec3 end) {
		auto ptrActionLine = m_ActionLine.lock();
		if (ptrActionLine) {
			ptrActionLine->ResetObject(origin,end);
			ptrActionLine->SetOnDraw(true);
		}
		else {
			//���C���̍쐬
			auto ptrLine = GetStage()->AddGameObject<ActionLine>(origin, end);
			m_ActionLine = ptrLine;
			ptrLine->SetDrawActive(true);
		}

	}
	//---------------------------------------------------------------------------------------------
	//Ray��Link�I�u�W�F�N�g���������Ă��邩�𒲂ׂ�
	//---------------------------------------------------------------------------------------------
	void Player::LinkRayCheck(Vec3 origin,Vec3 originDir) {
		//���łɑ��̂��̂ɔ��ł�����
		if (m_isGoLink) {
			return;
		}
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
			bool hit = HitTest::SEGMENT_OBB(origin, origin + originDir * m_rayRange, obb);
			//�Ō�Ƀx�W�G�Ȑ��Ŕ�񂾃����N�I�u�W�F�N�g����Ȃ����̂ɓ������Ă�����
			if (hit && p2 + Vec3(0, -1, 0) != linkTrans->GetWorldPosition()) {
				//�Ə��ɓ������Ă��邱�Ƃ�������
				sightingDevice->SetCaptureLink(true);
				if (m_pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
					Vec3 dir = GetComponent<Transform>()->GetWorldPosition() - linkTrans->GetWorldPosition();
					dir.y = 0;
					//�I�u�W�F�N�g�ɔ��Ȃ��悤�ɕ������������Ă��炵���l��n��
					SetBezierPoint(linkTrans->GetWorldPosition() + dir.normalize());
					//�O��
					RayView(origin, linkTrans->GetWorldPosition() + dir.normalize());
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
					break;
				}
			}
		}
	}
	//---------------------------------------------------------------------------------------------
	//Ray��Drone�I�u�W�F�N�g���������Ă��邩�𒲂ׂ�
	//---------------------------------------------------------------------------------------------
	void Player::DroneRayCheck(Vec3 origin, Vec3 originDir) {
		//���łɑ��̂��̂ɔ��ł�����
		if (m_isGoLink) {
			return;
		}
		auto sightingDevice = m_SightingDevice.lock();
		//�h���[���I�u�W�F�N�g�̓����Ă���O���[�v�������Ă���
		auto& droneGroup = GetStage()->GetSharedObjectGroup(L"Drone");
		//���Ԗڂ�Drone��������
		int count = 0;
		//������o��
		for (auto& drone : droneGroup->GetGroupVector()) {
			auto droneObj = drone.lock();
			auto droneTrans = droneObj->GetComponent<Transform>();
			//�h���[���I�u�W�F�N�g��OBB�����
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
					if (m_ActionLine.lock()) {
						m_ActionLine.lock()->SetDrawActive(false);
					}
					//�o�߂����Z�b�g
					m_Lerp = 0;
					//��Ԃ̊m��
					m_isGoLink = true;
					//�X���[�̌o�ߎ��Ԃ����Z�b�g
					GameManager::GetInstance().ResetSloawPassage();
					m_StateMachine->ChangeState(LinkState::Instance());
					m_target = Target::DRONE;
					break;
				}
			}
			count++;
		}
	}
	//---------------------------------------------------------------------------------------------
	//Ray��CheckPoint�I�u�W�F�N�g���������Ă��邩�𒲂ׂ�
	//---------------------------------------------------------------------------------------------
	void Player::CheckPointsRayCheck(Vec3 origin, Vec3 originDir){
		//���łɑ��̂��̂ɔ��ł�����
		//if (m_target != Target::NOTHING) {
		//	return;
		//}
		auto sightingDevice = m_SightingDevice.lock();
		auto& checkPointsGroup = GetStage()->GetSharedObjectGroup(L"CheckPoints");
		int count = 0;
		for (auto& checkPoint : checkPointsGroup->GetGroupVector()){
			auto pointObj = checkPoint.lock();
			auto pointTrans = pointObj->GetComponent<Transform>();
			OBB obb(pointTrans->GetScale() * 2.0f, pointTrans->GetWorldMatrix());
			//�v���C���[����ł�Ray��OBB�Ŕ���
			bool hit = HitTest::SEGMENT_OBB(origin, origin + originDir * m_rayRange, obb);
			//�Ō�Ƀx�W�G�Ȑ��Ŕ�񂾃����N�I�u�W�F�N�g����Ȃ����̂ɓ������Ă�����
			if (hit && p2 + Vec3(0, -1, 0) != pointTrans->GetWorldPosition()) {
				//�Ə��ɓ������Ă��邱�Ƃ�������
				sightingDevice->SetCaptureLink(true);
				if (m_pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
					Vec3 dir = GetComponent<Transform>()->GetWorldPosition() - pointTrans->GetWorldPosition();
					dir.y = 0;
					//�I�u�W�F�N�g�ɔ��Ȃ��悤�ɕ������������Ă��炵���l��n��
					SetBezierPoint(pointTrans->GetWorldPosition() + dir.normalize());
					m_checkPointNum = count;
					//�O��
					RayView(origin, pointTrans->GetWorldPosition() + dir.normalize());
					m_Lerp = 0;
					//�h���[�����������܂܂��Ƃ�����̂ق��Ɍ������Ă��܂��̂�NULL�ɂ���
					m_DroneNo = NULL;
					m_StateMachine->ChangeState(LinkState::Instance());
					m_target = Target::CHECKPOINT;
					break;
				}
			}
			count++;
		}
	}
	//---------------------------------------------------------------------------------------------
	//Ray��Mail�I�u�W�F�N�g���������Ă��邩�𒲂ׂ�
	//---------------------------------------------------------------------------------------------
	void Player::MailRayCheck(Vec3 origin, Vec3 originDir) {
		//���łɑ��̂��̂ɔ��ł�����
		if (m_isGoLink) {
			return;
		}
		auto sightingDevice = m_SightingDevice.lock();
		auto& mailGroup = GetStage()->GetSharedObjectGroup(L"Mails");
		int count = 0;
		for (auto& mail : mailGroup->GetGroupVector()) {
			auto mailObj = mail.lock();
			if (dynamic_pointer_cast<MailObject>(mailObj)->GetIsArrive() != true) {
				auto mailTrans = mailObj->GetComponent<Transform>();
				OBB obb(mailTrans->GetScale() * 3.0f, mailTrans->GetWorldMatrix());
				//�v���C���[����ł�Ray��OBB�Ŕ���
				bool hit = HitTest::SEGMENT_OBB(origin, origin + originDir * m_rayRange, obb);
				//�Ō�Ƀx�W�G�Ȑ��Ŕ�񂾃����N�I�u�W�F�N�g����Ȃ����̂ɓ������Ă�����
				if (hit && p2 + Vec3(0, -1, 0) != mailTrans->GetWorldPosition()) {
					//�Ə��ɓ������Ă��邱�Ƃ�������
					sightingDevice->SetCaptureLink(true);
					if (m_pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
						Vec3 dir = GetComponent<Transform>()->GetWorldPosition() - mailTrans->GetWorldPosition();
						dir.y = 0;
						//�I�u�W�F�N�g�ɔ��Ȃ��悤�ɕ������������Ă��炵���l��n��
						SetBezierPoint(mailTrans->GetWorldPosition() + dir.normalize());
						//�O��
						RayView(origin, mailTrans->GetWorldPosition() + dir.normalize());
						//���Ԃ̃��[���ɃA�N�Z�X���Ă��邩�ۑ�
						m_MailNum = count;
						m_Lerp = 0;
						//��Ԃ̊m��
						m_isGoLink = true;
						//�h���[�����������܂܂��Ƃ�����̂ق��Ɍ������Ă��܂��̂�NULL�ɂ���
						m_DroneNo = NULL;
						m_StateMachine->ChangeState(LinkState::Instance());
						m_target = Target::MAIL;
						break;
					}
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

	// Y�{�^���������ꂽ���ǂ�����Ԃ�
	bool Player::CheckYButton()
	{
		if (m_pad.wPressedButtons & XINPUT_GAMEPAD_Y)
		{
			FaceChanger(m_faceNum, FaceState::Smile);
			return true;
		}
		return false;
	}

	void Player::FaceChanger(FaceState beforeFace, FaceState afterFace)
	{
		auto drawComp = GetComponent<PNTBoneModelDraw>();
		drawComp->SetMultiMeshIsDraw(beforeFace, false);
		drawComp->SetMultiMeshIsDraw(afterFace, true);
		m_faceNum = afterFace;
	}

	void Player::CheckPointArrived()
	{
		auto& checkPointGroup = GetStage()->GetSharedObjectGroup(L"CheckPoints");
		for (auto& checkPoint : checkPointGroup->GetGroupVector())
		{
			auto pointObj = checkPoint.lock();
			auto obj = dynamic_pointer_cast<CheckPoint>(pointObj);
			obj->ArriveCheckPoint();
		}
		GetAnimStateMachine()->ChangeState(PlayerClearAnim::Instance());
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
		wstring checkNum(L"CheckPointNum : ");
		auto& gm = GameManager::GetInstance();
		checkNum += Util::IntToWStr(gm.GetCheckPointNum()) + L"\n";
		wstring combo(L"Combo : ");
		combo += Util::IntToWStr(m_chain) + L"\n";
		wstring chainLimit(L"CHAIN_TIME : ");
		chainLimit += Util::IntToWStr(m_chainTime) + L"\n";
		wstring timeLimit(L"Limit : ");
		timeLimit += Util::IntToWStr(m_comboChainLimit) + L"\n";
		wstring faceNum(L"FACE_NUM : ");
		faceNum += Util::IntToWStr(m_faceNum) + L"\n";

		//�����������
		//wstring str = strFps + cameraStr + energy + combo + timeLimit;

		wstring str = strFps + faceNum + checkNum;
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
	//	class LinkState : public ObjState<Player>;
	//	�p�r: �����N�����ł�����
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	shared_ptr<LinkState> LinkState::Instance() {
		static shared_ptr<LinkState> instance(new LinkState);
		return instance;
	}


	//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
	void LinkState::Enter(const shared_ptr<Player>& Obj) {
		Obj->FaceChanger(Obj->m_faceNum, Player::FaceState::Smile);
		auto scenePtr = App::GetApp()->GetScene<Scene>();
		// ���ʉ���炷
		scenePtr->MusicOnceStart(L"Jump_SE", 1.0f);
		Obj->GetAnimStateMachine()->ChangeState(PlayerFlyAnim::Instance());
		auto pos = Obj->GetComponent<Transform>()->GetWorldPosition();
		auto camera = Obj->GetStage()->GetView()->GetTargetCamera();
		camera->SetAt(pos + Vec3(0.0f, 1.0f, 0.0f));
		camera->SetEye(pos + Vec3(0.0f, 1.5f, -10.0f));
		
		// �R���{�����Z����
		Obj->AddCombo();

		Obj->ResetGoLink();
	}
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void LinkState::Execute(const shared_ptr<Player>& Obj) {
		Obj->CameraControll();
		Obj->CameraRoll();

		//�X���[��ԂȂ�A�J�����ƃ��C���J������
		if (GameManager::GetInstance().GetOnSlow()) {
			Obj->RayShot();
			Obj->ResetGoLink();
		}
		else {
			Obj->PlayerRoll();
		}
		//�ǂ�ɔ��ł��邩�ŏ�����ς���
		if (Obj->GetTarget() == Obj->LINK){
			Obj->LinkGo();
		}
		if (Obj->GetTarget() == Obj->DRONE) {
			Obj->DroneGo();
		}
		if (Obj->GetTarget() == Obj->CHECKPOINT){
			Obj->CheckPointGo();
			//Obj->CheckPointArrived();
		}
		if (Obj->GetTarget() == Obj->MAIL) {
			Obj->MailGo();
		}
		Obj->SightingDeviceChangePosition();
	}
	//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
	void LinkState::Exit(const shared_ptr<Player>& Obj) {
		Obj->FaceChanger(Obj->m_faceNum, Player::FaceState::Default);
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
		Obj->ChengeEnergyMai();
		Obj->ChangeWalkSpeed(Player::State::DATA);

	}
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void DataState::Execute(const shared_ptr<Player>& Obj) {
		Obj->Walk();
		Obj->RayShot();
		Obj->SightingDeviceChangePosition();
		//�J��������̂��߂̒l��ύX����
		Obj->CameraControll();
		//�E�X�e�B�b�N�̒l�ŃJ�����̉�]�������s��
		Obj->CameraRoll();
		//�v���C���[�̑̂̌�����ς���
		Obj->PlayerRoll();
	}
	//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
	void DataState::Exit(const shared_ptr<Player>& Obj) {
		Obj->ResetTimeLim();
	}

}
//end basecross

