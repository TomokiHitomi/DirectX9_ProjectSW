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
	if (FAILED(D3DXCreateTextureFromFile(
		pDevice,				// �f�o�C�X
		pTexPass,				// �t�@�C����
		&m_pD3DTexture)))		// �ǂݍ��ރ������i�����Ȃ�z��Ɂj
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

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pD3DTexture);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, m_VertexWk, sizeof(VERTEX_2D));
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
