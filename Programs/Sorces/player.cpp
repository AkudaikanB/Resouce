//	�v���C���u���L�����N�^�[�̐���

//	�t�@�C������
#include "../Heads/input.h"
#include "../Heads/sound.h"
#include "../Heads/player.h"
#include "../Heads/stage.h"
#include "../Heads/goal.h"
#include "../Heads/item.h"

//	�O���[�o���ϐ��錾
Character g_objPlayer;	//	�v���C���u���L�����N�^�[�̐���ϐ�
int g_numItem;			//	�A�C�e���̏�����
int g_cntToSprint;		//	��������p�J�E���g
D3DXVECTOR3 g_posDestClimb;
DWORD g_cntStepSound;	//	���ݍ��݉�����p�J�E���^
bool g_flgSprint;		//	��������
bool g_flgOverSound;	//	��щz��������p�J�E���^
bool g_flgStand = false;	//	���V������
bool g_flgClimb = false;	//	�i���̏㏸����
bool g_flgGround = false;	//	���n
int numClimb[4];	//	�悶�o���i�Ǘ��ԍ�,����(0:z+, 1:x+, 2:z-, 3:x-),
					//				���̑̐�,���̑̐�(3:�ʂ�z��)�j
bool g_flgTransform = false;	//	����/�����؂�ւ�������

D3DXVECTOR3 g_sizStand	= INIT_sizPlayer;	//	������Ԏ��̑傫��
D3DXVECTOR3 g_sizLay	= INIT_sizPlayer;	//	������Ԏ��̑傫��

//	�v���g�^�C�v�錾
void RotationView(void);	//	�����_�ړ��i�J�����Ƃ͕ʁj
void Stepping(void);		//	�ڒn�ړ��i����/�삯��/����j
void Leaping(void);			//	��ђ��˂�
void Sliding(void);			//	�X���C�f�B���O
void Climbing(void);		//	�悶�o��
void Action(void);			//	�A�N�V����(��ԌŒ�)����

void Move_PlayerXZ(void);	//	�v���C���[��X/Z���ړ�
void Move_PlayerY(void);	//	�v���C���[��Y���ړ�
void TransformPlayer(void);	//	�v���C���[�̑傫���ύX

//
//	�֐��錾
//
//	������
void InitPlayer(LPDIRECT3DDEVICE9 pDevice) {
	ZeroMemory(&g_objPlayer,sizeof(Character));	//	�������ׂ�0�ɂ���i�s��l�ǂݎ��h�~�j

	g_objPlayer.pos = INIT_posPlayer;	//	�v���C���u���L�����N�^�[�̏����ʒu
	g_objPlayer.vec = INIT_vecPlayer;	//	�v���C���u���L�����N�^�[�̌����Ă������
	g_objPlayer.siz = INIT_sizPlayer;	//	�v���C���u���L�����N�^�[�̑傫��(x�𔼌a�Ƃ�������y�̉~��)
	g_objPlayer.rot = INIT_rotPlayer;	//	�v���C���u���L�����N�^�[�̉�]�x�̐ݒ�
	g_objPlayer.posEye = INIT_talEye;	//	�v���C���u���L�����N�^�[�̎��_�̍���
	g_objPlayer.higStand = INIT_sizPlayer.y;	//	�v���C���u���L�����N�^�[�̗������g��(1/2�T�C�Y)
	g_objPlayer.higLay = INIT_sizPlayer.y/4.0f;	//	����������(�g����1/8)
	g_objPlayer.vecFlow = INIT_vecFlow;	//	�v���C���u���L�����N�^�[�̔�ڒn�ړ��x�N�g��
	g_objPlayer.flgRun = false;			//	�v���C���u���L�����N�^�[�̑���X�C�b�`
	g_objPlayer.spdStep = 0.0f;			//	�v���C���u���L�����N�^�[�̈ړ����x
	g_objPlayer.spdJump =				//	�W�����v�͂�
	g_objPlayer.spdFall = 0.0f;			//	�������x��0�ɐݒ�

	g_objPlayer.rot.x -= D3DXToRadian(-30);

	for(int Manager=0;Manager<NUM_MaxItem;Manager++)
		g_objPlayer.Item[Manager] = false;	//	�����A�C�e�����Ȃ��ɐݒ�

	numClimb[0] = -1; numClimb[1] = -1;
	g_cntToSprint = 0;
	g_flgSprint = false;
	g_posDestClimb = INIT_Vector3D;
}
//	�ŏ�����
void UninitPlayer(void) {
}

//	�X�V
void UpdatePlayer(void) {
	//	�ϐ����Z�b�g
	g_flgStand	= false;	//	���V
	g_flgClimb	= false;	//	�i��
	g_flgGround	= false;	//	���n

	//	�v���C���[�̃T�C�Y�ύX
	TransformPlayer();

	//	�펞�\����
	//	-	����/����؂�ւ�
	if (GetKeyboardTrigger(DIK_F)||GetJoyStickTrigger(6)) {
		g_objPlayer.flgRun = !g_objPlayer.flgRun;
	}

	//	���(�A�N�V����)����
	//	-	�悶�o����
	if (g_objPlayer.mortion == morClimb) Climbing();
	//	-	�ʏ���
	else{
		Move_PlayerY();	//	y���ړ�
		Move_PlayerXZ();//	x/z���ʈړ�

		Action();	//	�A�N�V��������
	}

	RotationView();	//	�����_�ړ�

	//	�A�C�e���̎擾����
	int nItem = Get_TouchedItem(g_objPlayer.pos, g_objPlayer.siz);	//	�A�C�e���Ƃ̐ڐG����
	if (nItem != -1){
		g_numItem++;		//	�����A�C�e�����𑝉�
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
		IncreaseScore(10);	//	�A�C�e���擾�{�[�i�X�X�R�A���_
		EraseItem(nItem);	//	�擾�����A�C�e��������
	}

	//	�ړI�n�ƑΉ��A�C�e����������
	int nGoal = Get_TouchedGoal(g_objPlayer.pos, g_objPlayer.siz);	//	�S�[���Ƃ̐ڐG����
	if (nGoal != -1 && Get_toGoal(nGoal)) {	//	�S�[������
		int nDecrease = Get_numItemOfGoal(nGoal);		//	�S�[���K�v�A�C�e�������擾
		DWORD nBonus = 100 - Get_timOfGoal(nGoal)/1000;	//	�^�C���{�[�i�X�v�Z
		if (nBonus < 0 || Get_flgGoalCount(nGoal)==false)nBonus = 0;	//	�{�[�i�X�X�R�A�����̎�0�Ƃ���
		
		if (Get_flgSurvival()) {
			IncreaseCount(nDecrease*100);
			IncreaseCount((int)nBonus);
		}

		if (Get_toGoal(nGoal)) {
			ChangeColorGoal(nGoal, 1);
		}

		PlaySound(SOUND_GoalClear);

		EraseBlockByGoal(nGoal);
		IncreaseScore(nDecrease*100);	//	�S�[���B���X�R�A���_
		IncreaseScore(nBonus);			//	�^�C���{�[�i�X�X�R�A���_
		g_numItem -= nDecrease;	//	�S�[�����A�C�e������
		EraseGoal(nGoal);		//	�ړI�n����
	}


}

//	�v���C���u���L�����N�^�[�̈ʒu���擾
D3DXVECTOR3 Get_posEye(void) { 
	D3DXVECTOR3 posEye = g_objPlayer.pos;	//	�v���C���u���L�����N�^�[�̒��S�ʒu���擾
		posEye.y += g_objPlayer.posEye;		//	�ʒu����ڂ̈ʒu��ݒ�
	return posEye;
}
D3DXVECTOR3 Get_vecPlayer(void) { return g_objPlayer.vec; }	//	�v���C���u���L�����N�^�[�̌������擾
D3DXVECTOR3 Get_sizPlayer(void) { return g_objPlayer.siz; }	//	�v���C���u���L�����N�^�[�̑傫�����擾

//	�����_�ړ�
void RotationView(void){
	//	�ϐ��錾
	D3DXVECTOR3 posEye = g_objPlayer.pos;	//	���_�̈ʒu
	posEye.y += g_objPlayer.posEye;			//	�v���C���u���L�����N�^�[�̒��S�ʒu����ڂ̈ʒu�ւ��炷

	long inX = GetMouseAxisY();	//	�c������]����

	if (IsJoyStick(0)) {
		if (GetJoyStickPositionZ() > 40000) g_objPlayer.rot.y += 0.03125f;
		if (GetJoyStickPositionZ() < 25000) g_objPlayer.rot.y -= 0.03125f;
		if (GetJoyStickPositionRZ() > 40000 && g_objPlayer.rot.x < D3DXToRadian(+89)) g_objPlayer.rot.x += 0.015625f;
		if (GetJoyStickPositionRZ() < 25000 && g_objPlayer.rot.x > D3DXToRadian(-89)) g_objPlayer.rot.x -= 0.015625f;
	}

	g_objPlayer.rot.y += 0.001f* GetMouseAxisX();		//	����]
	if ((inX < 0 && g_objPlayer.rot.x > D3DXToRadian(-89)) ||	//	�c��]��
		(inX > 0 && g_objPlayer.rot.x < D3DXToRadian(+89)))		//	��]����
		g_objPlayer.rot.x += 0.001f* GetMouseAxisY();	//	�c��]

	//	����]�̐�������
	if (g_objPlayer.rot.y < 0)g_objPlayer.rot.y = D3DXToRadian(360)+g_objPlayer.rot.y;
	if (g_objPlayer.rot.y > D3DXToRadian(360))g_objPlayer.rot.y = g_objPlayer.rot.y-D3DXToRadian(360);

	//	�c��]�����i��]������߂��j
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

//	�ڒn�ړ��i����/�삯��/�����j
void Stepping(void) {
	//	�ϐ��錾
	D3DXVECTOR3 posEye =			//	���_�̈ʒu
		g_objPlayer.pos;			//	�v���C���u���L�����N�^�[�̒��S�ʒu
	posEye.y += g_objPlayer.posEye;	//	�v���C���u���L�����N�^�[�̒��S�ʒu����ڂ̈ʒu�ւ��炷

	//	�ړ��������擾
	D3DXVECTOR3 vecMove = g_objPlayer.vec - posEye;
	vecMove.y = 0.0f;

	//	��������
	if (GetKeyboardTrigger(DIK_W)){	//	�ړ��L�[�̉�������
		if (g_cntToSprint == 0 && g_objPlayer.mortion == morStand)
			g_cntToSprint++;
		else if(g_cntToSprint<15&&g_cntToSprint>1)
			g_flgSprint = true;
	}
	if (GetJoyStickPress(6))	g_flgSprint = true;
	if (GetJoyStickRelease(6))	g_flgSprint = false;
	if((g_objPlayer.flgRun ||g_flgSprint)&&g_objPlayer.mortion!=morLay){
		if (GetKeyboardRelease(DIK_W)) {	//	�ړ��L�[�̉������
			g_objPlayer.mortion = morStagger;	//	��낯
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

	//	�ړ����x��ݒ�
	if (GetKeyboardPress(DIK_W) ||	//	�ړ��L�[/�{�^���̓��͔���
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
		g_objPlayer.spdStep += 2.0f;	//	����
		g_cntStepSound++;
	}
	else {
		g_objPlayer.spdStep -= 1.0f;
		if(g_objPlayer.spdStep<10)g_objPlayer.spdStep=0;	//	-�ȉ��ɂ��Ȃ�
	}
	//	-	�ړ����x����
	if (g_flgSprint){
		if (g_objPlayer.spdStep > 70.0f) {
			g_objPlayer.spdStep = 70.0f;	//	����
		}
	}
	else{
	if (g_objPlayer.flgRun==true	&& g_objPlayer.spdStep>50.0f)
		g_objPlayer.spdStep = 50.0f;	//	�삯��
	if ((g_objPlayer.flgRun==false||g_objPlayer.mortion==morLay)&& g_objPlayer.spdStep>20.0f && g_objPlayer.mortion!=morSlide)
		g_objPlayer.spdStep = 20.0f;	//	����
	}

	//	�ړ��x�N�g����P�ʃx�N�g����
	D3DXVec3Normalize(&vecMove, &vecMove);

	//	�ړ�
	if (GetKeyboardPress(DIK_W)||(IsJoyStick(0)&&GetJoyStickPositionY()<25000)) { 	//	�O���ړ�
		g_objPlayer.vecFlow += vecMove*g_objPlayer.spdStep; 
	}
	if (GetKeyboardPress(DIK_S)||(IsJoyStick(0)&&GetJoyStickPositionY()>40000)) {	//	����ړ�
		g_objPlayer.vecFlow -= vecMove*g_objPlayer.spdStep; 
	}
	if (GetKeyboardPress(DIK_A)||(IsJoyStick(0)&&GetJoyStickPositionX()<25000)) {	//	�����ֈړ�
		D3DXVECTOR3 vec2;
		vec2.x = -vecMove.z;
		vec2.z = vecMove.x;
		vec2.y = 0.0f;
		g_objPlayer.vecFlow += vec2*g_objPlayer.spdStep;
	}
	if (GetKeyboardPress(DIK_D)||(IsJoyStick(0)&&GetJoyStickPositionX()>40000)) {	//	�E���ֈړ�
		D3DXVECTOR3 vec2;
		vec2.x = vecMove.z;
		vec2.z = -vecMove.x;
		vec2.y = 0.0f;
		g_objPlayer.vecFlow += vec2*g_objPlayer.spdStep;
	}

	//	��낯�ړ�
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
//	��ԌŒ�A�N�V����(�X���C�f�B���O,�悶�o�蓙)�ւ̑���
void Action(void) {
	//	�ϐ��錾
	int Manager=0;	//	�J��Ԃ�������p
	bool flgBreak[NUM_MaxBlock];
	D3DXVECTOR3 dest = g_objPlayer.pos;	//	����p�ʒu
	D3DXVECTOR3 vec = g_objPlayer.vec-g_objPlayer.pos;	//	����ړ��p����
	float		rot = g_objPlayer.rot.y;//	��]�p����

	vec.y = 0.0f;					//	���ׂẴA�N�V������x/z���ʂŔ�����s��
	D3DXVec3Normalize(&vec,&vec);	//	�P�ʃx�N�g����
	
	for(Manager=0;Manager<NUM_MaxBlock;Manager++)flgBreak[Manager]=false;

	//	�悶�o��
	for (Manager = 0; Manager < 100; Manager++) {	//	1���[�g������
		dest += vec;	//	1cm�ړ�

		for(int ManagerB=0;ManagerB<NUM_MaxBlock;ManagerB++){	//	�u���b�N�Ƃ̐ڐG����
			if (Get_flgBlock(ManagerB) == false || flgBreak[ManagerB])continue;	//	�������Ă��Ȃ��X�e�[�W�𖳎�
			if (dest.y + g_objPlayer.siz.y*1.5f > Get_PosBlock(ManagerB).y + Get_SizBlock(ManagerB).y &&	//	��������
				dest.y - g_objPlayer.siz.y/2.0f < Get_PosBlock(ManagerB).y + Get_SizBlock(ManagerB).y)
			if (dest.x + g_objPlayer.siz.x > Get_PosBlock(ManagerB).x - Get_SizBlock(ManagerB).x &&	//	x����
				dest.x - g_objPlayer.siz.x < Get_PosBlock(ManagerB).x + Get_SizBlock(ManagerB).x &&
				dest.z + g_objPlayer.siz.z > Get_PosBlock(ManagerB).z - Get_SizBlock(ManagerB).z &&	//	z����
				dest.z - g_objPlayer.siz.z < Get_PosBlock(ManagerB).z + Get_SizBlock(ManagerB).z){ 
				//	�ϐ��錾
				D3DXVECTOR3 posBlock = Get_PosBlock(ManagerB),	//	�u���b�N�̈ʒu
							sizBlock = Get_SizBlock(ManagerB);	//	

				//	�ڐG����̐���
				//	-	x�������̐���
				if ((rot > D3DXToRadian(315) || rot < D3DXToRadian(45)) ||	//	��]�����ł̔���
					(rot > D3DXToRadian(135) && rot < D3DXToRadian(225))){
					if (vec.x>0 && dest.x + g_objPlayer.siz.x > posBlock.x - sizBlock.x){
						dest.x-=1.0f;
						vec.x = 0;	//	x�������ړ����Ȃ���
					}else
					if (vec.x<0 && dest.x - g_objPlayer.siz.x < posBlock.x + sizBlock.x){
						dest.x+=1.0f;
						vec.x = 0;	//	x�������ړ����Ȃ���
					}
				}
				//	-	z�������̐���
				if((	rot > D3DXToRadian(45) && rot < D3DXToRadian(135)) ||	//	��]�����ł̔���
						rot > D3DXToRadian(225) && rot < D3DXToRadian(315)){
					if (vec.z>0 && dest.z + g_objPlayer.siz.z > posBlock.z - sizBlock.z){
						dest.z-=1.0f;
						vec.z = 0;	//	z�������ړ����Ȃ���
					}else
					if (vec.z<0 && dest.z - g_objPlayer.siz.z < posBlock.z + sizBlock.z){
						dest.z+=1.0f;
						vec.z = 0;	//	z�������ړ����Ȃ���
					}
				}

				//	�悶�o��A�N�V��������i����,�ړ��\,���[�V�����̔���j
				if((GetKeyboardPress(DIK_SPACE) && GetKeyboardPress(DIK_W))||GetJoyStickPress(5)){
					if(Get_Floating(
					D3DXVECTOR3(dest.x,posBlock.y+sizBlock.y+g_objPlayer.higLay,dest.z),
					D3DXVECTOR3(g_objPlayer.siz.x, g_objPlayer.higLay,g_objPlayer.siz.z))&&
					g_objPlayer.mortion!=morClimb){

					//	-	�v���C���[�̉�]�p�x�ł̔���
					if ((rot >= D3DXToRadian(315) || rot < D3DXToRadian(45)) && 
						g_objPlayer.pos.z<posBlock.z-sizBlock.z) {

						//	+z����
						PlaySound(SOUND_Climb);
						numClimb[0] = ManagerB;	//	�悶�o����ݒ�
						numClimb[1] = 2;		//	�o�����
						g_posDestClimb = D3DXVECTOR3(	//	�o��ʒu
							dest.x,
							posBlock.y + sizBlock.y + g_objPlayer.higStand,
							posBlock.z - sizBlock.z);
						//	�o���� ����/���� �̐ݒ�
						if (Get_Floating(g_posDestClimb,g_sizStand)==false)	//	��ڐG����
							numClimb[2]=morLay;	//	����

						//	���p��
						//	-	�u���b�N����̍����Ŕ���
						if (g_objPlayer.pos.y <= posBlock.y + sizBlock.y) {
							numClimb[3] = 0;
							g_posDestClimb.y =posBlock.y + sizBlock.y;
							g_posDestClimb.z =posBlock.z - sizBlock.z;
						}
						else {
							numClimb[3] = 1;
						}

						g_objPlayer.mortion = morClimb;	//	�悶�o��ɏ�Ԃ�؂�ւ�
					}else
					if (rot >= D3DXToRadian(45)&&rot < D3DXToRadian(135) &&
						g_objPlayer.pos.x<posBlock.x-sizBlock.x) {

						//	+x����
						PlaySound(SOUND_Climb);
						numClimb[0] = ManagerB;	//	�悶�o����ݒ�
						numClimb[1] = 3;		//	�o�����
						g_posDestClimb = D3DXVECTOR3(	//	�o��ʒu
							posBlock.x - sizBlock.x,
							posBlock.y + sizBlock.y + g_objPlayer.higStand,
							dest.z);
						//	�o���� ����/���� �̐ݒ�
						if (Get_Floating(g_posDestClimb,g_sizStand)==false)	//	��ڐG����
							numClimb[2]=morLay;	//	����

						//	���p��
						//	-	�u���b�N����̍����Ŕ���
						if (g_objPlayer.pos.y <= posBlock.y + sizBlock.y) {
							numClimb[3] = 0;
							g_posDestClimb.y =posBlock.y + sizBlock.y;
							g_posDestClimb.x =posBlock.x - sizBlock.x;
						}
						else {
							numClimb[3] = 1;
						}

						g_objPlayer.mortion = morClimb;	//	�悶�o��ɏ�Ԃ�؂�ւ�
					}else
					if (rot >= D3DXToRadian(135)&&rot < D3DXToRadian(225) && 
						g_objPlayer.pos.z>posBlock.z+sizBlock.z) {
						//	-z����
						PlaySound(SOUND_Climb);
						numClimb[0] = ManagerB;	//	�悶�o����ݒ�
						numClimb[1] = 0;		//	�o�����
						g_posDestClimb = D3DXVECTOR3(	//	�o��ʒu
							dest.x,
							posBlock.y + sizBlock.y + g_objPlayer.higStand,
							posBlock.z + sizBlock.z);
						//	�o���� ����/���� �̐ݒ�
						if (Get_Floating(g_posDestClimb,g_sizStand)==false)	//	��ڐG����
							numClimb[2]=morLay;	//	����

						//	���p��
						//	-	�u���b�N����̍����Ŕ���
						if (g_objPlayer.pos.y <= posBlock.y + sizBlock.y) {
							numClimb[3] = 0;
							g_posDestClimb.y =posBlock.y + sizBlock.y;
							g_posDestClimb.z =posBlock.z + sizBlock.z;
						}
						else {
							numClimb[3] = 1;
						}

						g_objPlayer.mortion = morClimb;	//	�悶�o��ɏ�Ԃ�؂�ւ�

					}else
					if (rot >= D3DXToRadian(225) && rot < D3DXToRadian(315) &&
						g_objPlayer.pos.x>posBlock.x+sizBlock.x) {

						//	-x����
						PlaySound(SOUND_Climb);
						numClimb[0] = ManagerB;	//	�悶�o����ݒ�
						numClimb[1] = 1;		//	�o�����
						g_posDestClimb = D3DXVECTOR3(	//	�o��ʒu
							posBlock.x + sizBlock.x,
							posBlock.y + sizBlock.y + g_objPlayer.higStand,
							dest.z);
						//	�o���� ����/���� �̐ݒ�
						if (Get_Floating(g_posDestClimb,g_sizStand)==false)	//	��ڐG����
							numClimb[2]=morLay;	//	����

						//	���p��
						//	-	�u���b�N����̍����Ŕ���
						if (g_objPlayer.pos.y <= posBlock.y + sizBlock.y) {
							numClimb[3] = 0;
							g_posDestClimb.y =posBlock.y + sizBlock.y;
							g_posDestClimb.x =posBlock.x + sizBlock.x;
						}
						else {
							numClimb[3] = 1;
						}

						g_objPlayer.mortion = morClimb;	//	�悶�o��ɏ�Ԃ�؂�ւ�
					}
					else {
						continue;	//	���Ă͂܂�Ȃ����̂��X���[
					}
				}
			}
			flgBreak[ManagerB] = true; 
	}}}
}
void Sliding(void) {
	if (g_objPlayer.mortion == morSlide) {	//	�X���C�f�B���O����

	//	�ړ�����
	g_objPlayer.spdStep -= 1.0f;
	if (g_objPlayer.spdStep < 15) {
		g_objPlayer.spdStep = 0;	//	-�ȉ��ɂ��Ȃ�
	}

		D3DXVECTOR3 vecSlide;
		D3DXVec3Normalize(&vecSlide, &g_objPlayer.vecFlow);

		g_objPlayer.vecFlow += vecSlide*g_objPlayer.spdStep;
		if (g_objPlayer.spdStep == 0)g_objPlayer.mortion = morLay;
	}
}
//	�悶�o��
void Climbing(void) {
	//	�ϐ��錾
	D3DXVECTOR3 dest = INIT_Vector3D;

	if (numClimb[3] == 0)	//	�����グ
		dest = g_posDestClimb;
	else if (numClimb[3] == 2) {	//	��щz��
		if (numClimb[1] == 0)dest = D3DXVECTOR3(g_objPlayer.pos.x, Get_PosBlock(numClimb[0]).y + Get_SizBlock(numClimb[0]).y, Get_PosBlock(numClimb[0]).z - Get_SizBlock(numClimb[0]).z - g_objPlayer.siz.z);
		if (numClimb[1] == 1)dest = D3DXVECTOR3(Get_PosBlock(numClimb[0]).x - Get_SizBlock(numClimb[0]).x - g_objPlayer.siz.z, Get_PosBlock(numClimb[0]).y + Get_SizBlock(numClimb[0]).y, g_objPlayer.pos.z);
		if (numClimb[1] == 2)dest = D3DXVECTOR3(g_objPlayer.pos.x, Get_PosBlock(numClimb[0]).y + Get_SizBlock(numClimb[0]).y, Get_PosBlock(numClimb[0]).z + Get_SizBlock(numClimb[0]).z + g_objPlayer.siz.z);
		if (numClimb[1] == 3)dest = D3DXVECTOR3(Get_PosBlock(numClimb[0]).x + Get_SizBlock(numClimb[0]).x + g_objPlayer.siz.z, Get_PosBlock(numClimb[0]).y + Get_SizBlock(numClimb[0]).y, g_objPlayer.pos.z);
	}
	else {
		if (numClimb[1] == 0)	//	+z��
			dest = D3DXVECTOR3(g_objPlayer.pos.x,
				Get_PosBlock(numClimb[0]).y + Get_SizBlock(numClimb[0]).y,
				Get_PosBlock(numClimb[0]).z + Get_SizBlock(numClimb[0]).z);
		if (numClimb[1] == 1)	//	+x��
			dest = D3DXVECTOR3(
				Get_PosBlock(numClimb[0]).x + Get_SizBlock(numClimb[0]).x,
				Get_PosBlock(numClimb[0]).y + Get_SizBlock(numClimb[0]).y,
				g_objPlayer.pos.z);
		if (numClimb[1] == 2)	//	-z��
			dest = D3DXVECTOR3(g_objPlayer.pos.x,
				Get_PosBlock(numClimb[0]).y + Get_SizBlock(numClimb[0]).y,
				Get_PosBlock(numClimb[0]).z - Get_SizBlock(numClimb[0]).z);
		if (numClimb[1] == 3)	//	-x��
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

	//	�o��h���s��
	if (numClimb[2] != morLay) {
		if (GetKeyboardTrigger(DIK_SPACE)||GetJoyStickTrigger(5))	numClimb[2] = morJump;	//	�o��W�����v
		if (GetKeyboardTrigger(DIK_LSHIFT)||GetJoyStickTrigger(4))	numClimb[2] = morLay;	//	�قӂ�

		float lenOver = 0.0f;	//	��щz������
		D3DXVECTOR3 posOver = INIT_Vector3D;	//	��щz����v�Z
		if (numClimb[1] == 0)posOver = D3DXVECTOR3(g_objPlayer.pos.x, Get_PosBlock(numClimb[0]).y + Get_SizBlock(numClimb[0]).y, Get_PosBlock(numClimb[0]).z - Get_SizBlock(numClimb[0]).z - g_objPlayer.siz.z - 1);
		if (numClimb[1] == 1)posOver = D3DXVECTOR3(Get_PosBlock(numClimb[0]).x - Get_SizBlock(numClimb[0]).x - g_objPlayer.siz.x - 1, Get_PosBlock(numClimb[0]).y + Get_SizBlock(numClimb[0]).y, g_objPlayer.pos.z);
		if (numClimb[1] == 2)posOver = D3DXVECTOR3(g_objPlayer.pos.x, Get_PosBlock(numClimb[0]).y + Get_SizBlock(numClimb[0]).y, Get_PosBlock(numClimb[0]).z + Get_SizBlock(numClimb[0]).z + g_objPlayer.siz.z + 1);
		if (numClimb[1] == 3)posOver = D3DXVECTOR3(Get_PosBlock(numClimb[0]).x + Get_SizBlock(numClimb[0]).x + g_objPlayer.siz.x + 1, Get_PosBlock(numClimb[0]).y + Get_SizBlock(numClimb[0]).y, g_objPlayer.pos.z);

		if (numClimb[1] == 0 || numClimb[1] == 2)lenOver = Get_SizBlock(numClimb[0]).z;
		if (numClimb[1] == 1 || numClimb[1] == 3)lenOver = Get_SizBlock(numClimb[0]).x;

		//	��щz����Ԃւ̑���
		if ((GetKeyboardPress(DIK_F)||GetJoyStickPress(7)) && lenOver < 50.0f&& g_flgOverSound == false &&	//	����&��������
			Get_Floating(posOver, g_objPlayer.siz)) {	//	��ڐG����

			numClimb[3] = 2;	//	��щz�����
			PlaySound(SOUND_Over);
			g_flgOverSound = true;
		}
	}

	//	�ړ�
	if (Move_10to1(&g_objPlayer.pos, dest) < 1.0f) { //	�ړ��ʂŔ���
		g_objPlayer.pos = dest;				//	�ړ�
		if (numClimb[3]) {
			g_objPlayer.mortion = numClimb[2];	//	�̐��ύX

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
//	���˂�
void Leaping(void){
	if (GetKeyboardRelease(DIK_SPACE)||GetJoyStickTrigger(5)) {
		g_objPlayer.spdJump += 10.1f;
		PlaySound( SOUND_Jump);
	}
}
//	�v���C���[��X/Z���ړ�
void Move_PlayerXZ(void) {
	//	�ϐ��錾
	D3DXVECTOR3 dest = INIT_Vector3D;

	if (g_flgGround == false)
		g_objPlayer.pos.x += g_objPlayer.vecFlow.x;	//	x�ړ�
	for (int Manager = 0; Manager < NUM_MaxBlock; Manager++) {
		if (Get_flgBlock(Manager) == false)continue;	//	�������Ă��Ȃ��X�e�[�W�𖳎�

		//	�{�b�N�X�R���C�_�[(�ڐG����)
		if (g_objPlayer.pos.y + g_objPlayer.siz.y > Get_PosBlock(Manager).y - Get_SizBlock(Manager).y &&	//	��������
			g_objPlayer.pos.y - g_objPlayer.siz.y < Get_PosBlock(Manager).y + Get_SizBlock(Manager).y)
		if (g_objPlayer.pos.x + g_objPlayer.siz.x > Get_PosBlock(Manager).x - Get_SizBlock(Manager).x &&	//	x����
			g_objPlayer.pos.x - g_objPlayer.siz.x < Get_PosBlock(Manager).x + Get_SizBlock(Manager).x &&
			g_objPlayer.pos.z + g_objPlayer.siz.z > Get_PosBlock(Manager).z - Get_SizBlock(Manager).z &&	//	z����
			g_objPlayer.pos.z - g_objPlayer.siz.z < Get_PosBlock(Manager).z + Get_SizBlock(Manager).z) {	//	-1�͔���O�i��ڐG�j

			//	���v����
			D3DXVECTOR3
				posBlock = Get_PosBlock(Manager),
				sizBlock = Get_SizBlock(Manager),
				dest3 = g_objPlayer.pos; dest3.y = posBlock.y + sizBlock.y + g_objPlayer.siz.y + 1;

			//	�Ⴂ�i���𔻒�
			if (g_flgStand && g_objPlayer.pos.y - g_objPlayer.siz.y / 2.0f > posBlock.y + sizBlock.y
				&& Get_Floating(dest3, g_objPlayer.siz)) {

				//	������x�̍��������
				g_objPlayer.pos.y = posBlock.y + sizBlock.y + g_objPlayer.siz.y + 1;
			}
			else {
				//	�����o��
				if (g_objPlayer.vecFlow.x > 0)
					g_objPlayer.pos.x = posBlock.x - sizBlock.x - g_objPlayer.siz.x - 1;
				if (g_objPlayer.vecFlow.x < 0)
					g_objPlayer.pos.x = posBlock.x + sizBlock.x + g_objPlayer.siz.x + 1;
			}
		}
	}
	if (g_flgGround == false)
		g_objPlayer.pos.z += g_objPlayer.vecFlow.z;	//	z���ړ�
	for (int Manager = 0; Manager < NUM_MaxBlock; Manager++) {	//	�u���b�N�Ƃ̔���
		if (Get_flgBlock(Manager) == false)continue;	//	�������Ă��Ȃ��u���b�N�𖳎�
		if (g_objPlayer.pos.y + g_objPlayer.siz.y > Get_PosBlock(Manager).y - Get_SizBlock(Manager).y &&	//	��������
			g_objPlayer.pos.y - g_objPlayer.siz.y < Get_PosBlock(Manager).y + Get_SizBlock(Manager).y)
		if (g_objPlayer.pos.x + g_objPlayer.siz.x > Get_PosBlock(Manager).x - Get_SizBlock(Manager).x &&	//	x����
			g_objPlayer.pos.x - g_objPlayer.siz.x < Get_PosBlock(Manager).x + Get_SizBlock(Manager).x &&
			g_objPlayer.pos.z + g_objPlayer.siz.z > Get_PosBlock(Manager).z - Get_SizBlock(Manager).z &&	//	z����
			g_objPlayer.pos.z - g_objPlayer.siz.z < Get_PosBlock(Manager).z + Get_SizBlock(Manager).z) {	//	-1�͔���O�i��ڐG�j

			//	���v����
			D3DXVECTOR3
				posBlock = Get_PosBlock(Manager),
				sizBlock = Get_SizBlock(Manager),
				dest3 = g_objPlayer.pos; dest3.y = posBlock.y + sizBlock.y + g_objPlayer.siz.y + 1;

			//	�Ⴂ�i���𔻒�
			if (g_flgStand && (g_objPlayer.pos.y - g_objPlayer.siz.y / 2.0f > posBlock.y + sizBlock.y || g_flgClimb)
				&& Get_Floating(dest3, g_objPlayer.siz)) {

				//	������x�̍��������
				g_objPlayer.pos.y = posBlock.y + sizBlock.y + g_objPlayer.siz.y + 1;
			}
			else {
				//	�����o��
				if (g_objPlayer.vecFlow.z > 0)
					g_objPlayer.pos.z = posBlock.z - sizBlock.z - g_objPlayer.siz.z - 1;
				if (g_objPlayer.vecFlow.z < 0)
					g_objPlayer.pos.z = posBlock.z + sizBlock.z + g_objPlayer.siz.z + 1;

				//	�悶�o�蔻��p�v�Z
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

	//	�����������p���ɂ���
	if (g_objPlayer.spdFall > 0 && Get_Floating(g_objPlayer.pos, INIT_sizPlayer) &&
		!g_flgTransform && g_objPlayer.mortion == morLay) {
		g_objPlayer.mortion = morStand; TransformPlayer();
	}


}
//	�v���C���[��Y���ړ�
void Move_PlayerY(void) {
	//	�ϐ��錾
	D3DXVECTOR3 dest = INIT_Vector3D;

	g_objPlayer.pos += D3DXVECTOR3(0.0f, g_objPlayer.spdJump - g_objPlayer.spdFall, 0.0f);

	//	�㉺���v����
	for (int Manager = 0; Manager < NUM_MaxBlock; Manager++) {
		if (Get_flgBlock(Manager) == false)continue;	//	�������Ă��Ȃ��X�e�[�W�𖳎�
		if (g_objPlayer.pos.y + g_objPlayer.siz.y > Get_PosBlock(Manager).y - Get_SizBlock(Manager).y &&	//	��������
			g_objPlayer.pos.y - g_objPlayer.siz.y < Get_PosBlock(Manager).y + Get_SizBlock(Manager).y)
		if (g_objPlayer.pos.x + g_objPlayer.siz.x > Get_PosBlock(Manager).x - Get_SizBlock(Manager).x &&	//	x����
			g_objPlayer.pos.x - g_objPlayer.siz.x < Get_PosBlock(Manager).x + Get_SizBlock(Manager).x &&
			g_objPlayer.pos.z + g_objPlayer.siz.z > Get_PosBlock(Manager).z - Get_SizBlock(Manager).z &&	//	z����
			g_objPlayer.pos.z - g_objPlayer.siz.z < Get_PosBlock(Manager).z + Get_SizBlock(Manager).z){ 
			g_flgGround = true;
			if (g_objPlayer.spdJump - g_objPlayer.spdFall < 0) {	//	�ڒn����
				g_objPlayer.pos.y = Get_PosBlock(Manager).y + Get_SizBlock(Manager).y + g_objPlayer.siz.y;
				g_objPlayer.spdJump =		//	�W�����v�͂�
					g_objPlayer.spdFall = 0.0f;	//	�������x��0�ɐݒ�
				if (g_objPlayer.mortion == morStand && (g_flgSprint || g_objPlayer.flgRun) && g_objPlayer.spdStep > 0) {
					g_objPlayer.mortion = morStagger;
					PlaySound(SOUND_Jump);
				}
			}
			if (g_objPlayer.spdJump > 0) {	//	���㔻��
				g_objPlayer.pos.y = Get_PosBlock(Manager).y - Get_SizBlock(Manager).y - g_objPlayer.siz.y;
			}
	}}

	//	�ړ��\����
	//	-	�ڒn����
	g_objPlayer.pos.y -= 0.1f;	//	0.1f���Ɉړ�
	if (Get_Floating(g_objPlayer.pos, g_objPlayer.siz)==false) {	//	�ڐG����

		Sliding();
		if (g_objPlayer.mortion != morSlide)	//	�X���C�f�B���O���s��
			Stepping();		//	�ڒn�ړ��i����/�삯��/�����j
		g_flgStand = true;
		g_objPlayer.spdJump =		//	�W�����v�͂�
			g_objPlayer.spdFall = 0.0f;	//	�������x��0�ɐݒ�
		g_objPlayer.vecFlow *= 0.1f;	//	�ړ����x�����Z

		if (g_objPlayer.mortion != morSlide)	//	�X���C�f�B���O���s��
			Leaping();		//	�W�����v����

							//	����/�����؂�ւ�
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
	else {//	��ڒn��
		g_objPlayer.spdFall += 0.5f;	//	�������x�����Z
	}g_objPlayer.pos.y += 0.1f;	//	�ڒn����p�̈ړ�����߂�

								//	-	���㔻��
	g_objPlayer.pos.y += 0.1f;	//	���㔻��p�㏸
	if (Get_Floating(g_objPlayer.pos, g_objPlayer.siz)==false) {	//	�ڐG����
		g_objPlayer.spdJump = 0.0f;	//	�W�����v�͂�0�ɐݒ�
	}g_objPlayer.pos.y -= 0.1f;

}

//	�v���C���[�̃T�C�Y�ω�
void TransformPlayer(void) {

	if (g_flgTransform) {
		float diff=0.0f;	//	����

		if (g_objPlayer.mortion == morStand) {	//	����������
			diff = (g_objPlayer.higStand - g_objPlayer.siz.y) / 2.0f;

			g_objPlayer.siz.y += diff;
			g_objPlayer.pos.y += diff;
		}
		if (g_objPlayer.mortion == morLay ||
			(g_objPlayer.mortion == morClimb && numClimb[2]==morLay)) {	//	����������
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
		g_objPlayer.Item[Manager] = false;	//	�����A�C�e�����Ȃ��ɐݒ�

	g_cntStepSound = 0;

	numClimb[0] = -1; numClimb[1] = -1;
	g_cntToSprint = 0;
	g_flgSprint = false;
	g_flgOverSound = false;
	g_numItem = 0;
}

//	�A�C�e���̏�������
bool Get_flgItem(int nManager) {
	return g_objPlayer.Item[nManager];
}
//	�A�C�e���̏��������擾
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