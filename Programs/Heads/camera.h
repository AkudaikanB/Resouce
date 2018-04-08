//	カメラ設定
#ifndef	CAMERA_H	//	多重定義防止
#define CAMERA_H

//	ファイル結合
#include "main.h"

//	構造体定義
struct camera {			//	カメラ
	D3DXVECTOR3	posEye;		//	視点位置
	D3DXVECTOR3 posView;	//	注視点位置
	D3DXVECTOR3	vecUp;		//	視点上方向
};

//	マクロ,定数定義
const camera	INIT_Camera{	//	カメラの初期設定
		D3DXVECTOR3(0.0f,1.8f,0.0f),
		D3DXVECTOR3(0.0f,1.8f,1.0f),
		D3DXVECTOR3(0.0f,1.0f,0.0f)
	};
const float	INIT_AngleVision	= D3DXToRadian(80);
const float	INIT_AspectRatio	= (float)SIZE_ScreenWidth / SIZE_ScreenHeight;	//	初期アスペクト比
const float INIT_NearClip		= 1.0f;											//	初期ニアクリップ
const float INIT_FarClip		= 5000.0f;										//	初期ファークリップ

void InitCamera		(LPDIRECT3DDEVICE9 pDevice);	//	初期化
void UninitCamera	(void);							//	メモリ開放
void UpdateCamera	(LPDIRECT3DDEVICE9 pDevice);	//	更新

camera Get_Camera(void);	//	カメラの情報を取得する関数

#endif