/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"


namespace basecross {
	///-------------------------------------------------------------------------------
	//playerのコンストラクタ
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
	//初期化
	///-------------------------------------------------------------------------------
	void Player::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();

		ptrTrans->SetWorldPosition(m_position);
		ptrTrans->SetQuaternion(m_quaternion);
		ptrTrans->SetScale(m_scale);

		//四角形の当たり判定をセット
		auto col = AddComponent<CollisionObb>();
		//コリジョンの判定をしない
		col->SetAfterCollision(AfterCollision::None);
		//playerに持たせて使うものには衝突しない
		col->AddExcludeCollisionTag(L"PlayerUse");

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
		drawComp->AddAnimation(L"Clear", 400, 100, false, 30.0f);
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
	///-------------------------------------------------------------------------------
	//Update
	///-------------------------------------------------------------------------------
	void Player::OnUpdate() {
		// アプリケーション取得
		auto& app = App::GetApp();
		// 1フレームの実行にかかった時間を取得
		float delta = app->GetElapsedTime();

		if (GameManager::GetInstance().GetGameStart()) {
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
		Extrusion();
	}
	///-------------------------------------------------------------------------------
	//衝突したとき
	///-------------------------------------------------------------------------------
	void Player::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		auto trans = GetComponent<Transform>();
		auto otherTrans = Other->GetComponent<Transform>();
		//playerの下面と衝突した物体の上面が当たっていたら
		if (((otherTrans->GetWorldPosition().y + otherTrans->GetScale().y / 2) - (trans->GetWorldPosition().y - trans->GetScale().y / 2)) < 0.5f) {
			//自動でY方向に力を加える処理を行わないようにする
			m_isFall = false;
		}

		//最初のおりてくる演出、着地したらゲームを開始して操作できるようにする
		if (!GameManager::GetInstance().GetGameStart()) {
			GameManager::GetInstance().SetGameStart(true);
		}
	}
	///-------------------------------------------------------------------------------
	//衝突し続けているとき
	///-------------------------------------------------------------------------------
	void Player::OnCollisionExcute(shared_ptr<GameObject>& Other) {
		auto trans = GetComponent<Transform>();
		auto otherTrans = Other->GetComponent<Transform>();
		//playerの下面と衝突した物体の上面が当たっていたら
		if (((otherTrans->GetWorldPosition().y + otherTrans->GetScale().y / 2) - (trans->GetWorldPosition().y - trans->GetScale().y / 2)) < 0.5f) {
			//自動でY方向に力を加える処理を行わないようにする
			m_isFall = false;
		}
		ExtrusionJudgment(Other);

	}
	///-------------------------------------------------------------------------------
	//角度の向きを考慮したスティックの角度を算出する
	///-------------------------------------------------------------------------------
	void Player::Forword() {
		auto camera = GetStage()->GetView()->GetTargetCamera();
		auto tpsCamera = dynamic_pointer_cast<TpsCamera>(camera);
		// スティックの傾きを角度に変換する
		float padRad = atan2f(m_padDir.z, m_padDir.x);
		if (m_padDir.length() != 0.0f) {
			// スティックの角度にカメラの角度を加える
			padRad += tpsCamera->GetCameraAngleY() + XM_PIDIV2;  // カメラの角度に９０度加える
			// 角度をXZ成分に分解してベクトルを作り直す
			m_padDir.x = cosf(padRad); // 新しい角度を X 成分に分解する
			m_padDir.z = sinf(padRad); // 新しい角度を Z 成分に分解する

			m_forward = m_padDir.normalize();
		}
	}

	///-------------------------------------------------------------------------------
	//Y方向の移動処理
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
	//プレイヤーを向かう方向に回転させる
	///-------------------------------------------------------------------------------
	void Player::PlayerRoll() {
		auto ptrUtil = GetBehavior<UtilBehavior>();
		ptrUtil->RotToHead(m_forward, 0.1f);
	}
	///-------------------------------------------------------------------------------
	//ベジエ曲線の制御点設定
	///-------------------------------------------------------------------------------
	void Player::SetBezierPoint(Vec3 point) {

		p0 = GetComponent<Transform>()->GetWorldPosition();
		p2 = point + Vec3(0, 1.0f, 0);
		//飛ぶ先までの距離に応じて飛ぶ際のスピードを変える
		m_BezierSpeedLeap = Vec3(p2 - p0).length();
		//飛ぶ先までの距離に応じて飛ぶ際の放物線の形を変える
		if (m_BezierSpeedLeap >= 20.0f) {
			p1 = point + Vec3(0, 6, 0);
		}
		else if (m_BezierSpeedLeap >= 10.0f) {
			p1 = point + Vec3(0, 3, 0);
		}
		else if (m_BezierSpeedLeap < 10.0f) {
			p1 = point - ((point - p0) / 2);
		}
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
	///-------------------------------------------------------------------------------
	//照準の位置をカメラとプレイヤーの位置から求め変更する
	///-------------------------------------------------------------------------------
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
		rot.rotationRollPitchYawFromVector(Vec3(-atan2f(dir.y, syahen), atan2f(dir.x, dir.z), 0.0f));

		sightingDevice->GetComponent<Transform>()->SetQuaternion(rot);
	}
	///-------------------------------------------------------------------------------
	//Rayを飛ばす
	///-------------------------------------------------------------------------------
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
		//ロックオンの処理
		RockonObject(sightPos, dir, L"Drone", 1.0f);
		RockonObject(sightPos, dir, L"Link", 4.0f);
		RockonObject(sightPos, dir, L"Mails", 4.0f);
		//ロックオン対象を向く
		RockOn();
		//RBボタンが押されていたら飛べるかの判定も行う
		if (m_pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			//リンクオブジェクトとの判定
			LinkRayCheck(sightPos, dir);
			//メールとの判定
			MailRayCheck(sightPos, dir);
			//ドローンとの判定
			DroneRayCheck(sightPos, dir);
		}
	}
	///-------------------------------------------------------------------------------
	//RayとOBBの判定
	///-------------------------------------------------------------------------------
	bool Player::RayOBBHit(Vec3 origin, Vec3 originDir, shared_ptr<GameObject> obj) {
		auto trans = obj->GetComponent<Transform>();
		//オブジェクトのOBBを作る
		OBB obb(trans->GetScale() * 3.0f, trans->GetWorldMatrix());
		//プレイヤーからでるRayとOBBで判定
		bool hit = HitTest::SEGMENT_OBB(origin, origin + originDir * m_rayRange, obb);
		//Rayの範囲内にあるオブジェクトかを見る
		if ((origin - trans->GetWorldPosition()).length() <= m_rayRange + 1.0f) {
			//1つ前にベジエ曲線で飛んだリンクオブジェクトじゃないものに当たっていたら
			if (hit && p2 + Vec3(0, -1, 0) != trans->GetWorldPosition()) {
				//それは飛べるもの
				return true;
			}
		}
		return false;
	}
	///-------------------------------------------------------------------------------
	//RayとLinkオブジェクトが当たっているかを調べる
	///-------------------------------------------------------------------------------
	void Player::LinkRayCheck(Vec3 origin, Vec3 originDir) {
		//すでに他のものに飛んでいたら
		if (m_isGoLink) {
			return;
		}
		//リンクオブジェクトの入っているグループを持ってくる
		auto& linkGroup = GetStage()->GetSharedObjectGroup(L"Link");
		//一つずつ取り出す
		for (auto& link : linkGroup->GetGroupVector()) {
			auto linkObj = link.lock();
			//レイと今見ているオブジェクトが当たっているか判定
			if (RayOBBHit(origin, originDir, linkObj)) {
				auto sightingDevice = m_SightingDevice.lock();
				//照準に当たっていることを教える
				sightingDevice->SetCaptureLink(true);
				Vec3 dir = GetComponent<Transform>()->GetWorldPosition() - linkObj->GetComponent<Transform>()->GetWorldPosition();
				dir.y = 0;
				//オブジェクトに被らないように方向を加味してずらした値を渡す
				SetBezierPoint(linkObj->GetComponent<Transform>()->GetWorldPosition() + dir.normalize());
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
				return;
			}
		}
	}
	///-------------------------------------------------------------------------------
	//RayとDroneオブジェクトが当たっているかを調べる
	///-------------------------------------------------------------------------------
	void Player::DroneRayCheck(Vec3 origin, Vec3 originDir) {
		//すでに他のものに飛んでいたら
		if (m_isGoLink) {
			return;
		}
		//ドローンオブジェクトの入っているグループを持ってくる
		auto& droneGroup = GetStage()->GetSharedObjectGroup(L"Drone");
		//何番目のDroneか数える
		int count = 0;
		//一つずつ取り出す
		for (auto& drone : droneGroup->GetGroupVector()) {
			auto droneObj = drone.lock();
			if (RayOBBHit(origin, originDir, droneObj)) {
				auto sightingDevice = m_SightingDevice.lock();
				//照準に当たっていることを教える
				sightingDevice->SetCaptureLink(true);
				SetBezierPoint(droneObj->GetComponent<Transform>()->GetWorldPosition());
				m_DroneNo = count;
				//経過をリセット
				m_Lerp = 0;
				//飛ぶの確定
				m_isGoLink = true;
				//スローの経過時間をリセット
				GameManager::GetInstance().ResetSloawPassage();
				m_StateMachine->ChangeState(LinkState::Instance());
				m_target = Target::DRONE;
				return;
			}
			count++;
		}
	}
	///-------------------------------------------------------------------------------
	//RayとMailオブジェクトが当たっているかを調べる
	///-------------------------------------------------------------------------------
	void Player::MailRayCheck(Vec3 origin, Vec3 originDir) {
		//すでに他のものに飛んでいたら
		if (m_isGoLink) {
			return;
		}
		auto& mailGroup = GetStage()->GetSharedObjectGroup(L"Mails");
		int count = 0;
		for (auto& mail : mailGroup->GetGroupVector()) {
			auto mailObj = mail.lock();
			//今見ているメールが取得されていないメールか確認
			if (dynamic_pointer_cast<MailObject>(mailObj)->GetIsArrive() != true) {
				if (RayOBBHit(origin, originDir, mailObj)) {
					auto sightingDevice = m_SightingDevice.lock();
					//照準に当たっていることを教える
					sightingDevice->SetCaptureLink(true);
					if (m_pad.wPressedButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
						Vec3 dir = GetComponent<Transform>()->GetWorldPosition() - mailObj->GetComponent<Transform>()->GetWorldPosition();
						dir.y = 0;
						//オブジェクトに被らないように方向を加味してずらした値を渡す
						SetBezierPoint(mailObj->GetComponent<Transform>()->GetWorldPosition() + dir.normalize());
						//何番のメールにアクセスしているか保存
						m_MailNum = count;
						m_Lerp = 0;
						//飛ぶの確定
						m_isGoLink = true;
						//ドローンが入ったままだとそちらのほうに向かってしまうのでNULLにする
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
	//リンクへ飛ぶ処理
	///-------------------------------------------------------------------------------
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
			if (m_chain % BONUS_CHAIN == 0)
			{
				auto ptrEffect = GetStage()->GetSharedGameObject<BonusEffect>(L"BonusEffect", false);
				if (ptrEffect) {
					auto playerPos = GetComponent<Transform>()->GetWorldPosition();
					ptrEffect->InsertBounusEffect(Vec3(playerPos.x, playerPos.y - 3.0f, playerPos.z), Vec2(3.0f, 3.0f));
				}
			}
		}
		//飛ぶ処理が終わったら
		if (gameManager.GetSlowPassage() >= 1.0f) {
			m_Lerp = 1.0f;
			//スローを終わる
			gameManager.SetOnSlow(false);
			//スコアを増やす
			gameManager.AddScore(GetChain() * 20 + GetChain() * 10);
			//次のリンクへ飛べなかったのでチェインリセットする
			ChainFailure();
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
	///-------------------------------------------------------------------------------
	//ドローンへ飛ぶ処理
	///-------------------------------------------------------------------------------
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
			ChainFailure();
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
	///-------------------------------------------------------------------------------
	//メールへ飛ぶ処理
	///-------------------------------------------------------------------------------
	void Player::MailGo() {
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
				//次の目標へ飛べなかったのでチェインリセット
				ChainFailure();
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
			//演出でチェイン数を飛ばす
			auto flyingChain = GetStage()->AddGameObject<FlyingChain>();
			//演出でチェイン数を飛ばすために値を与える
			flyingChain->FlySet(GetChain());
			//スローの経過時間をリセット
			gameManager.ResetSloawPassage();
			//スローにする
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
		//ベジエ曲線の計算
		pos = (1 - m_Lerp) * (1 - m_Lerp) * p0 + 2 * (1 - m_Lerp) * m_Lerp * p1 + m_Lerp * m_Lerp * p2;
		GetComponent<Transform>()->SetWorldPosition(pos);
		//if (!gameManager.GetOnSlow()) {
		//	//プレイヤーと一緒に動くためにプレイヤーのLeapでカメラを動かす
		//	auto camera = GetStage()->GetView()->GetTargetCamera();
		//	dynamic_pointer_cast<TpsCamera>(camera)->BezierMove(m_Lerp, pos);
		//}

	}
	///-------------------------------------------------------------------------------
	//ロックオン対象がいたら、そちらを向く
	///-------------------------------------------------------------------------------
	void Player::RockOn() {
		//ロックオンしているとき
		if (m_islockon) {
			auto sightPos = m_SightingDevice.lock()->GetComponent<Transform>()->GetWorldPosition();
			auto lockOnPos = m_LockOnObj.lock()->GetComponent<Transform>()->GetWorldPosition();

			auto camera = GetStage()->GetView()->GetTargetCamera();
			auto tpsCamera = dynamic_pointer_cast<TpsCamera>(camera);
			//ロックオンオブジェを対象にカメラの処理
			tpsCamera->RockonCameraMove(sightPos, lockOnPos);
			//ロックオン対象が近くにいる場合はロックオンを解除
			Vec3 delta = GetComponent<Transform>()->GetWorldPosition() - lockOnPos;
			if (delta.length() <= 4.0f) {
				m_islockon = false;
			}
		}
	}
	///---------------------------------------------------------------------------------------------
	//ロックオンするオブジェクトを設定
	///---------------------------------------------------------------------------------------------
	void Player::RockonObject(Vec3 origin, Vec3 originDir, wstring groupName, float correction){
		//Lボタンを押し続けているとき
		if (GameManager::GetInstance().GetPad().wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
			m_islockon = false;
			return;
		}
		//もうすでにロックオンしている対象がいる
		else if(m_islockon){
			return;
		}

		//オブジェクトの入っているグループを持ってくる
		auto& objectsGroup = GetStage()->GetSharedObjectGroup(groupName);
		//一つずつ取り出す
		for (auto& object : objectsGroup->GetGroupVector()) {
			auto lockonObj = object.lock();
			auto mail = dynamic_pointer_cast<MailObject>(lockonObj);
			//メールが取得状態だったらスルーする
			if (mail) {
				if (mail->GetIsArrive()) {
					continue;
				}
			}
			auto objTrans = lockonObj->GetComponent<Transform>();
			//リンクオブジェクトのOBBを作る
			OBB obb(objTrans->GetScale() * correction, objTrans->GetWorldMatrix());
			//照準からでるRayとOBBで判定
			bool hit = HitTest::SEGMENT_OBB(origin, origin + originDir * m_rayRange, obb);
			if (hit) {
				Vec3 delta = origin - objTrans->GetWorldPosition();
				float deltaLength = delta.length();
				//近いときはロックオンしない
				if (deltaLength <= 8.0f) {
					continue;
				}
				//Rayの範囲よりも遠い時は、判定上は当たっていても無視する
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
	//押し出しが必要か判定する
	///-------------------------------------------------------------------------------
	void Player::ExtrusionJudgment(const weak_ptr<GameObject>& Other) {
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
	///-------------------------------------------------------------------------------
	//押し出しをする
	///-------------------------------------------------------------------------------
	void Player::Extrusion() {
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
	}

	void Player::FaceChanger(FaceState beforeFace, FaceState afterFace)
	{
		auto drawComp = GetComponent<PNTBoneModelDraw>();
		drawComp->SetMultiMeshIsDraw(beforeFace, false);
		drawComp->SetMultiMeshIsDraw(afterFace, true);
		m_faceNum = afterFace;
	}
	///-------------------------------------------------------------------------------
	// タイムアップ時にアニメーションを見せる
	///-------------------------------------------------------------------------------
	void Player::ShowTimeUpAnime() {
		GetAnimStateMachine()->ChangeState(PlayerClearAnim::Instance());
	}
	///-------------------------------------------------------------------------------
	//タイムアップアニメーションが終わったらリザルトにいく
	///-------------------------------------------------------------------------------
	void Player::TimeUpAnimeFinishToResult() {
		if (GetComponent<PNTBoneModelDraw>()->IsTargetAnimeEnd() && GameManager::GetInstance().GetTimeUp()) {
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToResultStage");
		}
	}
	///-------------------------------------------------------------------------------
	//プレイヤーの向いている方向を返す
	///-------------------------------------------------------------------------------
	Vec2 Player::forwardAngle() {
		Vec2 forwardAngle;
		//横のカメラ位置を制御する角度
		forwardAngle.y = atan2f(m_forward.z, m_forward.x);
		if (forwardAngle.y < 0.0f) {
			forwardAngle.y += Deg2Rad(360.0f);
		}
		//縦のカメラ位置をだすための辺出す
		float syahen = hypotf(m_forward.x, m_forward.z);
		//縦のカメラ位置を制御する角度
		forwardAngle.x = atan2f(m_forward.y, syahen);

		return forwardAngle;
	};
	///-------------------------------------------------------------------------------
	// チェイン数を増やす
	///-------------------------------------------------------------------------------
	void Player::AddChain()
	{
		m_chain++;
		auto& gm = GameManager::GetInstance();
		//現在チェイン数が最大チェイン数を上回ったら更新する
		if (gm.GetMaxChain() < m_chain) {
			gm.SetMaxChain(m_chain);
		}

		if (m_chain % BONUS_CHAIN == 0)
		{
			gm.AddBouns();
		}
	}
	///-------------------------------------------------------------------------------
	// チェイン失敗時に色々リセットする
	///-------------------------------------------------------------------------------
	void Player::ChainFailure() {
		m_chain = 0;
		m_rayRange = m_rayRangeDefolt;
		//ロックオン対象が居たら
		if (m_islockon) {
			Vec3 delta = m_SightingDevice.lock()->GetComponent<Transform>()->GetWorldPosition()
				- m_LockOnObj.lock()->GetComponent<Transform>()->GetWorldPosition();
			float deltaLength = delta.length();
			//Rayの範囲とロックオンオブジェとの距離を比べて範囲外だったらロックオンを外す
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
	//	用途: リンク上を飛んでいる状態
	//--------------------------------------------------------------------------------------
	shared_ptr<LinkState> LinkState::Instance() {
		static shared_ptr<LinkState> instance(new LinkState);
		return instance;
	}
	void LinkState::Enter(const shared_ptr<Player>& Obj) {
		Obj->FaceChanger(Obj->m_faceNum, Player::FaceState::Smile);
		auto scenePtr = App::GetApp()->GetScene<Scene>();
		// 効果音を鳴らす
		scenePtr->MusicOnceStart(L"Jump_SE", 1.0f);
		Obj->GetAnimStateMachine()->ChangeState(PlayerFlyAnim::Instance());

		// コンボを加算する
		Obj->AddChain();

	}
	void LinkState::Execute(const shared_ptr<Player>& Obj) {
		auto camera = Obj->GetStage()->GetView()->GetTargetCamera();
		auto tpsCamera = dynamic_pointer_cast<TpsCamera>(camera);
		//カメラを動かす
		tpsCamera->CameraRoll(Obj->GetSightPos());
		if (!GameManager::GetInstance().GetTimeUp()) {
			//カメラ制御のための値を変更する
			tpsCamera->CameraControll();

			//スロー状態なら、レイを開放する
			if (GameManager::GetInstance().GetOnSlow()) {
				Obj->ResetGoLink();
				Obj->RayShot();
			}
			else {
				Obj->PlayerRoll();
			}
		}
		//どれに飛んでいるかで処理を変える
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
	//	用途: データ体状態
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
		//カメラを動かす
		tpsCamera->CameraRoll(Obj->GetSightPos());
		//カメラ制御のための値を変更する
		tpsCamera->CameraControll();
		//プレイヤーの体の向きを変える
		Obj->PlayerRoll();
		//タイムアップしていたら、アニメーションを流す
		if (GameManager::GetInstance().GetTimeUp()) {
			Obj->GetStateMachine()->ChangeState(ClearState::Instance());
		}
	}
	void DataState::Exit(const shared_ptr<Player>& Obj) {
		Obj->ResetTimeLim();
	}
	//--------------------------------------------------------------------------------------
	//class ClearState : public ObjState<Player>;
	//用途: クリアアニメーション状態
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

		//カメラのプレイヤーの正面に回る処理を呼ぶ
		if (!tpsCamera->GoingAround()) {
			//正面に回ったらクリアアニメーション
			Obj->ShowTimeUpAnime();
		}
		tpsCamera->CameraRoll(Obj->GetSightPos());

		Obj->TimeUpAnimeFinishToResult();
	}
	void ClearState::Exit(const shared_ptr<Player>& Obj){
	}
}
//end basecross

