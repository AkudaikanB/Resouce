//	�ړI�n(=Goal)�̐���

//	�t�@�C������
#include "../Heads/goal.h"
#include "../Heads/player.h"

//	�O���[�o���ϐ��錾
LPDIRECT3DVERTEXBUFFER9 g_pVB_Goals;		//	�ړI�n�`��p�̒��_�o�b�t�@
Object	g_objGoals[NUM_MaxGoal];			//	�ړI�n�̐���p�ϐ�
bool	g_flgGoals[NUM_MaxGoal];			//	�ړI�n�̗L��
bool	g_flgGoalCount[NUM_MaxGoal];		//	�S�[����������
int		g_numItemToGoal[NUM_MaxGoal]
						[NUM_ItemToGoal];	//	�S�[�������p�A�C�e���Ǘ��ԍ�

//	�v���g�^�C�v�錾
void MakeGoalsBuffer(LPDIRECT3DDEVICE9 pDevice);

//	
//	�֐���`
//
//	�������֐�
void InitGoal(LPDIRECT3DDEVICE9 pDevice){
	MakeGoalsBuffer(pDevice);	//	���_�o�b�t�@�𐶐�

	ZeroMemory(g_objGoals, sizeof(g_objGoals));	//	�ϐ���0�N���A�i�s��l�ǂݎ��h�~�j
	ZeroMemory(g_flgGoals, sizeof(g_flgGoals));	//	�ϐ���0�N���A�i�s��l�ǂݎ��h�~�j
	ZeroMemory(g_flgGoalCount, sizeof(g_flgGoalCount));	//	�ϐ���0�N���A�i�s��l�ǂݎ��h�~�j

	//	�S�[��������������
	for(int Manager=0;Manager<NUM_MaxGoal;Manager++)
	for(int Eraser=0;Eraser<NUM_ItemToGoal;Eraser++)
		g_numItemToGoal[Manager][Eraser]=-1;
}

//	�g�p�����������̊J��
void UninitGoal(void) {
	if (g_pVB_Goals) {
		g_pVB_Goals->Release();
		g_pVB_Goals = NULL;
	}
}

//	�ړI�n�`��p�̒��_�o�b�t�@�𐶐�
void MakeGoalsBuffer(LPDIRECT3DDEVICE9 pDevice) {

	//	���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D)*NUM_MaxGoal*NUM_VtxGoal,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVB_Goals,
		NULL
	);
}

//	�X�V
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

//	�`��
void DrawGoals(LPDIRECT3DDEVICE9 pDevice) {
	D3DXMATRIXA16 matWorld;			//	���[���h�s��𐶐�

	//	�e�N�X�`���[��ݒ�i�Ȃ��j
	pDevice->SetTexture(0,NULL);	//	�e�N�X�`�������F�e�N�X�`���̃|�C���^��NULL�ɐݒ�

	pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);			//	���C�e�B���O����
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);	//	���u�����h��L��������
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);	//	���ʃJ�����O��L����

	//	�`��
	for (int Manager = 0; Manager < NUM_MaxGoal; Manager++) {
		if (g_flgGoals[Manager] == false)continue;	//	�������Ă��Ȃ��S�[���𖳎�

		//	���[���h�s����������i�ЂƂO�̃��[���h�s���ێ������Ȃ����߁j
		D3DXMatrixIdentity(&matWorld);					//	�P�ʍs����i�[
		pDevice->SetTransform(D3DTS_WORLD,&matWorld);	//	�P�ʍs���ݒ�

		//	���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(
			0,
			g_pVB_Goals,
			0,
			sizeof(VERTEX_3D)
		);

		//	�`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,
			Manager*NUM_VtxGoal,
			NUM_PlgGoal
		);
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);			//	���C�e�B���O�L��
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);	//	���u�����h�𖳌�������
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	//	���ʃJ�����O�𖳌���
}

//	�S�[���i�傫��siz.x*siz.z*siz.y�̒����́j���i�ʒupos�Ɂj����
int CreateGoal(D3DXVECTOR3 pos,float rot ,D3DXVECTOR3 siz) {
	
	//	�ϐ��錾
	int Manager = 0;	//	�Ǘ��ԍ�
	VERTEX_3D*pVtx;	//	���_��񐶐��p�̃|�C���^

	g_pVB_Goals->Lock(0, 0, (void**)&pVtx, 0);	//	���_�o�b�t�@�͈̔͂����b�N

	//	�������Ă��Ȃ��ړI�n�v�f������
	for (Manager = 0; Manager < NUM_MaxGoal; Manager++, pVtx += NUM_VtxGoal)
		if (g_flgGoals[Manager] == false) {
			break;	//	�������Ă��Ȃ��ړI�n��������ƌJ��Ԃ����I��
		}
	if (Manager >= NUM_MaxGoal-1 || Manager<0) {
		g_pVB_Goals->Unlock();
		return NULL;
	}

	//	�g�p�t���O�𗧂Ă�
	g_flgGoals[Manager] = true;

	//	�I�u�W�F�N�g�̃Q�[��������ݒ�
	g_objGoals[Manager].pos = pos;	//	�ʒu��ݒ�
	g_objGoals[Manager].siz = siz;	//	�傫����ݒ�

	//	���_���𐶐�
	//	-	�ʒu����
	pVtx[0].pos =pVtx[8].pos = pVtx[16].pos =
	pVtx[1].pos =pVtx[9].pos = pVtx[17].pos =
	pVtx[2].pos =pVtx[10].pos =pVtx[18].pos =
	pVtx[3].pos =pVtx[11].pos =pVtx[19].pos =
	pVtx[4].pos =pVtx[12].pos =pVtx[20].pos =
	pVtx[5].pos =pVtx[13].pos =pVtx[21].pos =
	pVtx[6].pos =pVtx[14].pos =pVtx[22].pos =
	pVtx[7].pos =pVtx[15].pos =pVtx[23].pos =   pos;
	//	-	�ʒu����傫�������_�����炷
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

	//	�ʂ̖@���x�N�g����ݒ�
	pVtx[0].vec =	pVtx[1].vec =	pVtx[2].vec =	pVtx[3].vec =	D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
	pVtx[4].vec =	pVtx[5].vec =	pVtx[6].vec =	pVtx[7].vec =	D3DXVECTOR3( 0.0f, 0.0f,-1.0f);
	pVtx[8].vec =	pVtx[9].vec =	pVtx[10].vec =	pVtx[11].vec =	D3DXVECTOR3( 0.0f,-1.0f, 0.0f);
	pVtx[12].vec =	pVtx[13].vec =	pVtx[14].vec =	pVtx[15].vec =	D3DXVECTOR3( 1.0f, 0.0f, 0.0f);
	pVtx[16].vec =	pVtx[17].vec =	pVtx[18].vec =	pVtx[19].vec =	D3DXVECTOR3( 0.0f, 0.0f, 1.0f);
	pVtx[20].vec =	pVtx[21].vec =	pVtx[22].vec =	pVtx[23].vec =	D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

	//	�F��ݒ�
	pVtx[0].def =pVtx[8].def = pVtx[16].def =
	pVtx[1].def =pVtx[9].def = pVtx[17].def =
	pVtx[2].def =pVtx[10].def =pVtx[18].def =
	pVtx[3].def =pVtx[11].def =pVtx[19].def =
	pVtx[4].def =pVtx[12].def =pVtx[20].def =
	pVtx[5].def =pVtx[13].def =pVtx[21].def =
	pVtx[6].def =pVtx[14].def =pVtx[22].def =	//	�ړI�n�̏����F�͓����i�b��ԁj
	pVtx[7].def =pVtx[15].def =pVtx[23].def =   D3DXCOLOR(1.0f,0.25f,0.25f,0.0f);

	//	�e�N�X�`�����W�i�g��Ȃ��j��ݒ�
	pVtx[0].tex =pVtx[8].tex = pVtx[16].tex =
	pVtx[1].tex =pVtx[9].tex = pVtx[17].tex =
	pVtx[2].tex =pVtx[10].tex =pVtx[18].tex =
	pVtx[3].tex =pVtx[11].tex =pVtx[19].tex =
	pVtx[4].tex =pVtx[12].tex =pVtx[20].tex =
	pVtx[5].tex =pVtx[13].tex =pVtx[21].tex =
	pVtx[6].tex =pVtx[14].tex =pVtx[22].tex =
	pVtx[7].tex =pVtx[15].tex =pVtx[23].tex =   D3DXVECTOR2(0.0f,0.0f);


	g_pVB_Goals->Unlock();	//	���_�o�b�t�@���A�����b�N

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

//	�w�肳�ꂽ�ړI�n�𖳌���
void EraseGoal(int Manager) {
	g_flgGoals[Manager] = false;		//	��������𖳌���
	g_flgGoalCount[Manager] = false;	//	�J�E���^�[���X�g�b�v

	//	�S�[��������������
	for(int Eraser=0;Eraser<NUM_ItemToGoal;Eraser++)
		g_numItemToGoal[Manager][Eraser]=-1;

	EraseGoalFLG(Manager);
	Reset_cntGoal(Manager);
}

//	�ڐG�����ړI�n��Ԃ��i��ڐG����1��Ԃ��j
int Get_TouchedGoal(D3DXVECTOR3 pos, D3DXVECTOR3 siz) {
	//	�ϐ��錾
	int	Manager = 0;	//	�ړI�n�̔ԍ�

	//	�ڐG����ړI�n�𔻒�i�{�b�N�X�R���C�_�[�j
	for (Manager = 0; Manager < NUM_MaxGoal; Manager++) {
		if (g_flgGoals[Manager] == false)continue;	//	�������Ă��Ȃ��ړI�n�𖳎�
		if (pos.y + siz.y > g_objGoals[Manager].pos.y - g_objGoals[Manager].siz.y &&	//	��������
			pos.y - siz.y < g_objGoals[Manager].pos.y + g_objGoals[Manager].siz.y)
		if (pos.x + siz.x > g_objGoals[Manager].pos.x - g_objGoals[Manager].siz.x &&	//	x����
			pos.x - siz.x < g_objGoals[Manager].pos.x + g_objGoals[Manager].siz.x &&
			pos.z + siz.z > g_objGoals[Manager].pos.z - g_objGoals[Manager].siz.z &&	//	z����
			pos.z - siz.z < g_objGoals[Manager].pos.z + g_objGoals[Manager].siz.z)
			return Manager;
	}

	return -1;	//	��ڐG����1��Ԃ�
}
//	�A�C�e���Ǘ��ԍ��ɑΉ������S�[���ԍ���Ԃ�
int Get_StartingGoal(int nItem) {
	for(int ManagerA=0;ManagerA<NUM_MaxGoal;ManagerA++)
	for(int ManagerB=0;ManagerB<NUM_ItemToGoal;ManagerB++)
		if(g_numItemToGoal[ManagerA][ManagerB]==nItem)return ManagerA;

	return -1;	//	������Ȃ��ꍇ��-1��Ԃ�
}
//	�S�[���̌��������Ԃ�
bool Get_flgGoalCount(int nManager) {
	return g_flgGoalCount[nManager];
}

//	�S�[������p�̃A�C�e�������擾
int Get_numItemOfGoal(int nManager){
	int cntItem = 0;
	for (int Manager = 0; Manager<NUM_ItemToGoal; Manager++)
		if (g_numItemToGoal[nManager][Manager] > -1)
			cntItem++;

	return cntItem;
}
//	�S�[������
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

	//	�F��ݒ�
	pVtx[0].def =pVtx[8].def = pVtx[16].def =
	pVtx[1].def =pVtx[9].def = pVtx[17].def =
	pVtx[2].def =pVtx[10].def =pVtx[18].def =
	pVtx[3].def =pVtx[11].def =pVtx[19].def =
	pVtx[4].def =pVtx[12].def =pVtx[20].def =
	pVtx[5].def =pVtx[13].def =pVtx[21].def =
	pVtx[6].def =pVtx[14].def =pVtx[22].def =	//	�ړI�n�̏����F�͐�
	pVtx[7].def =pVtx[15].def =pVtx[23].def =   COL_String[IDX_colDenial];

	if (type == 1) {
		pVtx[0].def = pVtx[8].def = pVtx[16].def =
		pVtx[1].def = pVtx[9].def = pVtx[17].def =
		pVtx[2].def = pVtx[10].def = pVtx[18].def =
		pVtx[3].def = pVtx[11].def = pVtx[19].def =
		pVtx[4].def = pVtx[12].def = pVtx[20].def =
		pVtx[5].def = pVtx[13].def = pVtx[21].def =
		pVtx[6].def = pVtx[14].def = pVtx[22].def =	//	�Ǘ��ԍ��ɍ��킹���J���[�����O
		pVtx[7].def = pVtx[15].def = pVtx[23].def = COL_INDEX[nManager];
	}
	if (type == 2) {
		pVtx[0].def = pVtx[8].def = pVtx[16].def =
		pVtx[1].def = pVtx[9].def = pVtx[17].def =
		pVtx[2].def = pVtx[10].def = pVtx[18].def =
		pVtx[3].def = pVtx[11].def = pVtx[19].def =
		pVtx[4].def = pVtx[12].def = pVtx[20].def =
		pVtx[5].def = pVtx[13].def = pVtx[21].def =
		pVtx[6].def = pVtx[14].def = pVtx[22].def =	//	������
		pVtx[7].def = pVtx[15].def = pVtx[23].def = D3DCOLOR_RGBA(255,255,255,0);
	}

	g_pVB_Goals->Unlock();
}

//	��������S�[���̐���Ԃ�
int Get_numGoal(void){
	int cnt=0;

	for (int Manager = 0; Manager < NUM_MaxGoal; Manager++)
		if (g_flgGoals[Manager])cnt++;

	return cnt;
}