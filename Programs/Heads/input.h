//=============================================================================
//
// ���͏��� [input.h]
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_CONTROLER		(2)				// �R���g���[���ő吔(�g�����ɉ����ĕύX���邱��)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInst, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

bool GetKeyboardPress(int key);
bool GetKeyboardTrigger(int key);
bool GetKeyboardRepeat(int key);
bool GetKeyboardRelease(int key);

// �}�E�X�p(*03)
bool GetMouseLeftPress(void);
bool GetMouseLeftTrigger(void);
bool GetMouseLeftRelease(void);
bool GetMouseRightPress(void);
bool GetMouseRightTrigger(void);
bool GetMouseRightRelease(void);
bool GetMouseCenterPress(void);
bool GetMouseCenterTrigger(void);
bool GetMouseCenterRelease(void);
long GetMouseAxisX(void);
long GetMouseAxisY(void);
long GetMouseAxisZ(void);

// �W���C�X�e�B�b�N�p (*04)
bool GetJoyStickPress( int button );
bool GetJoyStickTrigger( int button );
bool GetJoyStickRelease( int button );
LONG GetJoyStickPositionX();
LONG GetJoyStickPositionY();
LONG GetJoyStickPositionZ();
LONG GetJoyStickPositionRX();
LONG GetJoyStickPositionRY();
LONG GetJoyStickPositionRZ();
bool GetJoyStickLeftTrigger(int Direction);	//	���X�e�B�b�N���X�����u�Ԃ��擾
LONG GetJoyStickSlider( int number );
DWORD GetJoyStickRgdwPOV( int number );
bool IsJoyStick(int number);
bool GetJoyStickAnyControl(void);	//	�W���C�X�e�B�b�N���쒆���̔���
#endif