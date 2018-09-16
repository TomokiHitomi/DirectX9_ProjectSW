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
#include "sound.h"

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
extern float fGyroX;
extern float fGyroY;
extern float fGyroZ;

//=============================================================================
// コンストラクタ（初期化処理）
//=============================================================================
Player::Player(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// オブジェクトIDとプライオリティの設定処理
	SetIdAndPriority(ObjectID::PLAYER, Priority::Middle, Priority::Middle);

	// 各プロパティの初期化
	m_vPos = PLAYER_POS;
	m_vRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vScl = D3DXVECTOR3(PLAYER_SCL, PLAYER_SCL, PLAYER_SCL);

	m_vMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vRotIner = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_vX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_vY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	m_eMode = MODE_FLOAT;
	m_eModeOld = MODE_MAX;	// 1フレーム目はモードチェンジに入りたい

	m_fMoveSpeed = PLAYER_MOVE_SPEED_MIN;
	m_fRiseSpeed = 0.0f;
	m_bUse = true;

	// モデルの初期化
	m_CSkinMesh[CHARACTER] = new CSkinMesh;
	m_CSkinMesh[CHARACTER]->Init(pDevice, PLAYER_MODEL);

	// ウィングの初期化
	m_CSkinMesh[WING] = new CSkinMesh;
	m_CSkinMesh[WING]->Init(pDevice, PLAYER_MODEL_WING);

	// ウィングのローカル情報を作成
	D3DXVECTOR3 pos, rot, scl;
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot = D3DXVECTOR3(PLAYER_WING_ROT_X, 0.0f, 0.0f);
	scl = D3DXVECTOR3(PLAYER_WING_SCL, PLAYER_WING_SCL, PLAYER_WING_SCL);

	// ウィング行列を作成
	WorldConvert(&m_mtxWorldWing, pos, rot, scl);

	// ソードの初期化
	m_cSword = new Sword;

	D3DXMATRIX mtxTranslate;

	D3DXMatrixIdentity(&m_mtxWorld);

	// カメラをAtをモデルに設定
	Camera::SetAt(m_vPos + (m_vY * 30));

	// カメラUpをモデル上部に設定
	//Camera::SetUp(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	Camera::SetUp(m_vY);

	// カメラEyeをモデル後方にセット
	Camera::SetEye(m_vPos + m_vZ * 100);
}

//=============================================================================
// デストラクタ（終了処理）
//=============================================================================
Player::~Player(void)
{
	for (unsigned int i = 0; i < MODEL_ANIM_MAX; i++)
	{
		if (m_CSkinMesh[i] != NULL)
		{
			m_CSkinMesh[i]->Release();
			delete m_CSkinMesh[i];
		}
	}
	for (unsigned int i = 0; i < MODEL_NORMAL_MAX; i++)
	{
		if (m_cSword != NULL)
		{
			delete m_cSword;
		}
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
		case MODE_LOCKON:
			LOCKON();	// 照準処理
			break;
		case MODE_CHANGE:
			Change();	// 遷移処理
			break;
		}

		// モードチェンジ
		ModeChange();

		// ワールド変換
		WorldConvertAxis(&m_mtxWorld, m_vPos, m_vZ, m_vY, m_vScl);


		m_CSkinMesh[CHARACTER]->Update();
		m_CSkinMesh[WING]->Update();

		// ソードの更新処理
		if (m_cSword != NULL)
		{
			m_cSword->Update();
		}

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
		PrintDebugProc("mtxX[%f,%f,%f]\n",
			m_mtxWorld._11, m_mtxWorld._12, m_mtxWorld._13);
		PrintDebugProc("mtxY[%f,%f,%f]\n",
			m_mtxWorld._21, m_mtxWorld._22, m_mtxWorld._23);
		PrintDebugProc("mtxZ[%f,%f,%f]\n",
			m_mtxWorld._31, m_mtxWorld._32, m_mtxWorld._33);
		PrintDebugProc("mtxA[%f,%f,%f]\n",
			m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
		PrintDebugProc("atanTest [%f]\n",
			-atan2(m_vZ.y, 1.0f));
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
	if (m_bUse)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		D3DXMATRIX mtxWing, mtxTemp;		// アニメーション更新処理

		// ワールド変換
		WorldConvertAxis(&m_mtxWorld, m_vPos, m_vZ, m_vY, m_vScl);

		// ライティングを強めにあてる
		//SetLight(LIGHT_SUB1, TRUE);
		//SetLight(LIGHT_SUB2, TRUE);

		// αテストを有効に
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, PLAYER_ALPHA_TEST);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		// 両面描画する
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		// モデルを描画-
		m_CSkinMesh[CHARACTER]->Draw(pDevice, m_mtxWorld);

		// キャラクターのボーンマトリクスを取得
		mtxTemp = m_CSkinMesh[CHARACTER]->GetBoneMatrix(PLAYER_MODEL_BONE_WING);
		// ボーンマトリクスを正規化
		D3DXMatrixNormalize(&mtxTemp, &mtxTemp);
		
		// ウィング行列をコピー
		mtxWing = m_mtxWorldWing;

		// コピーしたウィング行列にプレイヤーのボーンマトリクスをかける
		D3DXMatrixMultiply(&mtxWing, &mtxWing, &mtxTemp);

		// ウィングを描画
		m_CSkinMesh[WING]->Draw(pDevice, mtxWing);

		// ソードを描画
		if (m_cSword != NULL)
		{
			m_cSword->Draw();
		}

		// 裏面をカリングに戻す
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		// αテストを無効に
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

#ifdef _DEBUG
		PrintDebugProc("Bone  [%d]\n", m_CSkinMesh[CHARACTER]->m_dwBoneCount);
		PrintDebugProc("Bone  [%d]\n", m_CSkinMesh[WING]->m_dwBoneCount);
#endif
		// ライティングを通常に戻す
		//SetLight(LIGHT_SUB1, FALSE);
		//SetLight(LIGHT_SUB2, FALSE);
	}
}


//=============================================================================
// 浮遊処理
//=============================================================================
void Player::Float(void)
{
	m_vY = m_vY + (D3DXVECTOR3(0.0f, 1.0f, 0.0f)- m_vY) * 0.1f;

	//m_vRot.y = atan2(m_vZ.z, m_vZ.x);

	if (InputPress(INPUT_UP_R) && InputPress(INPUT_DOWN_R))
	{
	}
	else if (InputPress(INPUT_UP_R))
	{
		m_vRotIner.x += 0.05f;
	}
	else if (InputPress(INPUT_DOWN_R))
	{
		m_vRotIner.x -= 0.05f;
	}

	if (InputPress(INPUT_LEFT_R) && InputPress(INPUT_RIGHT_R))
	{
	}
	else if (InputPress(INPUT_LEFT_R))
	{
		m_vRotIner.y += 0.05f;
	}
	else if (InputPress(INPUT_RIGHT_R))
	{
		m_vRotIner.y -= 0.05f;
	}

	// マウス
	m_vRotIner.x -= GetMobUseY() * 0.001f;
	m_vRotIner.y -= GetMobUseX() * 0.001f;

	m_vRot.x = m_vRot.x + ((m_vRotIner.x - m_vRot.x) * 0.05f);
	m_vRot.y = m_vRot.y + ((m_vRotIner.y - m_vRot.y) * 0.05f);



	if (GetKeyboardPress(DIK_SPACE) && GetKeyboardPress(DIK_LSHIFT))
	{
		m_fRiseSpeed = max(m_fRiseSpeed - 0.5f, -10.0f);
	}
	else if (GetKeyboardPress(DIK_SPACE))
	{
		m_fRiseSpeed = min(m_fRiseSpeed + 0.5f, PLAYER_MOVE_SPEED_MAX);
	}
	else
	{
		if (m_fRiseSpeed > 0.0f)
			m_fRiseSpeed = max(m_fRiseSpeed - 0.2f, 0.0f);
		else if (m_fRiseSpeed < 0.0f)
			m_fRiseSpeed = min(m_fRiseSpeed + 0.2f, 0.0f);
	}



	// ピッチ
	//if (InputPress(INPUT_UP_R) && InputPress(INPUT_DOWN_R))
	//{	// 同時押しは慣性を止める
	//	if (m_vRot.x > 0.0f)
	//		m_vRot.x = max(m_vRot.x - PLAYER_ROT_SPEED_X, 0.0f);
	//	else if (m_vRot.x < 0.0f)
	//		m_vRot.x = min(m_vRot.x + PLAYER_ROT_SPEED_X, 0.0f);
	//}
	//else if (InputPress(INPUT_UP_R))
	//{	// ピッチ角度を慣性で加算
	//	m_vRot.x = min(m_vRot.x + PLAYER_ROT_SPEED_X, PLAYER_ROT_SPEED_MAX_X);
	//}
	//else if (InputPress(INPUT_DOWN_R))
	//{	// ピッチ角度を慣性で減算
	//	m_vRot.x = max(m_vRot.x - PLAYER_ROT_SPEED_X, -PLAYER_ROT_SPEED_MAX_X);
	//}
	//else
	//{	// 入力なしは慣性を止める
	//	if (m_vRot.x > 0.0f)
	//		m_vRot.x = max(m_vRot.x - PLAYER_ROT_SPEED_X, 0.0f);
	//	else if (m_vRot.x < 0.0f)
	//		m_vRot.x = min(m_vRot.x + PLAYER_ROT_SPEED_X, 0.0f);
	//}

	//// ロール
	//if (InputPress(INPUT_LEFT_R) && InputPress(INPUT_RIGHT_R))
	//{	// 同時押しは慣性を止める
	//	if (m_vRot.z > 0.0f)
	//		m_vRot.z = max(m_vRot.z - PLAYER_ROT_SPEED_Z, 0.0f);
	//	else if (m_vRot.z < 0.0f)
	//		m_vRot.z = min(m_vRot.z + PLAYER_ROT_SPEED_Z, 0.0f);
	//}
	//else if (InputPress(INPUT_RIGHT_R))
	//{	// ロール角度を慣性で加算
	//	m_vRot.z = min(m_vRot.z + PLAYER_ROT_SPEED_Z, PLAYER_ROT_SPEED_MAX_Z);
	//}
	//else if (InputPress(INPUT_LEFT_R))
	//{	// ロール角度を慣性で減算
	//	m_vRot.z = max(m_vRot.z - PLAYER_ROT_SPEED_Z, -PLAYER_ROT_SPEED_MAX_Z);
	//}
	//else
	//{	// 入力なしは慣性を止める
	//	if (m_vRot.z > 0.0f)
	//		m_vRot.z = max(m_vRot.z - PLAYER_ROT_SPEED_Z, 0.0f);
	//	else if (m_vRot.z < 0.0f)
	//		m_vRot.z = min(m_vRot.z + PLAYER_ROT_SPEED_Z, 0.0f);
	//}


	//m_vZ = m_vZ + (D3DXVECTOR3(cosf(m_vRot.y), 0.0f, sinf(m_vRot.y)) - m_vZ) * 0.1f;


	//m_vZ *= 100;
	//m_vX = D3DXVECTOR3(m_mtxWorld._11, m_mtxWorld._12, m_mtxWorld._13);
	//m_vZ = D3DXVECTOR3(m_mtxWorld._31, m_mtxWorld._32, m_mtxWorld._33);
	D3DXVECTOR3 vTa = D3DXVECTOR3(0.0f,1.0f,0.0f);
	D3DXVECTOR3 vAxis =
		D3DXVECTOR3(cosf(m_vRot.y), 0.0f, sinf(m_vRot.y));

	QuaternionCalculate(&vTa, &vAxis, m_vRot.x, &vTa);
	
	m_vZ = vTa;

	// ローカルX軸のベクトルを外積で求める
	CrossProduct(&m_vX, &m_vY, &m_vZ);
	// ローカルX軸のベクトルを正規化
	D3DXVec3Normalize(&m_vX, &m_vX);

	// ローカルY軸のベクトルを外積で求める
	//CrossProduct(&m_vY, &m_vZ, &m_vX);


	//m_vX *= 2.0f;
	//m_vZ *= 2.0f;
	//vAxis *= 2.0f;

	//D3DXVec3Normalize(&m_vX, &m_vX);
	//D3DXVec3Normalize(&m_vZ, &m_vZ);
	//D3DXVec3Normalize(&vAxis, &vAxis);



	//// ピッチ回転を計算し、ロール回転用の軸を求める
	//QuaternionCalculate(&m_vZ, &m_vX, m_vRot.x, &m_vZ);

	//// ローカルY軸のベクトルを外積で求める
	//CrossProduct(&m_vY, &m_vZ, &m_vX);

	//// ピッチ回転を計算し、ロール回転用の軸を求める
	//QuaternionCalculate(&m_vZ, &m_vY, m_vRot.z, &m_vZ);

	//// ローカルY軸のベクトルを外積で求める
	//CrossProduct(&m_vX, &m_vY, &m_vZ);



	//D3DXVec3Normalize(&m_vX, &m_vX);
	//D3DXVec3Normalize(&m_vZ, &m_vZ);
	//D3DXVec3Normalize(&vAxis, &vAxis);


	//m_vZ = vEye;

#ifdef _DEBUG
	PrintDebugProc("【 Float 】\n");
	PrintDebugProc("vX  [%f,%f,%f]\n", m_vX.x, m_vX.y, m_vX.z);
	PrintDebugProc("vZ  [%f,%f,%f]\n", m_vZ.x, m_vZ.y, m_vZ.z);
	PrintDebugProc("Axis[%f,%f,%f]\n", vAxis.x, vAxis.y, vAxis.z);
	//PrintDebugProc("Axis[%f,%f,%f]\n", vAxis.x, vAxis.y, vAxis.z);
#endif

	// 移動処理
	if (InputPress(INPUT_LEFT))
	{
		if (InputPress(INPUT_UP))
		{// 左前移動
			//MoveFunc(m_fHAngle + D3DX_PI * 0.25f);
			m_vPos += (m_vX - m_vZ) * 0.71f * m_fMoveSpeed;
		}
		else if (InputPress(INPUT_DOWN))
		{// 左後移動
			//MoveFunc(m_fHAngle + D3DX_PI * 0.75f);
			m_vPos += (m_vX + m_vZ) * 0.71f * m_fMoveSpeed;
		}
		else if (InputPress(INPUT_RIGHT))
		{// 左右同時押しは処理なし
		}
		else
		{// 左移動
			//MoveFunc(m_fHAngle + D3DX_PI * 0.50f);
			m_vPos += m_vX * m_fMoveSpeed;
		}
	}
	else if (InputPress(INPUT_RIGHT))
	{
		if (InputPress(INPUT_UP))
		{// 右前移動
			//MoveFunc(m_fHAngle - D3DX_PI * 0.25f);
			m_vPos -= (m_vX + m_vZ) * 0.71f * m_fMoveSpeed;
		}
		else if (InputPress(INPUT_DOWN))
		{// 右後移動
			//MoveFunc(m_fHAngle - D3DX_PI * 0.75f);
			m_vPos -= (m_vX - m_vZ) * 0.71f * m_fMoveSpeed;
		}
		else
		{// 右移動
			//MoveFunc(m_fHAngle - D3DX_PI * 0.50f);
			m_vPos -= m_vX * m_fMoveSpeed;
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

	m_vPos.y += m_fRiseSpeed;

	// 移動を適用
	//m_vPos += m_vMove;


	// カメラをAtをモデルに設定
	Camera::SetAt(m_vPos + (m_vY * 30));

	// カメラEyeをモデル後方にセット
	Camera::SetEye(m_vPos + (m_vY * 30) + m_vZ * 100);

	// カメラ慣性を減らす
	Camera::AddEyeIner(0.02f);
	Camera::AddAtIner(0.02f);

	// カメラUpをモデル上部に設定
	Camera::SetUp(m_vY);
	//Camera::SetUp(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	
	//Cube::SetPos(m_vPos - m_vZ * 100);
}

//=============================================================================
// 飛行処理
//=============================================================================
void Player::Fly(void)
{
	// ピッチ
	if ((InputPress(INPUT_UP_R) && InputPress(INPUT_DOWN_R)) || (fGyroY < PLAYER_GYRO_MARGIN && fGyroY > -PLAYER_GYRO_MARGIN))
	{	// 同時押しは慣性を止める
		if (m_vRot.x > 0.0f)
			m_vRot.x = max(m_vRot.x - PLAYER_ROT_SPEED_X, 0.0f);
		else if (m_vRot.x < 0.0f)
			m_vRot.x = min(m_vRot.x + PLAYER_ROT_SPEED_X, 0.0f);
	}
	else if (InputPress(INPUT_DOWN_R) || fGyroY > PLAYER_GYRO_MARGIN)
	{	// ピッチ角度を慣性で加算
		m_vRot.x = min(m_vRot.x + PLAYER_ROT_SPEED_X, PLAYER_ROT_SPEED_MAX_X);
	}
	else if (InputPress(INPUT_UP_R) || fGyroY < -PLAYER_GYRO_MARGIN)
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
	if (InputPress(INPUT_LEFT_R) && InputPress(INPUT_RIGHT_R) || (fGyroZ < PLAYER_GYRO_MARGIN && fGyroZ > -PLAYER_GYRO_MARGIN))
	{	// 同時押しは慣性を止める
		if (m_vRot.z > 0.0f)
			m_vRot.z = max(m_vRot.z - PLAYER_ROT_SPEED_Z, 0.0f);
		else if (m_vRot.z < 0.0f)
			m_vRot.z = min(m_vRot.z + PLAYER_ROT_SPEED_Z, 0.0f);
	}
	else if (InputPress(INPUT_RIGHT_R) || fGyroZ < -PLAYER_GYRO_MARGIN)
	{	// ロール角度を慣性で加算
		m_vRot.z = min(m_vRot.z + PLAYER_ROT_SPEED_Z, PLAYER_ROT_SPEED_MAX_Z);
	}
	else if (InputPress(INPUT_LEFT_R) || fGyroZ > -PLAYER_GYRO_MARGIN)
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

	m_vRotIner.x = GetMobUseY() * 0.00006f;
	m_vRotIner.z = GetMobUseX() * 0.00006f;
	m_vRot += m_vRotIner;

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

	m_vUp = m_vY;

	// カメラをAtをモデルに設定
	Camera::SetAt(m_vPos + (m_vY * 30));

	// カメラUpをモデル上部に設定
	//Camera::SetUp(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	Camera::SetUp(m_vY);

	Camera::AddAtIner(0.02f);

	// カメラEyeをモデル後方にセット
	Camera::SetEye(m_vPos + m_vZ * 100);
	//SetCameraEye(m_vPos - m_vZ * 100 + m_vY * 50);
}

//=============================================================================
// 浮遊処理
//=============================================================================
void Player::LOCKON(void)
{

	if (GetKeyboardPress(DIK_SPACE) && GetKeyboardPress(DIK_LSHIFT))
	{
		m_fRiseSpeed = max(m_fRiseSpeed - 0.5f, -10.0f);
	}
	else if (GetKeyboardPress(DIK_SPACE))
	{
		m_fRiseSpeed = min(m_fRiseSpeed + 0.5f, PLAYER_MOVE_SPEED_MAX);
	}
	else
	{
		if (m_fRiseSpeed > 0.0f)
			m_fRiseSpeed = max(m_fRiseSpeed - 0.2f, 0.0f);
		else if (m_fRiseSpeed < 0.0f)
			m_fRiseSpeed = min(m_fRiseSpeed + 0.2f, 0.0f);
	}

	m_vPos.y += m_fRiseSpeed;

	// Ｙベクトルを上に向ける
	m_vY = m_vY + (D3DXVECTOR3(0.0f, 1.0f, 0.0f) - m_vY) * 0.1f;

	// 対象座標と自らの座標を減算
	D3DXVECTOR3 vLook = ZERO_D3DXVECTOR3 - m_vPos;
	vLook *= -1.0f;

	D3DXVec3Normalize(&vLook, &vLook);


	m_vZ = m_vZ + (vLook - m_vZ) * 0.1f;


	// 自らが向くベクトルを正規化する
	D3DXVec3Normalize(&m_vZ, &m_vZ);

	//m_vZ *= -1.0f;

	// ローカルX軸のベクトルを外積で求める
	CrossProduct(&m_vX, &m_vY, &m_vZ);

	// X軸を正規化する
	D3DXVec3Normalize(&m_vX, &m_vX);

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

	//m_vRotIner.x -= GetMobUseY() * 0.001f;
	//m_vRotIner.y -= GetMobUseX() * 0.001f;

	//m_vRot.x = m_vRot.x + ((m_vRotIner.x - m_vRot.x) * 0.05f);
	//m_vRot.y = m_vRot.y + ((m_vRotIner.y - m_vRot.y) * 0.05f);


	//D3DXVECTOR3 vTa = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//D3DXVECTOR3 vAxis =
	//	D3DXVECTOR3(cosf(m_vRot.y), 0.0f, sinf(m_vRot.y));

	//QuaternionCalculate(&m_vZ, &vAxis, m_vRot.x, &vTa);

	//m_vZ = vTa;

	//// ローカルY軸のベクトルを外積で求める
	//CrossProduct(&m_vX, &m_vY, &m_vZ);


#ifdef _DEBUG
	PrintDebugProc("【 Float 】\n");
	PrintDebugProc("vX  [%f,%f,%f]\n", m_vX.x, m_vX.y, m_vX.z);
	PrintDebugProc("vZ  [%f,%f,%f]\n", m_vZ.x, m_vZ.y, m_vZ.z);
	//PrintDebugProc("Axis[%f,%f,%f]\n", vAxis.x, vAxis.y, vAxis.z);
	//PrintDebugProc("Axis[%f,%f,%f]\n", vAxis.x, vAxis.y, vAxis.z);
#endif

	// 移動処理
	if (InputPress(INPUT_LEFT))
	{
		if (InputPress(INPUT_UP))
		{// 左前移動
		 //MoveFunc(m_fHAngle + D3DX_PI * 0.25f);
			m_vPos += (m_vX - m_vZ) * 0.71f * m_fMoveSpeed;
		}
		else if (InputPress(INPUT_DOWN))
		{// 左後移動
		 //MoveFunc(m_fHAngle + D3DX_PI * 0.75f);
			m_vPos += (m_vX + m_vZ) * 0.71f * m_fMoveSpeed;
		}
		else if (InputPress(INPUT_RIGHT))
		{// 左右同時押しは処理なし
		}
		else
		{// 左移動
		 //MoveFunc(m_fHAngle + D3DX_PI * 0.50f);
			m_vPos += m_vX * m_fMoveSpeed;
		}
	}
	else if (InputPress(INPUT_RIGHT))
	{
		if (InputPress(INPUT_UP))
		{// 右前移動
		 //MoveFunc(m_fHAngle - D3DX_PI * 0.25f);
			m_vPos -= (m_vX + m_vZ) * 0.71f * m_fMoveSpeed;
		}
		else if (InputPress(INPUT_DOWN))
		{// 右後移動
		 //MoveFunc(m_fHAngle - D3DX_PI * 0.75f);
			m_vPos -= (m_vX - m_vZ) * 0.71f * m_fMoveSpeed;
		}
		else
		{// 右移動
		 //MoveFunc(m_fHAngle - D3DX_PI * 0.50f);
			m_vPos -= m_vX * m_fMoveSpeed;
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
	Camera::SetAt(ZERO_D3DXVECTOR3);

	// カメラEyeをモデル後方にセット
	Camera::SetEye(m_vPos + (m_vY * 30) + m_vZ * 100);

	// カメラ慣性を減らす
	Camera::AddEyeIner(0.05f);
	Camera::AddAtIner(0.02f);

	// カメラUpをモデル上部に設定
	Camera::SetUp(m_vY);
	//Camera::SetUp(D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	//Cube::SetPos(m_vPos - m_vZ * 100);
}

//=============================================================================
// 移動処理
//=============================================================================
void Player::Change(void)
{
	if (m_nCount < PLAYER_CHANGE_FRAME)
	{
		m_vY += m_vRotChange;
		m_nCount++;
	}
	else
	{
		MatrixRotationVecAndUp(&m_mtxWorld, m_vZ, m_vY);
		m_vX = D3DXVECTOR3(m_mtxWorld._11, m_mtxWorld._12, m_mtxWorld._13);
		m_vZ = D3DXVECTOR3(m_mtxWorld._31, m_mtxWorld._32, m_mtxWorld._33);
		m_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		m_eMode = MODE_FLOAT;
	}

	// カメラをAtをモデルに設定
	Camera::SetAt(m_vPos + (m_vY * 30));

	// カメラEyeをモデル後方にセット
	Camera::SetEye(m_vPos + (m_vY * 30) + m_vZ * 200);


	// カメラUpをモデル上部に設定
	Camera::SetUp(m_vY);
	//Camera::SetUp(D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	Cube::SetPos(m_vPos - m_vZ * 100);
}


//=============================================================================
// 移動処理
//=============================================================================
void Player::Move(void)
{
	if (GetKeyboardPress(DIK_Q) && GetKeyboardPress(DIK_E))
	{
		//if (m_fMoveSpeed > 0.0f)
		//	m_fMoveSpeed = max(m_fMoveSpeed - PLAYER_MOVE_SPEED, 0.0f);
		//else if (m_fMoveSpeed < 0.0f)
		//	m_fMoveSpeed = min(m_fMoveSpeed + PLAYER_MOVE_SPEED, 0.0f);
	}
	else if (GetKeyboardPress(DIK_Q))
	{
		m_fMoveSpeed = max(m_fMoveSpeed - PLAYER_MOVE_SPEED, PLAYER_MOVE_SPEED_MIN);
	}
	else if (GetKeyboardPress(DIK_E))
	{
		m_fMoveSpeed = min(m_fMoveSpeed + PLAYER_MOVE_SPEED, PLAYER_MOVE_SPEED_MAX);
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
	if (GetKeyboardTrigger(DIK_TAB))
	{
		m_eMode = MODE_LOCKON;
	}
	if (m_eMode != m_eModeOld)
	{
		switch (m_eMode)
		{
		case MODE_FLOAT:
			SetSoundBgm(SOUND_BGM_TEST2);
			//m_vRot = D3DXVECTOR3(PLAYER_FLOAT_ROT_X,0.0f,0.0f);
			m_vRot = D3DXVECTOR3(PLAYER_FLOAT_ROT_X - atan2(m_vZ.y, 1.0f), -atan2(m_vZ.x, m_vZ.z),0.0f);
			m_vRotIner = m_vRot;
			Camera::SetEyeIner(0.01f);
			Camera::SetAtIner(0.05f);
			break;
		case MODE_FLY:
			SetSoundBgm(SOUND_BGM_TEST1);
			m_vRot = ZERO_D3DXVECTOR3;
			Camera::SetEyeIner(0.05f);
			Camera::SetAtIner(0.05f);
			break;
		case MODE_LOCKON:
			SetSoundBgm(SOUND_BGM_TEST2);
			m_vRot = ZERO_D3DXVECTOR3;
			Camera::SetEyeIner(1.0f);
			Camera::SetAtIner(0.05f);
			break;
		case MODE_CHANGE:
			m_vRotChange = (D3DXVECTOR3(0.0f, 1.0f, 0.0f) - m_vY) / PLAYER_CHANGE_FRAME;	
			m_nCount = 0;
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