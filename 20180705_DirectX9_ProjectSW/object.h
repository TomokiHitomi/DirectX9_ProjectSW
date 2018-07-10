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
public:
	// ���X�g���D�揇��
	enum Priority
	{
		High,
		Normal,
		Low
	};
	enum ObjectID
	{
		NON,
		COPYRIGHT,
		FRAME
	};
private:
	static int		nObjectCount;		// �S�I�u�W�F�N�g���̃J�E���^
	static Object	*s_pRoot;			// ���X�g�̃��[�g�|�C���^
	Object			*m_pPrev;			// ���X�g�̑O�|�C���^
	Object			*m_pNext;			// ���X�g�̎��|�C���^

	Priority		eMainPriority;		// �D�揇�ʁi���C���j
	Priority		eSubPriority;		// �D�揇�ʁi�T�u�j
	ObjectID		eObjectId;			// �I�u�W�F�N�g���ʗpID
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

	// �I�u�W�F�N�gID�ƃv���C�I���e�B�̐ݒ菈��
	void SetIdAndPriority(ObjectID, Priority, Priority);
	// �I�u�W�F�N�g�����X�g�ǉ�����
	void AppendList(void);

	// �S�I�u�W�F�N�g�̍X�V����
	static void UpdateAll(void);
	// �S�I�u�W�F�N�g�̕`�揈��
	static void DrawAll(void);
	// �S�I�u�W�F�N�g�̉������
	static void ReleaseAll(void);

	// ���[�g�|�C���^�̎擾����
	static Object *GetObjectRoot(void) { return s_pRoot; }
	// ���[�g�|�C���^�̃A�h���X�擾����
	static Object **GetObjectRootAdr(void) { return &s_pRoot; }
	// �l�N�X�g�|�C���^�̎擾����
	Object *GetObjectNext(void) { return m_pNext; }

	// �I�u�W�F�N�gID�ݒ菈��
	void SetObjectId(ObjectID eObjId) { eObjectId = eObjId; }
	// ���C���v���C�I���e�B�ݒ菈��
	void SetMainPriority(Priority eMain) { eMainPriority = eMain; }
	// �T�u�v���C�I���e�B�ݒ菈��
	void SetSubPriority(Priority eSub) { eSubPriority = eSub; }

	// �I�u�W�F�N�gID�擾����
	ObjectID GetObjectId(void) { return (eObjectId); }
	// ���C���v���C�I���e�B�擾����
	Priority GetMainPriority(void) { return (eMainPriority); }
	// �T�u�v���C�I���e�B�擾����
	Priority GetSubPriority(void) { return (eSubPriority); }
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

#endif
