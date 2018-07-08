//=============================================================================
//
// �^�C�g������ [title.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "title.h"

/* �S�̂ŕK�v�ȃC���N���[�h */
#include "input.h"
#include "camera.h"
#include "sound.h"

/* �^�C�g���ŕK�v�ȃC���N���[�h */
#include "copyright.h"
#include "air_water_frame.h"
#include "object.h"
#include "SkinMeshX.h"
#include "light.h"

/* Debug */
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
CSkinMesh			m_CSkinMesh;		// �X�L�����b�V���i�[�p
D3DXMATRIX			worldmtx;

//=============================================================================
// �X�V����
//=============================================================================
void TitleScene::Update(void)
{
	Object::UpdateAll();



	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	/******************** ���[���h�ϊ� ********************/
	// ���[���h�}�g���N�X�̏�����
	D3DXMatrixIdentity(&worldmtx);

	// �yS�z�X�P�[���𔽉f(Multiply�͍s��v�Z)
	D3DXMatrixScaling(&mtxScl, 3.0f, 3.0f, 3.0f);
	D3DXMatrixMultiply(&worldmtx, &worldmtx, &mtxScl);

	// �yR�z��]�𔽉f(YawPitchRoll��y,x,z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&worldmtx, &worldmtx, &mtxRot);

	// �yT�z���s�ړ��𔽉f(�I�u�W�F�N�g��z�u���Ă���j
	D3DXMatrixTranslation(&mtxTranslate, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&worldmtx, &worldmtx, &mtxTranslate);

	m_CSkinMesh.Update(worldmtx);

}

//=============================================================================
// �`�揈��
//=============================================================================
void TitleScene::Draw(void)
{

	// ���C�e�B���O�����߂ɂ��Ă�
	SetLight(LIGHT_SUB1, TRUE);
	SetLight(LIGHT_SUB2, TRUE);

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	m_CSkinMesh.Draw(pDevice);

	// ���C�e�B���O��ʏ�ɖ߂�
	SetLight(LIGHT_SUB1, FALSE);
	SetLight(LIGHT_SUB2, FALSE);


	Object::DrawAll();


}

//=============================================================================
// �R���X�g���N�^�����i�������j
//=============================================================================
TitleScene::TitleScene(void)
{
	//Object *CObject = new Copyright;
	//Object *CObject2 = new AirWaterFream;

	new Copyright;
	new AirWaterFream;

	//Object::Create(new Copyright);

	//NewObject<Copyright>::Create();
	//NewObject<AirWaterFream>::Create();
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	m_CSkinMesh.Init(pDevice, "data/MODEL/Yuko.x");

}

//=============================================================================
// �f�X�g���N�^�����i�I���j
//=============================================================================
TitleScene::~TitleScene(void)
{
	Object::ReleaseAll();
	m_CSkinMesh.Release();
}

//=============================================================================
// �^�C�g���f���̃��X�^�[�g���\�b�h
//=============================================================================
void TitleScene::DemoRestart(void)
{

}