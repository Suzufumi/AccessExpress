#include "stdafx.h"
#include "Project.h"

namespace basecross {
	Wall::Wall(const shared_ptr<Stage>& StagePtr, Vec3 pos, Vec3 scale)
		: GameObject(StagePtr), m_position(pos), m_scale(scale)
	{
	}
	//--------------------------------------------------------------------------------------
	//������
	//-------------------------------------------------------------------------------------
	void Wall::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();
		auto col = AddComponent<CollisionObb>();
		col->SetAfterCollision(AfterCollision::None);
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, 0, 0));
		ptrTrans->SetWorldPosition(m_position);
		ptrTrans->SetQuaternion(Qt);
		ptrTrans->SetScale(m_scale);
		Mat4x4 spanMat; // ���f���ƃg�����X�t�H�[���̊Ԃ̍����s��
		spanMat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		//�`��R���|�[�l���g�̒ǉ�
		auto drawComp = AddComponent<PNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		drawComp->SetMeshResource(L"DEFAULT_CUBE");
		drawComp->SetTextureResource(L"Building1_TX");
		drawComp->SetBlendState(BlendState::AlphaBlend);
		//drawComp->SetMeshToTransformMatrix(spanMat);
		//Col4 Color(0.4f, 0.4f, 0.4f, 1.0f);
		//drawComp->SetDiffuse(Color);
	}

	Goal::Goal(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale)
		: GameObject(stagePtr), m_position(pos), m_scale(scale)
	{
	}

	void Goal::OnCreate() {
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
		Col4 Color(0.0f, 1.0f, 1.0f, 1.0f);
		drawComp->SetDiffuse(Color);
	}
	void Goal::OnUpdate() {

	}
	void Goal::ArriveGoal() {
		PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToResultStage");
	}

	//---------------------------------------------------------------------------------------------
	//�d�g���̃R���X�g���N�^
	//-------------------------------------------------------------------------------------------
	RadioTower::RadioTower(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale,float acceleration) 
		:GameObject(stagePtr),m_position(pos),m_scale(scale),m_acceleration(acceleration)
	{
	}
	void RadioTower::OnCreate() {
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
		drawComp->SetMeshResource(L"DEFAULT_TETRAHEDRON");
		//drawComp->SetFogEnabled(true);
		//�����ɉe���f�肱�ނ悤�ɂ���
		Col4 Color(1.0f, 1.0f, 1.0f, 0.7f);
		drawComp->SetDiffuse(Color);
	}
	void RadioTower::OnUpdate() {

	}
	float RadioTower::GetAcceleration() {
		return m_acceleration;
	}

	//-------------------------------------------------------------------------------------------
	//�h���C�u
	//-------------------------------------------------------------------------------------------
	Drive::Drive(const shared_ptr<Stage>& stagePtr, Vec3 pos, Vec3 scale)
		:GameObject(stagePtr), m_position(pos), m_scale(scale)
	{
	}
	void Drive::OnCreate() {
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
		Col4 Color(1.0f, 1.0f, 0.4f, 0.7f);
		drawComp->SetDiffuse(Color);
		//drawComp->SetColorAndAlpha(Color);
	}

	//--------------------------------------------------------------------------------------------------------------
	// �w�i�p�̋�
	//--------------------------------------------------------------------------------------------------------------
	void SkySphere::OnCreate()
	{
		auto camera = GetStage()->GetView()->GetTargetCamera();

		auto transComp = GetComponent<Transform>();
		transComp->SetScale(Vec3(200, 200, 200));
		transComp->SetPosition(camera->GetEye());

		vector<VertexPositionNormalTexture> vertices;
		//���_��`��ς���
		vector<VertexPositionColorTexture> new_vertices;
		
		vector<uint16_t> indices;
		//���̂̍쐬
		MeshUtill::CreateSphere(1.0f, 18, vertices, indices);
		for (size_t i = 0; i < vertices.size(); i++)
		{
			VertexPositionColorTexture temp;
			temp.position = vertices[i].position;
			temp.textureCoordinate = vertices[i].textureCoordinate;
			temp.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
			new_vertices.push_back(temp);
		}

		auto drawComp = AddComponent<PCTStaticDraw>();
		drawComp->CreateOriginalMesh(new_vertices, indices);
		drawComp->SetOriginalMeshUse(true);
		drawComp->SetTextureResource(L"SKY_TX");
		drawComp->SetSamplerState(SamplerState::LinearClamp);
		drawComp->SetDepthStencilState(DepthStencilState::Read);
		SetAlphaActive(true);

	}

	void SkySphere::OnUpdate()
	{
		auto camera = GetStage()->GetView()->GetTargetCamera();
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(camera->GetEye());
	}


	//--------------------------------------------------------------------------------------------------------------
	// �w�i�p�̃X�v���C�g���쐬
	//--------------------------------------------------------------------------------------------------------------
	SkySprite::SkySprite(const shared_ptr<Stage>& ptrStage, const Vec3 &rot, const Vec3 &pos)
		: GameObject(ptrStage), m_rotation(rot), m_position(pos)
	{
		m_scale = Vec2(100, 75);
	}

	void SkySprite::OnCreate()
	{
		float helfSize = 0.5f;
		//���_�z��(�c��5���\��)
		vector<VertexPositionColorTexture> vertices = {
			{ VertexPositionColorTexture(Vec3(-helfSize, helfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(helfSize, helfSize, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(5.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-helfSize, -helfSize, 0), Col4(1.0f, 0.0f, 1.0f, 1.0f), Vec2(0.0f, 5.0f)) },
			{ VertexPositionColorTexture(Vec3(helfSize, -helfSize, 0), Col4(1.0f,1.0f, 1.0f, 1.0f), Vec2(5.0f, 5.0f)) },
		};
		//�C���f�b�N�X�z��
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(true);
		auto transComp = GetComponent<Transform>();
		transComp->SetScale(Vec3(m_scale.x, m_scale.y, 1.0f));
		transComp->SetRotation(m_rotation);
		transComp->SetPosition(m_position);

		//���_�ƃC���f�b�N�X���w�肵�ăX�v���C�g�쐬
		auto drawComp = AddComponent<PNTStaticDraw>();
		//auto drawComp = AddComponent<PCTSpriteDraw>(vertices, indices);
		drawComp->SetMeshResource(L"DEFAULT_SQUARE");
		drawComp->SetSamplerState(SamplerState::LinearWrap);
		drawComp->SetTextureResource(L"SKY_TX");
		SetDrawLayer(-2);
	}

	void SkySprite::OnUpdate()
	{

	}
}
