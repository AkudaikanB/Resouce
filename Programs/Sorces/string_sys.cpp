//	������(UI)�n�i�Ɣ�G���A�Q�[����ʁj


//	�t�@�C������
#include "../Heads/string_sys.h"
#include "../Heads/sound.h"
#include "../Heads/player.h"
#include "../Heads/item.h"
#include "../Heads/stage.h"

//	�O���[�o���ϐ��錾
LPD3DXFONT	g_pFont_Clause,	//	����UI
			g_pFont_Index,	//	���o��
			g_pFont_Common;	//	�ėp
bool flgMouseOver = false;

//	�v���g�^�C�v�錾
void DrawTitle			(LPDIRECT3DDEVICE9 pDevice);
void DrawResult			(LPDIRECT3DDEVICE9 pDevice);
void DrawSet			(LPDIRECT3DDEVICE9 pDevice);
void DrawCoolTime		(LPDIRECT3DDEVICE9 pDevice);
void DrawEmergencyMenu	(LPDIRECT3DDEVICE9 pDevice);
void DrawTime			(LPDIRECT3DDEVICE9 pDevice,DWORD tim,RECT rec,D3DXCOLOR col);


//
//	�֐���`
//
//	������
void InitString(LPDIRECT3DDEVICE9 pDevice) {
	//	DirectX�Ńt�H���g���쐬
	//	-	�ėp�t�H���g
	D3DXCreateFont(
		pDevice,	//	�C���^�[�t�F�C�X�ւ̃|�C���^
		18, 8,		//	�����̍���,���i�����ɂ���ď���ɐݒ肳��邽��0�ŗǂ��j
		0, 0,		//	�����ݒ�,�~�b�v�}�b�v
		FALSE,		//	�Α̐ݒ�
		SHIFTJIS_CHARSET,	//	�����Z�b�g
		OUT_DEFAULT_PRECIS,	//	�t�H���g�T�C�Y�̈�v���@
		DEFAULT_QUALITY,	//	���ۂ̃t�H���g�ւ̈�v���@
		DEFAULT_PITCH,		//	�s�b�`
		"MS UI Gothic",		//	����
		&g_pFont_Common		//	��������t�H���g���ւ̃|�C���^
	);
	//	-	�����t�H���g
	D3DXCreateFont(
		pDevice, 18, 8, 700, 0, FALSE,		//	������700
		SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH,
		"�l�r �o����", &g_pFont_Index
	);
	//	-	�^�C�g�����j���[�t�H���g
	D3DXCreateFont(
		pDevice, 24, 12, 700, 0, FALSE,
		SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH,
		"MS UI Gothic", &g_pFont_Clause
	);
}

//	�t�H���g�Ɏg�p���������������
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
	int nPhase = Get_numPhase(),	//	�Q�[���̏�Ԃ��擾
		nState = Get_numState();	//	�Q�[����ł̏�Ԃ��擾

	//	�^�C�g����ʕ`��
	if (nPhase == IDX_phaTitle)DrawTitle(pDevice);

	//	���ʉ�ʕ`��
	if(nPhase == IDX_phaResult)DrawResult(pDevice);

	//	�G���A���ł̕`��
	if (nPhase == IDX_phaGaming) {
		if (nState == IDX_steSet)	DrawSet(pDevice);				//	������Ԃ̕`��
		if (nState == IDX_steCool)	DrawCoolTime(pDevice);			//	�I�����̕`��
		if (nState == IDX_steEmergency)DrawEmergencyMenu(pDevice);	//	�ً}���j���[�̕`��

		//	������`�悷���`
		RECT rec = { 480,450,SIZE_ScreenWidth,SIZE_ScreenHeight };
		RECT recItem{ 0,450,100,SIZE_ScreenHeight };

		char strItem[8];
		wsprintf(strItem,"�� = %02d",Get_numItem());

		//	���Ԃ̕`��
		if (nState != IDX_steCool) {
			DrawTime(pDevice, Get_timOfArea(), rec, COL_String[IDX_colCommon]);		rec.top -= 30;
			g_pFont_Clause->DrawText(NULL, strItem, -1, &recItem, DT_LEFT, COL_String[IDX_colIndex]);
		}
		for (int Manager = 0; Manager<NUM_MaxGoal; Manager++) {if (Get_flgGoalCount(Manager)) {
			DrawTime(pDevice, Get_timOfGoal(Manager), rec, COL_INDEX[Manager]);	rec.top -= 30;
		}}
	}
}


//	�^�C�g�����j���[�̕`��
void DrawTitle(LPDIRECT3DDEVICE9 pDevice) {
	//	�J�[�\���̎擾
	int sel = Get_selMenu();

	//	������`��p�̕ϐ��錾
	RECT recItem00	={2,282,SIZE_ScreenWidth,316},
		recItem01	={2,322,SIZE_ScreenWidth,356},
		recItem02	={2,362,SIZE_ScreenWidth,396},
		recItem10	={0,280,SIZE_ScreenWidth,315},
		recItem11	={0,320,SIZE_ScreenWidth,355},
		recItem12	={0,360,SIZE_ScreenWidth,395};

	char strItem00[24]	="�`�������W���[�h",
		strItem01[24]	="�T�o�C�o�����[�h",
		strItem02[8]	="�I���";

	//	�^�C�g�����j���[1�̕`��
	g_pFont_Clause->DrawText(
		NULL,		//	��������܂񂾃X�v���C�g�ւ̃|�C���^�i�Ȃ�����NULL�j
		strItem00,	//	�`�悷�镶����
		-1,			//	NULL���I�[�����Ƃ��������������v�Z
		&recItem00,	//	�`�悷���`
		DT_CENTER,	//	���������������낦
		COL_String[IDX_colShadow]	//	�`�悷��F
	);
	//	�^�C�g�����j���[2�̕`��
	g_pFont_Clause->DrawText(NULL, strItem01, -1, &recItem01, DT_CENTER, COL_String[IDX_colShadow]);
	//	�^�C�g�����j���[3�̕`��
	g_pFont_Clause->DrawText(NULL, strItem02, -1, &recItem02, DT_CENTER, COL_String[IDX_colShadow]);

	//	�^�C�g�����j���[�̃|�b�v�A�b�v�̕`��
	D3DXVECTOR2 posMouse = Get_posMouse();
	if (sel!=-1||(posMouse.x > 160 && posMouse.x < 480)) {	//	�������J�[�\���ł̌��o
		if(sel==0||(posMouse.y>280&&posMouse.y<315)){g_pFont_Clause->DrawText(NULL,strItem00,-1,&recItem10,DT_CENTER,COL_String[IDX_colCommon]);if(flgMouseOver==false)PlaySound(SOUND_Jump);flgMouseOver=true;}else
		if(sel==1||(posMouse.y>320&&posMouse.y<355)){g_pFont_Clause->DrawText(NULL,strItem01,-1,&recItem11,DT_CENTER,COL_String[IDX_colCommon]);if(flgMouseOver==false)PlaySound(SOUND_Jump);flgMouseOver=true;}else
		if(sel==2||(posMouse.y>360&&posMouse.y<395)){g_pFont_Clause->DrawText(NULL,strItem02,-1,&recItem12,DT_CENTER,COL_String[IDX_colCommon]);if(flgMouseOver==false)PlaySound(SOUND_Jump);flgMouseOver=true;}
		else flgMouseOver = false;
	}
}
//	�J�n�O�ҋ@��Ԃ̕\��
void DrawSet(LPDIRECT3DDEVICE9 pDevice) {
	//	�`�悷���`�𐶐�
	RECT recGuide = {0,POS_ScreenCenter_Y-30,SIZE_ScreenWidth,POS_ScreenCenter_Y+30},
		recArea = {0,POS_ScreenCenter_Y-100,SIZE_ScreenWidth,POS_ScreenCenter_Y-60};
	char strGuide[64] = "�Z�{�^����\n�N���b�N�������ăX�^�[�g�I",	//	�K�C�h
		strArea[24];	//	�G���A�\��

	wsprintf(strArea, "�G���A%02d", Get_numInArea() + 1);
	if (Get_flgSurvival())
		wsprintf(strArea, "�T�o�C�o�����[�h");

	//	��Ԋm�F��`��
	bool flgReady = Get_flgReady();
	
	if (flgReady)g_pFont_Clause->DrawText(NULL,strGuide,-1,&recGuide,DT_CENTER,COL_String[IDX_colPositive]);
	if(!flgReady)g_pFont_Clause->DrawText(NULL,strGuide,-1,&recGuide,DT_CENTER,COL_String[IDX_colCommon]);
	g_pFont_Clause->DrawText(NULL,strArea,-1,&recArea,DT_CENTER,COL_String[IDX_colIndex]);
}
//	�I�����ҋ@��Ԃ̕\��
void DrawCoolTime(LPDIRECT3DDEVICE9 pDevice) {
	//	�J�[�\���̎擾
	int sel = Get_selMenu();
	//	������`��p�̋�`
	RECT recMenu[3] = {
		{POS_ScreenCenter_X-180,POS_ScreenCenter_Y+20,POS_ScreenCenter_X-60,	POS_ScreenCenter_Y+38},
		{POS_ScreenCenter_X-60,	POS_ScreenCenter_Y+20,POS_ScreenCenter_X+60,	POS_ScreenCenter_Y+38},
		{POS_ScreenCenter_X+60,	POS_ScreenCenter_Y+20,POS_ScreenCenter_X+180,	POS_ScreenCenter_Y+38}
	},
		recArea = {220,200,320,220},
		recTime = {260,195,500,220},	//	�G���A������
		recScore= {260,225,500,250};	//	�G���A���X�R�A

	//	������̐���
	char strMenu[3][12] = {"���g���C", "�I���", "���̃G���A"};	//	���j���[
	char strScore[16];	//	�X�R�A
	char strArea[16];	//	�G���A�\�L
	
	//	�}�E�X�̈ʒu���擾
	D3DXVECTOR2 posMouse = Get_posMouse();

	//	���j���[�w�i�p�̋�`
	VERTEX_2D vtxMenu[2][4];
	Create_recPolygon(vtxMenu[0],RECT{120,185,520,290},D3DCOLOR_RGBA(0, 0, 64, 255));
	Create_recPolygon(vtxMenu[1],RECT{117,182,523,293},D3DCOLOR_RGBA(0, 128, 0, 255));

	//	���j���[�{�^���p�̋�`(�|�b�v�A�b�v)
	VERTEX_2D vtxItem[3][4];
	Create_recPolygon(vtxItem[0],recMenu[0],D3DCOLOR_RGBA(255,255,255,64));
	Create_recPolygon(vtxItem[1],recMenu[1],D3DCOLOR_RGBA(255,255,255,64));
	Create_recPolygon(vtxItem[2],recMenu[2],D3DCOLOR_RGBA(255,255,255,64));

	//	�`��
	pDevice->SetTexture(0, NULL);	//	�w�i�̃e�N�X�`���͖���

	pDevice->DrawPrimitiveUP(		//	�w�i1�̕`��
		D3DPT_TRIANGLESTRIP,
		NUM_Polygon2D,
		vtxMenu[1],
		sizeof(VERTEX_2D)
	);
	pDevice->DrawPrimitiveUP(		//	�w�i2�̕`��
		D3DPT_TRIANGLESTRIP,
		NUM_Polygon2D,
		vtxMenu[0],
		sizeof(VERTEX_2D)
	);

	g_pFont_Index->DrawText(NULL,strMenu[0],-1,&recMenu[0],DT_CENTER,COL_String[IDX_colShadow]);
	g_pFont_Index->DrawText(NULL,strMenu[1],-1,&recMenu[1],DT_CENTER,COL_String[IDX_colShadow]);
	g_pFont_Index->DrawText(NULL,strMenu[2],-1,&recMenu[2],DT_CENTER,COL_String[IDX_colShadow]);

	//	�G���A�\�L
	wsprintf(strArea,"�G���A%02d :",Get_numInArea()+1);
	if(Get_flgSurvival())
		wsprintf(strArea, "�T�o�C�o�� :");
	g_pFont_Common->DrawText(NULL,strArea,-1,&recArea,DT_LEFT,COL_String[IDX_colCommon]);
	//	�o�ߎ���
	if (Get_flgSurvival())
		DrawTime(pDevice,Get_timAll(),recTime,COL_String[IDX_colCommon]);
	else
		DrawTime(pDevice,Get_timOfArea(),recTime,COL_String[IDX_colCommon]);
	//	�X�R�A
	wsprintf(strScore,"�X�R�A : %d",Get_numScore());
	g_pFont_Index->DrawText(NULL,strScore,-1,&recScore,DT_CENTER,COL_String[IDX_colCommon]);

	//	���j���[�̃|�b�v�A�b�v�i�}�E�X�̈ʒu�ɉ�����j
	if(sel!=-1||	//	�J�[�\���̔���
		(posMouse.y>260 && posMouse.y<285)){	//	�c�ł̌��o
		if ((posMouse.x > recMenu[0].left&&posMouse.x < recMenu[0].right)||sel==0) {
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);	//	���u�����h��L���ɂ���
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,NUM_Polygon2D,vtxItem[0],sizeof(VERTEX_2D));
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);	//	���u�����h�𖳌��ɂ���
			g_pFont_Index->DrawText(NULL,strMenu[0],-1,&recMenu[0],DT_CENTER,COL_String[IDX_colCommon]);
			if (flgMouseOver == false)PlaySound(SOUND_Over); flgMouseOver = true;
		}else
		if ((posMouse.x > recMenu[1].left&&posMouse.x < recMenu[1].right)||sel==1) {
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);	//	���u�����h��L���ɂ���
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,NUM_Polygon2D,vtxItem[1],sizeof(VERTEX_2D));
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);	//	���u�����h�𖳌��ɂ���
			g_pFont_Index->DrawText(NULL,strMenu[1],-1,&recMenu[1],DT_CENTER,COL_String[IDX_colCommon]);
			if (flgMouseOver == false)PlaySound(SOUND_Over); flgMouseOver = true;
		}else
		if ((posMouse.x > recMenu[2].left&&posMouse.x < recMenu[2].right)||sel==2) {
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);	//	���u�����h��L���ɂ���
			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,NUM_Polygon2D,vtxItem[2],sizeof(VERTEX_2D));
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);	//	���u�����h�𖳌��ɂ���
			if(Get_numInArea()<NUM_LastArea&&Get_flgGoNext())
				g_pFont_Index->DrawText(NULL,strMenu[2],-1,&recMenu[2],DT_CENTER,COL_String[IDX_colPositive]);
			else 
				g_pFont_Index->DrawText(NULL,strMenu[2],-1,&recMenu[2],DT_CENTER,COL_String[IDX_colDisable]);
			if (flgMouseOver == false)PlaySound(SOUND_Over); flgMouseOver = true;
		}
		else flgMouseOver = false;
	}
}
//	���ʉ�ʂ̕`��
void DrawResult(LPDIRECT3DDEVICE9 pDevice) {
	//	������`��p�̋�`�𐶐�
	RECT recCaption= {	0,50,SIZE_ScreenWidth,80 }, 
		recSentence = {	0,360,SIZE_ScreenWidth,420 },
		recTime [2]= {{120,170,SIZE_ScreenWidth-120,200},{122,172,SIZE_ScreenWidth-118,202}},
		recTimeB[2]= {{120,200,SIZE_ScreenWidth-120,230},{122,202,SIZE_ScreenWidth-118,232}},
		recScore[2]= {{120,250,SIZE_ScreenWidth-120,280},{122,252,SIZE_ScreenWidth-118,282}},
		recScoreB[2]={{120,280,SIZE_ScreenWidth-120,310},{122,282,SIZE_ScreenWidth-118,312}};

	//	�����񐶐�
	char strSentence[64] = "�Z�{�^����������\n��ʃN���b�N�Ń^�C�g���ɖ߂�܂��B";	//	�K�C�h
	char strScore[8], strScoreB[8],	//	�X�R�A
		rblTime[12],rblScore[12],	//	���x��
		strCaption[36];				//	�L���v�V����
		wsprintf(strScore,"%7d",Get_numScoreA());
		wsprintf(rblTime, "�^�C�� : ");
		wsprintf(rblScore,"�X�R�A : ");

	if (Get_flgSurvival()){
		wsprintf(strCaption,"�T�o�C�o�����[�h : ����");
	}
	else{
		wsprintf(strCaption, "�`�������W���[�h : �G���A%02d�`%02d", Get_numArea0()+1, Get_numInArea()+1);
		if (Get_numArea0() == Get_numInArea()) 
			wsprintf(strCaption, "�`�������W���[�h : �G���A%02d", Get_numInArea()+1);
		if (Get_numArea0() == 0 && Get_numInArea() == NUM_LastArea)
			wsprintf(strCaption, "�`�������W���[�h : �R���v���[�g");
	}
	
	//	�e��`��
	DrawTime(pDevice,Get_timAll(),recTime[1],COL_String[IDX_colShadow]);
	g_pFont_Clause->DrawText(NULL,strScore,-1,&recScore[1],DT_CENTER,COL_String[IDX_colShadow]);
	//	�K�C�h��`��
	g_pFont_Common->DrawText(NULL,strSentence,-1,&recSentence,DT_CENTER,COL_String[IDX_colCommon]);
	g_pFont_Clause->DrawText(NULL,rblTime,-1,&recTime[1],	DT_LEFT,COL_String[IDX_colCommon]);
	g_pFont_Clause->DrawText(NULL,rblScore,-1,&recScore[1],	DT_LEFT,COL_String[IDX_colCommon]);
	g_pFont_Clause->DrawText(NULL,strCaption,-1,&recCaption,DT_CENTER,COL_String[IDX_colCommon]);
	//	���ʂ�`��
	DrawTime(pDevice,Get_timAll(),recTime[0],COL_String[IDX_colCommon]);
	g_pFont_Clause->DrawText(NULL,strScore,-1,&recScore[0],DT_CENTER,COL_String[IDX_colCommon]);
}
//	�ً}���j���[�̕`��
void DrawEmergencyMenu(LPDIRECT3DDEVICE9 pDevice) {
	//	�J�[�\���̎擾
	int sel = Get_selMenu();
	//	������`��p�̋�`
	RECT recCaption = {	120,175,520,195 },
		recBackGame = { 120,210,520,230 },
		recRetry	= {	120,235,520,255 },
		recBackTitle= { 120,260,520,280 },
		recQuit		= {	120,285,520,305 };
	//	�`�悷�镶����
	char strCaption[32],
		strBackGame	[16] = "�Q�[���ɂ��ǂ�",
		strRetry	[16] = "���g���C",
		strBackTitle[16] = "�^�C�g����ʂ�",
		strQuit		[16] = "�Q�[�����I���";
	D3DXVECTOR2 posMouse = Get_posMouse();	//	�}�E�X�̈ʒu���擾
	//	���j���[�̔w�i���쐬�i�ݒ�j
	VERTEX_2D vtxMenu[2][4];
	Create_recPolygon(vtxMenu[0],RECT{120,165,520,315},D3DCOLOR_RGBA(128,0,0,255));
	Create_recPolygon(vtxMenu[1],RECT{117,162,523,318},D3DCOLOR_RGBA(255,255,0,255));

	//	�����񐶐�
	wsprintf(strCaption,"�|�[�Y�� : �G���A%02d",Get_numInArea()+1);
	if(Get_flgSurvival())
		wsprintf(strCaption, "�|�[�Y�� : �T�o�C�o��");

	//	�`��
	pDevice->SetTexture(0,NULL);	//	�w�i�̃e�N�X�`���͖���

	pDevice->DrawPrimitiveUP(		//	�w�i1�̕`��
		D3DPT_TRIANGLESTRIP,
		NUM_Polygon2D,
		vtxMenu[1],
		sizeof(VERTEX_2D)
	);
	pDevice->DrawPrimitiveUP(		//	�w�i2�̕`��
		D3DPT_TRIANGLESTRIP,
		NUM_Polygon2D,
		vtxMenu[0],
		sizeof(VERTEX_2D)
	);
	//	���j���[�̉e��`��
	g_pFont_Index->DrawText(NULL,strBackGame,-1,&recBackGame,DT_CENTER,COL_String[IDX_colShadow]);
	g_pFont_Index->DrawText(NULL,strRetry,-1,	&recRetry,DT_CENTER,COL_String[IDX_colShadow]);
	g_pFont_Index->DrawText(NULL,strBackTitle,-1,&recBackTitle,DT_CENTER,COL_String[IDX_colShadow]);
	g_pFont_Index->DrawText(NULL,strQuit,-1,	&recQuit,DT_CENTER,COL_String[IDX_colShadow]);

	//	���͂̕`��
	g_pFont_Common->DrawText(NULL, strCaption, -1, &recCaption, DT_CENTER, COL_String[IDX_colCommon]);
	//	���j���[�̃|�b�v�A�b�v
	if (sel != -1 ||
		((posMouse.x > 120 && posMouse.x < 520))) {	//	�c�ʒu�ł̌��o
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

//	���Ԃ�`��
void DrawTime(LPDIRECT3DDEVICE9 pDevice, DWORD tim, RECT rec,D3DXCOLOR col) {
	//	�ϐ��錾
	char strTime[12];		//	�`�悷�鐔����
	int hou,min, sec, dot;	//	�`�悷�鐔��

	//�����̌v�Z
	hou = (tim / 3600000)%24;	//	����
	min = (tim / 60000)%60;		//	��
	sec = (tim / 1000)%60;		//	�b
	dot = (tim / 10)%100;		//	0�_��

	//	�`�悷�鐔��
		wsprintf(strTime, "%02d'%02d.%02d", min, sec, dot);
	if(tim /1000 > 3600)
		wsprintf(strTime, "%02d:%02d'%02d.%02d", hou, min, sec, dot);

	//	�`��
	g_pFont_Clause->DrawText(NULL,strTime,-1,&rec,DT_CENTER,col);
}
