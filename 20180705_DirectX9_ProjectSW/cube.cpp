//=============================================================================
//
// �X�J�C�h�[������ [cube.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "main.h"
#include "cube.h"
#include "calculate.h"
#include "joycon.h"

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
Cube::Prop		Cube::m_tagProp;


//=============================================================================
// �R���X�g���N�^�i�����������j
//=============================================================================
Cube::Cube(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �I�u�W�F�N�gID�ƃv���C�I���e�B�̐ݒ菈��
	SetIdAndPriority(ObjectID::CUBE, Priority::Low, Priority::Middle);

	// �e�v���p�e�B�̏�����
	m_tagProp.vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_tagProp.vRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_tagProp.vScl = D3DXVECTOR3(MODEL_CUBE_SCL, MODEL_CUBE_SCL, MODEL_CUBE_SCL);
	m_tagProp.bUse = true;

	// ���f���̏�����
	//m_CSkinMesh->Init(pDevice, "data/MODEL/Yuko.x");
	m_tagProp.CXModel = new CXModel;
	m_tagProp.CXModel->Init(pDevice, CUBE_MODEL, CUBE_TEXTURE);

}

//=============================================================================
// �f�X�g���N�^�i�I�������j
//=============================================================================
Cube::~Cube(void)
{
	if (m_tagProp.CXModel != NULL)
	{
		m_tagProp.CXModel->Release();
		delete m_tagProp.CXModel;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void Cube::Update(void)
{
	if (m_tagProp.bUse)
	{
		// �A�j���[�V�����X�V����
		m_tagProp.CXModel->Update();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void Cube::Draw(void)
{
	if (m_tagProp.bUse)
	{
		//// joycon�e�X�g
		//D3DXVECTOR3 vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//D3DXVECTOR3 vecTemp = GetJoyconAccel(0);
		//D3DXVec3Normalize(&vecTemp, &vecTemp);

		////WorldConvertAxis(&m_tagProp.mtxWorld, m_tagProp.vPos, vecTemp,
		////	vecUp, m_tagProp.vScl);

		//PrintDebugProc("Cube [X:%f Y:%f Z:%f]\n",
		//	vecTemp.x, vecTemp.y, vecTemp.z);

		//m_tagProp.vPos += vecTemp;
		// ���[���h�ϊ�
		WorldConvert(&m_tagProp.mtxWorld,
			m_tagProp.vPos, m_tagProp.vRot, m_tagProp.vScl);

		m_tagProp.CXModel->Draw(m_tagProp.mtxWorld);
	}
}