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
	static Object *s_pRoot;
	Object *m_pPrev;
	Object *m_pNext;
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
	// ネクストポインタの取得処理
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
// プロトタイプ宣言
//*****************************************************************************

#endif
