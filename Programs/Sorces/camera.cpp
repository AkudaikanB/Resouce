//	カメラ設定

//	ファイル結合
#include "../Heads/camera.h"
#include "../Heads/player.h"

//	グローバル変数宣言
camera cameraPlayer;	//	プレイヤーの目
D3DXVECTOR3 g_rotCamera;

//	関数定義
void InitCamera(LPDIRECT3DDEVICE9 pDevice){

	//	変数宣言
	D3DXMATRIXA16	matProj,	//	射影行列
					matView;	//	視点座標行列

	//	変数初期化
	cameraPlayer = INIT_Camera;	//	カメラの初期行列を作成


	//	射影変換
	D3DXMatrixPerspectiveFovLH(	//	射影行列を作成
		&matProj,			//	代入先
		INIT_AngleVision,	//	視野角
		INIT_AspectRatio,	//	アスペクト比
		INIT_NearClip,		//	ニアクリップ	(映させない距離)
		INIT_FarClip		//	ファークリップ	(映させなくなる距離)
	);
	pDevice->SetTransform(D3DTS_PROJECTION, &matProj);	//	射影変換

	//	ビュー変換
	D3DXMatrixLookAtLH(		//	ビュー行列を作成
		&matView,				//	代入先	
		&cameraPlayer.posEye,	//	視点位置
		&cameraPlayer.posView,	//	注視点位置
		&cameraPlayer.vecUp		//	視点上方向
	);
	pDevice->SetTransform(D3DTS_VIEW, &matView);

	//	視点の回転度を初期化
	ZeroMemory(&g_rotCamera,sizeof(D3DXVECTOR3));
}

void UpdateCamera(LPDIRECT3DDEVICE9 pDevice) {

	cameraPlayer.posEye =	Get_posEye();		//	視点の位置
	cameraPlayer.posView =	Get_vecPlayer();	//	注視点の位置

	D3DXMATRIXA16 matView;
	D3DXMatrixIdentity(&matView);

	D3DXMatrixLookAtLH(		//	ビュー行列を作成
		&matView,				//	代入先	
		&cameraPlayer.posEye,	//	視点位置
		&cameraPlayer.posView,	//	注視点位置
		&cameraPlayer.vecUp		//	視点上方向
	);
	pDevice->SetTransform(D3DTS_VIEW, &matView);
}

//	カメラの情報を取得
camera Get_Camera(void) {
	return cameraPlayer;
}