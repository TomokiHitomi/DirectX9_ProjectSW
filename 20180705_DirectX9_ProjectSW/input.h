//=============================================================================
//
// ���͏��� [input.h]
// Author :  GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"
#include <Xinput.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************

// �v���O������������Ƃ��Ɏg��
#define	USE_KEYBOARD										// �錾����ƃL�[�{�[�h�ő���\�ɂȂ�
#define	USE_MOUSE											// �錾����ƃ}�E�X�ő���\�ɂȂ�
#define	USE_PAD												// �錾����ƃp�b�h�ő���\�ɂȂ�

/* mouse */
#define MOUSE_COUNT_MAX		(100)		// �}�E�X����J�E���g

// game pad�p�ݒ�l
#define DEADZONE		2500			// �e����25%�𖳌��]�[���Ƃ���
#define RANGE_MAX		1000			// �L���͈͂̍ő�l
#define RANGE_MIN		-1000			// �L���͈͂̍ŏ��l

//#define DEADZONE_GYRO	(250)			// �e����25%�𖳌��]�[���Ƃ���
#define RANGE_MAX_GYRO	(32767)			// �L���͈͂̍ő�l
#define RANGE_MIN_GYRO	(-32768)		// �L���͈͂̍ŏ��l

/* game pad��� */
#define BUTTON_UP		0x00000001l	// �����L�[��(.rgdwPOV == 0)
#define BUTTON_DOWN		0x00000002l	// �����L�[��(.rgdwPOV == 9000)
#define BUTTON_LEFT		0x00000004l	// �����L�[��(.rgdwPOV == 18000)
#define BUTTON_RIGHT	0x00000008l	// �����L�[�E(.rgdwPOV == 27000)
#define BUTTON_A		0x00000010l	// �`�{�^��(.rgbButtons[0]&0x80)
#define BUTTON_B		0x00000020l	// �a�{�^��(.rgbButtons[1]&0x80)
#define BUTTON_C		0x00000040l	// �b�{�^��(.rgbButtons[2]&0x80)
#define BUTTON_X		0x00000080l	// �w�{�^��(.rgbButtons[3]&0x80)
//#define BUTTON_Y		0x00000100l	// �x�{�^��(.rgbButtons[4]&0x80)
//#define BUTTON_Z		0x00000200l	// �y�{�^��(.rgbButtons[5]&0x80)
#define BUTTON_L		0x00000400l	// �k�{�^��(.rgbButtons[6]&0x80)
#define BUTTON_R		0x00000800l	// �q�{�^��(.rgbButtons[7]&0x80)
#define BUTTON_START	0x00001000l	// �r�s�`�q�s�{�^��(.rgbButtons[8]&0x80)
#define BUTTON_MINUS	0x00002000l	// �l�{�^��(.rgbButtons[9]&0x80)
#define LSTICK_UP		0x00004000l	// ���X�e�B�b�N��(.IY<0)
#define LSTICK_DOWN		0x00008000l	// ���X�e�B�b�N��(.IY>0)
#define LSTICK_LEFT		0x00010000l	// ���X�e�B�b�N��(.IX<0)
#define LSTICK_RIGHT	0x00020000l	// ���X�e�B�b�N�E(.IX>0)
#define RSTICK_UP		0x00040000l	// �E�X�e�B�b�N��(.lRz<0x8000)
#define RSTICK_DOWN		0x00080000l	// �E�X�e�B�b�N��(.lRz>0x8000)
#define RSTICK_LEFT		0x00100000l	// �E�X�e�B�b�N��(.lZ<0x8000)
#define RSTICK_RIGHT	0x00200000l	// �E�X�e�B�b�N�E(.lZ>0x8000)
#define LSTICK_BUTTON	0x00400000l	// ���X�e�B�b�N�{�^��
#define RSTICK_BUTTON	0x00800000l	// �E�X�e�B�b�N�{�^��

#define BUTTON_CAP		0x00000100l	// �L���v�`���[�{�^��[13]
#define BUTTON_HOME		0x00000200l	// Home�{�^��[28]

// Joy-con(R)�g���p
#define R_BUTTON_Y		0x01000000l	// Y�{�^��
#define R_BUTTON_X		0x02000000l	// X�{�^��
#define R_BUTTON_B		0x04000000l	// B�{�^��
#define R_BUTTON_A		0x08000000l	// A�{�^��
#define R_BUTTON_R		0x10000000l	// R�{�^��
#define R_BUTTON_ZR		0x20000000l	// ZR�{�^��
#define R_BUTTON_PLUS	0x40000000l	// PLUS�{�^��
#define R_BUTTON_LEFT	0x80000000l	// LEFT�{�^��


#define GAMEPADMAX			4			// �����ɐڑ�����W���C�p�b�h�̍ő吔���Z�b�g

#define PAD_SLIDER_MARGIN	(10)
#define PAD_SLIDER_SPEED	(0.0001f)
#define PAD_SLIDER_DEFAULT	(5)
#define PAD_SLIDER_MIN		(1)
#define PAD_SLIDER_MAX		(9)

/* XInput */
#define GAMEPADMAX_XINPUT		(4)			// �����ɐڑ�����W���C�p�b�h�̍ő吔���Z�b�g
#define XINPUT_VIBRATION_MAX	(65535)		// �o�C�u���[�V�����̍ő�l
#define XINPUT_VIBRATION_DEBUG	(500)		// �o�C�u���[�V�����̕ϓ��l�i���[�^�[��]���E�f�o�b�O�j

// �{�^�����i�ʏ�g�j
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
// �{�^�����i�g���g�j
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

// ���L�f�t�@�C����DirectX�ɂĒ�`����Ă���
// �R�����g�A�E�g���O���ׂ��炸
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

enum INPUT_CHECK
{
	INPUT_UP,
	INPUT_DOWN,
	INPUT_LEFT,
	INPUT_RIGHT,
	INPUT_UP_R,
	INPUT_DOWN_R,
	INPUT_LEFT_R,
	INPUT_RIGHT_R
};

enum
{
	PAD_STICK_L_X,
	PAD_STICK_L_Y,
	PAD_STICK_R_X,
	PAD_STICK_R_Y
};

enum
{
	PAD_SLIDER_V,
	PAD_SLIDER_H
};

enum
{
	PAD_GYRO_X,
	PAD_GYRO_Y,
	PAD_GYRO_Z
};


//*****************************************************************************
// �v���g�^�C�v�錾
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
BOOL IsMobUseLeftPressed(void);      // ���N���b�N�������
BOOL IsMobUseLeftTriggered(void);    // ���N���b�N�����u��
BOOL IsMobUseRightPressed(void);     // �E�N���b�N�������
BOOL IsMobUseRightTriggered(void);   // �E�N���b�N�����u��
BOOL IsMobUseCenterPressed(void);    // ���N���b�N�������
BOOL IsMobUseCenterTriggered(void);  // ���N���b�N�����u��
long GetMobUseX(void);               // �}�E�X��X�����ɓ��������Βl
long GetMobUseY(void);               // �}�E�X��Y�����ɓ��������Βl
long GetMobUseZ(void);               // �}�E�X�z�C�[�������������Βl

									 //---------------------------- game pad
BOOL IsButtonPressed(int padNo, DWORD button);
BOOL IsButtonTriggered(int padNo, DWORD button);
BOOL IsButtonReleased(int padNo, DWORD button);

float GetButtonlZ(int padNo);
float GetButtonlRz(int padNo);
float GetStick(int padNo, int nStick);
D3DXVECTOR3 GetGyro(void);

bool XButtonPress(int padNo, DWORD button);
bool XButtonTrigger(int padNo, DWORD button);
bool XButtonRelease(int padNo, DWORD button);
int GetXInputPacCount(void);
void SetXInputVibration(int padNo, int nFlag, WORD wVib);
bool InputPress(INPUT_CHECK eInput);
#endif