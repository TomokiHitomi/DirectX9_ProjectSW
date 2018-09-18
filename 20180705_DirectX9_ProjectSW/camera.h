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
#define	POS_X_CAM				(0.0f)										// カメラの初期位置(X座標)
#define	POS_Y_CAM				(25.0f)										// カメラの初期位置(Y座標)
#define	POS_Z_CAM				(100.0f)										// カメラの初期位置(Z座標)

#define	VIEW_ANGLE				(D3DXToRadian(35.0f))							// ビュー平面の視野角
#define	VIEW_ASPECT				((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比	
#define	VIEW_NEAR_Z				(10.0f)											// ビュー平面のNearZ値
#define	VIEW_FAR_Z				(5000.0f)										// ビュー平面のFarZ値

class Camera
{
private:
	static D3DXVECTOR3	s_vEye;				// カメラの視点
	static D3DXVECTOR3	s_vAt;				// カメラの注視点
	static D3DXVECTOR3	s_vUp;				// カメラの上方向ベクトル

	static D3DXVECTOR3	s_vEyeNext;			// カメラの視点
	static D3DXVECTOR3	s_vAtNext;			// カメラの注視点
	static D3DXVECTOR3	s_vUpNext;			// カメラの上方向ベクトル

	static D3DXMATRIX	s_mtxView;			// ビューマトリックス
	static D3DXMATRIX	s_mtxProjection;	// プロジェクションマトリックス
	static D3DXMATRIX	s_mtxWorld;			// ワールドマトリックス

	static float		s_fEyeIner;			// カメラの視点
	static float		s_fAtIner;			// カメラの注視点
	static float		s_fUpIner;			// カメラの上方向ベクトル
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

	static void SetEye(D3DXVECTOR3 vEye);
	static void SetUp(D3DXVECTOR3 vUp);
	static void Set(void);
	//D3DXMATRIX GetInvRotateMat(D3DXVECTOR3 pos);
	static D3DXMATRIX GetMtxView(void);
	static D3DXMATRIX GetMtxProjection(void);
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
#endif