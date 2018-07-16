//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "main.h"
#include "calculate.h"

// デバッグ用
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
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
// 初期化処理
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
// 終了処理
//=============================================================================
void Camera::Uninit(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void Camera::Update(void)
{
	s_vEye = s_vEye + ((s_vEyeNext - s_vEye) * s_fEyeIner);
	s_vAt = s_vAt + ((s_vAtNext - s_vAt) * s_fAtIner);

#ifdef _DEBUG
	PrintDebugProc("【 Camera 】\n");
	PrintDebugProc("Eye [%f,%f,%f]  Iner[%f]\n", s_vEye.x, s_vEye.y, s_vEye.z, s_fEyeIner);
	PrintDebugProc("At  [%f,%f,%f]  Iner[%f]\n", s_vAt.x, s_vAt.y, s_vAt.z, s_fAtIner);
	PrintDebugProc("Up  [%f,%f,%f]  Iner[%f]\n", s_vUp.x, s_vUp.y, s_vUp.z, s_fUpIner);
	PrintDebugProc("\n");
#endif
}

//=============================================================================
// カメラの注視点設定処理
//=============================================================================
void Camera::SetAt(D3DXVECTOR3 vAt)
{
	//camera->posCameraAt = vAt;
	s_vAtNext = vAt;
}

//=============================================================================
// カメラの視点設定処理
//=============================================================================
void Camera::SetEye(D3DXVECTOR3 vEye)
{
	//camera->posCameraEye = vEye;
	s_vEyeNext = vEye;
}

//=============================================================================
// カメラの上部設定処理
//=============================================================================
void Camera::SetUp(D3DXVECTOR3 vUp)
{
	s_vUp = vUp;
	//camera->vUpNext = vUp;
}

//=============================================================================
// カメラの注視点の慣性設定処理
//=============================================================================
void Camera::SetAtIner(float fIner)
{
	s_fAtIner = SetLimit(fIner, 1.0f, 0.0f);
}

//=============================================================================
// カメラの視点の慣性設定処理
//=============================================================================
void Camera::SetEyeIner(float fIner)
{
	s_fEyeIner = SetLimit(fIner, 1.0f, 0.0f);
}

//=============================================================================
// カメラの上部の慣性設定処理
//=============================================================================
void Camera::SetUpIner(float fIner)
{
	s_fUpIner = SetLimit(fIner, 1.0f, 0.0f);
}

//=============================================================================
// カメラの注視点の慣性増減処理
//=============================================================================
void Camera::AddAtIner(float fIner)
{
	s_fAtIner = SetLimit(s_fAtIner + fIner, 1.0f, 0.0f);
}

//=============================================================================
// カメラの視点の慣性増減処理
//=============================================================================
void Camera::AddEyeIner(float fIner)
{
	s_fEyeIner = SetLimit(s_fEyeIner + fIner, 1.0f, 0.0f);
}

//=============================================================================
// カメラの上部の慣性増減処理
//=============================================================================
void Camera::AddUpIner(float fIner)
{
	s_fUpIner = SetLimit(s_fUpIner + fIner, 1.0f, 0.0f);
}


//=============================================================================
// カメラの設定処理
//=============================================================================
void Camera::Set(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	/******************** ビューイング変換 ********************/
	// ビューマトリクスの初期化
	D3DXMatrixIdentity(&s_mtxView);

	// ビューマトリクスの作成
	D3DXMatrixLookAtLH(&s_mtxView,
		&s_vEye,	// 視点
		&s_vAt,		// 注視点
		&s_vUp);	// 上方向

	// ビューマトリクスの設定
	pDevice->SetTransform(D3DTS_VIEW, &s_mtxView);


	/******************** プロジェクション変換 ********************/
	// プロジェクションマトリクスの初期化
	D3DXMatrixIdentity(&s_mtxProjection);

	// プロジェクションマトリクスの作成
	D3DXMatrixPerspectiveFovLH(&s_mtxProjection,
		VIEW_ANGLE,			// ビュー平面の視野角
		VIEW_ASPECT,		// ビュー平面のアスペクト比
		VIEW_NEAR_Z,		// ビュー平面のNearZ値（近いと描画しない）
		VIEW_FAR_Z);		// ビュー平面のFarZ値（遠いと描画しない）

	// プロジェクションマトリクスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &s_mtxProjection);
}

////=============================================================================
//// カメラと対になる回転行列を取得
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
//// 注視点と視点の正規化されたベクトルを取得する関数
////=============================================================================
//D3DXVECTOR3 Camera::GetLookAtVct(void)
//{
//	CAMERA *camera = GetCamera();
//	D3DXVECTOR3	vecLookAtVct;
//	vecLookAtVct = camera->posCameraEye - camera->posCameraAt;
//	return vecLookAtVct;
//}

//=============================================================================
// ビューマトリックスの取得
//=============================================================================
D3DXMATRIX Camera::GetMtxView(void)
{
	return s_mtxView;
}

//=============================================================================
// プロジェクションマトリックスの取得
//=============================================================================
D3DXMATRIX Camera::GetMtxProjection(void)
{
	return s_mtxProjection;
}