//=============================================================================
//
// デバッグ表示処理 [debugproc.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include <stdio.h>
#include "debugproc.h"
#include "input.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPD3DXFONT	g_pD3DXFont = NULL;			// フォントへのポインタ
char		g_aStrDebug[4096 * 10] = {"\0"};	// デバッグ情報
long		g_lDrawPos = 0;
bool		g_bDispDebug = true;

extern SceneManager		g_cScene;					// Sceneマネージャ

//=============================================================================
// デバッグ表示処理の初期化
//=============================================================================
HRESULT InitDebugProc(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HRESULT hr;

	// 情報表示用フォントを設定
	hr = D3DXCreateFont(pDevice, 25, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
					OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pD3DXFont);

	// 情報クリア
	memset(g_aStrDebug, 0, sizeof g_aStrDebug);

	return hr;
}

//=============================================================================
// デバッグ表示処理の終了処理
//=============================================================================
void UninitDebugProc(void)
{
	if(g_pD3DXFont != NULL)
	{// 情報表示用フォントの開放
		g_pD3DXFont->Release();
		g_pD3DXFont = NULL;
	}
}

//=============================================================================
// デバッグ表示処理の更新処理
//=============================================================================
void UpdateDebugProc(void)
{
	if (GetKeyboardTrigger(DIK_F1))
	{// デバッグ表示ON/OFF
		g_bDispDebug = g_bDispDebug ? false : true;
	}
	if (g_bDispDebug)
	{
		// デバッグフォントのスクロール
		if (GetKeyboardPress(DIK_9) && GetKeyboardPress(DIK_0))
		{	// 同時押しは不動
		}
		else if (GetKeyboardPress(DIK_9))
		{
			g_lDrawPos += (long)DEBUG_SCROLL_SPEED;
		}
		else if (GetKeyboardPress(DIK_0))
		{
			g_lDrawPos -= (long)DEBUG_SCROLL_SPEED;
		}

		// マウスホイール
		long ModUseZ = GetMobUseZ();
		if (ModUseZ != 0)
		{
			g_lDrawPos += ModUseZ;
		}
	}
}

//=============================================================================
// デバッグ表示処理の描画処理
//=============================================================================
void DrawDebugProc(void)
{
	if (g_bDispDebug)
	{
		RECT rect = { 0, g_lDrawPos, SCREEN_WIDTH, SCREEN_HEIGHT };

		// 現在のシーンを取得
		SceneManager::SCENE eScene = g_cScene.GetScene();

		// 情報表示（白文字）
		g_pD3DXFont->DrawText(NULL, g_aStrDebug, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0x00, 0x00, 0x00));

		// 情報クリア
		memset(g_aStrDebug, 0, sizeof g_aStrDebug);
	}
}

//=============================================================================
// デバッグ操作表示
//=============================================================================
void DebugHowTo(void)
{
	PrintDebugProc("【 Debug操作方法 】\n");
	PrintDebugProc("F1:デバッグ表示/非表示  F2:TITLE  F3:GAME  F4:RESULT\n");
	PrintDebugProc("9/0/マウスホイール:デバッグスクロール\n");
	PrintDebugProc("←→:アニメーション変更\n");
	PrintDebugProc("LSHIFT押下中  WASD:カメラ移動  ↑↓:拡大縮小  Enter:カメラリセット\n");
	PrintDebugProc("\n");
}

//=============================================================================
// デバッグ表示の登録
//=============================================================================
void PrintDebugProc(char *fmt,...)
{
#if 0
	long *pParam;
	static char aBuf[256];

	pParam = (long*)&fmt;
	sprintf(aBuf, fmt, pParam[1], pParam[2], pParam[3], pParam[4],
									pParam[5], pParam[6], pParam[7], pParam[8],
									pParam[9], pParam[10], pParam[11], pParam[12]);
#else
	va_list list;			// 可変引数を処理する為に使用する変数
	char *pCur;
	char aBuf[256]={"\0"};
	char aWk[128];

	// 可変引数にアクセスする前の初期処理
	va_start(list, fmt);

	pCur = fmt;
	for( ; *pCur; ++pCur)
	{
		if(*pCur != '%')
		{
			sprintf(aWk, "%c", *pCur);
		}
		else
		{
			pCur++;

			switch(*pCur)
			{
			case 'd':
				// 可変引数にアクセスしてその変数を取り出す処理
				sprintf(aWk, "%d", va_arg(list, int));
				break;

			case 'u':
				// 可変引数にアクセスしてその変数を取り出す処理
				sprintf(aWk, "%u", va_arg(list, int));
				break;

			case 'f':
				// 可変引数にアクセスしてその変数を取り出す処理
				sprintf(aWk, "%.3f", va_arg(list, double));		// double型で指定
				break;

			case 'l':
				// 可変引数にアクセスしてその変数を取り出す処理
				sprintf(aWk, "%.2ld", va_arg(list, long));		// long型で指定
				break;

			case 's':
				// 可変引数にアクセスしてその変数を取り出す処理
				sprintf(aWk, "%s", va_arg(list, char*));
				break;

			case 'c':
				// 可変引数にアクセスしてその変数を取り出す処理
				sprintf(aWk, "%c", va_arg(list, char));
				break;

			default:
				sprintf(aWk, "%c", *pCur);
				break;
			}
		}
		strcat(aBuf, aWk);
	}

	// 可変引数にアクセスした後の終了処理
	va_end(list);

	// 連結
	if((strlen(g_aStrDebug) + strlen(aBuf)) < sizeof g_aStrDebug - 1)
	{
		strcat(g_aStrDebug, aBuf);
	}
#endif
}

