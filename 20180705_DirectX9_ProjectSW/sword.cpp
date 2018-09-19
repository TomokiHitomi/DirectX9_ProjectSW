//=============================================================================
//
// ソード処理 [sword.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "main.h"
#include "sword.h"
#include "calculate.h"
#include "input.h"
#include "camera.h"
#include "player.h"
#include "cube.h"
#include "collision.h"
#include "joycon.h"

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
Sword::Sword(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ソードの初期化
	m_CXModel = new CXModel;
	m_CXModel->Init(pDevice, SWORD_MODEL, SWORD_MODEL_TEX);

	m_eMode = Sword::SHOT;
	m_nCnt = 0;
	m_nCntRot = 0;
	m_fRot = 0.01f;
	m_fRotSpeed = SWORD_ROT_SPEED_MAX;

	for (unsigned int i = 0; i < SWORD_MAX; i++)
	{
		m_tagSword[i].vPos = D3DXVECTOR3(0.0f + i*100.0f, 0.0f + i*100.0f, 500.0f);
		m_tagSword[i].vX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		m_tagSword[i].vY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		m_tagSword[i].vZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		m_tagSword[i].eMode = SwordData::STAY;
		m_tagSword[i].fLength = SWORD_ROT_LENGTH;
		m_tagSword[i].fSpeed = 0.0f;
		m_tagSword[i].fAngle = 0.0f;
		if (i < SWORD_MAX_INIT)
		{
			m_tagSword[i].bUse = true;
			m_nCnt++;
		}
		else
		{
			m_tagSword[i].bUse = false;
		}
	}

	m_nCntMax = m_nCnt;

	m_vScl = D3DXVECTOR3(SWORD_SCL, SWORD_SCL, SWORD_SCL);


}

//=============================================================================
// デストラクタ（終了処理）
//=============================================================================
Sword::~Sword(void)
{
	if (m_CXModel != NULL)
	{
		m_CXModel->Release();
		delete m_CXModel;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void Sword::Update(void)
{
#ifdef _DEBUG
	PrintDebugProc("【 SWORD 】\nCnt[%d]\n", m_nCnt);
#endif

	if (m_bUse)
	{
		// ソードの更新処理
		D3DXVECTOR3 temp,posTemp, posTarget,vUp;
		D3DXMATRIX mtxTemp;


		// テスト用：プレイヤーのポインタを取得して、上半身２のマトリクスを取得
		Player *pPlayer = ObjectManager::GetObjectPointer<Player>(ObjectManager::PLAYER);
		mtxTemp = pPlayer->m_CSkinMesh[Player::CHARACTER]->GetBoneMatrix(PLAYER_MODEL_BONE_WING);
		// テスト用：キューブのポインタを取得
		Cube *pCube = ObjectManager::GetObjectPointer<Cube>(ObjectManager::CUBE);

		//posTemp = pPlayer->GetPos();
		posTemp = D3DXVECTOR3(mtxTemp._41, mtxTemp._42, mtxTemp._43);
		posTarget = pCube->GetPos();

		if (IsMobUseLeftTriggered() || JcTriggered(0, JC_R_BUTTON_ZR))
		{
			//m_eMode = Sword::SHOT;
			Shot();
		}

		if (IsMobUseRightTriggered() || JcTriggered(0, JC_R_BUTTON_R))
		{
			//StayAll();
			RetrunAll();
		}

		// テスト用ソード追加
		if (GetKeyboardTrigger(DIK_E))
		{
			Add();
		}
		else if (GetKeyboardTrigger(DIK_Q))
		{
			Sub();
		}

		//else
		//{
		//	//m_eMode = Sword::ROT;
		//	Stay();
		//}
		
		// ソード回転処理
		Rot();

		int nCnt = 0;
		m_fAngle = D3DX_PI * 2 / m_nCnt;
		pPlayer->GetVecZ(&m_vAxis);
		pPlayer->GetVecX(&m_vRadian);
		CrossProduct(&vUp, &m_vAxis, &m_vRadian);
		m_fLength = SWORD_ROT_LENGTH;

		for (unsigned int i = 0; i < m_nCntMax; i++)
		{
			
			switch (m_tagSword[i].eMode)
			{
			case SwordData::RETURN:

				m_tagSword[i].fAngle += ((m_fAngle * nCnt + m_fRot) - m_tagSword[i].fAngle) / SWORD_ROT_ADJUST_TIME;
				QuaternionCalculate(&m_vRadian, &m_vAxis, m_tagSword[i].fAngle, &temp);
				if (CheckHitBC(m_tagSword[i].vPos, posTemp + temp * m_tagSword[i].fLength, 100, 100))
				{
					m_tagSword[i].eMode = SwordData::STAY;
					m_tagSword[i].fSpeed = 0.0f;
					//break;
				}
				else
				{
					m_tagSword[i].fSpeed += (SWORD_SPEED_RETRUN - m_tagSword[i].fSpeed) / SWORD_SPEED_ADD;
					temp = (posTemp + temp * m_tagSword[i].fLength) - m_tagSword[i].vPos;
					D3DXVec3Normalize(&temp, &temp);
					m_tagSword[i].vPos += temp * m_tagSword[i].fSpeed;
				}
				m_tagSword[i].vX = vUp;
				m_tagSword[i].vZ = temp;
				nCnt++;
				break;
			case SwordData::STAY:
				m_tagSword[i].fAngle += ((m_fAngle * nCnt + m_fRot) - m_tagSword[i].fAngle) / SWORD_ROT_ADJUST_TIME;
				QuaternionCalculate(&m_vRadian, &m_vAxis, m_tagSword[i].fAngle, &temp);
				m_tagSword[i].vX = vUp;
				m_tagSword[i].vZ = temp;
				m_tagSword[i].vPos = posTemp + temp * m_tagSword[i].fLength;
				m_tagSword[i].fLength =
					max(SWORD_ROT_LENGTH, m_tagSword[i].fLength - SWORD_LENGTH_RETRUN_SUB);
				nCnt++;
				break;
			case SwordData::SHOT:
				m_tagSword[i].fSpeed += (SWORD_SPEED_MAX - m_tagSword[i].fSpeed) / SWORD_SPEED_ADD;

				temp = posTarget - m_tagSword[i].vPos;
				D3DXVec3Normalize(&temp, &temp);
				temp = temp * SWORD_TRUN + m_tagSword[i].vZ;
				D3DXVec3Normalize(&temp, &temp);
				m_tagSword[i].vPos += temp * m_tagSword[i].fSpeed;
				m_tagSword[i].vZ = temp;
				break;
			}

#ifdef _DEBUG
			PrintDebugProc("No[%d] Use[%d] Mode[%d] Len[%f] Spd[%f]\n"
				,i, m_tagSword[i].bUse, m_tagSword[i].eMode,
				m_tagSword[i].fLength, m_tagSword[i].fSpeed);
#endif
		}


#ifdef _DEBUG
		//PrintDebugProc("Pos [%f,%f,%f]\n", m_vPos.x, m_vPos.y, m_vPos.z);
#endif
	}

#ifdef _DEBUG
	PrintDebugProc("\n");
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void Sword::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (m_bUse)
	{
		// アニメーション更新処理
		D3DXMATRIX mtxWorld;

		// ソードを描画
		for (unsigned int i = 0; i < m_nCntMax; i++)
		{
			WorldConvertAxis(&mtxWorld,
				m_tagSword[i].vPos,
				-m_tagSword[i].vZ,
				m_tagSword[i].vX,
				m_vScl);

			m_CXModel->Draw(mtxWorld);
		}
	}
}

//=============================================================================
// 射出処理
//=============================================================================
void Sword::Shot(void)
{
	int nCntMax = m_nCntMax / 2;
	//if (m_nCntMax & 1)
	//{
	//	nCntMax++;
	//}
	for (unsigned int i = 0; i < nCntMax; i++)
	{
		if (m_tagSword[i].eMode != SwordData::SHOT)
		{
			m_tagSword[i].eMode = SwordData::SHOT;
			m_tagSword[i].fLength = SWORD_LENGTH_RETRUN;
			m_nCnt--;
			m_eMode = Sword::SHOT;
			return;
		}
		else if(m_tagSword[i+ nCntMax].eMode != SwordData::SHOT)
		{
			m_tagSword[i + nCntMax].eMode = SwordData::SHOT;
			m_tagSword[i + nCntMax].fLength = SWORD_LENGTH_RETRUN;
			m_nCnt--;
			m_eMode = Sword::SHOT;
			return;
		}
	}
}

//=============================================================================
// 格納処理
//=============================================================================
void Sword::Stay(void)
{
	for (unsigned int i = 0; i < m_nCntMax; i++)
	{
		if (m_tagSword[i].eMode == SwordData::SHOT)
		{
			m_tagSword[i].eMode = SwordData::STAY;
			return;
		}
	}
}

//=============================================================================
// 格納処理
//=============================================================================
void Sword::StayAll(void)
{
	for (unsigned int i = 0; i < m_nCntMax; i++)
	{
		if (m_tagSword[i].eMode != SwordData::STAY)
		{
			m_tagSword[i].eMode = SwordData::STAY;
			m_nCnt++;
		}
	}
}

//=============================================================================
// 格納処理
//=============================================================================
void Sword::RetrunAll(void)
{
	for (unsigned int i = 0; i < m_nCntMax; i++)
	{
		if (m_tagSword[i].eMode == SwordData::SHOT)
		{
			m_tagSword[i].eMode = SwordData::RETURN;
			m_nCnt++;
		}
	}
	m_eMode = Sword::RETURN;
	m_nCntRot = SWORD_ROT_SPEED_COUNT;
}

//=============================================================================
// 格納処理
//=============================================================================
void Sword::Add(void)
{
	for (unsigned int i = 0; i < SWORD_MAX; i++)
	{
		if (!m_tagSword[i].bUse)
		{
			m_tagSword[i].bUse = true;
			m_nCnt++;
			m_nCntMax++;
			return;
		}
	}
}

//=============================================================================
// 格納処理
//=============================================================================
void Sword::Sub(void)
{
	for (unsigned int i = SWORD_MAX - 1; i >= 0; i--)
	{
		if (m_tagSword[i].bUse)
		{
			m_tagSword[i].bUse = false;
			if (m_tagSword[i].eMode != SwordData::SHOT)
			{
				m_nCnt--;
			}
			m_nCntMax--;
			return;
		}
	}
}

//=============================================================================
// 格納処理
//=============================================================================
void Sword::Rot(void)
{
	//if (m_eMode == Sword::RETURN)
	//{
	//	//m_fRotSpeed += (SWORD_ROT_SPEED_MIN - m_fRotSpeed) / SWORD_ROT_SPEED_SUB;
	//	m_fRotSpeed = SWORD_ROT_SPEED_MIN;
	//	m_nCntRot--;
	//	if (m_nCntRot < 0)
	//	{
	//		m_eMode = Sword::ROT;
	//		m_nCntRot = 0;
	//	}
	//}
	//else
	//{
	//	m_fRotSpeed += (SWORD_ROT_SPEED_MAX - m_fRotSpeed) / SWORD_ROT_SPEED_ADD;
	//}
	m_fRot += m_fRotSpeed;
}