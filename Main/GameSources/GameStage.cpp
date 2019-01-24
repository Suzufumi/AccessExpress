/*!
@file GameStage.cpp
@brief ƒQ[ƒ€ƒXƒe[ƒWÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ƒQ[ƒ€ƒXƒe[ƒWƒNƒ‰ƒXÀ‘Ì
	//--------------------------------------------------------------------------------------
	void GameStage::CreateViewLight() {
		auto ptrView = CreateView<SingleView>();

		//auto ptrCamera = ObjectFactory::Create<Camera>();
		// ƒfƒoƒbƒO—pƒAƒ“ƒOƒ‹(ƒXƒe[ƒW‘S‘Ì‚ğŒ©‚é)
		//ptrView->SetCamera(ptrCamera);
		//ptrCamera->SetEye(Vec3(0.0f, 60.0f, -100.0f));
		//ptrCamera->SetAt(Vec3(0.0f, -30.0f, 50.0f));

		//ƒrƒ…[‚ÌƒJƒƒ‰‚Ìİ’è
		auto ptrCamera = ObjectFactory::Create<TpsCamera>();
		ptrView->SetCamera(ptrCamera);
		//ƒJƒƒ‰‚ª”»’è‚ğ‚Ä‚È‚¢‚Ì‚ÅAŒÂ•Ê‚É”»’è‚ğì¬
		AddGameObject<TpsCameraJudgment>();
		//ptrCamera->SetEye(Vec3(0.0f, 10.0f, -5.0f));
		//ptrCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//ƒ}ƒ‹ƒ`ƒ‰ƒCƒg‚Ìì¬
		auto ptrMultiLight = CreateLight<MultiLight>();
		//ƒfƒtƒHƒ‹ƒg‚Ìƒ‰ƒCƒeƒBƒ“ƒO‚ğw’èx
		ptrMultiLight->SetDefaultLighting();


	}


	//----------------------------------------------------------------
	//°‚Ìì¬
	//----------------------------------------------------------------
	void GameStage::CreateFloor() {
		auto ptr = AddGameObject<GameObject>();
		auto ptrTrans = ptr->GetComponent<Transform>();
		//lŠpŒ`‚ÌƒRƒŠƒWƒ‡ƒ“ƒZƒbƒg
		auto col = ptr->AddComponent<CollisionObb>();
		//“®‚©‚È‚¢‚æ‚¤‚É‚·‚é
		col->SetAfterCollision(AfterCollision::None);
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, 0, 0));
		ptrTrans->SetScale(100.0f, 0.5f, 100.0f);
		ptrTrans->SetQuaternion(Qt);
		ptrTrans->SetWorldPosition(Vec3(0.0f, -0.25f, 0.0f));
		//•`‰æƒRƒ“ƒ|[ƒlƒ“ƒg‚Ì’Ç‰Á
		auto drawComp = ptr->AddComponent<BcPNTStaticDraw>();
		//•`‰æƒRƒ“ƒ|[ƒlƒ“ƒg‚ÉŒ`óiƒƒbƒVƒ…j‚ğİ’è
		drawComp->SetMeshResource(L"DEFAULT_CUBE");
		drawComp->SetFogEnabled(true);
		//©•ª‚É‰e‚ª‰f‚è‚±‚Ş‚æ‚¤‚É‚·‚é
		drawComp->SetOwnShadowActive(false);
		Col4 Color(1.0f, 1.0f, 1.0f, 0.7f);
		drawComp->SetColorAndAlpha(Color);
	}
	//----------------------------------------------------------------
	//ƒvƒŒƒCƒ„[ŠÖŒW‚Ìì¬
	//----------------------------------------------------------------
	void GameStage::CreatePlayerRelationship() {
		GameObjecttXMLBuilder builder;
		// ƒvƒŒƒCƒ„[‚ğXml‚É“o˜^
		builder.Register<Player>(L"Player");
		// Xmlƒtƒ@ƒCƒ‹‚©‚çL"Player"‚ğ’T‚·
		builder.Build(GetThis<Stage>(), m_stageXmlPath, L"GameStage/Player");

		//ƒvƒŒƒCƒ„[ŠÖ˜A
		AddGameObject<ViewChainLetter>();
		auto chainNum = AddGameObject<ViewChainNum>();
		chainNum->GetComponent<Transform>()->SetPosition(Vec3(780, -480, 0));
		AddGameObject<RayRangeViewObj>();
	}

	void GameStage::CreateBill()
	{
		GameObjecttXMLBuilder builder;
		builder.Register<Wall>(L"Wall");
		builder.Build(GetThis<Stage>(), m_stageXmlPath, L"GameStage/Wall");
	}

	void GameStage::CreateLinkObject()
	{
		GameObjecttXMLBuilder builder;
		builder.Register<LinkObject>(L"RadioTower");
		builder.Build(GetThis<Stage>(), m_stageXmlPath, L"GameStage/RadioTower");
	}

	void GameStage::CreateDrone()
	{
		GameObjecttXMLBuilder builder;
		builder.Register<Drone>(L"Enemy");
		builder.Build(GetThis<Stage>(), m_stageXmlPath, L"GameStage/Enemy");
	}

	void GameStage::CreateGoal()
	{
		GameObjecttXMLBuilder builder;
		builder.Register<CheckPoint>(L"Goal");
		builder.Build(GetThis<Stage>(), m_stageXmlPath, L"GameStage/Goal");
	}
	void GameStage::CreateMail()
	{
		GameObjecttXMLBuilder builder;
		builder.Register<MailObject>(L"Mail");
		builder.Build(GetThis<Stage>(), m_stageXmlPath, L"GameStage/Mails");
	}

	void GameStage::CreateFire()
	{
		auto multiFirePtr = AddGameObject<FireEffect>();
		//‹¤—LƒIƒuƒWƒFƒNƒg‚É‰Š‚ğ“o˜^
		SetSharedGameObject(L"FireEffect", multiFirePtr);
		auto multiSparkPtr = AddGameObject<SparkEffect>();
		SetSharedGameObject(L"SparkEffect", multiSparkPtr);
	}


	void GameStage::OnCreate() {
		try {
			wstring dataDir;
			App::GetApp()->GetDataDirectory(dataDir);
			m_stageXmlPath = dataDir + L"Stage\\" + L"1.xml";
			auto& gm = GameManager::GetInstance();

			XmlDocReader xmlReader(m_stageXmlPath);
			//ƒQ[ƒ€‚ªn‚Ü‚Á‚Ä‚¢‚éƒtƒ‰ƒO‚ğØ‚é
			gm.SetGameStart(false);
			//ƒXƒ[ƒtƒ‰ƒO‚ğƒIƒt‚É‚µ‚Ä‚¨‚­
			gm.SetOnSlow(false);
			//ƒXƒRƒA‚ğ‰Šú‰»‚·‚é
			gm.ResetNowScore();
			//gm.ResetCheckPointNum();
			//ƒ}ƒbƒNƒXƒ`ƒFƒCƒ“‰Šú‰»
			gm.ResetMaxChain();
			//ƒ[ƒ‹”‰Šú‰»
			gm.ResetMail();
			//ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½bï¿½vï¿½tï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			gm.ResetTimeUp();
			CreateSharedObjectGroup(L"Link");
			CreateSharedObjectGroup(L"Drone");
			CreateSharedObjectGroup(L"CheckPoints");
			CreateSharedObjectGroup(L"Mails");
			//•¨—ŒvZ—LŒø
			SetPhysicsActive(true);
			//ƒrƒ…[‚Æƒ‰ƒCƒg‚Ìì¬
			CreateViewLight();
			//°
			//CreateFloor();
			//ƒvƒŒƒCƒ„[ŠÖŒW
			CreatePlayerRelationship();
			// Œš•¨‚Ì”z’u
			CreateBill();
			CreateLinkObject();

			CreateDrone();
			CreateMail();
			//ƒ[ƒ‹”‚ğ•\¦‚·‚éUI
			AddGameObject<MailNumUI>();
			//§ŒÀŠÔ‚ğì¬i’†‰›ãj
			auto timer = AddGameObject<RemainingTimerSprite>(40);
			timer->GetComponent<Transform>()->SetPosition(1280 - 64 * 10, 0, 0);
			//ƒXƒRƒAUI‚ğì¬
			auto score = AddGameObject<ScoreUI>();
			score->GetComponent<Transform>()->SetPosition(1280 - 64 * 4, 0, 0);

			CreateGoal();
			AddGameObject<SkyBox>();
			auto scoreSprite = AddGameObject<Sprite>(L"SCORE_TX", Vec2(100, 40));
			scoreSprite->SetPosition(Vec2(1120, 30));
			auto timeSprite = AddGameObject<Sprite>(L"TIME_TX", Vec2(100, 35));
			timeSprite->SetPosition(Vec2(640, 27));
			CreateFire();
		}
		catch (...) {
			throw;
		}
	}
	void GameStage::OnUpdate() {
		//ƒ}ƒl[ƒWƒƒ[‚ª‚Âƒpƒbƒhî•ñ‚ğXV
		GameManager::GetInstance().UpDatePadData();

		//ƒXƒ^[ƒgƒ{ƒ^ƒ“‚ğ‰Ÿ‚·‚±‚Æ‚ÅƒŠƒUƒ‹ƒg‚És‚­
		if (GameManager::GetInstance().GetPad().wPressedButtons & XINPUT_GAMEPAD_START){
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToResultStage");
			//auto fade = AddGameObject<FadeInOut>(Vec2(1.0f, 1.0f));
			//fade->SetIsAction(true);
		}
	}
}
//end basecross
