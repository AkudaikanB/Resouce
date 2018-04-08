//=============================================================================
//
// ���͏��� [input.cpp]
//
//=============================================================================
#include "../Heads/input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	NUM_KEY_MAX			(256)				// �L�[���̍ő�l

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd);
void UninitKeyboard(void);
HRESULT UpdateKeyboard(void);

// �}�E�X�p
HRESULT InitMouse(HINSTANCE hInst, HWND hWnd);
void UninitMouse(void);
HRESULT UpdateMouse(void);

// �W���C�p�b�h�֘A
HRESULT InitJoypad(HWND hWnd);
void UninitJoypad(void);
void UpdateJoypad(void);
BOOL CALLBACK EnumJoyCallback(const DIDEVICEINSTANCE* lpddi, VOID* pvRef);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECTINPUT8			g_pDInput = NULL;					// IDirectInput8�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECTINPUTDEVICE8	g_pDIDevKeyboard = NULL;			// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^(�L�[�{�[�h)
LPDIRECTINPUTDEVICE8	g_pDIDevMouse = NULL;				// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^(�}�E�X)
BYTE					g_keyState[NUM_KEY_MAX];			// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					g_keyStateTrigger[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					g_keyStateRepeat[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					g_keyStateRelease[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
int						g_keyStateRepeatCnt[NUM_KEY_MAX];	// �L�[�{�[�h�̃��s�[�g�J�E���^
DIMOUSESTATE2			g_mouseState;						// �}�E�X�̏�Ԃ��󂯎�郏�[�N
DIMOUSESTATE2			g_mouseStateTrigger;				// �}�E�X�̏�Ԃ��󂯎�郏�[�N
DIMOUSESTATE2			g_mouseStateRelease;				// �}�E�X�̏�Ԃ��󂯎�郏�[�N

DIJOYSTATE				g_joyState[MAX_CONTROLER];			// �W���C�p�b�h���z��
DIJOYSTATE				g_joyStatePrev[MAX_CONTROLER];		// �W���C�p�b�h���z��i���O�j
LPDIRECTINPUTDEVICE8	g_pDIDevJoypad[MAX_CONTROLER];		// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^(�W���C�p�b�h)
int						g_nJoypadNum = 0;					// ���������W���C�p�b�h�̐�
BYTE					g_joyStateTrigger[MAX_CONTROLER][32];
BYTE					g_joyStateRelease[MAX_CONTROLER][32];

//=============================================================================
// ���͏����̏�����
//=============================================================================
HRESULT InitInput(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	if(!g_pDInput)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
									IID_IDirectInput8, (void**)&g_pDInput, NULL);
	}

	// �L�[�{�[�h�̏�����
	InitKeyboard(hInst, hWnd);

	// �}�E�X�̏�����
	InitMouse(hInst, hWnd);

	// �Q�[���p�b�h�̏�����
	if(FAILED(InitJoypad(hWnd)))
		return E_FAIL;

	return hr;
}

//=============================================================================
// ���͏����̏I������
//=============================================================================
void UninitInput(void)
{
	// �L�[�{�[�h�̏I������
	UninitKeyboard();

	// �}�E�X�̏I������
	UninitMouse();

	// �Q�[���p�b�h�̏I������
	UninitJoypad();

	if(g_pDInput)
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
	// �L�[�{�[�h�̍X�V
	UpdateKeyboard();

	// �}�E�X�̍X�V
	UpdateMouse();

	// �Q�[���p�b�h�̍X�V
	UpdateJoypad();
}

//=============================================================================
// �L�[�{�[�h�̏�����
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pDIDevKeyboard, NULL);
	if(FAILED(hr) || g_pDIDevKeyboard == NULL)
	{
		MessageBox(hWnd, "�L�[�{�[�h���˂��I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = g_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = g_pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
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
	if(g_pDIDevKeyboard)
	{
		g_pDIDevKeyboard->Unacquire();

		g_pDIDevKeyboard->Release();
		g_pDIDevKeyboard = NULL;
	}
}

//=============================================================================
// �L�[�{�[�h�̍X�V����
//=============================================================================
HRESULT UpdateKeyboard(void)
{
	HRESULT hr;
	BYTE keyStateOld[256];

	// �O��̃f�[�^��ۑ�
	memcpy(keyStateOld, g_keyState, NUM_KEY_MAX);

	// �f�o�C�X����f�[�^���擾
	hr = g_pDIDevKeyboard->GetDeviceState(sizeof(g_keyState), g_keyState);
	if(SUCCEEDED(hr))
	{
		for(int cnt = 0; cnt < NUM_KEY_MAX; cnt++)
		{
			g_keyStateTrigger[cnt] = (keyStateOld[cnt] ^ g_keyState[cnt]) & g_keyState[cnt];
			g_keyStateRelease[cnt] = (keyStateOld[cnt] ^ g_keyState[cnt]) & ~g_keyState[cnt];
			g_keyStateRepeat[cnt] = g_keyStateTrigger[cnt];

			if(g_keyState[cnt])
			{
				g_keyStateRepeatCnt[cnt]++;
				if(g_keyStateRepeatCnt[cnt] >= 20)
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
		hr = g_pDIDevKeyboard->Acquire();
	}

	return hr;
}

//==============================
// �W���C�p�b�h�̏�����
//==============================
HRESULT InitJoypad(HWND hWnd)
{
	int nLoop;

	// ������
	for(nLoop = 0; nLoop < MAX_CONTROLER; nLoop++)
		g_pDIDevJoypad[nLoop] = NULL;

	// �f�o�C�X�I�u�W�F�N�g���쐬(�ڑ�����Ă���W���C�p�b�h��񋓂���)
	if(FAILED(g_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyCallback, NULL, DIEDFL_ATTACHEDONLY)))
		return E_FAIL;

	// �W���C�p�b�h�̐���������
	for(nLoop = 0; nLoop < MAX_CONTROLER; nLoop++)
	{
		// �W���C�p�b�h���Ȃ��ꍇ�͂�����΂�
		if(g_pDIDevJoypad[nLoop] == NULL)
			continue;

		// �f�[�^�t�H�[�}�b�g�̐ݒ�
		if(FAILED(g_pDIDevJoypad[nLoop]->SetDataFormat(&c_dfDIJoystick)))
			return E_FAIL;

		// �������x���̐ݒ�
		if(FAILED(g_pDIDevJoypad[nLoop]->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
			return E_FAIL;

		// �f�o�C�X�ւ̓��͐���J�n
		g_pDIDevJoypad[nLoop]->Acquire();	
	}

	return S_OK;
}

//==============================
// �W���C�p�b�h�₢���킹�p�R�[���o�b�N�֐�
//==============================
BOOL CALLBACK EnumJoyCallback(const DIDEVICEINSTANCE* lpddi, VOID* pvRef)
{
	DIDEVCAPS	diDevCaps;			// �f�o�C�X���

	// �W���C�p�b�h�p�f�o�C�X�I�u�W�F�N�g���쐬
	if(FAILED(g_pDInput->CreateDevice(lpddi->guidInstance, &g_pDIDevJoypad[g_nJoypadNum], NULL)))
		return DIENUM_CONTINUE;		// �񋓂𑱂���

	// �W���C�p�b�h�̔\�͂𒲂ׂ�
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	if(FAILED(g_pDIDevJoypad[g_nJoypadNum]->GetCapabilities(&diDevCaps)))
	{
		if(g_pDIDevJoypad[g_nJoypadNum])
			g_pDIDevJoypad[g_nJoypadNum]->Release();
		g_pDIDevJoypad[g_nJoypadNum] = NULL;
		return DIENUM_CONTINUE;		// �񋓂𑱂���
	}

	// �K�萔�ɒB������I��
	g_nJoypadNum++;
	if(g_nJoypadNum == MAX_CONTROLER)
		return DIENUM_STOP;		// �񋓂��I������
	else
		return DIENUM_CONTINUE;		// �񋓂𑱂���
}

//==============================
// �W���C�p�b�h�̏I������
//==============================
void UninitJoypad(void)
{
	for(int nLoop = 0; nLoop < MAX_CONTROLER; nLoop++)
		if (g_pDIDevJoypad[nLoop]) {
			g_pDIDevJoypad[nLoop]->Release();
			g_pDIDevJoypad[nLoop] = NULL;
		}
}

//==============================
// �W���C�p�b�h�̍X�V����
//==============================
void UpdateJoypad(void)
{
	int nLoop;
	
	for(nLoop = 0; nLoop < MAX_CONTROLER; nLoop++)
	{
		// ���O�W���C�p�b�h���̃o�b�N�A�b�v
		g_joyStatePrev[nLoop] = g_joyState[nLoop];

		if(g_pDIDevJoypad[nLoop])
		{
			// �f�o�C�X����f�[�^���擾
			if(FAILED(g_pDIDevJoypad[nLoop]->GetDeviceState(sizeof(DIJOYSTATE), &g_joyState[nLoop])))
				g_pDIDevJoypad[nLoop]->Acquire();
		}

		// �g���K�[/�����[�X���v�Z
		for( int i = 0; i < 32; i++ )
		{
			if( g_joyState[nLoop].rgbButtons[i] & 0x80 && 
				!(g_joyStatePrev[nLoop].rgbButtons[i] & 0x80) )
			{
				g_joyStateTrigger[nLoop][i] = 0x80;
			} else {
				g_joyStateTrigger[nLoop][i] = 0x00;
			}

			if (!(g_joyState[nLoop].rgbButtons[i] & 0x80) &&
				(g_joyStatePrev[nLoop].rgbButtons[i] & 0x80))
			{
				g_joyStateRelease[nLoop][i] = 0x80;
			}
			else {
				g_joyStateRelease[nLoop][i] = 0x00;
			}

		}
	}
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
// �}�E�X�̏�����
//=============================================================================
HRESULT InitMouse(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = g_pDInput->CreateDevice(GUID_SysMouse, &g_pDIDevMouse, NULL);
	if(FAILED(hr) || g_pDIDevMouse == NULL)
	{
		MessageBox(hWnd, "�}�E�X���˂��I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = g_pDIDevMouse->SetDataFormat(&c_dfDIMouse2);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�}�E�X�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = g_pDIDevMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�}�E�X�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�o�C�X�̐ݒ�
	{
		DIPROPDWORD dipdw;

		dipdw.diph.dwSize = sizeof(dipdw);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = DIPROPAXISMODE_REL;

		hr = g_pDIDevMouse->SetProperty(DIPROP_AXISMODE, &dipdw.diph);
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	g_pDIDevMouse->Acquire();

	return S_OK;
}

//=============================================================================
// �}�E�X�̏I������
//=============================================================================
void UninitMouse(void)
{
	if(g_pDIDevMouse)
	{
		g_pDIDevMouse->Unacquire();

		g_pDIDevMouse->Release();
		g_pDIDevMouse = NULL;
	}
}

//=============================================================================
// �}�E�X�̍X�V����
//=============================================================================
HRESULT UpdateMouse(void)
{
	HRESULT hr;
	DIMOUSESTATE2 mouseStateOld;

	// �O��̃f�[�^��ۑ�
	mouseStateOld = g_mouseState;

	// �f�o�C�X����f�[�^���擾
	hr = g_pDIDevMouse->GetDeviceState(sizeof(g_mouseState), &g_mouseState);
	if(SUCCEEDED(hr))
	{
		g_mouseStateTrigger.lX = ((mouseStateOld.lX ^ g_mouseState.lX) & g_mouseState.lX);
		g_mouseStateTrigger.lY = ((mouseStateOld.lY ^ g_mouseState.lY) & g_mouseState.lY);
		g_mouseStateTrigger.lZ = ((mouseStateOld.lZ ^ g_mouseState.lZ) & g_mouseState.lZ);
		for(int cnt = 0; cnt < 8; cnt++)
		{
			g_mouseStateTrigger.rgbButtons[cnt] = ((mouseStateOld.rgbButtons[cnt] ^ g_mouseState.rgbButtons[cnt]) & g_mouseState.rgbButtons[cnt]);
			g_mouseStateRelease.rgbButtons[cnt] = ((mouseStateOld.rgbButtons[cnt] ^ g_mouseState.rgbButtons[cnt]) & ~g_mouseState.rgbButtons[cnt]);
		}
	}
	else
	{
		g_pDIDevMouse->Acquire();
	}

	return S_OK;
}

//=============================================================================
// �}�E�X�f�[�^�擾(���v���X)
//=============================================================================
bool GetMouseLeftPress(void)
{
	return (g_mouseState.rgbButtons[0] & 0x80) ? true : false;
}

//=============================================================================
// �}�E�X�f�[�^�擾(���g���K�[)
//=============================================================================
bool GetMouseLeftTrigger(void)
{
	return (g_mouseStateTrigger.rgbButtons[0] & 0x80) ? true : false;
}
//=============================================================================
// �}�E�X�f�[�^�擾(�������)
//=============================================================================
bool GetMouseLeftRelease(void)
{
	return (g_mouseStateRelease.rgbButtons[0] & 0x80) ? true : false;
}

//=============================================================================
// �}�E�X�f�[�^�擾(�E�v���X)
//=============================================================================
bool GetMouseRightPress(void)
{
	return (g_mouseState.rgbButtons[1] & 0x80) ? true : false;
}

//=============================================================================
// �}�E�X�f�[�^�擾(�E�g���K�[)
//=============================================================================
bool GetMouseRightTrigger(void)
{
	return (g_mouseStateTrigger.rgbButtons[1] & 0x80) ? true : false;
}
//=============================================================================
// �}�E�X�f�[�^�擾(�E�����)
//=============================================================================
bool GetMouseRightRelease(void)
{
	return (g_mouseStateRelease.rgbButtons[1] & 0x80) ? true : false;
}

//=============================================================================
// �}�E�X�f�[�^�擾(�����v���X)
//=============================================================================
bool GetMouseCenterPress(void)
{
	return (g_mouseState.rgbButtons[2] & 0x80) ? true : false;
}

//=============================================================================
// �}�E�X�f�[�^�擾(�����g���K�[)
//=============================================================================
bool GetMouseCenterTrigger(void)
{
	return (g_mouseState.rgbButtons[2] & 0x80) ? true : false;
}
//=============================================================================
// �}�E�X�f�[�^�擾(���������)
//=============================================================================
bool GetMouseCenterRelease(void)
{
	return (g_mouseStateRelease.rgbButtons[2] & 0x80) ? true : false;
}

//=============================================================================
// �}�E�X�f�[�^�擾(�w���ړ�)
//=============================================================================
long GetMouseAxisX(void)
{
	return g_mouseState.lX;
}

//=============================================================================
// �}�E�X�f�[�^�擾(�x���ړ�)
//=============================================================================
long GetMouseAxisY(void)
{
	return g_mouseState.lY;
}

//=============================================================================
// �}�E�X�f�[�^�擾(�y���ړ�)
//=============================================================================
long GetMouseAxisZ(void)
{
	return g_mouseState.lZ;
}

//=============================================================================
// �W���C�X�e�B�b�N�f�[�^�擾
//=============================================================================
bool GetJoyStickPress( int button )
{
	// 0�Ԃ̃W���C�X�e�B�b�N�������ĂȂ�
	return (g_joyState[0].rgbButtons[button] & 0x80) ? true : false;
}
// �g���K�[
bool GetJoyStickTrigger( int button )
{
	// 0�Ԃ̃W���C�X�e�B�b�N�������ĂȂ�
	return (g_joyStateTrigger[0][button] & 0x80) ? true : false;
}
// �����[�X
bool GetJoyStickRelease(int button)
{
	// 0�Ԃ̃W���C�X�e�B�b�N�������ĂȂ�
	return (g_joyStateRelease[0][button] & 0x80) ? true : false;
}
//=============================================================================
// �W���C�X�e�B�b�N�f�[�^�擾�i�w�����j
//=============================================================================
LONG GetJoyStickPositionX()
{
	return g_joyState[0].lX;
}

//=============================================================================
// �W���C�X�e�B�b�N�f�[�^�擾�i�x�����j
//=============================================================================
LONG GetJoyStickPositionY()
{
	return g_joyState[0].lY;
}

//=============================================================================
// �W���C�X�e�B�b�N�f�[�^�擾�i�y�����j
//=============================================================================
LONG GetJoyStickPositionZ()
{
	return g_joyState[0].lZ;
}

//=============================================================================
// �W���C�X�e�B�b�N�f�[�^�擾�i�q�w�����j
//=============================================================================
LONG GetJoyStickPositionRX()
{
	return g_joyState[0].lRx;
}

//=============================================================================
// �W���C�X�e�B�b�N�f�[�^�擾�i�q�x�����j
//=============================================================================
LONG GetJoyStickPositionRY()
{
	return g_joyState[0].lRy;
}

//=============================================================================
// �W���C�X�e�B�b�N�f�[�^�擾�i�q�y�����j
//=============================================================================
LONG GetJoyStickPositionRZ()
{
	return g_joyState[0].lRz;
}

//=============================================================================
// �W���C�X�e�B�b�N�f�[�^�擾�i�X���C�_�[�j
//=============================================================================
LONG GetJoyStickSlider( int number )
{
	return g_joyState[0].rglSlider[number];
}

//=============================================================================
// �W���C�X�e�B�b�N�f�[�^�擾�iPOV�j
//=============================================================================
DWORD GetJoyStickRgdwPOV( int number )
{
	return g_joyState[0].rgdwPOV[number];
}
// �W���C�p�b�h�̗L���擾
bool IsJoyStick(int number)
{
	return g_pDIDevJoypad[number] ? true : false;
}

//	���X�e�B�b�N�̌X�����u�Ԃ��擾(0:��, 1:��, 2:�E, 3:��)
bool GetJoyStickLeftTrigger(int Direction) {
	switch (Direction) {
	case 0:
		if (g_joyStatePrev[0].lY > 25000 && g_joyState[0].lY <=25000)
			return true;
		break;
	case 1:
		if (g_joyStatePrev[0].lX > 25000 && g_joyState[0].lX <= 25000)
			return true;
		break;
	case 2:
		if (g_joyStatePrev[0].lX < 40000 && g_joyState[0].lX >= 40000)
			return true;
		break;
	case 3:
		if (g_joyStatePrev[0].lY < 40000 && g_joyState[0].lY >= 40000)
			return true;
		break;
	}
	return false;
}

// �W���C�X�e�B�b�N���쒆���ǂ����̔��f
bool GetJoyStickAnyControl(void) {
	if ((g_joyState[0].lX < 25000 || g_joyState[0].lX > 40000) ||
		(g_joyState[0].lY < 25000 || g_joyState[0].lY > 40000) ||
		(g_joyState[0].lZ < 25000 || g_joyState[0].lZ > 40000) ||
		(g_joyState[0].lRz < 25000 || g_joyState[0].lRz > 40000) || 
		(	g_joyState[0].rgbButtons[0] || g_joyState[0].rgbButtons[1] || 
			g_joyState[0].rgbButtons[2] || g_joyState[0].rgbButtons[3] ||
			g_joyState[0].rgbButtons[4] || g_joyState[0].rgbButtons[5] ||
			g_joyState[0].rgbButtons[6] || g_joyState[0].rgbButtons[7] ||
			g_joyState[0].rgbButtons[8] || g_joyState[0].rgbButtons[9] ||
			g_joyState[0].rgbButtons[10] || g_joyState[0].rgbButtons[11]))
		return true;

	return false;
}
