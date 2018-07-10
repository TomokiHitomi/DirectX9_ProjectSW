//=============================================================================
//
// �R�s�[���C�g���� [copyright.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "main.h"
#include "copyright.h"


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
Copyright::Copyright(void)
{
	// �I�u�W�F�N�gID�ƃv���C�I���e�B�̐ݒ菈��
	SetIdAndPriority(ObjectID::COPYRIGHT, Priority::High, Priority::Normal);

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// �e�N�X�`���̓ǂݍ��݁iDebug�̓t�@�C���^Release�̓A�[�J�C�u����j
#ifdef _DEBUG
	if (FAILED(D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
		TEXTURE_COPYRIGHT,							// �t�@�C���̖��O
		&m_pD3DTexture)))							// �ǂݍ��ރ������̃|�C���^
#else
	if (FAILED(D3DXCreateTextureFromArchive(pDevice,	// �f�o�C�X�̃|�C���^
		TEXTURE_COPYRIGHT,								// �t�@�C���̖��O
		&m_pD3DTexture)))								// �ǂݍ��ރ������̃|�C���^
#endif
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", "Copyright", MB_OK);
	}

	// �G�A�[�p�̍��W�ɐݒ�
	tagUi[CORYRIGHT_LOGO].vecPos = COPYRIGHT_POS;

	// ���ʃv���p�e�B�̏�����
	for (unsigned int i = 0; i < TEXTURE_COPYRIGHT_MAX; i++)
	{
		// �g�p�t���O��true�ɐݒ�
		tagUi[i].bUse = true;
		// ���_���̍쐬
		MakeVertex(i);
	}
}

//=============================================================================
// �f�X�g���N�^�i�I�������j
//=============================================================================
Copyright::~Copyright(void)
{
	if(m_pD3DTexture != NULL )
	{	// �e�N�X�`���̊J��
		m_pD3DTexture->Release();
		m_pD3DTexture = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
//void Copyright::Update(void)
//{
//		// �X�V�����Ȃ�
//}

//=============================================================================
// �`�揈��
//=============================================================================
void Copyright::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (unsigned int i = 0; i < TEXTURE_COPYRIGHT_MAX; i++)
	{
		// �g�p�t���O��ture�̏ꍇ
		if (tagUi[i].bUse)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, m_pD3DTexture);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, tagUi[i].vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT Copyright::MakeVertex(int nNum)
{
	// ���_���W�̐ݒ�
	tagUi[nNum].vertexWk[0].vtx = D3DXVECTOR3(tagUi[nNum].vecPos.x - TEXTURE_COPYRIGHT_SIZE_X, tagUi[nNum].vecPos.y - TEXTURE_COPYRIGHT_SIZE_Y, 0.0f);
	tagUi[nNum].vertexWk[1].vtx = D3DXVECTOR3(tagUi[nNum].vecPos.x + TEXTURE_COPYRIGHT_SIZE_X, tagUi[nNum].vecPos.y - TEXTURE_COPYRIGHT_SIZE_Y, 0.0f);
	tagUi[nNum].vertexWk[2].vtx = D3DXVECTOR3(tagUi[nNum].vecPos.x - TEXTURE_COPYRIGHT_SIZE_X, tagUi[nNum].vecPos.y + TEXTURE_COPYRIGHT_SIZE_Y, 0.0f);
	tagUi[nNum].vertexWk[3].vtx = D3DXVECTOR3(tagUi[nNum].vecPos.x + TEXTURE_COPYRIGHT_SIZE_X, tagUi[nNum].vecPos.y + TEXTURE_COPYRIGHT_SIZE_Y, 0.0f);

	// rhw�̐ݒ�
	tagUi[nNum].vertexWk[0].rhw =
	tagUi[nNum].vertexWk[1].rhw =
	tagUi[nNum].vertexWk[2].rhw =
	tagUi[nNum].vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	tagUi[nNum].vertexWk[0].diffuse = COPYRIGHT_DIFFUSE;
	tagUi[nNum].vertexWk[1].diffuse = COPYRIGHT_DIFFUSE;
	tagUi[nNum].vertexWk[2].diffuse = COPYRIGHT_DIFFUSE;
	tagUi[nNum].vertexWk[3].diffuse = COPYRIGHT_DIFFUSE;

	// �e�N�X�`�����W�̐ݒ�
	int x = nNum % TEXTURE_PATTERN_DIVIDE_X_COPYRIGHT;
	int y = nNum / TEXTURE_PATTERN_DIVIDE_X_COPYRIGHT;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_COPYRIGHT;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_COPYRIGHT;
	tagUi[nNum].vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	tagUi[nNum].vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	tagUi[nNum].vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	tagUi[nNum].vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	return S_OK;
}
