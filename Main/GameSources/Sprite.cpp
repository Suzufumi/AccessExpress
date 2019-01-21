#include "stdafx.h"
#include "Project.h"

namespace basecross{
	void Sprite::OnCreate() {
		float sprite_w = _rect.right - _rect.left;
		float sprite_h = _rect.bottom - _rect.top;

		float half_w = sprite_w * 0.5f;
		float half_h = sprite_h * 0.5f;

		//float uv_w = sprite_w / _size.x; // 0.1f
		float start_x = _rect.left / _size.x;
		float end_x = _rect.right / _size.x;
		float start_y = _rect.top / _size.y;
		float end_y = _rect.bottom / _size.y;

		// ���_(Vertex)�̔z������
		vector<VertexPositionTexture> vertices = {
			{Vec3(-half_w,+half_h,0.0f),Vec2(start_x,start_y)},	// 0�Ԗڂ̒��_
			{Vec3(+half_w,+half_h,0.0f),Vec2(end_x,start_y)},	// 1�Ԗڂ̒��_
			{Vec3(-half_w,-half_h,0.0f),Vec2(start_x,end_y)},	// 2�Ԗڂ̒��_
			{Vec3(+half_w,-half_h,0.0f),Vec2(end_x,end_y)}	// 3�Ԗڂ̒��_
		};

		// ���_�C���f�b�N�X(���_�̕��я�)
		vector<uint16_t> indices = {
			0, 1, 2,	// ��ڂ̃|���S��
			2, 1, 3		// ��ڂ̃|���S��
		};
		// PT = Position��Texture
		auto drawComp = AddComponent<PTSpriteDraw>(vertices, indices);
		drawComp->SetTextureResource(_key);
		SetAlphaActive(true);	// �����x�𔽉f���邩�ǂ����̃t���O

		this->SetPosition(Vec2(0, 0));
	}
	void Sprite::SetMesh(const shared_ptr<MeshResource> mesh) {
		auto drawComp = GetComponent<PTSpriteDraw>();
		drawComp->SetMeshResource(mesh);
		//this->SetPosition(Vec2(0, 0));

	}

	//------------------------------------------------------------------------------------
	/// fade�N���X
	//------------------------------------------------------------------------------------
	FadeInOut::FadeInOut(const shared_ptr<Stage>& stagePtr, const Vec2& pos)
		: Sprite(stagePtr, L"FADE_TX", Vec2(1280, 800), Rect2D<float>(1.0f, 1.0f, 1.0f, 1.0f)), m_startPos(pos)
	{
	}

	FadeInOut::~FadeInOut()
	{}

	void FadeInOut::OnCreate()
	{
		Sprite::OnCreate();
		this->SetPosition(m_startPos);
		//GetComponent<PTSpriteDraw>()->SetDrawActive(false);
		auto ptrAction = AddComponent<Action>();
		auto width = App::GetApp()->GetGameWidth();
		auto height = App::GetApp()->GetGameHeight();
		ptrAction->AddMoveTo(1.0f, Vec3((float)width, 0.0f, 0.0f));
		ptrAction->Run();
		ptrAction->SetLooped(true);
		SetDrawLayer(100);
	}

	void FadeInOut::OnUpdate()
	{
		auto ptrAction = AddComponent<Action>();
		if (m_isAction)
		{
			ptrAction->Run();
		}
		else
		{
			ptrAction->Stop();
		}
	}

	//--------------------------------------------------------------------------------------
///	�A�j���X�v���C�g
//--------------------------------------------------------------------------------------
	AnimSprite::AnimSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
		const Vec2& StartScale, const Vec2& StartPos) :
		GameObject(StagePtr),
		m_TextureKey(TextureKey),
		m_Trace(Trace),
		m_StartScale(StartScale),
		m_StartPos(StartPos),
		m_TotalTime(0.0f),
		m_Selected(false)
	{}

	AnimSprite::~AnimSprite() {}

	void AnimSprite::OnCreate() {
		float HelfSize = 0.5f;
		//���_�z��(�c��5���\��)
		vector<VertexPositionColorTexture> vertex = {
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 1.0f)) },
		};
		//�C���f�b�N�X�z��
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(m_Trace);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_StartScale.x, m_StartScale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(m_StartPos.x, m_StartPos.y, 0.0f);
		//���_�ƃC���f�b�N�X���w�肵�ăX�v���C�g�쐬
		auto PtrDraw = AddComponent<PCTSpriteDraw>(vertex, indices);
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
		PtrDraw->SetTextureResource(m_TextureKey);
	}

	void AnimSprite::OnUpdate() {
		if (m_Selected) {
			float ElapsedTime = App::GetApp()->GetElapsedTime();
			m_TotalTime += ElapsedTime * 5.0f;
			if (m_TotalTime >= XM_2PI) {
				m_TotalTime = 0;
			}
			auto PtrDraw = GetComponent<PCTSpriteDraw>();
			Col4 col(1.0, 1.0, 1.0, 1.0);
			col.w = sin(m_TotalTime) * 0.5f + 0.5f;
			PtrDraw->SetDiffuse(col);
		}
		else {
			auto PtrDraw = GetComponent<PCTSpriteDraw>();
			Col4 col(1.0, 1.0, 1.0, 1.0);
			PtrDraw->SetDiffuse(col);
		}
	}
	///------------------------------------------------------------------------------------
	//�����̃X�v���C�g
	///------------------------------------------------------------------------------------
	NumberSprite::NumberSprite(const shared_ptr<Stage>& stagePtr,int places,int num)
		: GameObject(stagePtr), m_places(places), m_num(num)
	{
	}
	///------------------------------------------------------------------------------------
	//�����̃X�v���C�g�\�z
	///------------------------------------------------------------------------------------
	void NumberSprite::OnCreate() {
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
			transComp->SetParent(GetThis<NumberSprite>());	// ��_����ʂ̍��ォ��NumberSprite�̏ꏊ�ɂȂ���
			number->SetPosition(Vec2(64 * (float)m_places - (128 + 32 + 64 * i), 128));
			transComp->SetScale(Vec3(1.2f, 0.7f, 0.9f));

			m_numbers.push_back(number);
		}
	}
	///------------------------------------------------------------------------------------
	//�����̃X�v���C�g�\�z
	///------------------------------------------------------------------------------------
	void NumberSprite::OnUpdate() {
		int number = m_num;
		for (int i = 0; i < m_places; i++) {
			int num = number % 10;	// ��̈ʂ𔲂��o��
			number /= 10;			// ��̈ʂ�؂�̂Ă�

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
	void NumberSprite::OnDraw() {
		for (auto number : m_numbers) {
			number->OnDraw();
		}
	}
}
