//=============================================================================
//
// コピーライト処理 [copyright.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _COPYRIGHT_H_
#define _COPYRIGHT_H_

/*******************************************************************************
* インクルード
*******************************************************************************/
#include "object.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// テクスチャ
#define TEXTURE_COPYRIGHT				("data/TEXTURE/copyright.png")

// テクスチャの最大数
#define TEXTURE_COPYRIGHT_MAX			(1)

// テクスチャスケール（フレーム）
#define TEXTURE_COPYRIGHT_SCL			(0.5f * SCREEN_SCALE)

// テクスチャ内分割数
#define TEXTURE_PATTERN_DIVIDE_X_COPYRIGHT	(1)	// X
#define TEXTURE_PATTERN_DIVIDE_Y_COPYRIGHT	(1)	// Y

// テクスチャサイズ
#define TEXTURE_COPYRIGHT_SIZE_X		(1024 / 2 / TEXTURE_PATTERN_DIVIDE_X_COPYRIGHT * TEXTURE_COPYRIGHT_SCL)
#define TEXTURE_COPYRIGHT_SIZE_Y		(128 / 2 / TEXTURE_PATTERN_DIVIDE_Y_COPYRIGHT * TEXTURE_COPYRIGHT_SCL)

// 反射色設定
#define COPYRIGHT_DIFFUSE				(D3DCOLOR_RGBA(255, 255, 255, 255))

//// 反射色設定
//#define COPYRIGHT_DIFFUSE				(D3DCOLOR_RGBA(160, 82, 45, 255))

// 初期位置
#define COPYRIGHT_POS			(D3DXVECTOR3(900.0f * SCREEN_SCALE, 690.0f * SCREEN_SCALE, 0.0f))

//*****************************************************************************
// 構造体定義
//*****************************************************************************

// テクスチャ管理用
typedef struct
{
	VERTEX_2D	vertexWk[NUM_VERTEX];
	D3DXVECTOR3	vecPos;
	bool		bUse;
}CopyrightTag;

//*****************************************************************************
// クラス定義
//*****************************************************************************

class Copyright : public Object
{
public:
	// AWF管理用の列挙型
	enum COPYRIGHT
	{
		CORYRIGHT_LOGO
	};
	// コンストラクタ（初期化処理）
	Copyright(void);
	//デストラクタ（終了処理）
	~Copyright(void);
	// 更新処理
	void	Update(void){/* 更新処理なし */}
	// 描画処理
	void	Draw(void);
private:
	// 頂点の作成
	HRESULT MakeVertex(int nNum);

	LPDIRECT3DTEXTURE9	m_pD3DTexture = NULL;		// テクスチャ
	CopyrightTag	tagUi[TEXTURE_COPYRIGHT_MAX];	// テクスチャ管理用
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

#endif
