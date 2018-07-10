//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "title.h"

/* 全体で必要なインクルード */
#include "input.h"
#include "camera.h"
#include "sound.h"

/* タイトルで必要なインクルード */
#include "copyright.h"
#include "air_water_frame.h"
#include "object.h"
#include "SkinMeshX.h"
#include "light.h"

/* Debug */
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
CSkinMesh			m_CSkinMesh;		// スキンメッシュ格納用
D3DXMATRIX			worldmtx;

//=============================================================================
// 更新処理
//=============================================================================
void TitleScene::Update(void)
{
	Object::UpdateAll();



	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	/******************** ワールド変換 ********************/
	// ワールドマトリクスの初期化
	D3DXMatrixIdentity(&worldmtx);

	// 【S】スケールを反映(Multiplyは行列計算)
	D3DXMatrixScaling(&mtxScl, 3.0f, 3.0f, 3.0f);
	D3DXMatrixMultiply(&worldmtx, &worldmtx, &mtxScl);

	// 【R】回転を反映(YawPitchRollはy,x,z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&worldmtx, &worldmtx, &mtxRot);

	// 【T】平行移動を反映(オブジェクトを配置している）
	D3DXMatrixTranslation(&mtxTranslate, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&worldmtx, &worldmtx, &mtxTranslate);

	m_CSkinMesh.Update(worldmtx);

}

//=============================================================================
// 描画処理
//=============================================================================
void TitleScene::Draw(void)
{

	// ライティングを強めにあてる
	SetLight(LIGHT_SUB1, TRUE);
	SetLight(LIGHT_SUB2, TRUE);

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	m_CSkinMesh.Draw(pDevice);

	// ライティングを通常に戻す
	SetLight(LIGHT_SUB1, FALSE);
	SetLight(LIGHT_SUB2, FALSE);


	Object::DrawAll();


}

//=============================================================================
// コンストラクタ処理（初期化）
//=============================================================================
TitleScene::TitleScene(void)
{
	//Object *CObject = new Copyright;
	//Object *CObject2 = new AirWaterFream;

	new Copyright;
	new AirWaterFream;

	//Object::Create(new Copyright);

	//NewObject<Copyright>::Create();
	//NewObject<AirWaterFream>::Create();
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	m_CSkinMesh.Init(pDevice, "data/MODEL/Yuko.x");

}

//=============================================================================
// デストラクタ処理（終了）
//=============================================================================
TitleScene::~TitleScene(void)
{
	Object::ReleaseAll();
	m_CSkinMesh.Release();
}

//=============================================================================
// タイトルデモのリスタートメソッド
//=============================================================================
void TitleScene::DemoRestart(void)
{

}