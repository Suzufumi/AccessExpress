#include"stdafx.h"
#include"Project.h"

namespace basecross {
	//---------------------------------------------------------------------------------------------
	//�d�g���Ƃ̏Փ˔�����s�������蔻��̃R���X�g���N�^
	//---------------------------------------------------------------------------------------------
	RadioTowerHitJudgment::RadioTowerHitJudgment(const shared_ptr<Stage>& StagePtr, weak_ptr<GameObject> player)
		:GameObject(StagePtr), m_player(player)
	{
	}
	void RadioTowerHitJudgment::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(10.0f, 10.0f, 10.0f);

		//���`�̓����蔻����Z�b�g
		auto col = AddComponent<CollisionSphere>();
		//�I�u�W�F�N�g�Ɠ������Ă��������Ȃ�
		col->SetAfterCollision(AfterCollision::None);
		//�����蔻�肪������悤�ɂ���
		//col->SetDrawActive(true);
		//Player�ƂԂ���Ȃ��悤�ɂ��邽�߂̃^�O
		AddTag(L"PlayerUse");
		//�R���X�g���N�^�Ŏ擾�����v���C���[�ɃA�N�Z�X
		auto obj = m_player.lock();
		//player�Ɛe�q�֌W�ɂȂ�
		ptrTrans->SetParent(obj);
		//player�Ɠ��ʒu�ɂȂ�
		ptrTrans->SetWorldPosition(obj->GetComponent<Transform>()->GetWorldPosition());
	}
	//-------------------------------------------------------------------------------------------------------------
	//�q�b�g���Ă���Œ�
	//-------------------------------------------------------------------------------------------------------------
	void RadioTowerHitJudgment::OnCollisionExcute(shared_ptr<GameObject>& Other) {
		//�Փ˂������̂��d�g�������ׂ�
		//auto radioTower = dynamic_pointer_cast<RadioTower>(Other);
		////�d�g��������
		//if (radioTower) {
		//	//Player�ʒu����d�g���̈ʒu������
		//	Vec3 d = this->GetComponent<Transform>()->GetWorldPosition() - radioTower->GetComponent<Transform>()->GetWorldPosition();
		//	//�ʒu�̍����狗�����o��
		//	float leng = d.length();
		//	//�����o�������������ݎ����Ă��鋗�������߂�������
		//	if (m_length > leng) {
		//		//��ԋ߂��������X�V����
		//		m_length = leng;
		//		//���x���擾����
		//		m_Acceleration = radioTower->GetAcceleration();
		//	}
		//}
	}

	//-------------------------------------------------------------------------------------
	//�t�@�C��
	//-------------------------------------------------------------------------------------
	File::File(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale)
		:GameObject(stagePtr), m_position(pos), m_scale(scale)
	{
	}
	//-------------------------------------------------------------------------------------
	//������
	//-------------------------------------------------------------------------------------
	void File::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		auto col = AddComponent<CollisionObb>();
		col->SetAfterCollision(AfterCollision::None);
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, 0, 0));
		ptrTrans->SetWorldPosition(m_position);
		ptrTrans->SetQuaternion(Qt);
		ptrTrans->SetScale(m_scale);
		//�`��R���|�[�l���g�̒ǉ�
		auto drawComp = AddComponent<PNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		drawComp->SetMeshResource(L"DEFAULT_CUBE");
		//drawComp->SetFogEnabled(true);
		//�����ɉe���f�肱�ނ悤�ɂ���
		drawComp->SetOwnShadowActive(true);
		Col4 Color(0.4f, 1.0f, 0.7f, 0.7f);
		drawComp->SetDiffuse(Color);
		//drawComp->SetColorAndAlpha(Color);
		//�^�O�t��
		AddTag(L"File");
	}
	void File::SetOnPlayer(weak_ptr<GameObject> obj) {
		auto trans = GetComponent<Transform>();
		//�I�u�W�F�N�g�ɃA�N�Z�X
		auto player = obj.lock();
		//�I�u�W�F�N�g��e�ɂ���
		trans->SetParent(player);
		//�e�̏�ɏ��
		trans->SetPosition(Vec3(0, player->GetComponent<Transform>()->GetScale().y / 2 + m_scale.y / 2, 0));
	}
	void File::LookFile() {
		auto drawComp = GetComponent<DrawComponent>();
		drawComp->SetDrawActive(true);
	}
	void File::UnLookFile() {
		auto drawComp = GetComponent<DrawComponent>();
		drawComp->SetDrawActive(false);
	}

	AddlessCertificate::AddlessCertificate(const shared_ptr<Stage>& stage, Vec3 pos, Vec3 scale)
		:OBBObject(stage, pos, scale)
	{
	}
	void AddlessCertificate::OnCreate() {
		//�e�I�u�W�F�N�g�̃R���X�g���N�^���ĂԁA�����Ō`�̌`������
		OBBObject::OnCreate();
		Col4 Color(1.0f, 0.3f, 0.7f, 0.7f);
		GetComponent<PNTStaticDraw>()->SetDiffuse(Color);
		GetComponent<CollisionObb>()->SetDrawActive(true);
	}
	void AddlessCertificate::FindGoal() {
		auto goal = m_goal.lock();
		//goal->OpenGoal();
		m_isUnlockGoal = true;
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
	//�`�F�C���̕�����������I�u�W�F�N�g
	//-------------------------------------------------------------------------------------------------
	ViewChainLetter::ViewChainLetter(const shared_ptr<Stage>& stagePtr)
		: Sprite(stagePtr, L"chain_TX", Vec2(124, 32))
	{

	}

	void ViewChainLetter::OnCreate() {
		Sprite::OnCreate();
		SetPosition(Vec2(740, 640));
		SetDrawLayer(1);
	}
	void ViewChainLetter::OnUpdate() {
		if (GetStage()->GetSharedGameObject<Player>(L"Player")->GetChain() > 0) {
			SetDrawActive(true);
		}
		else {
			SetDrawActive(false);
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
				GetStage(), L"Number2_TX", Vec2(640,128), m_numRects[0]);
			auto transComp = number->GetComponent<Transform>();
			// GetThis��This�I�u�W�F�N�g��shared_ptr������Ă����
			transComp->SetParent(GetThis<ViewChainNum>());	// ��_����ʂ̍��ォ��ScoreUI�̏ꏊ�ɂȂ���
			number->SetPosition(Vec2(64 * (float)m_places - (128 + 32 + 64 * i), 128));
			m_numbers.push_back(number);
		}

		auto flyingChain = GetStage()->AddGameObject<FlyingChain>();
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
	}

	void ViewChainNum::OnDraw() {
		if (GetStage()->GetSharedGameObject<Player>(L"Player")->GetChain() > 0) {
			for (auto number : m_numbers) {
				number->OnDraw();
			}
		}
	}

	//-------------------------------------------------------------------------------------------------
	//�X���[�̎c�莞�Ԃ�������I�u�W�F�N�g
	//-------------------------------------------------------------------------------------------------
	SlowTimeUI::SlowTimeUI(const shared_ptr<Stage>& stagePtr)
		: Sprite(stagePtr, L"SlowTimeGage_TX", Vec2(32, 124))
	{

	}

	void SlowTimeUI::OnCreate() {
		Sprite::OnCreate();
		SetPosition(Vec2(540, 600));
		SetDrawLayer(2);
		//SLOW�̕����̃X�v���C�g
		m_text = GetStage()->AddGameObject<Sprite>(L"SlowText_TX", Vec2(32,124));
		m_text.lock()->SetPosition(Vec2(540,600));
		m_text.lock()->SetDrawLayer(3);
		//�Q�[�W�o�[�̔w��̓y��
		m_gageBase = GetStage()->AddGameObject<Sprite>(L"SlowTimeGageBase_TX", Vec2(32, 124));
		m_gageBase.lock()->SetPosition(Vec2(540, 600));
		m_gageBase.lock()->SetDrawLayer(1);
	}
	void SlowTimeUI::OnUpdate() {
		auto& manager = GameManager::GetInstance();
		float remainingGage = 1.0f - manager.GetSlowPassage();
		GetComponent<Transform>()->SetScale(1.0f, remainingGage, 1.0f);
		SetPosition(Vec2(540.0f, 662.0f - (62.0f * remainingGage)));

		if (manager.GetOnSlow()) {
			SetDrawActive(true);
			m_text.lock()->SetDrawActive(true);
			m_gageBase.lock()->SetDrawActive(true);
		}
		else {
			SetDrawActive(false);
			m_text.lock()->SetDrawActive(false);
			m_gageBase.lock()->SetDrawActive(false);
		}
	}
	//--------------------------------------------------------------------------------------
	//�`�F�C���̐���
	//--------------------------------------------------------------------------------------
	FlyingChain::FlyingChain(const shared_ptr<Stage>& stagePtr)
		: GameObject(stagePtr) {
		m_places = 2;
	}
	//------------------------------------------------------------------------------------
	//�\�z
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
				GetStage(), L"Number2_TX", Vec2(640, 128), m_numRects[0]);
			auto transComp = number->GetComponent<Transform>();
			// GetThis��This�I�u�W�F�N�g��shared_ptr������Ă����
			transComp->SetParent(GetThis<FlyingChain>());	// ��_����ʂ̍��ォ��ScoreUI�̏ꏊ�ɂȂ���
			number->SetPosition(Vec2(64 * (float)m_places - (128 + 32 + 64 * i), 128));
			m_numbers.push_back(number);
		}
		GetStage()->SetSharedGameObject(L"FlyingChain", GetThis<FlyingChain>());
	}

	void FlyingChain::OnUpdate() {
		int chain = m_chain;

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
			m_leap += App::GetApp()->GetElapsedTime() * 2.0f;
			auto pos = (1 - m_leap)*(1 - m_leap)*p0 + 2 * (1 - m_leap)*m_leap*p1 + m_leap * m_leap*p2;
			GetComponent<Transform>()->SetPosition(pos);
			if (m_leap >= 1.0f) {
				//�X�R�A�ɂ����̂ŃX�R�A�̒l�𑝉�������
				GameManager::GetInstance().AddScore(m_chain * 30 + m_chain * 10);
				m_leap = 0;
				m_isFly = false;
			}
		}

		OnDraw();
	}

	void FlyingChain::OnDraw() {
		if (m_isFly) {
			for (auto number : m_numbers) {
				number->OnDraw();
			}
		}
	}
	void FlyingChain::FlySet(int chain) {
		m_chain = chain;
		m_isFly = true;
	}
}