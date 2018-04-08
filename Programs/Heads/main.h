
//-					-//--------------------------------------------------	
//	基礎構築ヘッダ	 //
//-					-//--------------------------------------------------

#ifndef	MAIN_H	//	多重定義防止
#define	MAIN_H

#define USE_SOUND	// サウンドの使用宣言

#include <windows.h>
#include <stdlib.h>		//	乱数
#include "d3dx9.h"		//	DirectX9 で使用するヘッダーをインクルード

#define DIRECTINPUT_VERSION	(0x0800)
#include "dinput.h"

#ifdef USE_SOUND
#include "xaudio2.h"	//  XAudio2関連のヘッダー
#endif

//	DirectX9 で使用するライブラリをインクルード
#pragma comment (lib,	"d3d9.lib")
#pragma comment (lib,	"d3dx9.lib")
#pragma comment (lib,	"dxguid.lib")
#pragma comment (lib,	"dinput8.lib")
#pragma comment (lib,	"winmm.lib")


//	マクロ定義
#define	SIZE_ScreenWidth	(640)					//	画面の横幅
#define SIZE_ScreenHeight	(480)					//	画面の縦幅
#define POS_ScreenCenter_X	(SIZE_ScreenWidth/2)	//	画面の中心位置(X軸)
#define POS_ScreenCenter_Y	(SIZE_ScreenHeight/2)	//	画面の中心位置(Y軸)
#define NUM_BackBuffer		(1)

#define	NUM_VertexPolygon2D	(4)	//	2D描画されるオブジェクトの頂点数(暫定)
#define NUM_Polygon2D		(2)	//	2D描画されるオブジェクトのポリゴン数(暫定)

//	-	メモリリークの安全な開放マクロ
#define SafeRelease(ptr)	{if(ptr){ptr->Release();ptr=NULL;}}

//	-	頂点フォーマット
#define FVF_VERTEX_2D	( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )				//	頂点座標/ 頂点カラー/ テクスチャ座標
#define FVF_VERTEX_3D	( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 )	//	頂点座標/ 法線ベクトル / 反射光 / テクスチャ座標

//	構造体定義
typedef struct {	//	2D頂点フォーマットに合わせた構造体
	D3DXVECTOR3	pos;	//	画面上の座標
	float		rhw;	//	同次座標の倍率(?)
	D3DCOLOR	col;	//	頂点の色
	D3DXVECTOR2	tex;	//	テクスチャ上の座標
}VERTEX_2D;

typedef struct {	//	3D頂点フォーマットに合わせた構造体
	D3DXVECTOR3 pos;	//	仮想空間上の座標
	D3DXVECTOR3 vec;	//	法線ベクトル
	D3DCOLOR	def;	//	反射光
	D3DXVECTOR2	tex;	//	テクスチャ上の座標
}VERTEX_3D;

//	プロトタイプ宣言
void QuitGame(void);
void SetMouseMode(bool);		//	マウスの移動制限
void SetScreenMode(void);		//	スクリーンモード切り替え
D3DXVECTOR2 Get_posMouse(void);	//	マウスの（画面上の）位置取得関数
DWORD		Get_timLast(void);	//	最終更新時刻を取得
void		IsolateMouse(void);		//	マウスの位置を離れた位置に設定

//-					-//--------------------------------------------------
//	ソース終了位置	 //
//-					-//--------------------------------------------------

#endif