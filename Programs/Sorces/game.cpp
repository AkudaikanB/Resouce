//	�Q�[���̃��C������

//	�t�@�C������
#include "../Heads/game.h"
#include "../Heads/input.h"		//	���͌n
#include "../Heads/sound.h"		//	��
#include "../Heads/light.h"		//	��
#include "../Heads/string_sys.h"//	������(UI)
#include "../Heads/camera.h"	//	�J����
#include "../Heads/Area.h"		//	�G���A
#include "../Heads/player.h"	//	�v���C���u���L�����N�^�[
#include "../Heads/stage.h"		//	�u���b�N
#include "../Heads/item.h"		//	�A�C�e���I�u�W�F�N�g
#include "../Heads/goal.h"		//	�ړI�n

//	�O���[�o���ϐ�
int g_numPhase = IDX_phaTitle;	//	�Q�[�����̏�ԕϐ��i�^�C�g��,�G���A��,���ʉ�ʁj
int g_numState = IDX_steSet;	//	�G���A���̏�ԕϐ��i����,�G���A��,�ً}���j���[,�I�����j

int g_numArea0= 0;	//	�J�n�G���A�̔ԍ�
int g_numArea = 0;	//	���݂̃G���A�̔ԍ�
int g_cntAwait = 0;	//	�ҋ@����

int g_scoAll,		//	�S�G���A�̍��v�X�R�A
	g_scoOnArea;	//	���݃G���A�ł̃X�R�A

bool g_flgStart = false;
bool g_flgSelect = false;
bool g_flgSurvival = 0;	//	���[�h�؂�ւ��i0:,1:�T�o�C�o���j
bool flgReady = false;	//	�Q�[���J�n�O�����m�F�p�ϐ�

DWORD g_timUpdate;	//	���ԍX�V����
DWORD g_cntAll;		//	�J�n�G���A����̍��v����
DWORD g_cntOfArea;	//	�G���A���o�ߎ���
DWORD g_cntOfGoal[NUM_MaxGoal];		//	�S�[���̔�������
DWORD g_cntAddGoal;	//	�S�[���ǉ�����

int g_redBack = COL_redBack,	//	�����p�w�i�F
	g_greBack = COL_greBack,
	g_bluBack = COL_bluBack,
	g_alpFade = 255;	//	�t�F�[�h�̕s�����x
bool g_flgFade = 0;		//	�t�F�[�h�A�E�g:0/�C��:1�̔���ϐ�
int	g_secFading = 0;	//	�t�F�[�h���ԁi�b�P�ʁj

int g_selMenu = -1;

bool g_flgGoalSUV[NUM_MaxGoal];	//	�T�o�C�o�����[�h�p�̌Œ�ʒu�g�p�t���O
bool g_flgItemSUV[NUM_MaxItem];	//
bool g_flgPointer;
bool g_flgClear = false;			//	�G���A�i�s��


//	�v���g�^�C�v�錾
void Operate_onTitle(void);		//	�^�C�g����ʂ̑���
void Operate_onResult(void);	//	���ʉ�ʂ̑���
void Ruling(void);				//	�Q�[������
void Operate_onSet(void);		//	�Q�[���J�n�O�ҋ@���
void Operate_onCoolTime(void);	//	�Q�[���I�����ҋ@���
void Operate_onEmergency(void);	//	�ً}���j���[��ł̑���

//
//	�֐���`
//
//	�e�I�u�W�F�N�g�̋N�����ݒ�֐����Ăяo��
void InitGame(LPDIRECT3DDEVICE9 pDevice) {
	//	�O���[�o���ϐ�������
	g_numPhase = IDX_phaTitle,
		g_numState = IDX_steSet,
		g_numArea = 0;
	g_cntAwait = 0;
	g_cntAll = g_cntOfArea = 0;
	for (int Manager = 0; Manager < NUM_MaxGoal; Manager++) g_cntOfGoal[Manager] = 0;
	g_scoAll = g_scoOnArea = 0;
	g_flgSurvival = 0;
	g_redBack = COL_redBack,	//�����p�w�i�F
		g_greBack = COL_greBack,
		g_bluBack = COL_bluBack,
	g_alpFade = 255;	//	�t�F�[�h�s�����x
		g_flgFade	= 1;//	�t�F�[�h�A�E�g:0/�C��:1����p�ϐ�
		g_secFading = 1;//	�t�F�[�h����
	flgReady = false;

	ShowCursor(false);		//	�f�t�H���g�}�E�X�J�[�\���̔�\��
	g_flgPointer = true;	//	�}�E�X�J�[�\���ɍ��킹���|�C���^�[�̕\��

	ShowCursor(true);		//	�f�t�H���g�}�E�X�J�[�\���̔�\��

	//	�\�[�X�ʏ�����
	InitDrawState(pDevice);	//	�`��p�̋��ʐ��l��ݒ�
	InitLight(pDevice);		//	���̋N�����ݒ�
	InitCamera(pDevice);	//	�J�����̋N�����ݒ�
	InitString(pDevice);	//	������n�̋N�����ݒ�

	InitPlayer(pDevice);	//	�v���C���u���L�����N�^�[�̋N�����ݒ�
	InitStage(pDevice);		//	�u���b�N�̋N�����ݒ�
	InitItems(pDevice);		//	�A�C�e���I�u�W�F�N�g�̋N�����ݒ�
	InitGoal(pDevice);		//	�ړI�n�̋N�����ݒ�

	InitArea(0);	//	�G���A���`���[�g���A���X�e�[�W�ɐݒ�

	StopSound();
	PlaySound(MUSIC_Title);
}

//	�e�I�u�W�F�N�g�̍ŏ������֐����Ăяo��
void UninitGame(void) {
	UninitInput();	//	���͌n�Ɏg�p�����������̉��
	UninitSound();
	UninitString();	//	������n�Ɏg�p�����������̊J��

	UninitPlayer();	//	�v���C���u���L�����N�^�[�Ɏg�p�����������̉��
	UninitItems();	//	�A�C�e���I�u�W�F�N�g�Ɏg�p�����������̉��
	UninitStage();	//	�u���b�N�Ɏg�p�����������̉��
	UninitGoal();	//	�ړI�n�Ɏg�p�����������̉��
}

//	�e�I�u�W�F�N�g�̍X�V�֐����Ăяo��
void UpdateGame(LPDIRECT3DDEVICE9 pDevice) {
	UpdateInput();	//	���͌n�̍X�V

	if (g_cntAwait > 0)g_cntAwait--;	//	�ҋ@�J�E���g������
	else {					//	�ҋ@�J�E���g���Ȃ���
		switch (g_numPhase) {
		case IDX_phaGaming:
			if (g_numState == IDX_steGaming) {
				UpdatePlayer();	//	�v���C���u���L�����N�^�̍X�V
				UpdateStage();	//	�u���b�N�̍X�V
				UpdateItems();	//	�A�C�e���I�u�W�F�N�g�̍X�V
				UpdateGoals();	//	�ړI�n�̍X�V
				Ruling();	//	�Q�[������
			}
			if (g_numState == IDX_steSet)Operate_onSet();
			if (g_numState == IDX_steCool)Operate_onCoolTime();
			if (g_numState == IDX_steEmergency) Operate_onEmergency();
			break;
		case IDX_phaTitle:
			Operate_onTitle();	//	�^�C�g����ʂ̑���
			break;
		case IDX_phaResult:
			Operate_onResult();	//	���ʉ�ʂ̑���
			break;
		}
	}
}

//	�e�I�u�W�F�N�g�̕`��֐����Ăяo��
void DrawGame(LPDIRECT3DDEVICE9 pDevice) {
	pDevice->Clear(	//	�w�i���N���A
		0,
		NULL,
		D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
		D3DCOLOR_RGBA(g_redBack,g_greBack,g_bluBack,255),			//	�w�i�F��ݒ�
		1.0f,
		0
	);

	pDevice->BeginScene();	//	�`��o�^�J�n
	{
		//	3D�`��
		pDevice->SetFVF(FVF_VERTEX_3D);	//	3D�̒��_�t�H�[�}�b�g��ݒ�
		switch (g_numPhase) {
		case IDX_phaGaming:
			if (GetKeyboardTrigger(DIK_RETURN))
				ChangeLight(pDevice);

			UpdateCamera(pDevice);	//	�J�������̍X�V
			UpdateLight(pDevice);	//	�����X�V

			DrawStage(pDevice);		//	�u���b�N�̕`��
			DrawGoals(pDevice);		//	�ړI�n�̕`��
			DrawItems(pDevice);		//	�A�C�e���I�u�W�F�N�g�̕`��
			break;
		}

		//	UI
		pDevice->SetFVF(FVF_VERTEX_2D);	//	2D�̒��_�t�H�[�}�b�g��ݒ�
		DrawString(pDevice);	//	������iUI�j�̕`��
	}
	pDevice->EndScene();	//	�`��o�^�I��
	pDevice->Present(NULL,NULL,NULL,NULL);	//	�`�悷��
}

//	�^�C�g����ʏ�ł̑���
void Operate_onTitle(void) {
	D3DXVECTOR2 posMouse = Get_posMouse();	//	�}�E�X�̈ʒu���擾
	
	if (GetJoyStickTrigger(2)||
		(GetMouseLeftTrigger()&& posMouse.x > 160 && posMouse.x < 480)) {	//	���N���b�N��
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
	
	//	�J�[�\���̔���
	if (g_selMenu==-1) {
		if (GetJoyStickLeftTrigger(0) || GetJoyStickLeftTrigger(1) ||
			GetJoyStickLeftTrigger(2) || GetJoyStickLeftTrigger(3)) {
			g_selMenu = 0;
		}
	}
	else {	//	�J�[�\���ړ�
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
//	���ʉ�ʏ�ł̑���
void Operate_onResult(void) {
	D3DXVECTOR2 posMouse = Get_posMouse();	//	�}�E�X�̈ʒu���擾

	if (GetMouseLeftTrigger()||GetJoyStickTrigger(2)) {	//	��ʃN���b�N
		g_cntAll = g_scoAll = 0;	//	�S�G���A���v����,���v�X�R�A�����Z�b�g
		g_numPhase = IDX_phaTitle; SetMouseMode(false);	//	�^�C�g����ʂɐ؂�ւ�

		StopSound();
		PlaySound(MUSIC_Title);
		PlaySound(SOUND_cngPhase);
	}
}
//	�Q�[������
void Ruling(void) {
	//	�I������
	//	-	�S�[���̐���0�ɂȂ�ƃG���A�N���A�Ƃ���
	if (Get_Clear()) { 
		g_flgClear=true;
		ShowCursor(true);		//	�f�t�H���g�}�E�X�J�[�\���̕\��
		SetMouseMode(false); g_numState = IDX_steCool;

		if (GetJoyStickAnyControl())IsolateMouse();	//	�}�E�X�ޔ�
	}
	if (Get_flgFallout()) {
		ShowCursor(true);
		SetMouseMode(false); g_numState = IDX_steCool;
		PlaySound(SOUND_Starting);

		if (GetJoyStickAnyControl())IsolateMouse();	//	�}�E�X�ޔ�
	}

	//	���[�� - �T�o�C�o�����[�h
	if (g_flgSurvival) {
		//	�I������	-	��������
		if((long)g_cntOfArea <= 0){
			ShowCursor(true);		//	�f�t�H���g�}�E�X�J�[�\���̕\��
			SetMouseMode(false); g_numState = IDX_steCool;

			if (GetJoyStickAnyControl())IsolateMouse();	//	�}�E�X�ޔ�
		}

		//	�S�[���ǉ�
		g_cntAddGoal += timeGetTime() - g_timUpdate;	//	�J�E���g
		if (g_cntAddGoal / 1000 > 1) {	//��b���ƃS�[���ǉ�
			g_cntAddGoal = 0;	//	�J�E���g���Z�b�g

			srand((unsigned int)timeGetTime());	//	�����\�����ݎ����ŏ�����
			int ManagerG=0,	//	���[�v������p�ϐ�
				ManagerI=0,	//	���[�v������p�ϐ�
				ranGoal=rand(),	//	�����_�������p�ϐ�
				ranItem=rand(),	//	�����_�������p�ϐ�
				ranFor=rand();	//	�ꕔ���[�v�������_���񐔉�

			for (ManagerG = 0; ManagerG < NUM_MaxGoal; ManagerG++) {
				ranGoal = (unsigned int)rand()%(NUM_MaxGoal);
				if (g_flgGoalSUV[ranGoal] == false) {
					break;
			}}

			int nGoal;

			if(ranGoal<NUM_MaxGoal && g_flgGoalSUV[ranGoal]==false){
				nGoal = CreateGoal(POS_SurvivalGoals[ranGoal],0,Siz_SurvivalGoals[ranGoal]);
				if (Get_numGoal() < NUM_MaxGoal && g_flgGoalSUV[ranGoal]==false) {

					g_flgGoalSUV[ranGoal] = true;	//	�S�[���̈ʒu�g�p�t���O�𗧂Ă�

					ranFor = (unsigned)rand() % 3+1;	//	�S�[���ɕK�v�ȃA�C�e�����������_���Ŏ擾
					for (int Manager = 0; Manager < ranFor; Manager++) {	//	�S�[���ɕK�v�ȃA�C�e���𐶐�����for
						for (ManagerI = 0; ManagerI < NUM_MaxItem; ManagerI++) {
							ranItem = (unsigned)rand()%NUM_MaxItem;	//	�����ʒu�̎g�p���Ŕ���
							if (g_flgItemSUV[ranItem] == false) {	//	�g�p����Ă��Ȃ��ԍ��̎�������
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

	//	���Ԃ̃J�E���g�i�X�V�j
	if(g_flgSurvival==0)
		g_cntOfArea += timeGetTime() - g_timUpdate;	//	�G���A������
	else
		g_cntOfArea -= timeGetTime() - g_timUpdate;	//	�G���A������

	g_cntAll += timeGetTime() - g_timUpdate;	//	�G���A�S�̂ł̌o�ߎ���
	for(int Manager=0;Manager<NUM_MaxGoal;Manager++)if(Get_flgGoalCount(Manager))
		g_cntOfGoal[Manager] += timeGetTime() - g_timUpdate;	//	�A�C�e���̏�������
	g_timUpdate = timeGetTime();	//	�X�V�����̐ݒ�

	//	�ً}���j���[�̕\��
	if (GetKeyboardTrigger(DIK_ESCAPE) || GetJoyStickTrigger(9)) {
		ShowCursor(true);		//	�f�t�H���g�}�E�X�J�[�\���̕\��
		SetMouseMode(false); g_numState = IDX_steEmergency; 

		if (GetJoyStickAnyControl())IsolateMouse();	//	�}�E�X�ޔ�
	}
}
//	�J�n�O�ҋ@��Ԃł̑���
void Operate_onSet(void) {
	if (GetMouseLeftTrigger()||GetJoyStickTrigger(2)) {
		flgReady = true;	//	�}�E�X��������
		PlaySound(SOUND_GetItem);
	}
	if ((GetMouseLeftRelease()||GetJoyStickRelease(2))&&flgReady == true) {	//	�Q�[���X�^�[�g����
		SetMouseMode(true);			//	�}�E�X�Œ�on
		g_numState = IDX_steGaming;	//	�Q�[���X�^�[�g
		flgReady = false;			//	��������̉���
		g_timUpdate = timeGetTime();//	�X�V�����̍X�V

		ShowCursor(false);		//	�f�t�H���g�}�E�X�J�[�\���̔�\��

		PlaySound(SOUND_Starting);
	}
}
//	�I�����ҋ@��Ԃł̑���
void Operate_onCoolTime(void) {
	D3DXVECTOR2 posMouse = Get_posMouse();	//	�}�E�X�̈ʒu���擾

	//	�N�[���^�C�����j���[�̔���/����
	if (GetJoyStickPress(2)||
		posMouse.y > 260 && posMouse.y < 280 && GetMouseLeftTrigger()) {	//	�c�ʒu�ƍ��N���b�N�Ŕ���
		//	���ʒu�ł̔���

		if(posMouse.x>140&&posMouse.x<260 ||
			g_selMenu==0){	//	���g���C
			InitArea(g_numArea);	//	�G���A������
			PlaySound(SOUND_cngState);
		}
		if(posMouse.x>260&&posMouse.x<380||
			g_selMenu==1){	//	���ʉ�ʂɈړ�
			g_scoAll += g_scoOnArea;	//	�G���A���̓��_�����Z
			g_numState = IDX_steSet;	//	�G���A������
			g_numPhase = IDX_phaResult;	g_cntAwait = 3;		//	���ʉ�ʂɈړ�

			StopSound();
			PlaySound(MUSIC_Result);
			PlaySound(SOUND_cngPhase);
		}
		if(posMouse.x>380&&posMouse.x<500||
			g_selMenu==2){	//	���̃G���A�Ɉړ�
		if (g_numArea < NUM_LastArea && g_flgSurvival==0 && g_flgClear) {	//	�ŏI�G���A�łȂ����ǂ����̔���
			g_scoAll += g_scoOnArea;	//	�G���A���̓��_�����Z
			g_numArea++;	InitArea(g_numArea);	//	�G���A�ړ�,�G���A������
			g_numState = IDX_steSet; g_cntAwait = 3;//	�J�n�O�ҋ@��Ԃɐ؂�ւ�
			g_flgClear = false;
			PlaySound(SOUND_cngState);
		}}
	}
	//	�J�[�\���̔���
	if (g_selMenu==-1) {
		if (GetJoyStickLeftTrigger(0) || GetJoyStickLeftTrigger(1) ||
			GetJoyStickLeftTrigger(2) || GetJoyStickLeftTrigger(3)) {
			g_selMenu = 2;
		}
	}
	else {	//	�J�[�\���ړ�
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
//	�ً}���j���[
void Operate_onEmergency(void){
	D3DXVECTOR2 posMouse = Get_posMouse();	//	�}�E�X�̈ʒu���擾

	if (GetJoyStickPress(2)||
		(GetMouseLeftTrigger() && posMouse.x > 120 && posMouse.x < 520)) {	//	���N���b�N�Ɖ�ʏ�x���W�ł̌��o
		//	y���W�Ń��j���[�̌��o
		if (g_selMenu == 0||
			(posMouse.y > 210 && posMouse.y < 230)) {	//	�Q�[���ɂ��ǂ�
			//	�J�n�O�ҋ@��Ԃɐ؂�ւ�
			g_numState = IDX_steSet; g_cntAwait = 3;
		}
		if (g_selMenu == 1 ||
			(posMouse.y > 235 && posMouse.y < 255)) {	//	���g���C
			InitArea(g_numArea);
			PlaySound(SOUND_cngState);
		}
		if (g_selMenu == 2 ||
			(posMouse.y > 260 && posMouse.y < 280)) {	//	�^�C�g����ʂ�
			//	�G���A�Q�[���̂�������
			g_cntAll = g_scoAll = 0;
			Retry();
			//	�^�C�g���ɐ؂�ւ�
			g_numPhase = IDX_phaTitle;

			StopSound();
			PlaySound(MUSIC_Title);
			PlaySound(SOUND_cngPhase);
		}
		if (g_selMenu == 3 ||
			(posMouse.y > 285 && posMouse.y < 305)) {	//	�Q�[�����I���
			QuitGame();
		}
	}
	
	//	�J�[�\���̔���
	if (g_selMenu==-1) {
		if (GetJoyStickLeftTrigger(0) || GetJoyStickLeftTrigger(1) ||
			GetJoyStickLeftTrigger(2) || GetJoyStickLeftTrigger(3)) {
			g_selMenu = 0;
		}
	}
	else {	//	�J�[�\���ړ�
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

//	�`��p�̋��ʒl��ݒ�
void InitDrawState(LPDIRECT3DDEVICE9 pDevice) {
	//	�����_�[�X�e�[�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);			//	�|���S���̕`���\�ʂ݂̂ɂ���
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		//	�`�挳�̃���ݒ肷��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);	//	�`���̃���ݒ肷��

	//	�T���v���[�X�e�[�g�̐ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);//	�e�N�X�`����U�l�����[�v�ɐݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);//	�e�N�X�`����V�l�����[�v�ɐݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	//	��Ԑݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	//	��Ԑݒ�

	//	�e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	pDevice->SetTextureStageState(0,
		D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0,
		D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0,
		D3DTSS_ALPHAARG2, D3DTA_CURRENT);

}
//	2D�`��p���_���̋��ʒl��ݒ�
void InitVertex2D(VERTEX_2D*pVtx){
	ZeroMemory(pVtx, sizeof(VERTEX_2D));	//	0�N���A�i�s��l�ǂݎ��h�~�j

	pVtx->col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);	//	���_�E�����S�s�����̔��ɐݒ�

	pVtx->rhw = 1.0f;	//	rhw��ݒ�

	pVtx->tex = D3DXVECTOR2(0.0f, 0.0f);	//	�e�N�X�`�����W��ݒ�
}
//	4�p�`2D�|���S���̏��𐶐�
void Create_recPolygon(VERTEX_2D*pVtx0,RECT rec,D3DXCOLOR col) {
	//	4���_��������
	InitVertex2D(&pVtx0[0]);
	InitVertex2D(&pVtx0[1]);
	InitVertex2D(&pVtx0[2]);
	InitVertex2D(&pVtx0[3]);

	//	4���_�ɍ��킹���e�N�X�`�����W��ݒ�
	pVtx0[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx0[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx0[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx0[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//	4���_�̈ʒu��ݒ�
	pVtx0[0].pos = D3DXVECTOR3((float)rec.left, (float)rec.top, 0.0f);
	pVtx0[1].pos = D3DXVECTOR3((float)rec.right,(float)rec.top, 0.0f);
	pVtx0[2].pos = D3DXVECTOR3((float)rec.left, (float)rec.bottom, 0.0f);
	pVtx0[3].pos = D3DXVECTOR3((float)rec.right,(float)rec.bottom, 0.0f);

	//	�|���S���̐F��ݒ�
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
//	�u���b�N�Ƃ̐ڐG����
bool Get_Floating(D3DXVECTOR3 pos,D3DXVECTOR3 siz){
	//	�ڐG����u���b�N�𔻒�i�{�b�N�X�R���C�_�[�j
	for (int Manager = 0; Manager < NUM_MaxBlock; Manager++) {
		if (Get_flgBlock(Manager) == false)continue;	//	�������Ă��Ȃ��X�e�[�W�𖳎�
		if (pos.y + siz.y > Get_PosBlock(Manager).y - Get_SizBlock(Manager).y &&	//	��������
			pos.y - siz.y < Get_PosBlock(Manager).y + Get_SizBlock(Manager).y)
		if (pos.x + siz.x > Get_PosBlock(Manager).x - Get_SizBlock(Manager).x &&	//	x����
			pos.x - siz.x < Get_PosBlock(Manager).x + Get_SizBlock(Manager).x &&
			pos.z + siz.z > Get_PosBlock(Manager).z - Get_SizBlock(Manager).z &&	//	z����
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
//	�L�[�{�[�h/�W���C�X�e�B�b�N�ɂ�郁�j���[����ɗp����J�[�\�����擾
int Get_selMenu(void) {	
	return g_selMenu;
}

//	pos��dest��1/10�ړ��A������Ԃ�
float Move_10to1(D3DXVECTOR3*pos, D3DXVECTOR3 dest) {
	D3DXVECTOR3 vec = (dest-*pos)/10.0f;	//	2���W�Ԃ�1/2����

	*pos += vec;	//	�������l�߂�

	return D3DXVec3Length(&vec);
}

void Retry(void) {
	//	�G���A������,�S�[���������Ԃ����Z�b�g
	if (g_flgSurvival) {
		g_cntOfArea = 30000;
		g_cntAll = 0;
	}
	else
		g_cntOfArea = 0;
	for(int Manager=0;Manager<NUM_MaxGoal;Manager++)g_cntOfGoal[Manager]=0;
	g_cntAddGoal = 0;

	//	�A�C�e�������������Z�b�g

	//	�G���A���X�R�A�����Z�b�g
	g_scoOnArea = 0;
	
	//	�J�n�O�ҋ@��Ԃɐ؂�ւ�
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