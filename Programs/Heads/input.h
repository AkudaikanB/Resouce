//=============================================================================
//
// 入力処理 [input.h]
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_CONTROLER		(2)				// コントローラ最大数(使う数に応じて変更すること)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInst, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

bool GetKeyboardPress(int key);
bool GetKeyboardTrigger(int key);
bool GetKeyboardRepeat(int key);
bool GetKeyboardRelease(int key);

// マウス用(*03)
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

// ジョイスティック用 (*04)
bool GetJoyStickPress( int button );
bool GetJoyStickTrigger( int button );
bool GetJoyStickRelease( int button );
LONG GetJoyStickPositionX();
LONG GetJoyStickPositionY();
LONG GetJoyStickPositionZ();
LONG GetJoyStickPositionRX();
LONG GetJoyStickPositionRY();
LONG GetJoyStickPositionRZ();
bool GetJoyStickLeftTrigger(int Direction);	//	左スティックを傾けた瞬間を取得
LONG GetJoyStickSlider( int number );
DWORD GetJoyStickRgdwPOV( int number );
bool IsJoyStick(int number);
bool GetJoyStickAnyControl(void);	//	ジョイスティック操作中かの判定
#endif