/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"


namespace basecross{
	//------------------------------------------------------------------------------------------------
	//playerのコンストラクタ
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
	//初期化
	//-------------------------------------------------------------------------------------------------
	void Player::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();

		ptrTrans->SetWorldPosition(m_position);
		ptrTrans->SetQuaternion(m_quaternion);
		ptrTrans->SetScale(m_scale);

		//四角形の当たり判定をセット
		auto col = AddComponent<CollisionObb>();
		//col->SetDrawActive(true);
		//コリジョンの判定をしない
		col->SetAfterCollision(AfterCollision::None);
		//playerに持たせて使うものには衝突しない
		col->AddExcludeCollisionTag(L"PlayerUse");

		//文字列をつける
		auto ptrString = AddComponent<StringSprite>();
		ptrString->SetText(L"");
		ptrString->SetTextRect(Rect2D<float>(16.0f, 120.0f, 640.0f, 480.0f));

		Mat4x4 spanMat; // モデルとトランスフォームの間の差分行列
		spanMat.affineTransformation(
			Vec3(0.8f, 0.8f, 0.8f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, Deg2Rad(180), 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		//描画コンポーネントの追加
		auto drawComp = AddComponent<PNTBoneModelDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		drawComp->SetMultiMeshResource(L"PLAYER_MODEL");
		drawComp->SetTextureResource(L"PLAYER_TX");
		drawComp->SetMultiMeshIsDraw(0, false);
		drawComp->SetMultiMeshIsDraw(1, false);
		drawComp->SetMultiMeshIsDraw(2, false);
		drawComp->SetMultiMeshIsDraw(FaceState::Smile, false);
		drawComp->SetMultiMeshIsDraw(FaceState::EyeClose, false);
		drawComp->SetMultiMeshIsDraw(FaceState::Regret, false);
		// 変換した行列を代入
		drawComp->SetMeshToTransformMatrix(spanMat);
		// アニメーションを追加する
		drawComp->AddAnimation(L"Default", 0, 110, true, 60.0f);
		drawComp->AddAnimation(L"Move", 200, 30, true, 30.0f);
		drawComp->AddAnimation(L"Fly", 310, 30, false, 17.0f);
		drawComp->AddAnimation(L"Over", 350, 40, true, 30.0f);
		drawComp->AddAnimation(L"Clear", 400, 50, true, 30.0f);
		//Col4 Color(1.0f, 0.2f, 1.0f, 0.7f);
		//drawComp->SetDiffuse(Color);
		//drawComp->SetColorAndAlpha(Color);
		// レイヤーの調整
		SetDrawLayer(-10);

		GetStage()->SetSharedGameObject(L"Player", GetThis<Player>());
		auto dev = GetStage()->AddGameObject<SightingDevice>();
		this->SetSightingDevice(dev);

		m_StateMachine.reset(new StateMachine<Player>(GetThis<Player>()));
		//最初のステートをDataStateに設定
		m_StateMachine->ChangeState(DataState::Instance());

		m_animStateMachine.reset(new StateMachine<Player>(GetThis<Player>()));
		m_animStateMachine->ChangeState(PlayerDefaultAnim::Instance());
	}
	//-------------------------------------------------------------------------------------------------------------
	//Update
	//-------------------------------------------------------------------------------------------------------------
	void Player::OnUpdate() {
		// アプリケーション取得
		auto& app = App::GetApp();
		// 1フレームの実行にかかった時間を取得
		float delta = app->GetElapsedTime();

		// ゲームコントローラー取得
		auto device = app->GetInputDevice();
		if (app->GetScene<Scene>()->GetGameStart()) {
			m_pad = GameManager::GetInstance().GetPad();
		}
		else {
			m_nowFallSpeed = 5.0f;
			Fall();
		}

		//左スティックの入力方向を求める
		m_padDir = Vec3(m_pad.fThumbLX, 0.0f, m_pad.fThumbLY);
		m_padDir = m_padDir.normalize();

		//ステートマシンのアップデート
		m_StateMachine->Update();

		m_animStateMachine->Update();
		// アニメーションを更新する
		auto drawComp = GetComponent<PNTBoneModelDraw>();
		drawComp->UpdateAnimation(delta);
	};

	//--------------------------------------------------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------------------------------------------------
	void Player::OnUpdate2() {
		Vec3 pos = GetComponent<Transform>()->GetWorldPosition();
		// 最小になっている方向に対して押し出しを行う
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

		// デバッグ文字の表示
		DrawStrings();
		CheckYButton();
	}
	//--------------------------------------------------------------------------------------------------------------
	//衝突したとき
	//--------------------------------------------------------------------------------------------------------------
	void Player::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		auto trans = GetComponent<Transform>();
		auto otherTrans = Other->GetComponent<Transform>();
		//playerの下面と衝突した物体の上面が当たっていたら
		if (((otherTrans->GetWorldPosition().y + otherTrans->GetScale().y / 2) - (trans->GetWorldPosition().y - trans->GetScale().y / 2)) < 0.5f) {
			//自動でY方向に力を加える処理を行わないようにする
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
	//衝突し続けているとき
	//--------------------------------------------------------------------------------------------------------------
	void Player::OnCollisionExcute(shared_ptr<GameObject>& Other) {
		auto trans = GetComponent<Transform>();
		auto otherTrans = Other->GetComponent<Transform>();
		//playerの下面と衝突した物体の上面が当たっていたら
		if (((otherTrans->GetWorldPosition().y + otherTrans->GetScale().y / 2) - (trans->GetWorldPosition().y - trans->GetScale().y / 2)) < 0.5f) {
			//自動でY方向に力を加える処理を行わないようにする
			m_isFall = false;
		}
		Extrusion(Other);

	}
	//--------------------------------------------------------------------------------------------------------------
	//衝突が解除されたとき
	//-------------------------------------------------------------------------------------------------------------
	void Player::OnCollisionExit(shared_ptr<GameObject>& Other) {
		auto wall = dynamic_pointer_cast<Wall>(Other);
		if (wall) {
			auto trans = GetComponent<Transform>();
			auto wallTrans = wall->GetComponent<Transform>();
			//playerの下面と衝突した物体の上面が当たっていたら
			if (((wallTrans->GetWorldPosition().y + wallTrans->GetScale().y / 2) - (trans->GetWorldPosition().y - trans->GetScale().y / 2)) < 0.5f) {
				m_response = wallTrans->GetWorldPosition() + Vec3(0.0f, 5.0f, 0.0f);
				m_response.y += wallTrans->GetScale().y / 2;
			}
		}
	}

																	
	//--------------------------------------------------------------------------------------------
	//XZ平面の移動処理
	//--------------------------------------------------------------------------------------------
	void Player::Walk() {
		auto playerTrans = GetComponent<Transform>();
		auto playerPos = playerTrans->GetWorldPosition();
		//左スティックに値が入力されていたら
		if (m_padDir.x > 0.4f || m_padDir.x < -0.4f ||
			m_padDir.z > 0.4f || m_padDir.z < -0.4f) {
			//方向と移動スピードを掛け算してpositonを変更する
			playerPos.x += m_nowWalkSpeed * m_forward.x * App::GetApp()->GetElapsedTime();
			playerPos.z += m_nowWalkSpeed * m_forward.z * App::GetApp()->GetElapsedTime();
			// 現在のアニムステートがDefaultだったら
			if (GetAnimStateMachine()->GetCurrentState() == PlayerDefaultAnim::Instance())
			{
				GetAnimStateMachine()->ChangeState(PlayerMoveAnim::Instance());
			}
		}
		// padが入力されてないかつ現在のアニムステートがMoveだったら
		else if(m_padDir.x == 0.0f && m_padDir.z == 0.0f && 
				GetAnimStateMachine()->GetCurrentState() == PlayerMoveAnim::Instance())
		{
			GetAnimStateMachine()->ChangeState(PlayerDefaultAnim::Instance());
		}
		playerTrans->SetWorldPosition(playerPos);
	}
	//---------------------------------------------------------------------------------------------
	//Y方向の移動処理
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
	//プレイヤーを向かう方向に回転させる
	//---------------------------------------------------------------------------------------------
	void Player::PlayerRoll() {
		auto ptrUtil = GetBehavior<UtilBehavior>();
		ptrUtil->RotToHead(m_forward, 0.1f);
	}
	//--------------------------------------------------------------------------------------------
	//平行移動の速度をステートで分けて変更する
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
	//設定高さ以下になったらリスポーン位置にワープさせる
	//--------------------------------------------------------------------------------------------
	void Player::Response() {
		if (GetComponent<Transform>()->GetWorldPosition().y <= m_responseHeght) {
			GetComponent<Transform>()->SetWorldPosition(m_response);
			//復帰後にすぐ動けるように少し回復させる
			m_changeEnergy = 20.0f;
		}
	}
	//--------------------------------------------------------------------------------------------
	//カメラの回転処理
	//--------------------------------------------------------------------------------------------
	void Player::CameraRoll() {
		float delta = App::GetApp()->GetElapsedTime();

		//右スティックに値が入力されていたら
		if (m_pad.fThumbRX > 0.7f || m_pad.fThumbRX < -0.7f ||
			m_pad.fThumbRY > 0.7f || m_pad.fThumbRY < -0.7f) {
			m_angleY += -m_pad.fThumbRX * m_maxAngleSpeed*2 * delta; // カメラを回転させる
			m_angleX += -m_pad.fThumbRY * m_maxAngleSpeed*2 * delta; // カメラを昇降させる
		}
		else if(m_pad.fThumbRX > 0.2f || m_pad.fThumbRX < -0.2f ||
			m_pad.fThumbRY > 0.2f || m_pad.fThumbRY < -0.2f) {
			m_angleY += -m_pad.fThumbRX * m_maxAngleSpeed * delta; // カメラを回転させる
			m_angleX += -m_pad.fThumbRY * m_maxAngleSpeed * delta; // カメラを昇降させる
		}
		//Y軸基準角度の丸め(-360<360)
		if (m_angleY > Deg2Rad(360.0f)) {
			m_angleY = Deg2Rad(0.0f);
		}
		if (m_angleY < Deg2Rad(0.0f)) {
			m_angleY = Deg2Rad(360.0f);
		}
		//X軸基準角度の丸め(-70<70)
		if (m_angleX > Deg2Rad(m_angeleXMax)) {
			m_angleX = Deg2Rad(m_angeleXMax);
		}
		if (m_angleX < -Deg2Rad(m_angeleXMax)) {
			m_angleX = -Deg2Rad(m_angeleXMax);
		}

		// スティックの傾きを角度に変換する
		float padRad = atan2f(m_padDir.z, m_padDir.x);
		if (m_padDir.length() != 0.0f) {
			// スティックの角度にカメラの角度を加える
			padRad += m_angleY + XM_PIDIV2;  // カメラの角度に９０度加える
			// 角度をXZ成分に分解してベクトルを作り直す
			m_padDir.x = cosf(padRad); // 新しい角度を X 成分に分解する
			m_padDir.z = sinf(padRad); // 新しい角度を Z 成分に分解する

			m_forward = m_padDir;
		}
	}
	//---------------------------------------------------------------------------------------------
	//カメラのプレイヤー追従処理
	//---------------------------------------------------------------------------------------------
	void Player::CameraControll(){
		auto sightPos = m_SightingDevice.lock()->GetComponent<Transform>()->GetWorldPosition();
		auto camera = GetStage()->GetView()->GetTargetCamera();
		//照準を見る
		camera->SetAt(sightPos);
		auto eye = sightPos + Vec3(cos(m_angleY) * m_cameraDistance,
			sin(m_angleX) * m_cameraDistance, sin(m_angleY) * m_cameraDistance);
		camera->SetEye(eye);
	}
	//---------------------------------------------------------------------------------------------
	//Lボタンを押しているときにリンクオブジェが照準の近くだったらそっちを向く
	//---------------------------------------------------------------------------------------------
	void Player::Rock(Vec3 origin, Vec3 originDir, wstring groupName, float correction) {
		//Lボタンを押し続けているとき
		if (GameManager::GetInstance().GetPad().wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
			if (!m_islockon) {
				RockonObject(origin, originDir, groupName, correction);
			}
		}
		//Lボタンを押していないとき
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
		//リンクオブジェクトを照準にとらえてロックオンしているとき
		if (m_islockon) {
			//カメラの制御をおこなう
			auto sightingDevicePos = m_SightingDevice.lock()->GetComponent<Transform>()->GetWorldPosition();
			auto linkTrans = m_LockOnObj.lock()->GetComponent<Transform>();
			float deltaX = sightingDevicePos.x - linkTrans->GetWorldPosition().x;
			float deltaZ = sightingDevicePos.z - linkTrans->GetWorldPosition().z;
			float deltaY = sightingDevicePos.y - linkTrans->GetWorldPosition().y;
			//横のカメラ位置を制御する角度
			m_angleY = atan2f(deltaZ, deltaX);
			if (m_angleY < 0.0f) {
				m_angleY += Deg2Rad(360.0f);
			}
			//縦のカメラ位置をだすための辺出す
			float syahen = hypotf(deltaX, deltaZ);
			//縦のカメラ位置を制御する角度
			m_angleX = atan2f(deltaY, syahen);
		}
	}

	///---------------------------------------------------------------------------------------------
	//ロックオンするオブジェクトを設定
	///---------------------------------------------------------------------------------------------
	void Player::RockonObject(Vec3 origin, Vec3 originDir, wstring groupName, float correction)
	{
		//リンクオブジェクトの入っているグループを持ってくる
		auto& objectsGroup = GetStage()->GetSharedObjectGroup(groupName);
		//一つずつ取り出す
		for (auto& object : objectsGroup->GetGroupVector()) {
			auto lockonObj = object.lock();
			auto mail = dynamic_pointer_cast<MailObject>(lockonObj);
			//メールとの判定をしていて、メールが取得状態だったらスルーする
			if (mail) {
				if (mail->GetIsArrive()) {
					break;
				}
			}
			auto objTrans = lockonObj->GetComponent<Transform>();
			//リンクオブジェクトのOBBを作る
			OBB obb(objTrans->GetScale() * correction, objTrans->GetWorldMatrix());
			//照準からでるRayとOBBで判定
			bool hit = HitTest::SEGMENT_OBB(origin, origin + originDir * m_rayRange, obb);
			if (hit) {
				Vec3 delta = GetComponent<Transform>()->GetWorldPosition() - objTrans->GetWorldPosition();
				//近いときはロックオンしない
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
	//リンクへ飛ぶ処理
	//---------------------------------------------------------------------------------------------
	void Player::LinkGo() {
		auto pos = GetComponent<Transform>()->GetWorldPosition();
		auto& gameManager = GameManager::GetInstance();
		//計算のための時間加算
		//移動の経過にチェイン数による加速をいれた
		auto addLerp = (App::GetApp()->GetElapsedTime() * (m_BezierSpeed + m_chain)) / (m_BezierSpeedLeap);
		//スロー状態かどうかで処理
		if (gameManager.GetOnSlow()) {
			//20分の1の更新速度にする
			m_Lerp += addLerp / gameManager.GetSlowSpeed();
			//マネージャーにスローの経過を伝える
			gameManager.AddSlowPassage((addLerp / gameManager.GetSlowSpeed()) * 10.0f);
		}
		else {
			m_Lerp += addLerp;
		}
		//着地寸前でスローになっていない
		if (m_Lerp >= 0.9f && gameManager.GetOnSlow() == false) {
			//スローにする
			gameManager.SetOnSlow(true);
		}
		//飛ぶ処理が終わったら
		if (gameManager.GetSlowPassage() >= 1.0f) {
			m_Lerp = 1.0f;
			//スローを終わる
			gameManager.SetOnSlow(false);
			//スコアを増やす
			gameManager.AddScore(GetChain() * 20 + GetChain() * 10);
			//軌跡を消す
			m_ActionLine.lock()->SetOnDraw(false);
			//次のリンクへ飛べなかったのでコンボリセットする
			ResetCombo();
			//飛び終わったらステートをデータ体にする
			m_StateMachine->ChangeState(DataState::Instance());
		}
		//ベジエ曲線の計算
		pos = (1 - m_Lerp) * (1 - m_Lerp) * p0 + 2 * (1 - m_Lerp) * m_Lerp * p1 + m_Lerp * m_Lerp * p2;
		GetComponent<Transform>()->SetWorldPosition(pos);
		if (!gameManager.GetOnSlow()) {
			////プレイヤーと一緒に動くためにプレイヤーのLeapでカメラを動かす
			//auto camera = GetStage()->GetView()->GetTargetCamera();
			//dynamic_pointer_cast<TpsCamera>(camera)->BezierMove(m_Lerp, pos);
		}
	}
	//---------------------------------------------------------------------------------------------
	//ドローンへ飛ぶ処理
	//---------------------------------------------------------------------------------------------
	void Player::DroneGo() {
		auto& gameManager = GameManager::GetInstance();
		auto pos = GetComponent<Transform>()->GetWorldPosition();
		auto droneGroup = GetStage()->GetSharedObjectGroup(L"Drone");
		auto drone = dynamic_pointer_cast<Drone>(droneGroup->at(m_DroneNo));
		//計算のための時間加算
		//移動の経過にチェイン数による加速をいれた
		auto addLerp = (App::GetApp()->GetElapsedTime() * (m_BezierSpeed + m_chain)) / m_BezierSpeedLeap;
		//経過に加算
		m_Lerp += addLerp;
		//動いているので終点の位置を更新する
		p2 = drone->GetComponent<Transform>()->GetWorldPosition();
		//飛ぶ処理が終わった
		if (m_Lerp >= 1.0f && m_DroneNo != NULL) {
			m_Lerp = 1.0f;
			//コンボリセットする
			ResetCombo();
			//飛ぶ対象がいる状態のオフ
			ResetGoLink();
			//向かっているドローンの初期化
			m_DroneNo = NULL;
			//飛び終わったらステートをデータ体にする
			m_StateMachine->ChangeState(DataState::Instance());
		}
		//ベジエ曲線の計算
		pos = (1 - m_Lerp) * (1 - m_Lerp) * p0 + 2 * (1 - m_Lerp) * m_Lerp * p1 + m_Lerp * m_Lerp * p2;
		GetComponent<Transform>()->SetWorldPosition(pos);
		////カメラを追尾させる
		//auto camera = GetStage()->GetView()->GetTargetCamera();
		//dynamic_pointer_cast<TpsCamera>(camera)->BezierMove(m_Lerp, pos);
	}
	//---------------------------------------------------------------------------------------------
	//チェックポイントへ飛ぶ処理
	//---------------------------------------------------------------------------------------------
	void Player::CheckPointGo()
	{
		auto& gameManager = GameManager::GetInstance();
		auto pos = GetComponent<Transform>()->GetWorldPosition();
		auto checkPointGroup = GetStage()->GetSharedObjectGroup(L"CheckPoints");
		auto checkPoint = dynamic_pointer_cast<CheckPoint>(checkPointGroup->at(m_checkPointNum));
		//計算のための時間加算
		//移動の経過にチェイン数による加速をいれた
		auto addLerp = (App::GetApp()->GetElapsedTime() * (m_BezierSpeed + m_chain)) / (m_BezierSpeedLeap);
		//スロー状態かどうかで処理
		if (gameManager.GetOnSlow()) {
			//マネージャーにスローの経過を伝える
			gameManager.AddSlowPassage((addLerp * 10 / gameManager.GetSlowSpeed()));
			if (gameManager.GetSlowPassage() >= 1.0f) {
				//スローを終わる
				gameManager.SetOnSlow(false);
				//向かっているドローンの初期化
				m_checkPointNum = NULL;
				//次の目標へ飛べなかったのでコンボリセット
				ResetCombo();
				//スロー時間が終了したためステートをデータ体にする
				m_StateMachine->ChangeState(DataState::Instance());
			}
		}
		else {
			//経過に加算
			m_Lerp += addLerp;
			//動いているので終点の位置を更新する
			p2 = checkPoint->GetComponent<Transform>()->GetWorldPosition();
		}
		//飛ぶ処理が終わったあとで、スロー状態でなければ
		if (m_Lerp >= 1.0f && gameManager.GetOnSlow() == false && m_checkPointNum != NULL) {
			m_Lerp = 1.0f;
			// 対象のチェックポイントがfalseだったら
			if (!checkPoint->GetIsArrive()) {
				//演出でチェイン数を飛ばすために値を与える
				GetStage()->GetSharedGameObject<FlyingChain>(L"FlyingChain")->FlySet(GetChain());
				// フラグを立てる
				checkPoint->ArriveCheckPoint();
				//スローの経過時間をリセット
				gameManager.ResetSloawPassage();
				//スローにする
				gameManager.SetOnSlow(true);
			}
			else 
			{
				//スロー時間が終了したためステートをデータ体にする
				m_StateMachine->ChangeState(DataState::Instance());
			}
		}
		//ベジエ曲線の計算
		pos = (1 - m_Lerp) * (1 - m_Lerp) * p0 + 2 * (1 - m_Lerp) * m_Lerp * p1 + m_Lerp * m_Lerp * p2;
		GetComponent<Transform>()->SetWorldPosition(pos);
		if (!gameManager.GetOnSlow()) {
			//プレイヤーと一緒に動くためにプレイヤーのLeapでカメラを動かす
			auto camera = GetStage()->GetView()->GetTargetCamera();
			dynamic_pointer_cast<TpsCamera>(camera)->BezierMove(m_Lerp, pos);
		}

	}
	//---------------------------------------------------------------------------------------------
	//メールへ飛ぶ処理
	//---------------------------------------------------------------------------------------------
	void Player::MailGo(){
		auto& gameManager = GameManager::GetInstance();
		auto pos = GetComponent<Transform>()->GetWorldPosition();
		auto mailGroup = GetStage()->GetSharedObjectGroup(L"Mails");
		auto mail = dynamic_pointer_cast<MailObject>(mailGroup->at(m_MailNum));
		//計算のための時間加算
		//移動の経過にチェイン数による加速をいれた
		auto addLerp = (App::GetApp()->GetElapsedTime() * (m_BezierSpeed + m_chain)) / (m_BezierSpeedLeap);
		//スロー状態かどうかで処理
		if (gameManager.GetOnSlow()) {
			//マネージャーにスローの経過を伝える
			gameManager.AddSlowPassage((addLerp * 10 / gameManager.GetSlowSpeed()));
			if (gameManager.GetSlowPassage() >= 1.0f) {
				//スローを終わる
				gameManager.SetOnSlow(false);
				//次の目標へ飛べなかったのでコンボリセット
				ResetCombo();
				//軌跡を消す
				m_ActionLine.lock()->SetDrawActive(false);
				//スロー時間が終了したためステートをデータ体にする
				m_StateMachine->ChangeState(DataState::Instance());
			}
		}
		else {
			//経過に加算
			m_Lerp += addLerp;
		}
		//飛ぶ処理が終わったあとで、スロー状態でなければ
		if (m_Lerp >= 1.0f && gameManager.GetOnSlow() == false && mail->GetIsArrive() != true) {
			m_Lerp = 1.0f;
			//メールを取得状態にする
			mail->ArriveMail();
			//演出でチェイン数を飛ばすために値を与える
			GetStage()->GetSharedGameObject<FlyingChain>(L"FlyingChain")->FlySet(GetChain());
			//スローの経過時間をリセット
			gameManager.ResetSloawPassage();
			//スローにする
			gameManager.SetOnSlow(true);
		}
		//ベジエ曲線の計算
		pos = (1 - m_Lerp) * (1 - m_Lerp) * p0 + 2 * (1 - m_Lerp) * m_Lerp * p1 + m_Lerp * m_Lerp * p2;
		GetComponent<Transform>()->SetWorldPosition(pos);
		//if (!gameManager.GetOnSlow()) {
		//	//プレイヤーと一緒に動くためにプレイヤーのLeapでカメラを動かす
		//	auto camera = GetStage()->GetView()->GetTargetCamera();
		//	dynamic_pointer_cast<TpsCamera>(camera)->BezierMove(m_Lerp, pos);
		//}

	}
	//---------------------------------------------------------------------------------------------
	//ベジエ曲線の制御点設定
	//---------------------------------------------------------------------------------------------
	void Player::SetBezierPoint(Vec3 point) {

		p0 = GetComponent<Transform>()->GetWorldPosition();
		p2 = point + Vec3(0,1.0f,0);
		//飛ぶ先までの距離に応じて飛ぶ際のスピードを変える
		m_BezierSpeedLeap = Vec3(p2 - p0).length();
		//飛ぶ先までの距離に応じて飛ぶ際の放物線の形を変える
		if (m_BezierSpeedLeap >= 20.0f) {
			p1 = point + Vec3(0, 6, 0);
		}
		else if(m_BezierSpeedLeap >= 10.0f){
			p1 = point + Vec3(0, 3, 0);
		}
		else if(m_BezierSpeedLeap < 10.0f){
			p1 = point - ((point - p0) / 2);
		}
		//飛んだ際にリスポーン位置の更新も行う
		m_response = p2;
		//カメラの追従する動きを設定する
		auto camera = GetStage()->GetView()->GetTargetCamera();
		dynamic_pointer_cast<TpsCamera>(camera)->SetBezier(GetThis<Player>(), p2);
		//次のリンクに移るのでスローを解く
		GameManager::GetInstance().SetOnSlow(false);
		//プレイヤーの正面向き飛ぶ方向にする
		m_forward = Vec3(p2 - p0).normalize();
		//リンクオブジェのロックオンを解除
		m_islockon = false;
	}
	//---------------------------------------------------------------------------------------------
	//照準の位置をカメラとプレイヤーの位置から求め変更する
	//---------------------------------------------------------------------------------------------
	void Player::SightingDeviceChangePosition() {
		auto pos = GetComponent<Transform>()->GetWorldPosition();
		auto m_cameraPos = GetStage()->GetView()->GetTargetCamera()->GetEye();
		//playerとカメラの位置から照準がある方向を求める
		//playerの少し上のほうに置くためplayerのpositionに調整を入れる
		auto dir = (pos + Vec3(0.0f, 5.0f, 0.0f)) - m_cameraPos;
		dir = dir.normalize();

		auto sightingDevice = m_SightingDevice.lock();
		//playerの頭辺りに、被らないようカメラからの方向を加味して置く
		sightingDevice->GetComponent<Transform>()->SetWorldPosition((pos + Vec3(0.0f, m_cameraLookUp, 0.0f)));

		float syahen = hypotf(dir.x, dir.z);
		Quat rot;
		rot.rotationRollPitchYawFromVector(Vec3(-atan2f(dir.y,syahen),atan2f(dir.x, dir.z), 0.0f));
		
		sightingDevice->GetComponent<Transform>()->SetQuaternion(rot);
	}
	//---------------------------------------------------------------------------------------------
	//Rayを飛ばす
	//---------------------------------------------------------------------------------------------
	void Player::RayShot() {
		auto sightingDevice = m_SightingDevice.lock();
		//Rayが当たったフラグをfalseに戻す
		sightingDevice->ResetCaptureLink();
		//Rayの射程を設定、コンボ数で伸びる
		m_rayRange = m_rayRangeDefolt + (m_chain * 0.5f);

		auto sightPos = sightingDevice->GetComponent<Transform>()->GetWorldPosition();
		auto m_cameraPos = GetStage()->GetView()->GetTargetCamera()->GetEye();
		//照準とカメラの位置から飛ばす方向を求める
		auto dir = sightPos - m_cameraPos;
		dir = dir.normalize();
		//少し大きめの判定をとって、Lを押して当たっていた場合リンクオブジェにカメラが向く
		Rock(sightPos, dir, L"Link", 3.0f);
		Rock(sightPos, dir, L"CheckPoints", 2.0f);
		Rock(sightPos, dir, L"Mails", 3.0f);
		//ドローンとの判定
		DroneRayCheck(sightPos, dir);
		//リンクオブジェクトとの判定
		LinkRayCheck(sightPos, dir);
		// チェックポイントとの判定
		CheckPointsRayCheck(sightPos, dir);
		//メールとの判定
		MailRayCheck(sightPos, dir);
	}
	//---------------------------------------------------------------------------------------------
	//Rayを可視化する	
	//---------------------------------------------------------------------------------------------
	void Player::RayView(Vec3 origin, Vec3 end) {
		auto ptrActionLine = m_ActionLine.lock();
		if (ptrActionLine) {
			ptrActionLine->ResetObject(origin,end);
			ptrActionLine->SetOnDraw(true);
		}
		else {
			//ラインの作成
			auto ptrLine = GetStage()->AddGameObject<ActionLine>(origin, end);
			m_ActionLine = ptrLine;
			ptrLine->SetDrawActive(true);
		}

	}
	//---------------------------------------------------------------------------------------------
	//RayとLinkオブジェクトが当たっているかを調べる
	//---------------------------------------------------------------------------------------------
	void Player::LinkRayCheck(Vec3 origin,Vec3 originDir) {
		//すでに他のものに飛んでいたら
		if (m_isGoLink) {
			return;
		}
		auto sightingDevice = m_SightingDevice.lock();
		//リンクオブジェクトの入っているグループを持ってくる
		auto& linkGroup = GetStage()->GetSharedObjectGroup(L"Link");
		//一つずつ取り出す
		for (auto& link : linkGroup->GetGroupVector()) {
			auto linkObj = link.lock();
			auto linkTrans = linkObj->GetComponent<Transform>();
			//リンクオブジェクトのOBBを作る
			OBB obb(linkTrans->GetScale() * 3, linkTrans->GetWorldMatrix());
			//プレイヤーからでるRayとOBBで判定
			bool hit = HitTest::SEGMENT_OBB(origin, origin + originDir * m_rayRange, obb);
			//最後にベジエ曲線で飛んだリンクオブジェクトじゃないものに当たっていたら
			if (hit && p2 + Vec3(0, -1, 0) != linkTrans->GetWorldPosition()) {
				//照準に当たっていることを教える
				sightingDevice->SetCaptureLink(true);
				if (m_pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
					Vec3 dir = GetComponent<Transform>()->GetWorldPosition() - linkTrans->GetWorldPosition();
					dir.y = 0;
					//オブジェクトに被らないように方向を加味してずらした値を渡す
					SetBezierPoint(linkTrans->GetWorldPosition() + dir.normalize());
					//軌跡
					RayView(origin, linkTrans->GetWorldPosition() + dir.normalize());
					//経過をリセット
					m_Lerp = 0;
					//飛ぶの確定
					m_isGoLink = true;
					//スローの経過時間をリセット
					GameManager::GetInstance().ResetSloawPassage();
					//ドローンが入ったままだとそちらのほうに向かってしまうのでNULLにする
					m_DroneNo = NULL;
					m_StateMachine->ChangeState(LinkState::Instance());
					m_target = Target::LINK;
					break;
				}
			}
		}
	}
	//---------------------------------------------------------------------------------------------
	//RayとDroneオブジェクトが当たっているかを調べる
	//---------------------------------------------------------------------------------------------
	void Player::DroneRayCheck(Vec3 origin, Vec3 originDir) {
		//すでに他のものに飛んでいたら
		if (m_isGoLink) {
			return;
		}
		auto sightingDevice = m_SightingDevice.lock();
		//ドローンオブジェクトの入っているグループを持ってくる
		auto& droneGroup = GetStage()->GetSharedObjectGroup(L"Drone");
		//何番目のDroneか数える
		int count = 0;
		//一つずつ取り出す
		for (auto& drone : droneGroup->GetGroupVector()) {
			auto droneObj = drone.lock();
			auto droneTrans = droneObj->GetComponent<Transform>();
			//ドローンオブジェクトのOBBを作る
			OBB obb(droneTrans->GetScale() * 3, droneTrans->GetWorldMatrix());
			//プレイヤーからでるRayとOBBで判定
			bool hit = HitTest::SEGMENT_OBB(origin, origin + originDir * 30.0f, obb);
			//最後にベジエ曲線で飛んだオブジェクトじゃないものに当たっていたら
			if (hit && p2 + Vec3(0, -1, 0) != droneTrans->GetWorldPosition()) {
				//照準に当たっていることを教える
				sightingDevice->SetCaptureLink(true);
				if (m_pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
					SetBezierPoint(droneTrans->GetWorldPosition());
					m_DroneNo = count;
					if (m_ActionLine.lock()) {
						m_ActionLine.lock()->SetDrawActive(false);
					}
					//経過をリセット
					m_Lerp = 0;
					//飛ぶの確定
					m_isGoLink = true;
					//スローの経過時間をリセット
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
	//RayとCheckPointオブジェクトが当たっているかを調べる
	//---------------------------------------------------------------------------------------------
	void Player::CheckPointsRayCheck(Vec3 origin, Vec3 originDir){
		//すでに他のものに飛んでいたら
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
			//プレイヤーからでるRayとOBBで判定
			bool hit = HitTest::SEGMENT_OBB(origin, origin + originDir * m_rayRange, obb);
			//最後にベジエ曲線で飛んだリンクオブジェクトじゃないものに当たっていたら
			if (hit && p2 + Vec3(0, -1, 0) != pointTrans->GetWorldPosition()) {
				//照準に当たっていることを教える
				sightingDevice->SetCaptureLink(true);
				if (m_pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
					Vec3 dir = GetComponent<Transform>()->GetWorldPosition() - pointTrans->GetWorldPosition();
					dir.y = 0;
					//オブジェクトに被らないように方向を加味してずらした値を渡す
					SetBezierPoint(pointTrans->GetWorldPosition() + dir.normalize());
					m_checkPointNum = count;
					//軌跡
					RayView(origin, pointTrans->GetWorldPosition() + dir.normalize());
					m_Lerp = 0;
					//ドローンが入ったままだとそちらのほうに向かってしまうのでNULLにする
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
	//RayとMailオブジェクトが当たっているかを調べる
	//---------------------------------------------------------------------------------------------
	void Player::MailRayCheck(Vec3 origin, Vec3 originDir) {
		//すでに他のものに飛んでいたら
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
				//プレイヤーからでるRayとOBBで判定
				bool hit = HitTest::SEGMENT_OBB(origin, origin + originDir * m_rayRange, obb);
				//最後にベジエ曲線で飛んだリンクオブジェクトじゃないものに当たっていたら
				if (hit && p2 + Vec3(0, -1, 0) != mailTrans->GetWorldPosition()) {
					//照準に当たっていることを教える
					sightingDevice->SetCaptureLink(true);
					if (m_pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
						Vec3 dir = GetComponent<Transform>()->GetWorldPosition() - mailTrans->GetWorldPosition();
						dir.y = 0;
						//オブジェクトに被らないように方向を加味してずらした値を渡す
						SetBezierPoint(mailTrans->GetWorldPosition() + dir.normalize());
						//軌跡
						RayView(origin, mailTrans->GetWorldPosition() + dir.normalize());
						//何番のメールにアクセスしているか保存
						m_MailNum = count;
						m_Lerp = 0;
						//飛ぶの確定
						m_isGoLink = true;
						//ドローンが入ったままだとそちらのほうに向かってしまうのでNULLにする
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
	//押し出しの判定
	//---------------------------------------------------------------------------------------------
	void Player::Extrusion(const weak_ptr<GameObject>& Other) {
		//playerの情報
		auto trans = GetComponent<Transform>();
		auto pos = trans->GetWorldPosition();
		auto scale = trans->GetScale();
		auto half = scale * 0.5f;
		//衝突したオブジェクトの情報
		auto other = Other.lock();
		auto otherTrans = other->GetComponent<Transform>();
		auto otherPos = otherTrans->GetWorldPosition();
		auto otherScale = otherTrans->GetScale();
		auto otherHalf = otherScale * 0.5f;
		// 衝突しているので応答を行う
		float diff[6] = {
			(otherPos.x + otherHalf.x) - (pos.x - half.x), // 右
			(pos.x + half.x) - (otherPos.x - otherHalf.x), // 左
			(otherPos.y + otherHalf.y) - (pos.y - half.y), // 上
			(pos.y + half.y) - (otherPos.y - otherHalf.y), // 下
			(otherPos.z + otherHalf.z) - (pos.z - half.z), // 奥
			(pos.z + half.z) - (otherPos.z - otherHalf.z), // 手前
		};
		// 側面の距離が最小になっている要素を見つける
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
	//Aボタンが押されたかどうかを返す
	//---------------------------------------------------------------------------------------------
	bool Player::CheckAButton() {
		if (m_pad.wPressedButtons & XINPUT_GAMEPAD_A) {
			return true;
		}
		return false;
	}

	// Yボタンが押されたかどうかを返す
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
	//情報の表示
	//---------------------------------------------------------------------------------------------
	void Player::DrawStrings(){
		// FPSの取得
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

		//文字列をつける
		//wstring str = strFps + cameraStr + energy + combo + timeLimit;

		wstring str = strFps + faceNum + checkNum;
		auto ptrString = GetComponent<StringSprite>();
		ptrString->SetText(str);
	}

	//--------------------------------------------------------------------------------------
	//	class WalkState : public ObjState<Player>;
	//	用途: 歩き状態
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	shared_ptr<WalkState> WalkState::Instance() {
		static shared_ptr<WalkState> instance(new WalkState);
		return instance;
	}
	//ステートに入ったときに呼ばれる関数
	void WalkState::Enter(const shared_ptr<Player>& Obj) {
		Obj->ChengeEnergyPur();
		Obj->ChangeWalkSpeed(Player::State::HUMAN);
	}
	//ステート実行中に毎ターン呼ばれる関数
	void WalkState::Execute(const shared_ptr<Player>& Obj) {
		Obj->Walk();
		Obj->Fall();
		Obj->Response();
		Obj->SightingDeviceChangePosition();
		Obj->CameraControll();
		//右スティックの値でカメラの回転処理を行う
		Obj->CameraRoll();
		if (Obj->CheckAButton()) {
			Obj->GetStateMachine()->ChangeState(DataState::Instance());
			Obj->SightingDeviceDrawActive(true);
		}
	}
	//ステートにから抜けるときに呼ばれる関数
	void WalkState::Exit(const shared_ptr<Player>& Obj) {
	}
	//--------------------------------------------------------------------------------------
	//	class LinkState : public ObjState<Player>;
	//	用途: リンク上を飛んでいる状態
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	shared_ptr<LinkState> LinkState::Instance() {
		static shared_ptr<LinkState> instance(new LinkState);
		return instance;
	}


	//ステートに入ったときに呼ばれる関数
	void LinkState::Enter(const shared_ptr<Player>& Obj) {
		Obj->FaceChanger(Obj->m_faceNum, Player::FaceState::Smile);
		auto scenePtr = App::GetApp()->GetScene<Scene>();
		// 効果音を鳴らす
		scenePtr->MusicOnceStart(L"Jump_SE", 1.0f);
		Obj->GetAnimStateMachine()->ChangeState(PlayerFlyAnim::Instance());
		auto pos = Obj->GetComponent<Transform>()->GetWorldPosition();
		auto camera = Obj->GetStage()->GetView()->GetTargetCamera();
		camera->SetAt(pos + Vec3(0.0f, 1.0f, 0.0f));
		camera->SetEye(pos + Vec3(0.0f, 1.5f, -10.0f));
		
		// コンボを加算する
		Obj->AddCombo();

		Obj->ResetGoLink();
	}
	//ステート実行中に毎ターン呼ばれる関数
	void LinkState::Execute(const shared_ptr<Player>& Obj) {
		Obj->CameraControll();
		Obj->CameraRoll();

		//スロー状態なら、カメラとレイを開放する
		if (GameManager::GetInstance().GetOnSlow()) {
			Obj->RayShot();
			Obj->ResetGoLink();
		}
		else {
			Obj->PlayerRoll();
		}
		//どれに飛んでいるかで処理を変える
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
	//ステートにから抜けるときに呼ばれる関数
	void LinkState::Exit(const shared_ptr<Player>& Obj) {
		Obj->FaceChanger(Obj->m_faceNum, Player::FaceState::Default);
	}
	//--------------------------------------------------------------------------------------
	//	class DataState : public ObjState<Player>;
	//	用途: データ体状態
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	shared_ptr<DataState> DataState::Instance() {
		static shared_ptr<DataState> instance(new DataState);
		return instance;
	}
	//ステートに入ったときに呼ばれる関数
	void DataState::Enter(const shared_ptr<Player>& Obj) {
		Obj->ChengeEnergyMai();
		Obj->ChangeWalkSpeed(Player::State::DATA);

	}
	//ステート実行中に毎ターン呼ばれる関数
	void DataState::Execute(const shared_ptr<Player>& Obj) {
		Obj->Walk();
		Obj->RayShot();
		Obj->SightingDeviceChangePosition();
		//カメラ制御のための値を変更する
		Obj->CameraControll();
		//右スティックの値でカメラの回転処理を行う
		Obj->CameraRoll();
		//プレイヤーの体の向きを変える
		Obj->PlayerRoll();
	}
	//ステートにから抜けるときに呼ばれる関数
	void DataState::Exit(const shared_ptr<Player>& Obj) {
		Obj->ResetTimeLim();
	}

}
//end basecross

