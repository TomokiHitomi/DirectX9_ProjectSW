//=============================================================================
//
// �J�������� [camera.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CAMERA_ANGLE_MARGIN		(0.1f)

#define CAMERA_MOVE_SPEED		(1)
#define CAMERA_MOVE_SLANT		(CAMERA_MOVE_SPEED * 0.71f)
#define CAMERA_ROT_SPEED		(0.02f)
#define CAMERA_ROT_SLANT		(CAMERA_ROT_SPEED * 0.71f)
#define CAMERA_ROT_SPEED_AUTO	(0.2f)


#define CAMERA_V_ANGLE			(4.71f)
#define CAMERA_H_ANGLE			(D3DX_PI * 0.5f)
#define CAMERA_V_ANGLE_MAX					(6.28f)
#define CAMERA_V_ANGLE_MIN					(3.16f)

#define CAMERA_HEIGHT			(25.0f)

#define CAMERA_H_ANGLE_MARGIN	(D3DX_PI / 180 * 45)
#define CAMERA_MARGIN_SPEED		(D3DX_PI / 180)
#define CAMERA_LENGTH_SPEED			(0.1f)
#define CAMERA_LENGTH				(300.0f)
#define CAMERA_LENGTH_MIN			(10.0f)
#define CAMERA_LENGTH_MAX			(500.0f)
#define CAMERA_LENGTH_AUTO_SPEED	(30.0f)

#define CAMERA_ROT_MOUSE_X			(0.002f)
#define CAMERA_ROT_MOUSE_Y			(0.002f)

#define CAMERA_HEIGHT_SUSPENSION	(3)

#define ROOT_2					(1.414213f)

#define	POS_X_CAM				(0.0f)										// �J�����̏����ʒu(X���W)
#define	POS_Y_CAM				(25.0f)										// �J�����̏����ʒu(Y���W)
#define	POS_Z_CAM				(100.0f)										// �J�����̏����ʒu(Z���W)

#define	VIEW_ANGLE				(D3DXToRadian(35.0f))							// �r���[���ʂ̎���p
#define	VIEW_ASPECT				((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��	
#define	VIEW_NEAR_Z				(10.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z				(5000.0f)										// �r���[���ʂ�FarZ�l

typedef struct
{
	D3DXVECTOR3			posCameraEye;				// �J�����̎��_
	D3DXVECTOR3			posCameraAt;				// �J�����̒����_
	D3DXVECTOR3			vecCameraUp;				// �J�����̏�����x�N�g��
	D3DXVECTOR3			rotCamera;					// �J�����̉�]
	D3DXVECTOR3			vecTag;

	D3DXVECTOR3			vecCameraAtPos;

	D3DXMATRIX			mtxView;				// �r���[�}�g���b�N�X
	D3DXMATRIX			mtxProjection;			// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX			mtxWorld;				// ���[���h�}�g���b�N�X

	float				fHAngleMargin;
	float				fHAngle;
	float				fVAngle;
	float				fHAngleDiff;
	float				fVAngleDiff;
	float				fLength;
	float				fLengthTemp;
	float				fMoveSpeed;
	float				fRotSpeed;

}CAMERA;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);

void SetCameraAt(D3DXVECTOR3);
void SetCameraEye(D3DXVECTOR3);
void SetCameraUp(D3DXVECTOR3);
void SetCamera(void);
D3DXMATRIX GetInvRotateMat(D3DXVECTOR3 pos);
D3DXMATRIX GetMtxView(void);
D3DXMATRIX GetMtxProjection(void);
CAMERA *GetCamera(void);
#endif