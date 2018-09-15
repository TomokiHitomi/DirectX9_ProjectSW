//=============================================================================
//
// 2D�e�N�X�`������ [XTexture.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _XTEXTURE_H_
#define _XTEXTURE_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CXTexture
{
private:
	LPDIRECT3DTEXTURE9	m_pD3DTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DTEXTURE9	m_pD3DTexture2;		// �e�N�X�`���ւ̃|�C���^
	VERTEX_2D	m_VertexWk[NUM_VERTEX];		// 2D���_���

	typedef struct XTextureData
	{
		typedef struct Divide
		{
			int x, y;
		};
		D3DXVECTOR2		vPos;
		D3DXVECTOR2		vSize;
		Divide			nDivide;
		D3DXCOLOR		xColor;
		int				nTexNum;
	};

	// ���_�̍쐬
	HRESULT MakeVertex(void);

public:
	XTextureData data;

	// �R���X�g���N�^
	CXTexture();
	// �f�X�g���N�^
	~CXTexture() { Release(); }

	// �X�L�����b�V����������
	HRESULT Init(LPDIRECT3DDEVICE9, LPSTR);
	// �������
	void Release();
	// �X�V����
	void Update(void);
	// �`�揈��
	void Draw(void);
	// ���_�̍쐬
	HRESULT SetVertex(void);
};
#endif