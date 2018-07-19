//=============================================================================
//
// �I�u�W�F�N�g���� [object.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "object.h"

/* �S�̂ŕK�v�ȃC���N���[�h */

/* �I�u�W�F�N�g�ŕK�v�ȃC���N���[�h */

/* Debug */
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// �}�N����`
//*****************************************************************************
Object		*Object::s_pRoot[ObjectRootMax] = { NULL, NULL };
//Object	*Object::s_pRootUpdate = NULL;
//Object	*Object::s_pRootDraw = NULL;
int			Object::nObjectCount = 0;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//=============================================================================
// �X�V����
//=============================================================================


//=============================================================================
// �`�揈��
//=============================================================================

//=============================================================================
// �������
//=============================================================================
void Object::Release(void)
{
	// �I�u�W�F�N�g�̃��X�g���E����
	RemoveList();

	// ���g�̉��
	delete this;

	// �I�u�W�F�N�g����0�̏ꍇ
	if (nObjectCount == 0)
	{
		for (unsigned int i = 0; i < ObjectRootMax; i++)
		{
			// ���[�g�|�C���^��NULL
			s_pRoot[i] = NULL;
		}
	}
}

//=============================================================================
// �R���X�g���N�^�����i�������j
//=============================================================================
Object::Object(void)
{
	// �I�u�W�F�N�g�J�E���^���C���N�������g
	nObjectCount++;

	// �v���p�e�B�̏�����
	for (unsigned int i = 0; i < ObjectRootMax; i++)
	{
		m_pPrev[i] = NULL;			// �O�|�C���^�̏�����
		m_pNext[i] = NULL;			// ��|�C���^�̏�����
		m_ePriority[i] = Middle;	// �D�揇�ʂ� Middle �ŏ�����
	}
}


//=============================================================================
// �f�X�g���N�^�����i�I���j
//=============================================================================
Object::~Object(void)
{
	// �I�u�W�F�N�g�J�E���^���f�N�������g
	nObjectCount--;
}

//=============================================================================
// �I�u�W�F�N�gID�ƃv���C�I���e�B�̐ݒ菈��
// �����FID, �X�V�v���C�I���e�B, �`��v���C�I���e�B
//=============================================================================
void Object::SetIdAndPriority(ObjectID eObjId, Priority eUpdateP, Priority eDrawP)
{
	//// �v���C�I���e�B�e�X�g�p
	//eObjId = ObjectID(nObjectCount);

	SetObjectId(eObjId);				// �I�u�W�F�N�gID��ݒ�
	SetPriority(UpdateRoot, eUpdateP);	// �X�V�v���C�I���e�B��ݒ�
	SetPriority(DrawRoot, eDrawP);		// �`��v���C�I���e�B��ݒ�

	// �I�u�W�F�N�g�����X�g�ǉ�����
	InsertList();
}

//=============================================================================
// �I�u�W�F�N�g�̃��X�g�ǉ�����
//=============================================================================
void Object::InsertList(void)
{
	Object **pList = NULL;
	Object **pPrevTemp = NULL;

	for (unsigned int i = 0; i < ObjectRootMax; i++)
	{
		// ���[�g�|�C���^��pList�Ɋi�[
		pList = GetObjectRootAdr(ObjectRoot(i));

		// ���X�g�\���`�����[�v
		while (true)
		{
			// pList�|�C���^���ݒ�ς݂Ȃ��
			if (*pList != NULL)
			{
				// �v���C�I���e�B������
				if ((*pList)->m_ePriority[i] > m_ePriority[i])
				{
					// ���荞�ݏ���
					// pList�̑O�|�C���^�Ɏ�����i�[
					(*pList)->m_pPrev[i] = this;
					// pList�����[�g�ȊO
					if (*pList != s_pRoot[i])
					{
						// �O�|�C���^��pPrevTemp��ݒ�
						m_pPrev[i] = *pPrevTemp;
					}
					m_pNext[i] = *pList;
					// pList�Ɏ�����i�[
					*pList = this;
					break;
				}

				// �O�|�C���^���Ƃ���pList��ۊ�
				pPrevTemp = pList;

				// ���|�C���^��pList�Ɋi�[���Ȃ���
				pList = &(*pList)->m_pNext[i];
			}
			// ���|�C���^�����ݒ�Ȃ��
			else if (*pList == NULL)
			{
				// pList�����[�g�ȊO
				if (*pList != s_pRoot[i])
				{
					// �O�|�C���^��pPrevTemp��ݒ�
					m_pPrev[i] = *pPrevTemp;
				}

				// pList�̎w��Root�܂���Next�Ɏ�����i�[
				*pList = this;
				break;
			}
		}
	}
}

//=============================================================================
// �I�u�W�F�N�g�̃��X�g���E����
//=============================================================================
void Object::RemoveList(void)
{
	// ���[�g�i�[�p�|�C���^
	Object	**pRoot = NULL;

	// ���X�g�\���̍Č�������
	for (unsigned int i = 0; i < ObjectRootMax; i++)
	{
		// ���[�g�|�C���^���擾
		pRoot = GetObjectRootAdr(ObjectRoot(i));

		// �O�|�C���^��NULL�ł͂Ȃ�
		if ((m_pPrev[i]) != NULL)
		{
			// �O�|�C���^���w�����|�C���^��
			// ���炪�i�[���Ă��鎟�|�C���^�ɍ����ւ���
			(m_pPrev[i])->m_pNext[i] = m_pNext[i];
		}
		// �O�|�C���^��NULL
		else
		{
			// ���[�g�|�C���^�����|�C���^�ɍ����ւ���
			*pRoot = m_pNext[i];
		}

		// ���|�C���^��NULL�ł͂Ȃ�
		if (m_pNext[i] != NULL)
		{
			// ���|�C���^���w���O�|�C���^��
			// ���炪�i�[���Ă���O�|�C���^�ɍ����ւ���
			m_pNext[i]->m_pPrev[i] = m_pPrev[i];
		}
	}
}

//=============================================================================
// �I�u�W�F�N�g�̃|�C���^�擾����
//=============================================================================
Object *Object::GetObjectPointer(ObjectID eObjId)
{
	Object *pList = Object::GetObjectRoot(UpdateRoot);

	while (pList != NULL)
	{
		if (eObjId == pList->GetObjectId())
		{
			return pList;
		}
		pList = pList->GetObjectNext(UpdateRoot);
	}
	return NULL;
}

//=============================================================================
// �S�I�u�W�F�N�g�̍X�V����
//=============================================================================
void Object::UpdateAll(void)
{

#ifdef _DEBUG
	PrintDebugProc("�y Object �z\n");
	PrintDebugProc("ObjectCount [%d]\n", nObjectCount);
	PrintDebugProc("\n");

	Object *pList = Object::GetObjectRoot(UpdateRoot);

	while (pList != NULL)
	{
		PrintDebugProc("Update[ ID : %d  Pri : %d] \n",
			pList->GetObjectId(), pList->GetPriority(UpdateRoot));
		pList = pList->GetObjectNext(UpdateRoot);
	}
	PrintDebugProc("\n");
#endif


	pList = Object::GetObjectRoot(UpdateRoot);

	while (pList != NULL)
	{
		pList->Update();
		pList = pList->GetObjectNext(UpdateRoot);
	}
}

//=============================================================================
// �S�I�u�W�F�N�g�̕`�揈��
//=============================================================================
void Object::DrawAll(void)
{
	Object *pList = Object::GetObjectRoot(DrawRoot);

	
	while (pList != NULL)
	{
		pList->Draw();
#ifdef _DEBUG
		PrintDebugProc(" Draw [ ID : %d  Pri : %d] \n",
			pList->GetObjectId(), pList->GetPriority(DrawRoot));
#endif
		pList = pList->GetObjectNext(DrawRoot);
	}
#ifdef _DEBUG
	PrintDebugProc("\n");
#endif
}

//=============================================================================
// �S�I�u�W�F�N�g�̉������
//=============================================================================
void Object::ReleaseAll(void)
{
	Object *pList = NULL;
	Object *pNext = NULL;
	for (unsigned int i = 0; i < ObjectRootMax; i++)
	{
		// ���[�g�|�C���^��pList�Ɋi�[
		pList = GetObjectRoot(ObjectRoot(i));

		// NULL������܂�pList�����ǂ�
		while (pList != NULL)
		{
			pNext = pList->GetObjectNext(ObjectRoot(i));
			pList->Release();
			pList = pNext;
		}
	}
}

//=============================================================================
// ���[�g�|�C���^�̃A�h���X�擾����
//=============================================================================
Object **Object::GetObjectRootAdr(ObjectRoot eObjRoot)
{
	return &s_pRoot[eObjRoot];
}

//=============================================================================
// �l�N�X�g�|�C���^�̎擾����
//=============================================================================
Object *Object::GetObjectNext(ObjectRoot eObjRoot)
{ 
	return m_pNext[eObjRoot];
}