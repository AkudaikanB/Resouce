//	文字列(UI)系（と非エリアゲーム画面）


//	ファイル結合
#include "../Heads/string_sys.h"
#include "../Heads/sound.h"
#include "../Heads/player.h"
#include "../Heads/item.h"
#include "../Heads/stage.h"

//	グローバル変数宣言
LPD3DXFONT	g_pFont_Clause,	//	操作UI
			g_pFont_Index,	//	見出し
			g_pFont_Common;	//	汎用
bool flgMouseOver = false;

//	プロトタイプ宣言
void DrawTitle			(LPDIRECT3DDEVICE9 pDevice);
void DrawResult			(LPDIRECT3DDEVICE9 pDevice);
void DrawSet			(LPDIRECT3DDEVICE9 pDevice);
void DrawCoolTime		(LPDIRECT3DDEVICE9 pDevice);
void DrawEmergencyMenu	(LPDIRECT3DDEVICE9 pDevice);
void DrawTime			(LPDIRECT3DDEVICE9 pDevice,DWORD tim,RECT rec,D3DXCOLOR col);


//
//	関数定義
//
//	初期化
void InitString(LPDIRECT3DDEVICE9 pDevice) {
	//	DirectXでフォントを作成
	//	-	汎用フォント
	D3DXCreateFont(
		pDevice,	//	インターフェイスへのポインタ
		18, 8,		//	文字の高さ,幅（高さによって勝手に設定されるため0で良し）
		0, 0,		//	太字設定,ミップマップ
		FALSE,		//	斜体設定
		SHIFTJIS_CHARSET,	//	文字セット
		OUT_DEFAULT_PRECIS,	//	フォントサイズの一致方法
		DEFAULT_QUALITY,	//	実際のフォントへの一致方法
		DEFAULT_PITCH,		//	ピッチ
		"MS UI Gothic",		//	書体
		&g_pFont_Common		//	生成するフォント情報へのポインタ
	);
	//	-	強調フォント
	D3DXCreateFont(
		pDevice, 18, 8, 700, 0, FALSE,		//	太字は700
		SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH,
		"ＭＳ Ｐ明朝", &g_pFont_Index
	);
	//	-	タイトルメニューフォント
	D3DXCreateFont(
		pDevice, 24, 12, 700, 0, FALSE,
		SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH,
		"MS UI Gothic", &g_pFont_Clause
	);
}

//	フォントに使用したメモリを解放
void UninitString() {
	if (g_pFont_Clause) {
		g_pFont_Clause->Release();
		g_pFont_Clause = NULL;
	}
	if (g_pFont_Common) {
		g_pFont_Common->Release();
		g_pFont_Common = NULL;
	}
	if (g_pFont_Index) {
		g_pFont_Index->Release();
		g_pFont_Index = NULL;
	}
}

void DrawString(LPDIRECT3DDEVICE9 pDevice) {
	int nPhase = Get_numPhase(),	//	ゲームの状態を取得
		nState = Get_numState();	//	ゲーム上での状態を取得

	//	タイトル画面描画
	if (nPhase == IDX_phaTitle)DrawTitle(pDevice);

	//	結果画面描画
	if(nPhase == IDX_phaResult)DrawResult(pDevice);

	//	エリア内での描画
	if (nPhase == IDX_phaGaming) {
		if (nState == IDX_steSet)	DrawSet(pDevice);				//	準備状態の描画
		if (nState == IDX_steCool)	DrawCoolTime(pDevice);			//	終了時の描画
		if (nState == IDX_steEmergency)DrawEmergencyMenu(pDevice);	//	緊急メニューの描画

		//	数字を描画する矩形
		RECT rec = { 480,450,SIZE_ScreenWidth,SIZE_ScreenHeight };
		RECT recItem{ 0,450,100,SIZE_ScreenHeight };

		char strItem[8];
		wsprintf(strItem,"愛 = %02d",Get_numItem());

		//	時間の描画
		if (nState != IDX_steCool) {
			DrawTime(pDevice, Get_timOfArea(), rec, COL_String[IDX_colCommon]);		rec.top -= 30;
			g_pFont_Clause->DrawText(NULL, strItem, -1, &recItem, DT_LEFT, COL_String[IDX_colIndex]);
		}
		for (int Manager = 0; Manager<NUM_MaxGoal; Manager++) {if (Get_flgGoalCount(Manager)) {
			DrawTime(pDevice, Get_timOfGoal(Manager), rec, COL_INDEX[Manager]);	rec.top -= 30;
		}}
	}
}


//	タイトルメニューの描画
void DrawTitle(LPDIRECT3DDEVICE9 pDevice) {
	//	カーソルの取得
	int sel = Get_selMenu();

	//	文字列描画用の変数宣言
	RECT recItem00	={2,282,SIZE_ScreenWidth,316},
		recItem01	={2,322,SIZE_ScreenWidth,356},
		recItem02	={2,362,SIZE_ScreenWidth,396},
		recItem10	={0,280,SIZE_ScreenWidth,315},
		recItem11	={0,320,SIZE_ScreenWidth,355},
		recItem12	={0,360,SIZE_ScreenWidth,395};

	char strItem00[24]	="チャレンジモード",
		strItem01[24]	="サバイバルモード",
		strItem02[8]	="終わる";

	//	タイトルメニュー1の描画
	g_pFont_Clause->DrawText(
		NULL,		//	文字列を含んだスプライトへのポインタ（ないためNULL）
		strItem00,	//	描画する文字列
		-1,			//	NULLを終端文字とした文字数自動計算
		&recItem00,	//	描画する矩形
		DT_CENTER,	//	水平方向中央ぞろえ
		COL_String[IDX_colShadow]	//	描画する色
	);
	//	タイトルメニュー2の描画
	g_pFont_Clause->DrawText(NULL, strItem01, -1, &recItem01, DT_CENTER, COL_String[IDX_colShadow]);
	//	タイトルメニュー3の描画
	g_pFont_Clause->DrawText(NULL, strItem02, -1, &recItem02, DT_CENTER, COL_String[IDX_colShadow]);

	//	タイトルメニューのポップアップの描画
	D3DXVECTOR2 posMouse = Get_posMouse();
	if (sel!=-1||(posMouse.x > 160 && posMouse.x < 480)) {	//	横幅かカーソルでの検出
		if(sel==0||(posMouse.y>280&&posMouse.y<315)){g_pFont_Clause->DrawText(NULL,strItem00,-1,&recItem10,DT_CENTER,COL_String[IDX_colCommon]);if(flgMouseOver==false)PlaySound(SOUND_Jump);flgMouseOver=true;}else
		if(sel==1||(posMouse.y>320&&posMouse.y<355)){g_pFont_Clause->DrawText(NULL,strItem01,-1,&recItem11,DT_CENTER,COL_String[IDX_colCommon]);if(flgMouseOver==false)PlaySound(SOUND_Jump);flgMouseOver=true;}else
		if(sel==2||(posMouse.y>360&&posMouse.y<395)){g_pFont_Clause->DrawText(NULL,strItem02,-1,&recItem12,DT_CENTER,COL_String[IDX_colCommon]);if(flgMouseOver==false)PlaySound(SOUND_Jump);flgMouseOver=true;}
		else flgMouseOver = false;
	}
}
//	開始前待機状態の表示
void DrawSet(LPDIRECT3DDEVICE9 pDevice) {
	//	描画する矩形を生成
	RECT recGuide = {0,POS_ScreenCenter_Y-30,SIZE_ScreenWidth,POS_ScreenCenter_Y+30},
		recArea = {0,POS_ScreenCenter_Y-100,SIZE_ScreenWidth,POS_ScreenCenter_Y-60};
	char strGuide[64] = "〇ボタンか\nクリック→離してスタート！",	//	ガイド
		strArea[24];	//	エリア表示

	wsprintf(strArea, "エリア%02d", Get_numInArea() + 1);
	if (Get_flgSurvival())
		wsprintf(strArea, "サバイバルモード");

	//	状態確認を描画
	bool flgReady = Get_flgReady();
	
	if (flgReady)g_pFont_Clause->DrawText(NULL,strGuide,-1,&recGuide,DT_CENTER,COL_String[IDX_colPositive]);
	if(!flgReady)g_pFont_Clause->DrawText(NULL,strGuide,-1,&recGuide,DT_CENTER,COL_String[IDX_colCommon]);
	g_pFont_Clause->DrawText(NULL,strArea,-1,&recArea,DT_CENTER,COL_String[IDX_colIndex]);
}
//	終了時待機状態の表示
void DrawCoolTime(LPDIRECT3DDEVICE9 pDevice) {
	//	カーソルの取得
	int sel = Get_selMenu();
	//	文字列描画用の矩形
	RECT recMenu[3] = {
		{POS_ScreenCenter_X-180,POS_ScreenCenter_Y+20,POS_ScreenCenter_X-60,	POS_ScreenCenter_Y+38},
		{POS_ScreenCenter_X-60,	POS_ScreenCenter_Y+20,POS_ScreenCenter_X+60,	POS_ScreenCenter_Y+38},
		{POS_ScreenCenter_X+60,	POS_ScreenCenter_Y+20,POS_ScreenCenter_X+180,	POS_ScreenCenter_Y+38}
	},
		recArea = {220,200,320,220},
		recTime = {260,195,500,220},	//	エリア内時間
		recScore= {260,225,500,250};	//	エリア内スコア

	//	文字列の生成
	char strMenu[3][12] = {"リトライ", "終わる", "次のエリア"};	//	メニュー
	char strScore[16];	//	スコア
	char strArea[16];	//	エリア表記
	
	//	マウスの位置を取得
	D3DXVECTOR2 posMouse = Get_posMouse();

	//	メニュー背景用の矩形
	VERTEX_2D vtxMenu[2][4];
	Create_recPolygon(vtxMenu[0],RECT{120,185,520,290},D3DCOLOR_RGBA(0, 0, 64, 255));
	Create_recPolygon(vtxMenu[1],RECT{117,182,523,293},D3DCOLOR_RGBA(0, 128, 0, 255));

	//	メニューボタン用の矩形(ポップアップ)
	VERTEX_2D vtxItem[3][4];
	Create_recPolygon(vtxItem[0],recMenu[0],D3DCOLOR_RGBA(255,255,255,64));
	Create_recPolygon(vtxItem[1],recMenu[1],D3DCOLOR_RGBA(255,255,255,64));
	Create_recPolygon(vtxItem[2],recMenu[2],D3DCOLOR_RGBA(255,255,255,64));

	//	描画
	pDevice->SetTexture(0, NULL);	//	背景のテクスチャは無し

	pDevice->DrawPrimitiveUP(		//	背景1の描画
		D3DPT_TRIANGLESTRIP,
		NUM_Polygon2D,
		vtxMenu[1],
		sizeof(VERTEX_2D)
	);
	pDevice->DrawPrimitiveUP(		//	背景2の描画
		D3DPT_TRIANGLESTRIP,
		NUM_Polygon2D,
		vtxMenu[0],
		sizeof(VERTEX_2D)
	);

	g_pFont_Index->DrawText(NULL,strMenu[0],-1,&recMenu[0],DT_CENTER,COL_String[IDX_colShadow]);
	g_pFont_Index->DrawText(NULL,strMenu[1],-1,&recMenu[1],DT_CENTER,COL_String[IDX_colShadow]);
	g_pFont_Index->DrawText(NULL,strMenu[2],-1,&recMenu[2],DT_CENTER,COL_String[IDX_colShadow]);

	//	エリア表記
	wsprintf(strArea,"エリア%02d :",Get_numInArea()+1);
	if(Get_flgSurvival())
		wsprintf(strArea, "サバイバル :");
	g_pFont_Common->DrawText(NULL,strArea,-1,&recArea,DT_LEFT,COL_String[IDX_colCommon]);
	//	経過時間
	if (Get_flgSurvival())
		DrawTime(pDevice,Get_timAll(),recTime,COL_String[IDX_colCommon]);
	else
		DrawTime(pDevice,Get_timOfArea(),recTime,COL_String[IDX_colCommon]);
	//	スコア
	wsprintf(strScore,"スコア : %d",Get_numScore());
	g_pFont_Index->DrawText(NULL,strScore,-1,&recScore,DT_CENTER,COL_String[IDX_colCommon]);

	//	メニューのポップアップ（マウスの位置に応じる）
	if(sel!=-1||	//	カーソルの発生
		(posMouse.y>260 && posMouse.y<285)){	//	縦での検出
		if ((posMouse.x > recMenu[0].left&&posMouse.x < recMenu[0].right)||sel==0) {
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);	//	αブレンドを有効にする
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,NUM_Polygon2D,vtxItem[0],sizeof(VERTEX_2D));
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);	//	αブレンドを無効にする
			g_pFont_Index->DrawText(NULL,strMenu[0],-1,&recMenu[0],DT_CENTER,COL_String[IDX_colCommon]);
			if (flgMouseOver == false)PlaySound(SOUND_Over); flgMouseOver = true;
		}else
		if ((posMouse.x > recMenu[1].left&&posMouse.x < recMenu[1].right)||sel==1) {
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);	//	αブレンドを有効にする
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,NUM_Polygon2D,vtxItem[1],sizeof(VERTEX_2D));
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);	//	αブレンドを無効にする
			g_pFont_Index->DrawText(NULL,strMenu[1],-1,&recMenu[1],DT_CENTER,COL_String[IDX_colCommon]);
			if (flgMouseOver == false)PlaySound(SOUND_Over); flgMouseOver = true;
		}else
		if ((posMouse.x > recMenu[2].left&&posMouse.x < recMenu[2].right)||sel==2) {
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);	//	αブレンドを有効にする
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,NUM_Polygon2D,vtxItem[2],sizeof(VERTEX_2D));
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);	//	αブレンドを無効にする
			if(Get_numInArea()<NUM_LastArea&&Get_flgGoNext())
				g_pFont_Index->DrawText(NULL,strMenu[2],-1,&recMenu[2],DT_CENTER,COL_String[IDX_colPositive]);
			else 
				g_pFont_Index->DrawText(NULL,strMenu[2],-1,&recMenu[2],DT_CENTER,COL_String[IDX_colDisable]);
			if (flgMouseOver == false)PlaySound(SOUND_Over); flgMouseOver = true;
		}
		else flgMouseOver = false;
	}
}
//	結果画面の描画
void DrawResult(LPDIRECT3DDEVICE9 pDevice) {
	//	文字列描画用の矩形を生成
	RECT recCaption= {	0,50,SIZE_ScreenWidth,80 }, 
		recSentence = {	0,360,SIZE_ScreenWidth,420 },
		recTime [2]= {{120,170,SIZE_ScreenWidth-120,200},{122,172,SIZE_ScreenWidth-118,202}},
		recTimeB[2]= {{120,200,SIZE_ScreenWidth-120,230},{122,202,SIZE_ScreenWidth-118,232}},
		recScore[2]= {{120,250,SIZE_ScreenWidth-120,280},{122,252,SIZE_ScreenWidth-118,282}},
		recScoreB[2]={{120,280,SIZE_ScreenWidth-120,310},{122,282,SIZE_ScreenWidth-118,312}};

	//	文字列生成
	char strSentence[64] = "〇ボタンを押すか\n画面クリックでタイトルに戻ります。";	//	ガイド
	char strScore[8], strScoreB[8],	//	スコア
		rblTime[12],rblScore[12],	//	ラベル
		strCaption[36];				//	キャプション
		wsprintf(strScore,"%7d",Get_numScoreA());
		wsprintf(rblTime, "タイム : ");
		wsprintf(rblScore,"スコア : ");

	if (Get_flgSurvival()){
		wsprintf(strCaption,"サバイバルモード : 結果");
	}
	else{
		wsprintf(strCaption, "チャレンジモード : エリア%02d～%02d", Get_numArea0()+1, Get_numInArea()+1);
		if (Get_numArea0() == Get_numInArea()) 
			wsprintf(strCaption, "チャレンジモード : エリア%02d", Get_numInArea()+1);
		if (Get_numArea0() == 0 && Get_numInArea() == NUM_LastArea)
			wsprintf(strCaption, "チャレンジモード : コンプリート");
	}
	
	//	影を描画
	DrawTime(pDevice,Get_timAll(),recTime[1],COL_String[IDX_colShadow]);
	g_pFont_Clause->DrawText(NULL,strScore,-1,&recScore[1],DT_CENTER,COL_String[IDX_colShadow]);
	//	ガイドを描画
	g_pFont_Common->DrawText(NULL,strSentence,-1,&recSentence,DT_CENTER,COL_String[IDX_colCommon]);
	g_pFont_Clause->DrawText(NULL,rblTime,-1,&recTime[1],	DT_LEFT,COL_String[IDX_colCommon]);
	g_pFont_Clause->DrawText(NULL,rblScore,-1,&recScore[1],	DT_LEFT,COL_String[IDX_colCommon]);
	g_pFont_Clause->DrawText(NULL,strCaption,-1,&recCaption,DT_CENTER,COL_String[IDX_colCommon]);
	//	結果を描画
	DrawTime(pDevice,Get_timAll(),recTime[0],COL_String[IDX_colCommon]);
	g_pFont_Clause->DrawText(NULL,strScore,-1,&recScore[0],DT_CENTER,COL_String[IDX_colCommon]);
}
//	緊急メニューの描画
void DrawEmergencyMenu(LPDIRECT3DDEVICE9 pDevice) {
	//	カーソルの取得
	int sel = Get_selMenu();
	//	文字列描画用の矩形
	RECT recCaption = {	120,175,520,195 },
		recBackGame = { 120,210,520,230 },
		recRetry	= {	120,235,520,255 },
		recBackTitle= { 120,260,520,280 },
		recQuit		= {	120,285,520,305 };
	//	描画する文字列
	char strCaption[32],
		strBackGame	[16] = "ゲームにもどる",
		strRetry	[16] = "リトライ",
		strBackTitle[16] = "タイトル画面へ",
		strQuit		[16] = "ゲームを終わる";
	D3DXVECTOR2 posMouse = Get_posMouse();	//	マウスの位置を取得
	//	メニューの背景を作成（設定）
	VERTEX_2D vtxMenu[2][4];
	Create_recPolygon(vtxMenu[0],RECT{120,165,520,315},D3DCOLOR_RGBA(128,0,0,255));
	Create_recPolygon(vtxMenu[1],RECT{117,162,523,318},D3DCOLOR_RGBA(255,255,0,255));

	//	文字列生成
	wsprintf(strCaption,"ポーズ中 : エリア%02d",Get_numInArea()+1);
	if(Get_flgSurvival())
		wsprintf(strCaption, "ポーズ中 : サバイバル");

	//	描画
	pDevice->SetTexture(0,NULL);	//	背景のテクスチャは無し

	pDevice->DrawPrimitiveUP(		//	背景1の描画
		D3DPT_TRIANGLESTRIP,
		NUM_Polygon2D,
		vtxMenu[1],
		sizeof(VERTEX_2D)
	);
	pDevice->DrawPrimitiveUP(		//	背景2の描画
		D3DPT_TRIANGLESTRIP,
		NUM_Polygon2D,
		vtxMenu[0],
		sizeof(VERTEX_2D)
	);
	//	メニューの影を描画
	g_pFont_Index->DrawText(NULL,strBackGame,-1,&recBackGame,DT_CENTER,COL_String[IDX_colShadow]);
	g_pFont_Index->DrawText(NULL,strRetry,-1,	&recRetry,DT_CENTER,COL_String[IDX_colShadow]);
	g_pFont_Index->DrawText(NULL,strBackTitle,-1,&recBackTitle,DT_CENTER,COL_String[IDX_colShadow]);
	g_pFont_Index->DrawText(NULL,strQuit,-1,	&recQuit,DT_CENTER,COL_String[IDX_colShadow]);

	//	文章の描画
	g_pFont_Common->DrawText(NULL, strCaption, -1, &recCaption, DT_CENTER, COL_String[IDX_colCommon]);
	//	メニューのポップアップ
	if (sel != -1 ||
		((posMouse.x > 120 && posMouse.x < 520))) {	//	縦位置での検出
		if (sel == 0 ||
			(posMouse.y > 210 && posMouse.y < 230)) {
			g_pFont_Index->DrawText(NULL, strBackGame, -1, &recBackGame, DT_CENTER, COL_String[IDX_colCommon]);
			if (flgMouseOver==false)PlaySound(SOUND_Over); flgMouseOver = true;
		}else
		if (sel == 1 ||
			(posMouse.y > 235 && posMouse.y < 255)) {
			g_pFont_Index->DrawText(NULL, strRetry, -1, &recRetry, DT_CENTER, COL_String[IDX_colCommon]);
			if (flgMouseOver == false)PlaySound(SOUND_Over); flgMouseOver = true;
		}else
		if (sel == 2 ||
			(posMouse.y > 260 && posMouse.y < 280)) {
			g_pFont_Index->DrawText(NULL, strBackTitle, -1, &recBackTitle, DT_CENTER, COL_String[IDX_colCommon]);
			if (flgMouseOver == false)PlaySound(SOUND_Over); flgMouseOver = true;
		}else
		if (sel == 3 ||
			(posMouse.y > 285 && posMouse.y < 305)) {
			g_pFont_Index->DrawText(NULL, strQuit, -1, &recQuit, DT_CENTER, COL_String[IDX_colCommon]);
			if (flgMouseOver == false)PlaySound(SOUND_Over); flgMouseOver = true;
		}
		else flgMouseOver = false;
	}
}

//	時間を描画
void DrawTime(LPDIRECT3DDEVICE9 pDevice, DWORD tim, RECT rec,D3DXCOLOR col) {
	//	変数宣言
	char strTime[12];		//	描画する数字列
	int hou,min, sec, dot;	//	描画する数字

	//数字の計算
	hou = (tim / 3600000)%24;	//	時間
	min = (tim / 60000)%60;		//	分
	sec = (tim / 1000)%60;		//	秒
	dot = (tim / 10)%100;		//	0点下

	//	描画する数字
		wsprintf(strTime, "%02d'%02d.%02d", min, sec, dot);
	if(tim /1000 > 3600)
		wsprintf(strTime, "%02d:%02d'%02d.%02d", hou, min, sec, dot);

	//	描画
	g_pFont_Clause->DrawText(NULL,strTime,-1,&rec,DT_CENTER,col);
}
