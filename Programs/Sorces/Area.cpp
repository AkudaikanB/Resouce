//	エリア制御


//	ファイル結合
#include "../Heads/player.h"	//	プレイヤブルキャラクター
#include "../Heads/sound.h"
#include "../Heads/stage.h"		//	ブロック
#include "../Heads/item.h"		//	アイテムオブジェクト
#include "../Heads/goal.h"		//	目的地

//
//	関数定義
//
//	エリア初期化
void InitArea(unsigned int Index) {
	//	変数宣言
	int Manager=0;	//	繰り返し文操作用変数
	int nGoal;	//	生成した目的地の管理番号
	int nStage;	//	消去するドアの管理番号

	//	音を止める
	StopSound();

	//	全オブジェクトの消去
	for (Manager = 0; Manager < NUM_MaxBlock; Manager++)EraseBlock(Manager);
	for (Manager = 0; Manager < NUM_MaxItem; Manager++)	EraseItem(Manager);
	for (Manager = 0; Manager < NUM_MaxGoal; Manager++)	EraseGoal(Manager);

	//	エリア内時間をリセット
	Retry();

	//	エリア生成
	switch (Index) {	//	エリア番号
	case 0:	//	チュートリアルステージ
		//	ブロック
		CreateBlock(D3DXVECTOR3(0.0f,	0.0f,	0.0f),	D3DXToRadian(0), D3DXVECTOR3(200.0f, 7.5f, 450.0f));
		CreateBlock(D3DXVECTOR3(0.0f,	7.5f,	150.0f),D3DXToRadian(0), D3DXVECTOR3(200.0f, 7.5f, 300.0f));
		CreateBlock(D3DXVECTOR3(0.0f,	22.5f,	300.0f),D3DXToRadian(0), D3DXVECTOR3(200.0f, 7.5f, 150.0f));
		CreateBlock(D3DXVECTOR3(0.0f,	350.0f,	0.0f),	D3DXToRadian(0), D3DXVECTOR3(200.0f, 50.0f, 450.0f));	//	天井
		CreateBlock(D3DXVECTOR3(0.0f,	292.5f,	-150.0f),D3DXToRadian(0), D3DXVECTOR3(200.0f, 7.5f, 300.0f));
		CreateBlock(D3DXVECTOR3(0.0f,	277.5f,	-300.0f),D3DXToRadian(0), D3DXVECTOR3(200.0f, 7.5f, 150.0f));
		CreateBlock(D3DXVECTOR3(150.0f,	200.0f,	0.0f),	D3DXToRadian(0), D3DXVECTOR3(0.5f, 300.0f, 450.0f));
		CreateBlock(D3DXVECTOR3(-150.0f,200.0f,	0.0f),	D3DXToRadian(0), D3DXVECTOR3(0.5f, 300.0f, 450.0f));
		CreateBlock(D3DXVECTOR3(0.0f,	200.0f,	450.0f),D3DXToRadian(0), D3DXVECTOR3(200.0f, 300.0f, 0.5f));
		CreateBlock(D3DXVECTOR3(0.0f,	200.0f,-450.0f),D3DXToRadian(0), D3DXVECTOR3(200.0f, 300.0f, 0.5f));
	
		//	目的地とアイテムの生成（ボーナスカウント発生条件の登録）
		nGoal =	CreateGoal(D3DXVECTOR3(0.0f, 150.0f, -400.0f), D3DXToRadian(0), D3DXVECTOR3(150.0f, 150.0f, 50.0f));
		Set_numItemToGoal(nGoal,
			CreateItem(D3DXVECTOR3(0.0f, 150.0f, 300.0f), 10.0f)
		);	//	生成したアイテムの番号でnGoalボーナスカウント発生条件に登録する

		//	BGMを流す
		PlaySound(MUSIC_Area00);

		//	プレイヤーの位置
		SetPlayer(D3DXVECTOR3(0.0f,150.0f,0.0f),D3DXToRadian(180),morStand);
		break;
	case 1:	
		//	足場
		CreateBlock(D3DXVECTOR3(0.0f,	0.0f,	0.0f),	D3DXToRadian(0), D3DXVECTOR3(150.0f, 7.5f, 450.0f));
		CreateBlock(D3DXVECTOR3(150.0f,	300.0f,	0.0f),	D3DXToRadian(0), D3DXVECTOR3(0.5f, 400.0f, 450.0f));
		CreateBlock(D3DXVECTOR3(-150.0f,300.0f,	0.0f),	D3DXToRadian(0), D3DXVECTOR3(0.5f, 400.0f, 450.0f));
		CreateBlock(D3DXVECTOR3(0.0f,	300.0f,	450.0f),D3DXToRadian(0), D3DXVECTOR3(150.0f, 400.0f, 0.5f));
		CreateBlock(D3DXVECTOR3(0.0f,	300.0f,-450.0f),D3DXToRadian(0), D3DXVECTOR3(150.0f, 400.0f, 0.5f));
		CreateBlock(D3DXVECTOR3(0.0f,	450.0f,	0.0f),	D3DXToRadian(0), D3DXVECTOR3(150.0f, 50.0f, 450.0f));	//	天井
		CreateBlock(D3DXVECTOR3(0.0f,	150.0f, 300.0f),D3DXToRadian(0), D3DXVECTOR3(150.0f, 50.0f, 150.0f));

		//	目的地とアイテムの生成（ボーナスカウント発生条件の登録）
		nGoal = CreateGoal(D3DXVECTOR3(0.0f, 150.0f, -400.0f), D3DXToRadian(0), D3DXVECTOR3(150.0f, 150.0f, 50.0f));
		Set_numItemToGoal(nGoal,
			CreateItem(D3DXVECTOR3(0.0f, 20.0f, 300.0f), 10.0f));
		Set_numItemToGoal(nGoal,
			CreateItem(D3DXVECTOR3(0.0f, 320.0f, 300.0f), 10.0f));

		//	BGMを流す
		PlaySound(MUSIC_Area01);

		//	プレイヤーの位置
		SetPlayer(D3DXVECTOR3(0.0f,150.0f,0.0f),0.0f,morStand);
		break;
	case 2:
			//	ブロック 
		CreateBlock(D3DXVECTOR3(0.0f, -100.0f, -100.0f),	D3DXToRadian(0), D3DXVECTOR3(350.0f, 100.0f, 400.0f));
		CreateBlock(D3DXVECTOR3(0.0f, 200.0f, -400.0f),		D3DXToRadian(0), D3DXVECTOR3(100.0f, 200.0f, 100.0f));
		CreateBlock(D3DXVECTOR3(325.0f, 200.0f, -400.0f),	D3DXToRadian(0), D3DXVECTOR3(25.0f, 200.0f, 100.0f));
		CreateBlock(D3DXVECTOR3(-325.0f, 200.0f, -400.0f),	D3DXToRadian(0), D3DXVECTOR3(25.0f, 200.0f, 100.0f));
		CreateBlock(D3DXVECTOR3(0.0f, 500.0f, -400.0f),		D3DXToRadian(0), D3DXVECTOR3(350.0f, 100.0f, 100.0f));
		CreateBlock(D3DXVECTOR3(0.0f, 800.0f, 0.0f),		D3DXToRadian(0), D3DXVECTOR3(350.0f, 50.0f, 500.0f));	//	天井

		CreateBlock(D3DXVECTOR3(-300.0f, 12.5f, 300.0f),	D3DXToRadian(0), D3DXVECTOR3(50.0f, 12.5f, 200.0f));
		CreateBlock(D3DXVECTOR3(-300.0f, 25.0f, 350.0f),	D3DXToRadian(0), D3DXVECTOR3(50.0f, 25.0f, 150.0f));
		CreateBlock(D3DXVECTOR3(-300.0f, 50.0f, 400.0f),	D3DXToRadian(0), D3DXVECTOR3(50.0f, 25.0f, 100.0f));
		CreateBlock(D3DXVECTOR3(-300.0f, 75.0f, 450.0f),	D3DXToRadian(0), D3DXVECTOR3(50.0f, 25.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(-200.0f, 100.0f, 450.0f),	D3DXToRadian(0), D3DXVECTOR3(50.0f, 25.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(-100.0f, 125.0f, 450.0f),	D3DXToRadian(0), D3DXVECTOR3(50.0f, 25.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(0.0f, 150.0f, 450.0f),		D3DXToRadian(0), D3DXVECTOR3(50.0f, 25.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(150.0f, 175.0f, 450.0f),	D3DXToRadian(0), D3DXVECTOR3(100.0f, 25.0f, 50.0f));

		CreateBlock(D3DXVECTOR3(0.0f, 347.5f, 102.5f),		D3DXToRadian(0), D3DXVECTOR3(250.0f, 2.5f, 2.5f));
		CreateBlock(D3DXVECTOR3(0.0f, 250.0f, 102.5f),		D3DXToRadian(0), D3DXVECTOR3(250.0f, 50.0f, 2.5f));
		CreateBlock(D3DXVECTOR3(247.5f, 347.5f, 250.0f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 2.5f, 150.0f));
		CreateBlock(D3DXVECTOR3(247.5f, 250.0f, 250.0f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 50.0f, 150.0f));
		CreateBlock(D3DXVECTOR3(-247.5f, 347.5f, 250.0f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 2.5f, 150.0f));
		CreateBlock(D3DXVECTOR3(-247.5f, 250.0f, 250.0f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 50.0f, 150.0f));
		CreateBlock(D3DXVECTOR3(247.5f, 325.0f, 397.5f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 25.0f, 2.5f));
		CreateBlock(D3DXVECTOR3(247.5f, 325.0f, 300.0f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 25.0f, 2.5f));
		CreateBlock(D3DXVECTOR3(247.5f, 325.0f, 200.0f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 25.0f, 2.5f));
		CreateBlock(D3DXVECTOR3(247.5f, 325.0f, 102.5f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 25.0f, 2.5f));
		CreateBlock(D3DXVECTOR3(150.0f, 325.0f, 102.5f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 25.0f, 2.5f));
		CreateBlock(D3DXVECTOR3(50.0f, 325.0f, 102.5f),		D3DXToRadian(0), D3DXVECTOR3(2.5f, 25.0f, 2.5f));
		CreateBlock(D3DXVECTOR3(-50.0f, 325.0f, 102.5f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 25.0f, 2.5f));
		CreateBlock(D3DXVECTOR3(-150.0f, 325.0f, 102.5f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 25.0f, 2.5f));
		CreateBlock(D3DXVECTOR3(-247.5f, 325.0f, 102.5f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 25.0f, 2.5f));
		CreateBlock(D3DXVECTOR3(-247.5f, 325.0f, 200.0f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 25.0f, 2.5f));
		CreateBlock(D3DXVECTOR3(-247.5f, 325.0f, 300.0f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 25.0f, 2.5f));
		CreateBlock(D3DXVECTOR3(-247.5f, 325.0f, 397.5f),	D3DXToRadian(0), D3DXVECTOR3(2.5f, 25.0f, 2.5f));

		CreateBlock(D3DXVECTOR3(0.0f,-150.0f, 350.0f),		D3DXToRadian(0), D3DXVECTOR3(350.0f, 50.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(0.0f,-50.0f, 350.0f),		D3DXToRadian(0), D3DXVECTOR3(250.0f, 50.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(300.0f,-200.0f, 450.0f),	D3DXToRadian(0), D3DXVECTOR3(50.0f, 50.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(0.0f,-250.0f, 450.0f),		D3DXToRadian(0), D3DXVECTOR3(350.0f, 50.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(0.0f, 100.0f, 250.0f),		D3DXToRadian(0), D3DXVECTOR3(250.0f, 100.0f, 150.0f));

		CreateBlock(D3DXVECTOR3(0.0f,	300.0f, -500.0f),	D3DXToRadian(0), D3DXVECTOR3(350.0f, 500.0f, 0.5f));
		CreateBlock(D3DXVECTOR3(350.0f, 300.0f, 0.0f),		D3DXToRadian(0), D3DXVECTOR3(0.5f, 500.0f, 500.0f));
		CreateBlock(D3DXVECTOR3(-350.0f,300.0f, 0.0f),		D3DXToRadian(0), D3DXVECTOR3(0.5f, 500.0f, 500.0f));
		CreateBlock(D3DXVECTOR3(0.0f,	300.0f, 500.0f),	D3DXToRadian(0), D3DXVECTOR3(350.0f, 500.0f, 0.5f));

		//	目的地とアイテムの生成（ボーナスカウント発生条件の登録）
		nGoal = 
			CreateGoal(D3DXVECTOR3(200.0f, 150.0f, -450.0f), D3DXToRadian(0), D3DXVECTOR3(150.0f, 150.0f, 50.0f));
		Set_numItemToGoal(nGoal,
			CreateItem(D3DXVECTOR3(0.0f, 350.0f, 150.0f), 10.0f));
		Set_numItemToGoal(nGoal,
			CreateItem(D3DXVECTOR3(0.0f, -100.0f, 450.0f), 10.0f));
		nGoal = 
			CreateGoal(D3DXVECTOR3(-200.0f, 150.0f, -450.0f), D3DXToRadian(0), D3DXVECTOR3(150.0f, 150.0f, 50.0f));
		Set_numItemToGoal(nGoal,
			CreateItem(D3DXVECTOR3(300.0f, 100.0f, 450.0f), 10.0f));

			//	BGMを流す
		PlaySound(MUSIC_Area02);

		//	プレイヤーの位置
		SetPlayer(D3DXVECTOR3(0.0f,150.0f,-250.0f),0.0f,morStand);
		break;
		
	case 3:
		//	足場の設置
		CreateBlock(D3DXVECTOR3(0,250.0f,500.0f),	0,	D3DXVECTOR3(650.0f,250.0f,500.0f));
		CreateBlock(D3DXVECTOR3(0,900.0f,250.0f),		0,	D3DXVECTOR3(450.0f,1.0f,250.0f));
		CreateBlock(D3DXVECTOR3(450.0f,	700.0f,250.0f),	0,D3DXVECTOR3(1.0f,200.0f,250.0f));
		CreateBlock(D3DXVECTOR3(-450.0f,700.0f,250.0f),	0,D3DXVECTOR3(1.0f,200.0f,250.0f));
		CreateBlock(D3DXVECTOR3(250.0f,	700.0f,150.0f),	0,D3DXVECTOR3(1.0f,200.0f,150.0f));
		CreateBlock(D3DXVECTOR3(-250.0f,700.0f,150.0f),	0,D3DXVECTOR3(1.0f,200.0f,150.0f));
		CreateBlock(D3DXVECTOR3(0,700.0f,0.0f),		0,	D3DXVECTOR3(450.0f,200.0f,1.0f));
		CreateBlock(D3DXVECTOR3(275,700.0f,500.0f),	0,	D3DXVECTOR3(175.0f,200.0f,1.0f));
		CreateBlock(D3DXVECTOR3(-275,700.0f,500.0f),0,	D3DXVECTOR3(175.0f,200.0f,1.0f));
		CreateBlock(D3DXVECTOR3(150,590.0f,300.0f),	0,	D3DXVECTOR3(100.0f,90.0f,1.0f));
		CreateBlock(D3DXVECTOR3(-150,590.0f,300.0f),0,	D3DXVECTOR3(100.0f,90.0f,1.0f));
		CreateBlock(D3DXVECTOR3(75,690.0f,300.0f),	0,	D3DXVECTOR3(25.0f,10.0f,1.0f));
		CreateBlock(D3DXVECTOR3(-75,690.0f,300.0f),0,	D3DXVECTOR3(25.0f,10.0f,1.0f));
		CreateBlock(D3DXVECTOR3(0,750.0f,300.0f),	0,	D3DXVECTOR3(250.0f,50.0f,1.0f));
		CreateBlock(D3DXVECTOR3(250, 700.0f, 300.0f),	0, D3DXVECTOR3(10.0f, 200.0f, 10.0f));
		CreateBlock(D3DXVECTOR3(-250, 700.0f, 300.0f),	0, D3DXVECTOR3(10.0f, 200.0f, 10.0f));
		CreateBlock(D3DXVECTOR3(0,850.0f,500.0f),	0,	D3DXVECTOR3(100.0f,50.0f,1.0f));

		//	階段
		CreateBlock(D3DXVECTOR3(200, 520.0f, 550.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(250, 540.0f, 550.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(300, 560.0f, 550.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(350, 580.0f, 550.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(400, 600.0f, 550.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(450, 620.0f, 550.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(500, 640.0f, 550.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(500, 660.0f, 450.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(500, 680.0f, 400.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(500, 700.0f, 350.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(500, 720.0f, 300.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(500, 740.0f, 250.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(500, 760.0f, 200.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(500, 780.0f, 150.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(500, 800.0f, 100.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(500, 820.0f, 50.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(500, 840.0f, 0.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(500, 860.0f, -50.0f), 0, D3DXVECTOR3(50.0f, 10.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(0, 880.0f, -50.0f), 0, D3DXVECTOR3(450.0f, 10.0f, 50.0f));

		CreateBlock(D3DXVECTOR3(-250, 900.0f, 750.0f), 0, D3DXVECTOR3(50.0f, 1.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(-250, 900.0f, 1150.0f), 0, D3DXVECTOR3(50.0f, 1.0f, 50.0f));
		CreateBlock(D3DXVECTOR3(200, 550.0f, 50.0f), 0, D3DXVECTOR3(50.0f, 1.0f, 50.0f));

		CreateBlock(D3DXVECTOR3(-150, 350.0f, 1500.0f), 0, D3DXVECTOR3(250.0f, 350.0f, 150.0f));

		//	ゴール,アイテム,ドアの設置
		nGoal =						//	ゴール
			CreateGoal(D3DXVECTOR3(0.0f, -1000.0f, 500.0f), 0.0f, D3DXVECTOR3(10000.0f, 1.0f, 10000.0f));
		Set_numItemToGoal(nGoal,	//	アイテム
			CreateItem(D3DXVECTOR3(0, 750, 1500), 10));
		Set_numItemToGoal(nGoal,	//	アイテム
			CreateItem(D3DXVECTOR3(400, 910, 400), 10));

		nGoal =						//	ゴール
			CreateGoal(D3DXVECTOR3(350.0f, 650.0f, 50.0f), 0.0f, D3DXVECTOR3(100.0f, 150.0f, 50.0f));
		Set_numItemToGoal(nGoal,	//	アイテム
			CreateItem(D3DXVECTOR3(-350,550,50),50));
		Set_numEraseByGoal(			//	ドア
			CreateBlock(D3DXVECTOR3(0, 600.0f, 300.0f), 0, D3DXVECTOR3(50.0f, 100.0f, 1.0f)),nGoal);

		nGoal =						//	ゴール
			CreateGoal(D3DXVECTOR3(-200.0f, 650.0f, 50.0f), 0.0f, D3DXVECTOR3(50.0f, 150.0f, 100.0f));
		Set_numItemToGoal(nGoal,	//	アイテム
			CreateItem(D3DXVECTOR3(200,530,20),10));
		Set_numEraseByGoal(			//	ドア
			CreateBlock(D3DXVECTOR3(0, 650.0f, 500.0f), 0, D3DXVECTOR3(100.0f, 150.0f, 1.0f)),nGoal);

		CreateItem(D3DXVECTOR3(500, 600, 50), 10);
		CreateItem(D3DXVECTOR3(-500, 600, 50), 10);

		//	プレイヤーの設置
		SetPlayer(D3DXVECTOR3(350.0f, 500.0f+Get_sizPlayer().y, 400.0f), D3DXToRadian(-90), morStand);

		//	音楽の再生
		PlaySound(MUSIC_Area03);
		break;

	case 4:
		CreateBlock(D3DXVECTOR3(0, 0, 0), 0, D3DXVECTOR3(700.0f, 1.0f, 700.0f));
		//	ゴールの設置
		CreateGoal(D3DXVECTOR3(-1.0f, 2000.0f, 1000.0f), 0.0f, D3DXVECTOR3(0.5f, 1.0f, 1.0f));

		//	プレイヤーの設置
		SetPlayer(D3DXVECTOR3(100.0f, 200.0f, 400.0f), 0.0f, morStand);

		//	音楽の再生
		PlaySound(MUSIC_Area04);
		break;

	case 25:	//	サバイバルエリア
		//	ブロック
		CreateBlock(D3DXVECTOR3(0.0f, 0.0f, 0.0f),		D3DXToRadian(0), D3DXVECTOR3(5000.0f, 1.0f, 5000.0f));
		CreateBlock(D3DXVECTOR3(5000.0f, 500.0f, 0.0f), D3DXToRadian(0), D3DXVECTOR3(1.0f, 1000.0f, 5000.0f));
		CreateBlock(D3DXVECTOR3(-5000.0f,500.0f, 0.0f), D3DXToRadian(0), D3DXVECTOR3(1.0f, 1000.0f, 5000.0f));
		CreateBlock(D3DXVECTOR3(0.0f,500.0f, 5000.0f ), D3DXToRadian(0), D3DXVECTOR3(5000.0f, 1000.0f, 0.0f));
		CreateBlock(D3DXVECTOR3(0.0f,500.0f, -5000.0f), D3DXToRadian(0), D3DXVECTOR3(5000.0f, 1000.0f, 0.0f));
		CreateBlock(D3DXVECTOR3(0.0f, 300.0f, 0.0f),	D3DXToRadian(0), D3DXVECTOR3(100.0f, 1.0f, 200.0f));
		CreateBlock(D3DXVECTOR3(-200.0f, 30.0f, 0.0f),	D3DXToRadian(0), D3DXVECTOR3(100.0f, 30.0f, 200.0f));
		CreateBlock(D3DXVECTOR3(0.0f, 10.0f, 300.0f),	D3DXToRadian(0), D3DXVECTOR3(100.0f, 10.0f, 100.0f));
		CreateBlock(D3DXVECTOR3(-200.0f, 30.0f, 300.0f), D3DXToRadian(0), D3DXVECTOR3(100.0f, 10.0f, 100.0f));

		//	目的地とアイテムの生成（ボーナスカウント発生条件の登録）
		nGoal = CreateGoal(D3DXVECTOR3(-200.0f, 150.0f, -300.0f), D3DXToRadian(0), D3DXVECTOR3(200.0f, 200.0f, 100.0f));
		Set_numItemToGoal(nGoal,
			CreateItem(D3DXVECTOR3(-200.0f, 150.0f, 0.0f), 10.0f));

		//	BGMを流す
		PlaySound(MUSIC_Survival);

		//	プレイヤーの位置
		SetPlayer(INIT_posPlayer, 0.0f, morStand);
		break;
	default:	//	存在しないエリア情報
		SetPlayer(INIT_posPlayer, 0.0f, morStand);
		break;
	}
}
