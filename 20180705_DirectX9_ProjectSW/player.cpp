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

	m_vX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_vY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	m_eMode = MODE_FLY;
	m_eModeOld = MODE_MAX;	// 1�t���[���ڂ̓��[�h�`�F���W�ɓ��肽��

	m_fMoveSpeed = PLAYER_MOVE_SPEED_MIN;
	m_bUse = true;

	// ���f���̏�����
	//m_CSkinMesh->Init(pDevice, "data/MODEL/Yuko.x");
	m_CSkinMesh = new CSkinMesh;
	m_CSkinMesh->Init(pDevice, PLAYER_MODEL);
	
	D3DXMATRIX mtxTranslate;

	D3DXMatrixIdentity(&m_mtxWorld);



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

		switch (m_eMode)
		{
		case MODE_FLOAT:
			Float();	// ���V����
			break;
		case MODE_FLY:
			Fly();		// ��s����
			break;
		}

		// ���[�h�`�F���W
		ModeChange();

		// ���[���h�ϊ�
		WorldConvertAxis(&m_mtxWorld, m_vPos, m_vZ, m_vY, m_vScl);



		// �A�j���[�V�����X�V����
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
		// ���e�X�g��L����
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, PLAYER_ALPHA_TEST);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		// ���ʕ`�悷��
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		// ���f����`��
		m_CSkinMesh->Draw(pDevice);

		// ���ʂ��J�����O�ɖ߂�
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		// ���e�X�g�𖳌���
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	}

	// ���C�e�B���O��ʏ�ɖ߂�
	//SetLight(LIGHT_SUB1, FALSE);
	//SetLight(LIGHT_SUB2, FALSE);
}


//=============================================================================
// ���V����
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

	// �s�b�`
	if (InputPress(INPUT_UP_R) && InputPress(INPUT_DOWN_R))
	{	// ���������͊������~�߂�
		if (m_vRot.x > 0.0f)
			m_vRot.x = max(m_vRot.x - PLAYER_ROT_SPEED_X, 0.0f);
		else if (m_vRot.x < 0.0f)
			m_vRot.x = min(m_vRot.x + PLAYER_ROT_SPEED_X, 0.0f);
	}
	else if (InputPress(INPUT_UP_R))
	{	// �s�b�`�p�x�������ŉ��Z
		m_vRot.x = min(m_vRot.x + PLAYER_ROT_SPEED_X, PLAYER_ROT_SPEED_MAX_X);
	}
	else if (InputPress(INPUT_DOWN_R))
	{	// �s�b�`�p�x�������Ō��Z
		m_vRot.x = max(m_vRot.x - PLAYER_ROT_SPEED_X, -PLAYER_ROT_SPEED_MAX_X);
	}
	else
	{	// ���͂Ȃ��͊������~�߂�
		if (m_vRot.x > 0.0f)
			m_vRot.x = max(m_vRot.x - PLAYER_ROT_SPEED_X, 0.0f);
		else if (m_vRot.x < 0.0f)
			m_vRot.x = min(m_vRot.x + PLAYER_ROT_SPEED_X, 0.0f);
	}


	//m_vZ = m_vZ + (D3DXVECTOR3(cosf(m_vRot.y), 0.0f, sinf(m_vRot.y)) - m_vZ) * 0.1f;
	CrossProduct(&m_vX, &m_vY, &m_vZ);



	D3DXVECTOR3 vEye = m_vZ;

	//m_vZ *= 100;

	// ���[����]���v�Z���A�s�b�`��]�p�̎������߂�
	QuaternionCalculate(&m_vZ, &m_vX, m_vRot.x, &m_vZ);
	//D3DXVec3Normalize(&m_vZ, &m_vZ);

	//m_vZ = vEye;

	// �ړ�����
	if (InputPress(INPUT_LEFT))
	{
		if (InputPress(INPUT_UP))
		{// ���O�ړ�
			//MoveFunc(m_fHAngle + D3DX_PI * 0.25f);
		}
		else if (InputPress(INPUT_DOWN))
		{// ����ړ�
			//MoveFunc(m_fHAngle + D3DX_PI * 0.75f);
		}
		else if (InputPress(INPUT_RIGHT))
		{// ���E���������͏����Ȃ�
		}
		else
		{// ���ړ�
			//MoveFunc(m_fHAngle + D3DX_PI * 0.50f);
		}
	}
	else if (InputPress(INPUT_RIGHT))
	{
		if (InputPress(INPUT_UP))
		{// �E�O�ړ�
			//MoveFunc(m_fHAngle - D3DX_PI * 0.25f);
		}
		else if (InputPress(INPUT_DOWN))
		{// �E��ړ�
			//MoveFunc(m_fHAngle - D3DX_PI * 0.75f);
		}
		else
		{// �E�ړ�
			//MoveFunc(m_fHAngle - D3DX_PI * 0.50f);
		}
	}
	else if (InputPress(INPUT_UP))
	{// �O�ړ�
		if (InputPress(INPUT_DOWN))
		{// �O�㓯�������͏����Ȃ�
		}
		else
		{
			m_vPos += -m_vZ * m_fMoveSpeed;
		}
	}
	else if (InputPress(INPUT_DOWN))
	{// ��ړ�
		m_vPos += m_vZ * m_fMoveSpeed;
	}


	// �ړ���K�p
	//m_vPos += m_vMove;


	// �J������At�����f���ɐݒ�
	Camera::SetAt(m_vPos + (m_vY * 30));

	// �J����Eye�����f������ɃZ�b�g
	Camera::SetEye(m_vPos + (m_vY * 30) + m_vZ * 100);

	// �J�������������炷
	Camera::AddEyeIner(0.02f);

	// �J����Up�����f���㕔�ɐݒ�
	Camera::SetUp(m_vY);

}

//=============================================================================
// ��s����
//=============================================================================
void Player::Fly(void)
{
	// �s�b�`
	if (InputPress(INPUT_UP_R) && InputPress(INPUT_DOWN_R))
	{	// ���������͊������~�߂�
		if (m_vRot.x > 0.0f)
			m_vRot.x = max(m_vRot.x - PLAYER_ROT_SPEED_X, 0.0f);
		else if (m_vRot.x < 0.0f)
			m_vRot.x = min(m_vRot.x + PLAYER_ROT_SPEED_X, 0.0f);
	}
	else if (InputPress(INPUT_UP_R))
	{	// �s�b�`�p�x�������ŉ��Z
		m_vRot.x = min(m_vRot.x + PLAYER_ROT_SPEED_X, PLAYER_ROT_SPEED_MAX_X);
	}
	else if (InputPress(INPUT_DOWN_R))
	{	// �s�b�`�p�x�������Ō��Z
		m_vRot.x = max(m_vRot.x - PLAYER_ROT_SPEED_X, -PLAYER_ROT_SPEED_MAX_X);
	}
	else
	{	// ���͂Ȃ��͊������~�߂�
		if (m_vRot.x > 0.0f)
			m_vRot.x = max(m_vRot.x - PLAYER_ROT_SPEED_X, 0.0f);
		else if (m_vRot.x < 0.0f)
			m_vRot.x = min(m_vRot.x + PLAYER_ROT_SPEED_X, 0.0f);
	}

	// ���[��
	if (InputPress(INPUT_LEFT_R) && InputPress(INPUT_RIGHT_R))
	{	// ���������͊������~�߂�
		if (m_vRot.z > 0.0f)
			m_vRot.z = max(m_vRot.z - PLAYER_ROT_SPEED_Z, 0.0f);
		else if (m_vRot.z < 0.0f)
			m_vRot.z = min(m_vRot.z + PLAYER_ROT_SPEED_Z, 0.0f);
	}
	else if (InputPress(INPUT_RIGHT_R))
	{	// ���[���p�x�������ŉ��Z
		m_vRot.z = min(m_vRot.z + PLAYER_ROT_SPEED_Z, PLAYER_ROT_SPEED_MAX_Z);
	}
	else if (InputPress(INPUT_LEFT_R))
	{	// ���[���p�x�������Ō��Z
		m_vRot.z = max(m_vRot.z - PLAYER_ROT_SPEED_Z, -PLAYER_ROT_SPEED_MAX_Z);
	}
	else
	{	// ���͂Ȃ��͊������~�߂�
		if (m_vRot.z > 0.0f)
			m_vRot.z = max(m_vRot.z - PLAYER_ROT_SPEED_Z, 0.0f);
		else if (m_vRot.z < 0.0f)
			m_vRot.z = min(m_vRot.z + PLAYER_ROT_SPEED_Z, 0.0f);
	}

	// ���[����]���v�Z���A�s�b�`��]�p�̎������߂�
	QuaternionCalculate(&m_vX, &m_vZ, m_vRot.z, &m_vX);

	// �s�b�`��]���v�Z���A���[����]�p�̎������߂�
	QuaternionCalculate(&m_vZ, &m_vX, m_vRot.x, &m_vZ);

	// ���[�J��Y���̃x�N�g�����O�ςŋ��߂�
	CrossProduct(&m_vY, &m_vZ, &m_vX);

	// �ړ��ʂ��Z�o
	m_vMove = m_vZ * m_fMoveSpeed;

	// �ړ��ʂ�K�p
	m_vPos -= m_vMove;

	// �J������At�����f���ɐݒ�
	Camera::SetAt(m_vPos + (m_vY * 30));

	// �J����Up�����f���㕔�ɐݒ�
	//Camera::SetUp(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	Camera::SetUp(m_vY);

	// �J����Eye�����f������ɃZ�b�g
	Camera::SetEye(m_vPos + m_vZ * 100);
	//SetCameraEye(m_vPos - m_vZ * 100 + m_vY * 50);
}

//=============================================================================
// �ړ�����
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
// ���[�h�`�F���W����
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
// �ړ������֐�
//=============================================================================
void Player::MoveFunc(float fAngle)
{
	m_vPos.x += cosf(fAngle) * m_fMoveSpeed;
	m_vPos.z += sinf(fAngle) * m_fMoveSpeed;
}

//=============================================================================
// ��]�����֐�
//=============================================================================
void Player::RotFunc(D3DXVECTOR3 vRot)
{
	//m_vPos.x += sinf(fAngle) * PLAYER_MOVE_SPEED;
	//m_vPos.z += cosf(fAngle) * PLAYER_MOVE_SPEED;
}