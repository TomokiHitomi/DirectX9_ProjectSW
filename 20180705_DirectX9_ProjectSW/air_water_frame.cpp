//=============================================================================
//
// �G�A�[�E�E�H�[�^�[�g���� [air_water_frame.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "main.h"
#include "air_water_frame.h"


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
AirWaterFream::AirWaterFream(void)
{

	// �I�u�W�F�N�gID�ƃv���C�I���e�B�̐ݒ菈��
	SetIdAndPriority(ObjectID::FRAME, Priority::Low, Priority::Lowest);


	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// �e�N�X�`���̓ǂݍ��݁iDebug�̓t�@�C���^Release�̓A�[�J�C�u����j
#ifdef _DEBUG
	if (FAILED(D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�̃|�C���^
		TEXTURE_AIR_WATER_FRAME,					// �t�@�C���̖��O
		&m_pD3DTexture)))				// �ǂݍ��ރ������̃|�C���^
#else
	if (FAILED(D3DXCreateTextureFromArchive(pDevice,	// �f�o�C�X�̃|�C���^
		TEXTURE_AIR_WATER_FRAME,					// �t�@�C���̖��O
		&m_pD3DTexture)))				// �ǂݍ��ރ������̃|�C���^
#endif
	{
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", "AirWaterFream", MB_OK);
	}

	// �G�A�[�p�̍��W�ɐݒ�
	tagAWFS[AIR].vecPos = 
		D3DXVECTOR3((float)AIR_POS_FRAME_X, (float)AIR_POS_FRAME_Y, 0.0f);
	// �E�H�[�^�[�p�̍��W�ɐݒ�
	tagAWFS[WATER].vecPos = 
		D3DXVECTOR3((float)WATER_FRAME_POS_X, (float)WATER_FRAME_POS_Y, 0.0f);

	// ���ʃv���p�e�B�̏�����
	for (unsigned int i = 0; i < FREAM_MAX; i++)
	{
		// �g�p�t���O��true�ɐݒ�
		tagAWFS[i].bUse = true;
		// ���_���̍쐬
		MakeVertex(i);
	}
}

//=============================================================================
// �f�X�g���N�^�i�I�������j
//=============================================================================
AirWaterFream::~AirWaterFream(void)
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
//void AirWaterFream::Update(void)
//{
//		// �X�V�����Ȃ�
//}

//=============================================================================
// �`�揈��
//=============================================================================
void AirWaterFream::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (unsigned int i = 0; i < FREAM_MAX; i++)
	{
		// �g�p�t���O��ture�̏ꍇ
		if (tagAWFS[i].bUse)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, m_pD3DTexture);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

			// �|���S���̕`��
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, tagAWFS[i].vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT AirWaterFream::MakeVertex(int nNum)
{
	// ���_���W�̐ݒ�
	tagAWFS[nNum].vertexWk[0].vtx = D3DXVECTOR3(tagAWFS[nNum].vecPos.x - TEXTURE_AIR_WATER_FRAME_SIZE_X, tagAWFS[nNum].vecPos.y - TEXTURE_AIR_WATER_FRAME_SIZE_Y, 0.0f);
	tagAWFS[nNum].vertexWk[1].vtx = D3DXVECTOR3(tagAWFS[nNum].vecPos.x + TEXTURE_AIR_WATER_FRAME_SIZE_X, tagAWFS[nNum].vecPos.y - TEXTURE_AIR_WATER_FRAME_SIZE_Y, 0.0f);
	tagAWFS[nNum].vertexWk[2].vtx = D3DXVECTOR3(tagAWFS[nNum].vecPos.x - TEXTURE_AIR_WATER_FRAME_SIZE_X, tagAWFS[nNum].vecPos.y + TEXTURE_AIR_WATER_FRAME_SIZE_Y, 0.0f);
	tagAWFS[nNum].vertexWk[3].vtx = D3DXVECTOR3(tagAWFS[nNum].vecPos.x + TEXTURE_AIR_WATER_FRAME_SIZE_X, tagAWFS[nNum].vecPos.y + TEXTURE_AIR_WATER_FRAME_SIZE_Y, 0.0f);

	// rhw�̐ݒ�
	tagAWFS[nNum].vertexWk[0].rhw =
	tagAWFS[nNum].vertexWk[1].rhw =
	tagAWFS[nNum].vertexWk[2].rhw =
	tagAWFS[nNum].vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	tagAWFS[nNum].vertexWk[0].diffuse = AIR_WATER_FRAME_DIFFUSE;
	tagAWFS[nNum].vertexWk[1].diffuse = AIR_WATER_FRAME_DIFFUSE;
	tagAWFS[nNum].vertexWk[2].diffuse = AIR_WATER_FRAME_DIFFUSE;
	tagAWFS[nNum].vertexWk[3].diffuse = AIR_WATER_FRAME_DIFFUSE;

	// �e�N�X�`�����W�̐ݒ�
	int x = nNum % TEXTURE_PATTERN_DIVIDE_X_AWFREAM;
	int y = nNum / TEXTURE_PATTERN_DIVIDE_X_AWFREAM;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_AWFREAM;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_AWFREAM;
	tagAWFS[nNum].vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	tagAWFS[nNum].vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	tagAWFS[nNum].vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	tagAWFS[nNum].vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	return S_OK;
}
