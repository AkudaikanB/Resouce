//	目的地(=Goal)の制御

//	ファイル結合
#include "../Heads/goal.h"
#include "../Heads/player.h"

//	グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9 g_pVB_Goals;		//	目的地描画用の頂点バッファ
Object	g_objGoals[NUM_MaxGoal];			//	目的地の制御用変数
bool	g_flgGoals[NUM_MaxGoal];			//	目的地の有無
bool	g_flgGoalCount[NUM_MaxGoal];		//	ゴール発生時間
int		g_numItemToGoal[NUM_MaxGoal]
						[NUM_ItemToGoal];	//	ゴール条件用アイテム管理番号

//	プロトタイプ宣言
void MakeGoalsBuffer(LPDIRECT3DDEVICE9 pDevice);

//	
//	関数定義
//
//	初期化関数
void InitGoal(LPDIRECT3DDEVICE9 pDevice){
	MakeGoalsBuffer(pDevice);	//	頂点バッファを生成

	ZeroMemory(g_objGoals, sizeof(g_objGoals));	//	変数を0クリア（不定値読み取り防止）
	ZeroMemory(g_flgGoals, sizeof(g_flgGoals));	//	変数を0クリア（不定値読み取り防止）
	ZeroMemory(g_flgGoalCount, sizeof(g_flgGoalCount));	//	変数を0クリア（不定値読み取り防止）

	//	ゴール条件を初期化
	for(int Manager=0;Manager<NUM_MaxGoal;Manager++)
	for(int Eraser=0;Eraser<NUM_ItemToGoal;Eraser++)
		g_numItemToGoal[Manager][Eraser]=-1;
}

//	使用したメモリの開放
void UninitGoal(void) {
	if (g_pVB_Goals) {
		g_pVB_Goals->Release();
		g_pVB_Goals = NULL;
	}
}

//	目的地描画用の頂点バッファを生成
void MakeGoalsBuffer(LPDIRECT3DDEVICE9 pDevice) {

	//	頂点バッファを生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D)*NUM_MaxGoal*NUM_VtxGoal,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVB_Goals,
		NULL
	);
}

//	更新
void UpdateGoals(void) {
	for (int Manager = 0; Manager < NUM_MaxGoal; Manager++) {
		if (g_flgGoals[Manager] == false)continue;
		if (g_flgGoalCount[Manager]==false && Get_numItemOfGoal(Manager)<=Get_numItem())
			ChangeColorGoal(Manager, 0);else
		if (Get_numItemOfGoal(Manager) <= Get_numItem())
			ChangeColorGoal(Manager, 1);else
		if (Get_numItemOfGoal(Manager) > Get_numItem())
			ChangeColorGoal(Manager, 2);
	}
}

//	描画
void DrawGoals(LPDIRECT3DDEVICE9 pDevice) {
	D3DXMATRIXA16 matWorld;			//	ワールド行列を生成

	//	テクスチャーを設定（なし）
	pDevice->SetTexture(0,NULL);	//	テクスチャ無し：テクスチャのポインタをNULLに設定

	pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);			//	ライティング無効
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);	//	αブレンドを有効化する
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);	//	裏面カリングを有効化

	//	描画
	for (int Manager = 0; Manager < NUM_MaxGoal; Manager++) {
		if (g_flgGoals[Manager] == false)continue;	//	現存していないゴールを無視

		//	ワールド行列を初期化（ひとつ前のワールド行列を保持させないため）
		D3DXMatrixIdentity(&matWorld);					//	単位行列を格納
		pDevice->SetTransform(D3DTS_WORLD,&matWorld);	//	単位行列を設定

		//	頂点バッファをデータストリームにバインド
		pDevice->SetStreamSource(
			0,
			g_pVB_Goals,
			0,
			sizeof(VERTEX_3D)
		);

		//	描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,
			Manager*NUM_VtxGoal,
			NUM_PlgGoal
		);
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);			//	ライティング有効
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);	//	αブレンドを無効化する
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	//	裏面カリングを無効化
}

//	ゴール（大きさsiz.x*siz.z*siz.yの直方体）を（位置posに）生成
int CreateGoal(D3DXVECTOR3 pos,float rot ,D3DXVECTOR3 siz) {
	
	//	変数宣言
	int Manager = 0;	//	管理番号
	VERTEX_3D*pVtx;	//	頂点情報生成用のポインタ

	g_pVB_Goals->Lock(0, 0, (void**)&pVtx, 0);	//	頂点バッファの範囲をロック

	//	現存していない目的地要素を検索
	for (Manager = 0; Manager < NUM_MaxGoal; Manager++, pVtx += NUM_VtxGoal)
		if (g_flgGoals[Manager] == false) {
			break;	//	現存していない目的地が見つかると繰り返しを終了
		}
	if (Manager >= NUM_MaxGoal-1 || Manager<0) {
		g_pVB_Goals->Unlock();
		return NULL;
	}

	//	使用フラグを立てる
	g_flgGoals[Manager] = true;

	//	オブジェクトのゲーム内情報を設定
	g_objGoals[Manager].pos = pos;	//	位置を設定
	g_objGoals[Manager].siz = siz;	//	大きさを設定

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
	pVtx[6].def =pVtx[14].def =pVtx[22].def =	//	目的地の初期色は透明（暫定赤）
	pVtx[7].def =pVtx[15].def =pVtx[23].def =   D3DXCOLOR(1.0f,0.25f,0.25f,0.0f);

	//	テクスチャ座標（使わない）を設定
	pVtx[0].tex =pVtx[8].tex = pVtx[16].tex =
	pVtx[1].tex =pVtx[9].tex = pVtx[17].tex =
	pVtx[2].tex =pVtx[10].tex =pVtx[18].tex =
	pVtx[3].tex =pVtx[11].tex =pVtx[19].tex =
	pVtx[4].tex =pVtx[12].tex =pVtx[20].tex =
	pVtx[5].tex =pVtx[13].tex =pVtx[21].tex =
	pVtx[6].tex =pVtx[14].tex =pVtx[22].tex =
	pVtx[7].tex =pVtx[15].tex =pVtx[23].tex =   D3DXVECTOR2(0.0f,0.0f);


	g_pVB_Goals->Unlock();	//	頂点バッファをアンロック

	if (Manager>=NUM_MaxGoal)return NULL;
	return Manager;
}

void Set_numItemToGoal(int nGoal, int nItem) {
	for (int Manager = 0; Manager < NUM_ItemToGoal + 1; Manager++) {
		if (g_numItemToGoal[nGoal][Manager] == -1) {
			g_numItemToGoal[nGoal][Manager] = nItem;
			return;
		}
	}
}

void SetGoalCount(int Manager, bool set) {
	g_flgGoalCount[Manager] = set;
}

//	指定された目的地を無効化
void EraseGoal(int Manager) {
	g_flgGoals[Manager] = false;		//	現存判定を無効化
	g_flgGoalCount[Manager] = false;	//	カウンターをストップ

	//	ゴール条件を初期化
	for(int Eraser=0;Eraser<NUM_ItemToGoal;Eraser++)
		g_numItemToGoal[Manager][Eraser]=-1;

	EraseGoalFLG(Manager);
	Reset_cntGoal(Manager);
}

//	接触した目的地を返す（非接触時は1を返す）
int Get_TouchedGoal(D3DXVECTOR3 pos, D3DXVECTOR3 siz) {
	//	変数宣言
	int	Manager = 0;	//	目的地の番号

	//	接触する目的地を判定（ボックスコライダー）
	for (Manager = 0; Manager < NUM_MaxGoal; Manager++) {
		if (g_flgGoals[Manager] == false)continue;	//	現存していない目的地を無視
		if (pos.y + siz.y > g_objGoals[Manager].pos.y - g_objGoals[Manager].siz.y &&	//	高さ判定
			pos.y - siz.y < g_objGoals[Manager].pos.y + g_objGoals[Manager].siz.y)
		if (pos.x + siz.x > g_objGoals[Manager].pos.x - g_objGoals[Manager].siz.x &&	//	x判定
			pos.x - siz.x < g_objGoals[Manager].pos.x + g_objGoals[Manager].siz.x &&
			pos.z + siz.z > g_objGoals[Manager].pos.z - g_objGoals[Manager].siz.z &&	//	z判定
			pos.z - siz.z < g_objGoals[Manager].pos.z + g_objGoals[Manager].siz.z)
			return Manager;
	}

	return -1;	//	非接触時は1を返す
}
//	アイテム管理番号に対応したゴール番号を返す
int Get_StartingGoal(int nItem) {
	for(int ManagerA=0;ManagerA<NUM_MaxGoal;ManagerA++)
	for(int ManagerB=0;ManagerB<NUM_ItemToGoal;ManagerB++)
		if(g_numItemToGoal[ManagerA][ManagerB]==nItem)return ManagerA;

	return -1;	//	見つからない場合は-1を返す
}
//	ゴールの現存判定を返す
bool Get_flgGoalCount(int nManager) {
	return g_flgGoalCount[nManager];
}

//	ゴール判定用のアイテム数を取得
int Get_numItemOfGoal(int nManager){
	int cntItem = 0;
	for (int Manager = 0; Manager<NUM_ItemToGoal; Manager++)
		if (g_numItemToGoal[nManager][Manager] > -1)
			cntItem++;

	return cntItem;
}
//	ゴール判定
bool Get_toGoal(int nManager) {
	if (Get_numItem() >= Get_numItemOfGoal(nManager))return true;
	return false;
}
bool Get_Clear(void) {
	for (int Manager = 0; Manager < NUM_MaxGoal; Manager++)
		if (g_flgGoals[Manager])return false;
	return true;
}

void ChangeColorGoal(int nManager,int type) {
	VERTEX_3D*pVtx = NULL;

	g_pVB_Goals->Lock(0, 0,(void**)&pVtx,0);
	pVtx += nManager*NUM_VtxGoal;

	//	色を設定
	pVtx[0].def =pVtx[8].def = pVtx[16].def =
	pVtx[1].def =pVtx[9].def = pVtx[17].def =
	pVtx[2].def =pVtx[10].def =pVtx[18].def =
	pVtx[3].def =pVtx[11].def =pVtx[19].def =
	pVtx[4].def =pVtx[12].def =pVtx[20].def =
	pVtx[5].def =pVtx[13].def =pVtx[21].def =
	pVtx[6].def =pVtx[14].def =pVtx[22].def =	//	目的地の初期色は赤
	pVtx[7].def =pVtx[15].def =pVtx[23].def =   COL_String[IDX_colDenial];

	if (type == 1) {
		pVtx[0].def = pVtx[8].def = pVtx[16].def =
		pVtx[1].def = pVtx[9].def = pVtx[17].def =
		pVtx[2].def = pVtx[10].def = pVtx[18].def =
		pVtx[3].def = pVtx[11].def = pVtx[19].def =
		pVtx[4].def = pVtx[12].def = pVtx[20].def =
		pVtx[5].def = pVtx[13].def = pVtx[21].def =
		pVtx[6].def = pVtx[14].def = pVtx[22].def =	//	管理番号に合わせたカラーリング
		pVtx[7].def = pVtx[15].def = pVtx[23].def = COL_INDEX[nManager];
	}
	if (type == 2) {
		pVtx[0].def = pVtx[8].def = pVtx[16].def =
		pVtx[1].def = pVtx[9].def = pVtx[17].def =
		pVtx[2].def = pVtx[10].def = pVtx[18].def =
		pVtx[3].def = pVtx[11].def = pVtx[19].def =
		pVtx[4].def = pVtx[12].def = pVtx[20].def =
		pVtx[5].def = pVtx[13].def = pVtx[21].def =
		pVtx[6].def = pVtx[14].def = pVtx[22].def =	//	透明化
		pVtx[7].def = pVtx[15].def = pVtx[23].def = D3DCOLOR_RGBA(255,255,255,0);
	}

	g_pVB_Goals->Unlock();
}

//	現存するゴールの数を返す
int Get_numGoal(void){
	int cnt=0;

	for (int Manager = 0; Manager < NUM_MaxGoal; Manager++)
		if (g_flgGoals[Manager])cnt++;

	return cnt;
}