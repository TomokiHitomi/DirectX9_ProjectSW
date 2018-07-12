//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "calculate.h"
#include "input.h"
#include "camera.h"
#include "cube.h"

// �f�o�b�O�p
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^�i�����������j
//=============================================================================
Player::Player(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �I�u�W�F�N�gID�ƃv���C�I���e�B�̐ݒ菈��
	SetIdAndPriority(ObjectID::PLAYER, Priority::Middle, Priority::Middle);

	// �e�v���p�e�B�̏�����
	m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vScl = D3DXVECTOR3(PLAYER_SCL, PLAYER_SCL, PLAYER_SCL);

	m_vMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vRotIner = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_bUse = true;

	// ���f���̏�����
	//m_CSkinMesh->Init(pDevice, "data/MODEL/Yuko.x");
	m_CSkinMesh = new CSkinMesh;
	m_CSkinMesh->Init(pDevice, PLAYER_MODEL);

}

//=============================================================================
// �f�X�g���N�^�i�I�������j
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
// �X�V����
//=============================================================================
void Player::Update(void)
{
#ifdef _DEBUG
	PrintDebugProc("�y MODEL �z\n");
#endif

	if (m_bUse)
	{
		// �ړ�����
		Move();

		// ���[���h�ϊ�
		//WorldConvert(&m_mtxWorld, m_vPos, m_vRot, m_vScl);
		//WorldConvertPR(&m_mtxWorld, m_vPos, m_vRot, m_vScl);

		// �A�j���[�V�����X�V����
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
// �`�揈��
//=============================================================================
void Player::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���C�e�B���O�����߂ɂ��Ă�
	//SetLight(LIGHT_SUB1, TRUE);
	//SetLight(LIGHT_SUB2, TRUE);

	if (m_bUse)
	{
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_CSkinMesh->Draw(pDevice);
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}

	// ���C�e�B���O��ʏ�ɖ߂�
	//SetLight(LIGHT_SUB1, FALSE);
	//SetLight(LIGHT_SUB2, FALSE);
}

//=============================================================================
// �J�����ړ��֐�
//=============================================================================
void Player::Move(void)
{
	//// �ړ�����
	//if (InputPress(INPUT_LEFT))
	//{
	//	if (InputPress(INPUT_UP))
	//	{// ���O�ړ�
	//		MoveFunc(m_vRot.y + D3DX_PI * 0.75f);
	//	}
	//	else if (InputPress(INPUT_DOWN))
	//	{// ����ړ�
	//		MoveFunc(m_vRot.y + D3DX_PI * 0.25f);
	//	}
	//	else if (InputPress(INPUT_RIGHT))
	//	{// ���E���������͏����Ȃ�
	//	}
	//	else
	//	{// ���ړ�
	//		MoveFunc(m_vRot.y + D3DX_PI * 0.50f);
	//	}
	//}
	//else if (InputPress(INPUT_RIGHT))
	//{
	//	if (InputPress(INPUT_UP))
	//	{// �E�O�ړ�
	//		MoveFunc(m_vRot.y - D3DX_PI * 0.75f);
	//	}
	//	else if (InputPress(INPUT_DOWN))
	//	{// �E��ړ�
	//		MoveFunc(m_vRot.y - D3DX_PI * 0.25f);
	//	}
	//	else
	//	{// �E�ړ�
	//		MoveFunc(m_vRot.y - D3DX_PI * 0.50f);
	//	}
	//}
	//else if (InputPress(INPUT_UP))
	//{// �O�ړ�
	//	if (InputPress(INPUT_DOWN))
	//	{// �O�㓯�������͏����Ȃ�
	//	}
	//	else
	//	{
	//		MoveFunc(m_vRot.y + D3DX_PI);
	//	}
	//}
	//else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN))
	//{// ��ړ�
	//	MoveFunc(m_vRot.y);
	//}


	if (InputPress(INPUT_UP) && InputPress(INPUT_DOWN))
	{
		// ���������͏����Ȃ�
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
		// ���������͏����Ȃ�
	}
	else if (InputPress(INPUT_LEFT))
	{
		m_vRotIner.z -= PLAYER_ROT_SPEED;
	}
	else if (InputPress(INPUT_RIGHT))
	{
		m_vRotIner.z += PLAYER_ROT_SPEED;
	}


	//// ��]�ʂ����b�g�ɓK�p
	//m_vRot += m_vRotIner;

	//// ��]������K�p
	//m_vRotIner -= m_vRotIner * PLAYER_ROT_INERTIA;

	// �J���������f���ɃZ�b�g
	D3DXVECTOR3 vTemp = m_vPos;
	vTemp.y += PLAYER_HEIGHT;
	SetCameraAt(vTemp);

	// ��邺����������

	//D3DXVECTOR3 vTa = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 vAxis = D3DXVECTOR3(cosf(m_vRotIner.z), sinf(m_vRotIner.z), 0.0f);
	//D3DXVECTOR3 vTa = D3DXVECTOR3(-sinf(m_vRotIner.z), cosf(m_vRotIner.z), 0.0f);

	D3DXVECTOR3 vTa = D3DXVECTOR3(cosf(D3DX_PI * 0.5f + m_vRotIner.z), sinf(D3DX_PI * 0.5f + m_vRotIner.z), 0.0f);

	////void QuaternionCalculate(D3DXVECTOR3 *pOrigin, D3DXVECTOR3 *pAxis, float fAngle, D3DXVECTOR3 *pPos)
	////{
	// �����A�������ʗp�̕ϐ����`
	D3DXQUATERNION qReversal, qAnswer, qAnswer2;
	// ��]���������_�̏����ʒu
	D3DXQUATERNION qOrigin(vTa.x, vTa.y, vTa.z, 0.0f);
	// �P�ʃN�H�[�^�j�I��
	D3DXQUATERNION qQuaternion(0.0f, 0.0f, 0.0f, 1.0f);

	// ��pAxis���̉�]�N�H�[�^�j�I���𐶐�
	D3DXQuaternionRotationAxis(&qQuaternion, &vAxis, m_vRotIner.x);
	// XYZ�}�C�i�X�̋������Z�o
	D3DXQuaternionConjugate(&qReversal, &qQuaternion);
	// qReversal�EqOrigin�EqQuaternion���s��v�Z
	D3DXQuaternionMultiply(&qAnswer, &qReversal, &qOrigin);
	// qAnswer�ɉ�]������̌��ʂ��i�[
	D3DXQuaternionMultiply(&qAnswer, &qAnswer, &qQuaternion);

	D3DXVECTOR3 vAxis2 = D3DXVECTOR3(qAnswer.x, qAnswer.y, qAnswer.z);
	D3DXVECTOR3 vTa2 = D3DXVECTOR3(-sinf(m_vRotIner.z), qAnswer.x, 0.0f);

	// ��]���������_�̏����ʒu
	D3DXQUATERNION qOrigin2(vTa2.x, vTa2.y, vTa2.z, 0.0f);

	// ��pAxis���̉�]�N�H�[�^�j�I���𐶐�
	D3DXQuaternionRotationAxis(&qQuaternion, &vAxis2, D3DX_PI *0.5f + m_vRotIner.x);
	// XYZ�}�C�i�X�̋������Z�o
	D3DXQuaternionConjugate(&qReversal, &qQuaternion);
	// qReversal�EqOrigin�EqQuaternion���s��v�Z
	D3DXQuaternionMultiply(&qAnswer2, &qReversal, &qOrigin2);
	// qAnswer�ɉ�]������̌��ʂ��i�[
	D3DXQuaternionMultiply(&qAnswer2, &qAnswer2, &qQuaternion);

	 //��]�����̌��ʂ𔽉f
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



	/******************** ���[���h�ϊ� ********************/
	// ���[���h�}�g���N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �yS�z�X�P�[���𔽉f(Multiply�͍s��v�Z)
	D3DXMatrixScaling(&mtxScl, m_vScl.x, m_vScl.y, m_vScl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	//// �yR�z��]�𔽉f(YawPitchRoll��y,x,z)

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



	// �yT�z���s�ړ��𔽉f(�I�u�W�F�N�g��z�u���Ă���j
	D3DXMatrixTranslation(&mtxTranslate, m_vPos.x, m_vPos.y, m_vPos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);


	//Cube::SetPos(m_vRot);

	//// ��]���a��ݒ�
	//D3DXVECTOR3 vTa = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//D3DXVECTOR3 vZero = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//D3DXVECTOR3 vAxis;

	//// ���݂̊p�x�ɐ����ȉ�]���x�N�g����ݒ�
	//vAxis = D3DXVECTOR3(cosf(m_vRot.z), 0, sinf(m_vRot.z));

	//// �N�H�[�^�j�I������
	//QuaternionCalculate(&vTa, &vAxis, D3DX_PI * 1.5f + m_vRot.x,
	//	&vZero);

	//// ��]���ɐݒu
	//Cube::SetPos((m_vPos + vZero) * 100.0f);
	//m_vPos += vZero;

//	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
//
//	/******************** ���[���h�ϊ� ********************/
//	// ���[���h�}�g���N�X�̏�����
//	D3DXMatrixIdentity(&m_mtxWorld);
//
//	// �yT�z���s�ړ��𔽉f(�I�u�W�F�N�g��z�u���Ă���j
//	D3DXMatrixTranslation(&mtxTranslate, 0.0f, 0.0f, 1.0f);
//	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);
//
//	// �yR�z��]�𔽉f(YawPitchRoll��y,x,z)
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
// �ړ������֐�
//=============================================================================
void Player::MoveFunc(float fAngle)
{
	m_vPos.x += sinf(fAngle) * PLAYER_MOVE_SPEED;
	m_vPos.z += cosf(fAngle) * PLAYER_MOVE_SPEED;
}

//=============================================================================
// ��]�����֐�
//=============================================================================
void Player::RotFunc(D3DXVECTOR3 vRot)
{
	//m_vPos.x += sinf(fAngle) * PLAYER_MOVE_SPEED;
	//m_vPos.z += cosf(fAngle) * PLAYER_MOVE_SPEED;
}