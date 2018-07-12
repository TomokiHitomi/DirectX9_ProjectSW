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
		//WorldConvert(&m_mtxWorld, m_vPos, m_vRot, m_vScl);
		//WorldConvertPR(&m_mtxWorld, m_vPos, m_vRot, m_vScl);

		// アニメーション更新処理
		m_CSkinMesh->Update(m_mtxWorld);
#ifdef _DEBUG
		PrintDebugProc("Pos [%f,%f,%f]\n", m_vPos.x, m_vPos.y, m_vPos.z);
		PrintDebugProc("Rot [%f,%f,%f]\n", m_vRot.x, m_vRot.y, m_vRot.z);
		PrintDebugProc("RotI[%f,%f,%f]\n", m_vRotIner.x, m_vRotIner.y, m_vRotIner.z);
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

	// やるぜぇぇぇええ

	//D3DXVECTOR3 vTa = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 vAxis = D3DXVECTOR3(cosf(m_vRotIner.z), sinf(m_vRotIner.z), 0.0f);
	//D3DXVECTOR3 vTa = D3DXVECTOR3(-sinf(m_vRotIner.z), cosf(m_vRotIner.z), 0.0f);

	D3DXVECTOR3 vTa = D3DXVECTOR3(cosf(D3DX_PI * 0.5f + m_vRotIner.z), sinf(D3DX_PI * 0.5f + m_vRotIner.z), 0.0f);

	////void QuaternionCalculate(D3DXVECTOR3 *pOrigin, D3DXVECTOR3 *pAxis, float fAngle, D3DXVECTOR3 *pPos)
	////{
	// 反数、処理結果用の変数を定義
	D3DXQUATERNION qReversal, qAnswer, qAnswer2;
	// 回転させたい点の初期位置
	D3DXQUATERNION qOrigin(vTa.x, vTa.y, vTa.z, 0.0f);
	// 単位クォータニオン
	D3DXQUATERNION qQuaternion(0.0f, 0.0f, 0.0f, 1.0f);

	// 軸pAxis回りの回転クォータニオンを生成
	D3DXQuaternionRotationAxis(&qQuaternion, &vAxis, m_vRotIner.x);
	// XYZマイナスの共役を算出
	D3DXQuaternionConjugate(&qReversal, &qQuaternion);
	// qReversal・qOrigin・qQuaternionを行列計算
	D3DXQuaternionMultiply(&qAnswer, &qReversal, &qOrigin);
	// qAnswerに回転処理後の結果を格納
	D3DXQuaternionMultiply(&qAnswer, &qAnswer, &qQuaternion);

	D3DXVECTOR3 vAxis2 = D3DXVECTOR3(qAnswer.x, qAnswer.y, qAnswer.z);
	D3DXVECTOR3 vTa2 = D3DXVECTOR3(-sinf(m_vRotIner.z), qAnswer.x, 0.0f);

	// 回転させたい点の初期位置
	D3DXQUATERNION qOrigin2(vTa2.x, vTa2.y, vTa2.z, 0.0f);

	// 軸pAxis回りの回転クォータニオンを生成
	D3DXQuaternionRotationAxis(&qQuaternion, &vAxis2, D3DX_PI *0.5f + m_vRotIner.x);
	// XYZマイナスの共役を算出
	D3DXQuaternionConjugate(&qReversal, &qQuaternion);
	// qReversal・qOrigin・qQuaternionを行列計算
	D3DXQuaternionMultiply(&qAnswer2, &qReversal, &qOrigin2);
	// qAnswerに回転処理後の結果を格納
	D3DXQuaternionMultiply(&qAnswer2, &qAnswer2, &qQuaternion);

	 //回転処理の結果を反映
	m_vPos.x += qAnswer.x;
	m_vPos.y += qAnswer.y;
	m_vPos.z += qAnswer.z;

	D3DXVECTOR3 vLook = m_vPos +
		D3DXVECTOR3(qAnswer.x, qAnswer.y, qAnswer.z) * 50.0f;

	Cube::SetPos(m_vPos + 
		D3DXVECTOR3(qAnswer.x, qAnswer.y, qAnswer.z) * 50.0f);





#ifdef _DEBUG
	PrintDebugProc("Axis [%f,%f,%f]\n", vAxis.x, vAxis.y, vAxis.z);
	PrintDebugProc("vTa [%f,%f,%f]\n", vTa.x, vTa.y, vTa.z);
	PrintDebugProc("vAns [%f,%f,%f,%f]\n", qAnswer.x, qAnswer.y, qAnswer.z, qAnswer.w);
#endif

	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;




	m_qText = qAnswer;



	/******************** ワールド変換 ********************/
	// ワールドマトリクスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 【S】スケールを反映(Multiplyは行列計算)
	D3DXMatrixScaling(&mtxScl, m_vScl.x, m_vScl.y, m_vScl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	//// 【R】回転を反映(YawPitchRollはy,x,z)

	//mtxRot._11 = 1.0f - 2.0f * qAnswer.y * qAnswer.y - 2.0f * qAnswer.z * qAnswer.z;
	//mtxRot._12 = 2.0f * qAnswer.x * qAnswer.y + 2.0f * qAnswer.w * qAnswer.z;
	//mtxRot._13 = 2.0f * qAnswer.x * qAnswer.z - 2.0f * qAnswer.w * qAnswer.y;
	//mtxRot._14 = 0.0f;

	//mtxRot._21 = 2.0f * qAnswer.x * qAnswer.y - 2.0f * qAnswer.w * qAnswer.z;
	//mtxRot._22 = 1.0f - 2.0f * qAnswer.x * qAnswer.x - 2.0f * qAnswer.z * qAnswer.z;
	//mtxRot._23 = 2.0f * qAnswer.y * qAnswer.z + 2.0f * qAnswer.w * qAnswer.x;
	//mtxRot._24 = 0.0f;

	//mtxRot._31 = 2.0f * qAnswer.x * qAnswer.z + 2.0f * qAnswer.w * qAnswer.y;
	//mtxRot._32 = 2.0f * qAnswer.y * qAnswer.z - 2.0f * qAnswer.w * qAnswer.x;
	//mtxRot._33 = 1.0f - 2.0f * qAnswer.x * qAnswer.x - 2.0f * qAnswer.y * qAnswer.y;
	//mtxRot._34 = 0.0f;

	//mtxRot._41 =
	//	mtxRot._42 =
	//	mtxRot._43 = 0.0f;
	//mtxRot._44 = 1.0f;

	//D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, m_vRotIner.z);
	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	D3DXVECTOR3 X, Y, Z, Up;
	//Up = D3DXVECTOR3(qAnswer2.x, qAnswer2.y, qAnswer2.z);
	Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	Z = vLook - m_vPos;
	D3DXVec3Normalize(&Z, &Z);
	D3DXVec3Normalize(&Up, &Up);
	D3DXVec3Cross(&X, D3DXVec3Normalize(&Y, &Up), &Z);
	D3DXVec3Normalize(&X, &X);
	D3DXVec3Normalize(&Y, D3DXVec3Cross(&Y, &Z, &X));

	mtxRot._11 = X.x;
	mtxRot._12 = X.y;
	mtxRot._13 = X.z;
	mtxRot._14 = 0.0f;

	mtxRot._21 = Y.x;
	mtxRot._22 = Y.y;
	mtxRot._23 = Y.z;
	mtxRot._24 = 0.0f;

	mtxRot._31 = -Z.x;
	mtxRot._32 = -Z.y;
	mtxRot._33 = -Z.z;
	mtxRot._34 = 0.0f;

	mtxRot._41 =
		mtxRot._42 =
		mtxRot._43 = 0.0f;
	mtxRot._44 = 1.0f;


	//D3DXMatrixRotationQuaternion(&mtxRot, &qAnswer);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);



	// 【T】平行移動を反映(オブジェクトを配置している）
	D3DXMatrixTranslation(&mtxTranslate, m_vPos.x, m_vPos.y, m_vPos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);


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

//	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
//
//	/******************** ワールド変換 ********************/
//	// ワールドマトリクスの初期化
//	D3DXMatrixIdentity(&m_mtxWorld);
//
//	// 【T】平行移動を反映(オブジェクトを配置している）
//	D3DXMatrixTranslation(&mtxTranslate, 0.0f, 0.0f, 1.0f);
//	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);
//
//	// 【R】回転を反映(YawPitchRollはy,x,z)
//
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, m_vRotIner.x, 0.0f);
//	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
//
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, m_vRotIner.z);
//	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
//
//
//
//
//	//D3DXVECTOR3 vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	//D3DXVECTOR3 vScl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
//
//	//WorldConvertPR(&m_mtxWorld, vPos, m_vRot, vScl);
//
//	m_vPos += D3DXVECTOR3(
//		m_mtxWorld._41,
//		m_mtxWorld._42,
//		m_mtxWorld._43);
//
//#ifdef _DEBUG
//	PrintDebugProc("mtx [%f,%f,%f]\n", m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
//#endif
//
//	//m_vRot.x = m_vRotIner.x;
//	//m_vRot.z = m_vRotIner.z;
//
//
//	m_vRot.x = -atan2(m_mtxWorld._42,m_mtxWorld._43);
//
//	//m_vRot.y = atan2(m_mtxWorld._41, m_mtxWorld._43);
//
//	m_vRot.z = -atan2(m_mtxWorld._41, m_mtxWorld._42);
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