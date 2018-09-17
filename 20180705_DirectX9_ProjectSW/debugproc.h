//=============================================================================
//
// �f�o�b�O�\������ [debugproc.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define DEBUG_SCROLL_SPEED		(5)
#define DEBUG_MSECOND			(1000)

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class Debugtimer
{
private:
	LARGE_INTEGER 	 m_liStart, m_liFreq;
public:
	void			Restart(void);
	double			End(void);
	Debugtimer(void) { Restart(); }
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitDebugProc(void);
void UninitDebugProc(void);
void UpdateDebugProc(void);
void DrawDebugProc(void);
void DebugHowTo(void);

void PrintDebugProc(char *fmt,...);

#endif