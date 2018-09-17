//=============================================================================
//
// モデル処理 [XModel.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "XModel.h"

// デバッグ用
#ifdef _DEBUG
#include "debugproc.h"
#endif

//=============================================================================
// コンストラクタ（初期化処理）
//=============================================================================
CXModel::CXModel(void)
{
	// モデル関係の初期化
	m_pD3DTexture = NULL;
	m_pD3DXMesh = NULL;
	m_pD3DXBuffMat = NULL;
	m_nNumMat = 0;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CXModel::Init(LPDIRECT3DDEVICE9 pDevice, LPSTR pMeshPass, LPSTR pTexPass)
{
	// モデルデータ読み込み
	if (FAILED(D3DXLoadMeshFromX(
		pMeshPass,				// モデルデータ
		D3DXMESH_SYSTEMMEM,		// 使用するメモリのオプション
		pDevice,				// デバイス
		NULL,					// 未使用
		&m_pD3DXBuffMat,		// マテリアルデータ
		NULL,					// 未使用
		&m_nNumMat,				// D3DXMATERIAL構造体の数
		&m_pD3DXMesh)))			// メッシュデータへのポインタ
	{
		return E_FAIL;
	}

	// テクスチャの読み込み
	if (FAILED(D3DXCreateTextureFromFile(
		pDevice,				// デバイス
		pTexPass,				// ファイル名
		&m_pD3DTexture)))		// 読み込むメモリ（複数なら配列に）
	{
		return E_FAIL;
	}
}

//=============================================================================
// 解放処理
//=============================================================================
void CXModel::Release(void)
{
	if (m_pD3DTexture != NULL)
	{// テクスチャの開放
		m_pD3DTexture->Release();
		m_pD3DTexture = NULL;
	}

	if (m_pD3DXMesh != NULL)
	{// メッシュの開放
		m_pD3DXMesh->Release();
		m_pD3DXMesh = NULL;
	}

	if (m_pD3DXBuffMat != NULL)
	{// マテリアルの開放
		m_pD3DXBuffMat->Release();
		m_pD3DXBuffMat = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CXModel::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CXModel::Draw(D3DXMATRIX mtxWorld)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	// ワールドマトリクスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	/******************** ビューポート変換 ********************/
	// 現在のマテリアルを保存
	pDevice->GetMaterial(&matDef);
	// マテリアル情報に対するポインタの取得
	pD3DXMat = (D3DXMATERIAL*)m_pD3DXBuffMat->GetBufferPointer();

	for (int i = 0; i < (int)m_nNumMat; i++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[i].MatD3D);
		// テクスチャの設定（NULL:テクスチャ無し）
		pDevice->SetTexture(0, m_pD3DTexture);
		// 描画
		m_pD3DXMesh->DrawSubset(i);
	}

	// マテリアルを元に戻す
	pDevice->SetMaterial(&matDef);
}