//=============================================================================
//
// �G�A�[�E�E�H�[�^�[�g [air_water_frame.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _AIR_WATER_FRAME_H_
#define _AIR_WATER_FRAME_H_

/*******************************************************************************
* �C���N���[�h
*******************************************************************************/
#include "object.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �e�N�X�`��
#define TEXTURE_AIR_WATER_FRAME				("data/TEXTURE/air_wind_frame.png")

// �e�N�X�`���X�P�[���i�t���[���j
#define TEXTURE_AIR_WATER_FRAME_SCL			(0.4f * SCREEN_SCALE)

// �e�N�X�`����������
#define TEXTURE_PATTERN_DIVIDE_X_AWFREAM	(2)	// X
#define TEXTURE_PATTERN_DIVIDE_Y_AWFREAM	(1)	// Y

// �z�u���̃}�[�W��
#define AIR_WATER_FRAME_POS_MARGIN_X		(20)
#define AIR_WATER_FRAME_POS_MARGIN_Y		(22)

// �e�N�X�`���T�C�Y
#define TEXTURE_AIR_WATER_FRAME_SIZE_X		(768 / 2 / TEXTURE_PATTERN_DIVIDE_X_AWFREAM * TEXTURE_AIR_WATER_FRAME_SCL)
#define TEXTURE_AIR_WATER_FRAME_SIZE_Y		(512 / 2 / TEXTURE_PATTERN_DIVIDE_Y_AWFREAM * TEXTURE_AIR_WATER_FRAME_SCL)

// ���ːF�ݒ�
#define AIR_WATER_FRAME_DIFFUSE				(D3DCOLOR_RGBA(160, 82, 45, 255))


/* �Q�[�W�Ǘ��p */

// �e�N�X�`���X�P�[���i�Q�[�W�j
#define TEXTURE_AIR_WATER_SCL				(0.9f)

// �e�N�X�`���T�C�Y�i�Q�[�W�j
#define TEXTURE_AIR_WATER_SIZE_X			(TEXTURE_AIR_WATER_FRAME_SIZE_X * TEXTURE_AIR_WATER_SCL)
#define TEXTURE_AIR_WATER_SIZE_Y			(TEXTURE_AIR_WATER_FRAME_SIZE_Y * TEXTURE_AIR_WATER_SCL)

// �G�A�[�t���[���̏����ʒu
#define AIR_POS_FRAME_X						(TEXTURE_AIR_WATER_FRAME_SIZE_X + AIR_WATER_FRAME_POS_MARGIN_X)
#define AIR_POS_FRAME_Y						(TEXTURE_AIR_WATER_FRAME_SIZE_Y + AIR_WATER_FRAME_POS_MARGIN_Y)

// �E�H�[�^�[�t���[���̏����ʒu
#define WATER_FRAME_POS_X					(-TEXTURE_AIR_WATER_FRAME_SIZE_X - AIR_WATER_FRAME_POS_MARGIN_X + SCREEN_WIDTH)
#define WATER_FRAME_POS_Y					(TEXTURE_AIR_WATER_FRAME_SIZE_Y + AIR_WATER_FRAME_POS_MARGIN_Y)


//*****************************************************************************
// �\���̒�`
//*****************************************************************************

// �G�A�[�E�E�H�[�^�[�t���[���Ǘ��p
typedef struct
{
	VERTEX_2D	vertexWk[NUM_VERTEX];
	D3DXVECTOR3	vecPos;
	bool		bUse;
}AirWaterFreamTag;

//*****************************************************************************
// �N���X��`
//*****************************************************************************

class AirWaterFream : public Object
{
public:
	// AWF�Ǘ��p�̗񋓌^
	enum FREAM
	{
		AIR,
		WATER,
		FREAM_MAX
	};
	// �R���X�g���N�^�i�����������j
	AirWaterFream(void);
	//�f�X�g���N�^�i�I�������j
	~AirWaterFream(void);
	// �X�V����
	void	Update(void){/* �X�V�����Ȃ� */}
	// �`�揈��
	void	Draw(void);
private:
	// ���_�̍쐬
	HRESULT MakeVertex(int nNum);

	LPDIRECT3DTEXTURE9	m_pD3DTexture = NULL;	// �e�N�X�`��
	AirWaterFreamTag	tagAWFS[FREAM_MAX];		// AWF�Ǘ��p
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

#endif
