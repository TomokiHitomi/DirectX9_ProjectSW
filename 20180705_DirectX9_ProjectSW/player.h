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
#include "XTexture.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// モデル
//#define PLAYER_MODEL				("flower.X")
#define PLAYER_MODEL				("test3.X")
#define PLAYER_MODEL_WING			("test_wing2.X")
#define PLAYER_MODEL_BONE_WING		("No_9_joint_Torso2")
#define PLAYER_MODEL_BONE_RM		("No_45_joint_RightMiddle2")

// モデルスケール
#define PLAYER_SCL					(0.5f)

#define PLAYER_MOVE_SPEED			(0.05f)
#define PLAYER_MOVE_SPEED_MAX		(10.0f)
#define PLAYER_MOVE_SPEED_MIN		(1.0f)

#define PLAYER_ROT_SPEED_X			(0.0005f)
#define PLAYER_ROT_SPEED_Z			(0.0015f)

#define PLAYER_ROT_SPEED_MAX_X		(0.02f)
#define PLAYER_ROT_SPEED_MAX_Z		(0.03f)

#define PLAYER_HEIGHT				(30.0f)
#define PLAYER_MOVE_INERTIA			(0.3f)
#define PLAYER_ROT_INERTIA			(0.3f)

#define PLAYER_ALPHA_TEST			(150)

/***** FloatMode *****/
#define PLAYER_FLOAT_ROT_X			(D3DX_PI * 0.5f)
#define PLAYER_FLOAT_ROT_Y			(D3DX_PI)

/***** FlyMode *****/

/***** ChangeMode *****/
#define PLAYER_CHANGE_FRAME			(10)

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************

class Player : public Object
{
private:
	enum MODE
	{	// プレイヤーのモード管理
		MODE_FLOAT,
		MODE_FLY,
		MODE_LOCKON,
		MODE_CHANGE,
		MODE_MAX
	};
	enum MODEL_TYPE
	{
		CHARACTER,
		WING,
		MODEL_TYPE_MAX
	};
	CSkinMesh		*m_CSkinMesh[MODEL_TYPE_MAX];	// スキンメッシュ格納用

	D3DXVECTOR3		m_vPos;				// 座標情報
	D3DXVECTOR3		m_vRot;				// 回転情報
	D3DXVECTOR3		m_vScl;				// 拡縮情報
	D3DXVECTOR3		m_vMove;			// 移動量情報
	D3DXVECTOR3		m_vRotIner;			// 回転情報
	D3DXVECTOR3		m_vRotChange;		// 回転情報
	D3DXVECTOR3		m_vUp;

	// ローカルベクトル
	D3DXVECTOR3		m_vX;
	D3DXVECTOR3		m_vY;
	D3DXVECTOR3		m_vZ;

	D3DXMATRIX		m_mtxWorld;			// ワールドマトリクス
	MODE			m_eMode;
	MODE			m_eModeNext;
	MODE			m_eModeOld;
	
	int				m_nCount;			// 汎用カウンター
	float			m_fMoveSpeed;
	float			m_fRiseSpeed;
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
	void	Float(void);
	void	Fly(void);
	void	LOCKON(void);
	void	Change(void);
	void	ModeChange(void);
	void	Move(void);
	void	MoveFunc(float);
	void	RotFunc(D3DXVECTOR3);
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

#endif
