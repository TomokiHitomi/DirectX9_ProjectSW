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
D3DXVECTOR3 GetLookAtVct(void);
void MoveCamera(int no);
void MoveCameraFunc(float fAngle);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
CAMERA					cameraWk;

//=============================================================================
// ����������
//=============================================================================
void InitCamera(void)
{
	CAMERA *camera = GetCamera();

	camera->fHAngleMargin = 0.0f;
	camera->fMoveSpeed = CAMERA_MOVE_SPEED;
	camera->posCameraEye = D3DXVECTOR3(0.0f, CAMERA_HEIGHT, 0.0f);
	camera->posCameraAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	camera->vecCameraAtPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	camera->vecCameraUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	camera->fVAngle = CAMERA_V_ANGLE;
	camera->fHAngle = CAMERA_H_ANGLE;
	camera->fVAngleDiff = CAMERA_V_ANGLE;
	camera->fHAngleDiff = CAMERA_H_ANGLE;
	camera->fLength = CAMERA_LENGTH;
	camera->fLengthTemp = CAMERA_LENGTH;
}

//=============================================================================
// �I������
//=============================================================================
void UninitCamera(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateCamera(void)
{
//	CAMERA *camera = GetCamera();
//
//	// �J�����㉺���]�p
//	int nCameraReverse = 1;
//
//	// �N�H�[�^�j�I���p�ϐ�
//	// ��]���a��ݒ�
//	D3DXVECTOR3 vecTa = D3DXVECTOR3(0.0f, camera->fLength, 0.0f);
//	// ���݂̊p�x�ɐ����ȉ�]���x�N�g����ݒ�
//	D3DXVECTOR3 vecAxis = D3DXVECTOR3(cosf(camera->fHAngle + D3DX_PI * 0.5f), 0, sinf(camera->fHAngle + D3DX_PI * 0.5f));
//
//
//	// �΂߉�]���ɑ��x���Z
//	if ( ((GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_DOWN)) && (GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_RIGHT)))
//		|| ((IsButtonPressed(0, BUTTON_UP) || IsButtonPressed(0, BUTTON_DOWN)) && (IsButtonPressed(0, BUTTON_LEFT) || IsButtonPressed(0, BUTTON_RIGHT))))
//	{
//		camera->fRotSpeed = CAMERA_ROT_SLANT;
//	}
//	else
//	{
//		camera->fRotSpeed = CAMERA_ROT_SPEED;
//	}
//
//	// �J�������E��]�i�L�[�{�[�h�j
//	if (GetKeyboardPress(DIK_LEFT) && GetKeyboardPress(DIK_RIGHT))
//	{
//		// ���������͏����Ȃ�
//	}
//	else if (GetKeyboardPress(DIK_LEFT))
//	{
//		camera->fHAngleDiff += camera->fRotSpeed;
//	}
//	else if (GetKeyboardPress(DIK_RIGHT))
//	{
//		camera->fHAngleDiff -= camera->fRotSpeed;
//	}
//
//	// �J�������E��]�i�}�E�X�j
//	if ((float)GetMobUseX())
//	{
//		camera->fHAngleDiff -= ((float)GetMobUseX()) * CAMERA_ROT_MOUSE_X;
//	}
//
//	// �J�����㉺��]�i�L�[�{�[�h�j
//	if (GetKeyboardPress(DIK_UP) && GetKeyboardPress(DIK_DOWN))
//	{
//		// ���������͏����Ȃ�
//	}
//	else if (GetKeyboardPress(DIK_UP))
//	{
//		camera->fVAngleDiff += camera->fRotSpeed * nCameraReverse;
//	}
//	else if (GetKeyboardPress(DIK_DOWN))
//	{
//		camera->fVAngleDiff -= camera->fRotSpeed * nCameraReverse;
//	}
//	// �J�����㉺��]�i�}�E�X�j
//	if ((float)GetMobUseY())
//	{
//		camera->fVAngleDiff -= ((float)GetMobUseY()) * CAMERA_ROT_MOUSE_Y * nCameraReverse;
//	}
//
//	// �}�E�X�z�C�[��
//	long ModUseZ = GetMobUseZ();
//	if (ModUseZ != 0)
//	{
//		camera->fLength -= ModUseZ * CAMERA_LENGTH_SPEED;
//		if (camera->fLength < CAMERA_LENGTH_MIN) { camera->fLength = CAMERA_LENGTH_MIN; }
//		else if (camera->fLength > CAMERA_LENGTH_MAX) { camera->fLength = CAMERA_LENGTH_MAX; }
//	}
//
//	camera->fVAngle += (camera->fVAngleDiff - camera->fVAngle) * CAMERA_ROT_SPEED_AUTO;
//	camera->fHAngle += (camera->fHAngleDiff - camera->fHAngle) * CAMERA_ROT_SPEED_AUTO;
//
//	// �J���������E
//	if (camera->fVAngle > CAMERA_V_ANGLE_MAX)
//	{
//		camera->fVAngle = CAMERA_V_ANGLE_MAX;
//		camera->fVAngleDiff = CAMERA_V_ANGLE_MAX;
//	}
//	else if (camera->fVAngle < CAMERA_V_ANGLE_MIN)
//	{
//		camera->fVAngle = CAMERA_V_ANGLE_MIN;
//		camera->fVAngleDiff = CAMERA_V_ANGLE_MIN;
//	}
//
//	MoveCamera(0);
//
//	// �N�H�[�^�j�I������
//	QuaternionCalculate(&vecTa, &vecAxis, camera->fVAngle,
//		&camera->posCameraEye);
//	// ��]���ɐݒu
//	camera->posCameraEye += camera->posCameraAt;
//
//#ifdef _DEBUG
//	PrintDebugProc("�yCANERA�z\n");
//	PrintDebugProc("CameraEye[X:%f Y : %f Z : %f]\n",
//		camera->posCameraEye.x, camera->posCameraEye.y, camera->posCameraEye.z);
//	PrintDebugProc("CameraAt [X:%f Y : %f Z : %f]\n",
//		camera->posCameraAt.x, camera->posCameraAt.y, camera->posCameraAt.z);
//	PrintDebugProc("CameraAng[V:%f  H:%f]  CameraLength[%f]\n",
//		camera->fVAngle, camera->fHAngle, camera->fLength);
//	PrintDebugProc("\n");
//#endif
}

//=============================================================================
// �J�����̒����_�ݒ菈��
//=============================================================================
void SetCameraAt(D3DXVECTOR3 vAt)
{
	CAMERA *camera = GetCamera();
	camera->posCameraAt = vAt;
}

//=============================================================================
// �J�����̎��_�ݒ菈��
//=============================================================================
void SetCameraEye(D3DXVECTOR3 vEye)
{
	CAMERA *camera = GetCamera();
	camera->posCameraEye = vEye;
}

//=============================================================================
// �J�����̎��_�ݒ菈��
//=============================================================================
void SetCameraUp(D3DXVECTOR3 vUp)
{
	CAMERA *camera = GetCamera();
	camera->vecCameraUp = vUp;
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	CAMERA *camera = GetCamera();
	/******************** �r���[�C���O�ϊ� ********************/
	// �r���[�}�g���N�X�̏�����
	D3DXMatrixIdentity(&camera->mtxView);

	// �r���[�}�g���N�X�̍쐬
	D3DXMatrixLookAtLH(&camera->mtxView,
		&camera->posCameraEye,	// ���_
		&camera->posCameraAt,	// �����_
		&camera->vecCameraUp);	// �����

								// �r���[�}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &camera->mtxView);


	/******************** �v���W�F�N�V�����ϊ� ********************/
	// �v���W�F�N�V�����}�g���N�X�̏�����
	D3DXMatrixIdentity(&camera->mtxProjection);

	// �v���W�F�N�V�����}�g���N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&camera->mtxProjection,
		VIEW_ANGLE,			// �r���[���ʂ̎���p
		VIEW_ASPECT,		// �r���[���ʂ̃A�X�y�N�g��
		VIEW_NEAR_Z,		// �r���[���ʂ�NearZ�l�i�߂��ƕ`�悵�Ȃ��j
		VIEW_FAR_Z);		// �r���[���ʂ�FarZ�l�i�����ƕ`�悵�Ȃ��j

							// �v���W�F�N�V�����}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &camera->mtxProjection);
}

//=============================================================================
// �J�����Ƒ΂ɂȂ��]�s����擾
//=============================================================================
D3DXMATRIX GetInvRotateMat(D3DXVECTOR3 pos)
{
	CAMERA *camera = GetCamera();

	D3DXMATRIX Inv;
	D3DXMatrixIdentity(&Inv);
	D3DXMatrixLookAtLH(&Inv, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &GetLookAtVct(), &camera->vecCameraUp);
	D3DXMatrixInverse(&Inv, NULL, &Inv);

	return Inv;
}

//=============================================================================
// �����_�Ǝ��_�̐��K�����ꂽ�x�N�g�����擾����֐�
//=============================================================================
D3DXVECTOR3 GetLookAtVct(void)
{
	CAMERA *camera = GetCamera();
	D3DXVECTOR3	vecLookAtVct;
	vecLookAtVct = camera->posCameraEye - camera->posCameraAt;
	return vecLookAtVct;
}

//=============================================================================
// �r���[�}�g���b�N�X�̎擾
//=============================================================================
D3DXMATRIX GetMtxView(void)
{
	CAMERA *camera = GetCamera();
	return camera->mtxView;
}

//=============================================================================
// �v���W�F�N�V�����}�g���b�N�X�̎擾
//=============================================================================
D3DXMATRIX GetMtxProjection(void)
{
	CAMERA *camera = GetCamera();
	return camera->mtxProjection;
}

//=============================================================================
// �J�����擾�֐�
//=============================================================================
CAMERA *GetCamera(void)
{
	return(&cameraWk);
}

//=============================================================================
// �J�����ړ��֐�
//=============================================================================
void MoveCamera(int no)
{
	//CAMERA *camera = GetCamera();
	//// �ړ�����
	//if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_LEFT))
	//{
	//	if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP))
	//	{// ���O�ړ�
	//		MoveCameraFunc(camera->fHAngle + D3DX_PI * 0.25f);
	//	}
	//	else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN))
	//	{// ����ړ�
	//		MoveCameraFunc(camera->fHAngle + D3DX_PI * 0.75f);
	//	}
	//	else if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT))
	//	{// ���E���������͏����Ȃ�
	//	}
	//	else
	//	{// ���ړ�
	//		MoveCameraFunc(camera->fHAngle + D3DX_PI * 0.50f);
	//	}
	//}
	//else if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT))
	//{
	//	if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_LEFT))
	//	{// �E�O�ړ�
	//		MoveCameraFunc(camera->fHAngle - D3DX_PI * 0.25f);
	//	}
	//	else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN))
	//	{// �E��ړ�
	//		MoveCameraFunc(camera->fHAngle - D3DX_PI * 0.75f);
	//	}
	//	else
	//	{// �E�ړ�
	//		MoveCameraFunc(camera->fHAngle - D3DX_PI * 0.50f);
	//	}
	//}
	//else if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP))
	//{// �O�ړ�
	//	if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN))
	//	{// �O�㓯�������͏����Ȃ�
	//	}
	//	else
	//	{
	//		MoveCameraFunc(camera->fHAngle);
	//	}
	//}
	//else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN))
	//{// ��ړ�
	//	MoveCameraFunc(camera->fHAngle + D3DX_PI);
	//}
}

//=============================================================================
// �ړ������֐�
//=============================================================================
void MoveCameraFunc(float fAngle)
{
	CAMERA *camera = GetCamera();
	camera->posCameraEye.x += cosf(fAngle) * CAMERA_MOVE_SPEED;
	camera->posCameraEye.z += sinf(fAngle) * CAMERA_MOVE_SPEED;
}