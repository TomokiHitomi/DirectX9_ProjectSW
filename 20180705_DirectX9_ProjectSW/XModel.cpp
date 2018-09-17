//=============================================================================
//
// ���f������ [XModel.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "XModel.h"

// �f�o�b�O�p
#ifdef _DEBUG
#include "debugproc.h"
#endif

//=============================================================================
// �R���X�g���N�^�i�����������j
//=============================================================================
CXModel::CXModel(void)
{
	// ���f���֌W�̏�����
	m_pD3DTexture = NULL;
	m_pD3DXMesh = NULL;
	m_pD3DXBuffMat = NULL;
	m_nNumMat = 0;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CXModel::Init(LPDIRECT3DDEVICE9 pDevice, LPSTR pMeshPass, LPSTR pTexPass)
{
	// ���f���f�[�^�ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(
		pMeshPass,				// ���f���f�[�^
		D3DXMESH_SYSTEMMEM,		// �g�p���郁�����̃I�v�V����
		pDevice,				// �f�o�C�X
		NULL,					// ���g�p
		&m_pD3DXBuffMat,		// �}�e���A���f�[�^
		NULL,					// ���g�p
		&m_nNumMat,				// D3DXMATERIAL�\���̂̐�
		&m_pD3DXMesh)))			// ���b�V���f�[�^�ւ̃|�C���^
	{
		return E_FAIL;
	}

	// �e�N�X�`���̓ǂݍ���
	if (FAILED(D3DXCreateTextureFromFile(
		pDevice,				// �f�o�C�X
		pTexPass,				// �t�@�C����
		&m_pD3DTexture)))		// �ǂݍ��ރ������i�����Ȃ�z��Ɂj
	{
		return E_FAIL;
	}
}

//=============================================================================
// �������
//=============================================================================
void CXModel::Release(void)
{
	if (m_pD3DTexture != NULL)
	{// �e�N�X�`���̊J��
		m_pD3DTexture->Release();
		m_pD3DTexture = NULL;
	}

	if (m_pD3DXMesh != NULL)
	{// ���b�V���̊J��
		m_pD3DXMesh->Release();
		m_pD3DXMesh = NULL;
	}

	if (m_pD3DXBuffMat != NULL)
	{// �}�e���A���̊J��
		m_pD3DXBuffMat->Release();
		m_pD3DXBuffMat = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CXModel::Update(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void CXModel::Draw(D3DXMATRIX mtxWorld)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	// ���[���h�}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	/******************** �r���[�|�[�g�ϊ� ********************/
	// ���݂̃}�e���A����ۑ�
	pDevice->GetMaterial(&matDef);
	// �}�e���A�����ɑ΂���|�C���^�̎擾
	pD3DXMat = (D3DXMATERIAL*)m_pD3DXBuffMat->GetBufferPointer();

	for (int i = 0; i < (int)m_nNumMat; i++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[i].MatD3D);
		// �e�N�X�`���̐ݒ�iNULL:�e�N�X�`�������j
		pDevice->SetTexture(0, m_pD3DTexture);
		// �`��
		m_pD3DXMesh->DrawSubset(i);
	}

	// �}�e���A�������ɖ߂�
	pDevice->SetMaterial(&matDef);
}