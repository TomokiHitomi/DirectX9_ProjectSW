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
Object	*Object::s_pRoot = NULL;

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
	// リスト構造の再結合処理

	// 前ポインタがNULLではない
	if (this->m_pPrev != NULL)
	{
		// 前ポインタが指す次ポインタを
		// 自らが格納している次ポインタに差し替える
		this->m_pPrev->m_pNext = this->m_pNext;
	}
	// 前ポインタがNULL
	else
	{	// ルートポインタをNULL
		s_pRoot = NULL;
	}

	// 次ポインタがNULLではない
	if (this->m_pNext != NULL)
	{
		// 次ポインタが指す前ポインタを
		// 自らが格納している前ポインタに差し替える
		this->m_pNext->m_pPrev = this->m_pPrev;
	}

	delete this;
}

//=============================================================================
// コンストラクタ処理（初期化）
//=============================================================================
Object::Object(void)
{
	// プロパティの初期化
	this->m_pPrev = NULL;
	this->m_pNext = NULL;

	// ルートポインタをpListに格納
	Object **pList = &s_pRoot;
	Object **pPrevTemp = NULL;

	// リスト構造形成ループ
	while (true)
	{
		// pListポインタが設定済みならば
		if (*pList != NULL)
		{
			// 前ポインタ候補としてpListを保管
			pPrevTemp = pList;

			// 次ポインタをpListに格納しなおす
			pList = &(*pList)->m_pNext;
		}
		// 次ポインタが未設定ならば
		else if (*pList == NULL)
		{
			// pListがルート以外
			if (*pList != s_pRoot)
			{
				// 前ポインタにpPrevTempを設定
				this->m_pPrev = *pPrevTemp;
			}

			// pListの指すRootまたはNextに自らを格納
			*pList = this;
			break;
		}
	}
}


//=============================================================================
// デストラクタ処理（終了）
//=============================================================================
Object::~Object(void)
{

}

//=============================================================================
// 全オブジェクトの更新処理
//=============================================================================
void Object::UpdateAll(void)
{
	int	nDebugCount = 0;

	Object *pList = Object::GetObjectRoot();

	while (pList != NULL)
	{
		pList->Update();
		pList = pList->GetObjectNext();
		nDebugCount++;
	}

	PrintDebugProc("【 LIST_TEST : %d 】\n", nDebugCount);
}

//=============================================================================
// 全オブジェクトの描画処理
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
// 全オブジェクトの解放処理
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