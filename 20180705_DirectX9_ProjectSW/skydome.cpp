//=============================================================================
//
// �X�J�C�h�[������ [skydome.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "main.h"
#include "skydome.h"
#include "calculate.h"

// �f�o�b�O�p
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^�i�����������j
//=============================================================================
Skydome::Skydome(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �I�u�W�F�N�gID�ƃv���C�I���e�B�̐ݒ菈��
	SetIdAndPriority(ObjectID::SKYDOME, Priority::Middle, Priority::Highest);

	// �e�v���p�e�B�̏�����
	m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vScl = D3DXVECTOR3(MODEL_SKYDOME_SCL, MODEL_SKYDOME_SCL, MODEL_SKYDOME_SCL);
	m_bUse = true;

	// ���f���̏�����
	//m_CSkinMesh->Init(pDevice, "data/MODEL/Yuko.x");
	m_CXModel = new CXModel;
	m_CXModel->Init(pDevice, SKYDOME_MODEL, SKYDOME_TEXTURE);

}

//=============================================================================
// �f�X�g���N�^�i�I�������j
//=============================================================================
Skydome::~Skydome(void)
{
	if (m_CXModel != NULL)
	{
		m_CXModel->Release();
		delete m_CXModel;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void Skydome::Update(void)
{
	if (m_bUse)
	{
		// �A�j���[�V�����X�V����
		m_CXModel->Update();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void Skydome::Draw(void)
{
	if (m_bUse)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// ���[���h�ϊ�
		WorldConvert(&m_mtxWorld, m_vPos, m_vRot, m_vScl);

		// ���C���e�B���O�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		m_CXModel->Draw(m_mtxWorld);

		// ���C���e�B���O��L���ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	}
}