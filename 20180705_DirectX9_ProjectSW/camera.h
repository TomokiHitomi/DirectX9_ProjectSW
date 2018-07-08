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
#define CAMERA_MOVE_SPEED		(5.0f)
#define CAMERA_ZOOM_SPEED		(10.0f)
#define CAMERA_H_ANGLE			(D3DX_PI * 0.5f)

#define	POS_X_CAM				(0.0f)					// �J�����̏����ʒu(X���W)
#define	POS_Y_CAM				(1090.0f)				// �J�����̏����ʒu(Y���W)
#define	POS_Z_CAM				(-1190.0f)				// �J�����̏����ʒu(Z���W)

#define	POS_X_CAM_AT			(0.0f)
#define	POS_Y_CAM_AT			(0.0f)
#define	POS_Z_CAM_AT			(-100.0f)

#define	VIEW_ANGLE				(D3DXToRadian(35.0f))							// �r���[���ʂ̎���p
#define	VIEW_ASPECT				((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��	
#define	VIEW_NEAR_Z				(10.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z				(10000.0f)										// �r���[���ʂ�FarZ�l

#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.01f)									// �J�����̈ړ���

/* SelectScene */

// �J�������_�̏����ʒu
#define	POS_X_CAM_SELECT		(0.0f)
#define	POS_Y_CAM_SELECT		(50.0f)
#define	POS_Z_CAM_SELECT		(-500.0f)

// �J���������_�̏����ʒu
#define	POS_X_CAM_SELECT_AT		(0.0f)
#define	POS_Y_CAM_SELECT_AT		(50.0f)
#define	POS_Z_CAM_SELECT_AT		(-100.0f)


/* ResultScene */

// �J�������_�̏����ʒu
#define	POS_X_CAM_RESULT		(0.0f)
#define	POS_Y_CAM_RESULT		(50.0f)
#define	POS_Z_CAM_RESULT		(-500.0f)

// �J���������_�̏����ʒu
#define	POS_X_CAM_RESULT_AT		(0.0f)
#define	POS_Y_CAM_RESULT_AT		(50.0f)
#define	POS_Z_CAM_RESULT_AT		(-100.0f)

typedef struct
{
	D3DXVECTOR3			posCameraEye;				// �J�����̎��_
	D3DXVECTOR3			posCameraAt;				// �J�����̒����_
	D3DXVECTOR3			vecCameraUp;				// �J�����̏�����x�N�g��
	D3DXVECTOR3			rotCamera;					// �J�����̉�]

	D3DXMATRIX			mtxView;				// �r���[�}�g���b�N�X
	D3DXMATRIX			mtxProjection;			// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX			mtxWorld;				// ���[���h�}�g���b�N�X

	float				fHAngle;

}CAMERA;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void			InitCamera(void);
void			UninitCamera(void);
void			UpdateCamera(void);
void			SetCamera(void);
D3DXVECTOR3		GetRotCamera(void);
D3DXMATRIX		GetMtxView(void);
D3DXMATRIX		GetMtxProjection(void);
CAMERA			*GetCamera(void);
#endif