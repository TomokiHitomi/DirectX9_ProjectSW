//=============================================================================
//
// ライト処理 [light.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	LIGHT_MAX				(3)						// ライトの数

#define LIGHT_DIFFUSE_0			(1.0f)
#define LIGHT_DIFFUSE_1			(0.5f)
#define LIGHT_DIFFUSE_2			(0.5f)
#define LIGHT_AMBIENT			(0.6f)


#define LIGHT_SUB_DIFFUSE		(0.3f)
#define LIGHT_SUB_XZ			(0.8)
#define LIGHT_SUB_Y				(1.0f)

enum
{
	LIGHT_MAIN,
	LIGHT_SUB1,
	LIGHT_SUB2
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitLight(void);
void SetLight(int nLight, bool bFlag);

#endif
