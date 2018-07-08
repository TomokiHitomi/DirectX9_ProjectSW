//=============================================================================
//
// カメラ処理 [camera.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CAMERA_MOVE_SPEED		(5.0f)
#define CAMERA_ZOOM_SPEED		(10.0f)
#define CAMERA_H_ANGLE			(D3DX_PI * 0.5f)

#define	POS_X_CAM				(0.0f)					// カメラの初期位置(X座標)
#define	POS_Y_CAM				(1090.0f)				// カメラの初期位置(Y座標)
#define	POS_Z_CAM				(-1190.0f)				// カメラの初期位置(Z座標)

#define	POS_X_CAM_AT			(0.0f)
#define	POS_Y_CAM_AT			(0.0f)
#define	POS_Z_CAM_AT			(-100.0f)

#define	VIEW_ANGLE				(D3DXToRadian(35.0f))							// ビュー平面の視野角
#define	VIEW_ASPECT				((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比	
#define	VIEW_NEAR_Z				(10.0f)											// ビュー平面のNearZ値
#define	VIEW_FAR_Z				(10000.0f)										// ビュー平面のFarZ値

#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.01f)									// カメラの移動量

/* SelectScene */

// カメラ視点の初期位置
#define	POS_X_CAM_SELECT		(0.0f)
#define	POS_Y_CAM_SELECT		(50.0f)
#define	POS_Z_CAM_SELECT		(-500.0f)

// カメラ注視点の初期位置
#define	POS_X_CAM_SELECT_AT		(0.0f)
#define	POS_Y_CAM_SELECT_AT		(50.0f)
#define	POS_Z_CAM_SELECT_AT		(-100.0f)


/* ResultScene */

// カメラ視点の初期位置
#define	POS_X_CAM_RESULT		(0.0f)
#define	POS_Y_CAM_RESULT		(50.0f)
#define	POS_Z_CAM_RESULT		(-500.0f)

// カメラ注視点の初期位置
#define	POS_X_CAM_RESULT_AT		(0.0f)
#define	POS_Y_CAM_RESULT_AT		(50.0f)
#define	POS_Z_CAM_RESULT_AT		(-100.0f)

typedef struct
{
	D3DXVECTOR3			posCameraEye;				// カメラの視点
	D3DXVECTOR3			posCameraAt;				// カメラの注視点
	D3DXVECTOR3			vecCameraUp;				// カメラの上方向ベクトル
	D3DXVECTOR3			rotCamera;					// カメラの回転

	D3DXMATRIX			mtxView;				// ビューマトリックス
	D3DXMATRIX			mtxProjection;			// プロジェクションマトリックス
	D3DXMATRIX			mtxWorld;				// ワールドマトリックス

	float				fHAngle;

}CAMERA;

//*****************************************************************************
// プロトタイプ宣言
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