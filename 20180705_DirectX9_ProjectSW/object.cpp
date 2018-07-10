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
Object	*Object::s_pRoot = NULL;
int		Object::nObjectCount = 0;

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
	// ���X�g�\���̍Č�������

	// �O�|�C���^��NULL�ł͂Ȃ�
	if (this->m_pPrev != NULL)
	{
		// �O�|�C���^���w�����|�C���^��
		// ���炪�i�[���Ă��鎟�|�C���^�ɍ����ւ���
		this->m_pPrev->m_pNext = this->m_pNext;
	}
	// �O�|�C���^��NULL
	else
	{	// ���[�g�|�C���^��NULL
		s_pRoot = NULL;
	}

	// ���|�C���^��NULL�ł͂Ȃ�
	if (this->m_pNext != NULL)
	{
		// ���|�C���^���w���O�|�C���^��
		// ���炪�i�[���Ă���O�|�C���^�ɍ����ւ���
		this->m_pNext->m_pPrev = this->m_pPrev;
	}

	delete this;
}

//=============================================================================
// �R���X�g���N�^�����i�������j
//=============================================================================
Object::Object(void)
{
	// �I�u�W�F�N�g�J�E���^���C���N�������g
	nObjectCount++;

	// �v���p�e�B�̏�����
	this->m_pPrev = NULL;			// �O�|�C���^�̏�����
	this->m_pNext = NULL;			// ��|�C���^�̏�����

	this->eMainPriority = Normal;	// ���C���v���C�I���e�B��Normal�ŏ�����
	this->eSubPriority = Normal;	// �T�u�v���C�I���e�B��Normal�ŏ�����
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
//=============================================================================
void Object::SetIdAndPriority(ObjectID eObjId, Priority eMain, Priority eSub)
{
	//// �v���C�I���e�B�e�X�g�p
	eSub = Priority(11 - Priority(nObjectCount) - eSub);
	eObjId = ObjectID(nObjectCount);

	this->SetObjectId(eObjId);		// �I�u�W�F�N�gID��ݒ�
	this->SetMainPriority(eMain);	// ���C���v���C�I���e�B��ݒ�
	this->SetSubPriority(eSub);		// �T�u�v���C�I���e�B��ݒ�

	// �I�u�W�F�N�g�����X�g�ǉ�����
	AppendList();
}

//=============================================================================
// �I�u�W�F�N�g�����X�g�ǉ�����
//=============================================================================
void Object::AppendList(void)
{
	// ���[�g�|�C���^��pList�Ɋi�[
	Object **pList = GetObjectRootAdr();
	Object **pPrevTemp = NULL;

	// ���X�g�\���`�����[�v
	while (true)
	{
		// pList�|�C���^���ݒ�ς݂Ȃ��
		if (*pList != NULL)
		{
			// ���C���v���C�I���e�B�������A�܂��͓���
			if ((*pList)->eMainPriority >= this->eMainPriority)
			{
				// ���C���v���C�I���e�B�������A���T�u�v���C�I���e�B���Ⴂ
				if ((*pList)->eMainPriority == this->eMainPriority
					&& (*pList)->eSubPriority < this->eSubPriority)
				{
					// �������Ȃ�
				}
				else
				{
					// ���荞�ݏ���
					// pList�̑O�|�C���^�Ɏ�����i�[
					(*pList)->m_pPrev = this;
					// pList�����[�g�ȊO
					if (*pList != s_pRoot)
					{
						// �O�|�C���^��pPrevTemp��ݒ�
						this->m_pPrev = *pPrevTemp;
					}
					this->m_pNext = *pList;
					// pList�Ɏ�����i�[
					*pList = this;
					return;
				}
			}

			// �O�|�C���^���Ƃ���pList��ۊ�
			pPrevTemp = pList;

			// ���|�C���^��pList�Ɋi�[���Ȃ���
			pList = &(*pList)->m_pNext;
		}
		// ���|�C���^�����ݒ�Ȃ��
		else if (*pList == NULL)
		{
			// pList�����[�g�ȊO
			if (*pList != s_pRoot)
			{
				// �O�|�C���^��pPrevTemp��ݒ�
				this->m_pPrev = *pPrevTemp;
			}

			// pList�̎w��Root�܂���Next�Ɏ�����i�[
			*pList = this;
			return;
		}
	}
}

//=============================================================================
// �S�I�u�W�F�N�g�̍X�V����
//=============================================================================
void Object::UpdateAll(void)
{
	Object *pList = Object::GetObjectRoot();

#ifdef _DEBUG
	PrintDebugProc("�y ObjectCount : %d �z\n", nObjectCount);
#endif

	while (pList != NULL)
	{
		pList->Update();

#ifdef _DEBUG
		PrintDebugProc("[ ID : %d  PriMain : %d  PriSub : %d] \n",
			pList->GetObjectId(), pList->GetMainPriority(),pList->GetSubPriority());
#endif

		pList = pList->GetObjectNext();
	}
#ifdef _DEBUG
	PrintDebugProc("\n");
#endif
}

//=============================================================================
// �S�I�u�W�F�N�g�̕`�揈��
//=============================================================================
void Object::DrawAll(void)
{
	Object *pList = Object::GetObjectRoot();

	
	while (pList != NULL)
	{
		pList->Draw();
		pList = pList->GetObjectNext();
	}
}

//=============================================================================
// �S�I�u�W�F�N�g�̉������
//=============================================================================
void Object::ReleaseAll(void)
{
	Object *pList = Object::GetObjectRoot();
	Object *pNext = NULL;

	while (pList != NULL)
	{
		pNext = pList->GetObjectNext();
		pList->Release();
		pList = pNext;
	}
}