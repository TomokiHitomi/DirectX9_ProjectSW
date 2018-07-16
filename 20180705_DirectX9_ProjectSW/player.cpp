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

	m_vX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_vY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	m_eMode = MODE_FLY;
	m_eModeOld = MODE_MAX;	// 1フレーム目はモードチェンジに入りたい

	m_fMoveSpeed = PLAYER_MOVE_SPEED_MIN;
	m_bUse = true;

	// モデルの初期化
	//m_CSkinMesh->Init(pDevice, "data/MODEL/Yuko.x");
	m_CSkinMesh = new CSkinMesh;
	m_CSkinMesh->Init(pDevice, PLAYER_MODEL);
	
	D3DXMATRIX mtxTranslate;

	D3DXMatrixIdentity(&m_mtxWorld);



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

		switch (m_eMode)
		{
		case MODE_FLOAT:
			Float();	// 浮遊処理
			break;
		case MODE_FLY:
			Fly();		// 飛行処理
			break;
		}

		// モードチェンジ
		ModeChange();

		// ワールド変換
		WorldConvertAxis(&m_mtxWorld, m_vPos, m_vZ, m_vY, m_vScl);



		// アニメーション更新処理
		m_CSkinMesh->Update(m_mtxWorld);
#ifdef _DEBUG
		PrintDebugProc("Pos [%f,%f,%f]\n", m_vPos.x, m_vPos.y, m_vPos.z);
		PrintDebugProc("Rot [%f,%f,%f]\n", m_vRot.x, m_vRot.y, m_vRot.z);
		PrintDebugProc("RotI[%f,%f,%f]\n", m_vRotIner.x, m_vRotIner.y, m_vRotIner.z);
		PrintDebugProc("Move[%f,%f,%f]\n", m_vMove.x, m_vMove.y, m_vMove.z);
		PrintDebugProc("Spd [%f]\n", m_fMoveSpeed);
		PrintDebugProc("vX  [%f,%f,%f]\n",
			m_vX.x, m_vX.y, m_vX.z);
		PrintDebugProc("vY  [%f,%f,%f]\n",
			m_vY.x, m_vY.y, m_vY.z);
		PrintDebugProc("vZ  [%f,%f,%f]\n",
			m_vZ.x, m_vZ.y, m_vZ.z);
		
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
		// αテストを有効に
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, PLAYER_ALPHA_TEST);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		// 両面描画する
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		// モデルを描画
		m_CSkinMesh->Draw(pDevice);

		// 裏面をカリングに戻す
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		// αテストを無効に
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	}

	// ライティングを通常に戻す
	//SetLight(LIGHT_SUB1, FALSE);
	//SetLight(LIGHT_SUB2, FALSE);
}


//=============================================================================
// 浮遊処理
//=============================================================================
void Player::Float(void)
{
	m_vY = m_vY + (D3DXVECTOR3(0.0f, 1.0f, 0.0f)- m_vY) * 0.1f;

	m_vRot.y = atan2(m_vZ.z, m_vZ.x);
	//m_vZ = m_vZ + (D3DXVECTOR3(cosf(m_fHAngle), 0.0f, sinf(m_fHAngle)) - m_vZ) * 0.1f;


	//if (InputPress(INPUT_UP_R) && InputPress(INPUT_DOWN_R))
	//{
	//}
	//else if (InputPress(INPUT_UP_R))
	//{
	//	m_vRotIner.x += 0.05f;
	//}
	//else if (InputPress(INPUT_DOWN_R))
	//{
	//	m_vRotIner.x -= 0.05f;
	//}

	//if (InputPress(INPUT_LEFT_R) && InputPress(INPUT_RIGHT_R))
	//{
	//}
	//else if (InputPress(INPUT_LEFT_R))
	//{
	//	m_vRotIner.y += 0.05f;
	//}
	//else if (InputPress(INPUT_RIGHT_R))
	//{
	//	m_vRotIner.y -= 0.05f;
	//}

	//m_vRot.x = m_vRot.x + ((m_vRotIner.x - m_vRot.x) * 0.05f);
	//m_vRot.y = m_vRot.y + ((m_vRotIner.y - m_vRot.y) * 0.5f);

	// ピッチ
	if (InputPress(INPUT_UP_R) && InputPress(INPUT_DOWN_R))
	{	// 同時押しは慣性を止める
		if (m_vRot.x > 0.0f)
			m_vRot.x = max(m_vRot.x - PLAYER_ROT_SPEED_X, 0.0f);
		else if (m_vRot.x < 0.0f)
			m_vRot.x = min(m_vRot.x + PLAYER_ROT_SPEED_X, 0.0f);
	}
	else if (InputPress(INPUT_UP_R))
	{	// ピッチ角度を慣性で加算
		m_vRot.x = min(m_vRot.x + PLAYER_ROT_SPEED_X, PLAYER_ROT_SPEED_MAX_X);
	}
	else if (InputPress(INPUT_DOWN_R))
	{	// ピッチ角度を慣性で減算
		m_vRot.x = max(m_vRot.x - PLAYER_ROT_SPEED_X, -PLAYER_ROT_SPEED_MAX_X);
	}
	else
	{	// 入力なしは慣性を止める
		if (m_vRot.x > 0.0f)
			m_vRot.x = max(m_vRot.x - PLAYER_ROT_SPEED_X, 0.0f);
		else if (m_vRot.x < 0.0f)
			m_vRot.x = min(m_vRot.x + PLAYER_ROT_SPEED_X, 0.0f);
	}


	//m_vZ = m_vZ + (D3DXVECTOR3(cosf(m_vRot.y), 0.0f, sinf(m_vRot.y)) - m_vZ) * 0.1f;
	CrossProduct(&m_vX, &m_vY, &m_vZ);



	D3DXVECTOR3 vEye = m_vZ;

	//m_vZ *= 100;

	// ロール回転を計算し、ピッチ回転用の軸を求める
	QuaternionCalculate(&m_vZ, &m_vX, m_vRot.x, &m_vZ);
	//D3DXVec3Normalize(&m_vZ, &m_vZ);

	//m_vZ = vEye;

	// 移動処理
	if (InputPress(INPUT_LEFT))
	{
		if (InputPress(INPUT_UP))
		{// 左前移動
			//MoveFunc(m_fHAngle + D3DX_PI * 0.25f);
		}
		else if (InputPress(INPUT_DOWN))
		{// 左後移動
			//MoveFunc(m_fHAngle + D3DX_PI * 0.75f);
		}
		else if (InputPress(INPUT_RIGHT))
		{// 左右同時押しは処理なし
		}
		else
		{// 左移動
			//MoveFunc(m_fHAngle + D3DX_PI * 0.50f);
		}
	}
	else if (InputPress(INPUT_RIGHT))
	{
		if (InputPress(INPUT_UP))
		{// 右前移動
			//MoveFunc(m_fHAngle - D3DX_PI * 0.25f);
		}
		else if (InputPress(INPUT_DOWN))
		{// 右後移動
			//MoveFunc(m_fHAngle - D3DX_PI * 0.75f);
		}
		else
		{// 右移動
			//MoveFunc(m_fHAngle - D3DX_PI * 0.50f);
		}
	}
	else if (InputPress(INPUT_UP))
	{// 前移動
		if (InputPress(INPUT_DOWN))
		{// 前後同時押しは処理なし
		}
		else
		{
			m_vPos += -m_vZ * m_fMoveSpeed;
		}
	}
	else if (InputPress(INPUT_DOWN))
	{// 後移動
		m_vPos += m_vZ * m_fMoveSpeed;
	}


	// 移動を適用
	//m_vPos += m_vMove;


	// カメラをAtをモデルに設定
	Camera::SetAt(m_vPos + (m_vY * 30));

	// カメラEyeをモデル後方にセット
	Camera::SetEye(m_vPos + (m_vY * 30) + m_vZ * 100);

	// カメラ慣性を減らす
	Camera::AddEyeIner(0.02f);

	// カメラUpをモデル上部に設定
	Camera::SetUp(m_vY);

}

//=============================================================================
// 飛行処理
//=============================================================================
void Player::Fly(void)
{
	// ピッチ
	if (InputPress(INPUT_UP_R) && InputPress(INPUT_DOWN_R))
	{	// 同時押しは慣性を止める
		if (m_vRot.x > 0.0f)
			m_vRot.x = max(m_vRot.x - PLAYER_ROT_SPEED_X, 0.0f);
		else if (m_vRot.x < 0.0f)
			m_vRot.x = min(m_vRot.x + PLAYER_ROT_SPEED_X, 0.0f);
	}
	else if (InputPress(INPUT_UP_R))
	{	// ピッチ角度を慣性で加算
		m_vRot.x = min(m_vRot.x + PLAYER_ROT_SPEED_X, PLAYER_ROT_SPEED_MAX_X);
	}
	else if (InputPress(INPUT_DOWN_R))
	{	// ピッチ角度を慣性で減算
		m_vRot.x = max(m_vRot.x - PLAYER_ROT_SPEED_X, -PLAYER_ROT_SPEED_MAX_X);
	}
	else
	{	// 入力なしは慣性を止める
		if (m_vRot.x > 0.0f)
			m_vRot.x = max(m_vRot.x - PLAYER_ROT_SPEED_X, 0.0f);
		else if (m_vRot.x < 0.0f)
			m_vRot.x = min(m_vRot.x + PLAYER_ROT_SPEED_X, 0.0f);
	}

	// ロール
	if (InputPress(INPUT_LEFT_R) && InputPress(INPUT_RIGHT_R))
	{	// 同時押しは慣性を止める
		if (m_vRot.z > 0.0f)
			m_vRot.z = max(m_vRot.z - PLAYER_ROT_SPEED_Z, 0.0f);
		else if (m_vRot.z < 0.0f)
			m_vRot.z = min(m_vRot.z + PLAYER_ROT_SPEED_Z, 0.0f);
	}
	else if (InputPress(INPUT_RIGHT_R))
	{	// ロール角度を慣性で加算
		m_vRot.z = min(m_vRot.z + PLAYER_ROT_SPEED_Z, PLAYER_ROT_SPEED_MAX_Z);
	}
	else if (InputPress(INPUT_LEFT_R))
	{	// ロール角度を慣性で減算
		m_vRot.z = max(m_vRot.z - PLAYER_ROT_SPEED_Z, -PLAYER_ROT_SPEED_MAX_Z);
	}
	else
	{	// 入力なしは慣性を止める
		if (m_vRot.z > 0.0f)
			m_vRot.z = max(m_vRot.z - PLAYER_ROT_SPEED_Z, 0.0f);
		else if (m_vRot.z < 0.0f)
			m_vRot.z = min(m_vRot.z + PLAYER_ROT_SPEED_Z, 0.0f);
	}

	// ロール回転を計算し、ピッチ回転用の軸を求める
	QuaternionCalculate(&m_vX, &m_vZ, m_vRot.z, &m_vX);

	// ピッチ回転を計算し、ロール回転用の軸を求める
	QuaternionCalculate(&m_vZ, &m_vX, m_vRot.x, &m_vZ);

	// ローカルY軸のベクトルを外積で求める
	CrossProduct(&m_vY, &m_vZ, &m_vX);

	// 移動量を算出
	m_vMove = m_vZ * m_fMoveSpeed;

	// 移動量を適用
	m_vPos -= m_vMove;

	// カメラをAtをモデルに設定
	Camera::SetAt(m_vPos + (m_vY * 30));

	// カメラUpをモデル上部に設定
	//Camera::SetUp(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	Camera::SetUp(m_vY);

	// カメラEyeをモデル後方にセット
	Camera::SetEye(m_vPos + m_vZ * 100);
	//SetCameraEye(m_vPos - m_vZ * 100 + m_vY * 50);
}

//=============================================================================
// 移動処理
//=============================================================================
void Player::Move(void)
{
	if (InputPress(INPUT_UP) && InputPress(INPUT_DOWN))
	{
		//if (m_fMoveSpeed > 0.0f)
		//	m_fMoveSpeed = max(m_fMoveSpeed - PLAYER_MOVE_SPEED, 0.0f);
		//else if (m_fMoveSpeed < 0.0f)
		//	m_fMoveSpeed = min(m_fMoveSpeed + PLAYER_MOVE_SPEED, 0.0f);
	}
	else if (InputPress(INPUT_UP))
	{
		m_fMoveSpeed = min(m_fMoveSpeed + PLAYER_MOVE_SPEED, PLAYER_MOVE_SPEED_MAX);
	}
	else if (InputPress(INPUT_DOWN))
	{
		m_fMoveSpeed = max(m_fMoveSpeed - PLAYER_MOVE_SPEED, PLAYER_MOVE_SPEED_MIN);
	}
	else
	{
		//if (m_fMoveSpeed > 0.0f)
		//	m_fMoveSpeed = max(m_fMoveSpeed - PLAYER_MOVE_SPEED, 0.0f);
		//else if (m_fMoveSpeed < 0.0f)
		//	m_fMoveSpeed = min(m_fMoveSpeed + PLAYER_MOVE_SPEED, 0.0f);
	}
}

//=============================================================================
// モードチェンジ処理
//=============================================================================
void Player::ModeChange(void)
{
	if (GetKeyboardTrigger(DIK_1))
	{
		m_eMode = MODE_FLOAT;
	}
	if (GetKeyboardTrigger(DIK_2))
	{
		m_eMode = MODE_FLY;
	}
	if (GetKeyboardTrigger(DIK_3))
	{
		m_eMode = MODE_LOCKON;
	}
	if (m_eMode != m_eModeOld)
	{
		switch (m_eMode)
		{
		case MODE_FLOAT:
			//m_vRot = D3DXVECTOR3(PLAYER_FLOAT_ROT_X,0.0f,0.0f);
			m_vRot = ZERO_D3DXVECTOR3;
			m_vRotIner = m_vRot;
			Camera::SetEyeIner(0.01f);
			break;
		case MODE_FLY:
			m_vRot = ZERO_D3DXVECTOR3;
			Camera::SetEyeIner(0.05f);
			break;
		case MODE_LOCKON:
			break;
		}
		m_eModeOld = m_eMode;
	}
}

//=============================================================================
// 移動処理関数
//=============================================================================
void Player::MoveFunc(float fAngle)
{
	m_vPos.x += cosf(fAngle) * m_fMoveSpeed;
	m_vPos.z += sinf(fAngle) * m_fMoveSpeed;
}

//=============================================================================
// 回転処理関数
//=============================================================================
void Player::RotFunc(D3DXVECTOR3 vRot)
{
	//m_vPos.x += sinf(fAngle) * PLAYER_MOVE_SPEED;
	//m_vPos.z += cosf(fAngle) * PLAYER_MOVE_SPEED;
}