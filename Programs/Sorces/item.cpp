//	アイテムオブジェクト(:Item,以下=アイテム)の制御

//	ファイル結合
#include "../Heads/item.h"
#include "../Heads/camera.h"	//	ビルボードの描画に使用
#include "../Heads/goal.h"

//	グローバル変数宣言
Object	g_objItems[NUM_MaxItem];	//	アイテムの情報変数
bool	g_flgItems[NUM_MaxItem];	//	アイテムの現存情報
LPDIRECT3DVERTEXBUFFER9	g_pVB_Items = NULL;		//	頂点(情報)バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTex_Items = NULL;	//	テクスチャへのポインタ

//	プロトタイプ宣言
void MakeItemsBuffer(LPDIRECT3DDEVICE9 pDevice);

//
//	関数定義
//
//	初期化関数
void InitItems(LPDIRECT3DDEVICE9 pDevice) {

	ZeroMemory(g_objItems,sizeof(g_objItems));	//	管理情報をすべて0で初期化（不定値読み取り防止）
	ZeroMemory(g_flgItems,sizeof(g_flgItems));	//	管理情報をすべて0で初期化（不定値読み取り防止）
	
	MakeItemsBuffer(pDevice);	//	頂点バッファの生成

	D3DXCreateTextureFromFile(	//	テクスチャの生成
		pDevice,
		ADR_TextureFruit00,
		&g_pTex_Items
	);
}
void UninitItems(void) {
	if (g_pVB_Items) {	//	頂点バッファを解放
		g_pVB_Items->Release();
		g_pVB_Items = NULL;
	}
	if (g_pTex_Items) {	//	テクスチャの解放
		g_pTex_Items->Release();
		g_pTex_Items = NULL;
	}
}

void UpdateItems(void) {
}
void DrawItems(LPDIRECT3DDEVICE9 pDevice) {
	//	変数宣言
	D3DXMATRIX matInvCamera;		//	カメラの逆行列
	D3DXMATRIX matBillboard;		//	ビルボード描画に使用する行列
	D3DXMATRIX matTranslate;		//	ビルボードの描画に使用する行列
	camera Camera = Get_Camera();	//	カメラの情報を取得

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);				//	ライティング無効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		//	αテストを有効化
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	D3DXMatrixIdentity(&matBillboard);	//	初期化

	//	逆行列を設定
	pDevice->GetTransform(D3DTS_VIEW,&matInvCamera);
	D3DXMatrixInverse(&matInvCamera,NULL,&matInvCamera);
	matInvCamera._41=
	matInvCamera._42=
	matInvCamera._43=0.0f;

	//	テクスチャを設定
	pDevice->SetTexture(0,g_pTex_Items);

	//	バッファの参照範囲を設定
	pDevice->SetStreamSource(0,g_pVB_Items,0,sizeof(VERTEX_3D));

	//	描画
	for (int Manager = 0; Manager < NUM_MaxItem; Manager++) {
		if (g_flgItems[Manager] == false)continue;	//	現存していないアイテムを描画しない

		D3DXMatrixIdentity(&matBillboard);	//	初期化

		D3DXMatrixTranslation(&matTranslate,g_objItems[Manager].pos.x, g_objItems[Manager].pos.y, g_objItems[Manager].pos.z);
		D3DXMatrixMultiply(&matBillboard, &matBillboard, &matInvCamera);
		D3DXMatrixMultiply(&matBillboard, &matBillboard, &matTranslate);
		pDevice->SetTransform(D3DTS_WORLD,&matBillboard);

		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,
			Manager*NUM_VertexPolygon2D,
			NUM_Polygon2D
		);
	}

	//	ライティングを有効
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		//	αテストを無効化
}

void MakeItemsBuffer(LPDIRECT3DDEVICE9 pDevice) {
	//	アイテムの頂点バッファを生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D)*NUM_VertexPolygon2D*NUM_MaxItem,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVB_Items,
		NULL
	);
}

//	アイテムを生成
int CreateItem(D3DXVECTOR3 pos, float siz) {
	//	変数宣言
	int Manager;	//	管理番号
	VERTEX_3D*pVtx;	//	頂点情報生成用のポインタ
	
	g_pVB_Items->Lock(0,0,(void**)&pVtx,0);	//	頂点バッファの範囲をロック

	//	現存していない変数（と番号）を検索
	for (Manager = 0; Manager < NUM_MaxItem; Manager++, pVtx += NUM_VertexPolygon2D)
		if (g_flgItems[Manager] == false)break;	//	使われていない変数が見つかると繰り返しから抜ける
	if (Manager >= NUM_MaxItem) {
		g_pVB_Items->Unlock();
		return -1;
	}

	g_flgItems[Manager] = true;	//	使用フラグを立てる

	//	頂点の初期化
	//	-	大きさを設定
	pVtx[0].pos = D3DXVECTOR3(-siz, +siz, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+siz, +siz, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-siz, -siz, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+siz, -siz, 0.0f);
	//	-	法線ベクトルを設定
	pVtx[0].vec = 
	pVtx[1].vec =
	pVtx[2].vec =
	pVtx[3].vec = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	//	-	頂点カラーを設定
	pVtx[0].def = 
	pVtx[1].def =
	pVtx[2].def =
	pVtx[3].def = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	//	-	テクスチャ座標を設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//	アイテムの管理情報を設定
	g_objItems[Manager].pos = pos;
	g_objItems[Manager].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	g_objItems[Manager].rot = D3DXVECTOR2(0.0f,0.0f);
	g_objItems[Manager].siz = D3DXVECTOR3(siz,siz,siz);

	g_pVB_Items->Unlock();	//	頂点バッファの範囲をアンロック
	
	return Manager;
}

//	接触したアイテムの番号を返す（非接触時は1を返す）
int Get_TouchedItem(D3DXVECTOR3 pos, D3DXVECTOR3 siz) {
	//	変数宣言
	int	Manager = 0;	//	アイテムの番号

						//	接触するアイテムを判定（ボックスコライダー）
	for (Manager = 0; Manager < NUM_MaxItem; Manager++) {
		if (g_flgItems[Manager] == false)continue;	//	現存していないアイテムを無視
		if (pos.y + siz.y > g_objItems[Manager].pos.y - g_objItems[Manager].siz.y &&	//	高さ判定
			pos.y - siz.y < g_objItems[Manager].pos.y + g_objItems[Manager].siz.y)
			if (pos.x + siz.x > g_objItems[Manager].pos.x - g_objItems[Manager].siz.x &&	//	x判定
				pos.x - siz.x < g_objItems[Manager].pos.x + g_objItems[Manager].siz.x &&
				pos.z + siz.z > g_objItems[Manager].pos.z - g_objItems[Manager].siz.z &&	//	z判定
				pos.z - siz.z < g_objItems[Manager].pos.z + g_objItems[Manager].siz.z)
				return Manager;
	}

	return -1;	//	非接触時は1を返す
}

//	指定されたアイテムを無効化
void EraseItem(int Manager) {
	g_flgItems[Manager] = false;
	EraseItemFLG(Manager);
}