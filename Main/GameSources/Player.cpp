/*!
@file Player.cpp
@brief ƒvƒŒƒCƒ„[‚È‚ÇÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"


namespace basecross{
	//------------------------------------------------------------------------------------------------
	//player‚ÌƒRƒ“ƒXƒgƒ‰ƒNƒ^
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
	//‰Šú‰»
	//-------------------------------------------------------------------------------------------------
	void Player::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();

		ptrTrans->SetWorldPosition(m_position);
		ptrTrans->SetQuaternion(m_quaternion);
		ptrTrans->SetScale(m_scale);

		//lŠpŒ`‚Ì“–‚½‚è”»’è‚ğƒZƒbƒg
		auto col = AddComponent<CollisionObb>();
		//col->SetDrawActive(true);
		//ƒRƒŠƒWƒ‡ƒ“‚Ì”»’è‚ğ‚µ‚È‚¢
		col->SetAfterCollision(AfterCollision::None);
		//player‚É‚½‚¹‚Äg‚¤‚à‚Ì‚É‚ÍÕ“Ë‚µ‚È‚¢
		col->AddExcludeCollisionTag(L"PlayerUse");

		//•¶š—ñ‚ğ‚Â‚¯‚é
		auto ptrString = AddComponent<StringSprite>();
		ptrString->SetText(L"");
		ptrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));

		Mat4x4 spanMat; // ƒ‚ƒfƒ‹‚Æƒgƒ‰ƒ“ƒXƒtƒH[ƒ€‚ÌŠÔ‚Ì·•ªs—ñ
		spanMat.affineTransformation(
			Vec3(0.8f, 0.8f, 0.8f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, Deg2Rad(180), 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		//•`‰æƒRƒ“ƒ|[ƒlƒ“ƒg‚Ì’Ç‰Á
		auto drawComp = AddComponent<PNTBoneModelDraw>();
		//•`‰æƒRƒ“ƒ|[ƒlƒ“ƒg‚ÉŒ`óiƒƒbƒVƒ…j‚ğİ’è
		drawComp->SetMultiMeshResource(L"PLAYER_MODEL");
		drawComp->SetTextureResource(L"PLAYER_TX");
		// •ÏŠ·‚µ‚½s—ñ‚ğ‘ã“ü
		drawComp->SetMeshToTransformMatrix(spanMat);
		// ƒAƒjƒ[ƒVƒ‡ƒ“‚ğ’Ç‰Á‚·‚é
		drawComp->AddAnimation(L"Default", 0, 110, true, 60.0f);
		drawComp->AddAnimation(L"Move", 200, 30, true, 30.0f);
		drawComp->AddAnimation(L"Fly", 300, 40, false, 30.0f);
		//Col4 Color(1.0f, 0.2f, 1.0f, 0.7f);
		//drawComp->SetDiffuse(Color);
		//drawComp->SetColorAndAlpha(Color);
		// ƒŒƒCƒ„[‚Ì’²®
		SetDrawLayer(1);

		GetStage()->SetSharedGameObject(L"Player", GetThis<Player>());
		auto dev = GetStage()->AddGameObject<SightingDevice>();
		this->SetSightingDevice(dev);

		m_StateMachine.reset(new StateMachine<Player>(GetThis<Player>()));
		//Å‰‚ÌƒXƒe[ƒg‚ğDataState‚Éİ’è
		m_StateMachine->ChangeState(DataState::Instance());

		m_animStateMachine.reset(new StateMachine<Player>(GetThis<Player>()));
		m_animStateMachine->ChangeState(PlayerDefaultAnim::Instance());
	}
	//-------------------------------------------------------------------------------------------------------------
	//Update
	//-------------------------------------------------------------------------------------------------------------
	void Player::OnUpdate() {
		// ƒAƒvƒŠƒP[ƒVƒ‡ƒ“æ“¾
		auto& app = App::GetApp();
		// 1ƒtƒŒ[ƒ€‚ÌÀs‚É‚©‚©‚Á‚½ŠÔ‚ğæ“¾
		float delta = app->GetElapsedTime();

		// ƒQ[ƒ€ƒRƒ“ƒgƒ[ƒ‰[æ“¾
		auto device = app->GetInputDevice();
		if (app->GetScene<Scene>()->GetGameStart()) {
			m_pad = GameManager::GetInstance().GetPad();
		}
		else {
			m_nowFallSpeed = 5.0f;
			Fall();
		}

		//¶ƒXƒeƒBƒbƒN‚Ì“ü—Í•ûŒü‚ğ‹‚ß‚é
		m_padDir = Vec3(m_pad.fThumbLX, 0.0f, m_pad.fThumbLY);
		m_padDir = m_padDir.normalize();

		//ƒXƒe[ƒgƒ}ƒVƒ“‚ÌƒAƒbƒvƒf[ƒg
		m_StateMachine->Update();

		m_animStateMachine->Update();
		// ƒAƒjƒ[ƒVƒ‡ƒ“‚ğXV‚·‚é
		auto drawComp = GetComponent<PNTBoneModelDraw>();
		drawComp->UpdateAnimation(delta);
	};

	//--------------------------------------------------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------------------------------------------------
	void Player::OnUpdate2() {
		Vec3 pos = GetComponent<Transform>()->GetWorldPosition();
		// Å¬‚É‚È‚Á‚Ä‚¢‚é•ûŒü‚É‘Î‚µ‚Ä‰Ÿ‚µo‚µ‚ğs‚¤
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

		// ƒfƒoƒbƒO•¶š‚Ì•\¦
		DrawStrings();
	}
	//--------------------------------------------------------------------------------------------------------------
	//Õ“Ë‚µ‚½‚Æ‚«
	//--------------------------------------------------------------------------------------------------------------
	void Player::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		auto trans = GetComponent<Transform>();
		auto otherTrans = Other->GetComponent<Transform>();
		//player‚Ì‰º–Ê‚ÆÕ“Ë‚µ‚½•¨‘Ì‚Ìã–Ê‚ª“–‚½‚Á‚Ä‚¢‚½‚ç
		if (((otherTrans->GetWorldPosition().y + otherTrans->GetScale().y / 2) - (trans->GetWorldPosition().y - trans->GetScale().y / 2)) < 0.5f) {
			//©“®‚ÅY•ûŒü‚É—Í‚ğ‰Á‚¦‚éˆ—‚ğs‚í‚È‚¢‚æ‚¤‚É‚·‚é
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
	//Õ“Ë‚µ‘±‚¯‚Ä‚¢‚é‚Æ‚«
	//--------------------------------------------------------------------------------------------------------------
	void Player::OnCollisionExcute(shared_ptr<GameObject>& Other) {
		auto trans = GetComponent<Transform>();
		auto otherTrans = Other->GetComponent<Transform>();
		//player‚Ì‰º–Ê‚ÆÕ“Ë‚µ‚½•¨‘Ì‚Ìã–Ê‚ª“–‚½‚Á‚Ä‚¢‚½‚ç
		if (((otherTrans->GetWorldPosition().y + otherTrans->GetScale().y / 2) - (trans->GetWorldPosition().y - trans->GetScale().y / 2)) < 0.5f) {
			//©“®‚ÅY•ûŒü‚É—Í‚ğ‰Á‚¦‚éˆ—‚ğs‚í‚È‚¢‚æ‚¤‚É‚·‚é
			m_isFall = false;
		}
		Extrusion(Other);

	}
	//--------------------------------------------------------------------------------------------------------------
	//Õ“Ë‚ª‰ğœ‚³‚ê‚½‚Æ‚«
	//-------------------------------------------------------------------------------------------------------------
	void Player::OnCollisionExit(shared_ptr<GameObject>& Other) {
		auto wall = dynamic_pointer_cast<Wall>(Other);
		if (wall) {
			auto trans = GetComponent<Transform>();
			auto wallTrans = wall->GetComponent<Transform>();
			//player‚Ì‰º–Ê‚ÆÕ“Ë‚µ‚½•¨‘Ì‚Ìã–Ê‚ª“–‚½‚Á‚Ä‚¢‚½‚ç
			if (((wallTrans->GetWorldPosition().y + wallTrans->GetScale().y / 2) - (trans->GetWorldPosition().y - trans->GetScale().y / 2)) < 0.5f) {
				m_response = wallTrans->GetWorldPosition() + Vec3(0.0f, 5.0f, 0.0f);
				m_response.y += wallTrans->GetScale().y / 2;
			}
		}
	}

																	
	//--------------------------------------------------------------------------------------------
	//XZ•½–Ê‚ÌˆÚ“®ˆ—
	//--------------------------------------------------------------------------------------------
	void Player::Walk() {
		auto playerTrans = GetComponent<Transform>();
		auto playerPos = playerTrans->GetWorldPosition();
		//¶ƒXƒeƒBƒbƒN‚É’l‚ª“ü—Í‚³‚ê‚Ä‚¢‚½‚ç
		if (m_padDir.x > 0.4f || m_padDir.x < -0.4f ||
			m_padDir.z > 0.4f || m_padDir.z < -0.4f) {
			//•ûŒü‚ÆˆÚ“®ƒXƒs[ƒh‚ğŠ|‚¯Z‚µ‚Äpositon‚ğ•ÏX‚·‚é
			playerPos.x += m_nowWalkSpeed * m_forward.x * App::GetApp()->GetElapsedTime() * m_JummerSpeed;
			playerPos.z += m_nowWalkSpeed * m_forward.z * App::GetApp()->GetElapsedTime() * m_JummerSpeed;
			GetAnimStateMachine()->ChangeState(PlayerMoveAnim::Instance());
		}
		playerTrans->SetWorldPosition(playerPos);
	}
	//---------------------------------------------------------------------------------------------
	//Y•ûŒü‚ÌˆÚ“®ˆ—
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
	//ƒvƒŒƒCƒ„[‚ğŒü‚©‚¤•ûŒü‚É‰ñ“]‚³‚¹‚é
	//---------------------------------------------------------------------------------------------
	void Player::PlayerRoll() {
		auto ptrUtil = GetBehavior<UtilBehavior>();
		ptrUtil->RotToHead(m_forward, 0.1f);
	}
	//--------------------------------------------------------------------------------------------
	//•½sˆÚ“®‚Ì‘¬“x‚ğƒXƒe[ƒg‚Å•ª‚¯‚Ä•ÏX‚·‚é
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
	//İ’è‚‚³ˆÈ‰º‚É‚È‚Á‚½‚çƒŠƒXƒ|[ƒ“ˆÊ’u‚Éƒ[ƒv‚³‚¹‚é
	//--------------------------------------------------------------------------------------------
	void Player::Response() {
		if (GetComponent<Transform>()->GetWorldPosition().y <= m_responseHeght) {
			GetComponent<Transform>()->SetWorldPosition(m_response);
			//•œ‹AŒã‚É‚·‚®“®‚¯‚é‚æ‚¤‚É­‚µ‰ñ•œ‚³‚¹‚é
			m_changeEnergy = 20.0f;
		}
	}
	//--------------------------------------------------------------------------------------------
	//ƒJƒƒ‰‚Ì‰ñ“]ˆ—
	//--------------------------------------------------------------------------------------------
	void Player::CameraRoll() {
		float delta = App::GetApp()->GetElapsedTime();

		//‰EƒXƒeƒBƒbƒN‚É’l‚ª“ü—Í‚³‚ê‚Ä‚¢‚½‚ç
		if (m_pad.fThumbRX > 0.7f || m_pad.fThumbRX < -0.7f ||
			m_pad.fThumbRY > 0.7f || m_pad.fThumbRY < -0.7f) {
			m_angleY += -m_pad.fThumbRX * m_maxAngleSpeed*2 * delta; // ƒJƒƒ‰‚ğ‰ñ“]‚³‚¹‚é
			m_angleX += -m_pad.fThumbRY * m_maxAngleSpeed*2 * delta; // ƒJƒƒ‰‚ğ¸~‚³‚¹‚é
		}
		else if(m_pad.fThumbRX > 0.2f || m_pad.fThumbRX < -0.2f ||
			m_pad.fThumbRY > 0.2f || m_pad.fThumbRY < -0.2f) {
			m_angleY += -m_pad.fThumbRX * m_maxAngleSpeed * delta; // ƒJƒƒ‰‚ğ‰ñ“]‚³‚¹‚é
			m_angleX += -m_pad.fThumbRY * m_maxAngleSpeed * delta; // ƒJƒƒ‰‚ğ¸~‚³‚¹‚é
		}
		//Y²Šî€Šp“x‚ÌŠÛ‚ß(-360<360)
		if (m_angleY > Deg2Rad(360.0f)) {
			m_angleY = Deg2Rad(0.0f);
		}
		if (m_angleY < Deg2Rad(0.0f)) {
			m_angleY = Deg2Rad(360.0f);
		}
		//X²Šî€Šp“x‚ÌŠÛ‚ß(-70<70)
		if (m_angleX > Deg2Rad(m_angeleXMax)) {
			m_angleX = Deg2Rad(m_angeleXMax);
		}
		if (m_angleX < -Deg2Rad(m_angeleXMax)) {
			m_angleX = -Deg2Rad(m_angeleXMax);
		}

		// ƒXƒeƒBƒbƒN‚ÌŒX‚«‚ğŠp“x‚É•ÏŠ·‚·‚é
		float padRad = atan2f(m_padDir.z, m_padDir.x);
		if (m_padDir.length() != 0.0f) {
			// ƒXƒeƒBƒbƒN‚ÌŠp“x‚ÉƒJƒƒ‰‚ÌŠp“x‚ğ‰Á‚¦‚é
			padRad += m_angleY + XM_PIDIV2;  // ƒJƒƒ‰‚ÌŠp“x‚É‚X‚O“x‰Á‚¦‚é
			// Šp“x‚ğXZ¬•ª‚É•ª‰ğ‚µ‚ÄƒxƒNƒgƒ‹‚ğì‚è’¼‚·
			m_padDir.x = cosf(padRad); // V‚µ‚¢Šp“x‚ğ X ¬•ª‚É•ª‰ğ‚·‚é
			m_padDir.z = sinf(padRad); // V‚µ‚¢Šp“x‚ğ Z ¬•ª‚É•ª‰ğ‚·‚é

			m_forward = m_padDir;
		}
	}
	//---------------------------------------------------------------------------------------------
	//ƒJƒƒ‰‚ÌƒvƒŒƒCƒ„[’Ç]ˆ—
	//---------------------------------------------------------------------------------------------
	void Player::CameraControll(){
		auto pos = GetComponent<Transform>()->GetWorldPosition();
		auto sightPos = m_SightingDevice.lock()->GetComponent<Transform>()->GetWorldPosition();
		auto camera = GetStage()->GetView()->GetTargetCamera();
		//ï¿½Æï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
		camera->SetAt(sightPos);
		auto eye = pos + Vec3(cos(m_angleY) * m_cameraDistance,
			sin(m_angleX) * m_cameraDistance, sin(m_angleY) * m_cameraDistance);
		camera->SetEye(eye);
	}
	//---------------------------------------------------------------------------------------------
	//Lƒ{ƒ^ƒ“‚ğ‰Ÿ‚µ‚Ä‚¢‚é‚Æ‚«‚ÉƒŠƒ“ƒNƒIƒuƒWƒF‚ªÆ€‚Ì‹ß‚­‚¾‚Á‚½‚ç‚»‚Á‚¿‚ğŒü‚­
	//---------------------------------------------------------------------------------------------
	void Player::Rock(Vec3 origin, Vec3 originDir) {
		//Lƒ{ƒ^ƒ“‚ğ‰Ÿ‚µ‘±‚¯‚Ä‚¢‚é‚Æ‚«
		if (GameManager::GetInstance().GetPad().wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
			if (!m_islockon) {
				//ƒŠƒ“ƒNƒIƒuƒWƒFƒNƒg‚Ì“ü‚Á‚Ä‚¢‚éƒOƒ‹[ƒv‚ğ‚Á‚Ä‚­‚é
				auto& linkGroup = GetStage()->GetSharedObjectGroup(L"Link");
				//ˆê‚Â‚¸‚Âæ‚èo‚·
				for (auto& link : linkGroup->GetGroupVector()) {
					auto linkObj = link.lock();
					auto linkTrans = linkObj->GetComponent<Transform>();
					//ƒŠƒ“ƒNƒIƒuƒWƒFƒNƒg‚ÌOBB‚ğì‚é
					OBB obb(linkTrans->GetScale() * 5, linkTrans->GetWorldMatrix());
					//Æ€‚©‚ç‚Å‚éRay‚ÆOBB‚Å”»’è
					bool hit = HitTest::SEGMENT_OBB(origin, origin + originDir * m_rayRange, obb);
					if (hit) {
						m_LockOnObj = dynamic_pointer_cast<LinkObject>(linkObj);
						m_islockon = true;
						break;
					}
				}
			}
		}
		//Lƒ{ƒ^ƒ“‚ğ‰Ÿ‚µ‚Ä‚¢‚È‚¢‚Æ‚«
		else {
			m_islockon = false;
		}
		//ƒŠƒ“ƒNƒIƒuƒWƒFƒNƒg‚ğÆ€‚É‚Æ‚ç‚¦‚ÄƒƒbƒNƒIƒ“‚µ‚Ä‚¢‚é‚Æ‚«
		if (m_islockon) {
			//ƒJƒƒ‰‚Ì§Œä‚ğ‚¨‚±‚È‚¤
			auto m_cameraPos = GetStage()->GetView()->GetTargetCamera()->GetEye();
			auto linkTrans = m_LockOnObj.lock()->GetComponent<Transform>();
			float deltaX = m_cameraPos.x - linkTrans->GetWorldPosition().x;
			float deltaZ = m_cameraPos.z - linkTrans->GetWorldPosition().z;
			float deltaY = m_cameraPos.y - linkTrans->GetWorldPosition().y;
			//‰¡‚ÌƒJƒƒ‰ˆÊ’u‚ğ§Œä‚·‚éŠp“x
			m_angleY = atan2f(deltaZ, deltaX);
			if (m_angleY < 0.0f) {
				m_angleY += Deg2Rad(360.0f);
			}
			//c‚ÌƒJƒƒ‰ˆÊ’u‚ğ§Œä‚·‚éŠp“x
			float syahen = hypotf(deltaX, deltaZ);
			//ƒvƒŒƒCƒ„[‚Ìã‚ğŒ©‚é‚æ‚¤‚É‚µ‚Ä‚¢‚é‚Ì‚Å‚»‚Ì•ªã‚É‚¸‚ç‚·
			m_angleX = atan2f(deltaY, syahen) + (m_cameraLookUp / m_cameraDistance);
		}
	}
	//---------------------------------------------------------------------------------------------
	//ƒŠƒ“ƒN‚Ö”ò‚Ôˆ—
	//---------------------------------------------------------------------------------------------
	void Player::LinkGo() {
		auto pos = GetComponent<Transform>()->GetWorldPosition();
		auto& gameManager = GameManager::GetInstance();
		//ŒvZ‚Ì‚½‚ß‚ÌŠÔ‰ÁZ
		//ˆÚ“®‚ÌŒo‰ß‚Éƒ`ƒFƒCƒ“”‚É‚æ‚é‰Á‘¬‚ğ‚¢‚ê‚½
		auto addLerp = (App::GetApp()->GetElapsedTime() * (m_BezierSpeed + m_chain)) / (m_BezierSpeedLeap);
		//ƒXƒ[ó‘Ô‚©‚Ç‚¤‚©‚Åˆ—
		if (gameManager.GetOnSlow()) {
			//20•ª‚Ì1‚ÌXV‘¬“x‚É‚·‚é
			m_Lerp += addLerp / gameManager.GetSlowSpeed();
			//ƒ}ƒl[ƒWƒƒ[‚ÉƒXƒ[‚ÌŒo‰ß‚ğ“`‚¦‚é
			gameManager.AddSlowPassage((addLerp * 10 / gameManager.GetSlowSpeed()));
		}
		else {
			m_Lerp += addLerp;
		}
		//’…’n¡‘O‚ÅƒXƒ[‚É‚È‚Á‚Ä‚¢‚È‚¢
		if (m_Lerp >= 0.9f && gameManager.GetOnSlow() == false) {
			//ƒXƒ[‚ÌŒo‰ßŠÔ‚ğƒŠƒZƒbƒg
			gameManager.ResetSloawPassage();
			//ƒXƒ[‚É‚·‚é
			gameManager.SetOnSlow(true);
		}
		//”ò‚Ôˆ—‚ªI‚í‚Á‚½‚ç
		if (m_Lerp >= 1.0f) {
			m_Lerp = 1.0f;
			//ƒXƒ[‚ğI‚í‚é
			gameManager.SetOnSlow(false);
			//ƒXƒRƒA‚ğ‘‚â‚·
			gameManager.AddScore(GetChain() * 20 + GetChain() * 10);
			//Ÿ‚ÌƒŠƒ“ƒN‚Ö”ò‚×‚È‚©‚Á‚½‚Ì‚ÅƒRƒ“ƒ{ƒŠƒZƒbƒg‚·‚é
			ResetCombo();
			//”ò‚ÑI‚í‚Á‚½‚çƒXƒe[ƒg‚ğƒf[ƒ^‘Ì‚É‚·‚é
			m_StateMachine->ChangeState(DataState::Instance());
		}
		//ƒxƒWƒG‹Èü‚ÌŒvZ
		pos = (1 - m_Lerp) * (1 - m_Lerp) * p0 + 2 * (1 - m_Lerp) * m_Lerp * p1 + m_Lerp * m_Lerp * p2;
		GetComponent<Transform>()->SetWorldPosition(pos);
		if (!gameManager.GetOnSlow()) {
			//ƒvƒŒƒCƒ„[‚Æˆê‚É“®‚­‚½‚ß‚ÉƒvƒŒƒCƒ„[‚ÌLeap‚ÅƒJƒƒ‰‚ğ“®‚©‚·
			auto camera = GetStage()->GetView()->GetTargetCamera();
			dynamic_pointer_cast<TpsCamera>(camera)->BezierMove(m_Lerp, pos);
		}
	}
	//---------------------------------------------------------------------------------------------
	//ƒhƒ[ƒ“‚Ö”ò‚Ôˆ—
	//---------------------------------------------------------------------------------------------
	void Player::DroneGo() {
		auto& gameManager = GameManager::GetInstance();
		auto pos = GetComponent<Transform>()->GetWorldPosition();
		auto droneGroup = GetStage()->GetSharedObjectGroup(L"Drone");
		auto drone = dynamic_pointer_cast<Drone>(droneGroup->at(m_DroneNo));
		//ŒvZ‚Ì‚½‚ß‚ÌŠÔ‰ÁZ
		//ˆÚ“®‚ÌŒo‰ß‚Éƒ`ƒFƒCƒ“”‚É‚æ‚é‰Á‘¬‚ğ‚¢‚ê‚½
		auto addLerp = (App::GetApp()->GetElapsedTime() * (m_BezierSpeed + m_chain)) / (m_BezierSpeedLeap);
		//ƒXƒ[ó‘Ô‚©‚Ç‚¤‚©‚Åˆ—
		if (gameManager.GetOnSlow()) {
			//ƒ}ƒl[ƒWƒƒ[‚ÉƒXƒ[‚ÌŒo‰ß‚ğ“`‚¦‚é
			gameManager.AddSlowPassage((addLerp * 10 / gameManager.GetSlowSpeed()));
			if (gameManager.GetSlowPassage() >= 1.0f) {
				//ƒXƒ[‚ğI‚í‚é
				gameManager.SetOnSlow(false);
				//Œü‚©‚Á‚Ä‚¢‚éƒhƒ[ƒ“‚Ì‰Šú‰»
				m_DroneNo = NULL;
				//Ÿ‚Ì–Ú•W‚Ö”ò‚×‚È‚©‚Á‚½‚Ì‚ÅƒRƒ“ƒ{ƒŠƒZƒbƒg
				ResetCombo();
				//ƒXƒ[ŠÔ‚ªI—¹‚µ‚½‚½‚ßƒXƒe[ƒg‚ğƒf[ƒ^‘Ì‚É‚·‚é
				m_StateMachine->ChangeState(DataState::Instance());
			}
		}
		else {
			//Œo‰ß‚É‰ÁZ
			m_Lerp += addLerp;
			//“®‚¢‚Ä‚¢‚é‚Ì‚ÅI“_‚ÌˆÊ’u‚ğXV‚·‚é
			p2 = drone->GetComponent<Transform>()->GetWorldPosition();
		}
		//”ò‚Ôˆ—‚ªI‚í‚Á‚½‚ ‚Æ‚ÅAƒXƒ[ó‘Ô‚Å‚È‚¯‚ê‚Î
		if (m_Lerp >= 1.0f && gameManager.GetOnSlow() == false && m_DroneNo != NULL) {
			m_Lerp = 1.0f;
			//Œ»İƒ`ƒFƒCƒ“‚ªƒhƒ[ƒ“‚Ì€‚Éƒ`ƒFƒCƒ“”‚ğ’´‚¦‚Ä‚¢‚½ê‡
			if (drone->GetDeadChain() <= GetChain()) {
				//‰‰o‚Åƒ`ƒFƒCƒ“”‚ğ”ò‚Î‚·‚½‚ß‚É’l‚ğ—^‚¦‚é
				GetStage()->GetSharedGameObject<FlyingChain>(L"FlyingChain")->FlySet(GetChain());
				//“®‚©‚È‚­‚·‚é
				drone->Die();
				//ƒXƒ[‚ÌŒo‰ßŠÔ‚ğƒŠƒZƒbƒg
				gameManager.ResetSloawPassage();
				//ƒXƒ[‚É‚·‚é
				gameManager.SetOnSlow(true);
			}
			else {
				//“|‚¹‚È‚©‚Á‚½‚Ì‚ÅƒRƒ“ƒ{ƒŠƒZƒbƒg‚·‚é
				ResetCombo();
				//Œü‚©‚Á‚Ä‚¢‚éƒhƒ[ƒ“‚Ì‰Šú‰»
				m_DroneNo = NULL;
				//”ò‚ÑI‚í‚Á‚½‚çƒXƒe[ƒg‚ğƒf[ƒ^‘Ì‚É‚·‚é
				m_StateMachine->ChangeState(DataState::Instance());
			}
		}
		//ƒxƒWƒG‹Èü‚ÌŒvZ
		pos = (1 - m_Lerp) * (1 - m_Lerp) * p0 + 2 * (1 - m_Lerp) * m_Lerp * p1 + m_Lerp * m_Lerp * p2;
		GetComponent<Transform>()->SetWorldPosition(pos);
		if (!gameManager.GetOnSlow()) {
			//ƒvƒŒƒCƒ„[‚Æˆê‚É“®‚­‚½‚ß‚ÉƒvƒŒƒCƒ„[‚ÌLeap‚ÅƒJƒƒ‰‚ğ“®‚©‚·
			auto camera = GetStage()->GetView()->GetTargetCamera();
			dynamic_pointer_cast<TpsCamera>(camera)->BezierMove(m_Lerp, pos);
		}
	}
	//---------------------------------------------------------------------------------------------
	//ƒxƒWƒG‹Èü‚Ì§Œä“_İ’è
	//---------------------------------------------------------------------------------------------
	void Player::SetBezierPoint(Vec3 point) {

		p0 = GetComponent<Transform>()->GetWorldPosition();
		p2 = point + Vec3(0,1.0f,0);
		//”ò‚Ôæ‚Ü‚Å‚Ì‹——£‚É‰‚¶‚Ä”ò‚ÔÛ‚ÌƒXƒs[ƒh‚ğ•Ï‚¦‚é
		m_BezierSpeedLeap = Vec3(p2 - p0).length();
		//”ò‚Ôæ‚Ü‚Å‚Ì‹——£‚É‰‚¶‚Ä”ò‚ÔÛ‚Ì•ú•¨ü‚ÌŒ`‚ğ•Ï‚¦‚é
		if (m_BezierSpeedLeap >= 20.0f) {
			p1 = point + Vec3(0, 6, 0);
		}
		else if(m_BezierSpeedLeap >= 10.0f){
			p1 = point + Vec3(0, 3, 0);
		}
		else if(m_BezierSpeedLeap < 10.0f){
			p1 = point - ((point - p0) / 2);
		}
		//”ò‚ñ‚¾Û‚ÉƒŠƒXƒ|[ƒ“ˆÊ’u‚ÌXV‚às‚¤
		m_response = p2;
		//ƒJƒƒ‰‚Ì’Ç]‚·‚é“®‚«‚ğİ’è‚·‚é
		auto camera = GetStage()->GetView()->GetTargetCamera();
		dynamic_pointer_cast<TpsCamera>(camera)->SetBezier(GetThis<Player>(), p2);
		//Ÿ‚ÌƒŠƒ“ƒN‚ÉˆÚ‚é‚Ì‚ÅƒXƒ[‚ğ‰ğ‚­
		GameManager::GetInstance().SetOnSlow(false);
		//ƒvƒŒƒCƒ„[‚Ì³–ÊŒü‚«”ò‚Ô•ûŒü‚É‚·‚é
		m_forward = Vec3(p2 - p0).normalize();
		//ƒŠƒ“ƒNƒIƒuƒWƒF‚ÌƒƒbƒNƒIƒ“‚ğ‰ğœ
		m_islockon = false;
	}
	//---------------------------------------------------------------------------------------------
	//Æ€‚ÌˆÊ’u‚ğƒJƒƒ‰‚ÆƒvƒŒƒCƒ„[‚ÌˆÊ’u‚©‚ç‹‚ß•ÏX‚·‚é
	//---------------------------------------------------------------------------------------------
	void Player::SightingDeviceChangePosition() {
		auto pos = GetComponent<Transform>()->GetWorldPosition();
		auto m_cameraPos = GetStage()->GetView()->GetTargetCamera()->GetEye();
		//player‚ÆƒJƒƒ‰‚ÌˆÊ’u‚©‚çÆ€‚ª‚ ‚é•ûŒü‚ğ‹‚ß‚é
		//player‚Ì­‚µã‚Ì‚Ù‚¤‚É’u‚­‚½‚ßplayer‚Ìposition‚É’²®‚ğ“ü‚ê‚é
		auto dir = (pos + Vec3(0.0f, 5.0f, 0.0f)) - m_cameraPos;
		dir = dir.normalize();

		auto sightingDevice = m_SightingDevice.lock();
		//player‚Ì“ª•Ó‚è‚ÉA”í‚ç‚È‚¢‚æ‚¤ƒJƒƒ‰‚©‚ç‚Ì•ûŒü‚ğ‰Á–¡‚µ‚Ä’u‚­
		sightingDevice->GetComponent<Transform>()->SetWorldPosition((pos + Vec3(0.0f, m_cameraLookUp, 0.0f)) + (dir * 2.0f));

		Quat rot;
		rot.rotationRollPitchYawFromVector(Vec3(0.0f,atan2f(dir.x, dir.z), 0.0f));
		
		sightingDevice->GetComponent<Transform>()->SetQuaternion(rot);
	}
	//---------------------------------------------------------------------------------------------
	//Ray‚ğ”ò‚Î‚·
	//---------------------------------------------------------------------------------------------
	void Player::RayShot() {
		auto sightingDevice = m_SightingDevice.lock();
		//ƒŠƒ“ƒNƒIƒuƒWƒFƒNƒg‚É“–‚½‚Á‚Ä‚¢‚éƒtƒ‰ƒO‚ğfalse‚É–ß‚·
		sightingDevice->ResetCaptureLink();

		auto pos = sightingDevice->GetComponent<Transform>()->GetWorldPosition();
		auto m_cameraPos = GetStage()->GetView()->GetTargetCamera()->GetEye();
		//Æ€‚ÆƒJƒƒ‰‚ÌˆÊ’u‚©‚ç”ò‚Î‚·•ûŒü‚ğ‹‚ß‚é
		auto dir = pos - m_cameraPos;
		dir = dir.normalize();
		//­‚µ‘å‚«‚ß‚Ì”»’è‚ğ‚Æ‚Á‚ÄAL‚ğ‰Ÿ‚µ‚Ä“–‚½‚Á‚Ä‚¢‚½ê‡ƒŠƒ“ƒNƒIƒuƒWƒF‚ÉƒJƒƒ‰‚ªŒü‚­
		Rock(pos, dir);
		//ƒŠƒ“ƒNƒIƒuƒWƒFƒNƒg‚Æ‚Ì”»’è
		LinkRayCheck(pos, dir);
		//ƒhƒ[ƒ“‚Æ‚Ì”»’è
		DroneRayCheck(pos, dir);
	}
	//---------------------------------------------------------------------------------------------
	//Ray‚ÆLinkƒIƒuƒWƒFƒNƒg‚ª“–‚½‚Á‚Ä‚¢‚é‚©‚ğ’²‚×‚é
	//---------------------------------------------------------------------------------------------
	void Player::LinkRayCheck(Vec3 origin,Vec3 originDir) {
		auto sightingDevice = m_SightingDevice.lock();
		//ƒŠƒ“ƒNƒIƒuƒWƒFƒNƒg‚Ì“ü‚Á‚Ä‚¢‚éƒOƒ‹[ƒv‚ğ‚Á‚Ä‚­‚é
		auto& linkGroup = GetStage()->GetSharedObjectGroup(L"Link");
		//ˆê‚Â‚¸‚Âæ‚èo‚·
		for (auto& link : linkGroup->GetGroupVector()) {
			auto linkObj = link.lock();
			auto linkTrans = linkObj->GetComponent<Transform>();
			//ƒŠƒ“ƒNƒIƒuƒWƒFƒNƒg‚ÌOBB‚ğì‚é
			OBB obb(linkTrans->GetScale() * 3, linkTrans->GetWorldMatrix());
			//ƒvƒŒƒCƒ„[‚©‚ç‚Å‚éRay‚ÆOBB‚Å”»’è
			bool hit = HitTest::SEGMENT_OBB(origin, origin + originDir * m_rayRange, obb);
			//ÅŒã‚ÉƒxƒWƒG‹Èü‚Å”ò‚ñ‚¾ƒŠƒ“ƒNƒIƒuƒWƒFƒNƒg‚¶‚á‚È‚¢‚à‚Ì‚É“–‚½‚Á‚Ä‚¢‚½‚ç
			if (hit && p2 + Vec3(0, -1, 0) != linkTrans->GetWorldPosition()) {
				//Æ€‚É“–‚½‚Á‚Ä‚¢‚é‚±‚Æ‚ğ‹³‚¦‚é
				sightingDevice->SetCaptureLink(true);
				if (m_pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
					Vec3 dir = GetComponent<Transform>()->GetWorldPosition() - linkTrans->GetWorldPosition();
					dir.y = 0;
					SetBezierPoint(linkTrans->GetWorldPosition() + dir.normalize());
					m_Lerp = 0;
					//ƒhƒ[ƒ“‚ª“ü‚Á‚½‚Ü‚Ü‚¾‚Æ‚»‚¿‚ç‚Ì‚Ù‚¤‚ÉŒü‚©‚Á‚Ä‚µ‚Ü‚¤‚Ì‚ÅNULL‚É‚·‚é
					m_DroneNo = NULL;
					m_StateMachine->ChangeState(LinkState::Instance());
					m_target = Target::LINK;
					break;
				}
			}
		}
	}
	//---------------------------------------------------------------------------------------------
	//Ray‚ÆDroneƒIƒuƒWƒFƒNƒg‚ª“–‚½‚Á‚Ä‚¢‚é‚©‚ğ’²‚×‚é
	//---------------------------------------------------------------------------------------------
	void Player::DroneRayCheck(Vec3 origin, Vec3 originDir) {
		auto sightingDevice = m_SightingDevice.lock();
		//ƒhƒ[ƒ“ƒIƒuƒWƒFƒNƒg‚Ì“ü‚Á‚Ä‚¢‚éƒOƒ‹[ƒv‚ğ‚Á‚Ä‚­‚é
		auto& droneGroup = GetStage()->GetSharedObjectGroup(L"Drone");
		//‰½”Ô–Ú‚ÌDrone‚©”‚¦‚é
		int count = 0;
		//ˆê‚Â‚¸‚Âæ‚èo‚·
		for (auto& drone : droneGroup->GetGroupVector()) {
			auto droneObj = drone.lock();
			auto droneTrans = droneObj->GetComponent<Transform>();
			//ƒhƒ[ƒ“ƒIƒuƒWƒFƒNƒg‚ÌOBB‚ğì‚é
			OBB obb(droneTrans->GetScale() * 3, droneTrans->GetWorldMatrix());
			//ƒvƒŒƒCƒ„[‚©‚ç‚Å‚éRay‚ÆOBB‚Å”»’è
			bool hit = HitTest::SEGMENT_OBB(origin, origin + originDir * 30.0f, obb);
			//ÅŒã‚ÉƒxƒWƒG‹Èü‚Å”ò‚ñ‚¾ƒIƒuƒWƒFƒNƒg‚¶‚á‚È‚¢‚à‚Ì‚É“–‚½‚Á‚Ä‚¢‚½‚ç
			if (hit && p2 + Vec3(0, -1, 0) != droneTrans->GetWorldPosition()) {
				//Æ€‚É“–‚½‚Á‚Ä‚¢‚é‚±‚Æ‚ğ‹³‚¦‚é
				sightingDevice->SetCaptureLink(true);
				if (m_pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
					SetBezierPoint(droneTrans->GetWorldPosition());
					m_DroneNo = count;
					m_Lerp = 0;
					m_StateMachine->ChangeState(LinkState::Instance());
					m_target = Target::DRONE;
					break;
				}
			}
			count++;
		}
	}
	//---------------------------------------------------------------------------------------------
	//‰Ÿ‚µo‚µ‚Ì”»’è
	//---------------------------------------------------------------------------------------------
	void Player::Extrusion(const weak_ptr<GameObject>& Other) {
		//player‚Ìî•ñ
		auto trans = GetComponent<Transform>();
		auto pos = trans->GetWorldPosition();
		auto scale = trans->GetScale();
		auto half = scale * 0.5f;
		//Õ“Ë‚µ‚½ƒIƒuƒWƒFƒNƒg‚Ìî•ñ
		auto other = Other.lock();
		auto otherTrans = other->GetComponent<Transform>();
		auto otherPos = otherTrans->GetWorldPosition();
		auto otherScale = otherTrans->GetScale();
		auto otherHalf = otherScale * 0.5f;
		// Õ“Ë‚µ‚Ä‚¢‚é‚Ì‚Å‰“š‚ğs‚¤
		float diff[6] = {
			(otherPos.x + otherHalf.x) - (pos.x - half.x), // ‰E
			(pos.x + half.x) - (otherPos.x - otherHalf.x), // ¶
			(otherPos.y + otherHalf.y) - (pos.y - half.y), // ã
			(pos.y + half.y) - (otherPos.y - otherHalf.y), // ‰º
			(otherPos.z + otherHalf.z) - (pos.z - half.z), // ‰œ
			(pos.z + half.z) - (otherPos.z - otherHalf.z), // è‘O
		};
		// ‘¤–Ê‚Ì‹——£‚ªÅ¬‚É‚È‚Á‚Ä‚¢‚é—v‘f‚ğŒ©‚Â‚¯‚é
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
	//Aƒ{ƒ^ƒ“‚ª‰Ÿ‚³‚ê‚½‚©‚Ç‚¤‚©‚ğ•Ô‚·
	//---------------------------------------------------------------------------------------------
	bool Player::CheckAButton() {
		if (m_pad.wPressedButtons & XINPUT_GAMEPAD_A) {
			return true;
		}
		return false;
	}

	//---------------------------------------------------------------------------------------------
	//ƒRƒ“ƒ{”‚É‰‚¶‚Äƒ{[ƒiƒX‚ğ—^‚¦‚é
	//---------------------------------------------------------------------------------------------
	void Player::ComboBonus(int nowChains)
	{
		m_energy += nowChains * 2.0f;
	}

	//---------------------------------------------------------------------------------------------
	//î•ñ‚Ì•\¦
	//---------------------------------------------------------------------------------------------
	void Player::DrawStrings(){
		// FPS‚Ìæ“¾
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
		//•¶š—ñ‚ğ‚Â‚¯‚é
		//wstring str = strFps + cameraStr + energy + combo + timeLimit;

		wstring str = combo + chainLimit + energy + timeLimit;
		auto ptrString = GetComponent<StringSprite>();
		ptrString->SetText(str);
	}

	//--------------------------------------------------------------------------------------
	//	class WalkState : public ObjState<Player>;
	//	—p“r: •à‚«ó‘Ô
	//--------------------------------------------------------------------------------------
	//ƒXƒe[ƒg‚ÌƒCƒ“ƒXƒ^ƒ“ƒXæ“¾
	shared_ptr<WalkState> WalkState::Instance() {
		static shared_ptr<WalkState> instance(new WalkState);
		return instance;
	}
	//ƒXƒe[ƒg‚É“ü‚Á‚½‚Æ‚«‚ÉŒÄ‚Î‚ê‚éŠÖ”
	void WalkState::Enter(const shared_ptr<Player>& Obj) {
		Obj->ChengeEnergyPur();
		Obj->ChangeWalkSpeed(Player::State::HUMAN);
	}
	//ƒXƒe[ƒgÀs’†‚É–ˆƒ^[ƒ“ŒÄ‚Î‚ê‚éŠÖ”
	void WalkState::Execute(const shared_ptr<Player>& Obj) {
		Obj->Walk();
		Obj->Fall();
		Obj->Response();
		Obj->SightingDeviceChangePosition();
		Obj->CameraControll();
		//‰EƒXƒeƒBƒbƒN‚Ì’l‚ÅƒJƒƒ‰‚Ì‰ñ“]ˆ—‚ğs‚¤
		Obj->CameraRoll();
		if (Obj->CheckAButton()) {
			Obj->GetStateMachine()->ChangeState(DataState::Instance());
			Obj->SightingDeviceDrawActive(true);
		}
	}
	//ƒXƒe[ƒg‚É‚©‚ç”²‚¯‚é‚Æ‚«‚ÉŒÄ‚Î‚ê‚éŠÖ”
	void WalkState::Exit(const shared_ptr<Player>& Obj) {
	}
	//--------------------------------------------------------------------------------------
	//	class WalkState : public ObjState<Player>;
	//	—p“r: ƒŠƒ“ƒNã‚ğ”ò‚ñ‚Å‚¢‚éó‘Ô
	//--------------------------------------------------------------------------------------
	//ƒXƒe[ƒg‚ÌƒCƒ“ƒXƒ^ƒ“ƒXæ“¾
	shared_ptr<LinkState> LinkState::Instance() {
		static shared_ptr<LinkState> instance(new LinkState);
		return instance;
	}

	const int CHAIN_TIMELIMIT = 180;	// ƒRƒ“ƒ{‚ª“rØ‚ê‚éŠÔ

	//ƒXƒe[ƒg‚É“ü‚Á‚½‚Æ‚«‚ÉŒÄ‚Î‚ê‚éŠÖ”
	void LinkState::Enter(const shared_ptr<Player>& Obj) {
		Obj->GetAnimStateMachine()->ChangeState(PlayerFlyAnim::Instance());
		auto pos = Obj->GetComponent<Transform>()->GetWorldPosition();
		auto camera = Obj->GetStage()->GetView()->GetTargetCamera();
		camera->SetAt(pos + Vec3(0.0f, 1.0f, 0.0f));
		camera->SetEye(pos + Vec3(0.0f, 1.5f, -10.0f));
		
		// ƒRƒ“ƒ{‚ğ‰ÁZ‚·‚é
		Obj->AddCombo();
		Obj->ComboBonus(Obj->GetChain());
		auto& gameManager = GameManager::GetInstance();
		//ƒXƒRƒA‚ğ‘‚â‚·
		gameManager.AddScore(20);
	}
	//ƒXƒe[ƒgÀs’†‚É–ˆƒ^[ƒ“ŒÄ‚Î‚ê‚éŠÖ”
	void LinkState::Execute(const shared_ptr<Player>& Obj) {
		//ƒXƒ[ó‘Ô‚È‚çAƒJƒƒ‰‚ÆƒŒƒC‚ğŠJ•ú‚·‚é
		if (GameManager::GetInstance().GetOnSlow()) {
			Obj->RayShot();
			Obj->CameraControll();
			Obj->CameraRoll();
		}
		else {
			Obj->PlayerRoll();
		}
		if (Obj->GetTarget() == Obj->LINK){
			Obj->LinkGo();
		}
		if (Obj->GetTarget() == Obj->DRONE) {
			Obj->DroneGo();
		}
		Obj->SightingDeviceChangePosition();
		if (Obj->GetEnergy() <= 0.0f) {
			Obj->GetStateMachine()->ChangeState(WalkState::Instance());
			Obj->SightingDeviceDrawActive(false);
		}
	}
	//ƒXƒe[ƒg‚É‚©‚ç”²‚¯‚é‚Æ‚«‚ÉŒÄ‚Î‚ê‚éŠÖ”
	void LinkState::Exit(const shared_ptr<Player>& Obj) {
	}
	//--------------------------------------------------------------------------------------
	//	class DataState : public ObjState<Player>;
	//	—p“r: ƒf[ƒ^‘Ìó‘Ô
	//--------------------------------------------------------------------------------------
	//ƒXƒe[ƒg‚ÌƒCƒ“ƒXƒ^ƒ“ƒXæ“¾
	shared_ptr<DataState> DataState::Instance() {
		static shared_ptr<DataState> instance(new DataState);
		return instance;
	}
	//ƒXƒe[ƒg‚É“ü‚Á‚½‚Æ‚«‚ÉŒÄ‚Î‚ê‚éŠÖ”
	void DataState::Enter(const shared_ptr<Player>& Obj) {
		//Obj->GetAnimStateMachine()->ChangeState(PlayerDefaultAnim::Instance());
		if (Obj->GetChain() >= 4)
		{
			// Œ»İ‚Ìƒ`ƒFƒCƒ“‚É‰‚¶‚Ä§ŒÀŠÔ‚ğİ’è
			Obj->SetComboChainLimit(Obj->GetFirstTime() / (Obj->GetChain() - 2));
		}
		else
		{
			Obj->SetComboChainLimit(Obj->GetFirstTime());
		}
		Obj->ChengeEnergyMai();
		Obj->ChangeWalkSpeed(Player::State::DATA);

		// ˆÈ‘O‚ÌƒXƒe[ƒg‚ªLinkState‚¾‚Á‚½‚ç
		if (Obj->GetStateMachine()->GetPreviousState() == LinkState::Instance())
		{
			// ƒ^ƒCƒ€ƒ`ƒFƒbƒJ[ƒtƒ‰ƒO‚ğƒIƒ“‚É‚·‚é
			Obj->SetAdvanceTimeActive(true);
		}
	}
	//ƒXƒe[ƒgÀs’†‚É–ˆƒ^[ƒ“ŒÄ‚Î‚ê‚éŠÖ”
	void DataState::Execute(const shared_ptr<Player>& Obj) {
		//// ƒRƒ“ƒ{ŠÔ‚ÌŠÔ‚ği‚ß‚é‚©‚Ç‚¤‚©
		//if (Obj->GetAdvanceTimeActive())
		//{
		//	// ŠÔ‚ğ‰ÁZ‚·‚é
		//	Obj->AddChainTime();
		//}
		//// Œ»İ‚ÌŠÔ‚ğæ“¾
		//int timeLimit = Obj->GetComboChainLimit();
		// §ŒÀŠÔ‚É’B‚µ‚½‚ç
		//if (Obj->GetChainTimeLim() >= Obj->GetComboChainLimit())
		//{
		//	
		//	// ƒRƒ“ƒ{‚ÌƒŠƒZƒbƒg
		//	Obj->ResetCombo();
		//	// §ŒÀŠÔ‚ğƒŠƒZƒbƒg
		//	Obj->ResetTimeLim();
		//	Obj->SetAdvanceTimeActive(false);
		//}
		Obj->Walk();
		Obj->RayShot();
		Obj->SightingDeviceChangePosition();
		//ƒJƒƒ‰§Œä‚Ì‚½‚ß‚Ì’l‚ğ•ÏX‚·‚é
		Obj->CameraControll();
		//‰EƒXƒeƒBƒbƒN‚Ì’l‚ÅƒJƒƒ‰‚Ì‰ñ“]ˆ—‚ğs‚¤
		Obj->CameraRoll();
		//ƒvƒŒƒCƒ„[‚Ì‘Ì‚ÌŒü‚«‚ğ•Ï‚¦‚é
		Obj->PlayerRoll();
		if (Obj->CheckAButton() || Obj->GetEnergy() <= 0.0f) {
			Obj->GetStateMachine()->ChangeState(WalkState::Instance());
			Obj->SightingDeviceDrawActive(false);
		}
	}
	//ƒXƒe[ƒg‚É‚©‚ç”²‚¯‚é‚Æ‚«‚ÉŒÄ‚Î‚ê‚éŠÖ”
	void DataState::Exit(const shared_ptr<Player>& Obj) {
		Obj->ResetTimeLim();
	}

}
//end basecross

