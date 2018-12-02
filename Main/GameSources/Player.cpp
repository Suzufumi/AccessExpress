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
	//初期化
	//-------------------------------------------------------------------------------------------------
	void Player::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();

		ptrTrans->SetWorldPosition(m_position);
		ptrTrans->SetQuaternion(m_quaternion);
		ptrTrans->SetScale(m_scale);

		//四角形の当たり判定をセット
		auto col = AddComponent<CollisionObb>();
		col->SetDrawActive(true);
		//コリジョンの判定をしない
		col->SetAfterCollision(AfterCollision::None);
		//playerに持たせて使うものには衝突しない
		col->AddExcludeCollisionTag(L"PlayerUse");

		//文字列をつける
		auto ptrString = AddComponent<StringSprite>();
		ptrString->SetText(L"");
		ptrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));

		Mat4x4 spanMat; // モデルとトランスフォームの間の差分行列
		spanMat.affineTransformation(
			Vec3(1.0f, 0.5f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, Deg2Rad(180), 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
		);

		//描画コンポーネントの追加
		auto drawComp = AddComponent<PNTStaticModelDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		drawComp->SetMultiMeshResource(L"PLAYER_MODEL");
		drawComp->SetTextureResource(L"PLAYER_TX");
		// 変換した行列を代入
		drawComp->SetMeshToTransformMatrix(spanMat);
		//Col4 Color(1.0f, 0.2f, 1.0f, 0.7f);
		//drawComp->SetDiffuse(Color);
		//drawComp->SetColorAndAlpha(Color);
		// レイヤーの調整
		SetDrawLayer(1);

		m_StateMachine.reset(new StateMachine<Player>(GetThis<Player>()));
		//最初のステートをWalkStateに設定
		m_StateMachine->ChangeState(WalkState::Instance());
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
		m_pad = device.GetControlerVec()[0];
		//左スティックの入力方向を求める
		m_padDir = Vec3(m_pad.fThumbLX, 0.0f, m_pad.fThumbLY);
		m_padDir = m_padDir.normalize();

		//右スティックの値でカメラの回転処理を行う
		CameraRoll();

		//ステートマシンのアップデート
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
		
		// デバッグ文字の表示
		DrawStrings();
	}
	//--------------------------------------------------------------------------------------------------------------
	//衝突したとき
	//--------------------------------------------------------------------------------------------------------------
	void Player::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		//ファイルに変換する
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

		auto access = dynamic_pointer_cast<LinkObject>(Other);
		if (access) {
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//衝突が解除されたとき
	//-------------------------------------------------------------------------------------------------------------
	void Player::OnCollisionExit(shared_ptr<GameObject>& Other) {
		m_isFall = true;
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
		}
		//左スティックが入力されてなかったら
		//else {
		//	//Velocity値をだんだん小さくする
		//	m_velocity.x = m_velocity.x * 0.9f;
		//	m_velocity.z = m_velocity.x * 0.9f;
		//}
		playerTrans->SetWorldPosition(playerPos);
	}
	//---------------------------------------------------------------------------------------------
	//Y方向の移動処理
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
	//カメラの回転処理
	//--------------------------------------------------------------------------------------------
	void Player::CameraRoll() {
		float delta = App::GetApp()->GetElapsedTime();
		// スティックの傾きを角度に変換する
		float padRad = atan2f(m_padDir.z, m_padDir.x);

		m_angleY += -m_pad.fThumbRX * m_maxAngleSpeed * delta; // カメラを回転させる
		m_cameraHeight += -m_pad.fThumbRY * m_maxAngleSpeed * 1.5f * delta; // カメラを昇降させる
		//360度を越えたら0にする
		if (m_angleX > 360) {
			m_angleX = 0;
		}
		//0度よりも小さくなったら
		if (m_angleX < 0) {
			m_angleX = 360;
		}

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
	//リンクからリンクへ飛ぶ処理
	//---------------------------------------------------------------------------------------------
	void Player::LinkGo() {
		auto pos = GetComponent<Transform>()->GetWorldPosition();
		//計算のための時間加算
		m_Lerp += App::GetApp()->GetElapsedTime();
		if (m_Lerp >= 1.0f) {
			m_Lerp = 1.0f;
			//飛び終わったらステートをデータ体にする
			m_StateMachine->ChangeState(DateState::Instance());
		}
		//ベジエ曲線の計算
		pos.x = (1 - m_Lerp) * (1 - m_Lerp) * p0.x + 2 * (1 - m_Lerp) * m_Lerp * p1.x + m_Lerp * m_Lerp * p2.x;
		pos.y = (1 - m_Lerp) * (1 - m_Lerp) * p0.y + 2 * (1 - m_Lerp) * m_Lerp * p1.y + m_Lerp * m_Lerp * p2.y;
		pos.z = (1 - m_Lerp) * (1 - m_Lerp) * p0.z + 2 * (1 - m_Lerp) * m_Lerp * p1.z + m_Lerp * m_Lerp * p2.z;
		GetComponent<Transform>()->SetWorldPosition(pos);
	}
	//ベジエ曲線の制御点設定
	void Player::SetBezierPoint(Vec3 point) {
		p0 = GetComponent<Transform>()->GetWorldPosition();
		p1 = point + Vec3(0, 10, 0);
		p2 = point + Vec3(0,1,0);
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
		sightingDevice->GetComponent<Transform>()->SetWorldPosition((pos + Vec3(0.0f, 3.0f, 0.0f)) + (dir * 2.0f));

		Quat rot;
		rot.rotationRollPitchYawFromVector(Vec3(0.0f,atan2f(dir.x, dir.z), 0.0f));
		
		sightingDevice->GetComponent<Transform>()->SetQuaternion(rot);
	}
	//---------------------------------------------------------------------------------------------
	//RayとLinkオブジェクトが当たっているかを調べる
	//---------------------------------------------------------------------------------------------
	void Player::RayHitLink() {
		auto sightingDevice = m_SightingDevice.lock();
		//リンクオブジェクトに当たっているフラグをfalseに戻す
		sightingDevice->ResetCaptureLink();

		auto pos = sightingDevice->GetComponent<Transform>()->GetWorldPosition();
		auto m_cameraPos = GetStage()->GetView()->GetTargetCamera()->GetEye();
		//playerとカメラの位置から飛ばす方向を求める
		auto dir = pos - m_cameraPos;
		dir = dir.normalize();
		//リンクオブジェクトの入っているグループを持ってくる
		auto& linkGroup = GetStage()->GetSharedObjectGroup(L"Link");
		//一つずつ取り出す
		for (auto& link : linkGroup->GetGroupVector()) {
			auto linkObj = link.lock();
			auto linkTrans = linkObj->GetComponent<Transform>();
			//リンクオブジェクトのOBBを作る
			OBB obb(linkTrans->GetScale() * 3, linkTrans->GetWorldMatrix());
			//プレイヤーからでるRayとOBBで判定
			bool hit = HitTest::SEGMENT_OBB(pos, pos + dir * 30.0f, obb);
			//最後にベジエ曲線で飛んだリンクオブジェクトじゃないものに当たっていたら
			if (hit && p2 + Vec3(0, -1, 0) != linkTrans->GetWorldPosition()) {
				//照準に当たっていることを教える
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
		// 最小になっている方向に対して押し出しを行う
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
	//Aボタンが押されたかどうかを返す
	//---------------------------------------------------------------------------------------------
	bool Player::CheckAButton() {
		if (m_pad.wPressedButtons & XINPUT_GAMEPAD_A) {
			return true;
		}
		return false;
	}

	//---------------------------------------------------------------------------------------------
	//コンボ数に応じてボーナスを与える
	//---------------------------------------------------------------------------------------------
	void Player::ComboBonus(int nowChains)
	{
		m_energy += nowChains * 5.0f;
	}

	//---------------------------------------------------------------------------------------------
	//情報の表示
	//---------------------------------------------------------------------------------------------
	void Player::DrawStrings()
	{
		// FPSの取得
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
		//文字列をつける
		wstring str = strFps + cameraStr + energy + combo + timeLimit;
		//wstring str = energy;
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
	}
	//ステート実行中に毎ターン呼ばれる関数
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
	//ステートにから抜けるときに呼ばれる関数
	void WalkState::Exit(const shared_ptr<Player>& Obj) {
	}
	//--------------------------------------------------------------------------------------
	//	class WalkState : public ObjState<Player>;
	//	用途: リンク上を飛んでいる状態
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	shared_ptr<LinkState> LinkState::Instance() {
		static shared_ptr<LinkState> instance(new LinkState);
		return instance;
	}

	const int CHAIN_TIMELIMIT = 180;	// コンボが途切れる時間

	//ステートに入ったときに呼ばれる関数
	void LinkState::Enter(const shared_ptr<Player>& Obj) {
		//auto pos = Obj->GetComponent<Transform>()->GetWorldPosition();
		//auto camera = Obj->GetStage()->GetView()->GetTargetCamera();
		//camera->SetAt(pos + Vec3(0.0f, 1.0f, 0.0f));
		//camera->SetEye(pos + Vec3(0.0f, 1.5f, -10.0f));
		
		// コンボを加算する
		Obj->AddCombo();
		Obj->ComboBonus(Obj->GetChain());
	}
	//ステート実行中に毎ターン呼ばれる関数
	void LinkState::Execute(const shared_ptr<Player>& Obj) {
		Obj->LinkGo();
		Obj->CameraControll();
		Obj->SightingDeviceChangePosition();
		if (Obj->GetEnergy() <= 0.0f) {
			Obj->GetStateMachine()->ChangeState(WalkState::Instance());
			Obj->SightingDeviceDrawActiveFlag(false);
		}
	}
	//ステートにから抜けるときに呼ばれる関数
	void LinkState::Exit(const shared_ptr<Player>& Obj) {
	}
	//--------------------------------------------------------------------------------------
	//	class DateState : public ObjState<Player>;
	//	用途: データ体状態
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	shared_ptr<DateState> DateState::Instance() {
		static shared_ptr<DateState> instance(new DateState);
		return instance;
	}
	//ステートに入ったときに呼ばれる関数
	void DateState::Enter(const shared_ptr<Player>& Obj) {
		Obj->ChengeEnergyMai();
		// 以前のステートがLinkStateだったら
		if (Obj->GetStateMachine()->GetPreviousState() == LinkState::Instance())
		{
			// タイムチェッカーフラグをオンにする
			Obj->SetAdvanceTimeActive(true);
		}
	}
	//ステート実行中に毎ターン呼ばれる関数
	void DateState::Execute(const shared_ptr<Player>& Obj) {
		// コンボ間の時間を進めるかどうか
		if (Obj->GetAdvanceTimeActive())
		{
			// 時間を加算する
			Obj->AddChainTimeLimit();
		}
		// 現在の時間を取得
		int timeLimit = Obj->GetChainTimeLim();
		// 制限時間に達したら
		if (timeLimit >= CHAIN_TIMELIMIT)
		{
			// コンボのリセット
			Obj->ResetCombo();
			// 制限時間をリセット
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
	//ステートにから抜けるときに呼ばれる関数
	void DateState::Exit(const shared_ptr<Player>& Obj) {
		Obj->ResetTimeLim();
	}

}
//end basecross

