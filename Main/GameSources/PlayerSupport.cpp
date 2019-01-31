#include"stdafx.h"
#include"Project.h"

namespace basecross {
	//---------------------------------------------------------------------------------------------
	//Ray�̔���͈͂�������I�u�W�F
	//---------------------------------------------------------------------------------------------
	RayRangeViewObj::RayRangeViewObj(const shared_ptr<Stage>& StagePtr)
		:GameObject(StagePtr)
	{
	}
	void RayRangeViewObj::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(10.0f, 10.0f, 10.0f);

		auto drawComp = AddComponent<BcPNTStaticDraw>();
		drawComp->SetMeshResource(L"DEFAULT_SPHERE");
		//Col4 Color(0.4f, 1.0f, 0.7f, 0.1f);
		//drawComp->SetDiffuse(Color);
		drawComp->SetRasterizerState(RasterizerState::CullFront);
		drawComp->SetTextureResource(L"RayRange_TX");
		SetAlphaActive(true);
		SetDrawLayer(-1);

		//�v���C���[�ɃA�N�Z�X
		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
		//player�Ɛe�q�֌W�ɂȂ�
		ptrTrans->SetParent(player);
		auto camera = GetStage()->GetView()->GetTargetCamera();
		auto m_tpsCamera = dynamic_pointer_cast<TpsCamera>(camera);
		//�Ə��ʒu�Ɠ��ʒu�ɂȂ�
		ptrTrans->SetWorldPosition(
			player->GetComponent<Transform>()->GetWorldPosition() + Vec3(0, m_tpsCamera->GetCameraLookUp(),0));
	}
	void RayRangeViewObj::OnUpdate() {
		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
		//Ray�̎˒����擾���ċ��̑傫���ύX
		float scale = player->GetRayRange()*2.1f;
		GetComponent<Transform>()->SetScale(scale, scale, scale);
		if (GameManager::GetInstance().GetGameStart() == true) {
			SetDrawActive(true);
		}
		else {
			SetDrawActive(false);
		}
	}


	//--------------------------------------------------------------------------------
	//�Ə�
	//--------------------------------------------------------------------------------
	SightingDevice::SightingDevice(const shared_ptr<Stage>& stagePtr)
		:GameObject(stagePtr)
	{
	}
	void SightingDevice::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, 0, 0));
		ptrTrans->SetWorldPosition(Vec3(0, 0, 0));
		ptrTrans->SetQuaternion(Qt);
		ptrTrans->SetScale(2, 2, 2);
		//�`��R���|�[�l���g�̒ǉ�
		auto drawComp = AddComponent<BcPNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		drawComp->SetMeshResource(L"DEFAULT_SQUARE");
		//drawComp->SetMeshResource(L"DEFAULT_CUBE");
		//�e���f�肱�܂Ȃ�
		drawComp->SetOwnShadowActive(false);
		//���C�g�𖳎����Č���
		drawComp->SetLightingEnabled(false);
		//�A���t�@�l�K��
		drawComp->SetBlendState(BlendState::AlphaBlend);
		//�e�N�X�`��
		drawComp->SetTextureResource(L"cursor_TX");
		//�Ə���ʂ��Đ��E������Ƃق��̃I�u�W�F�N�g�������ɂȂ��Ă��܂��̂�
		//���C���[�ʒu��ς��邱�ƂőΉ�
		SetDrawLayer(1);
	}
	void SightingDevice::OnUpdate() {
		auto drawComp = GetComponent<BcPNTStaticDraw>();
		if (m_captureLink) {
			Col4 Color(1.0f, 0.2f, 0.2f, 1.0f);
			drawComp->SetDiffuse(Color);
		}
		else {
			Col4 Color(1.0f, 1.0f, 1.0f, 1.0f);
			drawComp->SetDiffuse(Color);
		}
	}
	//-------------------------------------------------------------------------------------------------
	//�`�F�C���̕����ƃo�[��������I�u�W�F�N�g
	//-------------------------------------------------------------------------------------------------
	ViewChainLetter::ViewChainLetter(const shared_ptr<Stage>& stagePtr)
		: Sprite(stagePtr, L"chain_gauge_TX", Vec2(155, 155)), m_maxGageLength(1.0f)
	{

	}

	void ViewChainLetter::OnCreate() {
		Sprite::OnCreate();
		SetPosition(Vec2(790, 640));
		SetDrawLayer(1);
		//�Q�[�W�o�[
		m_gage = GetStage()->AddGameObject<Sprite>(L"chainbar_TX", Vec2(155,155));
		m_gage.lock()->SetPosition(Vec2(790, 640));
		m_gage.lock()->SetDrawLayer(1);
	}
	void ViewChainLetter::OnUpdate() {
		auto& manager = GameManager::GetInstance();
		float remainingGage = m_maxGageLength - (m_maxGageLength * manager.GetSlowPassage() * manager.GetControlGageSpeed());
		m_gage.lock()->GetComponent<Transform>()->SetScale(remainingGage,1.0f , 1.0f);
		m_gage.lock()->SetPosition(Vec2(718.0f + (74*remainingGage), 640));
		if (GetStage()->GetSharedGameObject<Player>(L"Player")->GetChain() > 0) {
			SetDrawActive(true);
			m_gage.lock()->SetDrawActive(true);
		}
		else {
			SetDrawActive(false);
			m_gage.lock()->SetDrawActive(false);
		}
	}
	//-------------------------------------------------------------------------------------------------
	//�`�F�C���̐�����������I�u�W�F�N�g
	//-------------------------------------------------------------------------------------------------
	ViewChainNum::ViewChainNum(const shared_ptr<Stage>& stagePtr)
		: GameObject(stagePtr){
		m_places = 2;
	}
	//------------------------------------------------------------------------------------
	//�\�z
	//------------------------------------------------------------------------------------
	void ViewChainNum::OnCreate() {
		// �������Ƃ͈̔͂�ݒ肷��
		for (int i = 0; i < 10; i++) {
			m_numRects.push_back({
				static_cast<float>(64 * i),			// left
				static_cast<float>(0),				// top
				static_cast<float>(64 * (i + 1)),	// right
				static_cast<float>(128)				// bottom
				});
		}

		float start_x = m_numRects[5].left / 640.0f;
		float end_x = m_numRects[5].right / 640.0f;
		float start_y = m_numRects[5].top / 128.0f;
		float end_y = m_numRects[5].bottom / 128.0f;

		for (int i = 0; i < m_places; i++) {
			m_vertices.push_back({
				{ Vec3(0.0f,128.0f,0.0f),Vec2(start_x,start_y) },
				{ Vec3(64.0f,128.0f,0.0f),Vec2(end_x,start_y) },
				{ Vec3(0.0f,0.0f,0.0f),Vec2(start_x,end_y) },
				{ Vec3(64.0f,0.0f,0.0f),Vec2(end_x,end_y) }
				});
		}

		for (int i = 0; i < m_places; i++) {
			auto number = ObjectFactory::Create<Sprite>(
				GetStage(), L"Number_TX", Vec2(128,128), m_numRects[0]);
			auto transComp = number->GetComponent<Transform>();
			// GetThis��This�I�u�W�F�N�g��shared_ptr������Ă����
			transComp->SetParent(GetThis<ViewChainNum>());	// ��_����ʂ̍��ォ��ScoreUI�̏ꏊ�ɂȂ���
			number->SetPosition(Vec2(64 * (float)m_places - (128 + 32 + 64 * i), 128));
			transComp->SetScale(1.2f, 0.9f, 0.9f);
			m_numbers.push_back(number);
		}

	}

	void ViewChainNum::OnUpdate() {
		auto player = GetStage()->GetSharedGameObject<Player>(L"Player");
		int chain = player->GetChain();

		//��������ׂ�
		for (int i = 0; i < m_places; i++) {
			int num = chain % 10;	// ��̈ʂ𔲂��o��
			chain /= 10;			// ��̈ʂ�؂�̂Ă�

			float start_x = m_numRects[num].left / 640.0f;
			float end_x = m_numRects[num].right / 640.0f;
			float start_y = m_numRects[num].top / 128.0f;
			float end_y = m_numRects[num].bottom / 128.0f;

			m_vertices[i][0].textureCoordinate = Vec2(start_x, start_y);
			m_vertices[i][1].textureCoordinate = Vec2(end_x, start_y);
			m_vertices[i][2].textureCoordinate = Vec2(start_x, end_y);
			m_vertices[i][3].textureCoordinate = Vec2(end_x, end_y);

			auto drawComp = m_numbers[i]->GetComponent<PTSpriteDraw>();
			drawComp->UpdateVertices(m_vertices[i]);	// �ʒu�͕ς����Ƀ|���S���̒��g�����ς���
		}
		OnDraw();
		//auto ptrEffect = GetStage()->GetSharedGameObject<GetEffect>(L"GetEffect", false);
		//if (ptrEffect)
		//{
		//	ptrEffect->InsertGetEffect(Vec3(GetComponent<Transform>()->GetPosition()));
		//}

	}

	void ViewChainNum::OnDraw() {
		if (GetStage()->GetSharedGameObject<Player>(L"Player")->GetChain() > 0) {
			for (auto number : m_numbers) {
				number->OnDraw();
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//�X�R�A�֔��ł�����������
	//--------------------------------------------------------------------------------------
	FlyingChain::FlyingChain(const shared_ptr<Stage>& stagePtr)
		: GameObject(stagePtr) {
		m_places = 4;
	}
	//------------------------------------------------------------------------------------
	//�X�R�A�֔��ł�������
	//------------------------------------------------------------------------------------
	void FlyingChain::OnCreate() {
		// �������Ƃ͈̔͂�ݒ肷��
		for (int i = 0; i < 10; i++) {
			m_numRects.push_back({
				static_cast<float>(64 * i),			// left
				static_cast<float>(0),				// top
				static_cast<float>(64 * (i + 1)),	// right
				static_cast<float>(128)				// bottom
				});
		}

		float start_x = m_numRects[5].left / 640.0f;
		float end_x = m_numRects[5].right / 640.0f;
		float start_y = m_numRects[5].top / 128.0f;
		float end_y = m_numRects[5].bottom / 128.0f;

		for (int i = 0; i < m_places; i++) {
			m_vertices.push_back({
				{ Vec3(0.0f,128.0f,0.0f),Vec2(start_x,start_y) },
				{ Vec3(64.0f,128.0f,0.0f),Vec2(end_x,start_y) },
				{ Vec3(0.0f,0.0f,0.0f),Vec2(start_x,end_y) },
				{ Vec3(64.0f,0.0f,0.0f),Vec2(end_x,end_y) }
				});
		}

		for (int i = 0; i < m_places; i++) {
			auto number = ObjectFactory::Create<Sprite>(
				GetStage(), L"Number_TX", Vec2(640, 128), m_numRects[0]);
			auto transComp = number->GetComponent<Transform>();
			// GetThis��This�I�u�W�F�N�g��shared_ptr������Ă����
			transComp->SetParent(GetThis<FlyingChain>());	// ��_����ʂ̍��ォ��ScoreUI�̏ꏊ�ɂȂ���
			number->SetPosition(Vec2(64 * (float)m_places - (128 + 32 + 64 * i), 128));
			transComp->SetScale(1.2f, 0.9f, 0.9f);

			m_numbers.push_back(number);
		}
	}

	void FlyingChain::OnUpdate() {
		int chain = m_score;

		//��������ׂ�
		for (int i = 0; i < m_places; i++) {
			int num = chain % 10;	// ��̈ʂ𔲂��o��
			chain /= 10;			// ��̈ʂ�؂�̂Ă�

			float start_x = m_numRects[num].left / 640.0f;
			float end_x = m_numRects[num].right / 640.0f;
			float start_y = m_numRects[num].top / 128.0f;
			float end_y = m_numRects[num].bottom / 128.0f;

			m_vertices[i][0].textureCoordinate = Vec2(start_x, start_y);
			m_vertices[i][1].textureCoordinate = Vec2(end_x, start_y);
			m_vertices[i][2].textureCoordinate = Vec2(start_x, end_y);
			m_vertices[i][3].textureCoordinate = Vec2(end_x, end_y);

			auto drawComp = m_numbers[i]->GetComponent<PTSpriteDraw>();
			drawComp->UpdateVertices(m_vertices[i]);	// �ʒu�͕ς����Ƀ|���S���̒��g�����ς���
		}
		//���ł����Ԃł̏���
		if (m_isFly) {
			m_wait += App::GetApp()->GetElapsedTime();
			if (m_wait >= 0.5f) {
				m_leap += App::GetApp()->GetElapsedTime() * 2.0f;
			}
			auto pos = (1 - m_leap)*(1 - m_leap)*p0 + 2 * (1 - m_leap)*m_leap*p1 + m_leap * m_leap*p2;
			GetComponent<Transform>()->SetPosition(pos);
			if (m_leap >= 1.0f) {
				//�X�R�A�ɂ����̂ŃX�R�A�̒l�𑝉�������
				GameManager::GetInstance().AddScore(m_score);
				m_leap = 0;
				m_isFly = false;
				m_wait = 0;
				GetStage()->RemoveGameObject<FlyingChain>(GetThis<FlyingChain>());
			}
		}

		OnDraw();
	}

	void FlyingChain::OnDraw() {
		if (m_isFly) {
			int score = m_score;
			int places = 0;
			//�����\�����邩���Z�o����
			while (score > 0) {
				score /= 10;
				places++;
			}
			//�e���̕\���̐ؑ�
			int count = 0;
			for (auto number : m_numbers) {
				if (count < places || count < 3) {
					number->OnDraw();

					number->SetDrawActive(true);
				}
				else {
					number->SetDrawActive(false);
				}
				count++;
			}
		}
	}
	void FlyingChain::FlySet(int chain) {
		m_score = (chain * 30) + (chain * 10);
		m_isFly = true;
	}
}