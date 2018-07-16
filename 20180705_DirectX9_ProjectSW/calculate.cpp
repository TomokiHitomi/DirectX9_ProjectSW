//=============================================================================
//
// �v�Z���� [calculate.cpp]
// Author : GP12A295 25 �l���F��
//
// �v�Z�����S�ʂ��s��cpp
//
//=============================================================================
#include "calculate.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//=============================================================================
// ���[���h�ϊ�
//=============================================================================
void WorldConvert(D3DXMATRIX *world, D3DXVECTOR3 pos,
	D3DXVECTOR3 rot, D3DXVECTOR3 scl)
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	/******************** ���[���h�ϊ� ********************/
	// ���[���h�}�g���N�X�̏�����
	D3DXMatrixIdentity(world);

	// �yS�z�X�P�[���𔽉f(Multiply�͍s��v�Z)
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
	D3DXMatrixMultiply(world, world, &mtxScl);

	// �yR�z��]�𔽉f(YawPitchRoll��y,x,z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(world, world, &mtxRot);

	// �yT�z���s�ړ��𔽉f(�I�u�W�F�N�g��z�u���Ă���j
	D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(world, world, &mtxTranslate);
}

//=============================================================================
// ���[���h�ϊ��iMatrixRotationVecAndUp�d�l�j
//=============================================================================
void WorldConvertAxis(D3DXMATRIX *world, D3DXVECTOR3 pos,
	D3DXVECTOR3 vLook, D3DXVECTOR3 vUp, D3DXVECTOR3 scl)
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	/******************** ���[���h�ϊ� ********************/
	// ���[���h�}�g���N�X�̏�����
	D3DXMatrixIdentity(world);

	// �yS�z�X�P�[���𔽉f(Multiply�͍s��v�Z)
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
	D3DXMatrixMultiply(world, world, &mtxScl);

	// �yR�z��]�𔽉f(YawPitchRoll��y,x,z)
	MatrixRotationVecAndUp(&mtxRot, vLook, vUp);
	D3DXMatrixMultiply(world, world, &mtxRot);

	// �yT�z���s�ړ��𔽉f(�I�u�W�F�N�g��z�u���Ă���j
	D3DXMatrixTranslation(&mtxTranslate, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(world, world, &mtxTranslate);
}

//=============================================================================
// ��]�s��v�Z
// vLook�������AvUp����Ƃ����]�s���mtxOut�ɃA�E�g�v�b�g����
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
// ����
//=============================================================================
float DotProduct(D3DXVECTOR3 *vl, D3DXVECTOR3 *vr)
{
	float ans = 0;
	//ans = (vl->x * vr->x) + (vl->y * vr->y) + (vl->z * vr->z);
	ans = D3DXVec3Dot(vl, vr);
	return(ans);
}

//=============================================================================
// �O��
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
// ���l���E�ݒ�
//=============================================================================
float SetLimit(float fTag, float fMax, float fMin)
{
	if (fTag > fMax) { return fMax; }
	else if (fMin > fTag) { return fMin; }
	else { return fTag; }
}
//=============================================================================
// PI���������i180�x�j
//=============================================================================
float PiCalculate180(float fAngle)
{
	// 180�x�𒴂�����360�x������
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
// PI���������i360�x�j
//=============================================================================
float PiCalculate360(float fAngle)
{
	// 360�x�𒴂�����360�x������
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
// �N�H�[�^�j�I������
//=============================================================================
void QuaternionCalculate(D3DXVECTOR3 *pOrigin, D3DXVECTOR3 *pAxis, float fAngle, D3DXVECTOR3 *pPos)
{
	// �����A�������ʗp�̕ϐ����`
	D3DXQUATERNION qReversal, qAnswer;
	// ��]���������_�̏����ʒu
	D3DXQUATERNION qOrigin(pOrigin->x, pOrigin->y, pOrigin->z, 0.0f);
	// �P�ʃN�H�[�^�j�I��
	D3DXQUATERNION qQuaternion(0.0f, 0.0f, 0.0f, 1.0f);

	// ��pAxis���̉�]�N�H�[�^�j�I���𐶐�
	D3DXQuaternionRotationAxis(&qQuaternion, pAxis, fAngle);
	// XYZ�}�C�i�X�̋������Z�o
	D3DXQuaternionConjugate(&qReversal, &qQuaternion);
	// qReversal�EqOrigin�EqQuaternion���s��v�Z
	D3DXQuaternionMultiply(&qAnswer, &qReversal, &qOrigin);
	// qAnswer�ɉ�]������̌��ʂ��i�[
	D3DXQuaternionMultiply(&qAnswer, &qAnswer, &qQuaternion);

	// ��]�����̌��ʂ𔽉f
	pPos->x = qAnswer.x;
	pPos->y = qAnswer.y;
	pPos->z = qAnswer.z;
}

//=============================================================================
// �J���[�p���b�g�Ăяo������
//=============================================================================
D3DXCOLOR SetColorPallet(int nColor)
{
	D3DXCOLOR temp;
	//temp = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	switch (nColor)
	{
	case COLOR_PALLET_WHITE:		// �z���C�g
		temp = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	case COLOR_PALLET_RED:			// ���b�h
		temp = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		break;
	case COLOR_PALLET_YELLOW:		// �C�G���[
		temp = D3DXCOLOR(0.9f, 0.9f, 0.0f, 1.0f);
		break;
	case COLOR_PALLET_GREEN:		// �O���[��
		temp = D3DXCOLOR(0.0f, 1.0f, 0.05f, 1.0f);
		break;
	case COLOR_PALLET_CYAN:			// �V�A��
		temp = D3DXCOLOR(0.0f, 0.9f, 0.9f, 1.0f);
		break;
	case COLOR_PALLET_BLUE:			// �u���[
		temp = D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f);
		break;
	case COLOR_PALLET_MAGENTA:		// �}�[���_
		temp = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		break;
	case COLOR_PALLET_BROWN:		// �u���E��
		temp = D3DXCOLOR(0.6f, 0.4f, 0.0f, 1.0f);
		break;
	case COLOR_PALLET_LIGHTGREEN:	// ���C�g�O���[��
		temp = D3DXCOLOR(0.3f, 0.9f, 0.3f, 1.0f);
		break;
	case COLOR_PALLET_ORANGE:		// �I�����W
		temp = D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f);
		break;
	case COLOR_PALLET_BLACK:		// �u���b�N
		temp = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	case COLOR_PALLET_LIGHTBLUE:	// �u���[
		temp = D3DXCOLOR(0.4f, 0.7f, 1.0f, 1.0f);
		break;
	case COLOR_PALLET_GRAY:			// �O���C
		temp = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		break;
	}
	return temp;
}

//=============================================================================
// int�z��̃V���b�t���֐�
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
* �֐����FD3DXVECTOR3 Calculate_Vec_2pos(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
* ����  �F��̈ʒu
* �߂�l�Ffloat�^�̒���
* ����  �Fpos2����pos1�̃x�N�g�������߂鏈��
*******************************************************************************/
D3DXVECTOR3 Calculate_Vec_2pos(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	// �x�N�g����Ԃ�
	return(pos1 - pos2);
}

/*******************************************************************************
* �֐����Ffloat Calculate_Atan2XZ(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
* ����  �F��̈ʒu
* �߂�l�Ffloat�^�̊p�x
* ����  �Fpos1�݂�pos2�ւ̊p�x�iXZ���j
*******************************************************************************/
float Calculate_Atan2XZ(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	D3DXVECTOR3 vecTemp;
	vecTemp.x = pos2.x - pos1.x;
	vecTemp.z = pos2.z - pos1.z;

	// atan2�̃G���[�h�~
	if (vecTemp.x == 0 && vecTemp.z == 0)
	{
		vecTemp.x += 0.00001;
	}

	// �p�x�v�Z
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