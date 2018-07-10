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
public:
	// リスト内優先順位
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
	static int		nObjectCount;		// 全オブジェクト数のカウンタ
	static Object	*s_pRoot;			// リストのルートポインタ
	Object			*m_pPrev;			// リストの前ポインタ
	Object			*m_pNext;			// リストの次ポインタ

	Priority		eMainPriority;		// 優先順位（メイン）
	Priority		eSubPriority;		// 優先順位（サブ）
	ObjectID		eObjectId;			// オブジェクト識別用ID
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

	// オブジェクトIDとプライオリティの設定処理
	void SetIdAndPriority(ObjectID, Priority, Priority);
	// オブジェクトをリスト追加処理
	void AppendList(void);

	// 全オブジェクトの更新処理
	static void UpdateAll(void);
	// 全オブジェクトの描画処理
	static void DrawAll(void);
	// 全オブジェクトの解放処理
	static void ReleaseAll(void);

	// ルートポインタの取得処理
	static Object *GetObjectRoot(void) { return s_pRoot; }
	// ルートポインタのアドレス取得処理
	static Object **GetObjectRootAdr(void) { return &s_pRoot; }
	// ネクストポインタの取得処理
	Object *GetObjectNext(void) { return m_pNext; }

	// オブジェクトID設定処理
	void SetObjectId(ObjectID eObjId) { eObjectId = eObjId; }
	// メインプライオリティ設定処理
	void SetMainPriority(Priority eMain) { eMainPriority = eMain; }
	// サブプライオリティ設定処理
	void SetSubPriority(Priority eSub) { eSubPriority = eSub; }

	// オブジェクトID取得処理
	ObjectID GetObjectId(void) { return (eObjectId); }
	// メインプライオリティ取得処理
	Priority GetMainPriority(void) { return (eMainPriority); }
	// サブプライオリティ取得処理
	Priority GetSubPriority(void) { return (eSubPriority); }
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

#endif
