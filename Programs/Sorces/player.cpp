//	プレイヤブルキャラクターの制御

//	ファイル結合
#include "../Heads/input.h"
#include "../Heads/sound.h"
#include "../Heads/player.h"
#include "../Heads/stage.h"
#include "../Heads/goal.h"
#include "../Heads/item.h"

//	グローバル変数宣言
Character g_objPlayer;	//	プレイヤブルキャラクターの制御変数
int g_numItem;			//	アイテムの所持数
int g_cntToSprint;		//	疾走判定用カウント
D3DXVECTOR3 g_posDestClimb;
DWORD g_cntStepSound;	//	踏み込み音判定用カウンタ
bool g_flgSprint;		//	疾走判定
bool g_flgOverSound;	//	飛び越し音判定用カウンタ
bool g_flgStand = false;	//	浮遊中判定
bool g_flgClimb = false;	//	段差の上昇判定
bool g_flgGround = false;	//	着地
int numClimb[4];	//	よじ登り先（管理番号,方向(0:z+, 1:x+, 2:z-, 3:x-),
					//				次の体勢,今の体勢(3:通り越し)）
bool g_flgTransform = false;	//	立ち/伏せ切り替え時制限

D3DXVECTOR3 g_sizStand	= INIT_sizPlayer;	//	立ち状態時の大きさ
D3DXVECTOR3 g_sizLay	= INIT_sizPlayer;	//	伏せ状態時の大きさ

//	プロトタイプ宣言
void RotationView(void);	//	注視点移動（カメラとは別）
void Stepping(void);		//	接地移動（歩き/駆け足/走り）
void Leaping(void);			//	飛び跳ねる
void Sliding(void);			//	スライディング
void Climbing(void);		//	よじ登り
void Action(void);			//	アクション(状態固定)操作

void Move_PlayerXZ(void);	//	プレイヤーのX/Z軸移動
void Move_PlayerY(void);	//	プレイヤーのY軸移動
void TransformPlayer(void);	//	プレイヤーの大きさ変更

//
//	関数宣言
//
//	初期化
void InitPlayer(LPDIRECT3DDEVICE9 pDevice) {
	ZeroMemory(&g_objPlayer,sizeof(Character));	//	情報をすべて0にする（不定値読み取り防止）

	g_objPlayer.pos = INIT_posPlayer;	//	プレイヤブルキャラクターの初期位置
	g_objPlayer.vec = INIT_vecPlayer;	//	プレイヤブルキャラクターの向いている方向
	g_objPlayer.siz = INIT_sizPlayer;	//	プレイヤブルキャラクターの大きさ(xを半径とした高さyの円柱)
	g_objPlayer.rot = INIT_rotPlayer;	//	プレイヤブルキャラクターの回転度の設定
	g_objPlayer.posEye = INIT_talEye;	//	プレイヤブルキャラクターの視点の高さ
	g_objPlayer.higStand = INIT_sizPlayer.y;	//	プレイヤブルキャラクターの立ち時身長(1/2サイズ)
	g_objPlayer.higLay = INIT_sizPlayer.y/4.0f;	//	伏せ時高さ(身長の1/8)
	g_objPlayer.vecFlow = INIT_vecFlow;	//	プレイヤブルキャラクターの非接地移動ベクトル
	g_objPlayer.flgRun = false;			//	プレイヤブルキャラクターの走りスイッチ
	g_objPlayer.spdStep = 0.0f;			//	プレイヤブルキャラクターの移動速度
	g_objPlayer.spdJump =				//	ジャンプ力と
	g_objPlayer.spdFall = 0.0f;			//	落下速度を0に設定

	g_objPlayer.rot.x -= D3DXToRadian(-30);

	for(int Manager=0;Manager<NUM_MaxItem;Manager++)
		g_objPlayer.Item[Manager] = false;	//	所持アイテムをなしに設定

	numClimb[0] = -1; numClimb[1] = -1;
	g_cntToSprint = 0;
	g_flgSprint = false;
	g_posDestClimb = INIT_Vector3D;
}
//	最初期化
void UninitPlayer(void) {
}

//	更新
void UpdatePlayer(void) {
	//	変数リセット
	g_flgStand	= false;	//	浮遊
	g_flgClimb	= false;	//	段差
	g_flgGround	= false;	//	着地

	//	プレイヤーのサイズ変更
	TransformPlayer();

	//	常時可能操作
	//	-	歩き/走り切り替え
	if (GetKeyboardTrigger(DIK_F)||GetJoyStickTrigger(6)) {
		g_objPlayer.flgRun = !g_objPlayer.flgRun;
	}

	//	状態(アクション)判定
	//	-	よじ登り状態
	if (g_objPlayer.mortion == morClimb) Climbing();
	//	-	通常状態
	else{
		Move_PlayerY();	//	y軸移動
		Move_PlayerXZ();//	x/z平面移動

		Action();	//	アクション操作
	}

	RotationView();	//	注視点移動

	//	アイテムの取得判定
	int nItem = Get_TouchedItem(g_objPlayer.pos, g_objPlayer.siz);	//	アイテムとの接触判定
	if (nItem != -1){
		g_numItem++;		//	所持アイテム数を増加
		int nGoal = Get_StartingGoal(nItem);
		if (nGoal != -1) {
			g_objPlayer.Item[nItem] = true;
			SetGoalCount(nGoal,true);
			ChangeColorGoal(nGoal, 0);
			if (Get_toGoal(nGoal)) {
				ChangeColorGoal(nGoal, 1);
			}
		}
		PlaySound(SOUND_GetItem);
		IncreaseScore(10);	//	アイテム取得ボーナススコア加点
		EraseItem(nItem);	//	取得したアイテムを消去
	}

	//	目的地と対応アイテム所持判定
	int nGoal = Get_TouchedGoal(g_objPlayer.pos, g_objPlayer.siz);	//	ゴールとの接触判定
	if (nGoal != -1 && Get_toGoal(nGoal)) {	//	ゴール判定
		int nDecrease = Get_numItemOfGoal(nGoal);		//	ゴール必要アイテム数を取得
		DWORD nBonus = 100 - Get_timOfGoal(nGoal)/1000;	//	タイムボーナス計算
		if (nBonus < 0 || Get_flgGoalCount(nGoal)==false)nBonus = 0;	//	ボーナススコアが負の時0とする
		
		if (Get_flgSurvival()) {
			IncreaseCount(nDecrease*100);
			IncreaseCount((int)nBonus);
		}

		if (Get_toGoal(nGoal)) {
			ChangeColorGoal(nGoal, 1);
		}

		PlaySound(SOUND_GoalClear);

		EraseBlockByGoal(nGoal);
		IncreaseScore(nDecrease*100);	//	ゴール達成スコア加点
		IncreaseScore(nBonus);			//	タイムボーナススコア加点
		g_numItem -= nDecrease;	//	ゴール分アイテム減少
		EraseGoal(nGoal);		//	目的地消去
	}


}

//	プレイヤブルキャラクターの位置を取得
D3DXVECTOR3 Get_posEye(void) { 
	D3DXVECTOR3 posEye = g_objPlayer.pos;	//	プレイヤブルキャラクターの中心位置を取得
		posEye.y += g_objPlayer.posEye;		//	位置から目の位置を設定
	return posEye;
}
D3DXVECTOR3 Get_vecPlayer(void) { return g_objPlayer.vec; }	//	プレイヤブルキャラクターの向きを取得
D3DXVECTOR3 Get_sizPlayer(void) { return g_objPlayer.siz; }	//	プレイヤブルキャラクターの大きさを取得

//	注視点移動
void RotationView(void){
	//	変数宣言
	D3DXVECTOR3 posEye = g_objPlayer.pos;	//	視点の位置
	posEye.y += g_objPlayer.posEye;			//	プレイヤブルキャラクターの中心位置から目の位置へずらす

	long inX = GetMouseAxisY();	//	縦方向回転入力

	if (IsJoyStick(0)) {
		if (GetJoyStickPositionZ() > 40000) g_objPlayer.rot.y += 0.03125f;
		if (GetJoyStickPositionZ() < 25000) g_objPlayer.rot.y -= 0.03125f;
		if (GetJoyStickPositionRZ() > 40000 && g_objPlayer.rot.x < D3DXToRadian(+89)) g_objPlayer.rot.x += 0.015625f;
		if (GetJoyStickPositionRZ() < 25000 && g_objPlayer.rot.x > D3DXToRadian(-89)) g_objPlayer.rot.x -= 0.015625f;
	}

	g_objPlayer.rot.y += 0.001f* GetMouseAxisX();		//	横回転
	if ((inX < 0 && g_objPlayer.rot.x > D3DXToRadian(-89)) ||	//	縦回転の
		(inX > 0 && g_objPlayer.rot.x < D3DXToRadian(+89)))		//	回転制限
		g_objPlayer.rot.x += 0.001f* GetMouseAxisY();	//	縦回転

	//	横回転の数字制限
	if (g_objPlayer.rot.y < 0)g_objPlayer.rot.y = D3DXToRadian(360)+g_objPlayer.rot.y;
	if (g_objPlayer.rot.y > D3DXToRadian(360))g_objPlayer.rot.y = g_objPlayer.rot.y-D3DXToRadian(360);

	//	縦回転制限（回転後引き戻し）
	if (g_objPlayer.rot.x < D3DXToRadian(-90)) {
		g_objPlayer.rot.x = D3DXToRadian(-89);
	}if (g_objPlayer.rot.x > D3DXToRadian(90)) {
		g_objPlayer.rot.x = D3DXToRadian(89);
	}

	g_objPlayer.vec = D3DXVECTOR3(
		posEye.x + sinf(g_objPlayer.rot.y)*(cosf(g_objPlayer.rot.x)),
		posEye.y - sinf(g_objPlayer.rot.x),
		posEye.z + cosf(g_objPlayer.rot.y)*(cosf(g_objPlayer.rot.x))
	);
}

//	接地移動（歩き/駆け足/疾走）
void Stepping(void) {
	//	変数宣言
	D3DXVECTOR3 posEye =			//	視点の位置
		g_objPlayer.pos;			//	プレイヤブルキャラクターの中心位置
	posEye.y += g_objPlayer.posEye;	//	プレイヤブルキャラクターの中心位置から目の位置へずらす

	//	移動方向を取得
	D3DXVECTOR3 vecMove = g_objPlayer.vec - posEye;
	vecMove.y = 0.0f;

	//	疾走判定
	if (GetKeyboardTrigger(DIK_W)){	//	移動キーの押下判定
		if (g_cntToSprint == 0 && g_objPlayer.mortion == morStand)
			g_cntToSprint++;
		else if(g_cntToSprint<15&&g_cntToSprint>1)
			g_flgSprint = true;
	}
	if (GetJoyStickPress(6))	g_flgSprint = true;
	if (GetJoyStickRelease(6))	g_flgSprint = false;
	if((g_objPlayer.flgRun ||g_flgSprint)&&g_objPlayer.mortion!=morLay){
		if (GetKeyboardRelease(DIK_W)) {	//	移動キーの解放判定
			g_objPlayer.mortion = morStagger;	//	よろけ
			g_flgSprint = false;
		}
		if (GetKeyboardTrigger(DIK_LSHIFT)||GetJoyStickTrigger(4)) {
			g_objPlayer.mortion = morSlide;
			PlaySound(SOUND_Slide);
			g_flgSprint = false;
		}
	}
	if(g_cntToSprint > 15)g_cntToSprint=0;
	if (g_cntToSprint > 0)g_cntToSprint++;

	//	移動速度を設定
	if (GetKeyboardPress(DIK_W) ||	//	移動キー/ボタンの入力判定
		GetKeyboardPress(DIK_S) ||
		GetKeyboardPress(DIK_A) ||
		GetKeyboardPress(DIK_D) ||
		(IsJoyStick(0)&&(
		GetJoyStickPositionY()<25000|| 
		GetJoyStickPositionY()>40000|| 
		GetJoyStickPositionX()<25000|| 
		GetJoyStickPositionX()>40000 ))) {
		if(g_objPlayer.mortion != morLay && g_objPlayer.mortion != morSlide)
			g_objPlayer.mortion = morStand;
		g_objPlayer.spdStep += 2.0f;	//	加速
		g_cntStepSound++;
	}
	else {
		g_objPlayer.spdStep -= 1.0f;
		if(g_objPlayer.spdStep<10)g_objPlayer.spdStep=0;	//	-以下にしない
	}
	//	-	移動速度制限
	if (g_flgSprint){
		if (g_objPlayer.spdStep > 70.0f) {
			g_objPlayer.spdStep = 70.0f;	//	疾走
		}
	}
	else{
	if (g_objPlayer.flgRun==true	&& g_objPlayer.spdStep>50.0f)
		g_objPlayer.spdStep = 50.0f;	//	駆け足
	if ((g_objPlayer.flgRun==false||g_objPlayer.mortion==morLay)&& g_objPlayer.spdStep>20.0f && g_objPlayer.mortion!=morSlide)
		g_objPlayer.spdStep = 20.0f;	//	歩き
	}

	//	移動ベクトルを単位ベクトル化
	D3DXVec3Normalize(&vecMove, &vecMove);

	//	移動
	if (GetKeyboardPress(DIK_W)||(IsJoyStick(0)&&GetJoyStickPositionY()<25000)) { 	//	前方移動
		g_objPlayer.vecFlow += vecMove*g_objPlayer.spdStep; 
	}
	if (GetKeyboardPress(DIK_S)||(IsJoyStick(0)&&GetJoyStickPositionY()>40000)) {	//	後方移動
		g_objPlayer.vecFlow -= vecMove*g_objPlayer.spdStep; 
	}
	if (GetKeyboardPress(DIK_A)||(IsJoyStick(0)&&GetJoyStickPositionX()<25000)) {	//	左側へ移動
		D3DXVECTOR3 vec2;
		vec2.x = -vecMove.z;
		vec2.z = vecMove.x;
		vec2.y = 0.0f;
		g_objPlayer.vecFlow += vec2*g_objPlayer.spdStep;
	}
	if (GetKeyboardPress(DIK_D)||(IsJoyStick(0)&&GetJoyStickPositionX()>40000)) {	//	右側へ移動
		D3DXVECTOR3 vec2;
		vec2.x = vecMove.z;
		vec2.z = -vecMove.x;
		vec2.y = 0.0f;
		g_objPlayer.vecFlow += vec2*g_objPlayer.spdStep;
	}

	//	よろけ移動
	if (g_objPlayer.mortion == morStagger) {
		D3DXVECTOR3 vecWobble;
		D3DXVec3Normalize(&vecWobble, &g_objPlayer.vecFlow);

		g_cntStepSound++;

		g_objPlayer.vecFlow += vecWobble*g_objPlayer.spdStep;
		if (g_objPlayer.spdStep == 0)g_objPlayer.mortion=morStand;
	}

	if ((g_flgSprint || g_objPlayer.flgRun) && g_cntStepSound > 20) {
		g_cntStepSound=0;
		PlaySound(SOUND_Step);
	}
	else if(g_cntStepSound>30){
		g_cntStepSound = 0;
		PlaySound(SOUND_Step);
	}
}
//	状態固定アクション(スライディング,よじ登り等)への操作
void Action(void) {
	//	変数宣言
	int Manager=0;	//	繰り返し文制御用
	bool flgBreak[NUM_MaxBlock];
	D3DXVECTOR3 dest = g_objPlayer.pos;	//	判定用位置
	D3DXVECTOR3 vec = g_objPlayer.vec-g_objPlayer.pos;	//	判定移動用方向
	float		rot = g_objPlayer.rot.y;//	回転角判定

	vec.y = 0.0f;					//	すべてのアクションはx/z平面で判定を行う
	D3DXVec3Normalize(&vec,&vec);	//	単位ベクトル化
	
	for(Manager=0;Manager<NUM_MaxBlock;Manager++)flgBreak[Manager]=false;

	//	よじ登り
	for (Manager = 0; Manager < 100; Manager++) {	//	1メートル判定
		dest += vec;	//	1cm移動

		for(int ManagerB=0;ManagerB<NUM_MaxBlock;ManagerB++){	//	ブロックとの接触判定
			if (Get_flgBlock(ManagerB) == false || flgBreak[ManagerB])continue;	//	現存していないステージを無視
			if (dest.y + g_objPlayer.siz.y*1.5f > Get_PosBlock(ManagerB).y + Get_SizBlock(ManagerB).y &&	//	高さ判定
				dest.y - g_objPlayer.siz.y/2.0f < Get_PosBlock(ManagerB).y + Get_SizBlock(ManagerB).y)
			if (dest.x + g_objPlayer.siz.x > Get_PosBlock(ManagerB).x - Get_SizBlock(ManagerB).x &&	//	x判定
				dest.x - g_objPlayer.siz.x < Get_PosBlock(ManagerB).x + Get_SizBlock(ManagerB).x &&
				dest.z + g_objPlayer.siz.z > Get_PosBlock(ManagerB).z - Get_SizBlock(ManagerB).z &&	//	z判定
				dest.z - g_objPlayer.siz.z < Get_PosBlock(ManagerB).z + Get_SizBlock(ManagerB).z){ 
				//	変数宣言
				D3DXVECTOR3 posBlock = Get_PosBlock(ManagerB),	//	ブロックの位置
							sizBlock = Get_SizBlock(ManagerB);	//	

				//	接触判定の制限
				//	-	x軸成分の制限
				if ((rot > D3DXToRadian(315) || rot < D3DXToRadian(45)) ||	//	回転成分での判定
					(rot > D3DXToRadian(135) && rot < D3DXToRadian(225))){
					if (vec.x>0 && dest.x + g_objPlayer.siz.x > posBlock.x - sizBlock.x){
						dest.x-=1.0f;
						vec.x = 0;	//	x軸成分移動をなくす
					}else
					if (vec.x<0 && dest.x - g_objPlayer.siz.x < posBlock.x + sizBlock.x){
						dest.x+=1.0f;
						vec.x = 0;	//	x軸成分移動をなくす
					}
				}
				//	-	z軸成分の制限
				if((	rot > D3DXToRadian(45) && rot < D3DXToRadian(135)) ||	//	回転成分での判定
						rot > D3DXToRadian(225) && rot < D3DXToRadian(315)){
					if (vec.z>0 && dest.z + g_objPlayer.siz.z > posBlock.z - sizBlock.z){
						dest.z-=1.0f;
						vec.z = 0;	//	z軸成分移動をなくす
					}else
					if (vec.z<0 && dest.z - g_objPlayer.siz.z < posBlock.z + sizBlock.z){
						dest.z+=1.0f;
						vec.z = 0;	//	z軸成分移動をなくす
					}
				}

				//	よじ登りアクション判定（入力,移動可能,モーションの判定）
				if((GetKeyboardPress(DIK_SPACE) && GetKeyboardPress(DIK_W))||GetJoyStickPress(5)){
					if(Get_Floating(
					D3DXVECTOR3(dest.x,posBlock.y+sizBlock.y+g_objPlayer.higLay,dest.z),
					D3DXVECTOR3(g_objPlayer.siz.x, g_objPlayer.higLay,g_objPlayer.siz.z))&&
					g_objPlayer.mortion!=morClimb){

					//	-	プレイヤーの回転角度での判定
					if ((rot >= D3DXToRadian(315) || rot < D3DXToRadian(45)) && 
						g_objPlayer.pos.z<posBlock.z-sizBlock.z) {

						//	+z向き
						PlaySound(SOUND_Climb);
						numClimb[0] = ManagerB;	//	よじ登り先を設定
						numClimb[1] = 2;		//	登る方向
						g_posDestClimb = D3DXVECTOR3(	//	登る位置
							dest.x,
							posBlock.y + sizBlock.y + g_objPlayer.higStand,
							posBlock.z - sizBlock.z);
						//	登り後の 立ち/伏せ の設定
						if (Get_Floating(g_posDestClimb,g_sizStand)==false)	//	非接触判定
							numClimb[2]=morLay;	//	伏せ

						//	上る姿勢
						//	-	ブロック頂上の高さで判定
						if (g_objPlayer.pos.y <= posBlock.y + sizBlock.y) {
							numClimb[3] = 0;
							g_posDestClimb.y =posBlock.y + sizBlock.y;
							g_posDestClimb.z =posBlock.z - sizBlock.z;
						}
						else {
							numClimb[3] = 1;
						}

						g_objPlayer.mortion = morClimb;	//	よじ登りに状態を切り替え
					}else
					if (rot >= D3DXToRadian(45)&&rot < D3DXToRadian(135) &&
						g_objPlayer.pos.x<posBlock.x-sizBlock.x) {

						//	+x向き
						PlaySound(SOUND_Climb);
						numClimb[0] = ManagerB;	//	よじ登り先を設定
						numClimb[1] = 3;		//	登る方向
						g_posDestClimb = D3DXVECTOR3(	//	登る位置
							posBlock.x - sizBlock.x,
							posBlock.y + sizBlock.y + g_objPlayer.higStand,
							dest.z);
						//	登り後の 立ち/伏せ の設定
						if (Get_Floating(g_posDestClimb,g_sizStand)==false)	//	非接触判定
							numClimb[2]=morLay;	//	伏せ

						//	上る姿勢
						//	-	ブロック頂上の高さで判定
						if (g_objPlayer.pos.y <= posBlock.y + sizBlock.y) {
							numClimb[3] = 0;
							g_posDestClimb.y =posBlock.y + sizBlock.y;
							g_posDestClimb.x =posBlock.x - sizBlock.x;
						}
						else {
							numClimb[3] = 1;
						}

						g_objPlayer.mortion = morClimb;	//	よじ登りに状態を切り替え
					}else
					if (rot >= D3DXToRadian(135)&&rot < D3DXToRadian(225) && 
						g_objPlayer.pos.z>posBlock.z+sizBlock.z) {
						//	-z向き
						PlaySound(SOUND_Climb);
						numClimb[0] = ManagerB;	//	よじ登り先を設定
						numClimb[1] = 0;		//	登る方向
						g_posDestClimb = D3DXVECTOR3(	//	登る位置
							dest.x,
							posBlock.y + sizBlock.y + g_objPlayer.higStand,
							posBlock.z + sizBlock.z);
						//	登り後の 立ち/伏せ の設定
						if (Get_Floating(g_posDestClimb,g_sizStand)==false)	//	非接触判定
							numClimb[2]=morLay;	//	伏せ

						//	上る姿勢
						//	-	ブロック頂上の高さで判定
						if (g_objPlayer.pos.y <= posBlock.y + sizBlock.y) {
							numClimb[3] = 0;
							g_posDestClimb.y =posBlock.y + sizBlock.y;
							g_posDestClimb.z =posBlock.z + sizBlock.z;
						}
						else {
							numClimb[3] = 1;
						}

						g_objPlayer.mortion = morClimb;	//	よじ登りに状態を切り替え

					}else
					if (rot >= D3DXToRadian(225) && rot < D3DXToRadian(315) &&
						g_objPlayer.pos.x>posBlock.x+sizBlock.x) {

						//	-x向き
						PlaySound(SOUND_Climb);
						numClimb[0] = ManagerB;	//	よじ登り先を設定
						numClimb[1] = 1;		//	登る方向
						g_posDestClimb = D3DXVECTOR3(	//	登る位置
							posBlock.x + sizBlock.x,
							posBlock.y + sizBlock.y + g_objPlayer.higStand,
							dest.z);
						//	登り後の 立ち/伏せ の設定
						if (Get_Floating(g_posDestClimb,g_sizStand)==false)	//	非接触判定
							numClimb[2]=morLay;	//	伏せ

						//	上る姿勢
						//	-	ブロック頂上の高さで判定
						if (g_objPlayer.pos.y <= posBlock.y + sizBlock.y) {
							numClimb[3] = 0;
							g_posDestClimb.y =posBlock.y + sizBlock.y;
							g_posDestClimb.x =posBlock.x + sizBlock.x;
						}
						else {
							numClimb[3] = 1;
						}

						g_objPlayer.mortion = morClimb;	//	よじ登りに状態を切り替え
					}
					else {
						continue;	//	当てはまらないものをスルー
					}
				}
			}
			flgBreak[ManagerB] = true; 
	}}}
}
void Sliding(void) {
	if (g_objPlayer.mortion == morSlide) {	//	スライディング判定

	//	移動減衰
	g_objPlayer.spdStep -= 1.0f;
	if (g_objPlayer.spdStep < 15) {
		g_objPlayer.spdStep = 0;	//	-以下にしない
	}

		D3DXVECTOR3 vecSlide;
		D3DXVec3Normalize(&vecSlide, &g_objPlayer.vecFlow);

		g_objPlayer.vecFlow += vecSlide*g_objPlayer.spdStep;
		if (g_objPlayer.spdStep == 0)g_objPlayer.mortion = morLay;
	}
}
//	よじ登り
void Climbing(void) {
	//	変数宣言
	D3DXVECTOR3 dest = INIT_Vector3D;

	if (numClimb[3] == 0)	//	持ち上げ
		dest = g_posDestClimb;
	else if (numClimb[3] == 2) {	//	飛び越し
		if (numClimb[1] == 0)dest = D3DXVECTOR3(g_objPlayer.pos.x, Get_PosBlock(numClimb[0]).y + Get_SizBlock(numClimb[0]).y, Get_PosBlock(numClimb[0]).z - Get_SizBlock(numClimb[0]).z - g_objPlayer.siz.z);
		if (numClimb[1] == 1)dest = D3DXVECTOR3(Get_PosBlock(numClimb[0]).x - Get_SizBlock(numClimb[0]).x - g_objPlayer.siz.z, Get_PosBlock(numClimb[0]).y + Get_SizBlock(numClimb[0]).y, g_objPlayer.pos.z);
		if (numClimb[1] == 2)dest = D3DXVECTOR3(g_objPlayer.pos.x, Get_PosBlock(numClimb[0]).y + Get_SizBlock(numClimb[0]).y, Get_PosBlock(numClimb[0]).z + Get_SizBlock(numClimb[0]).z + g_objPlayer.siz.z);
		if (numClimb[1] == 3)dest = D3DXVECTOR3(Get_PosBlock(numClimb[0]).x + Get_SizBlock(numClimb[0]).x + g_objPlayer.siz.z, Get_PosBlock(numClimb[0]).y + Get_SizBlock(numClimb[0]).y, g_objPlayer.pos.z);
	}
	else {
		if (numClimb[1] == 0)	//	+z側
			dest = D3DXVECTOR3(g_objPlayer.pos.x,
				Get_PosBlock(numClimb[0]).y + Get_SizBlock(numClimb[0]).y,
				Get_PosBlock(numClimb[0]).z + Get_SizBlock(numClimb[0]).z);
		if (numClimb[1] == 1)	//	+x側
			dest = D3DXVECTOR3(
				Get_PosBlock(numClimb[0]).x + Get_SizBlock(numClimb[0]).x,
				Get_PosBlock(numClimb[0]).y + Get_SizBlock(numClimb[0]).y,
				g_objPlayer.pos.z);
		if (numClimb[1] == 2)	//	-z側
			dest = D3DXVECTOR3(g_objPlayer.pos.x,
				Get_PosBlock(numClimb[0]).y + Get_SizBlock(numClimb[0]).y,
				Get_PosBlock(numClimb[0]).z - Get_SizBlock(numClimb[0]).z);
		if (numClimb[1] == 3)	//	-x側
			dest = D3DXVECTOR3(
				Get_PosBlock(numClimb[0]).x - Get_SizBlock(numClimb[0]).x,
				Get_PosBlock(numClimb[0]).y + Get_SizBlock(numClimb[0]).y,
				g_objPlayer.pos.z);
		if (numClimb[2] == morStand || numClimb[2] == morJump)
			dest.y += g_objPlayer.higStand;
		if (numClimb[2] == morLay)
			dest.y += g_objPlayer.higLay;

	}
	g_flgTransform = true;

	//	登り派生行動
	if (numClimb[2] != morLay) {
		if (GetKeyboardTrigger(DIK_SPACE)||GetJoyStickTrigger(5))	numClimb[2] = morJump;	//	登りジャンプ
		if (GetKeyboardTrigger(DIK_LSHIFT)||GetJoyStickTrigger(4))	numClimb[2] = morLay;	//	ほふく

		float lenOver = 0.0f;	//	飛び越し距離
		D3DXVECTOR3 posOver = INIT_Vector3D;	//	飛び越し先計算
		if (numClimb[1] == 0)posOver = D3DXVECTOR3(g_objPlayer.pos.x, Get_PosBlock(numClimb[0]).y + Get_SizBlock(numClimb[0]).y, Get_PosBlock(numClimb[0]).z - Get_SizBlock(numClimb[0]).z - g_objPlayer.siz.z - 1);
		if (numClimb[1] == 1)posOver = D3DXVECTOR3(Get_PosBlock(numClimb[0]).x - Get_SizBlock(numClimb[0]).x - g_objPlayer.siz.x - 1, Get_PosBlock(numClimb[0]).y + Get_SizBlock(numClimb[0]).y, g_objPlayer.pos.z);
		if (numClimb[1] == 2)posOver = D3DXVECTOR3(g_objPlayer.pos.x, Get_PosBlock(numClimb[0]).y + Get_SizBlock(numClimb[0]).y, Get_PosBlock(numClimb[0]).z + Get_SizBlock(numClimb[0]).z + g_objPlayer.siz.z + 1);
		if (numClimb[1] == 3)posOver = D3DXVECTOR3(Get_PosBlock(numClimb[0]).x + Get_SizBlock(numClimb[0]).x + g_objPlayer.siz.x + 1, Get_PosBlock(numClimb[0]).y + Get_SizBlock(numClimb[0]).y, g_objPlayer.pos.z);

		if (numClimb[1] == 0 || numClimb[1] == 2)lenOver = Get_SizBlock(numClimb[0]).z;
		if (numClimb[1] == 1 || numClimb[1] == 3)lenOver = Get_SizBlock(numClimb[0]).x;

		//	飛び越し状態への操作
		if ((GetKeyboardPress(DIK_F)||GetJoyStickPress(7)) && lenOver < 50.0f&& g_flgOverSound == false &&	//	入力&条件判定
			Get_Floating(posOver, g_objPlayer.siz)) {	//	非接触判定

			numClimb[3] = 2;	//	飛び越し状態
			PlaySound(SOUND_Over);
			g_flgOverSound = true;
		}
	}

	//	移動
	if (Move_10to1(&g_objPlayer.pos, dest) < 1.0f) { //	移動量で判定
		g_objPlayer.pos = dest;				//	移動
		if (numClimb[3]) {
			g_objPlayer.mortion = numClimb[2];	//	体制変更

			if (numClimb[2] == morJump) {
				g_objPlayer.pos.y++;
				g_objPlayer.spdJump += 10.0f;
				g_objPlayer.mortion = morStand;
				PlaySound(SOUND_Jump);
			}
			g_flgOverSound = false;
		}
		else
			numClimb[3]++;
	}
}
//	跳ねる
void Leaping(void){
	if (GetKeyboardRelease(DIK_SPACE)||GetJoyStickTrigger(5)) {
		g_objPlayer.spdJump += 10.1f;
		PlaySound( SOUND_Jump);
	}
}
//	プレイヤーのX/Z軸移動
void Move_PlayerXZ(void) {
	//	変数宣言
	D3DXVECTOR3 dest = INIT_Vector3D;

	if (g_flgGround == false)
		g_objPlayer.pos.x += g_objPlayer.vecFlow.x;	//	x移動
	for (int Manager = 0; Manager < NUM_MaxBlock; Manager++) {
		if (Get_flgBlock(Manager) == false)continue;	//	現存していないステージを無視

		//	ボックスコライダー(接触判定)
		if (g_objPlayer.pos.y + g_objPlayer.siz.y > Get_PosBlock(Manager).y - Get_SizBlock(Manager).y &&	//	高さ判定
			g_objPlayer.pos.y - g_objPlayer.siz.y < Get_PosBlock(Manager).y + Get_SizBlock(Manager).y)
		if (g_objPlayer.pos.x + g_objPlayer.siz.x > Get_PosBlock(Manager).x - Get_SizBlock(Manager).x &&	//	x判定
			g_objPlayer.pos.x - g_objPlayer.siz.x < Get_PosBlock(Manager).x + Get_SizBlock(Manager).x &&
			g_objPlayer.pos.z + g_objPlayer.siz.z > Get_PosBlock(Manager).z - Get_SizBlock(Manager).z &&	//	z判定
			g_objPlayer.pos.z - g_objPlayer.siz.z < Get_PosBlock(Manager).z + Get_SizBlock(Manager).z) {	//	-1は判定外（非接触）

			//	埋没判定
			D3DXVECTOR3
				posBlock = Get_PosBlock(Manager),
				sizBlock = Get_SizBlock(Manager),
				dest3 = g_objPlayer.pos; dest3.y = posBlock.y + sizBlock.y + g_objPlayer.siz.y + 1;

			//	低い段差を判定
			if (g_flgStand && g_objPlayer.pos.y - g_objPlayer.siz.y / 2.0f > posBlock.y + sizBlock.y
				&& Get_Floating(dest3, g_objPlayer.siz)) {

				//	ある程度の高さを上る
				g_objPlayer.pos.y = posBlock.y + sizBlock.y + g_objPlayer.siz.y + 1;
			}
			else {
				//	押し出し
				if (g_objPlayer.vecFlow.x > 0)
					g_objPlayer.pos.x = posBlock.x - sizBlock.x - g_objPlayer.siz.x - 1;
				if (g_objPlayer.vecFlow.x < 0)
					g_objPlayer.pos.x = posBlock.x + sizBlock.x + g_objPlayer.siz.x + 1;
			}
		}
	}
	if (g_flgGround == false)
		g_objPlayer.pos.z += g_objPlayer.vecFlow.z;	//	z軸移動
	for (int Manager = 0; Manager < NUM_MaxBlock; Manager++) {	//	ブロックとの判定
		if (Get_flgBlock(Manager) == false)continue;	//	現存していないブロックを無視
		if (g_objPlayer.pos.y + g_objPlayer.siz.y > Get_PosBlock(Manager).y - Get_SizBlock(Manager).y &&	//	高さ判定
			g_objPlayer.pos.y - g_objPlayer.siz.y < Get_PosBlock(Manager).y + Get_SizBlock(Manager).y)
		if (g_objPlayer.pos.x + g_objPlayer.siz.x > Get_PosBlock(Manager).x - Get_SizBlock(Manager).x &&	//	x判定
			g_objPlayer.pos.x - g_objPlayer.siz.x < Get_PosBlock(Manager).x + Get_SizBlock(Manager).x &&
			g_objPlayer.pos.z + g_objPlayer.siz.z > Get_PosBlock(Manager).z - Get_SizBlock(Manager).z &&	//	z判定
			g_objPlayer.pos.z - g_objPlayer.siz.z < Get_PosBlock(Manager).z + Get_SizBlock(Manager).z) {	//	-1は判定外（非接触）

			//	埋没判定
			D3DXVECTOR3
				posBlock = Get_PosBlock(Manager),
				sizBlock = Get_SizBlock(Manager),
				dest3 = g_objPlayer.pos; dest3.y = posBlock.y + sizBlock.y + g_objPlayer.siz.y + 1;

			//	低い段差を判定
			if (g_flgStand && (g_objPlayer.pos.y - g_objPlayer.siz.y / 2.0f > posBlock.y + sizBlock.y || g_flgClimb)
				&& Get_Floating(dest3, g_objPlayer.siz)) {

				//	ある程度の高さを上る
				g_objPlayer.pos.y = posBlock.y + sizBlock.y + g_objPlayer.siz.y + 1;
			}
			else {
				//	押し出し
				if (g_objPlayer.vecFlow.z > 0)
					g_objPlayer.pos.z = posBlock.z - sizBlock.z - g_objPlayer.siz.z - 1;
				if (g_objPlayer.vecFlow.z < 0)
					g_objPlayer.pos.z = posBlock.z + sizBlock.z + g_objPlayer.siz.z + 1;

				//	よじ登り判定用計算
				if (g_objPlayer.vecFlow.z > 0) {
					dest = D3DXVECTOR3(
						g_objPlayer.pos.x,
						Get_PosBlock(Manager).y + Get_SizBlock(Manager).y + 1,
						posBlock.z - sizBlock.z
					);
				}
				if (g_objPlayer.vecFlow.z < 0) {
					dest = D3DXVECTOR3(
						g_objPlayer.pos.x,
						Get_PosBlock(Manager).y + Get_SizBlock(Manager).y + 1,
						posBlock.z + sizBlock.z
					);
				}
			}
		}
	}

	//	落下時立ち姿勢にする
	if (g_objPlayer.spdFall > 0 && Get_Floating(g_objPlayer.pos, INIT_sizPlayer) &&
		!g_flgTransform && g_objPlayer.mortion == morLay) {
		g_objPlayer.mortion = morStand; TransformPlayer();
	}


}
//	プレイヤーのY軸移動
void Move_PlayerY(void) {
	//	変数宣言
	D3DXVECTOR3 dest = INIT_Vector3D;

	g_objPlayer.pos += D3DXVECTOR3(0.0f, g_objPlayer.spdJump - g_objPlayer.spdFall, 0.0f);

	//	上下埋没判定
	for (int Manager = 0; Manager < NUM_MaxBlock; Manager++) {
		if (Get_flgBlock(Manager) == false)continue;	//	現存していないステージを無視
		if (g_objPlayer.pos.y + g_objPlayer.siz.y > Get_PosBlock(Manager).y - Get_SizBlock(Manager).y &&	//	高さ判定
			g_objPlayer.pos.y - g_objPlayer.siz.y < Get_PosBlock(Manager).y + Get_SizBlock(Manager).y)
		if (g_objPlayer.pos.x + g_objPlayer.siz.x > Get_PosBlock(Manager).x - Get_SizBlock(Manager).x &&	//	x判定
			g_objPlayer.pos.x - g_objPlayer.siz.x < Get_PosBlock(Manager).x + Get_SizBlock(Manager).x &&
			g_objPlayer.pos.z + g_objPlayer.siz.z > Get_PosBlock(Manager).z - Get_SizBlock(Manager).z &&	//	z判定
			g_objPlayer.pos.z - g_objPlayer.siz.z < Get_PosBlock(Manager).z + Get_SizBlock(Manager).z){ 
			g_flgGround = true;
			if (g_objPlayer.spdJump - g_objPlayer.spdFall < 0) {	//	接地判定
				g_objPlayer.pos.y = Get_PosBlock(Manager).y + Get_SizBlock(Manager).y + g_objPlayer.siz.y;
				g_objPlayer.spdJump =		//	ジャンプ力と
					g_objPlayer.spdFall = 0.0f;	//	落下速度を0に設定
				if (g_objPlayer.mortion == morStand && (g_flgSprint || g_objPlayer.flgRun) && g_objPlayer.spdStep > 0) {
					g_objPlayer.mortion = morStagger;
					PlaySound(SOUND_Jump);
				}
			}
			if (g_objPlayer.spdJump > 0) {	//	頭上判定
				g_objPlayer.pos.y = Get_PosBlock(Manager).y - Get_SizBlock(Manager).y - g_objPlayer.siz.y;
			}
	}}

	//	移動可能判定
	//	-	接地判定
	g_objPlayer.pos.y -= 0.1f;	//	0.1f下に移動
	if (Get_Floating(g_objPlayer.pos, g_objPlayer.siz)==false) {	//	接触判定

		Sliding();
		if (g_objPlayer.mortion != morSlide)	//	スライディング時不可
			Stepping();		//	接地移動（歩き/駆け足/疾走）
		g_flgStand = true;
		g_objPlayer.spdJump =		//	ジャンプ力と
			g_objPlayer.spdFall = 0.0f;	//	落下速度を0に設定
		g_objPlayer.vecFlow *= 0.1f;	//	移動速度を減算

		if (g_objPlayer.mortion != morSlide)	//	スライディング時不可
			Leaping();		//	ジャンプ操作

							//	立ち/伏せ切り替え
		if ((GetKeyboardTrigger(DIK_LSHIFT)||GetJoyStickTrigger(4)) && g_objPlayer.mortion != morSlide) {
			dest = g_objPlayer.pos;
			D3DXVECTOR3 sizD = g_objPlayer.siz;
			dest.y += g_objPlayer.siz.y*3.0f + 1;
			sizD.y += g_objPlayer.siz.y*3.0f;

			if (g_objPlayer.mortion == morStand) {
				g_objPlayer.mortion = morLay;
				g_flgTransform = true;
			}
			else if (g_objPlayer.mortion == morLay&&Get_Floating(dest, sizD)) {
				g_objPlayer.mortion = morStand;
				g_flgTransform = true;
			}
		}
	}
	else {//	非接地時
		g_objPlayer.spdFall += 0.5f;	//	落下速度を加算
	}g_objPlayer.pos.y += 0.1f;	//	接地判定用の移動分を戻す

								//	-	頭上判定
	g_objPlayer.pos.y += 0.1f;	//	頭上判定用上昇
	if (Get_Floating(g_objPlayer.pos, g_objPlayer.siz)==false) {	//	接触判定
		g_objPlayer.spdJump = 0.0f;	//	ジャンプ力を0に設定
	}g_objPlayer.pos.y -= 0.1f;

}

//	プレイヤーのサイズ変化
void TransformPlayer(void) {

	if (g_flgTransform) {
		float diff=0.0f;	//	差分

		if (g_objPlayer.mortion == morStand) {	//	伏せ→立ち
			diff = (g_objPlayer.higStand - g_objPlayer.siz.y) / 2.0f;

			g_objPlayer.siz.y += diff;
			g_objPlayer.pos.y += diff;
		}
		if (g_objPlayer.mortion == morLay ||
			(g_objPlayer.mortion == morClimb && numClimb[2]==morLay)) {	//	立ち→伏せ
			diff = (g_objPlayer.siz.y- g_objPlayer.higLay) / 2.0f;

			g_objPlayer.siz.y -= diff;
			g_flgSprint = false;
		}

		if (diff < 1.0f) {
			if (g_objPlayer.mortion == morStand || g_objPlayer.mortion == morStagger) {
				g_objPlayer.siz.y = g_objPlayer.higStand;
			}
			if (g_objPlayer.mortion == morLay || g_objPlayer.mortion == morSlide) {
				g_objPlayer.siz.y = g_objPlayer.higLay;
			}
			g_flgTransform = false;
		}
	}
	else {
		if (g_objPlayer.mortion == morStand || g_objPlayer.mortion == morStagger)
			g_objPlayer.siz.y = g_objPlayer.higStand;
		if (g_objPlayer.mortion == morLay || g_objPlayer.mortion == morSlide)
			g_objPlayer.siz.y = g_objPlayer.higLay;
	}

	g_objPlayer.posEye = g_objPlayer.siz.y-20.0f;
}

void SetPlayer(D3DXVECTOR3 pos,float rot,int mortion) {
	g_objPlayer.pos = pos;
	g_objPlayer.rot.y = rot;
	g_objPlayer.mortion = mortion;

	if(mortion == morStand)
		g_objPlayer.siz = g_sizStand;
	if (mortion == morLay)
		g_objPlayer.siz = g_sizLay;
	g_objPlayer.posEye = g_objPlayer.siz.y - 20;
	
	g_objPlayer.vecFlow = INIT_vecFlow;
	g_objPlayer.spdJump =
	g_objPlayer.spdFall = 
	g_objPlayer.spdStep = 0.0f;

	for (int Manager = 0; Manager<NUM_MaxItem; Manager++)
		g_objPlayer.Item[Manager] = false;	//	所持アイテムをなしに設定

	g_cntStepSound = 0;

	numClimb[0] = -1; numClimb[1] = -1;
	g_cntToSprint = 0;
	g_flgSprint = false;
	g_flgOverSound = false;
	g_numItem = 0;
}

//	アイテムの所持判定
bool Get_flgItem(int nManager) {
	return g_objPlayer.Item[nManager];
}
//	アイテムの所持数を取得
int Get_numItem(void) {
	return g_numItem;
}
bool Get_flgFallout(void) {
	if (g_objPlayer.pos.y < -1500.0f)
		return true;
	return false;
}

D3DXVECTOR2 Get_rotPlayer(void) {
	return g_objPlayer.rot;
}