//=============================================================================
//
// オブジェクト処理 [object.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

/*******************************************************************************
* インクルード
*******************************************************************************/
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************

// オブジェクト用
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

	// 更新処理
	virtual void Update(void) {}
	// 描画処理
	virtual void Draw(void) {}
	// 解放処理
	void Release(void);

	// コンストラクタ（初期化処理）
	Object();
	// デストラクタ（終了処理）
	virtual ~Object();

	//void

	// メインプライオリティ設定処理
	void SetMainPriority(Priority eMain) { eMainPriority = eMain; }
	// サブプライオリティ設定処理
	void SetSubPriority(Priority eSub) { eSubPriority = eSub; }

	// メインプライオリティ取得処理
	Priority GetMainPriority(void) { return (eMainPriority); }
	// サブプライオリティ取得処理
	Priority GetSubPriority(void) { return (eSubPriority); }


	// 全オブジェクトの更新処理
	static void UpdateAll(void);
	// 全オブジェクトの描画処理
	static void DrawAll(void);
	// 全オブジェクトの解放処理
	static void ReleaseAll(void);

	// オブジェクト生成処理
	static void Create(Object*) {}
	// ルートポインタの取得処理
	static Object *GetObjectRoot(void) { return s_pRoot; }
	// ルートポインタのアドレス取得処理
	static Object **GetObjectRootAdr(void) { return &s_pRoot; }
	// ネクストポインタの取得処理
	Object *GetObjectNext(void) { return m_pNext; }
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

#endif
