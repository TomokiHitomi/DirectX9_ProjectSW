//=============================================================================
//
// �J�������� [camera.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "main.h"
#include "calculate.h"

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
D3DXVECTOR3		Camera::s_vEye;
D3DXVECTOR3		Camera::s_vAt;
D3DXVECTOR3		Camera::s_vUp;
D3DXVECTOR3		Camera::s_vEyeNext;
D3DXVECTOR3		Camera::s_vAtNext;
D3DXVECTOR3		Camera::s_vUpNext;
D3DXMATRIX		Camera::s_mtxView;
D3DXMATRIX		Camera::s_mtxProjection;
D3DXMATRIX		Camera::s_mtxWorld;
float			Camera::s_fEyeIner;
float			Camera::s_fAtIner;
float			Camera::s_fUpIner;

//=============================================================================
// ����������
//=============================================================================
void Camera::Init(void)
{
	s_vEye = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_vAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	s_vEyeNext = s_vEye;
	s_vAtNext		= s_vAt;
	s_vUpNext		= s_vUp;

	s_fEyeIner	= 1.0f;
	s_fAtIner		= 1.0f;
	s_fUpIner		= 1.0f;
}

//=============================================================================
// �I������
//=============================================================================
void Camera::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void Camera::Update(void)
{
	s_vEye = s_vEye + ((s_vEyeNext - s_vEye) * s_fEyeIner);
	s_vAt = s_vAt + ((s_vAtNext - s_vAt) * s_fAtIner);

#ifdef _DEBUG
	PrintDebugProc("�y Camera �z\n");
	PrintDebugProc("Eye [%f,%f,%f]  Iner[%f]\n", s_vEye.x, s_vEye.y, s_vEye.z, s_fEyeIner);
	PrintDebugProc("At  [%f,%f,%f]  Iner[%f]\n", s_vAt.x, s_vAt.y, s_vAt.z, s_fAtIner);
	PrintDebugProc("Up  [%f,%f,%f]  Iner[%f]\n", s_vUp.x, s_vUp.y, s_vUp.z, s_fUpIner);
	PrintDebugProc("\n");
#endif
}

//=============================================================================
// �J�����̒����_�ݒ菈��
//=============================================================================
void Camera::SetAt(D3DXVECTOR3 vAt)
{
	//camera->posCameraAt = vAt;
	s_vAtNext = vAt;
}

//=============================================================================
// �J�����̎��_�ݒ菈��
//=============================================================================
void Camera::SetEye(D3DXVECTOR3 vEye)
{
	//camera->posCameraEye = vEye;
	s_vEyeNext = vEye;
}

//=============================================================================
// �J�����̏㕔�ݒ菈��
//=============================================================================
void Camera::SetUp(D3DXVECTOR3 vUp)
{
	s_vUp = vUp;
	//camera->vUpNext = vUp;
}

//=============================================================================
// �J�����̒����_�̊����ݒ菈��
//=============================================================================
void Camera::SetAtIner(float fIner)
{
	s_fAtIner = SetLimit(fIner, 1.0f, 0.0f);
}

//=============================================================================
// �J�����̎��_�̊����ݒ菈��
//=============================================================================
void Camera::SetEyeIner(float fIner)
{
	s_fEyeIner = SetLimit(fIner, 1.0f, 0.0f);
}

//=============================================================================
// �J�����̏㕔�̊����ݒ菈��
//=============================================================================
void Camera::SetUpIner(float fIner)
{
	s_fUpIner = SetLimit(fIner, 1.0f, 0.0f);
}

//=============================================================================
// �J�����̒����_�̊�����������
//=============================================================================
void Camera::AddAtIner(float fIner)
{
	s_fAtIner = SetLimit(s_fAtIner + fIner, 1.0f, 0.0f);
}

//=============================================================================
// �J�����̎��_�̊�����������
//=============================================================================
void Camera::AddEyeIner(float fIner)
{
	s_fEyeIner = SetLimit(s_fEyeIner + fIner, 1.0f, 0.0f);
}

//=============================================================================
// �J�����̏㕔�̊�����������
//=============================================================================
void Camera::AddUpIner(float fIner)
{
	s_fUpIner = SetLimit(s_fUpIner + fIner, 1.0f, 0.0f);
}


//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void Camera::Set(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	/******************** �r���[�C���O�ϊ� ********************/
	// �r���[�}�g���N�X�̏�����
	D3DXMatrixIdentity(&s_mtxView);

	// �r���[�}�g���N�X�̍쐬
	D3DXMatrixLookAtLH(&s_mtxView,
		&s_vEye,	// ���_
		&s_vAt,		// �����_
		&s_vUp);	// �����

	// �r���[�}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &s_mtxView);


	/******************** �v���W�F�N�V�����ϊ� ********************/
	// �v���W�F�N�V�����}�g���N�X�̏�����
	D3DXMatrixIdentity(&s_mtxProjection);

	// �v���W�F�N�V�����}�g���N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&s_mtxProjection,
		VIEW_ANGLE,			// �r���[���ʂ̎���p
		VIEW_ASPECT,		// �r���[���ʂ̃A�X�y�N�g��
		VIEW_NEAR_Z,		// �r���[���ʂ�NearZ�l�i�߂��ƕ`�悵�Ȃ��j
		VIEW_FAR_Z);		// �r���[���ʂ�FarZ�l�i�����ƕ`�悵�Ȃ��j

	// �v���W�F�N�V�����}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &s_mtxProjection);
}

////=============================================================================
//// �J�����Ƒ΂ɂȂ��]�s����擾
////=============================================================================
//D3DXMATRIX Camera::GetInvRotateMat(D3DXVECTOR3 pos)
//{
//	D3DXMATRIX Inv;
//	D3DXMatrixIdentity(&Inv);
//	D3DXMatrixLookAtLH(&Inv, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &GetLookAtVct(), &camera->vecCameraUp);
//	D3DXMatrixInverse(&Inv, NULL, &Inv);
//
//	return Inv;
//}

////=============================================================================
//// �����_�Ǝ��_�̐��K�����ꂽ�x�N�g�����擾����֐�
////=============================================================================
//D3DXVECTOR3 Camera::GetLookAtVct(void)
//{
//	CAMERA *camera = GetCamera();
//	D3DXVECTOR3	vecLookAtVct;
//	vecLookAtVct = camera->posCameraEye - camera->posCameraAt;
//	return vecLookAtVct;
//}

//=============================================================================
// �r���[�}�g���b�N�X�̎擾
//=============================================================================
D3DXMATRIX Camera::GetMtxView(void)
{
	return s_mtxView;
}

//=============================================================================
// �v���W�F�N�V�����}�g���b�N�X�̎擾
//=============================================================================
D3DXMATRIX Camera::GetMtxProjection(void)
{
	return s_mtxProjection;
}