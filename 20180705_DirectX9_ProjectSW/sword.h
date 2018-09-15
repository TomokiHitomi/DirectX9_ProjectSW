//=============================================================================
//
// ソード処理 [sword.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _SWORD_H_
#define _SWORD_H_

/*******************************************************************************
* インクルード
*******************************************************************************/
#include "object.h"
#include "XModel.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// モデル
#define SWORD_MODEL					("sword.X")
#define SWORD_MODEL_TEX				("UV_Yat.tga")

#define SWORD_MAX					(50)
#define SWORD_MAX_INIT				(8)
#define SWORD_SCL					(0.5f)

#define SWORD_SPEED_MIN				(8.0f)
#define SWORD_SPEED_MAX				(8.0f)
#define SWORD_SPEED_RETRUN			(8.0f)
#define SWORD_SPEED_ADD				(5)


//#define SWORD_ROT_SPEED			(0.02f)
#define SWORD_ROT_SPEED_MIN			(-0.1f)
#define SWORD_ROT_SPEED_COUNT		(30)
#define SWORD_ROT_SPEED_MAX			(0.03f)
#define SWORD_ROT_SPEED_ADD			(20)
#define SWORD_ROT_SPEED_SUB			(3)

#define SWORD_ROT_LENGTH			(10.0f)
#define SWORD_ROT_ADJUST_TIME		(15)

#define SWORD_TRUN					(0.1f)
#define SWORD_LENGTH_RETRUN			(200.0f)
#define SWORD_LENGTH_RETRUN_SUB		(3.5f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************


class Sword
{
public:

	// コンストラクタ（初期化処理）
	Sword(void);
	//デストラクタ（終了処理）
	~Sword(void);

	// 更新処理
	void	Update(void);
	// 描画処理
	void	Draw(void);
private:
	CXModel			*m_CXModel;			// モデル格納用
	class SwordData
	{
	public:
		enum Mode
		{
			STAY,
			SHOT,
			RETURN
		};
		D3DXVECTOR3			vPos;			// 座標情報

		// ローカルベクトル
		D3DXVECTOR3			vX;
		D3DXVECTOR3			vY;
		D3DXVECTOR3			vZ;
		SwordData::Mode		eMode;
		float				fSpeed;
		float				fLength;
		float				fAngle;
		bool				bUse;
	};

	enum Mode
	{
		SHOT,
		ROT,
		RETURN
	};
	Sword::Mode		m_eMode;

	SwordData		m_tagSword[SWORD_MAX];
	D3DXVECTOR3		m_vScl;				// 拡縮情報
	int				m_nMax;

	// クォータニオン用
	D3DXVECTOR3		m_vAxis;
	D3DXVECTOR3		m_vRadian;

	int				m_nCnt;
	int				m_nCntMax;
	int				m_nCntRot;

	float			m_fAngle;
	float			m_fLength;
	float			m_fRot;
	float			m_fRotSpeed;

	bool			m_bUse;
	void	Shot(void);
	void	Stay(void);
	void	StayAll(void);
	void	RetrunAll(void);
	void	Add(void);
	void	Sub(void);
	void	Rot(void);
};

	//Sword	m_cSword;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

#endif
