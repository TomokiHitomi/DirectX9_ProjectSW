//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

/*******************************************************************************
* インクルード
*******************************************************************************/
#include "object.h"
#include "SkinMeshX.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// モデル
#define PLAYER_MODEL				("flower.X")

// モデルスケール
#define PLAYER_SCL					(0.5f)
#define PLAYER_MOVE_SPEED			(3.0f)

#define PLAYER_ROT_SPEED_X			(0.01f)
#define PLAYER_ROT_SPEED_Z			(0.03f)

#define PLAYER_HEIGHT				(30.0f)
#define PLAYER_MOVE_INERTIA			(0.3f)
#define PLAYER_ROT_INERTIA			(0.3f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************

class Player : public Object
{
private:
	CSkinMesh		*m_CSkinMesh;		// スキンメッシュ格納用
	D3DXVECTOR3		m_vPos;				// 座標情報
	D3DXVECTOR3		m_vRot;				// 回転情報
	D3DXVECTOR3		m_vScl;				// 拡縮情報
	D3DXVECTOR3		m_vMove;			// 移動量情報
	D3DXVECTOR3		m_vAxisX;
	D3DXVECTOR3		m_vAxisZ;
	D3DXVECTOR3		m_vRotIner;			// 回転慣性情報

	D3DXMATRIX		m_mtxWorld;			// ワールドマトリクス
	D3DXQUATERNION	m_qText;
	bool			m_bUse;				// 使用フラグ
public:
	enum PLAYER
	{	// プレイヤー管理
		PLAYER_1P,
		PLAYER_MAX
	};

	// コンストラクタ（初期化処理）
	Player(void);
	//デストラクタ（終了処理）
	~Player(void);

	// 更新処理
	void	Update(void);
	// 描画処理
	void	Draw(void);
private:
	void	Move(void);
	void	MoveFunc(float);
	void	RotFunc(D3DXVECTOR3);
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

#endif
