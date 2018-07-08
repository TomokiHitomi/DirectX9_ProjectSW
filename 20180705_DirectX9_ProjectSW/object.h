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
	static Object *s_pRoot;
	Object *m_pPrev;
	Object *m_pNext;
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
	// �l�N�X�g�|�C���^�̎擾����
	Object *GetObjectNext(void) { return m_pNext; }
};

//template <typename ObjectType>
//class NewObject
//{
//public:
//	static void Create(void){ Object *temp = new ObjectType; }
//};

//template <typename ObjectType> class CreateObject
//{
//public:
//	void Creat(ObjectType)
//	{
//		ObjectType = new ObjectType;
//	}
//};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

#endif
