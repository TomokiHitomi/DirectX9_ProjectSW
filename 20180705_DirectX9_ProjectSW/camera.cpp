//=============================================================================
//
// �J�������� [camera.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "main.h"
#include "scene.h"

// �f�o�b�O�p
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
#ifdef _DEBUG
void DebugCamera(CAMERA *camera);
void DebugMoveCamera(CAMERA *camera);
void DebugZoomCamera(CAMERA *camera);
void DebugResetCamera(CAMERA *camera);
#endif
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
CAMERA					cameraWk;

#ifdef _DEBUG
D3DXVECTOR3 g_posCameraEyeToAt;
#endif

extern SceneManager		g_cScene;				// Scene�}�l�[�W��

//=============================================================================
// ����������
//=============================================================================
void InitCamera(void)
{
	CAMERA *camera = &cameraWk;

	// ���݂̃V�[�����擾
	SceneManager::SCENE eScene = g_cScene.GetScene();

	// RESULT�V�[���̏ꍇ
	if (eScene == SceneManager::RESULT)
	{
		camera->posCameraEye = D3DXVECTOR3(POS_X_CAM_RESULT, POS_Y_CAM_RESULT, POS_Z_CAM_RESULT);
		camera->posCameraAt = D3DXVECTOR3(POS_X_CAM_RESULT_AT, POS_Y_CAM_RESULT_AT, POS_Z_CAM_RESULT_AT);
		camera->vecCameraUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		camera->fHAngle = CAMERA_H_ANGLE;
	}
	// TITLE�EGAME�V�[���̏ꍇ
	else
	{
		camera->posCameraEye = D3DXVECTOR3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);
		camera->posCameraAt = D3DXVECTOR3(POS_X_CAM_AT, POS_Y_CAM_AT, POS_Z_CAM_AT);
		camera->vecCameraUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		camera->fHAngle = CAMERA_H_ANGLE;
	}

#ifdef _DEBUG
	g_posCameraEyeToAt = camera->posCameraEye - camera->posCameraAt;
	D3DXVec3Normalize(&g_posCameraEyeToAt, &g_posCameraEyeToAt);
#endif
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
	CAMERA *camera = &cameraWk;
#ifdef _DEBUG
	PrintDebugProc("�y CAMERA �z\n");
	PrintDebugProc("Eye [X:%f  Y:%f  Z:%f]\n", camera->posCameraEye.x, camera->posCameraEye.y, camera->posCameraEye.z);
	PrintDebugProc("At  [X:%f  Y:%f  Z:%f]\n", camera->posCameraAt.x, camera->posCameraAt.y, camera->posCameraAt.z);
	PrintDebugProc("\n");
	DebugCamera(camera);
#endif

//#ifdef _DEBUG
//	CAMERA *camera = GetCamera();
//
//	//�f�o�b�O����ZC�ŃJ������]
//	if (GetKeyboardPress(DIK_Z))
//	{// ���_����u���v
//		camera->rotCamera.y += VALUE_ROTATE_CAMERA;
//		if (camera->rotCamera.y > D3DX_PI)
//		{
//			camera->rotCamera.y -= D3DX_PI * 2.0f;
//		}
//
//		camera->posCameraEye.x = camera->posCameraAt.x - sinf(camera->rotCamera.y) * camera->fLength;
//		camera->posCameraEye.z = camera->posCameraAt.z - cosf(camera->rotCamera.y) * camera->fLength;
//	}
//	if (GetKeyboardPress(DIK_C))
//	{// ���_����u�E�v
//		camera->rotCamera.y -= VALUE_ROTATE_CAMERA;
//		if (camera->rotCamera.y < -D3DX_PI)
//		{
//			camera->rotCamera.y += D3DX_PI * 2.0f;
//		}
//
//		camera->posCameraEye.x = camera->posCameraAt.x - sinf(camera->rotCamera.y) * camera->fLength;
//		camera->posCameraEye.z = camera->posCameraAt.z - cosf(camera->rotCamera.y) * camera->fLength;
//	}
//
//#endif
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	CAMERA	*camera = &cameraWk;
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
// �J�����̌����̎擾
//=============================================================================
D3DXVECTOR3 GetRotCamera(void)
{
	CAMERA	*camera = &cameraWk;
	return camera->rotCamera;
}

//=============================================================================
// �r���[�}�g���b�N�X�̎擾
//=============================================================================
D3DXMATRIX GetMtxView(void)
{
	CAMERA	*camera = &cameraWk;
	return camera->mtxView;
}

//=============================================================================
// �v���W�F�N�V�����}�g���b�N�X�̎擾
//=============================================================================
D3DXMATRIX GetMtxProjection(void)
{
	CAMERA	*camera = &cameraWk;
	return camera->mtxProjection;
}

//=============================================================================
// �J�����擾�֐�
//=============================================================================
CAMERA *GetCamera(void)
{
	return(&cameraWk);
}

#ifdef _DEBUG


//=============================================================================
// �J�����̈ړ������i�f�o�b�O�p�j
//=============================================================================
void DebugCamera(CAMERA *camera)
{
	// ���V�t�g�L�[���͒�
	if (GetKeyboardPress(DIK_LSHIFT))
	{
		DebugMoveCamera(camera);	// �J�����ړ�����
		DebugZoomCamera(camera);	// �J�����g�k����
		DebugResetCamera(camera);	// �J��������������
	}
}

//=============================================================================
// �J�����̈ړ������i�f�o�b�O�p�j
//=============================================================================
void DebugMoveCamera(CAMERA *camera)
{
	// �O��ړ�
	if (GetKeyboardPress(DIK_W) && GetKeyboardPress(DIK_S))
	{	// �ړ��Ȃ�
	}
	else if (GetKeyboardPress(DIK_W))
	{	// �O���ֈړ�
		camera->posCameraEye.z += CAMERA_MOVE_SPEED;
		camera->posCameraAt.z += CAMERA_MOVE_SPEED;
	}
	else if (GetKeyboardPress(DIK_S))
	{	// ����ֈړ�
		camera->posCameraEye.z -= CAMERA_MOVE_SPEED;
		camera->posCameraAt.z -= CAMERA_MOVE_SPEED;
	}

	// ���E�ړ�
	if (GetKeyboardPress(DIK_D) && GetKeyboardPress(DIK_A))
	{	// �ړ��Ȃ�
	}
	else if (GetKeyboardPress(DIK_D))
	{	// �E���ֈړ�
		camera->posCameraEye.x += CAMERA_MOVE_SPEED;
		camera->posCameraAt.x += CAMERA_MOVE_SPEED;
	}
	else if (GetKeyboardPress(DIK_A))
	{	// �����ֈړ�
		camera->posCameraEye.x -= CAMERA_MOVE_SPEED;
		camera->posCameraAt.x -= CAMERA_MOVE_SPEED;
	}
}

//=============================================================================
// �J�����̊g�k�����i�f�o�b�O�p�j
//=============================================================================
void DebugZoomCamera(CAMERA *camera)
{
	if (GetKeyboardPress(DIK_UP) && GetKeyboardPress(DIK_DOWN))
	{	// �g�k�Ȃ�
	}
	else if (GetKeyboardPress(DIK_UP))
	{	// �g��
		camera->posCameraEye -= g_posCameraEyeToAt * CAMERA_ZOOM_SPEED;
	}
	else if (GetKeyboardPress(DIK_DOWN))
	{	// �k��
		camera->posCameraEye += g_posCameraEyeToAt * CAMERA_ZOOM_SPEED;
	}
}
//=============================================================================
// �J�����̃��Z�b�g�����i�f�o�b�O�p�j
//=============================================================================
void DebugResetCamera(CAMERA *camera)
{
	if (GetKeyboardPress(DIK_RETURN))
	{	
		camera->posCameraEye = D3DXVECTOR3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);
		camera->posCameraAt = D3DXVECTOR3(POS_X_CAM_AT, POS_Y_CAM_AT, POS_Z_CAM_AT);
	}
}
#endif