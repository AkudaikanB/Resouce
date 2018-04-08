
//-					-//--------------------------------------------------	
//	基礎構築ソース	 //
//-					-//--------------------------------------------------

//	ファイル結合
#include "../Heads/main.h"
#include "../Heads/game.h"
#include "../Heads/input.h"
#include "../Heads/sound.h"

//	マクロ定義
#define	CLASS_NAME	"AppClass"
#define WINDOW_NAME	"TestProject"

//	プロトタイプ宣言
LRESULT	CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Initialize(HINSTANCE hInstance, HWND hWnd, bool bWindow);				//	初期化関数
void ClipMouse(HWND);			//	マウス位置制限
void Update_posCursor(HWND);	//	マウス位置を更新
void IsolateMouse(HWND);		//	マウスの位置を離す

//	グローバル変数宣言
LPDIRECT3D9			g_pD3DObject = NULL;
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;
bool				g_flgQuit = false;

bool				g_flgClipMouse = false;
bool				g_flgIsolate = false;
D3DXVECTOR2			g_posMouse;
DWORD				g_timLast;	//	最終更新時刻

//
//	関数定義
//
//	メイン関数
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

	//	ウィンドウを作成
	hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_POPUP,	//	枠無し(ポップアップウィンドウ)
		GetSystemMetrics(SM_CXSCREEN)/2-(SIZE_ScreenWidth/2),	//	ウィンドウを画面中央に配置（横位置）
		GetSystemMetrics(SM_CYSCREEN)/2-(SIZE_ScreenHeight/2),	//	ウィンドウを画面中央に配置（縦位置）
		SIZE_ScreenWidth,
		SIZE_ScreenHeight,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	//	初期化を行う（失敗時は強制終了）
	if (FAILED(Initialize(hInstance, hWnd, TRUE)))return -1;

	ShowWindow(hWnd,nCmdShow);	//	ウィンドウを表示
	UpdateWindow(hWnd);			//	ウィンドウを更新

	g_timLast = timeGetTime();

	//** * 	アプリケーションのループ	* **//
	while (1) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) {	//	特殊なメッセージが入力されると入る
			if (msg.message == WM_QUIT) {
				break;
			}else{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}else if(timeGetTime() -g_timLast >= (1000/60)){	//	基本的に（1秒間に60回）入る
			g_timLast = timeGetTime();

			//	マウス制限
			if (g_flgClipMouse && GetActiveWindow() == hWnd)ClipMouse(hWnd);
			if (g_flgIsolate && GetActiveWindow() == hWnd)IsolateMouse(hWnd);
			Update_posCursor(hWnd);

			UpdateGame	(g_pD3DDevice);	//	各オブジェクトの更新処理関数を呼び出し
			DrawGame	(g_pD3DDevice);	//	各オブジェクトの描画処理関数を呼び出し

			if (g_flgQuit)DestroyWindow(hWnd);
		}
	}

	//** * 	アプリケーションの終了処理	* **//
	UnregisterClass(CLASS_NAME,wcex.hInstance);	//	アプリケーションの片づけ
	UninitGame();								//	最初期化(メモリの開放)

	return (int)msg.wParam;
}

//	特定の入力で読み込ませるアプリケーション操作の関数
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

	g_pD3DObject = Direct3DCreate9(D3D_SDK_VERSION);	//	オブジェクトのインスタンス生成
	if (g_pD3DObject == NULL)return E_FAIL;				//	インスタンス生成失敗処理
	if (FAILED(g_pD3DObject->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&d3ddm)))
		return E_FAIL;									//	ディスプレイモードの取得、および失敗処理

	//	出力の基本パラメータを設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));					//	メモリを初期化
	d3dpp.BackBufferWidth	= SIZE_ScreenWidth;			//	仮想ウィンドウ上の大きさ（横幅）
	d3dpp.BackBufferHeight	= SIZE_ScreenHeight;		//	仮想ウィンドウ上の大きさ（縦幅）
	d3dpp.BackBufferFormat	= d3ddm.Format;				//	仮想ウィンドウの型をディスプレイモードに合わせる
	d3dpp.BackBufferCount	= NUM_BackBuffer;			//	仮想ウィンドウの数
	d3dpp.SwapEffect		= D3DSWAPEFFECT_DISCARD;	//	ウィンドウ更新時のエフェクト
	d3dpp.Windowed			= bWindow;					//	ウィンドウモードかどうかの(ウィンドウモード)
	d3dpp.EnableAutoDepthStencil = TRUE;				//	深度を設けるかどうかの設定
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;		//	深度の型を設定

	//	画面出力の速度関連設定
	if (bWindow == TRUE) {	//	ウィンドウモード
		d3dpp.FullScreen_RefreshRateInHz	= 0;
		d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_IMMEDIATE;
	}if(bWindow == FALSE){	//	フルスクリーンモード
		d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_DEFAULT;
	}

	//	描画デバイスの設定
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

	InitInput(hInstance,hWnd);	//	入力系の初期化
	InitSound(hWnd);
	InitGame(g_pD3DDevice);	//** *	ゲームの起動時設定を行う	* **//

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

	GetCursorPos(&cursor);			//	マウスの位置を取得
	GetWindowRect(hWnd, &window);	//	ウィンドウの位置を取得


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