//=============================================================================
//
// �X�e���V������ [stencil.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "main.h"
#include "stencil.h"

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
Stencil::Stencil(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �I�u�W�F�N�gID�ƃv���C�I���e�B�̐ݒ菈��
	SetIdAndPriority(ObjectID::STENCIL, Priority::Middle, Priority::Lowest);

	// �e�v���p�e�B�̏�����
	m_bUse = true;
	m_CXTexture.data.vPos = D3DXVECTOR2(1920 / 2-30, 1080 / 2 - 50);
	m_CXTexture.data.nTexNum = 0;
	m_CXTexture.data.vSize = D3DXVECTOR2(50, 50);
	m_CXTexture.data.xColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_CXTexture.data.nDivide.x = 1;
	m_CXTexture.data.nDivide.y = 4;

	// �e�N�X�`���̏�����
	m_CXTexture.Init(pDevice, STENCIL_TEXTURE);

	m_CXTexture2.data.vPos = D3DXVECTOR2(1920 / 2, 1080 / 2);
	m_CXTexture2.data.vSize = D3DXVECTOR2(1920 / 2, 1080 / 2);
	m_CXTexture2.data.nDivide.x = 1;
	m_CXTexture2.data.nDivide.y = 1;
	m_CXTexture2.data.xColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_CXTexture2.data.nTexNum = 0;

	// �e�N�X�`���̏�����
	m_CXTexture2.Init(pDevice, STENCIL_TEXTURE2);
}

//=============================================================================
// �f�X�g���N�^�i�I�������j
//=============================================================================
Stencil::~Stencil(void)
{
	m_CXTexture.Release();
	m_CXTexture2.Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void Stencil::Update(void)
{
	m_CXTexture.data.vPos.x++;
	m_CXTexture.SetVertex();
}

//=============================================================================
// �`�揈��
//=============================================================================
void Stencil::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	if (m_bUse)
	{
		// Z�o�b�t�@�ւ̏������݋֎~
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		// �X�e���V���e�X�g����
		pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
		// �X�e���V���}�X�N�̐ݒ� �f�[�^�͂��̂܂܎g��
		pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);
		// �X�e���V���e�X�g�ɋ����I�ɍ��i�i�K���������ށj
		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
		// �o�b�N�o�b�t�@��RGB�l���������܂Ȃ�
		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x00);
		// �X�e���V���e�X�g�ɕs���i�̏ꍇ�̓X�e���V���l�ɉ������Ȃ�
		pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILCAPS_KEEP);
		// Z�o�b�t�@�ɕs���i�̏ꍇ�̓X�e���V���l�ɂȂɂ����Ȃ�
		pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILCAPS_KEEP);
		// �X�e���V���e�X�g�ɍ��i�̏ꍇ�X�e���V���l��1���₷
		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILCAPS_INCRSAT);

		// �X�e���V���`��
		m_CXTexture.Draw();

		// �X�e���V���o�b�t�@�̒l�Ɣ�r����Q�ƒl
		pDevice->SetRenderState(D3DRS_STENCILREF, 0x09);
		// ��r�֐��A�������^�̎��X�e���V���e�X�g���i
		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL);
		// �X�e���V���e�X�g�ɍ��i�����ꍇ�X�e���V���l�ɉ������Ȃ�
		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILCAPS_KEEP);
		// �t���[���o�b�t�@��RGB���������߂�悤�ɂ���
		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE,
			D3DCOLORWRITEENABLE_RED |
			D3DCOLORWRITEENABLE_GREEN |
			D3DCOLORWRITEENABLE_BLUE |
			D3DCOLORWRITEENABLE_ALPHA);
		// Z�o�b�t�@�ւ̏������݂�������
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		m_CXTexture2.Draw();
	}
}