#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	TpsCamera::TpsCamera()
		: Camera(),
		m_angleX(Deg2Rad(10.0f)),
		m_angleY(Deg2Rad(270)),
		m_maxAngleSpeed(2.0f),
		m_cameraDistance(15.0f),
		m_cameraLookUp(3.5f)
	{}

	TpsCamera::~TpsCamera()
	{}

	void TpsCamera::OnCreate(){
		
	}

	void TpsCamera::OnUpdate(){

	}

	//--------------------------------------------------------------------------------------------
	//�J�����̒l�ύX
	//--------------------------------------------------------------------------------------------
	void TpsCamera::CameraControll() {
		float delta = App::GetApp()->GetElapsedTime();
		auto m_pad = GameManager::GetInstance().GetPad();

		//�E�X�e�B�b�N�ɒl�����͂���Ă�����
		if (m_pad.fThumbRX > 0.7f || m_pad.fThumbRX < -0.7f ||
			m_pad.fThumbRY > 0.7f || m_pad.fThumbRY < -0.7f) {
			m_angleY += -m_pad.fThumbRX * m_maxAngleSpeed * 2 * delta; // �J��������]������
			m_angleX += -m_pad.fThumbRY * m_maxAngleSpeed * 2 * delta; // �J���������~������
		}
		else if (m_pad.fThumbRX > 0.2f || m_pad.fThumbRX < -0.2f ||
			m_pad.fThumbRY > 0.2f || m_pad.fThumbRY < -0.2f) {
			m_angleY += -m_pad.fThumbRX * m_maxAngleSpeed * delta; // �J��������]������
			m_angleX += -m_pad.fThumbRY * m_maxAngleSpeed * delta; // �J���������~������
		}
		//���X�e�B�b�N�ɒl�����͂���Ă�����
		if (m_pad.fThumbLX > 0.7f || m_pad.fThumbLX < -0.7f ||
			m_pad.fThumbLY > 0.7f || m_pad.fThumbLY < -0.7f) {
			m_angleY += -m_pad.fThumbLX * m_maxAngleSpeed * 2 * delta; // �J��������]������
			m_angleX += -m_pad.fThumbLY * m_maxAngleSpeed * 2 * delta; // �J���������~������
		}
		else if (m_pad.fThumbLX > 0.2f || m_pad.fThumbLX < -0.2f ||
			m_pad.fThumbLY > 0.2f || m_pad.fThumbLY < -0.2f) {
			m_angleY += -m_pad.fThumbLX * m_maxAngleSpeed * delta; // �J��������]������
			m_angleX += -m_pad.fThumbLY * m_maxAngleSpeed * delta; // �J���������~������
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

	}
	//---------------------------------------------------------------------------------------------
	//�J�����̃v���C���[�Ǐ]����
	//---------------------------------------------------------------------------------------------
	void TpsCamera::CameraRoll(Vec3 sightPos) {
		//�Ə�������
		SetAt(sightPos);
		auto eye = sightPos + Vec3(cos(m_angleY) * m_cameraDistance,
			sin(m_angleX) * m_cameraDistance, sin(m_angleY) * m_cameraDistance);
		SetEye(eye);
	}
	//--------------------------------------------------------------------------------------------
	//����_�̐ݒ�
	//--------------------------------------------------------------------------------------------
	void TpsCamera::SetBezier(weak_ptr<GameObject> playerPtr,Vec3 p2Player) {
		auto player = playerPtr.lock();
		//��ԑO�̃v���C���[�ƃJ�����̈ʒu�֌W������
		Vec3 a = player->GetComponent<Transform>()->GetWorldPosition() - GetEye();
		p0 = GetEye();		//�n�_
		p2 = p2Player - a;	//�I�_
		//�n�_�ƏI�_�̒��_����a�̕����ɂ��炵�ė^����
		p1 = (p2 - ((p2 - p0) / 2)) - a.normalize() * Vec3(p2 - p0).length();
	}
	//--------------------------------------------------------------------------------------------
	//�x�W�F�Ȑ��Ŕ�΂�(player�ƈꏏ�ɓ������߂�player�̌���Leap��������Ă���)
	//--------------------------------------------------------------------------------------------
	void TpsCamera::BezierMove(float leap, Vec3 playerPos) {
		Vec3 pos;
		pos = (1 - leap)*(1 - leap)*p0 + 2 * (1 - leap)*leap*p1 + leap * leap*p2;
		SetEye(pos);
		SetAt(playerPos + Vec3(0.0f, 3.0f, 0.0f));
	}
	//--------------------------------------------------------------------------------------------
	//��荞�ޏ����̑O�����iin�F�f�O���[�p�j
	//--------------------------------------------------------------------------------------------
	void TpsCamera::SetGoingAround(float aroundEndAngleX, float aroundEndAngleY) {
		m_aroundStartAngleX = m_angleX;
		m_aroundStartAngleY = m_angleY;
		m_aroundEndAngleX = aroundEndAngleX;
		m_aroundEndAngleY = aroundEndAngleY;
		m_leap = 0.0f;
	}
	//--------------------------------------------------------------------------------------------
	//��荞�ޏ����iout�F��荞�ݏI�������true�j
	//--------------------------------------------------------------------------------------------
	bool TpsCamera::GoingAround() {
		if (m_leap < 1.0f) {
			m_leap += App::GetApp()->GetElapsedTime() * 2;
			m_angleX = (1 - m_leap) * m_aroundStartAngleX + m_leap * m_aroundEndAngleX;
			m_angleY = (1 - m_leap) * m_aroundStartAngleY + m_leap * m_aroundEndAngleY;
			return false;
		}
		else {
			m_leap = 1.0f;
			return true;
		}
	}
	///---------------------------------------------------------------------------------------------
	//���b�N�I�����Ă���Ώۂ�����ۂ̃J��������
	///---------------------------------------------------------------------------------------------
	void TpsCamera::RockonCameraMove(Vec3 sightPos,Vec3 linkPos) {
		float deltaX = sightPos.x - linkPos.x;
		float deltaZ = sightPos.z - linkPos.z;
		float deltaY = sightPos.y - linkPos.y;
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

	///--------------------------------------------------------------------------------------------
	//�J�����ƂƂ��ɓ�������̃R���X�g���N�^
	///--------------------------------------------------------------------------------------------
	TpsCameraJudgment::TpsCameraJudgment(const shared_ptr<Stage>& StagePtr)
		:GameObject(StagePtr)
	{
	}

	//--------------------------------------------------------------------------------------------
	//�쐬
	//--------------------------------------------------------------------------------------------
	void TpsCameraJudgment::OnCreate() {
		GetComponent<Transform>()->SetScale(3.0f, 3.0f, 3.0f);

		AddComponent<CollisionObb>();
	}
	//--------------------------------------------------------------------------------------------
	//�X�V
	//--------------------------------------------------------------------------------------------
	void TpsCameraJudgment::OnUpdate() {
		auto camera = GetStage()->GetView()->GetTargetCamera();
		auto tpsCamera = dynamic_pointer_cast<TpsCamera>(camera);
		GetComponent<Transform>()->SetWorldPosition(tpsCamera->GetEye());
	}
	//--------------------------------------------------------------------------------------------
	//�Փˎ�����
	//--------------------------------------------------------------------------------------------
	void TpsCameraJudgment::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		auto wall = dynamic_pointer_cast<Wall>(Other);
		//�ǂɃJ����������������A�ǂ������Ȃ�����
		if (wall) {
			wall->SetDrawActive(false);
		}
	}
	//--------------------------------------------------------------------------------------------
	//�Փˉ���������
	//--------------------------------------------------------------------------------------------
	void TpsCameraJudgment::OnCollisionExit(shared_ptr<GameObject>& Other) {
		auto wall = dynamic_pointer_cast<Wall>(Other);
		//�Փ˂��������ꂽ�ǂ�������悤�ɂ���
		if (wall) {
			wall->SetDrawActive(true);
		}
	}
}