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
#define	POS_X_CAM				(0.0f)										// �J�����̏����ʒu(X���W)
#define	POS_Y_CAM				(25.0f)										// �J�����̏����ʒu(Y���W)
#define	POS_Z_CAM				(100.0f)										// �J�����̏����ʒu(Z���W)

#define	VIEW_ANGLE				(D3DXToRadian(35.0f))							// �r���[���ʂ̎���p
#define	VIEW_ASPECT				((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��	
#define	VIEW_NEAR_Z				(10.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z				(5000.0f)										// �r���[���ʂ�FarZ�l

class Camera
{
private:
	static D3DXVECTOR3	s_vEye;				// �J�����̎��_
	static D3DXVECTOR3	s_vAt;				// �J�����̒����_
	static D3DXVECTOR3	s_vUp;				// �J�����̏�����x�N�g��

	static D3DXVECTOR3	s_vEyeNext;			// �J�����̎��_
	static D3DXVECTOR3	s_vAtNext;			// �J�����̒����_
	static D3DXVECTOR3	s_vUpNext;			// �J�����̏�����x�N�g��

	static D3DXMATRIX	s_mtxView;			// �r���[�}�g���b�N�X
	static D3DXMATRIX	s_mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	static D3DXMATRIX	s_mtxWorld;			// ���[���h�}�g���b�N�X

	static float		s_fEyeIner;			// �J�����̎��_
	static float		s_fAtIner;			// �J�����̒����_
	static float		s_fUpIner;			// �J�����̏�����x�N�g��
public:
	static void Init(void);
	static void Uninit(void);
	static void Update(void);

	static void SetAt(D3DXVECTOR3 vAt);
	static void SetEye(D3DXVECTOR3 vEye);
	static void SetUp(D3DXVECTOR3 vUp);

	static void SetAtIner(float fIner);
	static void SetEyeIner(float fIner);
	static void SetUpIner(float fIner);

	static void AddAtIner(float fIner);
	static void AddEyeIner(float fIner);
	static void AddUpIner(float fIner);

	static float GetAtIner(void) { return s_fAtIner; }
	static float GetEyeIner(void) { return s_fEyeIner; }
	static float GetUpIner(void) { return s_fUpIner; }

	//static void SetEye(D3DXVECTOR3 vEye);
	//static void SetUp(D3DXVECTOR3 vUp);
	static void Set(void);
	//D3DXMATRIX GetInvRotateMat(D3DXVECTOR3 pos);
	static D3DXMATRIX GetMtxView(void);
	static D3DXMATRIX GetMtxProjection(void);
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
#endif