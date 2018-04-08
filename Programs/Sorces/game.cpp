//	ゲームのメイン処理

//	ファイル結合
#include "../Heads/game.h"
#include "../Heads/input.h"		//	入力系
#include "../Heads/sound.h"		//	音
#include "../Heads/light.h"		//	光
#include "../Heads/string_sys.h"//	文字列(UI)
#include "../Heads/camera.h"	//	カメラ
#include "../Heads/Area.h"		//	エリア
#include "../Heads/player.h"	//	プレイヤブルキャラクター
#include "../Heads/stage.h"		//	ブロック
#include "../Heads/item.h"		//	アイテムオブジェクト
#include "../Heads/goal.h"		//	目的地

//	グローバル変数
int g_numPhase = IDX_phaTitle;	//	ゲーム内の状態変数（タイトル,エリア内,結果画面）
int g_numState = IDX_steSet;	//	エリア内の状態変数（準備,エリア内,緊急メニュー,終了時）

int g_numArea0= 0;	//	開始エリアの番号
int g_numArea = 0;	//	現在のエリアの番号
int g_cntAwait = 0;	//	待機時間

int g_scoAll,		//	全エリアの合計スコア
	g_scoOnArea;	//	現在エリアでのスコア

bool g_flgStart = false;
bool g_flgSelect = false;
bool g_flgSurvival = 0;	//	モード切り替え（0:,1:サバイバル）
bool flgReady = false;	//	ゲーム開始前準備確認用変数

DWORD g_timUpdate;	//	時間更新時刻
DWORD g_cntAll;		//	開始エリアからの合計時間
DWORD g_cntOfArea;	//	エリア内経過時間
DWORD g_cntOfGoal[NUM_MaxGoal];		//	ゴールの発生時間
DWORD g_cntAddGoal;	//	ゴール追加時刻

int g_redBack = COL_redBack,	//	調整用背景色
	g_greBack = COL_greBack,
	g_bluBack = COL_bluBack,
	g_alpFade = 255;	//	フェードの不透明度
bool g_flgFade = 0;		//	フェードアウト:0/イン:1の判定変数
int	g_secFading = 0;	//	フェード時間（秒単位）

int g_selMenu = -1;

bool g_flgGoalSUV[NUM_MaxGoal];	//	サバイバルモード用の固定位置使用フラグ
bool g_flgItemSUV[NUM_MaxItem];	//
bool g_flgPointer;
bool g_flgClear = false;			//	エリア進行可否


//	プロトタイプ宣言
void Operate_onTitle(void);		//	タイトル画面の操作
void Operate_onResult(void);	//	結果画面の操作
void Ruling(void);				//	ゲーム制御
void Operate_onSet(void);		//	ゲーム開始前待機状態
void Operate_onCoolTime(void);	//	ゲーム終了時待機状態
void Operate_onEmergency(void);	//	緊急メニュー上での操作

//
//	関数定義
//
//	各オブジェクトの起動時設定関数を呼び出し
void InitGame(LPDIRECT3DDEVICE9 pDevice) {
	//	グローバル変数初期化
	g_numPhase = IDX_phaTitle,
		g_numState = IDX_steSet,
		g_numArea = 0;
	g_cntAwait = 0;
	g_cntAll = g_cntOfArea = 0;
	for (int Manager = 0; Manager < NUM_MaxGoal; Manager++) g_cntOfGoal[Manager] = 0;
	g_scoAll = g_scoOnArea = 0;
	g_flgSurvival = 0;
	g_redBack = COL_redBack,	//調整用背景色
		g_greBack = COL_greBack,
		g_bluBack = COL_bluBack,
	g_alpFade = 255;	//	フェード不透明度
		g_flgFade	= 1;//	フェードアウト:0/イン:1判定用変数
		g_secFading = 1;//	フェード時間
	flgReady = false;

	ShowCursor(false);		//	デフォルトマウスカーソルの非表示
	g_flgPointer = true;	//	マウスカーソルに合わせたポインターの表示

	ShowCursor(true);		//	デフォルトマウスカーソルの非表示

	//	ソース別初期化
	InitDrawState(pDevice);	//	描画用の共通数値を設定
	InitLight(pDevice);		//	光の起動時設定
	InitCamera(pDevice);	//	カメラの起動時設定
	InitString(pDevice);	//	文字列系の起動時設定

	InitPlayer(pDevice);	//	プレイヤブルキャラクターの起動時設定
	InitStage(pDevice);		//	ブロックの起動時設定
	InitItems(pDevice);		//	アイテムオブジェクトの起動時設定
	InitGoal(pDevice);		//	目的地の起動時設定

	InitArea(0);	//	エリアをチュートリアルステージに設定

	StopSound();
	PlaySound(MUSIC_Title);
}

//	各オブジェクトの最初期化関数を呼び出し
void UninitGame(void) {
	UninitInput();	//	入力系に使用したメモリの解放
	UninitSound();
	UninitString();	//	文字列系に使用したメモリの開放

	UninitPlayer();	//	プレイヤブルキャラクターに使用したメモリの解放
	UninitItems();	//	アイテムオブジェクトに使用したメモリの解放
	UninitStage();	//	ブロックに使用したメモリの解放
	UninitGoal();	//	目的地に使用したメモリの解放
}

//	各オブジェクトの更新関数を呼び出し
void UpdateGame(LPDIRECT3DDEVICE9 pDevice) {
	UpdateInput();	//	入力系の更新

	if (g_cntAwait > 0)g_cntAwait--;	//	待機カウント発生中
	else {					//	待機カウントがない時
		switch (g_numPhase) {
		case IDX_phaGaming:
			if (g_numState == IDX_steGaming) {
				UpdatePlayer();	//	プレイヤブルキャラクタの更新
				UpdateStage();	//	ブロックの更新
				UpdateItems();	//	アイテムオブジェクトの更新
				UpdateGoals();	//	目的地の更新
				Ruling();	//	ゲーム制御
			}
			if (g_numState == IDX_steSet)Operate_onSet();
			if (g_numState == IDX_steCool)Operate_onCoolTime();
			if (g_numState == IDX_steEmergency) Operate_onEmergency();
			break;
		case IDX_phaTitle:
			Operate_onTitle();	//	タイトル画面の操作
			break;
		case IDX_phaResult:
			Operate_onResult();	//	結果画面の操作
			break;
		}
	}
}

//	各オブジェクトの描画関数を呼び出し
void DrawGame(LPDIRECT3DDEVICE9 pDevice) {
	pDevice->Clear(	//	背景をクリア
		0,
		NULL,
		D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
		D3DCOLOR_RGBA(g_redBack,g_greBack,g_bluBack,255),			//	背景色を設定
		1.0f,
		0
	);

	pDevice->BeginScene();	//	描画登録開始
	{
		//	3D描画
		pDevice->SetFVF(FVF_VERTEX_3D);	//	3Dの頂点フォーマットを設定
		switch (g_numPhase) {
		case IDX_phaGaming:
			if (GetKeyboardTrigger(DIK_RETURN))
				ChangeLight(pDevice);

			UpdateCamera(pDevice);	//	カメラ情報の更新
			UpdateLight(pDevice);	//	光を更新

			DrawStage(pDevice);		//	ブロックの描画
			DrawGoals(pDevice);		//	目的地の描画
			DrawItems(pDevice);		//	アイテムオブジェクトの描画
			break;
		}

		//	UI
		pDevice->SetFVF(FVF_VERTEX_2D);	//	2Dの頂点フォーマットを設定
		DrawString(pDevice);	//	文字列（UI）の描画
	}
	pDevice->EndScene();	//	描画登録終了
	pDevice->Present(NULL,NULL,NULL,NULL);	//	描画する
}

//	タイトル画面上での操作
void Operate_onTitle(void) {
	D3DXVECTOR2 posMouse = Get_posMouse();	//	マウスの位置を取得
	
	if (GetJoyStickTrigger(2)||
		(GetMouseLeftTrigger()&& posMouse.x > 160 && posMouse.x < 480)) {	//	左クリック時
		if (g_selMenu==0||
			(posMouse.y > 280 && posMouse.y < 315)) {	//	ChallengeMode
			g_numPhase = IDX_phaGaming;	g_numState = IDX_steSet;
			g_flgSurvival = 0;
			g_numArea0 = g_numArea = 0;	InitArea(0);
			g_cntAll = 0;

			PlaySound(SOUND_cngPhase);
		}
		if (g_selMenu == 1 ||
			(posMouse.y > 320 && posMouse.y < 355)) {	//	SurvivalMode
			g_numPhase = IDX_phaGaming;	g_numState = IDX_steSet;
			g_flgSurvival = 1;
			g_numArea = 25; InitArea(25);
			g_cntAll = 0;

			PlaySound(SOUND_cngPhase);
		}
		if (g_selMenu == 2 ||
			(posMouse.y > 360 && posMouse.y < 395))	//	Quit
			QuitGame();
	}
	
	//	カーソルの発生
	if (g_selMenu==-1) {
		if (GetJoyStickLeftTrigger(0) || GetJoyStickLeftTrigger(1) ||
			GetJoyStickLeftTrigger(2) || GetJoyStickLeftTrigger(3)) {
			g_selMenu = 0;
		}
	}
	else {	//	カーソル移動
		if (GetJoyStickLeftTrigger(0) && g_selMenu > 0) {
			g_selMenu--;
			IsolateMouse();
		}else
		if (GetJoyStickLeftTrigger(3) && g_selMenu < 2){
			g_selMenu++;
			IsolateMouse();
		}
	}
}
//	結果画面上での操作
void Operate_onResult(void) {
	D3DXVECTOR2 posMouse = Get_posMouse();	//	マウスの位置を取得

	if (GetMouseLeftTrigger()||GetJoyStickTrigger(2)) {	//	画面クリック
		g_cntAll = g_scoAll = 0;	//	全エリア合計時間,合計スコアをリセット
		g_numPhase = IDX_phaTitle; SetMouseMode(false);	//	タイトル画面に切り替え

		StopSound();
		PlaySound(MUSIC_Title);
		PlaySound(SOUND_cngPhase);
	}
}
//	ゲーム制御
void Ruling(void) {
	//	終了判定
	//	-	ゴールの数が0になるとエリアクリアとする
	if (Get_Clear()) { 
		g_flgClear=true;
		ShowCursor(true);		//	デフォルトマウスカーソルの表示
		SetMouseMode(false); g_numState = IDX_steCool;

		if (GetJoyStickAnyControl())IsolateMouse();	//	マウス退避
	}
	if (Get_flgFallout()) {
		ShowCursor(true);
		SetMouseMode(false); g_numState = IDX_steCool;
		PlaySound(SOUND_Starting);

		if (GetJoyStickAnyControl())IsolateMouse();	//	マウス退避
	}

	//	ルール - サバイバルモード
	if (g_flgSurvival) {
		//	終了判定	-	制限時間
		if((long)g_cntOfArea <= 0){
			ShowCursor(true);		//	デフォルトマウスカーソルの表示
			SetMouseMode(false); g_numState = IDX_steCool;

			if (GetJoyStickAnyControl())IsolateMouse();	//	マウス退避
		}

		//	ゴール追加
		g_cntAddGoal += timeGetTime() - g_timUpdate;	//	カウント
		if (g_cntAddGoal / 1000 > 1) {	//一秒立つとゴール追加
			g_cntAddGoal = 0;	//	カウントリセット

			srand((unsigned int)timeGetTime());	//	乱数表を現在時刻で初期化
			int ManagerG=0,	//	ループ文制御用変数
				ManagerI=0,	//	ループ文制御用変数
				ranGoal=rand(),	//	ランダム発生用変数
				ranItem=rand(),	//	ランダム発生用変数
				ranFor=rand();	//	一部ループをランダム回数化

			for (ManagerG = 0; ManagerG < NUM_MaxGoal; ManagerG++) {
				ranGoal = (unsigned int)rand()%(NUM_MaxGoal);
				if (g_flgGoalSUV[ranGoal] == false) {
					break;
			}}

			int nGoal;

			if(ranGoal<NUM_MaxGoal && g_flgGoalSUV[ranGoal]==false){
				nGoal = CreateGoal(POS_SurvivalGoals[ranGoal],0,Siz_SurvivalGoals[ranGoal]);
				if (Get_numGoal() < NUM_MaxGoal && g_flgGoalSUV[ranGoal]==false) {

					g_flgGoalSUV[ranGoal] = true;	//	ゴールの位置使用フラグを立てる

					ranFor = (unsigned)rand() % 3+1;	//	ゴールに必要なアイテム数をランダムで取得
					for (int Manager = 0; Manager < ranFor; Manager++) {	//	ゴールに必要なアイテムを生成するfor
						for (ManagerI = 0; ManagerI < NUM_MaxItem; ManagerI++) {
							ranItem = (unsigned)rand()%NUM_MaxItem;	//	発生位置の使用情報で判定
							if (g_flgItemSUV[ranItem] == false) {	//	使用されていない番号の時抜ける
								break;
						}}
						int nItem = CreateItem(POS_SurvivalItems[ranItem],INIT_SizItem);
						Set_numItemToGoal(nGoal,nItem);
						g_flgItemSUV[ranItem] = true;
					}
				}
				else if(g_flgGoalSUV)EraseGoal(nGoal);
			}
		}
	}

	//	時間のカウント（更新）
	if(g_flgSurvival==0)
		g_cntOfArea += timeGetTime() - g_timUpdate;	//	エリア内時間
	else
		g_cntOfArea -= timeGetTime() - g_timUpdate;	//	エリア内時間

	g_cntAll += timeGetTime() - g_timUpdate;	//	エリア全体での経過時間
	for(int Manager=0;Manager<NUM_MaxGoal;Manager++)if(Get_flgGoalCount(Manager))
		g_cntOfGoal[Manager] += timeGetTime() - g_timUpdate;	//	アイテムの所持時間
	g_timUpdate = timeGetTime();	//	更新時刻の設定

	//	緊急メニューの表示
	if (GetKeyboardTrigger(DIK_ESCAPE) || GetJoyStickTrigger(9)) {
		ShowCursor(true);		//	デフォルトマウスカーソルの表示
		SetMouseMode(false); g_numState = IDX_steEmergency; 

		if (GetJoyStickAnyControl())IsolateMouse();	//	マウス退避
	}
}
//	開始前待機状態での操作
void Operate_onSet(void) {
	if (GetMouseLeftTrigger()||GetJoyStickTrigger(2)) {
		flgReady = true;	//	マウス押下判定
		PlaySound(SOUND_GetItem);
	}
	if ((GetMouseLeftRelease()||GetJoyStickRelease(2))&&flgReady == true) {	//	ゲームスタート判定
		SetMouseMode(true);			//	マウス固定on
		g_numState = IDX_steGaming;	//	ゲームスタート
		flgReady = false;			//	押下判定の解除
		g_timUpdate = timeGetTime();//	更新時刻の更新

		ShowCursor(false);		//	デフォルトマウスカーソルの非表示

		PlaySound(SOUND_Starting);
	}
}
//	終了時待機状態での操作
void Operate_onCoolTime(void) {
	D3DXVECTOR2 posMouse = Get_posMouse();	//	マウスの位置を取得

	//	クールタイムメニューの判定/処理
	if (GetJoyStickPress(2)||
		posMouse.y > 260 && posMouse.y < 280 && GetMouseLeftTrigger()) {	//	縦位置と左クリックで判定
		//	横位置での判定

		if(posMouse.x>140&&posMouse.x<260 ||
			g_selMenu==0){	//	リトライ
			InitArea(g_numArea);	//	エリア初期化
			PlaySound(SOUND_cngState);
		}
		if(posMouse.x>260&&posMouse.x<380||
			g_selMenu==1){	//	結果画面に移動
			g_scoAll += g_scoOnArea;	//	エリア分の得点を加算
			g_numState = IDX_steSet;	//	エリア初期化
			g_numPhase = IDX_phaResult;	g_cntAwait = 3;		//	結果画面に移動

			StopSound();
			PlaySound(MUSIC_Result);
			PlaySound(SOUND_cngPhase);
		}
		if(posMouse.x>380&&posMouse.x<500||
			g_selMenu==2){	//	次のエリアに移動
		if (g_numArea < NUM_LastArea && g_flgSurvival==0 && g_flgClear) {	//	最終エリアでないかどうかの判定
			g_scoAll += g_scoOnArea;	//	エリア分の得点を加算
			g_numArea++;	InitArea(g_numArea);	//	エリア移動,エリア初期化
			g_numState = IDX_steSet; g_cntAwait = 3;//	開始前待機状態に切り替え
			g_flgClear = false;
			PlaySound(SOUND_cngState);
		}}
	}
	//	カーソルの発生
	if (g_selMenu==-1) {
		if (GetJoyStickLeftTrigger(0) || GetJoyStickLeftTrigger(1) ||
			GetJoyStickLeftTrigger(2) || GetJoyStickLeftTrigger(3)) {
			g_selMenu = 2;
		}
	}
	else {	//	カーソル移動
		if (GetJoyStickLeftTrigger(1) && g_selMenu > 0) {
			g_selMenu--;
			IsolateMouse();
		}else
		if (GetJoyStickLeftTrigger(2)&&g_selMenu<2){
			g_selMenu++;
			IsolateMouse();
		}
	}
}
//	緊急メニュー
void Operate_onEmergency(void){
	D3DXVECTOR2 posMouse = Get_posMouse();	//	マウスの位置を取得

	if (GetJoyStickPress(2)||
		(GetMouseLeftTrigger() && posMouse.x > 120 && posMouse.x < 520)) {	//	左クリックと画面上x座標での検出
		//	y座標でメニューの検出
		if (g_selMenu == 0||
			(posMouse.y > 210 && posMouse.y < 230)) {	//	ゲームにもどる
			//	開始前待機状態に切り替え
			g_numState = IDX_steSet; g_cntAwait = 3;
		}
		if (g_selMenu == 1 ||
			(posMouse.y > 235 && posMouse.y < 255)) {	//	リトライ
			InitArea(g_numArea);
			PlaySound(SOUND_cngState);
		}
		if (g_selMenu == 2 ||
			(posMouse.y > 260 && posMouse.y < 280)) {	//	タイトル画面へ
			//	エリアゲームのかたずけ
			g_cntAll = g_scoAll = 0;
			Retry();
			//	タイトルに切り替え
			g_numPhase = IDX_phaTitle;

			StopSound();
			PlaySound(MUSIC_Title);
			PlaySound(SOUND_cngPhase);
		}
		if (g_selMenu == 3 ||
			(posMouse.y > 285 && posMouse.y < 305)) {	//	ゲームを終わる
			QuitGame();
		}
	}
	
	//	カーソルの発生
	if (g_selMenu==-1) {
		if (GetJoyStickLeftTrigger(0) || GetJoyStickLeftTrigger(1) ||
			GetJoyStickLeftTrigger(2) || GetJoyStickLeftTrigger(3)) {
			g_selMenu = 0;
		}
	}
	else {	//	カーソル移動
		if (GetJoyStickLeftTrigger(0) && g_selMenu > 0) {
			g_selMenu--;
			IsolateMouse();
		}else
		if (GetJoyStickLeftTrigger(3) && g_selMenu < 3){
			g_selMenu++;
			IsolateMouse();
		}
	}
}

//	描画用の共通値を設定
void InitDrawState(LPDIRECT3DDEVICE9 pDevice) {
	//	レンダーステートの設定
	pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);			//	ポリゴンの描画を表面のみにする
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		//	描画元のαを設定する
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);	//	描画先のαを設定する

	//	サンプラーステートの設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);//	テクスチャのU値をループに設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);//	テクスチャのV値をループに設定
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	//	補間設定
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	//	補間設定

	//	テクスチャステージステートの設定
	pDevice->SetTextureStageState(0,
		D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0,
		D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0,
		D3DTSS_ALPHAARG2, D3DTA_CURRENT);

}
//	2D描画用頂点情報の共通値を設定
void InitVertex2D(VERTEX_2D*pVtx){
	ZeroMemory(pVtx, sizeof(VERTEX_2D));	//	0クリア（不定値読み取り防止）

	pVtx->col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);	//	頂点職を完全不透明の白に設定

	pVtx->rhw = 1.0f;	//	rhwを設定

	pVtx->tex = D3DXVECTOR2(0.0f, 0.0f);	//	テクスチャ座標を設定
}
//	4角形2Dポリゴンの情報を生成
void Create_recPolygon(VERTEX_2D*pVtx0,RECT rec,D3DXCOLOR col) {
	//	4頂点を初期化
	InitVertex2D(&pVtx0[0]);
	InitVertex2D(&pVtx0[1]);
	InitVertex2D(&pVtx0[2]);
	InitVertex2D(&pVtx0[3]);

	//	4頂点に合わせたテクスチャ座標を設定
	pVtx0[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx0[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx0[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx0[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//	4頂点の位置を設定
	pVtx0[0].pos = D3DXVECTOR3((float)rec.left, (float)rec.top, 0.0f);
	pVtx0[1].pos = D3DXVECTOR3((float)rec.right,(float)rec.top, 0.0f);
	pVtx0[2].pos = D3DXVECTOR3((float)rec.left, (float)rec.bottom, 0.0f);
	pVtx0[3].pos = D3DXVECTOR3((float)rec.right,(float)rec.bottom, 0.0f);

	//	ポリゴンの色を設定
	pVtx0[0].col =
	pVtx0[1].col =
	pVtx0[2].col =
	pVtx0[3].col = col;
}


void IncreaseScore(int Score) {
	g_scoOnArea += Score;
}
void IncreaseCount(int Count) {
	g_cntOfArea += Count*100;
}
void Reset_selMenu(void) {
	g_selMenu = -1;
}
//	ブロックとの接触判定
bool Get_Floating(D3DXVECTOR3 pos,D3DXVECTOR3 siz){
	//	接触するブロックを判定（ボックスコライダー）
	for (int Manager = 0; Manager < NUM_MaxBlock; Manager++) {
		if (Get_flgBlock(Manager) == false)continue;	//	現存していないステージを無視
		if (pos.y + siz.y > Get_PosBlock(Manager).y - Get_SizBlock(Manager).y &&	//	高さ判定
			pos.y - siz.y < Get_PosBlock(Manager).y + Get_SizBlock(Manager).y)
		if (pos.x + siz.x > Get_PosBlock(Manager).x - Get_SizBlock(Manager).x &&	//	x判定
			pos.x - siz.x < Get_PosBlock(Manager).x + Get_SizBlock(Manager).x &&
			pos.z + siz.z > Get_PosBlock(Manager).z - Get_SizBlock(Manager).z &&	//	z判定
			pos.z - siz.z < Get_PosBlock(Manager).z + Get_SizBlock(Manager).z){ 
			return false;
	}}
	return true;
}

int Get_numPhase(void){
	return g_numPhase;
}
int Get_numState(void) {
	return g_numState;
}
int Get_numInArea(void) {
	return g_numArea;
}
int Get_numArea0(void) {
	return g_numArea0;
}
bool Get_flgSurvival(void) {
	return g_flgSurvival;
}
bool Get_flgReady(void) {
	return flgReady;
}
bool Get_flgGoNext(void) {
	return g_flgClear;
}

DWORD Get_timAll(void) {
	return g_cntAll;
}
DWORD Get_timOfArea(void) {
	return g_cntOfArea;
}
DWORD Get_timOfGoal(int Manager) {
	return g_cntOfGoal[Manager];
}

int Get_numScoreA(void) {
	return g_scoAll;
}
int Get_numScore(void) {
	return g_scoOnArea;
}
//	キーボード/ジョイスティックによるメニュー操作に用いるカーソルを取得
int Get_selMenu(void) {	
	return g_selMenu;
}

//	posをdestに1/10移動、距離を返す
float Move_10to1(D3DXVECTOR3*pos, D3DXVECTOR3 dest) {
	D3DXVECTOR3 vec = (dest-*pos)/10.0f;	//	2座標間の1/2距離

	*pos += vec;	//	距離を詰める

	return D3DXVec3Length(&vec);
}

void Retry(void) {
	//	エリア内時間,ゴール発生時間をリセット
	if (g_flgSurvival) {
		g_cntOfArea = 30000;
		g_cntAll = 0;
	}
	else
		g_cntOfArea = 0;
	for(int Manager=0;Manager<NUM_MaxGoal;Manager++)g_cntOfGoal[Manager]=0;
	g_cntAddGoal = 0;

	//	アイテム所持数をリセット

	//	エリア内スコアをリセット
	g_scoOnArea = 0;
	
	//	開始前待機状態に切り替え
	g_numState = IDX_steSet; g_cntAwait = 3;
}

void EraseGoalFLG(int Manager) {
	g_flgGoalSUV[Manager] = false;
}
void EraseItemFLG(int Manager) {
	g_flgItemSUV[Manager] = false;
}
void Reset_cntGoal(int Manager) {
	g_cntOfGoal[Manager] = 0;
}