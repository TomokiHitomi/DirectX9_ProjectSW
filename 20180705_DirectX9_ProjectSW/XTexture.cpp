//=============================================================================
//
// 2D�e�N�X�`������ [XTexture.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "XTexture.h"

// �f�o�b�O�p
#ifdef _DEBUG
#include "debugproc.h"
#endif

//=============================================================================
// �R���X�g���N�^�i�����������j
//=============================================================================
CXTexture::CXTexture(void)
{
	// �e�N�X�`���o�b�t�@�̏�����
	m_pD3DTexture = NULL;
	m_pD3DTexture2 = NULL;

	data.vPos = D3DXVECTOR2(1920 / 2, 1080 / 2);
	data.vSize = D3DXVECTOR2(1920 / 2, 1080 / 2);
	data.nDivide.x = 1;
	data.nDivide.y = 1;
	data.xColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	data.nTexNum = 0;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CXTexture::Init(LPDIRECT3DDEVICE9 pDevice, LPSTR pTexPass)
{
	// �e�N�X�`���̓ǂݍ���
	//if (FAILED(D3DXCreateTextureFromFile(
	//	pDevice,				// �f�o�C�X
	//	pTexPass,				// �t�@�C����
	//	&m_pD3DTexture)))		// �ǂݍ��ރ������i�����Ȃ�z��Ɂj
	//{
	//	return E_FAIL;
	//}

	if (FAILED(D3DXCreateTextureFromFile(
		pDevice,				// �f�o�C�X
		"data/TEXTURE/�J�[�r�[.bmp",				// �t�@�C����
		&m_pD3DTexture)))		// �ǂݍ��ރ������i�����Ȃ�z��Ɂj
	{
		return E_FAIL;
	}


	// �e�N�X�`���̓ǂݍ���
	if (FAILED(D3DXCreateTextureFromFile(
		pDevice,				// �f�o�C�X
		"data/TEXTURE/�����ӂ��[.bmp",				// �t�@�C����
		&m_pD3DTexture2)))		// �ǂݍ��ރ������i�����Ȃ�z��Ɂj
	{
		return E_FAIL;
	}

	// ���_�̍쐬
	MakeVertex();
}

//=============================================================================
// �������
//=============================================================================
void CXTexture::Release(void)
{
	if (m_pD3DTexture != NULL)
	{// �e�N�X�`���̊J��
		m_pD3DTexture->Release();
		m_pD3DTexture = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CXTexture::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CXTexture::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTextureStageState(0,
		D3DTSS_TEXCOORDINDEX,		// �u�����h����0�Ԗڂ�UV���W���Q�Ƃ���
		1
	);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pD3DTexture);
	pDevice->SetTexture(1, m_pD3DTexture2);

	// �e�N�X�`���X�e�[�W0�̐ݒ�
	pDevice->SetTextureStageState(0,
		D3DTSS_COLOROP,				// RGB��OP��ݒ�
		D3DTOP_SELECTARG2			// ARG2�̓��͂����̂܂܏o��
	);

	pDevice->SetTextureStageState(0,
		D3DTSS_COLORARG1,			// ARG1�̐ݒ�
		D3DTA_CURRENT				// �|���S���̐F��K�p
	);

	pDevice->SetTextureStageState(0,
		D3DTSS_COLORARG2,			// ARG2�̐ݒ�
		D3DTA_TEXTURE				// �e�N�X�`��0�Ԃ����
	);

	pDevice->SetTextureStageState(0,
		D3DTSS_ALPHAOP,				// ����OP�ݒ�
		D3DTOP_SELECTARG2			// ARG2�̃f�[�^���o��
	);


	pDevice->SetTextureStageState(0,
		D3DTSS_ALPHAARG2,			// ����ARG2�̐ݒ�
		D3DTA_CURRENT				// �|���S���̐F��K�p
	);

	// �e�N�X�`���X�e�[�W1�̐ݒ�
	pDevice->SetTextureStageState(1,
		D3DTSS_COLOROP,				// RGB��OP��ݒ�
		D3DTOP_BLENDTEXTUREALPHA	// �e�N�X�`��1�̃����g���ău�����h
	);

	pDevice->SetTextureStageState(1,
		D3DTSS_COLORARG1,			// RGB��OP��ݒ�
		D3DTA_TEXTURE				// �e�N�X�`��1�̃e�N�X�`�������
	);

	pDevice->SetTextureStageState(1,
		D3DTSS_COLORARG2,			// ARG2�̐ݒ�
		D3DTA_CURRENT				// �O�̃X�e�[�W���o��
	);

	pDevice->SetTextureStageState(1,
		D3DTSS_ALPHAOP,				// ����OP�ݒ�
		D3DTOP_SELECTARG2			// ARG2�̃f�[�^���o��
	);

	pDevice->SetTextureStageState(1,
		D3DTSS_ALPHAARG1,			// ����ARG1�̐ݒ�
		D3DTA_CURRENT				// �O�̃X�e�[�W���o��
	);

	pDevice->SetTextureStageState(1,
		D3DTSS_ALPHAARG2,			// ����ARG2�̐ݒ�
		D3DTA_TEXTURE				// �e�N�X�`��1�Ԃ����
	);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, m_VertexWk, sizeof(VERTEX_2D));
	

	//pDevice->SetTextureStageState(1,
	//	D3DTSS_COLOROP,				// RGB��OP��ݒ�
	//	D3DTOP_DISABLE				// �w��X�e�[�W�ȍ~�͖���
	//);

	//pDevice->SetTextureStageState(1,
	//	D3DTSS_ALPHAOP,				// ����OP�ݒ�
	//	D3DTOP_DISABLE				// �w��X�e�[�W�ȍ~�͖���
	//);

	//pDevice->SetTextureStageState(0,
	//	D3DTSS_COLORARG1,			// ARG1�̐ݒ�
	//	D3DTA_TEXTURE				// �e�N�X�`��0�Ԃ����
	//);

	//pDevice->SetTextureStageState(0,
	//	D3DTSS_COLORARG2,			// ARG2�̐ݒ�
	//	D3DTA_CURRENT				// �|���S���̐F��K�p
	//);

	//pDevice->SetTextureStageState(0,
	//	D3DTSS_TEXCOORDINDEX,		// �u�����h����0�Ԗڂ�UV���W���Q�Ƃ���
	//	0
	//);


	//pDevice->SetTextureStageState(1,
	//	D3DTSS_TEXCOORDINDEX,		// �u�����h����0�Ԗڂ�UV���W���Q�Ƃ���
	//	1
	//);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT CXTexture::MakeVertex(void)
{
	// ���_���W�̐ݒ�
	m_VertexWk[0].vtx = D3DXVECTOR3(data.vPos.x - data.vSize.x, data.vPos.y - data.vSize.y, 0.0f);
	m_VertexWk[1].vtx = D3DXVECTOR3(data.vPos.x + data.vSize.x, data.vPos.y - data.vSize.y, 0.0f);
	m_VertexWk[2].vtx = D3DXVECTOR3(data.vPos.x - data.vSize.x, data.vPos.y + data.vSize.y, 0.0f);
	m_VertexWk[3].vtx = D3DXVECTOR3(data.vPos.x + data.vSize.x, data.vPos.y + data.vSize.y, 0.0f);

	// rhw�̐ݒ�
	m_VertexWk[0].rhw =
	m_VertexWk[1].rhw =
	m_VertexWk[2].rhw =
	m_VertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	m_VertexWk[0].diffuse =
	m_VertexWk[1].diffuse =
	m_VertexWk[2].diffuse =
	m_VertexWk[3].diffuse = data.xColor;

	// �e�N�X�`�����W�̐ݒ�
	int x = data.nTexNum % data.nDivide.x;
	int y = data.nTexNum / data.nDivide.x;
	float sizeX = 1.0f / data.nDivide.x;
	float sizeY = 1.0f / data.nDivide.y;
	m_VertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	m_VertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	m_VertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	m_VertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	return S_OK;
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT CXTexture::SetVertex(void)
{
	// ���_���W�̐ݒ�
	m_VertexWk[0].vtx = D3DXVECTOR3(data.vPos.x - data.vSize.x, data.vPos.y - data.vSize.y, 0.0f);
	m_VertexWk[1].vtx = D3DXVECTOR3(data.vPos.x + data.vSize.x, data.vPos.y - data.vSize.y, 0.0f);
	m_VertexWk[2].vtx = D3DXVECTOR3(data.vPos.x - data.vSize.x, data.vPos.y + data.vSize.y, 0.0f);
	m_VertexWk[3].vtx = D3DXVECTOR3(data.vPos.x + data.vSize.x, data.vPos.y + data.vSize.y, 0.0f);
	return S_OK;
}
