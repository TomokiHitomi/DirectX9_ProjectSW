//=============================================================================
//
// �I�u�W�F�N�g���� [object.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

/*******************************************************************************
* �C���N���[�h
*******************************************************************************/
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************

// �I�u�W�F�N�g�p
class Object
{
private:
	enum Priority
	{
		High,
		Normal,
		Low
	};
	static int		nObjectCount;
	static Object	*s_pRoot;
	Object			*m_pPrev;
	Object			*m_pNext;
	Priority		eMainPriority;
	Priority		eSubPriority;
public:

	// �X�V����
	virtual void Update(void) {}
	// �`�揈��
	virtual void Draw(void) {}
	// �������
	void Release(void);

	// �R���X�g���N�^�i�����������j
	Object();
	// �f�X�g���N�^�i�I�������j
	virtual ~Object();

	//void

	// ���C���v���C�I���e�B�ݒ菈��
	void SetMainPriority(Priority eMain) { eMainPriority = eMain; }
	// �T�u�v���C�I���e�B�ݒ菈��
	void SetSubPriority(Priority eSub) { eSubPriority = eSub; }

	// ���C���v���C�I���e�B�擾����
	Priority GetMainPriority(void) { return (eMainPriority); }
	// �T�u�v���C�I���e�B�擾����
	Priority GetSubPriority(void) { return (eSubPriority); }


	// �S�I�u�W�F�N�g�̍X�V����
	static void UpdateAll(void);
	// �S�I�u�W�F�N�g�̕`�揈��
	static void DrawAll(void);
	// �S�I�u�W�F�N�g�̉������
	static void ReleaseAll(void);

	// �I�u�W�F�N�g��������
	static void Create(Object*) {}
	// ���[�g�|�C���^�̎擾����
	static Object *GetObjectRoot(void) { return s_pRoot; }
	// ���[�g�|�C���^�̃A�h���X�擾����
	static Object **GetObjectRootAdr(void) { return &s_pRoot; }
	// �l�N�X�g�|�C���^�̎擾����
	Object *GetObjectNext(void) { return m_pNext; }
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

#endif
