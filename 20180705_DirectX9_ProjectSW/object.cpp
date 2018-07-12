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
Object	*Object::s_pRoot[ObjectRootMax] = { NULL, NULL };
//Object	*Object::s_pRootUpdate = NULL;
//Object	*Object::s_pRootDraw = NULL;
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
	// ���[�g�i�[�p�|�C���^
	Object	**pRoot = NULL;

	// ���X�g�\���̍Č�������
	for (unsigned int i = 0; i < ObjectRootMax; i++)
	{
		// ���[�g�|�C���^���擾
		pRoot = GetObjectRootAdr(ObjectRoot(i));

		// �O�|�C���^��NULL�ł͂Ȃ�
		if ((this->m_pPrev[i]) != NULL)
		{
			// �O�|�C���^���w�����|�C���^��
			// ���炪�i�[���Ă��鎟�|�C���^�ɍ����ւ���
			(this->m_pPrev[i])->m_pNext[i] = this->m_pNext[i];
		}
		// �O�|�C���^��NULL
		else
		{	// ���[�g�|�C���^��NULL
			*pRoot = NULL;
		}

		// ���|�C���^��NULL�ł͂Ȃ�
		if (this->m_pNext[i] != NULL)
		{
			// ���|�C���^���w���O�|�C���^��
			// ���炪�i�[���Ă���O�|�C���^�ɍ����ւ���
			this->m_pNext[i]->m_pPrev[i] = this->m_pPrev[i];
		}
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
	for (unsigned int i = 0; i < ObjectRootMax; i++)
	{
		this->m_pPrev[i] = NULL;			// �O�|�C���^�̏�����
		this->m_pNext[i] = NULL;			// ��|�C���^�̏�����
		this->m_ePriority[i] = Middle;
	}

	//this->eUpdatePriority = Normal;	// ���C���v���C�I���e�B��Normal�ŏ�����
	//this->eDrawPriority = Normal;	// �T�u�v���C�I���e�B��Normal�ŏ�����
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

	this->SetObjectId(eObjId);							// �I�u�W�F�N�gID��ݒ�
	this->SetPriority(UpdateRoot, eUpdateP);	// �X�V�v���C�I���e�B��ݒ�
	this->SetPriority(DrawRoot, eDrawP);		// �`��v���C�I���e�B��ݒ�

	// �I�u�W�F�N�g�����X�g�ǉ�����
	AppendList();
}

//=============================================================================
// �I�u�W�F�N�g�����X�g�ǉ�����
//=============================================================================
void Object::AppendList(void)
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
				if ((*pList)->m_ePriority[i] > this->m_ePriority[i])
				{
					// ���荞�ݏ���
					// pList�̑O�|�C���^�Ɏ�����i�[
					(*pList)->m_pPrev[i] = this;
					// pList�����[�g�ȊO
					if (*pList != s_pRoot[i])
					{
						// �O�|�C���^��pPrevTemp��ݒ�
						this->m_pPrev[i] = *pPrevTemp;
					}
					this->m_pNext[i] = *pList;
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
					this->m_pPrev[i] = *pPrevTemp;
				}

				// pList�̎w��Root�܂���Next�Ɏ�����i�[
				*pList = this;
				break;
			}
		}
	}
}

//=============================================================================
// �S�I�u�W�F�N�g�̍X�V����
//=============================================================================
void Object::UpdateAll(void)
{
	Object *pList = Object::GetObjectRoot(UpdateRoot);

#ifdef _DEBUG
	PrintDebugProc("�y ObjectCount : %d �z\n", nObjectCount);
#endif

	while (pList != NULL)
	{
		pList->Update();

#ifdef _DEBUG
		PrintDebugProc("Update[ ID : %d  Pri : %d] \n",
			pList->GetObjectId(), pList->GetPriority(UpdateRoot));
#endif

		pList = pList->GetObjectNext(UpdateRoot);
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