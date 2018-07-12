//=============================================================================
//
// エアー・ウォーター枠処理 [air_water_frame.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "main.h"
#include "air_water_frame.h"


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

//=============================================================================
// コンストラクタ（初期化処理）
//=============================================================================
AirWaterFream::AirWaterFream(void)
{

	// オブジェクトIDとプライオリティの設定処理
	SetIdAndPriority(ObjectID::FRAME, Priority::Low, Priority::Lowest);


	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// テクスチャの読み込み（Debugはファイル／Releaseはアーカイブから）
#ifdef _DEBUG
	if (FAILED(D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
		TEXTURE_AIR_WATER_FRAME,					// ファイルの名前
		&m_pD3DTexture)))				// 読み込むメモリのポインタ
#else
	if (FAILED(D3DXCreateTextureFromArchive(pDevice,	// デバイスのポインタ
		TEXTURE_AIR_WATER_FRAME,					// ファイルの名前
		&m_pD3DTexture)))				// 読み込むメモリのポインタ
#endif
	{
		MessageBox(NULL, "テクスチャの読み込みに失敗しました", "AirWaterFream", MB_OK);
	}

	// エアー用の座標に設定
	tagAWFS[AIR].vecPos = 
		D3DXVECTOR3((float)AIR_POS_FRAME_X, (float)AIR_POS_FRAME_Y, 0.0f);
	// ウォーター用の座標に設定
	tagAWFS[WATER].vecPos = 
		D3DXVECTOR3((float)WATER_FRAME_POS_X, (float)WATER_FRAME_POS_Y, 0.0f);

	// 共通プロパティの初期化
	for (unsigned int i = 0; i < FREAM_MAX; i++)
	{
		// 使用フラグをtrueに設定
		tagAWFS[i].bUse = true;
		// 頂点情報の作成
		MakeVertex(i);
	}
}

//=============================================================================
// デストラクタ（終了処理）
//=============================================================================
AirWaterFream::~AirWaterFream(void)
{
	if(m_pD3DTexture != NULL )
	{	// テクスチャの開放
		m_pD3DTexture->Release();
		m_pD3DTexture = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
//void AirWaterFream::Update(void)
//{
//		// 更新処理なし
//}

//=============================================================================
// 描画処理
//=============================================================================
void AirWaterFream::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (unsigned int i = 0; i < FREAM_MAX; i++)
	{
		// 使用フラグがtureの場合
		if (tagAWFS[i].bUse)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, m_pD3DTexture);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, tagAWFS[i].vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT AirWaterFream::MakeVertex(int nNum)
{
	// 頂点座標の設定
	tagAWFS[nNum].vertexWk[0].vtx = D3DXVECTOR3(tagAWFS[nNum].vecPos.x - TEXTURE_AIR_WATER_FRAME_SIZE_X, tagAWFS[nNum].vecPos.y - TEXTURE_AIR_WATER_FRAME_SIZE_Y, 0.0f);
	tagAWFS[nNum].vertexWk[1].vtx = D3DXVECTOR3(tagAWFS[nNum].vecPos.x + TEXTURE_AIR_WATER_FRAME_SIZE_X, tagAWFS[nNum].vecPos.y - TEXTURE_AIR_WATER_FRAME_SIZE_Y, 0.0f);
	tagAWFS[nNum].vertexWk[2].vtx = D3DXVECTOR3(tagAWFS[nNum].vecPos.x - TEXTURE_AIR_WATER_FRAME_SIZE_X, tagAWFS[nNum].vecPos.y + TEXTURE_AIR_WATER_FRAME_SIZE_Y, 0.0f);
	tagAWFS[nNum].vertexWk[3].vtx = D3DXVECTOR3(tagAWFS[nNum].vecPos.x + TEXTURE_AIR_WATER_FRAME_SIZE_X, tagAWFS[nNum].vecPos.y + TEXTURE_AIR_WATER_FRAME_SIZE_Y, 0.0f);

	// rhwの設定
	tagAWFS[nNum].vertexWk[0].rhw =
	tagAWFS[nNum].vertexWk[1].rhw =
	tagAWFS[nNum].vertexWk[2].rhw =
	tagAWFS[nNum].vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	tagAWFS[nNum].vertexWk[0].diffuse = AIR_WATER_FRAME_DIFFUSE;
	tagAWFS[nNum].vertexWk[1].diffuse = AIR_WATER_FRAME_DIFFUSE;
	tagAWFS[nNum].vertexWk[2].diffuse = AIR_WATER_FRAME_DIFFUSE;
	tagAWFS[nNum].vertexWk[3].diffuse = AIR_WATER_FRAME_DIFFUSE;

	// テクスチャ座標の設定
	int x = nNum % TEXTURE_PATTERN_DIVIDE_X_AWFREAM;
	int y = nNum / TEXTURE_PATTERN_DIVIDE_X_AWFREAM;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_AWFREAM;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_AWFREAM;
	tagAWFS[nNum].vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	tagAWFS[nNum].vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	tagAWFS[nNum].vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	tagAWFS[nNum].vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	return S_OK;
}
