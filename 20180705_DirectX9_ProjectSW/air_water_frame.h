//=============================================================================
//
// エアー・ウォーター枠 [air_water_frame.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _AIR_WATER_FRAME_H_
#define _AIR_WATER_FRAME_H_

/*******************************************************************************
* インクルード
*******************************************************************************/
#include "object.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// テクスチャ
#define TEXTURE_AIR_WATER_FRAME				("data/TEXTURE/air_wind_frame.png")

// テクスチャスケール（フレーム）
#define TEXTURE_AIR_WATER_FRAME_SCL			(0.4f * SCREEN_SCALE)

// テクスチャ内分割数
#define TEXTURE_PATTERN_DIVIDE_X_AWFREAM	(2)	// X
#define TEXTURE_PATTERN_DIVIDE_Y_AWFREAM	(1)	// Y

// 配置時のマージン
#define AIR_WATER_FRAME_POS_MARGIN_X		(20)
#define AIR_WATER_FRAME_POS_MARGIN_Y		(22)

// テクスチャサイズ
#define TEXTURE_AIR_WATER_FRAME_SIZE_X		(768 / 2 / TEXTURE_PATTERN_DIVIDE_X_AWFREAM * TEXTURE_AIR_WATER_FRAME_SCL)
#define TEXTURE_AIR_WATER_FRAME_SIZE_Y		(512 / 2 / TEXTURE_PATTERN_DIVIDE_Y_AWFREAM * TEXTURE_AIR_WATER_FRAME_SCL)

// 反射色設定
#define AIR_WATER_FRAME_DIFFUSE				(D3DCOLOR_RGBA(160, 82, 45, 255))


/* ゲージ管理用 */

// テクスチャスケール（ゲージ）
#define TEXTURE_AIR_WATER_SCL				(0.9f)

// テクスチャサイズ（ゲージ）
#define TEXTURE_AIR_WATER_SIZE_X			(TEXTURE_AIR_WATER_FRAME_SIZE_X * TEXTURE_AIR_WATER_SCL)
#define TEXTURE_AIR_WATER_SIZE_Y			(TEXTURE_AIR_WATER_FRAME_SIZE_Y * TEXTURE_AIR_WATER_SCL)

// エアーフレームの初期位置
#define AIR_POS_FRAME_X						(TEXTURE_AIR_WATER_FRAME_SIZE_X + AIR_WATER_FRAME_POS_MARGIN_X)
#define AIR_POS_FRAME_Y						(TEXTURE_AIR_WATER_FRAME_SIZE_Y + AIR_WATER_FRAME_POS_MARGIN_Y)

// ウォーターフレームの初期位置
#define WATER_FRAME_POS_X					(-TEXTURE_AIR_WATER_FRAME_SIZE_X - AIR_WATER_FRAME_POS_MARGIN_X + SCREEN_WIDTH)
#define WATER_FRAME_POS_Y					(TEXTURE_AIR_WATER_FRAME_SIZE_Y + AIR_WATER_FRAME_POS_MARGIN_Y)


//*****************************************************************************
// 構造体定義
//*****************************************************************************

// エアー・ウォーターフレーム管理用
typedef struct
{
	VERTEX_2D	vertexWk[NUM_VERTEX];
	D3DXVECTOR3	vecPos;
	bool		bUse;
}AirWaterFreamTag;

//*****************************************************************************
// クラス定義
//*****************************************************************************

class AirWaterFream : public Object
{
public:
	// AWF管理用の列挙型
	enum FREAM
	{
		AIR,
		WATER,
		FREAM_MAX
	};
	// コンストラクタ（初期化処理）
	AirWaterFream(void);
	//デストラクタ（終了処理）
	~AirWaterFream(void);
	// 更新処理
	void	Update(void){/* 更新処理なし */}
	// 描画処理
	void	Draw(void);
private:
	// 頂点の作成
	HRESULT MakeVertex(int nNum);

	LPDIRECT3DTEXTURE9	m_pD3DTexture = NULL;	// テクスチャ
	AirWaterFreamTag	tagAWFS[FREAM_MAX];		// AWF管理用
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

#endif
