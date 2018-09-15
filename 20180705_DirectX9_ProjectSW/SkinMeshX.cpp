//=============================================================================
//
// �A�j���[�V�������f������ [SkinMeshX.cpp]
// Author : GP12A295 25 �l���F��
//
// �Q�l�FGES�u���O
// http://ges.blog.shinobi.jp/directx/directx%209%E3%80%80%E3%82%B9%E3%82%AD%E3%83%B3%E3%83%A1%E3%83%83%E3%82%B7%E3%83%A5%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "SkinMeshX.h"

// �f�o�b�O�p
#ifdef _DEBUG
#include "debugproc.h"
#endif

static int testCreate = 0;
static int testDestroy = 0;

CHAR* HeapCopy(CHAR* sName)
{
	DWORD dwLen = (DWORD)strlen(sName) + 1;
	CHAR* sNewName = new CHAR[dwLen];
	if (sNewName)
		strcpy_s(sNewName, dwLen, sName);
	return sNewName;
}

//=============================================================================
// �t���[���i�[�֐�
//=============================================================================
HRESULT MY_HIERARCHY::CreateFrame(LPCTSTR Name, LPD3DXFRAME *ppNewFrame)
{
	HRESULT hr = S_OK;
	MYFRAME *pFrame;
	//�V�����t���[���A�h���X�i�[�p�ϐ���������
	*ppNewFrame = NULL;
	//�t���[���̗̈�m��
	pFrame = new MYFRAME;
	//�̈�m�ۂ̎��s���̏���
	if (pFrame == NULL)
	{
		return E_OUTOFMEMORY;
	}
	ZeroMemory(pFrame, sizeof(MYFRAME));


	//�t���[�����i�[�p�̈�m��
	if (Name)
		pFrame->Name = (CHAR *)HeapCopy((CHAR *)Name);
	else
	{
		// TODO: Add a counter to append to the string below
		//       so that we are using a different name for
		//       each bone.
		pFrame->Name = NULL;
	}

	////�t���[�����i�[�p�̈�m��
	//pFrame->Name = new TCHAR[lstrlen(Name) + 1];
	////�̈�m�ۂ̎��s���̏���
	//if (!pFrame->Name)
	//{
	//	return E_FAIL;
	//}
	// //strcpy(pFrame->Name,Name);
	////�t���[�����i�[
	//strcpy_s(pFrame->Name, lstrlen(Name) + 1, Name);

	//�s��̏�����
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);
	//�ǉ��F�I�t�Z�b�g�֌W������
	pFrame->OffsetID = 0xFFFFFFFF;
	D3DXMatrixIdentity(&(pFrame->OffsetMat));
	//�V�K�t���[���̃��b�V���R���e�i������
	pFrame->pMeshContainer = NULL;
	//�V�K�t���[���̌Z��t���[���A�h���X�i�[�p�ϐ�������
	pFrame->pFrameSibling = NULL;
	//�V�K�t���[���̎q�t���[���A�h���X�i�[�p�ϐ�������
	pFrame->pFrameFirstChild = NULL;
	//�O���̐V�K�t���[���A�h���X�i�[�p�ϐ��ɁA�쐬�����t���[���̃A�h���X���i�[
	*ppNewFrame = pFrame;
	return S_OK;
}

//=============================================================================
// ���b�V���R���e�i�[�쐬�֐�
//=============================================================================
//HRESULT MY_HIERARCHY::CreateMeshContainer
//���b�V���R���e�i�[���쐬����
HRESULT MY_HIERARCHY::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA* pMeshData,
	CONST D3DXMATERIAL* pMaterials, CONST D3DXEFFECTINSTANCE* pEffectInstances,
	DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppMeshContainer)
{
	HRESULT hr;
	//���[�J�������p
	MYMESHCONTAINER *pMeshContainer = NULL;

	//���b�V���̖ʂ̐����i�[
	int iFacesAmount;
	//for���[�v�Ŏg�p
	int iMaterial;
	//�ꎞ�I��DirectX�f�o�C�X�擾�p
	LPDIRECT3DDEVICE9 pDevice = NULL;
	//�ꎞ�I�ȃ��b�V���f�[�^�i�[�p
	LPD3DXMESH pMesh = NULL;
	//���b�V���R���e�i�i�[�p�ϐ�������
	*ppMeshContainer = NULL;
	//�{�[���̐��i�[�p�ϐ�������
	DWORD dwBoneNum = 0;
	//pMesh��"�O��������"���b�V���A�h���X���i�[
	pMesh = pMeshData->pMesh;

	//���b�V���R���e�i�̈�̓��I�m��
	pMeshContainer = new MYMESHCONTAINER;

	//�̈�m�ێ��s��
	if (pMeshContainer == NULL)
	{
		return E_OUTOFMEMORY;
	}
	//���b�V���R���e�i��������
	ZeroMemory(pMeshContainer, sizeof(MYMESHCONTAINER));
	//���b�V���R���e�i�̖��O�i�[�p�̈�𓮓I�m��
	pMeshContainer->Name = new TCHAR[lstrlen(Name) + 1];
	//���s���̏���
	if (!pMeshContainer->Name)
	{
		return E_FAIL;
	}
	//�m�ۂ����̈�Ƀ��b�V���R���e�i�����i�[
	strcpy_s(pMeshContainer->Name, lstrlen(Name) + 1, Name);
	//DirectX�f�o�C�X�擾
	pMesh->GetDevice(&pDevice);
	//���b�V���̖ʂ̐����擾
	iFacesAmount = pMesh->GetNumFaces();

	//// ���b�V���ɖ@�����Ȃ��̂Ŗ@����ǉ�
	//pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
	//// �_��Ȓ��_�t�H�[�}�b�g (FVF) �R�[�h���g���ă��b�V���̃R�s�[���쐬����
	//hr = pMesh->CloneMeshFVF(
	//	pMesh->GetOptions(),
	//	pMesh->GetFVF() | D3DFVF_NORMAL,
	//	pDevice,
	//	&pMeshContainer->MeshData.pMesh); // �������ɃR�s�[
	//if (FAILED(hr))
	//{
	//	return E_FAIL;
	//}
	//pMesh = pMeshContainer->MeshData.pMesh;
	//// ���b�V���Ɋ܂܂��e���_�̖@�����v�Z���āA�ݒ肷��
	//D3DXComputeNormals(pMesh, NULL);


	//// �@���̊m�F
	//if (!(pMesh->GetFVF() & D3DFVF_NORMAL))
	//{
	//	// ���b�V���ɖ@�����Ȃ��̂Ŗ@����ǉ�
	//	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
	//	// �_��Ȓ��_�t�H�[�}�b�g (FVF) �R�[�h���g���ă��b�V���̃R�s�[���쐬����
	//	hr = pMesh->CloneMeshFVF(
	//		pMesh->GetOptions(),
	//		pMesh->GetFVF() | D3DFVF_NORMAL,
	//		pDevice,
	//		&pMeshContainer->MeshData.pMesh); // �������ɃR�s�[
	//	if (FAILED(hr))
	//	{
	//		return E_FAIL;
	//	}
	//	pMesh = pMeshContainer->MeshData.pMesh;
	//	// ���b�V���Ɋ܂܂��e���_�̖@�����v�Z���āA�ݒ肷��
	//	D3DXComputeNormals(pMesh, NULL);
	//}
	//else
	//{
	//	pMeshContainer->MeshData.pMesh = pMesh;
	//	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
	//	// �Q�ƃJ�E���^���C���N�������g
	//	pMesh->AddRef();
	//}

	//- ���b�V���̃}�e���A���ݒ� -//
	//���b�V���̃}�e���A�������i�[(�ő��1��)
	pMeshContainer->NumMaterials = max(1, NumMaterials);
	//���b�V���R���e�i�́A�}�e���A���f�[�^�i�[�̈�𓮓I�m��
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	//���b�V���R���e�i�́A�e�N�X�`���f�[�^�i�[�̈�𓮓I�m��
	pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	//���b�V���R���e�i�́A�ʂ��ƂɎ���3�̗אڐ���񂪊i�[���ꂽDWORD�^�̃A�h���X�i�[�p(�|�C���^)�ϐ�
	pMeshContainer->pAdjacency = new DWORD[iFacesAmount * 3];
	//�̈�m�ۂ̎��s���̏���
	if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
	{
		return E_FAIL;
	}
	//�O�������̗אڐ��������b�V���R���e�i�Ɋi�[
	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * iFacesAmount * 3);
	//�e�N�X�`���f�[�^�i�[�p�̈��������(memset���g�p����0�Œ��g�𖄂߂�)
	memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);
	//�����̃}�e���A������0����Ȃ��ꍇ
	if (NumMaterials > 0)
	{
		//�O�������̃}�e���A���f�[�^�A�h���X�����b�V���R���e�i�Ɋi�[
		memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);
		//�}�e���A���������[�v������
		for (iMaterial = 0; (DWORD)iMaterial < NumMaterials; iMaterial++)
		{
			//�e�N�X�`���̃t�@�C������NULL�łȂ����(�e�N�X�`���f�[�^�������)
			if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL)
			{
				////////�}�e���A���J���[��0.5�ɐݒ�
				//pMeshContainer->pMaterials[iMaterial].MatD3D.Diffuse.r = 1.0f;
				//pMeshContainer->pMaterials[iMaterial].MatD3D.Diffuse.g = 1.0f;
				//pMeshContainer->pMaterials[iMaterial].MatD3D.Diffuse.b = 1.0f;
				//pMeshContainer->pMaterials[iMaterial].MatD3D.Diffuse.a = 1.0f;

				////pMeshContainer->pMaterials[iMaterial].MatD3D.Emissive.r = 0.1f;
				////pMeshContainer->pMaterials[iMaterial].MatD3D.Emissive.g = 0.1f;
				////pMeshContainer->pMaterials[iMaterial].MatD3D.Emissive.b = 0.1f;
				////pMeshContainer->pMaterials[iMaterial].MatD3D.Emissive.a = 1.0f;

				//////�X�y�L������0.5�ɐݒ�(��Őݒ肵���}�e���A���J���[��0.5�̐ݒ���R�s�[)
				//pMeshContainer->pMaterials[iMaterial].MatD3D.Specular = pMeshContainer->pMaterials[iMaterial].MatD3D.Diffuse;
				//////pMeshContainer->pMaterials[iMaterial].MatD3D.Emissive = pMeshContainer->pMaterials[iMaterial].MatD3D.Diffuse;
				//pMeshContainer->pMaterials[iMaterial].MatD3D.Ambient = pMeshContainer->pMaterials[iMaterial].MatD3D.Diffuse;
				//pMeshContainer->pMaterials[iMaterial].MatD3D.Power = 50.0f;

				//�e�N�X�`���̃t�@�C���p�X�ۑ��p�ϐ�
				TCHAR strTexturePath[MAX_PATH];
				//�e�N�X�`���̃t�@�C���p�X��ۑ�(�ēǂݍ��ݎ��ɕK�v)
				strcpy_s(strTexturePath, lstrlen(pMeshContainer->pMaterials[iMaterial].pTextureFilename) + 1, pMeshContainer->pMaterials[iMaterial].pTextureFilename);
				//�e�N�X�`�����̓ǂݍ��݁iDebug�̓t�@�C���^Release�̓A�[�J�C�u����j
#ifdef _DEBUG
				if (FAILED(D3DXCreateTextureFromFile(pDevice, strTexturePath,
					&pMeshContainer->ppTextures[iMaterial])))
#else
				if (FAILED(D3DXCreateTextureFromArchive(pDevice, strTexturePath,
					&pMeshContainer->ppTextures[iMaterial])))
#endif
				{
					//���s���̏���
					//�e�N�X�`���t�@�C�����i�[�p
					CHAR TexMeshPass[255];
					//�ǋL
					//�����Ȃ���΁AGraph�t�H���_�𒲂ׂ�
					//���j�t�@�C�����̌������ɁA�K�������Ƀt�@�C���������鎖���m�F���Ă���
					//  strcpy_s��strcat_s���g���悤�ɂ���(���̏ꍇ�́A��ɂ��� 
					//    �e�N�X�`���̃t�@�C��������A����ɂ��̃t�@�C�����̒�����0�łȂ���� �̏���if��)�B
					//  TexMeshPass�ɁAX�t�@�C��������ꏊ�Ɠ����f�B���N�g���ƁA�e�N�X�`���̃t�@�C������
					//  �����������̂��i�[
					// strcpy_s( TexMeshPass, sizeof( TexMeshPass ) , "./../Source/Graph/" );
					strcpy_s(TexMeshPass, sizeof(TexMeshPass), "./Graph/");
					strcat_s(TexMeshPass, sizeof(TexMeshPass) - strlen(TexMeshPass) - strlen(strTexturePath) - 1, strTexturePath);
					//�e�N�X�`�����̓ǂݍ��݁iDebug�̓t�@�C���^Release�̓A�[�J�C�u����j
#ifdef _DEBUG
					if (FAILED(D3DXCreateTextureFromFile(pDevice, TexMeshPass,
						&pMeshContainer->ppTextures[iMaterial])))
#else
					if (FAILED(D3DXCreateTextureFromArchive(pDevice, TexMeshPass,
						&pMeshContainer->ppTextures[iMaterial])))
#endif
					{
						pMeshContainer->ppTextures[iMaterial] = NULL;
					}
					//�e�N�X�`���̃t�@�C���p�X��NULL�ɂ���
					pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
				}
			}
		}
	}
	else
	{
		//- �}�e���A���Ȃ��̏ꍇ -//
		//�e�N�X�`���t�@�C������NULL��
		pMeshContainer->pMaterials[0].pTextureFilename = NULL;
		//�}�e���A���f�[�^������(memset���g�p���Ē��g��0�Ŗ��߂�)
		memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
		//�}�e���A���J���[��0.5�ɐݒ�
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		//�X�y�L������0.5�ɐݒ�(��Őݒ肵���}�e���A���J���[��0.5�̐ݒ���R�s�[)
		pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
		//pMeshContainer->pMaterials[0].MatD3D.Emissive = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
		//pMeshContainer->pMaterials[0].MatD3D.Ambient = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
	}
	//���b�V�������i�[(����͒ʏ탁�b�V���Ɗ��S�ɕ����Ă��邽�߂��ׂăX�L�����b�V�����ƂȂ�)
	pMeshContainer->pSkinInfo = pSkinInfo;
	//�Q�ƃJ�E���^
	pSkinInfo->AddRef();
	//�{�[���̐����擾
	dwBoneNum = pSkinInfo->GetNumBones();
	//�t���[��(�{�[��)�P�ʂł̃��[���h�s��i�[�p�̈�̓��I�m��
	pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[dwBoneNum];
	//�{�[���̐��������[�v������
	for (DWORD i = 0; i < dwBoneNum; i++)
	{
		//�p�t���[��(�{�[��)�̃I�t�Z�b�g�s����擾���Ċi�[
		memcpy(&pMeshContainer->pBoneOffsetMatrices[i],
			pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i), sizeof(D3DMATRIX));
	}
	//- �ϊ���� -//
	//���b�V���R���e�i�ɃI���W�i����pMesh�����i�[
	D3DVERTEXELEMENT9 Decl[MAX_FVF_DECL_SIZE];
	pMesh->GetDeclaration(&Decl[0]);
	pMesh->CloneMesh(pMesh->GetOptions(), &Decl[0], pDevice, &pMeshContainer->pOriMesh);
	//���b�V���̃^�C�v���`
	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	//- �Œ�p�C�v���C���`��p�ɕϊ� -//
	//�V�F�[�_�ŕ`�悷��ꍇ�͕ʓr�ϊ����K�v
	//���_�P�ʂł̃u�����h�̏d�݂ƃ{�[���̑g�ݍ��킹�e�[�u����K�������V�������b�V����Ԃ��B
	if (FAILED(pMeshContainer->pSkinInfo->ConvertToBlendedMesh(
		pMeshContainer->pOriMesh, //���̃��b�V���f�[�^�A�h���X
		NULL, //�I�v�V����(���݂͎g���Ă��Ȃ�����NULL�ł���)	
		pMeshContainer->pAdjacency, //���̃��b�V���̗אڐ����
		NULL, //�o�̓��b�V���̗אڐ����
		NULL, //�e�ʂ̐V�����C���f�b�N�X�l�i�[�p�ϐ��̃A�h���X
		NULL, //�p���_�̐V�����C���f�b�N�X�l�i�[�p�ϐ��̃A�h���X
		&pMeshContainer->dwWeight, //�{�[���̉e���̈�ʓ�����̍ő吔�i�[�p�ϐ��̃A�h���X
		&pMeshContainer->dwBoneNum, //�{�[���̑g�ݍ��킹�e�[�u���Ɋ܂܂��{�[�����i�[�p�ϐ��̃A�h���X
		&pMeshContainer->pBoneBuffer, //�{�[���̑g�ݍ��킹�e�[�u���ւ̃|�C���^
		&pMeshContainer->MeshData.pMesh //�o�͂���郁�b�V���A�h���X�i�[�p�ϐ��̃A�h���X(�Œ�p�C�v���C���p)
	)))
	{
		return E_FAIL;
	}
	//���[�J���ɐ����������b�V���R���e�i�[���Ăяo�����ɃR�s�[����
	*ppMeshContainer = pMeshContainer;

	//�Q�ƃJ�E���^�𑝂₵���̂Ō��炷
	SAFE_RELEASE(pDevice);
	return S_OK;
}

//=============================================================================
// �t���[���j���֐�
//=============================================================================
//HRESULT MY_HIERARCHY::DestroyFrame(LPD3DXFRAME pFrameToFree) 
//�t���[����j������
HRESULT MY_HIERARCHY::DestroyFrame(LPD3DXFRAME pFrameToFree)
{

	testDestroy++;
	if (testDestroy > 139)
	{
		testDestroy++;
	}

	// 2�d����h�~
	// if (pFrameToFree == NULL)return S_FALSE;
	SAFE_DELETE_ARRAY(pFrameToFree->Name);
	if (pFrameToFree->pFrameFirstChild)
	{
		DestroyFrame(pFrameToFree->pFrameFirstChild);
	}
	if (pFrameToFree->pFrameSibling)
	{
		DestroyFrame(pFrameToFree->pFrameSibling);
	}
	SAFE_DELETE(pFrameToFree);
	return S_OK;
}

//=============================================================================
// ���b�V���R���e�i�[�j���֐�
//=============================================================================
//HRESULT MY_HIERARCHY::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
//���b�V���R���e�i�[��j������
HRESULT MY_HIERARCHY::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	int iMaterial;
	MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;
	SAFE_DELETE_ARRAY(pMeshContainer->Name);
	SAFE_RELEASE(pMeshContainer->pSkinInfo);
	SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
	SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);
	SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrix);
	if (pMeshContainer->ppTextures != NULL)
	{
		for (iMaterial = 0; (DWORD)iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			//�e�N�X�`�����
			SAFE_RELEASE(pMeshContainer->ppTextures[iMaterial]);
		}
	}
	SAFE_DELETE_ARRAY(pMeshContainer->ppTextures);
	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	SAFE_RELEASE(pMeshContainer->pOriMesh);
	if (pMeshContainer->pBoneBuffer != NULL)
	{
		SAFE_RELEASE(pMeshContainer->pBoneBuffer);
		SAFE_DELETE_ARRAY(pMeshContainer->pBoneOffsetMatrices);
	}
	SAFE_DELETE(pMeshContainer);
	pMeshContainerBase = NULL;
	return S_OK;
}

//=============================================================================
// �������֐�
//=============================================================================
CSkinMesh::CSkinMesh()
{
	//�}�e���A���ύX�t���O
	m_MaterialFlg = FALSE;
	//�}�e���A���f�[�^
	ZeroMemory(&m_Material, sizeof(D3DMATERIAL9));
	//�P�ʍs��
	D3DXMatrixIdentity(&(this->m_World));
	//�A�j���[�V�������ԏ�����
	m_AnimeTime = 0;
	//�A�j���[�V�����X�s�[�h������
	m_AnimSpeed = SKIN_ANIME_SPEED; //�Œ�
	//���݂̃A�j���[�V�����g���b�N������
	m_CurrentTrack = 0;
	//�A�j���[�V�����f�[�^�g���b�N������
	//�L���ɂ���
	m_CurrentTrackDesc.Enable = TRUE;
	//�e���x100%
	m_CurrentTrackDesc.Weight = 1;
	//�J�n�ʒu������
	m_CurrentTrackDesc.Position = 0;
	//���x
	m_CurrentTrackDesc.Speed = 1;
	// ���̃A�j���[�V�����փV�t�g����̂ɂ����鎞��
	m_fShiftTime = SKIN_ANIME_WEIGHT;
}

//=============================================================================
// �{�[������֐�
//=============================================================================
VOID CSkinMesh::Release()
{
	if (m_pFrameRoot != NULL) {
		//�{�[���t���[���֌W���
		FreeAnim(m_pFrameRoot);
		//���̑����(�e�N�X�`���̎Q�ƃf�[�^�Ȃ�)�̉��
		m_cHierarchy.DestroyFrame(m_pFrameRoot);
		m_pFrameRoot = NULL;
	}
	//�A�j���[�V�����R���g���[���[���
	SAFE_RELEASE(m_pAnimController);
	//���ׂẴt���[���Q�ƕϐ��̗v�f���폜
	m_FrameArray.clear();
	//���b�V���R���e�i����̃t���[���Q�ƕϐ��̗v�f���폜
	m_IntoMeshFrameArray.clear();
}

//=============================================================================
// �{�[���s��擾�֐�
//=============================================================================
HRESULT CSkinMesh::AllocateBoneMatrix(LPD3DXFRAME pFrameRoot, LPD3DXMESHCONTAINER pMeshContainerBase)
{
	MYFRAME *pFrame = NULL;
	DWORD dwBoneNum = 0;
	//���b�V���R���e�i�̌^���I���W�i���̌^�Ƃ��Ĉ���
	//(���b�V���R���e�i�������ɃI���W�i���̌^�Ƃ��č���Ă���̂Ŗ��͂Ȃ����A
	//��{�_�E���L���X�g�͊댯�Ȃ̂ő��p�͔�����ׂ�)
	MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;
	//�X�L�����b�V���łȂ����
	if (pMeshContainer->pSkinInfo == NULL)
	{
		return S_OK;
	}
	//�{�[���̐��擾
	dwBoneNum = pMeshContainer->pSkinInfo->GetNumBones();
	//�e�{�[���̃��[���h�s��i�[�p�̈���m��
	SAFE_DELETE(pMeshContainer->ppBoneMatrix);
	pMeshContainer->ppBoneMatrix = new D3DXMATRIX*[dwBoneNum];
	//�{�[���̐��������[�v
	for (DWORD i = 0; i<dwBoneNum; i++)
	{
		//�q�t���[��(�{�[��)�̃A�h���X����������pFrame�Ɋi�[
		pFrame = (MYFRAME*)D3DXFrameFind(pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(i));
		//�q�t���[�����Ȃ���Ώ������I������
		if (pFrame == NULL)
		{
			return E_FAIL;
		}
		//�e�{�[���̃��[���h�s��i�[�p�ϐ��ɍŏI�s����i�[
		pMeshContainer->ppBoneMatrix[i] = &pFrame->CombinedTransformationMatrix;
	}
	return S_OK;
}

//=============================================================================
// �{�[���s�񏉊����֐�
//=============================================================================
//HRESULT AllocateAllBoneMatrices( THING* pThing,LPD3DXFRAME pFrame )
HRESULT CSkinMesh::AllocateAllBoneMatrices(LPD3DXFRAME pFrameRoot, LPD3DXFRAME pFrame)
{
	//�K�w�̑���(���������m�ۂ������b�V���R���e�i�̈��T������)
	if (pFrame->pMeshContainer != NULL)
	{
		//�{�[���s��̏���������
		if (FAILED(AllocateBoneMatrix(pFrameRoot, pFrame->pMeshContainer)))
		{
			return E_FAIL;
		}
	}
	//�ċN���f����
	if (pFrame->pFrameSibling != NULL)
	{
		if (FAILED(AllocateAllBoneMatrices(pFrameRoot, pFrame->pFrameSibling)))
		{
			return E_FAIL;
		}
	}
	if (pFrame->pFrameFirstChild != NULL)
	{
		if (FAILED(AllocateAllBoneMatrices(pFrameRoot, pFrame->pFrameFirstChild)))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

//=============================================================================
// ���b�V�������_�����O�֐�
//=============================================================================
//VOID RenderMeshContainer(LPDIRECT3DDEVICE9 pDevice,MYMESHCONTAINER* pMeshContainer, MYFRAME* pFrame)
//�t���[�����̂��ꂼ��̃��b�V���������_�����O����
VOID CSkinMesh::RenderMeshContainer(LPDIRECT3DDEVICE9 pDevice, MYMESHCONTAINER* pMeshContainer, MYFRAME* pFrame)
{
	DWORD i, k;
	DWORD dwBlendMatrixNum;
	DWORD dwPrevBoneID;
	LPD3DXBONECOMBINATION pBoneCombination;
	UINT iMatrixIndex;
	D3DXMATRIX mStack;
	//�X�L�����b�V���̕`��
	if (pMeshContainer->pSkinInfo != NULL)
	{
#ifdef _DEBUG
		PrintDebugProc("Container  [%d]\n", m_dwContainerCount);
#endif
		//�{�[���e�[�u������o�b�t�@�̐擪�A�h���X���擾
		pBoneCombination = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneBuffer->GetBufferPointer());
		//dwPrevBoneID��UINT_MAX�̒l(0xffffffff)���i�[
		dwPrevBoneID = UINT_MAX;
		//�X�L�j���O�v�Z
		for (i = 0; i < pMeshContainer->dwBoneNum; i++)
		{
			dwBlendMatrixNum = 0;
			//�e�����Ă���s�񐔎擾
			for (k = 0; k< pMeshContainer->dwWeight; k++)
			{
				//UINT_MAX(-1)
				if (pBoneCombination[i].BoneId[k] != UINT_MAX)
				{
					//���݉e�����󂯂Ă���{�[���̐�
					dwBlendMatrixNum = k;
				}
			}
			//�W�I���g���u�����f�B���O���g�p���邽�߂ɍs��̌����w��
			pDevice->SetRenderState(D3DRS_VERTEXBLEND, dwBlendMatrixNum);
			//�e�����Ă���s��̌���
			for (k = 0; k < pMeshContainer->dwWeight; k++)
			{
				//iMatrixIndex��1�x�̌Ăяo���ŕ`��o����e�{�[�������ʂ���l���i�[
				//( ����BoneID�z��̒����̓��b�V���̎�ނɂ���ĈقȂ�
				//( �C���f�b�N�X�Ȃ��ł���΁@=�@���_���Ƃ̏d�� �ł���
				// �C���f�b�N�X����ł���΁@=�@�{�[���s��p���b�g�̃G���g����)
				//���݂̃{�[��(i�Ԗ�)����݂�k�Ԗڂ̃{�[��id
				iMatrixIndex = pBoneCombination[i].BoneId[k];
				//�s��̏�񂪂����
				if (iMatrixIndex != UINT_MAX)
				{
					//mStack�ɃI�t�Z�b�g�s��*�{�[���s����i�[
					mStack = pMeshContainer->pBoneOffsetMatrices[iMatrixIndex] * (*pMeshContainer->ppBoneMatrix[iMatrixIndex]);
					//�s��X�^�b�N�Ɋi�[
					pDevice->SetTransform(D3DTS_WORLDMATRIX(k), &mStack);
				}
			}
			D3DMATERIAL9 TmpMat = pMeshContainer->pMaterials[pBoneCombination[i].AttribId].MatD3D;
			TmpMat.Emissive.a = TmpMat.Diffuse.a = TmpMat.Ambient.a = 1.0f;
			pDevice->SetMaterial(&TmpMat);
			pDevice->SetTexture(0, pMeshContainer->ppTextures[pBoneCombination[i].AttribId]);
			//dwPrevBoneID�ɑ����e�[�u���̎��ʎq���i�[
			dwPrevBoneID = pBoneCombination[i].AttribId;
			//���b�V���̕`��
			pMeshContainer->MeshData.pMesh->DrawSubset(i);

#ifdef _DEBUG
			PrintDebugProc("Bone  [Con:%d  ID:%d  Name:%s]\n", 
				m_dwContainerCount,
				m_dwBoneCount,
				pMeshContainer->pSkinInfo->GetBoneName(i));
#endif
			m_dwBoneCount++;
		}
#ifdef _DEBUG
		PrintDebugProc("BoneMax  [%d]\n", m_dwBoneCount);
#endif
	}
	//�ʏ탁�b�V���̏ꍇ
	else
	{
		MessageBox(NULL, "�X�L�����b�V��X�t�@�C���̕`��Ɏ��s���܂����B", NULL, MB_OK);
		exit(EOF);
	}
}

//���b�V���̌��݂�Matrix�f�[�^�擾
bool CSkinMesh::GetMatrix(D3DXMATRIX* out,int dwCon, int dwBone)
{
	MYFRAME* pFrame = (MYFRAME*)m_pFrameRoot;
	MYMESHCONTAINER* pMeshContainer = (MYMESHCONTAINER*)pFrame->pMeshContainer;

	for (unsigned int i = 0; i < dwCon; i++)
	{
		if (pMeshContainer != NULL)
		{
			//���̃��b�V���R���e�i�փA�N�e�B�u���ڂ�
			pMeshContainer = (MYMESHCONTAINER*)pMeshContainer->pNextMeshContainer;
		}
		else
		{
			return false;
		}
	}

	if (dwBone <= pMeshContainer->dwBoneNum)
	{
		UINT iMatrixIndex;
		LPD3DXBONECOMBINATION pBoneCombination;
		//�{�[���e�[�u������o�b�t�@�̐擪�A�h���X���擾
		pBoneCombination = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneBuffer->GetBufferPointer());
		iMatrixIndex = pBoneCombination[dwBone].BoneId[0];

		//�s��̏�񂪂����
		if (iMatrixIndex != UINT_MAX)
		{
			//mStack�ɃI�t�Z�b�g�s��*�{�[���s����i�[
			*out = pMeshContainer->pBoneOffsetMatrices[iMatrixIndex] * (*pMeshContainer->ppBoneMatrix[iMatrixIndex]);
		}
		return true;
	}
	return false;
}

//=============================================================================
// �t���[�������_�����O�֐�
//=============================================================================
//VOID DrawFrame(LPDIRECT3DDEVICE9 pDevice,LPD3DXFRAME pFrameBase)
//�t���[���������_�����O����B
VOID CSkinMesh::DrawFrame(LPDIRECT3DDEVICE9 pDevice, LPD3DXFRAME pFrameBase)
{
	m_dwBoneCount = 0;
	MYFRAME* pFrame = (MYFRAME*)pFrameBase;
	MYMESHCONTAINER* pMeshContainer = (MYMESHCONTAINER*)pFrame->pMeshContainer;
	while (pMeshContainer != NULL)
	{
		//SHADER_KIND a = GetpShader()->GetShaderKind();
		////�V�F�[�_���g�p���Ă���̂Ȃ��p�̕`��֐��ɔ�΂�
		//if( GetpShader() != NULL && GetpShader()->GetShaderKind() == SHADER_KIND_LAMBERT ){
		// ShaderDraw( pDevice, ControlNum, pMeshContainer, pFrame ); 
		//}else{
		RenderMeshContainer(pDevice, pMeshContainer, pFrame);
		// }
		//���̃��b�V���R���e�i�փA�N�e�B�u���ڂ�
		pMeshContainer = (MYMESHCONTAINER*)pMeshContainer->pNextMeshContainer;
		m_dwContainerCount++;
	}
	if (pFrame->pFrameSibling != NULL)
	{
		DrawFrame(pDevice, pFrame->pFrameSibling);
	}
	if (pFrame->pFrameFirstChild != NULL)
	{
		DrawFrame(pDevice, pFrame->pFrameFirstChild);
	}
}

//=============================================================================
// ���[���h�X�V�֐�
//=============================================================================
//VOID UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
//�t���[�����̃��b�V�����Ƀ��[���h�ϊ��s����X�V����
VOID CSkinMesh::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	MYFRAME *pFrame = (MYFRAME*)pFrameBase;
	if (pParentMatrix != NULL)
	{
		//CombinedTransformationMatrix�ɍŏI�s����i�[
		D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	}
	else
	{
		//CombinedTransformationMatrix�ɍŏI�s����i�[
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
	}
	if (pFrame->pFrameSibling != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}
	if (pFrame->pFrameFirstChild != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	}
}

//=============================================================================
// �t���[������֐�
//=============================================================================
//�S�Ă̊K�w�t���[�����������
VOID CSkinMesh::FreeAnim(LPD3DXFRAME pFrame)
{
	if (pFrame->pMeshContainer != NULL) {
		m_cHierarchy.DestroyMeshContainer(pFrame->pMeshContainer);
		pFrame->pMeshContainer = NULL;
	}
	if (pFrame->pFrameSibling != NULL)
		FreeAnim(pFrame->pFrameSibling);
	if (pFrame->pFrameFirstChild != NULL)
		FreeAnim(pFrame->pFrameFirstChild);
}

//=============================================================================
// �������֐�
//=============================================================================
HRESULT CSkinMesh::Init(LPDIRECT3DDEVICE9 lpD3DDevice, LPSTR pMeshPass) {
	CHAR TmpMeshPass[255];
	strcpy_s(TmpMeshPass, sizeof(TmpMeshPass) - 1, pMeshPass);
	// X�t�@�C������A�j���[�V�������b�V����ǂݍ��ݍ쐬����iDebug�̓t�@�C���^Release�̓A�[�J�C�u����j
#ifdef _DEBUG
	if (FAILED(
		D3DXLoadMeshHierarchyFromX(TmpMeshPass, D3DXMESH_MANAGED, lpD3DDevice, &m_cHierarchy,
			NULL,
			&m_pFrameRoot,
			&m_pAnimController)))
#else
	if (FAILED(
		D3DXLoadMeshHierarchyFromXInArchive(TmpMeshPass, D3DXMESH_MANAGED, lpD3DDevice, &m_cHierarchy,
			NULL,
			&m_pFrameRoot,
			&m_pAnimController)))
#endif
	{
		MessageBox(NULL, "X�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", TmpMeshPass, MB_OK);
		return E_FAIL;
	}
	//�{�[���s�񏉊���
	AllocateAllBoneMatrices(m_pFrameRoot, m_pFrameRoot);
	//�A�j���[�V�����g���b�N�̎擾
	for (DWORD i = 0; i<m_pAnimController->GetNumAnimationSets(); i++)
	{
		//�A�j���[�V�����擾
		m_pAnimController->GetAnimationSet(i, &(m_pAnimSet[i]));
	}
	//���ׂẴt���[���Q�ƕϐ��̐���
	m_FrameArray.clear();
	m_IntoMeshFrameArray.clear();
	CreateFrameArray(m_pFrameRoot);

	MYFRAME *pFrame = NULL;


	//�t���[���z��ɃI�t�Z�b�g���쐬
	for (DWORD i = 0; i<m_IntoMeshFrameArray.size(); i++) {
		MYMESHCONTAINER* pMyMeshContainer = (MYMESHCONTAINER*)m_IntoMeshFrameArray[i]->pMeshContainer;
		while (pMyMeshContainer) {
			//�X�L�����
			if (pMyMeshContainer->pSkinInfo) {
				DWORD cBones = pMyMeshContainer->pSkinInfo->GetNumBones();
				for (DWORD iBone = 0; iBone<cBones; iBone++) 
				{
					pFrame = (MYFRAME*)D3DXFrameFind(m_pFrameRoot, pMyMeshContainer->pSkinInfo->GetBoneName(iBone));
					if (pFrame == NULL)
					{
						return E_FAIL;
					}
					pMyMeshContainer->ppBoneMatrix[iBone] = &pFrame->CombinedTransformationMatrix;


					////�t���[�������瓯�����O�̃t���[��������
					//for (DWORD Idx = 0; Idx<m_FrameArray.size(); Idx++) {
					//	if (strcmp(pMyMeshContainer->pSkinInfo->GetBoneName(iBone), m_FrameArray[Idx]->Name) == 0) {
					//		pMyMeshContainer->BoneFrameArray.push_back(m_FrameArray[Idx]);
					//		//Offset�s��
					//		m_FrameArray[Idx]->OffsetMat = *(pMyMeshContainer->pSkinInfo->GetBoneOffsetMatrix(iBone));
					//		m_FrameArray[Idx]->OffsetID = Idx;
					//		break;
					//	}
					//}
				}
			}
			//����
			pMyMeshContainer = (MYMESHCONTAINER *)pMyMeshContainer->pNextMeshContainer;
		}
	}
	return S_OK;
}

//=============================================================================
// �t���[���|�C���^�i�[�����֐�
//=============================================================================
VOID CSkinMesh::CreateFrameArray(LPD3DXFRAME _pFrame) {
	if (_pFrame == NULL) { return; }
	//�t���[���A�h���X�i�[
	MYFRAME* pMyFrame = (MYFRAME*)_pFrame;
	m_FrameArray.push_back(pMyFrame);
	//���b�V���R���e�i������ꍇ��IntoMeshFrameArray�ɃA�h���X���i�[
	if (pMyFrame->pMeshContainer != NULL) {
		m_IntoMeshFrameArray.push_back(pMyFrame);
	}
	//�q�t���[������
	if (pMyFrame->pFrameFirstChild != NULL) {
		CreateFrameArray(pMyFrame->pFrameFirstChild);
	}
	//�Z��t���[������
	if (pMyFrame->pFrameSibling != NULL) {
		CreateFrameArray(pMyFrame->pFrameSibling);
	}
}

//=============================================================================
// �X�V����
//=============================================================================
VOID CSkinMesh::Update(void) {
	//�������ςȂ��ɂ��A���؂�ւ��h�~
	//static bool PushFlg = false; //�����ł͉��Ńt���O���g�p���邪�A�{���̓����o�ϐ��Ȃǂɂ���
	//							 //�A�j���[�V�����ύX�`�F�b�N
	//if ((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState(VK_RIGHT) & 0x8000)) {
	//	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
	//		if (PushFlg == false) {
	//			int Num = GetAnimTrack() - 1;
	//			if (Num < 0)Num = 0;
	//			
	//			(Num, m_fShiftTime);
	//		}
	//	}
	//	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
	//		if (PushFlg == false) {
	//			int Num = GetAnimTrack() + 1;
	//			if ((DWORD)Num > m_pAnimController->GetNumAnimationSets())Num = m_pAnimController->GetNumAnimationSets();
	//			ChangeAnim(Num, m_fShiftTime);
	//		}
	//	}
	//	PushFlg = true;
	//}
	//else {
	//	PushFlg = false;
	//}

	// ���[�V�����u�����h�m�F
	m_fCurWeight += m_fShiftTime;
	if (m_fCurWeight <= 1.0f)
	{
		// �u�����h��
		m_pAnimController->SetTrackWeight(0, m_fCurWeight);			// ���݂̃A�j���[�V����
		m_pAnimController->SetTrackWeight(1, 1.0f - m_fCurWeight);	// �O�̃A�j���[�V����
	}
	else
	{
		// �u�����h�I���B�ʏ�A�j���[�V����������Track0�̃E�F�C�g���ő�l�ɌŒ�
		m_pAnimController->SetTrackWeight(0, 1.0f);					// ���݂̃A�j���[�V����
		m_pAnimController->SetTrackEnable(1, false);				// �O�̃A�j���[�V�����𖳌��ɂ���
	}

	//�A�j���[�V�������Ԃ��X�V
	m_AnimeTime++;
}

//=============================================================================
// �X�L�����b�V���`��֐�
//=============================================================================
VOID CSkinMesh::Draw(LPDIRECT3DDEVICE9 lpD3DDevice, D3DXMATRIX _World)
{
#ifdef _DEBUG
	PrintDebugProc("�y SkinMesh �z\n");
#endif
	//�}�g���b�N�X�s�񔽉f
	m_World = _World;
	// ���b�V���R���e�i�J�E���^��������
	m_dwContainerCount = 0;
	//���݂̃A�j���[�V�����ԍ���K��
	m_pAnimController->SetTrackAnimationSet(0, m_pAnimSet[m_CurrentTrack]);
	//0(�Đ�����)�g���b�N����g���b�N�f�X�N���Z�b�g����
	m_pAnimController->SetTrackDesc(0, &(m_CurrentTrackDesc));
	//�A�j���[�V�������ԃf�[�^�̍X�V
	m_pAnimController->AdvanceTime(m_AnimSpeed, NULL);
	//�A�j���[�V�����f�[�^���X�V
	UpdateFrameMatrices(m_pFrameRoot, &m_World);
	//�A�j���[�V�����`��
	DrawFrame(lpD3DDevice, m_pFrameRoot);
	//0(�Đ�����)�g���b�N����X�V�����g���b�N�f�X�N���擾����
	m_pAnimController->GetTrackDesc(0, &m_CurrentTrackDesc);
}

//=============================================================================
// �A�j���[�V�����ύX�֐�
//=============================================================================
//�I�u�W�F�N�g�̃A�j���[�V�����ύX( �ύX����A�j���[�V�����ԍ� )
VOID CSkinMesh::ChangeAnim(DWORD _NewAnimNum, FLOAT fShift)
{
	if (m_CurrentTrack == _NewAnimNum)
	{	// �قȂ�A�j���[�V�����ł��邩�`�F�b�N
		return;
	}

	// ���݂̃A�j���[�V�����Z�b�g�̐ݒ�l���擾
	D3DXTRACK_DESC TD;   // �g���b�N�̔\��
	m_pAnimController->GetTrackDesc(0, &TD);
	// ���݂̃A�j���[�V�������g���b�N1�ֈڍs
	m_pAnimController->SetTrackAnimationSet(1, m_pAnimSet[m_CurrentTrack]);
	m_pAnimController->SetTrackDesc(1, &TD);


	// ���݂̃A�j���[�V������ۊ�
	m_OldTrack = m_CurrentTrack;
	// �V�K�A�j���[�V�����ɕύX
	m_CurrentTrack = _NewAnimNum;

	//�A�j���[�V�����^�C����������
	m_AnimeTime = 0;
	// �A�j���[�V�������ŏ��̈ʒu����Đ�������
	m_pAnimController->GetTrackDesc(0, &m_CurrentTrackDesc);
	m_CurrentTrackDesc.Position = 0;
	m_pAnimController->SetTrackDesc(0, &m_CurrentTrackDesc);

	// �E�F�C�g�^�C����������
	m_fCurWeight = 0.0f;
	// ���̃A�j���[�V�����փV�t�g����̂ɂ����鎞�Ԃ�ݒ�
	m_fShiftTime = fShift;

	// �g���b�N�̍���������
	m_pAnimController->SetTrackEnable(0, true);
	m_pAnimController->SetTrackEnable(1, true);
}

//=============================================================================
// �Ώۃ{�[�������֐�
//=============================================================================
MYFRAME* CSkinMesh::SearchBoneFrame(LPSTR _BoneName, D3DXFRAME* _pFrame) {
	MYFRAME* pFrame = (MYFRAME*)_pFrame;
	if (pFrame->Name != NULL)
	{
		if (strcmp(pFrame->Name, _BoneName) == 0) {
			return pFrame;
		}
	}
	if (_pFrame->pFrameSibling != NULL)
	{
		pFrame = SearchBoneFrame(_BoneName, _pFrame->pFrameSibling);
		if (pFrame != NULL) {
			return pFrame;
		}
	}
	if (_pFrame->pFrameFirstChild != NULL)
	{
		pFrame = SearchBoneFrame(_BoneName, _pFrame->pFrameFirstChild);
		if (pFrame != NULL) {
			return pFrame;
		}
	}
	return NULL;
}

//=============================================================================
// �{�[���̃}�g���N�X�擾�֐�
//=============================================================================
D3DXMATRIX CSkinMesh::GetBoneMatrix(LPSTR _BoneName) {
	MYFRAME* pFrame = SearchBoneFrame(_BoneName, m_pFrameRoot);
	//�{�[�����������
	if (pFrame != NULL) {
		//�{�[���s���Ԃ�
		return pFrame->CombinedTransformationMatrix;
	}
	//�{�[����������Ȃ����
	else {
		//�P�ʍs���Ԃ�
		D3DXMATRIX TmpMatrix;
		D3DXMatrixIdentity(&TmpMatrix);
		return TmpMatrix;
	}
}

//=============================================================================
// �{�[���̃}�g���N�X�|�C���^�擾�֐�
//=============================================================================
D3DXMATRIX* CSkinMesh::GetpBoneMatrix(LPSTR _BoneName) {
	/////////////////////////////////////
	//���ӁjRokDeBone�p�ɐݒ�(�Ώۃ{�[���̈��̍s����Ƃ��Ă���)
	MYFRAME* pFrame = SearchBoneFrame(_BoneName, m_pFrameRoot);
	//�{�[�����������
	if (pFrame != NULL) {
		return &pFrame->CombinedTransformationMatrix;
	}
	//�{�[����������Ȃ����
	else {
		//NULL��Ԃ�
		return NULL;
	}
}