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
#include "XModel.h"
#include "XTexture.h"
#include "sword.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// モデル
//#define PLAYER_MODEL				("flower.X")
#define PLAYER_MODEL				("test3.X")
#define PLAYER_MODEL_WING			("test_wing2.X")
#define PLAYER_MODEL_SWORD			("sword.X")
#define PLAYER_MODEL_SWORD_TEX		("UV_Yat.tga")

#define PLAYER_POS					(D3DXVECTOR3(0.0f,-300.0f,500.0f))

#define PLAYER_WING_SCL				(0.5f)
#define PLAYER_WING_ROT_X			(1.7f)

#define PLAYER_SWORD_MAX			(10)
#define PLAYER_SWORD_SCL			(0.5f)

#define PLAYER_MODEL_BONE_WING		("No_9_joint_Torso2")
#define PLAYER_MODEL_BONE_RM		("No_45_joint_RightMiddle2")

// モデルスケール
#define PLAYER_SCL					(0.5f)

#define PLAYER_MOVE_SPEED			(0.05f)
#define PLAYER_MOVE_SPEED_MAX		(10.0f)
#define PLAYER_MOVE_SPEED_MIN		(3.0f)

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

// テスト用
#define PLAYER_GYRO_MARGIN			(300.0f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************

class Player : public ObjectManager
{
public:
	enum MODE
	{	// プレイヤーのモード管理
		MODE_FLOAT,
		MODE_FLY,
		MODE_LOCKON,
		MODE_CHANGE,
		MODE_MAX
	};
	enum MODEL_ANIM
	{
		CHARACTER,
		WING,
		MODEL_ANIM_MAX
	};
	enum MODEL_NORMAL
	{
		SWORD,
		MODEL_NORMAL_MAX
	};
	CSkinMesh		*m_CSkinMesh[MODEL_ANIM_MAX];	// スキンメッシュ格納用
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
	D3DXMATRIX		m_mtxWorldWing;			// ワールドマトリクス

	MODE			m_eMode;
	MODE			m_eModeNext;
	MODE			m_eModeOld;

	int				m_nCount;			// 汎用カウンター
	float			m_fMoveSpeed;
	float			m_fRiseSpeed;
	bool			m_bUse;				// 使用フラグ

	void	Float(void);
	void	Fly(void);
	void	LOCKON(void);
	void	Change(void);
	void	ModeChange(void);
	void	Move(void);
	void	MoveFunc(float);
	void	RotFunc(D3DXVECTOR3);

	Sword	*m_cSword;
public:
	void GetVecX(D3DXVECTOR3* vX) { *vX = m_vX; }
	void GetVecY(D3DXVECTOR3* vY) { *vY = m_vY; }
	void GetVecZ(D3DXVECTOR3* vZ) { *vZ = m_vZ; }
	D3DXVECTOR3 GetPos(void) { return m_vPos; }
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

#endif
