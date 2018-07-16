//=============================================================================
//
// 計算処理 [calculate.cpp]
// Author : GP12A295 25 人見友基
//
// 計算処理全般を行うcpp
//
//=============================================================================
#include "calculate.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//=============================================================================
// ワールド変換
//=============================================================================
void WorldConvert(D3DXMATRIX *world, D3DXVECTOR3 pos,
	D3DXVECTOR3 rot, D3DXVECTOR3 scl)
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	/******************** ワールド変換 ********************/
	// ワールドマトリクスの初期化
	D3DXMatrixIdentity(world);

	// 【S】スケールを反映(Multiplyは行列計算)
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
	D3DXMatrixMultiply(world, world, &mtxScl);

	// 【R】回転を反映(YawPitchRollはy,x,z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(world, world, &mtxRot);

	// 【T】平行移動を反映(オブジェクトを配置している）
	D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(world, world, &mtxTranslate);
}

//=============================================================================
// ワールド変換（MatrixRotationVecAndUp仕様）
//=============================================================================
void WorldConvertAxis(D3DXMATRIX *world, D3DXVECTOR3 pos,
	D3DXVECTOR3 vLook, D3DXVECTOR3 vUp, D3DXVECTOR3 scl)
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	/******************** ワールド変換 ********************/
	// ワールドマトリクスの初期化
	D3DXMatrixIdentity(world);

	// 【S】スケールを反映(Multiplyは行列計算)
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
	D3DXMatrixMultiply(world, world, &mtxScl);

	// 【R】回転を反映(YawPitchRollはy,x,z)
	MatrixRotationVecAndUp(&mtxRot, vLook, vUp);
	D3DXMatrixMultiply(world, world, &mtxRot);

	// 【T】平行移動を反映(オブジェクトを配置している）
	D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(world, world, &mtxTranslate);
}

//=============================================================================
// 回転行列計算
// vLookを向き、vUpを上とする回転行列をmtxOutにアウトプットする
//=============================================================================
void MatrixRotationVecAndUp(D3DXMATRIX *mtxOut, D3DXVECTOR3 vLook, D3DXVECTOR3 vUp)
{
	D3DXVECTOR3 X, Y, Z, Up;

	Up = vUp;
	Z = vLook;
	D3DXVec3Normalize(&Z, &Z);
	D3DXVec3Normalize(&Up, &Up);
	D3DXVec3Cross(&X, D3DXVec3Normalize(&Y, &Up), &Z);
	D3DXVec3Normalize(&X, &X);
	D3DXVec3Normalize(&Y, D3DXVec3Cross(&Y, &Z, &X));

	mtxOut->_11 = X.x;
	mtxOut->_12 = X.y;
	mtxOut->_13 = X.z;
	mtxOut->_14 = 0.0f;

	mtxOut->_21 = Y.x;
	mtxOut->_22 = Y.y;
	mtxOut->_23 = Y.z;
	mtxOut->_24 = 0.0f;

	mtxOut->_31 = Z.x;
	mtxOut->_32 = Z.y;
	mtxOut->_33 = Z.z;
	mtxOut->_34 = 0.0f;

	mtxOut->_41 = 0.0f;
	mtxOut->_42 = 0.0f;
	mtxOut->_43 = 0.0f;
	mtxOut->_44 = 1.0f;
}

//=============================================================================
// 内積
//=============================================================================
float DotProduct(D3DXVECTOR3 *vl, D3DXVECTOR3 *vr)
{
	float ans = 0;
	//ans = (vl->x * vr->x) + (vl->y * vr->y) + (vl->z * vr->z);
	ans = D3DXVec3Dot(vl, vr);
	return(ans);
}

//=============================================================================
// 外積
//=============================================================================
D3DXVECTOR3 *CrossProduct(D3DXVECTOR3 *ret, D3DXVECTOR3 *vl, D3DXVECTOR3 *vr)
{
	//ret->x = (vl->y * vr->z) - (vl->z * vr->y);
	//ret->y = (vl->z * vr->x) - (vl->x * vr->z);
	//ret->z = (vl->x * vr->y) - (vl->y * vr->x);
	D3DXVec3Cross(ret, vl, vr);
	return(ret);
}

//=============================================================================
// 数値限界設定
//=============================================================================
float SetLimit(float fTag, float fMax, float fMin)
{
	if (fTag > fMax) { return fMax; }
	else if (fMin > fTag) { return fMin; }
	else { return fTag; }
}
//=============================================================================
// PI調整処理（180度）
//=============================================================================
float PiCalculate180(float fAngle)
{
	// 180度を超えたら360度を引く
	if (fAngle >= D3DX_PI)
	{
		fAngle -= D3DX_PI * 2.0f;
	}
	else if (fAngle < -D3DX_PI)
	{
		fAngle += D3DX_PI * 2.0f;
	}
	return fAngle;
}

//=============================================================================
// PI調整処理（360度）
//=============================================================================
float PiCalculate360(float fAngle)
{
	// 360度を超えたら360度を引く
	if (fAngle > D3DX_PI * 2)
	{
		fAngle -= D3DX_PI * 2.0f;
	}
	else if (fAngle < 0.0f)
	{
		fAngle += D3DX_PI * 2.0f;
	}
	return fAngle;
}

//=============================================================================
// クォータニオン処理
//=============================================================================
void QuaternionCalculate(D3DXVECTOR3 *pOrigin, D3DXVECTOR3 *pAxis, float fAngle, D3DXVECTOR3 *pPos)
{
	// 反数、処理結果用の変数を定義
	D3DXQUATERNION qReversal, qAnswer;
	// 回転させたい点の初期位置
	D3DXQUATERNION qOrigin(pOrigin->x, pOrigin->y, pOrigin->z, 0.0f);
	// 単位クォータニオン
	D3DXQUATERNION qQuaternion(0.0f, 0.0f, 0.0f, 1.0f);

	// 軸pAxis回りの回転クォータニオンを生成
	D3DXQuaternionRotationAxis(&qQuaternion, pAxis, fAngle);
	// XYZマイナスの共役を算出
	D3DXQuaternionConjugate(&qReversal, &qQuaternion);
	// qReversal・qOrigin・qQuaternionを行列計算
	D3DXQuaternionMultiply(&qAnswer, &qReversal, &qOrigin);
	// qAnswerに回転処理後の結果を格納
	D3DXQuaternionMultiply(&qAnswer, &qAnswer, &qQuaternion);

	// 回転処理の結果を反映
	pPos->x = qAnswer.x;
	pPos->y = qAnswer.y;
	pPos->z = qAnswer.z;
}

//=============================================================================
// カラーパレット呼び出し処理
//=============================================================================
D3DXCOLOR SetColorPallet(int nColor)
{
	D3DXCOLOR temp;
	//temp = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	switch (nColor)
	{
	case COLOR_PALLET_WHITE:		// ホワイト
		temp = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	case COLOR_PALLET_RED:			// レッド
		temp = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		break;
	case COLOR_PALLET_YELLOW:		// イエロー
		temp = D3DXCOLOR(0.9f, 0.9f, 0.0f, 1.0f);
		break;
	case COLOR_PALLET_GREEN:		// グリーン
		temp = D3DXCOLOR(0.0f, 1.0f, 0.05f, 1.0f);
		break;
	case COLOR_PALLET_CYAN:			// シアン
		temp = D3DXCOLOR(0.0f, 0.9f, 0.9f, 1.0f);
		break;
	case COLOR_PALLET_BLUE:			// ブルー
		temp = D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f);
		break;
	case COLOR_PALLET_MAGENTA:		// マゼンダ
		temp = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		break;
	case COLOR_PALLET_BROWN:		// ブラウン
		temp = D3DXCOLOR(0.6f, 0.4f, 0.0f, 1.0f);
		break;
	case COLOR_PALLET_LIGHTGREEN:	// ライトグリーン
		temp = D3DXCOLOR(0.3f, 0.9f, 0.3f, 1.0f);
		break;
	case COLOR_PALLET_ORANGE:		// オレンジ
		temp = D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f);
		break;
	case COLOR_PALLET_BLACK:		// ブラック
		temp = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	case COLOR_PALLET_LIGHTBLUE:	// ブルー
		temp = D3DXCOLOR(0.4f, 0.7f, 1.0f, 1.0f);
		break;
	case COLOR_PALLET_GRAY:			// グレイ
		temp = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		break;
	}
	return temp;
}

//=============================================================================
// int配列のシャッフル関数
//=============================================================================
void SetShuffle(int *nShuffle, int nSize)
{
	for (int i = 0; i < nSize; i++)
	{
		int j = rand() % nSize;
		int t = nShuffle[i];
		nShuffle[i] = nShuffle[j];
		nShuffle[j] = t;
	}
}

/*******************************************************************************
* 関数名：D3DXVECTOR3 Calculate_Vec_2pos(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
* 引数  ：二つの位置
* 戻り値：float型の長さ
* 説明  ：pos2からpos1のベクトルを求める処理
*******************************************************************************/
D3DXVECTOR3 Calculate_Vec_2pos(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	// ベクトルを返す
	return(pos1 - pos2);
}

/*******************************************************************************
* 関数名：float Calculate_Atan2XZ(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
* 引数  ：二つの位置
* 戻り値：float型の角度
* 説明  ：pos1みたpos2への角度（XZ軸）
*******************************************************************************/
float Calculate_Atan2XZ(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	D3DXVECTOR3 vecTemp;
	vecTemp.x = pos2.x - pos1.x;
	vecTemp.z = pos2.z - pos1.z;

	// atan2のエラー防止
	if (vecTemp.x == 0 && vecTemp.z == 0)
	{
		vecTemp.x += 0.00001;
	}

	// 角度計算
	return atan2(vecTemp.z, vecTemp.x);
}

//#ifdef _DEBUG
//PrintDebugProc("mtxX[%f,%f,%f]\n",
//	m_mtxWorld._11, m_mtxWorld._12, m_mtxWorld._13);
//PrintDebugProc("mtxY[%f,%f,%f]\n",
//	m_mtxWorld._21, m_mtxWorld._22, m_mtxWorld._23);
//PrintDebugProc("mtxZ[%f,%f,%f]\n",
//	m_mtxWorld._31, m_mtxWorld._32, m_mtxWorld._33);
//PrintDebugProc("mtxA[%f,%f,%f]\n",
//	m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);
//#endif