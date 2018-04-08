//	ステージオブジェクト(=Block,以下ブロック)・ゴール判定の制御

//	ファイル結合
#include "../Heads/stage.h"
#include "../Heads/sound.h"

//	グローバル変数
LPDIRECT3DVERTEXBUFFER9	g_pVB_Blocks = NULL;	//	頂点情報バッファへのポインタ
Object	g_objBlocks[NUM_MaxBlock];	//	ブロックの情報変数
bool	g_flgBlocks[NUM_MaxBlock];	//	ブロックの現存情報
int		g_numFlagGoal[NUM_MaxBlock];	//	ブロックの消去判定（目的地消去で判定）
D3DXVECTOR3 g_desBlock[NUM_MaxBlock];	//	ブロックの移動先

//	プロトタイプ宣言
void MakeBlocksBuffer(LPDIRECT3DDEVICE9 pDevice);	//	ブロックの頂点情報生成

//
//	関数定義
//
//	初期化関数
void InitStage(LPDIRECT3DDEVICE9 pDevice) {
	//	頂点・インデックスバッファの生成
	MakeBlocksBuffer(pDevice);

	//	初期化
	ZeroMemory(&g_objBlocks, sizeof(Object)*NUM_MaxBlock);	//	ブロックの情報をすべて0で上書き（不定値読み取り防止）
	ZeroMemory(&g_flgBlocks, sizeof(bool)*NUM_MaxBlock);	//	ブロック現存情報をすべて0で上書き（不定値読み取り防止）

//	SetMouseMode(true);	//	マウス制限
}
//	メモリー開放
void UninitStage(void) {
	if (g_pVB_Blocks) {	//	頂点バッファを解放
		g_pVB_Blocks->Release();
		g_pVB_Blocks = NULL;
	}
}

//	更新
void UpdateStage(void) {
}
//	描画
void DrawStage(LPDIRECT3DDEVICE9 pDevice) {
	//	変数宣言
	D3DXMATRIXA16 matWorld;

	//	頂点フォーマットを設定
	pDevice->SetFVF(FVF_VERTEX_3D);	//	3D頂点フォーマット

	//	テクスチャーを設定（なし）
	pDevice->SetTexture(0,NULL);	//	テクスチャ無し：テクスチャのポインタをNULLに設定

	for (int Manager = 0; Manager < NUM_MaxBlock; Manager++) {
		if (g_flgBlocks[Manager] == false)continue;		//	現存しないブロックを無視

		//	ワールド行列をリセット（ひとつ前の行列情報を保持させないため）
		D3DXMatrixIdentity(&matWorld);					//	単位行列を格納
		pDevice->SetTransform(D3DTS_WORLD, &matWorld);	//	行列を設定

		//	頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(
			0,					//	ストリームを指定
			g_pVB_Blocks,		//	頂点バッファへのポインタを渡す
			0,					//	オフセットを設定
			sizeof(VERTEX_3D)	//	大きさを渡す
		);

		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,
			Manager*NUM_VtxBlock,
			NUM_plgBlock
		);
	}
}

//	情報制御用バッファ準備(初期化)
void MakeBlocksBuffer(LPDIRECT3DDEVICE9 pDevice) {

	//	頂点情報制御用のバッファを準備
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D)*NUM_MaxBlock*NUM_VtxBlock,	//	用意するバッファのサイズ
		D3DUSAGE_WRITEONLY,		//	頂点バッファの使用法を指定
		FVF_VERTEX_3D,			//	情報のフォーマットを設定（3D描画用の頂点フォーマットを設定）
		D3DPOOL_MANAGED,		//	バッファを保持するメモリクラスを指定
		&g_pVB_Blocks,			//	準備するバッファへのポインタを指定
		NULL					//	(	使わないため基本はNULL	)
	);
}

//	pos位置にsiz.x*siz.y*siz.zの直方体を生成
int CreateBlock(D3DXVECTOR3 pos,float rot, D3DXVECTOR3 siz) {
	//	変数宣言
	int Manager = 0;	//	管理番号
	VERTEX_3D*pVtx;	//	頂点情報生成用のポインタ

	g_pVB_Blocks->Lock(0, 0, (void**)&pVtx, 0);	//	頂点バッファの範囲をロック

	//	現存していないブロック要素を検索
	for (Manager = 0; Manager < NUM_MaxBlock; Manager++, pVtx += NUM_VtxBlock)
		if (g_flgBlocks[Manager] == false)break;	//	現存していないブロックが見つかると繰り返しを終了

	//	使用フラグを立てる
	g_flgBlocks[Manager] = true;

	//	オブジェクトのゲーム内情報を設定
	g_objBlocks[Manager].pos = pos;	//	位置を設定
	g_objBlocks[Manager].siz = siz;	//	大きさを設定

	//	頂点情報を生成
	//	-	位置を代入
	pVtx[0].pos =pVtx[8].pos = pVtx[16].pos =
	pVtx[1].pos =pVtx[9].pos = pVtx[17].pos =
	pVtx[2].pos =pVtx[10].pos =pVtx[18].pos =
	pVtx[3].pos =pVtx[11].pos =pVtx[19].pos =
	pVtx[4].pos =pVtx[12].pos =pVtx[20].pos =
	pVtx[5].pos =pVtx[13].pos =pVtx[21].pos =
	pVtx[6].pos =pVtx[14].pos =pVtx[22].pos =
	pVtx[7].pos =pVtx[15].pos =pVtx[23].pos =   pos;
	//	-	位置から大きさ分頂点をずらす
	pVtx[0].pos += D3DXVECTOR3(+siz.x*cosf(rot)-siz.z*-sinf(rot), +siz.y, -siz.x*sinf(rot)+siz.z*cosf(rot));
	pVtx[1].pos += D3DXVECTOR3(-siz.x*cosf(rot)-siz.z*-sinf(rot), +siz.y, +siz.x*sinf(rot)+siz.z*cosf(rot));
	pVtx[2].pos += D3DXVECTOR3(+siz.x*cosf(rot)+siz.z*-sinf(rot), +siz.y, -siz.x*sinf(rot)-siz.z*cosf(rot));
	pVtx[3].pos += D3DXVECTOR3(-siz.x*cosf(rot)+siz.z*-sinf(rot), +siz.y, +siz.x*sinf(rot)-siz.z*cosf(rot));
	pVtx[10].pos+= D3DXVECTOR3(+siz.x*cosf(rot)-siz.z*-sinf(rot), -siz.y, -siz.x*sinf(rot)+siz.z*cosf(rot));
	pVtx[11].pos+= D3DXVECTOR3(-siz.x*cosf(rot)-siz.z*-sinf(rot), -siz.y, +siz.x*sinf(rot)+siz.z*cosf(rot));
	pVtx[6].pos += D3DXVECTOR3(+siz.x*cosf(rot)+siz.z*-sinf(rot), -siz.y, -siz.x*sinf(rot)-siz.z*cosf(rot));
	pVtx[7].pos += D3DXVECTOR3(-siz.x*cosf(rot)+siz.z*-sinf(rot), -siz.y, +siz.x*sinf(rot)-siz.z*cosf(rot));
	pVtx[14].pos = pVtx[16].pos = pVtx[0].pos;
	pVtx[18].pos = pVtx[20].pos = pVtx[1].pos;
	pVtx[4].pos = pVtx[12].pos = pVtx[2].pos;
	pVtx[5].pos = pVtx[22].pos = pVtx[3].pos;
	pVtx[15].pos = pVtx[17].pos = pVtx[10].pos;
	pVtx[19].pos = pVtx[21].pos = pVtx[11].pos;
	pVtx[8].pos = pVtx[13].pos = pVtx[6].pos;
	pVtx[9].pos = pVtx[23].pos = pVtx[7].pos;

	//	面の法線ベクトルを設定
	pVtx[0].vec =	pVtx[1].vec =	pVtx[2].vec =	pVtx[3].vec =	D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
	pVtx[4].vec =	pVtx[5].vec =	pVtx[6].vec =	pVtx[7].vec =	D3DXVECTOR3( 0.0f, 0.0f,-1.0f);
	pVtx[8].vec =	pVtx[9].vec =	pVtx[10].vec =	pVtx[11].vec =	D3DXVECTOR3( 0.0f,-1.0f, 0.0f);
	pVtx[12].vec =	pVtx[13].vec =	pVtx[14].vec =	pVtx[15].vec =	D3DXVECTOR3( 1.0f, 0.0f, 0.0f);
	pVtx[16].vec =	pVtx[17].vec =	pVtx[18].vec =	pVtx[19].vec =	D3DXVECTOR3( 0.0f, 0.0f, 1.0f);
	pVtx[20].vec =	pVtx[21].vec =	pVtx[22].vec =	pVtx[23].vec =	D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

	//	色を設定
	pVtx[0].def =pVtx[8].def = pVtx[16].def =
	pVtx[1].def =pVtx[9].def = pVtx[17].def =
	pVtx[2].def =pVtx[10].def =pVtx[18].def =
	pVtx[3].def =pVtx[11].def =pVtx[19].def =
	pVtx[4].def =pVtx[12].def =pVtx[20].def =
	pVtx[5].def =pVtx[13].def =pVtx[21].def =
	pVtx[6].def =pVtx[14].def =pVtx[22].def =	//	ブロックの色は白
	pVtx[7].def =pVtx[15].def =pVtx[23].def =   D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	//	テクスチャ座標（使わない）を設定
	pVtx[0].tex =pVtx[8].tex = pVtx[16].tex =
	pVtx[1].tex =pVtx[9].tex = pVtx[17].tex =
	pVtx[2].tex =pVtx[10].tex =pVtx[18].tex =
	pVtx[3].tex =pVtx[11].tex =pVtx[19].tex =
	pVtx[4].tex =pVtx[12].tex =pVtx[20].tex =
	pVtx[5].tex =pVtx[13].tex =pVtx[21].tex =
	pVtx[6].tex =pVtx[14].tex =pVtx[22].tex =
	pVtx[7].tex =pVtx[15].tex =pVtx[23].tex =   D3DXVECTOR2(0.0f,0.0f);


	g_pVB_Blocks->Unlock();	//	頂点バッファをアンロック

	return Manager;
}

//	接触したブロックを返す（非接触時は1を返す）
int Get_TouchedBlock(D3DXVECTOR3 pos, D3DXVECTOR3 siz) {
	//	変数宣言
	int	Manager = 0;	//	ブロックの番号

	//	接触するブロックを判定（ボックスコライダー）
	for (Manager = 0; Manager < NUM_MaxBlock; Manager++) {
		if (g_flgBlocks[Manager] == false)continue;	//	現存していないステージを無視
		if (pos.y + siz.y > g_objBlocks[Manager].pos.y - g_objBlocks[Manager].siz.y &&	//	高さ判定
			pos.y - siz.y < g_objBlocks[Manager].pos.y + g_objBlocks[Manager].siz.y)
		if (pos.x + siz.x > g_objBlocks[Manager].pos.x - g_objBlocks[Manager].siz.x &&	//	x判定
			pos.x - siz.x < g_objBlocks[Manager].pos.x + g_objBlocks[Manager].siz.x &&
			pos.z + siz.z > g_objBlocks[Manager].pos.z - g_objBlocks[Manager].siz.z &&	//	z判定
			pos.z - siz.z < g_objBlocks[Manager].pos.z + g_objBlocks[Manager].siz.z){ 
			return Manager;
		}
	}

	return -1;	//	非接触時は1を返す
}

//	指定されたブロックの位置を取得
D3DXVECTOR3 Get_PosBlock(int nManager) {
	return g_objBlocks[nManager].pos;
}
//	指定されたブロックの大きさを取得
D3DXVECTOR3 Get_SizBlock(int nManager) {
	return g_objBlocks[nManager].siz;
}
//	指定されたブロックの使用フラグを取得
bool Get_flgBlock(int nManager) {
	return g_flgBlocks[nManager];
}

void EraseBlock(int Index) {
	g_flgBlocks[Index] = false;
	g_desBlock[Index] = INIT_Vector3D;
	g_numFlagGoal[Index] = -1;
}

//	ゴール消滅に応じたブロック消去
void EraseBlockByGoal(int nGoal){
	for (int Manager = 0; Manager < NUM_MaxBlock; Manager++) {
		//	nGoal番目のゴールに対する消去対象を判定
		if (g_numFlagGoal[Manager] == nGoal) {
			EraseBlock(Manager);	//	消去
			PlaySound(SOUND_cngPhase);
		}
	}
}

//	足場消去に対応するゴール番号の設定
void Set_numEraseByGoal(int nStage,int nGoal){
	g_numFlagGoal[nStage] = nGoal;
}
