//=============================================================================
//
// �R�s�[���C�g���� [copyright.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _COPYRIGHT_H_
#define _COPYRIGHT_H_

/*******************************************************************************
* �C���N���[�h
*******************************************************************************/
#include "object.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �e�N�X�`��
#define TEXTURE_COPYRIGHT				("data/TEXTURE/copyright.png")

// �e�N�X�`���̍ő吔
#define TEXTURE_COPYRIGHT_MAX			(1)

// �e�N�X�`���X�P�[���i�t���[���j
#define TEXTURE_COPYRIGHT_SCL			(0.5f * SCREEN_SCALE)

// �e�N�X�`����������
#define TEXTURE_PATTERN_DIVIDE_X_COPYRIGHT	(1)	// X
#define TEXTURE_PATTERN_DIVIDE_Y_COPYRIGHT	(1)	// Y

// �e�N�X�`���T�C�Y
#define TEXTURE_COPYRIGHT_SIZE_X		(1024 / 2 / TEXTURE_PATTERN_DIVIDE_X_COPYRIGHT * TEXTURE_COPYRIGHT_SCL)
#define TEXTURE_COPYRIGHT_SIZE_Y		(128 / 2 / TEXTURE_PATTERN_DIVIDE_Y_COPYRIGHT * TEXTURE_COPYRIGHT_SCL)

// ���ːF�ݒ�
#define COPYRIGHT_DIFFUSE				(D3DCOLOR_RGBA(255, 255, 255, 255))

//// ���ːF�ݒ�
//#define COPYRIGHT_DIFFUSE				(D3DCOLOR_RGBA(160, 82, 45, 255))

// �����ʒu
#define COPYRIGHT_POS			(D3DXVECTOR3(900.0f * SCREEN_SCALE, 690.0f * SCREEN_SCALE, 0.0f))

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

// �e�N�X�`���Ǘ��p
typedef struct
{
	VERTEX_2D	vertexWk[NUM_VERTEX];
	D3DXVECTOR3	vecPos;
	bool		bUse;
}CopyrightTag;

//*****************************************************************************
// �N���X��`
//*****************************************************************************

class Copyright : public Object
{
public:
	// AWF�Ǘ��p�̗񋓌^
	enum COPYRIGHT
	{
		CORYRIGHT_LOGO
	};
	// �R���X�g���N�^�i�����������j
	Copyright(void);
	//�f�X�g���N�^�i�I�������j
	~Copyright(void);
	// �X�V����
	void	Update(void){/* �X�V�����Ȃ� */}
	// �`�揈��
	void	Draw(void);
private:
	// ���_�̍쐬
	HRESULT MakeVertex(int nNum);

	LPDIRECT3DTEXTURE9	m_pD3DTexture = NULL;		// �e�N�X�`��
	CopyrightTag	tagUi[TEXTURE_COPYRIGHT_MAX];	// �e�N�X�`���Ǘ��p
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

#endif
