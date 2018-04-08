
//-					-//--------------------------------------------------	
//	��b�\�z�\�[�X	 //
//-					-//--------------------------------------------------

//	�t�@�C������
#include "../Heads/main.h"
#include "../Heads/game.h"
#include "../Heads/input.h"
#include "../Heads/sound.h"

//	�}�N����`
#define	CLASS_NAME	"AppClass"
#define WINDOW_NAME	"TestProject"

//	�v���g�^�C�v�錾
LRESULT	CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Initialize(HINSTANCE hInstance, HWND hWnd, bool bWindow);				//	�������֐�
void ClipMouse(HWND);			//	�}�E�X�ʒu����
void Update_posCursor(HWND);	//	�}�E�X�ʒu���X�V
void IsolateMouse(HWND);		//	�}�E�X�̈ʒu�𗣂�

//	�O���[�o���ϐ��錾
LPDIRECT3D9			g_pD3DObject = NULL;
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;
bool				g_flgQuit = false;

bool				g_flgClipMouse = false;
bool				g_flgIsolate = false;
D3DXVECTOR2			g_posMouse;
DWORD				g_timLast;	//	�ŏI�X�V����

//
//	�֐���`
//
//	���C���֐�
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmd, int nCmdShow) {

	WNDCLASSEX wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW+1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND	hWnd;
	MSG		msg;

	RegisterClassEx(&wcex);

	//	�E�B���h�E���쐬
	hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_POPUP,	//	�g����(�|�b�v�A�b�v�E�B���h�E)
		GetSystemMetrics(SM_CXSCREEN)/2-(SIZE_ScreenWidth/2),	//	�E�B���h�E����ʒ����ɔz�u�i���ʒu�j
		GetSystemMetrics(SM_CYSCREEN)/2-(SIZE_ScreenHeight/2),	//	�E�B���h�E����ʒ����ɔz�u�i�c�ʒu�j
		SIZE_ScreenWidth,
		SIZE_ScreenHeight,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	//	���������s���i���s���͋����I���j
	if (FAILED(Initialize(hInstance, hWnd, TRUE)))return -1;

	ShowWindow(hWnd,nCmdShow);	//	�E�B���h�E��\��
	UpdateWindow(hWnd);			//	�E�B���h�E���X�V

	g_timLast = timeGetTime();

	//** * 	�A�v���P�[�V�����̃��[�v	* **//
	while (1) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) {	//	����ȃ��b�Z�[�W�����͂����Ɠ���
			if (msg.message == WM_QUIT) {
				break;
			}else{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}else if(timeGetTime() -g_timLast >= (1000/60)){	//	��{�I�Ɂi1�b�Ԃ�60��j����
			g_timLast = timeGetTime();

			//	�}�E�X����
			if (g_flgClipMouse && GetActiveWindow() == hWnd)ClipMouse(hWnd);
			if (g_flgIsolate && GetActiveWindow() == hWnd)IsolateMouse(hWnd);
			Update_posCursor(hWnd);

			UpdateGame	(g_pD3DDevice);	//	�e�I�u�W�F�N�g�̍X�V�����֐����Ăяo��
			DrawGame	(g_pD3DDevice);	//	�e�I�u�W�F�N�g�̕`�揈���֐����Ăяo��

			if (g_flgQuit)DestroyWindow(hWnd);
		}
	}

	//** * 	�A�v���P�[�V�����̏I������	* **//
	UnregisterClass(CLASS_NAME,wcex.hInstance);	//	�A�v���P�[�V�����̕ЂÂ�
	UninitGame();								//	�ŏ�����(�������̊J��)

	return (int)msg.wParam;
}

//	����̓��͂œǂݍ��܂���A�v���P�[�V��������̊֐�
LRESULT CALLBACK WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam) {
	
	switch (nMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_ESCAPE:
			break;
		}
		break;

	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
		
	default:
		break;
	}

	return DefWindowProc(hWnd, nMsg, wParam, lParam);
}

HRESULT Initialize(HINSTANCE hInstance, HWND hWnd, bool bWindow) {
	D3DPRESENT_PARAMETERS	d3dpp;
	D3DDISPLAYMODE			d3ddm;

	g_pD3DObject = Direct3DCreate9(D3D_SDK_VERSION);	//	�I�u�W�F�N�g�̃C���X�^���X����
	if (g_pD3DObject == NULL)return E_FAIL;				//	�C���X�^���X�������s����
	if (FAILED(g_pD3DObject->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&d3ddm)))
		return E_FAIL;									//	�f�B�X�v���C���[�h�̎擾�A����ю��s����

	//	�o�͂̊�{�p�����[�^��ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));					//	��������������
	d3dpp.BackBufferWidth	= SIZE_ScreenWidth;			//	���z�E�B���h�E��̑傫���i�����j
	d3dpp.BackBufferHeight	= SIZE_ScreenHeight;		//	���z�E�B���h�E��̑傫���i�c���j
	d3dpp.BackBufferFormat	= d3ddm.Format;				//	���z�E�B���h�E�̌^���f�B�X�v���C���[�h�ɍ��킹��
	d3dpp.BackBufferCount	= NUM_BackBuffer;			//	���z�E�B���h�E�̐�
	d3dpp.SwapEffect		= D3DSWAPEFFECT_DISCARD;	//	�E�B���h�E�X�V���̃G�t�F�N�g
	d3dpp.Windowed			= bWindow;					//	�E�B���h�E���[�h���ǂ�����(�E�B���h�E���[�h)
	d3dpp.EnableAutoDepthStencil = TRUE;				//	�[�x��݂��邩�ǂ����̐ݒ�
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;		//	�[�x�̌^��ݒ�

	//	��ʏo�͂̑��x�֘A�ݒ�
	if (bWindow == TRUE) {	//	�E�B���h�E���[�h
		d3dpp.FullScreen_RefreshRateInHz	= 0;
		d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_IMMEDIATE;
	}if(bWindow == FALSE){	//	�t���X�N���[�����[�h
		d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_DEFAULT;
	}

	//	�`��f�o�C�X�̐ݒ�
	if (FAILED(g_pD3DObject->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice
	))) {
		if (FAILED(g_pD3DObject->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice
		))) {
			if (FAILED(g_pD3DObject->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice
			))) {
				return	E_FAIL;
			}
		}
	}

	InitInput(hInstance,hWnd);	//	���͌n�̏�����
	InitSound(hWnd);
	InitGame(g_pD3DDevice);	//** *	�Q�[���̋N�����ݒ���s��	* **//

	return S_OK;
}

void SetMouseMode(bool bSet) {
	g_flgClipMouse = bSet;
}
void ClipMouse(HWND hWnd){
	RECT w;
	GetWindowRect(hWnd,&w);

	SetCursorPos(w.left+(SIZE_ScreenWidth/2),w.top+(SIZE_ScreenHeight/2));
}

void Update_posCursor(HWND hWnd) {
	POINT cursor;
	RECT window;

	GetCursorPos(&cursor);			//	�}�E�X�̈ʒu���擾
	GetWindowRect(hWnd, &window);	//	�E�B���h�E�̈ʒu���擾


	g_posMouse.x = cursor.x - (float)window.left;
	g_posMouse.y = cursor.y - (float)window.top;
}

DWORD Get_timLast(void) {
	return g_timLast;
}
D3DXVECTOR2 Get_posMouse(void) {
	return g_posMouse;
}
void IsolateMouse(void) {
	g_flgIsolate = true;
}
void IsolateMouse(HWND hWnd) {
	RECT w;
	GetWindowRect(hWnd, &w);

	SetCursorPos(w.right, w.bottom);

	g_flgIsolate = false;
}

void QuitGame(void) {
	g_flgQuit = true;
}