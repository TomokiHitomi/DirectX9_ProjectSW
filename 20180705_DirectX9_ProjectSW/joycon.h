//=============================================================================
//
// Joy-con���� [joycon.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _JOYCON_H_
#define _JOYCON_H_

/*******************************************************************************
* �C���N���[�h
*******************************************************************************/
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define JOYCON_MAX			(2*2)			// �����ɐڑ�����Joycon�̍ő吔���Z�b�g

/* Joycon��� */
#define JC_L_BUTTON_UP		0x00000001l	// 
#define JC_L_BUTTON_DOWN	0x00000002l	// 
#define JC_L_BUTTON_LEFT	0x00000004l	// 
#define JC_L_BUTTON_RIGHT	0x00000008l	// 
#define JC_L_BUTTON_L		0x00000010l	// 
#define JC_L_BUTTON_ZL		0x00000020l	// 
#define JC_L_BUTTON_SL		0x00000040l	// 
#define JC_L_BUTTON_SR		0x00000080l	// 
#define JC_L_BUTTON_MINUS	0x00000100l	// LEFT�{�^��
#define JC_L_BUTTON_CAP		0x00000200l	// �L���v�`���[�{�^��[13]
#define JC_L_BUTTON_STICK	0x00000400l	// 
#define JC_L_STICK_UP		0x00000800l	// 
#define JC_L_STICK_DOWN		0x00001000l	// 
#define JC_L_STICK_LEFT		0x00002000l	// 
#define JC_L_STICK_RIGHT	0x00004000l	// 

#define JC_R_BUTTON_A		0x00010000l	// 
#define JC_R_BUTTON_B		0x00020000l	// 
#define JC_R_BUTTON_X		0x00040000l	// 
#define JC_R_BUTTON_Y		0x00080000l	// 
#define JC_R_BUTTON_R		0x00100000l	// 
#define JC_R_BUTTON_ZR		0x00200000l	// 
#define JC_R_BUTTON_SL		0x00400000l	// 
#define JC_R_BUTTON_SR		0x00800000l	// 
#define JC_R_BUTTON_PLUS	0x01000000l	// LEFT�{�^��
#define JC_R_BUTTON_HOME	0x02000000l	// �L���v�`���[�{�^��[13]
#define JC_R_BUTTON_STICK	0x04000000l	// 
#define JC_R_STICK_UP		0x08000000l	// 
#define JC_R_STICK_DOWN		0x10000000l	// 
#define JC_R_STICK_LEFT		0x20000000l	// 
#define JC_R_STICK_RIGHT	0x40000000l	// 

#define JC_STICK_MARGIN		(0.5f)		// 

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void start();
void pollLoop();
void actuallyQuit();
D3DXVECTOR3 GetJoyconAccel(int jcNo);
D3DXVECTOR3 GetJoyconGyro(int jcNo);
void JoyconUpdate(void);
BOOL JcPressed(int jcNo, DWORD button);
BOOL JcTriggered(int jcNo, DWORD button);
BOOL JcReleased(int jcNo, DWORD button);


#endif
