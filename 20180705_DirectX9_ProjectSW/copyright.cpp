//=============================================================================
//
// コピーライト処理 [copyright.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "main.h"
#include "copyright.h"


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
Copyright::Copyright(void)
{
	// オブジェクトIDとプライオリティの設定処理
	SetIdAndPriority(ObjectID::COPYRIGHT, Priority::High, Priority::Normal);

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// テクスチャの読み込み（Debugはファイル／Releaseはアーカイブから）
#ifdef _DEBUG
	if (FAILED(D3DXCreateTextureFromFile(pDevice,	// デバイスのポインタ
		TEXTURE_COPYRIGHT,							// ファイルの名前
		&m_pD3DTexture)))							// 読み込むメモリのポインタ
#else
	if (FAILED(D3DXCreateTextureFromArchive(pDevice,	// デバイスのポインタ
		TEXTURE_COPYRIGHT,								// ファイルの名前
		&m_pD3DTexture)))								// 読み込むメモリのポインタ
#endif
	{
		MessageBox(NULL, "テクスチャの読み込みに失敗しました", "Copyright", MB_OK);
	}

	// エアー用の座標に設定
	tagUi[CORYRIGHT_LOGO].vecPos = COPYRIGHT_POS;

	// 共通プロパティの初期化
	for (unsigned int i = 0; i < TEXTURE_COPYRIGHT_MAX; i++)
	{
		// 使用フラグをtrueに設定
		tagUi[i].bUse = true;
		// 頂点情報の作成
		MakeVertex(i);
	}
}

//=============================================================================
// デストラクタ（終了処理）
//=============================================================================
Copyright::~Copyright(void)
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
//void Copyright::Update(void)
//{
//		// 更新処理なし
//}

//=============================================================================
// 描画処理
//=============================================================================
void Copyright::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (unsigned int i = 0; i < TEXTURE_COPYRIGHT_MAX; i++)
	{
		// 使用フラグがtureの場合
		if (tagUi[i].bUse)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, m_pD3DTexture);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			// ポリゴンの描画
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, tagUi[i].vertexWk, sizeof(VERTEX_2D));
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT Copyright::MakeVertex(int nNum)
{
	// 頂点座標の設定
	tagUi[nNum].vertexWk[0].vtx = D3DXVECTOR3(tagUi[nNum].vecPos.x - TEXTURE_COPYRIGHT_SIZE_X, tagUi[nNum].vecPos.y - TEXTURE_COPYRIGHT_SIZE_Y, 0.0f);
	tagUi[nNum].vertexWk[1].vtx = D3DXVECTOR3(tagUi[nNum].vecPos.x + TEXTURE_COPYRIGHT_SIZE_X, tagUi[nNum].vecPos.y - TEXTURE_COPYRIGHT_SIZE_Y, 0.0f);
	tagUi[nNum].vertexWk[2].vtx = D3DXVECTOR3(tagUi[nNum].vecPos.x - TEXTURE_COPYRIGHT_SIZE_X, tagUi[nNum].vecPos.y + TEXTURE_COPYRIGHT_SIZE_Y, 0.0f);
	tagUi[nNum].vertexWk[3].vtx = D3DXVECTOR3(tagUi[nNum].vecPos.x + TEXTURE_COPYRIGHT_SIZE_X, tagUi[nNum].vecPos.y + TEXTURE_COPYRIGHT_SIZE_Y, 0.0f);

	// rhwの設定
	tagUi[nNum].vertexWk[0].rhw =
	tagUi[nNum].vertexWk[1].rhw =
	tagUi[nNum].vertexWk[2].rhw =
	tagUi[nNum].vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	tagUi[nNum].vertexWk[0].diffuse = COPYRIGHT_DIFFUSE;
	tagUi[nNum].vertexWk[1].diffuse = COPYRIGHT_DIFFUSE;
	tagUi[nNum].vertexWk[2].diffuse = COPYRIGHT_DIFFUSE;
	tagUi[nNum].vertexWk[3].diffuse = COPYRIGHT_DIFFUSE;

	// テクスチャ座標の設定
	int x = nNum % TEXTURE_PATTERN_DIVIDE_X_COPYRIGHT;
	int y = nNum / TEXTURE_PATTERN_DIVIDE_X_COPYRIGHT;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_COPYRIGHT;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_COPYRIGHT;
	tagUi[nNum].vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	tagUi[nNum].vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	tagUi[nNum].vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	tagUi[nNum].vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	return S_OK;
}
