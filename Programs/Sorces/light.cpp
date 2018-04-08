//	環境光

//	ファイル結合
#include "../Heads/light.h"
#include "../Heads/player.h"
#include "../Heads/input.h"

//	グローバル変数宣言
D3DLIGHT9	Light_Directional00 = { D3DLIGHT_DIRECTIONAL };	//	全体的な光源
D3DLIGHT9	Light_Directional01 = { D3DLIGHT_DIRECTIONAL };	//	光源B
bool		flgLight01 = false;

D3DLIGHT9	Light_Player = { D3DLIGHT_POINT };	//	自己発光


//	関数定義

void InitLight(LPDIRECT3DDEVICE9 pDevice) {
	pDevice->SetRenderState(D3DRS_LIGHTING,TRUE);		//	光の設定を有効にする
	pDevice->SetRenderState(D3DRS_SPECULARENABLE,TRUE);	//	鏡面反射を有効にする

	//	全体的な光源（太陽的な）の設定
	Light_Directional00.Diffuse		= D3DXCOLOR(0.8f,1.0f,1.0f,1.0f);	//	直射色を最大に設定
	Light_Directional00.Specular	= D3DXCOLOR(0.8f,1.0f,1.0f,1.0f);	//	鏡面反射率を無しに設定
	Light_Directional00.Ambient		= D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);	//	環境光を最低に設定
	Light_Directional01.Diffuse		= D3DXCOLOR(1.0f,0.8f,1.0f,1.0f);	//	直射色を最大に設定
	Light_Directional01.Specular	= D3DXCOLOR(1.0f,0.8f,1.0f,1.0f);	//	鏡面反射率を無しに設定
	Light_Directional01.Ambient		= D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);	//	環境光を最低に設定

	D3DXVECTOR3 vecLight01 = INIT_vecLightDirectional*-1.0f;
	
	//	自己発光の光
	Light_Player.Diffuse	=	D3DXCOLOR(1.0f,1.0f,0.0f,1.0f);
	Light_Player.Specular	=	D3DXCOLOR(1.0f,1.0f,0.0f,1.0f);
	Light_Player.Ambient	=	D3DXCOLOR(1.0f,1.0f,0.0f,1.0f);
	Light_Player.Position	=	INIT_Vector3D;
	Light_Player.Range		=	300.0f;

	//	光の角度を単位化
	D3DXVec3Normalize((D3DXVECTOR3*)&Light_Directional00.Direction,&INIT_vecLightDirectional);
	D3DXVec3Normalize((D3DXVECTOR3*)&Light_Directional01.Direction, &vecLight01);

	pDevice->SetLight(0,&Light_Directional00);	//	光の設定値を0番に設定
	pDevice->SetLight(1,&Light_Directional01);	//	光の設定値を1番に設定
	pDevice->LightEnable(0, TRUE);				//	0番の光を有効化

	pDevice->SetLight(2,&Light_Player);		//	プレイヤー発光の設定値を0番に設定
	pDevice->LightEnable(2, TRUE);			//	0番の光を有効化
}

void UpdateLight(LPDIRECT3DDEVICE9 pDevice) {
	//	プレイヤーの自己発光をプレイヤーに追従させる
	Light_Player.Position = Get_posEye();

	pDevice->SetLight(2, &Light_Player);		//	プレイヤー発光の設定値を更新
}

//	ライト切り替え（light = 0,1）
void ChangeLight(LPDIRECT3DDEVICE9 pDevice) {
	flgLight01 = !flgLight01;

	if (flgLight01) {
		pDevice->LightEnable(1, TRUE);
	}
	else {
		pDevice->LightEnable(1, FALSE);
	}
}