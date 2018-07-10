//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "main.h"
#include "scene.h"

// デバッグ用
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
#ifdef _DEBUG
void DebugCamera(CAMERA *camera);
void DebugMoveCamera(CAMERA *camera);
void DebugZoomCamera(CAMERA *camera);
void DebugResetCamera(CAMERA *camera);
#endif
//*****************************************************************************
// グローバル変数
//*****************************************************************************
CAMERA					cameraWk;

#ifdef _DEBUG
D3DXVECTOR3 g_posCameraEyeToAt;
#endif

extern SceneManager		g_cScene;				// Sceneマネージャ

//=============================================================================
// 初期化処理
//=============================================================================
void InitCamera(void)
{
	CAMERA *camera = &cameraWk;

	// 現在のシーンを取得
	SceneManager::SCENE eScene = g_cScene.GetScene();

	// RESULTシーンの場合
	if (eScene == SceneManager::RESULT)
	{
		camera->posCameraEye = D3DXVECTOR3(POS_X_CAM_RESULT, POS_Y_CAM_RESULT, POS_Z_CAM_RESULT);
		camera->posCameraAt = D3DXVECTOR3(POS_X_CAM_RESULT_AT, POS_Y_CAM_RESULT_AT, POS_Z_CAM_RESULT_AT);
		camera->vecCameraUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		camera->fHAngle = CAMERA_H_ANGLE;
	}
	// TITLE・GAMEシーンの場合
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
// 終了処理
//=============================================================================
void UninitCamera(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCamera(void)
{
	CAMERA *camera = &cameraWk;
#ifdef _DEBUG
	PrintDebugProc("【 CAMERA 】\n");
	PrintDebugProc("Eye [X:%f  Y:%f  Z:%f]\n", camera->posCameraEye.x, camera->posCameraEye.y, camera->posCameraEye.z);
	PrintDebugProc("At  [X:%f  Y:%f  Z:%f]\n", camera->posCameraAt.x, camera->posCameraAt.y, camera->posCameraAt.z);
	PrintDebugProc("\n");
	DebugCamera(camera);
#endif

//#ifdef _DEBUG
//	CAMERA *camera = GetCamera();
//
//	//デバッグ時にZCでカメラ回転
//	if (GetKeyboardPress(DIK_Z))
//	{// 視点旋回「左」
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
//	{// 視点旋回「右」
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
// カメラの設定処理
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	CAMERA	*camera = &cameraWk;
	/******************** ビューイング変換 ********************/
	// ビューマトリクスの初期化
	D3DXMatrixIdentity(&camera->mtxView);

	// ビューマトリクスの作成
	D3DXMatrixLookAtLH(&camera->mtxView,
		&camera->posCameraEye,	// 視点
		&camera->posCameraAt,	// 注視点
		&camera->vecCameraUp);	// 上方向

								// ビューマトリクスの設定
	pDevice->SetTransform(D3DTS_VIEW, &camera->mtxView);


	/******************** プロジェクション変換 ********************/
	// プロジェクションマトリクスの初期化
	D3DXMatrixIdentity(&camera->mtxProjection);

	// プロジェクションマトリクスの作成
	D3DXMatrixPerspectiveFovLH(&camera->mtxProjection,
		VIEW_ANGLE,			// ビュー平面の視野角
		VIEW_ASPECT,		// ビュー平面のアスペクト比
		VIEW_NEAR_Z,		// ビュー平面のNearZ値（近いと描画しない）
		VIEW_FAR_Z);		// ビュー平面のFarZ値（遠いと描画しない）

							// プロジェクションマトリクスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &camera->mtxProjection);
}

//=============================================================================
// カメラの向きの取得
//=============================================================================
D3DXVECTOR3 GetRotCamera(void)
{
	CAMERA	*camera = &cameraWk;
	return camera->rotCamera;
}

//=============================================================================
// ビューマトリックスの取得
//=============================================================================
D3DXMATRIX GetMtxView(void)
{
	CAMERA	*camera = &cameraWk;
	return camera->mtxView;
}

//=============================================================================
// プロジェクションマトリックスの取得
//=============================================================================
D3DXMATRIX GetMtxProjection(void)
{
	CAMERA	*camera = &cameraWk;
	return camera->mtxProjection;
}

//=============================================================================
// カメラ取得関数
//=============================================================================
CAMERA *GetCamera(void)
{
	return(&cameraWk);
}

#ifdef _DEBUG


//=============================================================================
// カメラの移動処理（デバッグ用）
//=============================================================================
void DebugCamera(CAMERA *camera)
{
	// 左シフトキー入力中
	if (GetKeyboardPress(DIK_LSHIFT))
	{
		DebugMoveCamera(camera);	// カメラ移動処理
		DebugZoomCamera(camera);	// カメラ拡縮処理
		DebugResetCamera(camera);	// カメラ初期化処理
	}
}

//=============================================================================
// カメラの移動処理（デバッグ用）
//=============================================================================
void DebugMoveCamera(CAMERA *camera)
{
	// 前後移動
	if (GetKeyboardPress(DIK_W) && GetKeyboardPress(DIK_S))
	{	// 移動なし
	}
	else if (GetKeyboardPress(DIK_W))
	{	// 前方へ移動
		camera->posCameraEye.z += CAMERA_MOVE_SPEED;
		camera->posCameraAt.z += CAMERA_MOVE_SPEED;
	}
	else if (GetKeyboardPress(DIK_S))
	{	// 後方へ移動
		camera->posCameraEye.z -= CAMERA_MOVE_SPEED;
		camera->posCameraAt.z -= CAMERA_MOVE_SPEED;
	}

	// 左右移動
	if (GetKeyboardPress(DIK_D) && GetKeyboardPress(DIK_A))
	{	// 移動なし
	}
	else if (GetKeyboardPress(DIK_D))
	{	// 右方へ移動
		camera->posCameraEye.x += CAMERA_MOVE_SPEED;
		camera->posCameraAt.x += CAMERA_MOVE_SPEED;
	}
	else if (GetKeyboardPress(DIK_A))
	{	// 左方へ移動
		camera->posCameraEye.x -= CAMERA_MOVE_SPEED;
		camera->posCameraAt.x -= CAMERA_MOVE_SPEED;
	}
}

//=============================================================================
// カメラの拡縮処理（デバッグ用）
//=============================================================================
void DebugZoomCamera(CAMERA *camera)
{
	if (GetKeyboardPress(DIK_UP) && GetKeyboardPress(DIK_DOWN))
	{	// 拡縮なし
	}
	else if (GetKeyboardPress(DIK_UP))
	{	// 拡大
		camera->posCameraEye -= g_posCameraEyeToAt * CAMERA_ZOOM_SPEED;
	}
	else if (GetKeyboardPress(DIK_DOWN))
	{	// 縮小
		camera->posCameraEye += g_posCameraEyeToAt * CAMERA_ZOOM_SPEED;
	}
}
//=============================================================================
// カメラのリセット処理（デバッグ用）
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