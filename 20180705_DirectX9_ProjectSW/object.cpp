//=============================================================================
//
// オブジェクト処理 [object.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "object.h"

/* 全体で必要なインクルード */

/* オブジェクトで必要なインクルード */

/* Debug */
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// マクロ定義
//*****************************************************************************
Object	*Object::s_pRoot[ObjectRootMax] = { NULL, NULL };
//Object	*Object::s_pRootUpdate = NULL;
//Object	*Object::s_pRootDraw = NULL;
int		Object::nObjectCount = 0;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//=============================================================================
// 更新処理
//=============================================================================


//=============================================================================
// 描画処理
//=============================================================================

//=============================================================================
// 解放処理
//=============================================================================
void Object::Release(void)
{
	// ルート格納用ポインタ
	Object	**pRoot = NULL;

	// リスト構造の再結合処理
	for (unsigned int i = 0; i < ObjectRootMax; i++)
	{
		// ルートポインタを取得
		pRoot = GetObjectRootAdr(ObjectRoot(i));

		// 前ポインタがNULLではない
		if ((this->m_pPrev[i]) != NULL)
		{
			// 前ポインタが指す次ポインタを
			// 自らが格納している次ポインタに差し替える
			(this->m_pPrev[i])->m_pNext[i] = this->m_pNext[i];
		}
		// 前ポインタがNULL
		else
		{	// ルートポインタをNULL
			*pRoot = NULL;
		}

		// 次ポインタがNULLではない
		if (this->m_pNext[i] != NULL)
		{
			// 次ポインタが指す前ポインタを
			// 自らが格納している前ポインタに差し替える
			this->m_pNext[i]->m_pPrev[i] = this->m_pPrev[i];
		}
	}

	delete this;
}

//=============================================================================
// コンストラクタ処理（初期化）
//=============================================================================
Object::Object(void)
{
	// オブジェクトカウンタをインクリメント
	nObjectCount++;

	// プロパティの初期化
	for (unsigned int i = 0; i < ObjectRootMax; i++)
	{
		this->m_pPrev[i] = NULL;			// 前ポインタの初期化
		this->m_pNext[i] = NULL;			// 後ポインタの初期化
		this->m_ePriority[i] = Middle;
	}

	//this->eUpdatePriority = Normal;	// メインプライオリティをNormalで初期化
	//this->eDrawPriority = Normal;	// サブプライオリティをNormalで初期化
}


//=============================================================================
// デストラクタ処理（終了）
//=============================================================================
Object::~Object(void)
{
	// オブジェクトカウンタをデクリメント
	nObjectCount--;
}

//=============================================================================
// オブジェクトIDとプライオリティの設定処理
// 引数：ID, 更新プライオリティ, 描画プライオリティ
//=============================================================================
void Object::SetIdAndPriority(ObjectID eObjId, Priority eUpdateP, Priority eDrawP)
{
	//// プライオリティテスト用
	//eObjId = ObjectID(nObjectCount);

	this->SetObjectId(eObjId);							// オブジェクトIDを設定
	this->SetPriority(UpdateRoot, eUpdateP);	// 更新プライオリティを設定
	this->SetPriority(DrawRoot, eDrawP);		// 描画プライオリティを設定

	// オブジェクトをリスト追加処理
	AppendList();
}

//=============================================================================
// オブジェクトをリスト追加処理
//=============================================================================
void Object::AppendList(void)
{
	Object **pList = NULL;
	Object **pPrevTemp = NULL;

	for (unsigned int i = 0; i < ObjectRootMax; i++)
	{
		// ルートポインタをpListに格納
		pList = GetObjectRootAdr(ObjectRoot(i));

		// リスト構造形成ループ
		while (true)
		{
			// pListポインタが設定済みならば
			if (*pList != NULL)
			{
				// プライオリティが高い
				if ((*pList)->m_ePriority[i] > this->m_ePriority[i])
				{
					// 割り込み処理
					// pListの前ポインタに自らを格納
					(*pList)->m_pPrev[i] = this;
					// pListがルート以外
					if (*pList != s_pRoot[i])
					{
						// 前ポインタにpPrevTempを設定
						this->m_pPrev[i] = *pPrevTemp;
					}
					this->m_pNext[i] = *pList;
					// pListに自らを格納
					*pList = this;
					break;
				}

				// 前ポインタ候補としてpListを保管
				pPrevTemp = pList;

				// 次ポインタをpListに格納しなおす
				pList = &(*pList)->m_pNext[i];
			}
			// 次ポインタが未設定ならば
			else if (*pList == NULL)
			{
				// pListがルート以外
				if (*pList != s_pRoot[i])
				{
					// 前ポインタにpPrevTempを設定
					this->m_pPrev[i] = *pPrevTemp;
				}

				// pListの指すRootまたはNextに自らを格納
				*pList = this;
				break;
			}
		}
	}
}

//=============================================================================
// 全オブジェクトの更新処理
//=============================================================================
void Object::UpdateAll(void)
{
	Object *pList = Object::GetObjectRoot(UpdateRoot);

#ifdef _DEBUG
	PrintDebugProc("【 ObjectCount : %d 】\n", nObjectCount);
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
// 全オブジェクトの描画処理
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
// 全オブジェクトの解放処理
//=============================================================================
void Object::ReleaseAll(void)
{
	Object *pList = NULL;
	Object *pNext = NULL;
	for (unsigned int i = 0; i < ObjectRootMax; i++)
	{
		// ルートポインタをpListに格納
		pList = GetObjectRoot(ObjectRoot(i));

		// NULLがくるまでpListをたどる
		while (pList != NULL)
		{
			pNext = pList->GetObjectNext(ObjectRoot(i));
			pList->Release();
			pList = pNext;
		}
	}
}

//=============================================================================
// ルートポインタのアドレス取得処理
//=============================================================================
Object **Object::GetObjectRootAdr(ObjectRoot eObjRoot)
{
	return &s_pRoot[eObjRoot];
}

//=============================================================================
// ネクストポインタの取得処理
//=============================================================================
Object *Object::GetObjectNext(ObjectRoot eObjRoot)
{ 
	return m_pNext[eObjRoot];
}