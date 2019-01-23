#pragma once
#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//------------------------------------------------------------------------------------------------------------
	//player�ɂ�������Ray�̓͂��͈͂�������I�u�W�F�N�g
	//------------------------------------------------------------------------------------------------------------
	class RayRangeViewObj : public GameObject {
		float m_range;
	public:
		RayRangeViewObj(const shared_ptr<Stage>& StagePtr);
		virtual ~RayRangeViewObj() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		void GetRange(float range) { m_range = range; };
	};

	//--------------------------------------------------------------------------------------
	//�����N�I�u�W�F�N�g�𑨂��邽�߂̏Ə�
	//--------------------------------------------------------------------------------------
	class SightingDevice : public GameObject {
		bool m_captureLink = false;
	public:
		SightingDevice(const shared_ptr<Stage>& stageptr);
		virtual ~SightingDevice() {};
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
		void SetCaptureLink(bool f) { m_captureLink = f; }
		void ResetCaptureLink() { m_captureLink = false; }
	};
	//--------------------------------------------------------------------------------------
	//�`�F�C���̕����ƃo�[
	//--------------------------------------------------------------------------------------
	class ViewChainLetter : public Sprite {
		weak_ptr<Sprite> m_gage;
		public:
			ViewChainLetter(const shared_ptr<Stage>& stagePtr);
			virtual ~ViewChainLetter() {};
			virtual void OnCreate() override;
			virtual void OnUpdate() override;
	};
	//--------------------------------------------------------------------------------------
	//�`�F�C���̐���
	//--------------------------------------------------------------------------------------
	class ViewChainNum : public GameObject {
	protected:
		int m_places;	// �\����
		vector <Rect2D<float>> m_numRects;
		vector<shared_ptr<Sprite>>m_numbers;
		vector<vector<VertexPositionTexture>> m_vertices;
	public:
		ViewChainNum(const shared_ptr<Stage>& stagePtr);
		virtual ~ViewChainNum() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDraw() override;
	};
	//--------------------------------------------------------------------------------------
	//�X�R�A�ɔ��ł����`�F�C���̐���
	//--------------------------------------------------------------------------------------
	class FlyingChain : public GameObject {
	protected:
		int m_places;	// �\����
		int m_score;	// �\������X�R�A
		float m_wait;
		float m_leap;
		Vec3 p0 = Vec3(880, -380, 0);
		Vec3 p1 = Vec3(1000,-400,0);
		Vec3 p2 = Vec3(1152, 0, 0);

		bool m_isFly;	//���ł���Ƃ�true
		vector <Rect2D<float>> m_numRects;
		vector<shared_ptr<Sprite>>m_numbers;
		vector<vector<VertexPositionTexture>> m_vertices;
	public:
		FlyingChain(const shared_ptr<Stage>& stagePtr);
		virtual ~FlyingChain() {};
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDraw() override;
		void FlySet(int chain);
	};
	//--------------------------------------------------------------------------------------
	///	���C��
	//--------------------------------------------------------------------------------------
	class ActionLine : public GameObject {
		Vec3 m_StartObj;
		Vec3 m_EndObj;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		@param[in]	StartObj	�X�^�[�g�̃I�u�W�F�N�g
		@param[in]	EndObj	�G���h�̃I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		ActionLine(const shared_ptr<Stage>& StagePtr, Vec3 StartObj,Vec3 EndObj);
		virtual ~ActionLine() {}
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
		//���̃X�^�[�g�ƃG���h�I�u�W�F�N�g�̐ݒ�
		void ResetObject(Vec3 StartObj, Vec3 EndObj) {
			m_StartObj = StartObj;
			m_EndObj = EndObj;
		}
		void SetOnDraw(bool f);
	};
}