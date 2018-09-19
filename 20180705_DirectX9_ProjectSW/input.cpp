#pragma comment (lib, "xinput.lib")
//=============================================================================
//
// ���͏��� [input.cpp]
// Author :  GP12A295 25 �l���F��
//
//=============================================================================
#include "input.h"

// �f�o�b�O�p
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	NUM_KEY_MAX			(256)

// game pad�p�ݒ�l
#define DEADZONE		2500			// �e����25%�𖳌��]�[���Ƃ���
#define RANGE_MAX		1000			// �L���͈͂̍ő�l
#define RANGE_MIN		-1000			// �L���͈͂̍ŏ��l


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd);
void UninitKeyboard(void);
HRESULT UpdateKeyboard(void);

HRESULT InitializeMobUse(HINSTANCE hInst, HWND hWindow); // �}�E�X�̏�����
void UninitMobUse();						// �}�E�X�̏I������
HRESULT UpdateMobUse();					// �}�E�X�̍X�V����

HRESULT InitializePad(void);			// �p�b�h������
										//BOOL CALLBACK SearchPadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID);	// �p�b�h�����R�[���o�b�N
void UpdatePad(void);
void UninitPad(void);

HRESULT InitXinput(void);
void UpdateXinput(void);
void UninitXinput(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//------------------------------- keyboard
LPDIRECTINPUT8			g_pDInput = NULL;					// IDirectInput8�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECTINPUTDEVICE8	g_pDIDevKeyboard = NULL;			// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^(�L�[�{�[�h)
BYTE					g_keyState[NUM_KEY_MAX];			// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					g_keyStateTrigger[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					g_keyStateRepeat[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					g_keyStateRelease[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
int						g_keyStateRepeatCnt[NUM_KEY_MAX];	// �L�[�{�[�h�̃��s�[�g�J�E���^

															//--------------------------------- mobUse
static LPDIRECTINPUTDEVICE8 pMobUse = NULL; // mobUse

static DIMOUSESTATE2	mobUseState;		// �}�E�X�̃_�C���N�g�ȏ��
static DIMOUSESTATE2	mobUseTrigger;		// �����ꂽ�u�Ԃ���ON
static int				g_nMobCount;
										//--------------------------------- game pad

static LPDIRECTINPUTDEVICE8	pGamePad[GAMEPADMAX] = { NULL,NULL,NULL,NULL };// �p�b�h�f�o�C�X

static DWORD	padState[GAMEPADMAX];	// �p�b�h���i�����Ή��j
static DWORD	padTrigger[GAMEPADMAX];
static DWORD	padRelease[GAMEPADMAX];

static int		padCount = 0;			// ���o�����p�b�h�̐�

float			padlX;
float			padlY;
LONG			padlRx;
float			padlRy;
float			padlZ;
float			padlRz;
D3DXVECTOR3		g_vecGyro;
DIDEVCAPS		g_diDevCaps;

// �W���C���e�X�g�p
LONG			lGyroX = 0, lGyroY = 0, lGyroZ = 0;
float			fGyroX = 0.0f, fGyroY = 0.0f, fGyroZ = 0.0f;
float			fPreX = 0.0f, fPreY = 0.0f, fPreZ = 0.0f;

int				g_nJoyconSlider;

/* XInput */
static XINPUT_STATE		g_xState[GAMEPADMAX_XINPUT];
static XINPUT_VIBRATION	g_xVibration[GAMEPADMAX_XINPUT];

static DWORD			g_dwPadState[GAMEPADMAX_XINPUT];	// �p�b�h���i�����Ή��j
static DWORD			g_dwPadTrigger[GAMEPADMAX_XINPUT];
static DWORD			g_dwPadRelease[GAMEPADMAX_XINPUT];

static int				g_nPadCountX = 0;			// ���o�����p�b�h�̐�

													// �f�o�b�O�p
#ifdef _DEBUG
 bool					g_bDebug;					// �f�o�b�O�t�H���g�\���t���O
#endif


//=============================================================================
// ���͏����̏�����
//=============================================================================
HRESULT InitInput(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	if (!g_pDInput)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&g_pDInput, NULL);
	}

	// �L�[�{�[�h�̏�����
	InitKeyboard(hInst, hWnd);

	// �}�E�X�̏�����
	InitializeMobUse(hInst, hWnd);

	// �p�b�h�̏�����
	InitializePad();

	// XInput�̏�����
	InitXinput();

	return S_OK;
}

//=============================================================================
// ���͏����̏I������
//=============================================================================
void UninitInput(void)
{
	// �L�[�{�[�h�̏I������
	UninitKeyboard();

	// �}�E�X�̏I������
	UninitMobUse();

	// �p�b�h�̏I������
	UninitPad();

	// XInput�̏�����
	UninitXinput();

	if (g_pDInput)
	{
		g_pDInput->Release();
		g_pDInput = NULL;
	}
}

//=============================================================================
// ���͏����̍X�V����
//=============================================================================
void UpdateInput(void)
{
#ifdef _DEBUG
	PrintDebugProc("�yINPUT�z\n");
	PrintDebugProc("�b�L�[�F�p�b�h��ԕ\��\n");
	PrintDebugProc("\n");
#endif

	// �L�[�{�[�h�̍X�V
	UpdateKeyboard();

	// �}�E�X�̍X�V
	UpdateMobUse();

	// �p�b�h�̍X�V
	//UpdatePad();

	// XInput�̏�����
	UpdateXinput();

	// �f�o�b�O���̕\����\��
	if (GetKeyboardTrigger(DIK_C))
	{
		g_bDebug = g_bDebug ? false : true;
	}

#ifdef _DEBUG
	//PrintDebugProc("�y INPUT �z\n");
	//PrintDebugProc("Mouse[X:%l Y:%l Z:%l]\n", GetMobUseX(), GetMobUseY(), GetMobUseZ());
	//PrintDebugProc("\n");
#endif
}

//=============================================================================
// �L�[�{�[�h�̏�����
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pDIDevKeyboard, NULL);
	if (FAILED(hr) || g_pDIDevKeyboard == NULL)
	{
		MessageBox(hWnd, "�L�[�{�[�h���˂��I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = g_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = g_pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	g_pDIDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̏I������
//=============================================================================
void UninitKeyboard(void)
{
	if (g_pDIDevKeyboard)
	{
		g_pDIDevKeyboard->Release();
		g_pDIDevKeyboard = NULL;
	}
}

//=============================================================================
// �L�[�{�[�h�̍X�V
//=============================================================================
HRESULT UpdateKeyboard(void)
{
	HRESULT hr;
	BYTE keyStateOld[256];

	// �O��̃f�[�^��ۑ�
	memcpy(keyStateOld, g_keyState, NUM_KEY_MAX);

	// �f�o�C�X����f�[�^���擾
	hr = g_pDIDevKeyboard->GetDeviceState(sizeof(g_keyState), g_keyState);
	if (SUCCEEDED(hr))
	{
		for (int cnt = 0; cnt < NUM_KEY_MAX; cnt++)
		{
			g_keyStateTrigger[cnt] = (keyStateOld[cnt] ^ g_keyState[cnt]) & g_keyState[cnt];
			g_keyStateRelease[cnt] = (keyStateOld[cnt] ^ g_keyState[cnt]) & ~g_keyState[cnt];
			g_keyStateRepeat[cnt] = g_keyStateTrigger[cnt];

			if (g_keyState[cnt])
			{
				g_keyStateRepeatCnt[cnt]++;
				if (g_keyStateRepeatCnt[cnt] >= 20)
				{
					g_keyStateRepeat[cnt] = g_keyState[cnt];
				}
			}
			else
			{
				g_keyStateRepeatCnt[cnt] = 0;
				g_keyStateRepeat[cnt] = 0;
			}
		}
	}
	else
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		g_pDIDevKeyboard->Acquire();
	}

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̃v���X��Ԃ��擾
//=============================================================================
bool GetKeyboardPress(int key)
{
	return (g_keyState[key] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̃g���K�[��Ԃ��擾
//=============================================================================
bool GetKeyboardTrigger(int key)
{
	return (g_keyStateTrigger[key] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̃��s�[�g��Ԃ��擾
//=============================================================================
bool GetKeyboardRepeat(int key)
{
	return (g_keyStateRepeat[key] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̃����|�X��Ԃ��擾
//=============================================================================
bool GetKeyboardRelease(int key)
{
	return (g_keyStateRelease[key] & 0x80) ? true : false;
}


//=============================================================================
// �}�E�X�֌W�̏���
//=============================================================================
// �}�E�X�̏�����
HRESULT InitializeMobUse(HINSTANCE hInst, HWND hWindow)
{
	HRESULT result;
	RECT rectMove;

	// �}�E�X�̑���J�E���g��������
	g_nMobCount = 0;

	// �f�o�C�X�쐬
	result = g_pDInput->CreateDevice(GUID_SysMouse, &pMobUse, NULL);
	if (FAILED(result) || pMobUse == NULL)
	{
		MessageBox(hWindow, "No mobUse", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}
	// �f�[�^�t�H�[�}�b�g�ݒ�
	result = pMobUse->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(result))
	{
		MessageBox(hWindow, "Can't setup mobUse", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}

	// ���̃A�v���Ƌ������[�h�ɐݒ�
	result = pMobUse->SetCooperativeLevel(hWindow, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(result))
	{
		MessageBox(hWindow, "MobUse mode error", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}


	// �f�o�C�X�̐ݒ�
	DIPROPDWORD prop;

	prop.diph.dwSize = sizeof(prop);
	prop.diph.dwHeaderSize = sizeof(prop.diph);
	prop.diph.dwObj = 0;
	prop.diph.dwHow = DIPH_DEVICE;
	prop.dwData = DIPROPAXISMODE_REL;		// �}�E�X�̈ړ��l�@���Βl

	result = pMobUse->SetProperty(DIPROP_AXISMODE, &prop.diph);
	if (FAILED(result))
	{
		MessageBox(hWindow, "MobUse property error", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}

	// �A�N�Z�X���𓾂�
	pMobUse->Acquire();

	// �}�E�X�J�[�\����\��
	//ShowCursor(false);

	SetRect(&rectMove, 0, 0, 1280 * SCREEN_SCALE, 720 * SCREEN_SCALE);
	return result;
}
//---------------------------------------------------------
void UninitMobUse()
{
	if (pMobUse)
	{
		ShowCursor(true);
		pMobUse->Unacquire();
		pMobUse->Release();
		pMobUse = NULL;
	}

}
//-----------------------------------------------------------
HRESULT UpdateMobUse()
{
	HRESULT result;
	// �O��̒l�ۑ�
	DIMOUSESTATE2 lastMobUseState = mobUseState;

	// �f�[�^�擾
	result = pMobUse->GetDeviceState(sizeof(mobUseState), &mobUseState);
	if (SUCCEEDED(result))
	{
		mobUseTrigger.lX = mobUseState.lX;
		mobUseTrigger.lY = mobUseState.lY;
		mobUseTrigger.lZ = mobUseState.lZ;
		// �}�E�X�̃{�^�����
		for (int i = 0; i<8; i++)
		{
			mobUseTrigger.rgbButtons[i] = ((lastMobUseState.rgbButtons[i] ^
				mobUseState.rgbButtons[i]) & mobUseState.rgbButtons[i]);
		}

		// �}�E�X���쒆
		if (lastMobUseState.lX != mobUseState.lX
			|| lastMobUseState.lY != mobUseState.lY
			|| lastMobUseState.lZ != mobUseState.lZ)
		{
			// �}�E�X�J�[�\���\��
			ShowCursor(true);
			// �}�E�X����J�E���g��������
			g_nMobCount = 0;
		}

	}
	else	// �擾���s
	{
		// �A�N�Z�X���𓾂Ă݂�
		result = pMobUse->Acquire();
	}

	// �}�E�X����J�E���g
	g_nMobCount++;
	if (g_nMobCount > MOUSE_COUNT_MAX)
	{
		// �}�E�X�J�[�\����\��
		ShowCursor(false);
	}

	return result;
}


//----------------------------------------------
BOOL IsMobUseLeftPressed(void)
{
	return (BOOL)(mobUseState.rgbButtons[0] & 0x80);	// �����ꂽ�Ƃ��ɗ��r�b�g������
}
BOOL IsMobUseLeftTriggered(void)
{
	return (BOOL)(mobUseTrigger.rgbButtons[0] & 0x80);
}
BOOL IsMobUseRightPressed(void)
{
	return (BOOL)(mobUseState.rgbButtons[1] & 0x80);
}
BOOL IsMobUseRightTriggered(void)
{
	return (BOOL)(mobUseTrigger.rgbButtons[1] & 0x80);
}
BOOL IsMobUseCenterPressed(void)
{
	return (BOOL)(mobUseState.rgbButtons[2] & 0x80);
}
BOOL IsMobUseCenterTriggered(void)
{
	return (BOOL)(mobUseTrigger.rgbButtons[2] & 0x80);
}
//------------------
long GetMobUseX(void)
{
	return mobUseState.lX;
}
long GetMobUseY(void)
{
	return mobUseState.lY;
}
long GetMobUseZ(void)
{
	return mobUseState.lZ;
}
//================================================= game pad
//---------------------------------------- �R�[���o�b�N�֐�
BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID)
{
	HRESULT result;

	result = g_pDInput->CreateDevice(lpddi->guidInstance, &pGamePad[padCount++], NULL);
	return DIENUM_CONTINUE;	// ���̃f�o�C�X���

}
//---------------------------------------- ������
HRESULT InitializePad(void)			// �p�b�h������
{
	HRESULT		result;
	int			i;

	padCount = 0;
	// �W���C�p�b�h��T��
	g_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);
	// �Z�b�g�����R�[���o�b�N�֐����A�p�b�h�𔭌������������Ă΂��B

	for (i = 0; i<padCount; i++) {
		// �W���C�X�e�B�b�N�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
		result = pGamePad[i]->SetDataFormat(&c_dfDIJoystick2);
		if (FAILED(result))
			return false; // �f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s

						  // ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
						  //		result = pGamePad[i]->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
						  //		if ( FAILED(result) )
						  //			return false; // ���[�h�̐ݒ�Ɏ��s

						  // ���̒l�͈̔͂�ݒ�
						  // X���AY���̂��ꂼ��ɂ��āA�I�u�W�F�N�g���񍐉\�Ȓl�͈̔͂��Z�b�g����B
						  // (max-min)�́A�ő�10,000(?)�B(max-min)/2�������l�ɂȂ�B
						  // ����傫������΁A�A�i���O�l�ׂ̍��ȓ�����߂炦����B(�p�b�h�̐��\�ɂ��)
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = RANGE_MIN;
		diprg.lMax = RANGE_MAX;
		// X���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_X;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Z���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_Z;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// RX���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_RX;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// RY���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_RY;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// GYRO�p��RANGE��ݒ�
		diprg.lMin = RANGE_MIN_GYRO;
		diprg.lMax = RANGE_MAX_GYRO;
		// RZ���͈̔͂�ݒ�iZ��]�j
		diprg.diph.dwObj = DIJOFS_RZ;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// SLIDER(0)�͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_SLIDER(0);
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// SLIDER(1)�͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_SLIDER(1);
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);



		// �e�����ƂɁA�����̃]�[���l��ݒ肷��B
		// �����]�[���Ƃ́A��������̔����ȃW���C�X�e�B�b�N�̓����𖳎�����͈͂̂��ƁB
		// �w�肷��l�́A10000�ɑ΂��鑊�Βl(2000�Ȃ�20�p�[�Z���g)�B
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = DEADZONE;
		//X���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_X;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		//Y���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// Z���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_Z;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		// RX���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_RX;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		// RY���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_RY;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		//// GYRO�p��RANGE��ݒ�
		//dipdw.dwData = DEADZONE_GYRO;
		//// RZ���̖����]�[����ݒ�iZ��]�j
		//dipdw.diph.dwObj = DIJOFS_RZ;
		//pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		//// SLIDER(0)�̖����]�[����ݒ�
		//dipdw.diph.dwObj = DIJOFS_SLIDER(0);
		//pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		//// SLIDER(1)�̖����]�[����ݒ�
		//dipdw.diph.dwObj = DIJOFS_SLIDER(1);
		//pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		//�W���C�X�e�B�b�N���͐���J�n
		pGamePad[i]->Acquire();
	}

	return true;

}
//------------------------------------------- �I������
void UninitPad(void)
{
	for (int i = 0; i<GAMEPADMAX; i++) {
		if (pGamePad[i])
		{
			pGamePad[i]->Unacquire();
			pGamePad[i]->Release();
		}
	}

}

//------------------------------------------ �X�V
void UpdatePad(void)
{


	HRESULT			result;
	DIJOYSTATE2		dijs;
	int				i;

	for (i = 0; i<padCount; i++)
	{
		DWORD lastPadState;
		lastPadState = padState[i];
		padState[i] = 0x00000000l;	// ������

		result = pGamePad[i]->Poll();	// �W���C�X�e�B�b�N�Ƀ|�[����������
		if (FAILED(result)) {
			result = pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = pGamePad[i]->Acquire();
		}

		result = pGamePad[i]->GetDeviceState(sizeof(DIJOYSTATE2), &dijs);	// �f�o�C�X��Ԃ�ǂݎ��
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
			result = pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = pGamePad[i]->Acquire();
		}

		// �R�Q�̊e�r�b�g�ɈӖ����������A�{�^�������ɉ����ăr�b�g���I���ɂ���

		//* �����L�[��
		if (dijs.rgdwPOV[0] == 0)		padState[i] |= BUTTON_UP;
		//* �����L�[��
		if (dijs.rgdwPOV[0] == 18000)	padState[i] |= BUTTON_DOWN;
		//* �����L�[��
		if (dijs.rgdwPOV[0] == 27000)	padState[i] |= BUTTON_LEFT;
		//* �����L�[�E
		if (dijs.rgdwPOV[0] == 9000)	padState[i] |= BUTTON_RIGHT;

		//* �����L�[�E��
		if (dijs.rgdwPOV[0] == 4500)	padState[i] |= BUTTON_UP, padState[i] |= BUTTON_RIGHT;
		//* �����L�[�E��
		if (dijs.rgdwPOV[0] == 13500)	padState[i] |= BUTTON_DOWN, padState[i] |= BUTTON_RIGHT;
		//* �����L�[����
		if (dijs.rgdwPOV[0] == 22500)	padState[i] |= BUTTON_DOWN, padState[i] |= BUTTON_LEFT;
		//* �����L�[����
		if (dijs.rgdwPOV[0] == 31500)	padState[i] |= BUTTON_UP, padState[i] |= BUTTON_LEFT;

		//* �`�{�^��
		if (dijs.rgbButtons[0] & 0x80)	padState[i] |= BUTTON_A;
		//* �a�{�^��
		if (dijs.rgbButtons[1] & 0x80)	padState[i] |= BUTTON_B;
		//* �b�{�^��
		if (dijs.rgbButtons[2] & 0x80)	padState[i] |= BUTTON_C;
		//* �w�{�^��
		if (dijs.rgbButtons[3] & 0x80)	padState[i] |= BUTTON_X;
		////* �x�{�^��
		//if (dijs.rgbButtons[4] & 0x80)	padState[i] |= BUTTON_Y;
		////* �y�{�^��
		//if (dijs.rgbButtons[5] & 0x80)	padState[i] |= BUTTON_Z;
		//* �k�{�^��
		if (dijs.rgbButtons[6] & 0x80)	padState[i] |= BUTTON_L;
		//* �q�{�^��
		if (dijs.rgbButtons[7] & 0x80)	padState[i] |= BUTTON_R;

		//* �L���v�`���[�{�^��[13]
		if (dijs.rgbButtons[13] & 0x80)	padState[i] |= BUTTON_CAP;
		//* Home�{�^��[28]
		if (dijs.rgbButtons[28] & 0x80)	padState[i] |= BUTTON_HOME;
		//* �r�s�`�q�s�{�^��
		if (dijs.rgbButtons[9] & 0x80)	padState[i] |= BUTTON_START;
		//* �l�{�^��
		if (dijs.rgbButtons[8] & 0x80)	padState[i] |= BUTTON_MINUS;

		//*
		if (dijs.rgbButtons[16] & 0x80)	padState[i] |= R_BUTTON_Y;
		//*
		if (dijs.rgbButtons[17] & 0x80)	padState[i] |= R_BUTTON_X;
		//*
		if (dijs.rgbButtons[18] & 0x80)	padState[i] |= R_BUTTON_B;
		//*
		if (dijs.rgbButtons[19] & 0x80)	padState[i] |= R_BUTTON_A;
		//*
		if (dijs.rgbButtons[22] & 0x80)	padState[i] |= R_BUTTON_R;
		//*
		if (dijs.rgbButtons[23] & 0x80)	padState[i] |= R_BUTTON_ZR;
		//*
		if (dijs.rgbButtons[25] & 0x80)	padState[i] |= R_BUTTON_PLUS;
		//*
		if (dijs.rgbButtons[21] & 0x80)	padState[i] |= R_BUTTON_LEFT;

		//* y-axis (forward)
		if (dijs.lY < 0)				padState[i] |= LSTICK_UP;
		//* y-axis (backward)
		if (dijs.lY > 0)				padState[i] |= LSTICK_DOWN;
		//* x-axis (left)
		if (dijs.lX < 0)				padState[i] |= LSTICK_LEFT;
		//* x-axis (right)
		if (dijs.lX > 0)				padState[i] |= LSTICK_RIGHT;


		//* �E�X�e�B�b�N��
		if (dijs.lRy < 0)				padState[i] |= RSTICK_UP;
		//* �E�X�e�B�b�N��
		if (dijs.lRy > 0)				padState[i] |= RSTICK_DOWN;
		//* �E�X�e�B�b�N��
		if (dijs.lRx < 0)				padState[i] |= RSTICK_LEFT;
		//* �E�X�e�B�b�N�E
		if (dijs.lRx > 0)				padState[i] |= RSTICK_RIGHT;


		////* �E�X�e�B�b�N��
		//if (dijs.lRz < 0x8000 - RSTICK_MARGIN)	padState[i] |= RSTICK_UP;
		////* �E�X�e�B�b�N��
		//if (dijs.lRz > 0x8000 + RSTICK_MARGIN)	padState[i] |= RSTICK_DOWN;
		////* �E�X�e�B�b�N��
		//if (dijs.lZ < 0x8000 - RSTICK_MARGIN)	padState[i] |= RSTICK_LEFT;
		////* �E�X�e�B�b�N�E
		//if (dijs.lZ > 0x8000 + RSTICK_MARGIN)	padState[i] |= RSTICK_RIGHT;

		//* ���X�e�B�b�N�{�^��
		if (dijs.rgbButtons[10] & 0x80)	padState[i] |= LSTICK_BUTTON;
		//* �E�X�e�B�b�N�{�^��
		if (dijs.rgbButtons[11] & 0x80)	padState[i] |= RSTICK_BUTTON;


		// Trigger�ݒ�
		padTrigger[i] = ((lastPadState ^ padState[i])	// �O��ƈ���Ă���
			& padState[i]);					// ��������ON�̂��
											// Release�ݒ�
		padRelease[i] = ((lastPadState ^ padState[i])	// �O��ƈ���Ă���
			& ~padState[i]);					// ��������OFF�̂��

		if (i == 0)
		{
			// �E�X�e�B�b�N�̌X���ʂ��i�[
			padlRx = dijs.lRx;
			padlRy = dijs.lRy;

			// ���X�e�B�b�N�̌X���ʂ��i�[
			padlZ = dijs.lZ;
			padlRz = dijs.lRz;

			// �W���C�������i�[
			g_vecGyro = D3DXVECTOR3((float)dijs.rglSlider[0], (float)dijs.rglSlider[1], dijs.lRz);
		}


#ifdef _DEBUG
		if (g_bDebug)
		{
			PrintDebugProc("�yGAME PAD�z\n");
			PrintDebugProc("LStick[X:%l  Y:%l  Z:%l]  RStick[X:%l  Y:%l  Z:%l]\n",
				dijs.lX, dijs.lY, dijs.lZ, dijs.lRx, dijs.lRy, dijs.lRz);
			PrintDebugProc("POV[UP:%d  RIGHT:%d  DOWN:%d  LEFT:%d]\n",
				dijs.rgdwPOV[0], dijs.rgdwPOV[1], dijs.rgdwPOV[2], dijs.rgdwPOV[3]);
			PrintDebugProc("lV[X:%l  Y:%l  Z:%l]  lVR[X:%l  Y:%l  Z:%l]\n",
				dijs.lVX, dijs.lVY, dijs.lVZ, dijs.lVRx, dijs.lVRy, dijs.lVRz);
			PrintDebugProc("lA[X:%l  Y:%l  Z:%l]  lAR[X:%l  Y:%l  Z:%l]\n",
				dijs.lAX, dijs.lAY, dijs.lAZ, dijs.lARx, dijs.lARy, dijs.lARz);
			PrintDebugProc("lF[X:%l  Y:%l  Z:%l]  lFR[X:%l  Y:%l  Z:%l]\n",
				dijs.lFX, dijs.lFY, dijs.lFZ, dijs.lFRx, dijs.lFRy, dijs.lFRz);
			PrintDebugProc("rglSlider[0:%l  1:%l]  rglVSlider[0:%l  1:%l]\n",
				dijs.rglSlider[0], dijs.rglSlider[1], dijs.rglVSlider[0], dijs.rglVSlider[1]);
			PrintDebugProc("rglASlider[0:%l  1:%l]  rglFSlider[0:%l  1:%l]\n",
				dijs.rglASlider[0], dijs.rglASlider[1], dijs.rglFSlider[0], dijs.rglFSlider[1]);
			PrintDebugProc("rgbButtons\n");

			if (GetKeyboardTrigger(DIK_R))
			{
				lGyroX = 0;
				lGyroY = 0;
				lGyroZ = 0;
				fGyroX = 0.0f;
				fGyroY = 0.0f;
				fGyroZ = 0.0f;
			}
			lGyroX += dijs.lRz;
			lGyroY += dijs.rglSlider[1];
			lGyroZ += dijs.rglSlider[0];

			float fDegree;

			fDegree = (fPreX + dijs.lRz) * 15 / 2;
			fGyroX += fDegree * 0.0174533f;

			fDegree = (fPreY + dijs.rglSlider[1]) * 15 / 2;
			fGyroY += fDegree * 0.0174533f;

			fDegree = (fPreZ + dijs.rglSlider[0]) * 15 / 2;
			fGyroZ += fDegree * 0.0174533f;


			fPreX = dijs.lRz;
			fPreY = dijs.rglSlider[1];
			fPreZ = dijs.rglSlider[0];


			PrintDebugProc("lGyro[X:%l  Y:%l  Z:%l]\n",
				lGyroX, lGyroY, lGyroZ);

			PrintDebugProc("fGyro[X:%f  Y:%f  Z:%f]\n",
				fGyroX, fGyroY, fGyroZ);



			for (int i = 0; i < 128; i++)
			{
				PrintDebugProc("%d", dijs.rgbButtons[i]);
				if (i % 32 == 0 && i != 0)
				{
					PrintDebugProc("\n");
				}
			}
			PrintDebugProc("[%f] [%f]\n", (float)padlZ, (float)padlRz);
			PrintDebugProc("\n");
		}
#endif
	}

}
//----------------------------------------------- ����
BOOL IsButtonPressed(int padNo, DWORD button)
{
	return (button & padState[padNo]);
}

BOOL IsButtonTriggered(int padNo, DWORD button)
{
	return (button & padTrigger[padNo]);
}

BOOL IsButtonReleased(int padNo, DWORD button)
{
	return (button & padRelease[padNo]);
}


//=============================================================================
// XInput����������
//=============================================================================
HRESULT InitXinput(void)
{
	DWORD dwResult;
	for (DWORD i = 0; i< GAMEPADMAX_XINPUT; i++)
	{
		ZeroMemory(&g_xState[i], sizeof(XINPUT_STATE));
		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState( i, &g_xState[i]);
		if( dwResult == ERROR_SUCCESS )
		{
			// Controller is connected
			g_nPadCountX++;
			ZeroMemory(&g_xVibration[i], sizeof(XINPUT_VIBRATION));
			ZeroMemory(&g_dwPadState[i], sizeof(DWORD));
			ZeroMemory(&g_dwPadTrigger[i], sizeof(DWORD));
			ZeroMemory(&g_dwPadRelease[i], sizeof(DWORD));
		}
		else
		{
			// Controller is not connected
		}
	}

	return S_OK;
}

//=============================================================================
// XInput�I������
//=============================================================================
void UninitXinput(void)
{

}

//=============================================================================
// XInput�X�V����
//=============================================================================
void UpdateXinput(void)
{
	XINPUT_STATE xState;

	for (DWORD i = 0; i < g_nPadCountX; i++)
	{
		ZeroMemory(&xState, sizeof(XINPUT_STATE));
		// Simply get the state of the controller from XInput.
		XInputGetState(i, &xState);

		// �X�e�[�g���O���[�o���ɕۑ�
		// ��ɔ͈͂��ݒ�ł���X�e�B�b�N�E�g���K�[����
		g_xState[i] = xState;

		// �X�e�[�g���R�s�[
		DWORD dwLastPadState = g_dwPadState[i];

		// �X�e�B�b�N�E�g���K�[���{�^���Ƃ��Ĉ�����悤
		// WORD�^����DWORD�`�֊g������
		g_dwPadState[i] = (DWORD)xState.Gamepad.wButtons;

		// �X�e�B�b�N�E�g���K�[���X�e�[�g�ɒǉ��i�[
		if (xState.Gamepad.sThumbLY > 0)		g_dwPadState[i] |= XBOTTON_STICK_UP;
		if (xState.Gamepad.sThumbLY < 0)		g_dwPadState[i] |= XBOTTON_STICK_DOWN;
		if (xState.Gamepad.sThumbLX < 0)		g_dwPadState[i] |= XBOTTON_STICK_LEFT;
		if (xState.Gamepad.sThumbLX > 0)		g_dwPadState[i] |= XBOTTON_STICK_RIGHT;

		if (xState.Gamepad.sThumbRY > 0)		g_dwPadState[i] |= XBOTTON_STICKR_UP;
		if (xState.Gamepad.sThumbRY < 0)		g_dwPadState[i] |= XBOTTON_STICKR_DOWN;
		if (xState.Gamepad.sThumbRX < 0)		g_dwPadState[i] |= XBOTTON_STICKR_LEFT;
		if (xState.Gamepad.sThumbRX > 0)		g_dwPadState[i] |= XBOTTON_STICKR_RIGHT;

		if (xState.Gamepad.bLeftTrigger > 0)	g_dwPadState[i] |= XBOTTON_LT;
		if (xState.Gamepad.bRightTrigger > 0)	g_dwPadState[i] |= XBOTTON_RT;

		// Trigger�ݒ�
		g_dwPadTrigger[i] = ((dwLastPadState ^ g_dwPadState[i])	// �O��ƈ���Ă���
			& g_dwPadState[i]);					// ��������ON�̂��

		// Release�ݒ�
		g_dwPadRelease[i] = ((dwLastPadState ^ g_dwPadState[i])	// �O��ƈ���Ă���
			& ~g_dwPadState[i]);					// ��������OFF�̂��

		// �o�C�u���[�V������ݒ�
		XInputSetState(i, &g_xVibration[i]);

#ifdef _DEBUG
		// �o�C�u���[�V�����e�X�g
		if (XButtonPress(i, XINPUT_GAMEPAD_LEFT_THUMB))
		{	// ���E�E���[�^�[�̉�]�����グ��
			g_xVibration[i].wLeftMotorSpeed = 
				min(g_xVibration[i].wLeftMotorSpeed + XINPUT_VIBRATION_DEBUG, XINPUT_VIBRATION_MAX);
			g_xVibration[i].wRightMotorSpeed =
				min(g_xVibration[i].wRightMotorSpeed + XINPUT_VIBRATION_DEBUG, XINPUT_VIBRATION_MAX);
		}
		if (XButtonPress(i, XINPUT_GAMEPAD_RIGHT_THUMB))
		{	// ���E�E���[�^�[�̉�]����������
			g_xVibration[i].wLeftMotorSpeed =
				max(g_xVibration[i].wLeftMotorSpeed - XINPUT_VIBRATION_DEBUG, 0);
			g_xVibration[i].wRightMotorSpeed =
				max(g_xVibration[i].wRightMotorSpeed - XINPUT_VIBRATION_DEBUG, 0);
		}

		if (g_bDebug)
		{
			// �f�o�b�O�t�H���g�\��
			PrintDebugProc("�yXINPUT�z\n");
			//PrintDebugProc("���X�e�B�b�N����:�o�C�u��\n");
			//PrintDebugProc("�E�X�e�B�b�N����:�o�C�u��\n");
			PrintDebugProc("PAD       [%d]   PacketNum[%d]\n", i, xState.dwPacketNumber);
			PrintDebugProc("THUMB     [LX:%d LY:%d RX:%d RY:%d]\n",
				xState.Gamepad.sThumbLX, xState.Gamepad.sThumbLY,
				xState.Gamepad.sThumbRX, xState.Gamepad.sThumbRY);
			PrintDebugProc("TRIGGER   [LEFT:%d RIGHT:%d]\n",
				xState.Gamepad.bLeftTrigger, xState.Gamepad.bRightTrigger);
			PrintDebugProc("DPAD      [UP:%d DOWN:%d LEFT:%d RIGHT:%d]\n",
				XButtonPress(i, XBOTTON_DPAD_UP),
				XButtonPress(i, XBOTTON_DPAD_DOWN),
				XButtonPress(i, XBOTTON_DPAD_LEFT),
				XButtonPress(i, XBOTTON_DPAD_RIGHT));
			PrintDebugProc("LSTICK    [UP:%d DOWN:%d LEFT:%d RIGHT:%d]\n",
				XButtonPress(i, XBOTTON_STICK_UP),
				XButtonPress(i, XBOTTON_STICK_DOWN),
				XButtonPress(i, XBOTTON_STICK_LEFT),
				XButtonPress(i, XBOTTON_STICK_RIGHT));
			PrintDebugProc("RSTICK    [UP:%d DOWN:%d LEFT:%d RIGHT:%d]\n",
				XButtonPress(i, XBOTTON_STICKR_UP),
				XButtonPress(i, XBOTTON_STICKR_DOWN),
				XButtonPress(i, XBOTTON_STICKR_LEFT),
				XButtonPress(i, XBOTTON_STICKR_RIGHT));
			PrintDebugProc("THUMB     [LEFT:%d RIGHT:%d]\n",
				XButtonPress(i, XBOTTON_LEFT_THUMB),
				XButtonPress(i, XBOTTON_RIGHT_THUMB));
			PrintDebugProc("SHOULDER  [LEFT:%d RIGHT:%d]\n",
				XButtonPress(i, XBOTTON_LEFT_SHOULDER),
				XButtonPress(i, XBOTTON_RIGHT_SHOULDER));
			PrintDebugProc("BUTTON    [A:%d B:%d X:%d Y:%d START:%d BACK:%d]\n",
				XButtonPress(i, XBOTTON_A),
				XButtonPress(i, XBOTTON_B),
				XButtonPress(i, XBOTTON_X),
				XButtonPress(i, XBOTTON_Y),
				XButtonPress(i, XBOTTON_START),
				XButtonPress(i, XBOTTON_BACK));
			PrintDebugProc("          [LT:%d RT:%d START:%d BACK:%d]\n",
				XButtonPress(i, XBOTTON_LT),
				XButtonPress(i, XBOTTON_RT),
				XButtonPress(i, XBOTTON_START),
				XButtonPress(i, XBOTTON_BACK));
			PrintDebugProc("VIBRATION [LEFT:%d RIGHT:%d]\n",
				g_xVibration[i].wLeftMotorSpeed, g_xVibration[i].wRightMotorSpeed);
			PrintDebugProc("\n");
		}
#endif
	}
}

bool XButtonPress(int padNo, DWORD button)
{
	return (button & g_dwPadState[padNo]);
	//return (button & g_xState[padNo].Gamepad.wButtons);
}

bool XButtonTrigger(int padNo, DWORD button)
{
	return (button & g_dwPadTrigger[padNo]);
}

bool XButtonRelease(int padNo, DWORD button)
{
	return (button & g_dwPadRelease[padNo]);
}

//=============================================================================
// XInput�̃Q�[���p�b�h���̎擾�֐�
//=============================================================================
int GetXInputPacCount(void)
{
	return g_nPadCountX;
}

//=============================================================================
// XInput�p�o�C�u���[�V�����ݒ菈���iwVib�F0�`65535�j
//=============================================================================
void SetXInputVibration(int padNo, int nFlag, WORD wVib)
{
	if (nFlag == XINPUT_VIBRATION_BOTH)
	{
		g_xVibration[padNo].wLeftMotorSpeed = wVib;
		g_xVibration[padNo].wRightMotorSpeed = wVib;
	}
	else if (nFlag == XINPUT_VIBRATION_LEFT)
	{
		g_xVibration[padNo].wLeftMotorSpeed = wVib;
	}
	else if (nFlag == XINPUT_VIBRATION_RIGHT)
	{
		g_xVibration[padNo].wRightMotorSpeed = wVib;
	}
}

bool InputPress(INPUT_CHECK eInput)
{
	int nKey = 0;
	DWORD dwButton = 0, dwXButton = 0;

	switch (eInput)
	{
	case INPUT_UP:
		nKey = DIK_W;
		dwButton |= BUTTON_UP;
		dwXButton |= XBOTTON_DPAD_UP;
		break;
	case INPUT_DOWN:
		nKey = DIK_S;
		dwButton |= BUTTON_DOWN;
		dwXButton |= XBOTTON_DPAD_DOWN;
		break;
	case INPUT_LEFT:
		nKey = DIK_A;
		dwButton |= BUTTON_LEFT;
		dwXButton |= XBOTTON_DPAD_LEFT;
		break;
	case INPUT_RIGHT:
		nKey = DIK_D;
		dwButton |= BUTTON_RIGHT;
		dwXButton |= XBOTTON_DPAD_RIGHT;
		break;
	case INPUT_UP_R:
		nKey = DIK_UP;
		break;
	case INPUT_DOWN_R:
		nKey = DIK_DOWN;
		break;
	case INPUT_LEFT_R:
		nKey = DIK_LEFT;
		break;
	case INPUT_RIGHT_R:
		nKey = DIK_RIGHT;
		break;
	}
	
	if ((g_keyState[nKey] & 0x80) ? true : false
		|| (dwButton & padState[0])
		|| (dwXButton & g_dwPadState[0]))
	{
		return true;
	}
	return false;
}


float GetButtonlZ(int padNo)
{
	if (padlZ < 0.0f) padlZ *= -1.0f;
	padlZ = padlZ / RANGE_MAX;
#ifdef _DEBUG
	PrintDebugProc("padlZ[%f]\n", padlZ);
#endif
	return (padlZ);
}

float GetButtonlRz(int padNo)
{
	if (padlRz < 0.0f) padlRz *= -1.0f;
	//padlRz = padlRz / RANGE_MAX;
#ifdef _DEBUG
	PrintDebugProc("padlRz[%f]\n", padlRz);
#endif
	return (padlRz);
}

//=============================================================================
// �X�e�B�b�N���擾�֐�
//=============================================================================
float GetStick(int padNo, int nStick)
{
	float fStick = 0.0f;
	switch (nStick)
	{
	case PAD_STICK_R_X:
		fStick = (float)padlRx / RANGE_MAX;
		break;
	case PAD_STICK_R_Y:
		fStick = (float)padlRy / RANGE_MAX;
		break;
	}
	return (fStick);
}

//=============================================================================
// �W���C�����擾�֐�
//=============================================================================
D3DXVECTOR3 GetGyro(void)
{
	float fSlider = 0.0f;
	D3DXVECTOR3 vecGyro;
	vecGyro = g_vecGyro * PAD_SLIDER_SPEED * g_nJoyconSlider;
	return (vecGyro);
}

