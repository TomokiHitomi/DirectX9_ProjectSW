//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "calculate.h"
#include "input.h"
#include "camera.h"
#include "cube.h"

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
Player::Player(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// オブジェクトIDとプライオリティの設定処理
	SetIdAndPriority(ObjectID::PLAYER, Priority::Middle, Priority::Middle);

	// 各プロパティの初期化
	m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vScl = D3DXVECTOR3(PLAYER_SCL, PLAYER_SCL, PLAYER_SCL);

	m_vMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vRotIner = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_bUse = true;

	// モデルの初期化
	//m_CSkinMesh->Init(pDevice, "data/MODEL/Yuko.x");
	m_CSkinMesh = new CSkinMesh;
	m_CSkinMesh->Init(pDevice, PLAYER_MODEL);

}

//=============================================================================
// デストラクタ（終了処理）
//=============================================================================
Player::~Player(void)
{
	if (m_CSkinMesh != NULL)
	{
		m_CSkinMesh->Release();
		delete m_CSkinMesh;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void Player::Update(void)
{
#ifdef _DEBUG
	PrintDebugProc("【 MODEL 】\n");
#endif

	if (m_bUse)
	{
		// 移動処理
		Move();

		// ワールド変換
		WorldConvert(&m_mtxWorld, m_vPos, m_vRot, m_vScl);
		//WorldConvertPR(&m_mtxWorld, m_vPos, m_vRot, m_vScl);

		// アニメーション更新処理
		m_CSkinMesh->Update(m_mtxWorld);
#ifdef _DEBUG
		PrintDebugProc("Pos [%f,%f,%f]\n", m_vPos.x, m_vPos.y, m_vPos.z);
		PrintDebugProc("Rot [%f,%f,%f]\n", m_vRot.x, m_vRot.y, m_vRot.z);
		PrintDebugProc("Move[%f,%f,%f]\n", m_vMove.x, m_vMove.y, m_vMove.z);
#endif
	}

#ifdef _DEBUG
	PrintDebugProc("\n");
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void Player::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ライティングを強めにあてる
	//SetLight(LIGHT_SUB1, TRUE);
	//SetLight(LIGHT_SUB2, TRUE);

	if (m_bUse)
	{
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_CSkinMesh->Draw(pDevice);
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}

	// ライティングを通常に戻す
	//SetLight(LIGHT_SUB1, FALSE);
	//SetLight(LIGHT_SUB2, FALSE);
}

//=============================================================================
// カメラ移動関数
//=============================================================================
void Player::Move(void)
{
	//// 移動処理
	//if (InputPress(INPUT_LEFT))
	//{
	//	if (InputPress(INPUT_UP))
	//	{// 左前移動
	//		MoveFunc(m_vRot.y + D3DX_PI * 0.75f);
	//	}
	//	else if (InputPress(INPUT_DOWN))
	//	{// 左後移動
	//		MoveFunc(m_vRot.y + D3DX_PI * 0.25f);
	//	}
	//	else if (InputPress(INPUT_RIGHT))
	//	{// 左右同時押しは処理なし
	//	}
	//	else
	//	{// 左移動
	//		MoveFunc(m_vRot.y + D3DX_PI * 0.50f);
	//	}
	//}
	//else if (InputPress(INPUT_RIGHT))
	//{
	//	if (InputPress(INPUT_UP))
	//	{// 右前移動
	//		MoveFunc(m_vRot.y - D3DX_PI * 0.75f);
	//	}
	//	else if (InputPress(INPUT_DOWN))
	//	{// 右後移動
	//		MoveFunc(m_vRot.y - D3DX_PI * 0.25f);
	//	}
	//	else
	//	{// 右移動
	//		MoveFunc(m_vRot.y - D3DX_PI * 0.50f);
	//	}
	//}
	//else if (InputPress(INPUT_UP))
	//{// 前移動
	//	if (InputPress(INPUT_DOWN))
	//	{// 前後同時押しは処理なし
	//	}
	//	else
	//	{
	//		MoveFunc(m_vRot.y + D3DX_PI);
	//	}
	//}
	//else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN))
	//{// 後移動
	//	MoveFunc(m_vRot.y);
	//}


	if (InputPress(INPUT_UP) && InputPress(INPUT_DOWN))
	{
		// 同時押しは処理なし
	}
	else if (InputPress(INPUT_UP))
	{
		m_vRotIner.x += PLAYER_ROT_SPEED;
	}
	else if (InputPress(INPUT_DOWN))
	{
		m_vRotIner.x -= PLAYER_ROT_SPEED;
	}

	if (InputPress(INPUT_LEFT) && InputPress(INPUT_RIGHT))
	{
		// 同時押しは処理なし
	}
	else if (InputPress(INPUT_LEFT))
	{
		m_vRotIner.z -= PLAYER_ROT_SPEED;
	}
	else if (InputPress(INPUT_RIGHT))
	{
		m_vRotIner.z += PLAYER_ROT_SPEED;
	}


	//// 回転量をロットに適用
	//m_vRot += m_vRotIner;

	//// 回転慣性を適用
	//m_vRotIner -= m_vRotIner * PLAYER_ROT_INERTIA;

	// カメラをモデルにセット
	D3DXVECTOR3 vTemp = m_vPos;
	vTemp.y += PLAYER_HEIGHT;
	SetCameraAt(vTemp);

	//Cube::SetPos(m_vRot);

	//// 回転半径を設定
	//D3DXVECTOR3 vTa = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//D3DXVECTOR3 vZero = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//D3DXVECTOR3 vAxis;

	//// 現在の角度に垂直な回転軸ベクトルを設定
	//vAxis = D3DXVECTOR3(cosf(m_vRot.z), 0, sinf(m_vRot.z));

	//// クォータニオン処理
	//QuaternionCalculate(&vTa, &vAxis, D3DX_PI * 1.5f + m_vRot.x,
	//	&vZero);

	//// 回転軸に設置
	//Cube::SetPos((m_vPos + vZero) * 100.0f);
	//m_vPos += vZero;

	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	/******************** ワールド変換 ********************/
	// ワールドマトリクスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 【T】平行移動を反映(オブジェクトを配置している）
	D3DXMatrixTranslation(&mtxTranslate, 0.0f, 0.0f, -1.0f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

		// 【R】回転を反映(YawPitchRollはy,x,z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, m_vRotIner.x, 0.0f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_vRotIner.y, 0.0f, m_vRotIner.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);




	//D3DXVECTOR3 vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//D3DXVECTOR3 vScl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	//WorldConvertPR(&m_mtxWorld, vPos, m_vRot, vScl);

	m_vPos += D3DXVECTOR3(
		m_mtxWorld._41,
		m_mtxWorld._42,
		m_mtxWorld._43);

	m_vRot.x = atan2(m_mtxWorld._42, m_mtxWorld._43);

	//m_vRot.y = 2 * (D3DX_PI - atan2(m_mtxWorld._41, m_mtxWorld._43));

	//m_vRot.z = 2 * (D3DX_PI - atan2(m_mtxWorld._41, m_mtxWorld._42));
}

//=============================================================================
// 移動処理関数
//=============================================================================
void Player::MoveFunc(float fAngle)
{
	m_vPos.x += sinf(fAngle) * PLAYER_MOVE_SPEED;
	m_vPos.z += cosf(fAngle) * PLAYER_MOVE_SPEED;
}

//=============================================================================
// 回転処理関数
//=============================================================================
void Player::RotFunc(D3DXVECTOR3 vRot)
{
	//m_vPos.x += sinf(fAngle) * PLAYER_MOVE_SPEED;
	//m_vPos.z += cosf(fAngle) * PLAYER_MOVE_SPEED;
}