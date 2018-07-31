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
	m_fRiseSpeed = 0.0f;
	m_bUse = true;

	// ���f���̏�����
	//m_CSkinMesh->Init(pDevice, "data/MODEL/Yuko.x");
	m_CSkinMesh = new CSkinMesh;
	m_CSkinMesh->Init(pDevice, PLAYER_MODEL);
	
	D3DXMATRIX mtxTranslate;

	D3DXMatrixIdentity(&m_mtxWorld);

	// �J������At�����f���ɐݒ�
	Camera::SetAt(m_vPos + (m_vY * 30));

	// �J����Up�����f���㕔�ɐݒ�
	//Camera::SetUp(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	Camera::SetUp(m_vY);

	// �J����Eye�����f������ɃZ�b�g
	Camera::SetEye(m_vPos + m_vZ * 100);

	m_CXTexture2.data.vPos = D3DXVECTOR2(1920 / 2, 1080 / 2);
	m_CXTexture2.data.vSize = D3DXVECTOR2(1920 / 2, 1080 / 2);
	m_CXTexture2.data.nDivide.x = 1;
	m_CXTexture2.data.nDivide.y = 1;
	m_CXTexture2.data.xColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_CXTexture2.data.nTexNum = 0;

	// �e�N�X�`���̏�����
	m_CXTexture2.Init(pDevice, PLAYER_MODEL_TEST);
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
		case MODE_LOCKON:
			LOCKON();	// �Ə�����
			break;
		case MODE_CHANGE:
			Change();	// �J�ڏ���
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
		// Z�o�b�t�@�ւ̏������݋֎~
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		// �X�e���V���e�X�g����
		pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
		// �X�e���V���}�X�N�̐ݒ� �f�[�^�͂��̂܂܎g��
		pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);
		// �X�e���V���e�X�g�ɋ����I�ɍ��i�i�K���������ށj
		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
		// �o�b�N�o�b�t�@��RGB�l���������܂Ȃ�
		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x00);
		// �X�e���V���e�X�g�ɕs���i�̏ꍇ�̓X�e���V���l�ɉ������Ȃ�
		pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILCAPS_KEEP);
		// Z�o�b�t�@�ɕs���i�̏ꍇ�̓X�e���V���l�ɂȂɂ����Ȃ�
		pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILCAPS_KEEP);
		// �X�e���V���e�X�g�ɍ��i�̏ꍇ�X�e���V���l��1���₷
		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILCAPS_INCRSAT);


		// ���e�X�g��L����
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, PLAYER_ALPHA_TEST);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		// ���ʕ`�悷��
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		// ���f����`��-
		m_CSkinMesh->Draw(pDevice);

		// ���ʂ��J�����O�ɖ߂�
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		// ���e�X�g�𖳌���
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);



		// �X�e���V���o�b�t�@�̒l�Ɣ�r����Q�ƒl
		pDevice->SetRenderState(D3DRS_STENCILREF, 0x09);
		// ��r�֐��A�������^�̎��X�e���V���e�X�g���i
		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL);
		// �X�e���V���e�X�g�ɍ��i�����ꍇ�X�e���V���l�ɉ������Ȃ�
		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILCAPS_KEEP);
		// �t���[���o�b�t�@��RGB���������߂�悤�ɂ���
		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE,
			D3DCOLORWRITEENABLE_RED |
			D3DCOLORWRITEENABLE_GREEN |
			D3DCOLORWRITEENABLE_BLUE |
			D3DCOLORWRITEENABLE_ALPHA);
		// Z�o�b�t�@�ւ̏������݂�������
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		m_CXTexture2.Draw();
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

	// �}�E�X
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



	// �s�b�`
	//if (InputPress(INPUT_UP_R) && InputPress(INPUT_DOWN_R))
	//{	// ���������͊������~�߂�
	//	if (m_vRot.x > 0.0f)
	//		m_vRot.x = max(m_vRot.x - PLAYER_ROT_SPEED_X, 0.0f);
	//	else if (m_vRot.x < 0.0f)
	//		m_vRot.x = min(m_vRot.x + PLAYER_ROT_SPEED_X, 0.0f);
	//}
	//else if (InputPress(INPUT_UP_R))
	//{	// �s�b�`�p�x�������ŉ��Z
	//	m_vRot.x = min(m_vRot.x + PLAYER_ROT_SPEED_X, PLAYER_ROT_SPEED_MAX_X);
	//}
	//else if (InputPress(INPUT_DOWN_R))
	//{	// �s�b�`�p�x�������Ō��Z
	//	m_vRot.x = max(m_vRot.x - PLAYER_ROT_SPEED_X, -PLAYER_ROT_SPEED_MAX_X);
	//}
	//else
	//{	// ���͂Ȃ��͊������~�߂�
	//	if (m_vRot.x > 0.0f)
	//		m_vRot.x = max(m_vRot.x - PLAYER_ROT_SPEED_X, 0.0f);
	//	else if (m_vRot.x < 0.0f)
	//		m_vRot.x = min(m_vRot.x + PLAYER_ROT_SPEED_X, 0.0f);
	//}

	//// ���[��
	//if (InputPress(INPUT_LEFT_R) && InputPress(INPUT_RIGHT_R))
	//{	// ���������͊������~�߂�
	//	if (m_vRot.z > 0.0f)
	//		m_vRot.z = max(m_vRot.z - PLAYER_ROT_SPEED_Z, 0.0f);
	//	else if (m_vRot.z < 0.0f)
	//		m_vRot.z = min(m_vRot.z + PLAYER_ROT_SPEED_Z, 0.0f);
	//}
	//else if (InputPress(INPUT_RIGHT_R))
	//{	// ���[���p�x�������ŉ��Z
	//	m_vRot.z = min(m_vRot.z + PLAYER_ROT_SPEED_Z, PLAYER_ROT_SPEED_MAX_Z);
	//}
	//else if (InputPress(INPUT_LEFT_R))
	//{	// ���[���p�x�������Ō��Z
	//	m_vRot.z = max(m_vRot.z - PLAYER_ROT_SPEED_Z, -PLAYER_ROT_SPEED_MAX_Z);
	//}
	//else
	//{	// ���͂Ȃ��͊������~�߂�
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

	// ���[�J��Y���̃x�N�g�����O�ςŋ��߂�
	CrossProduct(&m_vX, &m_vY, &m_vZ);


	//m_vX *= 2.0f;
	//m_vZ *= 2.0f;
	//vAxis *= 2.0f;

	//D3DXVec3Normalize(&m_vX, &m_vX);
	//D3DXVec3Normalize(&m_vZ, &m_vZ);
	//D3DXVec3Normalize(&vAxis, &vAxis);



	//// �s�b�`��]���v�Z���A���[����]�p�̎������߂�
	//QuaternionCalculate(&m_vZ, &m_vX, m_vRot.x, &m_vZ);

	//// ���[�J��Y���̃x�N�g�����O�ςŋ��߂�
	//CrossProduct(&m_vY, &m_vZ, &m_vX);

	//// �s�b�`��]���v�Z���A���[����]�p�̎������߂�
	//QuaternionCalculate(&m_vZ, &m_vY, m_vRot.z, &m_vZ);

	//// ���[�J��Y���̃x�N�g�����O�ςŋ��߂�
	//CrossProduct(&m_vX, &m_vY, &m_vZ);



	//D3DXVec3Normalize(&m_vX, &m_vX);
	//D3DXVec3Normalize(&m_vZ, &m_vZ);
	//D3DXVec3Normalize(&vAxis, &vAxis);


	//m_vZ = vEye;

#ifdef _DEBUG
	PrintDebugProc("�y Float �z\n");
	PrintDebugProc("vX  [%f,%f,%f]\n", m_vX.x, m_vX.y, m_vX.z);
	PrintDebugProc("vZ  [%f,%f,%f]\n", m_vZ.x, m_vZ.y, m_vZ.z);
	PrintDebugProc("Axis[%f,%f,%f]\n", vAxis.x, vAxis.y, vAxis.z);
	//PrintDebugProc("Axis[%f,%f,%f]\n", vAxis.x, vAxis.y, vAxis.z);
#endif

	// �ړ�����
	if (InputPress(INPUT_LEFT))
	{
		if (InputPress(INPUT_UP))
		{// ���O�ړ�
			//MoveFunc(m_fHAngle + D3DX_PI * 0.25f);
			m_vPos += (m_vX - m_vZ) * 0.71f * m_fMoveSpeed;
		}
		else if (InputPress(INPUT_DOWN))
		{// ����ړ�
			//MoveFunc(m_fHAngle + D3DX_PI * 0.75f);
			m_vPos += (m_vX + m_vZ) * 0.71f * m_fMoveSpeed;
		}
		else if (InputPress(INPUT_RIGHT))
		{// ���E���������͏����Ȃ�
		}
		else
		{// ���ړ�
			//MoveFunc(m_fHAngle + D3DX_PI * 0.50f);
			m_vPos += m_vX * m_fMoveSpeed;
		}
	}
	else if (InputPress(INPUT_RIGHT))
	{
		if (InputPress(INPUT_UP))
		{// �E�O�ړ�
			//MoveFunc(m_fHAngle - D3DX_PI * 0.25f);
			m_vPos -= (m_vX + m_vZ) * 0.71f * m_fMoveSpeed;
		}
		else if (InputPress(INPUT_DOWN))
		{// �E��ړ�
			//MoveFunc(m_fHAngle - D3DX_PI * 0.75f);
			m_vPos -= (m_vX - m_vZ) * 0.71f * m_fMoveSpeed;
		}
		else
		{// �E�ړ�
			//MoveFunc(m_fHAngle - D3DX_PI * 0.50f);
			m_vPos -= m_vX * m_fMoveSpeed;
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

	m_vPos.y += m_fRiseSpeed;

	// �ړ���K�p
	//m_vPos += m_vMove;


	// �J������At�����f���ɐݒ�
	Camera::SetAt(m_vPos + (m_vY * 30));

	// �J����Eye�����f������ɃZ�b�g
	Camera::SetEye(m_vPos + (m_vY * 30) + m_vZ * 100);

	// �J�������������炷
	Camera::AddEyeIner(0.02f);
	Camera::AddAtIner(0.02f);

	// �J����Up�����f���㕔�ɐݒ�
	Camera::SetUp(m_vY);
	//Camera::SetUp(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	
	//Cube::SetPos(m_vPos - m_vZ * 100);
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
	else if (InputPress(INPUT_DOWN_R))
	{	// �s�b�`�p�x�������ŉ��Z
		m_vRot.x = min(m_vRot.x + PLAYER_ROT_SPEED_X, PLAYER_ROT_SPEED_MAX_X);
	}
	else if (InputPress(INPUT_UP_R))
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

	m_vRotIner.x = GetMobUseY() * 0.00006f;
	m_vRotIner.z = GetMobUseX() * 0.00006f;
	m_vRot += m_vRotIner;

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

	m_vUp = m_vY;

	// �J������At�����f���ɐݒ�
	Camera::SetAt(m_vPos + (m_vY * 30));

	// �J����Up�����f���㕔�ɐݒ�
	//Camera::SetUp(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	Camera::SetUp(m_vY);

	Camera::AddAtIner(0.02f);

	// �J����Eye�����f������ɃZ�b�g
	Camera::SetEye(m_vPos + m_vZ * 100);
	//SetCameraEye(m_vPos - m_vZ * 100 + m_vY * 50);
}

//=============================================================================
// ���V����
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

	// �x�x�N�g������Ɍ�����
	m_vY = m_vY + (D3DXVECTOR3(0.0f, 1.0f, 0.0f) - m_vY) * 0.1f;

	// �Ώۍ��W�Ǝ���̍��W�����Z
	D3DXVECTOR3 vLook = ZERO_D3DXVECTOR3 - m_vPos;
	vLook *= -1.0f;

	D3DXVec3Normalize(&vLook, &vLook);


	m_vZ = m_vZ + (vLook - m_vZ) * 0.1f;


	// ���炪�����x�N�g���𐳋K������
	D3DXVec3Normalize(&m_vZ, &m_vZ);

	//m_vZ *= -1.0f;

	// ���[�J��X���̃x�N�g�����O�ςŋ��߂�
	CrossProduct(&m_vX, &m_vY, &m_vZ);

	// X���𐳋K������
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

	//// ���[�J��Y���̃x�N�g�����O�ςŋ��߂�
	//CrossProduct(&m_vX, &m_vY, &m_vZ);


#ifdef _DEBUG
	PrintDebugProc("�y Float �z\n");
	PrintDebugProc("vX  [%f,%f,%f]\n", m_vX.x, m_vX.y, m_vX.z);
	PrintDebugProc("vZ  [%f,%f,%f]\n", m_vZ.x, m_vZ.y, m_vZ.z);
	//PrintDebugProc("Axis[%f,%f,%f]\n", vAxis.x, vAxis.y, vAxis.z);
	//PrintDebugProc("Axis[%f,%f,%f]\n", vAxis.x, vAxis.y, vAxis.z);
#endif

	// �ړ�����
	if (InputPress(INPUT_LEFT))
	{
		if (InputPress(INPUT_UP))
		{// ���O�ړ�
		 //MoveFunc(m_fHAngle + D3DX_PI * 0.25f);
			m_vPos += (m_vX - m_vZ) * 0.71f * m_fMoveSpeed;
		}
		else if (InputPress(INPUT_DOWN))
		{// ����ړ�
		 //MoveFunc(m_fHAngle + D3DX_PI * 0.75f);
			m_vPos += (m_vX + m_vZ) * 0.71f * m_fMoveSpeed;
		}
		else if (InputPress(INPUT_RIGHT))
		{// ���E���������͏����Ȃ�
		}
		else
		{// ���ړ�
		 //MoveFunc(m_fHAngle + D3DX_PI * 0.50f);
			m_vPos += m_vX * m_fMoveSpeed;
		}
	}
	else if (InputPress(INPUT_RIGHT))
	{
		if (InputPress(INPUT_UP))
		{// �E�O�ړ�
		 //MoveFunc(m_fHAngle - D3DX_PI * 0.25f);
			m_vPos -= (m_vX + m_vZ) * 0.71f * m_fMoveSpeed;
		}
		else if (InputPress(INPUT_DOWN))
		{// �E��ړ�
		 //MoveFunc(m_fHAngle - D3DX_PI * 0.75f);
			m_vPos -= (m_vX - m_vZ) * 0.71f * m_fMoveSpeed;
		}
		else
		{// �E�ړ�
		 //MoveFunc(m_fHAngle - D3DX_PI * 0.50f);
			m_vPos -= m_vX * m_fMoveSpeed;
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
	Camera::SetAt(ZERO_D3DXVECTOR3);

	// �J����Eye�����f������ɃZ�b�g
	Camera::SetEye(m_vPos + (m_vY * 30) + m_vZ * 100);

	// �J�������������炷
	Camera::AddEyeIner(0.05f);
	Camera::AddAtIner(0.02f);

	// �J����Up�����f���㕔�ɐݒ�
	Camera::SetUp(m_vY);
	//Camera::SetUp(D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	//Cube::SetPos(m_vPos - m_vZ * 100);
}

//=============================================================================
// �ړ�����
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

	// �J������At�����f���ɐݒ�
	Camera::SetAt(m_vPos + (m_vY * 30));

	// �J����Eye�����f������ɃZ�b�g
	Camera::SetEye(m_vPos + (m_vY * 30) + m_vZ * 200);


	// �J����Up�����f���㕔�ɐݒ�
	Camera::SetUp(m_vY);
	//Camera::SetUp(D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	Cube::SetPos(m_vPos - m_vZ * 100);
}


//=============================================================================
// �ړ�����
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
	if (GetKeyboardTrigger(DIK_TAB))
	{
		m_eMode = MODE_LOCKON;
	}
	if (m_eMode != m_eModeOld)
	{
		switch (m_eMode)
		{
		case MODE_FLOAT:
			//m_vRot = D3DXVECTOR3(PLAYER_FLOAT_ROT_X,0.0f,0.0f);
			m_vRot = D3DXVECTOR3(PLAYER_FLOAT_ROT_X - atan2(m_vZ.y, 1.0f), -atan2(m_vZ.x, m_vZ.z),0.0f);
			m_vRotIner = m_vRot;
			Camera::SetEyeIner(0.01f);
			Camera::SetAtIner(0.05f);
			break;
		case MODE_FLY:
			m_vRot = ZERO_D3DXVECTOR3;
			Camera::SetEyeIner(0.05f);
			Camera::SetAtIner(0.05f);
			break;
		case MODE_LOCKON:
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