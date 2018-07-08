//=============================================================================
//
// 入力処理 [input.h]
// Author :  GP12A295 25 人見友基
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"
#include <Xinput.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************

// プログラム分けするときに使う
#define	USE_KEYBOARD										// 宣言するとキーボードで操作可能になる
#define	USE_MOUSE											// 宣言するとマウスで操作可能になる
#define	USE_PAD												// 宣言するとパッドで操作可能になる

/* mouse */
#define MOUSE_COUNT_MAX		(100)		// マウス操作カウント

/* game pad情報 */
#define BUTTON_UP			0x00000001l	// 方向キー上(.IY<0)
#define BUTTON_DOWN			0x00000002l	// 方向キー下(.IY>0)
#define BUTTON_LEFT			0x00000004l	// 方向キー左(.IX<0)
#define BUTTON_RIGHT		0x00000008l	// 方向キー右(.IX>0)
#define BUTTON_A			0x00000010l	// Ａボタン(.rgbButtons[0]&0x80)
#define BUTTON_B			0x00000020l	// Ｂボタン(.rgbButtons[1]&0x80)
#define BUTTON_C			0x00000040l	// Ｃボタン(.rgbButtons[2]&0x80)
#define BUTTON_X			0x00000080l	// Ｘボタン(.rgbButtons[3]&0x80)
#define BUTTON_Y			0x00000100l	// Ｙボタン(.rgbButtons[4]&0x80)
#define BUTTON_Z			0x00000200l	// Ｚボタン(.rgbButtons[5]&0x80)
#define BUTTON_L			0x00000400l	// Ｌボタン(.rgbButtons[6]&0x80)
#define BUTTON_R			0x00000800l	// Ｒボタン(.rgbButtons[7]&0x80)
#define BUTTON_START		0x00001000l	// ＳＴＡＲＴボタン(.rgbButtons[8]&0x80)
#define BUTTON_M			0x00002000l	// Ｍボタン(.rgbButtons[9]&0x80)

#define BUTTON_POV_UP		0x00004000l	// POV方向キー上
#define BUTTON_POV_DOWN		0x00008000l	// POV方向キー下
#define BUTTON_POV_LEFT		0x00010000l	// POV方向キー左
#define BUTTON_POV_RIGHT	0x00020000l	// POV方向キー右

#define GAMEPADMAX			8			// 同時に接続するジョイパッドの最大数をセット


/* XInput */
#define GAMEPADMAX_XINPUT		(4)			// 同時に接続するジョイパッドの最大数をセット
#define XINPUT_VIBRATION_MAX	(65535)		// バイブレーションの最大値
#define XINPUT_VIBRATION_DEBUG	(500)		// バイブレーションの変動値（モーター回転数・デバッグ）

// ボタン情報（通常枠）
#define XBOTTON_DPAD_UP				0x00000001
#define XBOTTON_DPAD_DOWN			0x00000002
#define XBOTTON_DPAD_LEFT			0x00000004
#define XBOTTON_DPAD_RIGHT			0x00000008
#define XBOTTON_START				0x00000010
#define XBOTTON_BACK				0x00000020
#define XBOTTON_LEFT_THUMB			0x00000040
#define XBOTTON_RIGHT_THUMB			0x00000080
#define XBOTTON_LEFT_SHOULDER		0x00000100
#define XBOTTON_RIGHT_SHOULDER		0x00000200
#define XBOTTON_A					0x00001000
#define XBOTTON_B					0x00002000
#define XBOTTON_X					0x00004000
#define XBOTTON_Y					0x00008000
// ボタン情報（拡張枠）
#define XBOTTON_STICK_UP			0x00010000
#define XBOTTON_STICK_DOWN			0x00020000
#define XBOTTON_STICK_LEFT			0x00040000
#define XBOTTON_STICK_RIGHT			0x00080000
#define XBOTTON_STICKR_UP			0x00100000
#define XBOTTON_STICKR_DOWN			0x00200000
#define XBOTTON_STICKR_LEFT			0x00400000
#define XBOTTON_STICKR_RIGHT		0x00800000
#define XBOTTON_LT					0x01000000
#define XBOTTON_RT					0x02000000

// 下記デファインはDirectXにて定義されている
// コメントアウトを外すべからず
//#define XINPUT_GAMEPAD_DPAD_UP          0x0001
//#define XINPUT_GAMEPAD_DPAD_DOWN        0x0002
//#define XINPUT_GAMEPAD_DPAD_LEFT        0x0004
//#define XINPUT_GAMEPAD_DPAD_RIGHT       0x0008
//#define XINPUT_GAMEPAD_START            0x0010
//#define XINPUT_GAMEPAD_BACK             0x0020
//#define XINPUT_GAMEPAD_LEFT_THUMB       0x0040
//#define XINPUT_GAMEPAD_RIGHT_THUMB      0x0080
//#define XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100
//#define XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200
//#define XINPUT_GAMEPAD_A                0x1000
//#define XINPUT_GAMEPAD_B                0x2000
//#define XINPUT_GAMEPAD_X                0x4000
//#define XINPUT_GAMEPAD_Y                0x8000

enum
{
	XINPUT_VIBRATION_BOTH,
	XINPUT_VIBRATION_LEFT,
	XINPUT_VIBRATION_RIGHT
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInst, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

//---------------------------- keyboard
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRepeat(int nKey);
bool GetKeyboardRelease(int nKey);

//---------------------------- mobUse
BOOL IsMobUseLeftPressed(void);      // 左クリックした状態
BOOL IsMobUseLeftTriggered(void);    // 左クリックした瞬間
BOOL IsMobUseRightPressed(void);     // 右クリックした状態
BOOL IsMobUseRightTriggered(void);   // 右クリックした瞬間
BOOL IsMobUseCenterPressed(void);    // 中クリックした状態
BOOL IsMobUseCenterTriggered(void);  // 中クリックした瞬間
long GetMobUseX(void);               // マウスがX方向に動いた相対値
long GetMobUseY(void);               // マウスがY方向に動いた相対値
long GetMobUseZ(void);               // マウスホイールが動いた相対値

									 //---------------------------- game pad
BOOL IsButtonPressed(int padNo, DWORD button);
BOOL IsButtonTriggered(int padNo, DWORD button);

bool XButtonPress(int padNo, DWORD button);
bool XButtonTrigger(int padNo, DWORD button);
bool XButtonRelease(int padNo, DWORD button);
int GetXInputPacCount(void);
void SetXInputVibration(int padNo, int nFlag, WORD wVib);
#endif