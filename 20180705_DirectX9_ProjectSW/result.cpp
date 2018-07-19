//=============================================================================
//
// リザルト処理 [result.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "result.h"

/* 全体で必要なインクルード */
#include "input.h"
#include "camera.h"
#include "sound.h"

/* リザルトで必要なインクルード */
#include "air_water_frame.h"
#include "skydome.h"

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
extern SceneManager		g_cScene;				// Sceneマネージャ

//=============================================================================
// 更新処理
//=============================================================================
void ResultScene::Update(void)
{
	Object::UpdateAll();
}

//=============================================================================
// 描画処理
//=============================================================================
void ResultScene::Draw(void)
{
	Object::DrawAll();
}

//=============================================================================
// コンストラクタ処理（初期化）
//=============================================================================
ResultScene::ResultScene(void)
{
	new Skydome;
	new AirWaterFream;
}

//=============================================================================
// デストラクタ処理（終了）
//=============================================================================
ResultScene::~ResultScene(void)
{
	Object::ReleaseAll();
}
