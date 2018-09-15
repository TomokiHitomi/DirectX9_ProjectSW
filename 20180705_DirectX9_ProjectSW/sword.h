//=============================================================================
//
// �\�[�h���� [sword.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _SWORD_H_
#define _SWORD_H_

/*******************************************************************************
* �C���N���[�h
*******************************************************************************/
#include "object.h"
#include "XModel.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// ���f��
#define SWORD_MODEL					("sword.X")
#define SWORD_MODEL_TEX				("UV_Yat.tga")

#define SWORD_MAX					(50)
#define SWORD_MAX_INIT				(8)
#define SWORD_SCL					(0.5f)

#define SWORD_SPEED_MIN				(8.0f)
#define SWORD_SPEED_MAX				(8.0f)
#define SWORD_SPEED_RETRUN			(8.0f)
#define SWORD_SPEED_ADD				(5)


//#define SWORD_ROT_SPEED			(0.02f)
#define SWORD_ROT_SPEED_MIN			(-0.1f)
#define SWORD_ROT_SPEED_COUNT		(30)
#define SWORD_ROT_SPEED_MAX			(0.03f)
#define SWORD_ROT_SPEED_ADD			(20)
#define SWORD_ROT_SPEED_SUB			(3)

#define SWORD_ROT_LENGTH			(10.0f)
#define SWORD_ROT_ADJUST_TIME		(15)

#define SWORD_TRUN					(0.1f)
#define SWORD_LENGTH_RETRUN			(200.0f)
#define SWORD_LENGTH_RETRUN_SUB		(3.5f)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************


class Sword
{
public:

	// �R���X�g���N�^�i�����������j
	Sword(void);
	//�f�X�g���N�^�i�I�������j
	~Sword(void);

	// �X�V����
	void	Update(void);
	// �`�揈��
	void	Draw(void);
private:
	CXModel			*m_CXModel;			// ���f���i�[�p
	class SwordData
	{
	public:
		enum Mode
		{
			STAY,
			SHOT,
			RETURN
		};
		D3DXVECTOR3			vPos;			// ���W���

		// ���[�J���x�N�g��
		D3DXVECTOR3			vX;
		D3DXVECTOR3			vY;
		D3DXVECTOR3			vZ;
		SwordData::Mode		eMode;
		float				fSpeed;
		float				fLength;
		float				fAngle;
		bool				bUse;
	};

	enum Mode
	{
		SHOT,
		ROT,
		RETURN
	};
	Sword::Mode		m_eMode;

	SwordData		m_tagSword[SWORD_MAX];
	D3DXVECTOR3		m_vScl;				// �g�k���
	int				m_nMax;

	// �N�H�[�^�j�I���p
	D3DXVECTOR3		m_vAxis;
	D3DXVECTOR3		m_vRadian;

	int				m_nCnt;
	int				m_nCntMax;
	int				m_nCntRot;

	float			m_fAngle;
	float			m_fLength;
	float			m_fRot;
	float			m_fRotSpeed;

	bool			m_bUse;
	void	Shot(void);
	void	Stay(void);
	void	StayAll(void);
	void	RetrunAll(void);
	void	Add(void);
	void	Sub(void);
	void	Rot(void);
};

	//Sword	m_cSword;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

#endif
