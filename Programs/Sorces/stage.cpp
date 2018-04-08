//	�X�e�[�W�I�u�W�F�N�g(=Block,�ȉ��u���b�N)�E�S�[������̐���

//	�t�@�C������
#include "../Heads/stage.h"
#include "../Heads/sound.h"

//	�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9	g_pVB_Blocks = NULL;	//	���_���o�b�t�@�ւ̃|�C���^
Object	g_objBlocks[NUM_MaxBlock];	//	�u���b�N�̏��ϐ�
bool	g_flgBlocks[NUM_MaxBlock];	//	�u���b�N�̌������
int		g_numFlagGoal[NUM_MaxBlock];	//	�u���b�N�̏�������i�ړI�n�����Ŕ���j
D3DXVECTOR3 g_desBlock[NUM_MaxBlock];	//	�u���b�N�̈ړ���

//	�v���g�^�C�v�錾
void MakeBlocksBuffer(LPDIRECT3DDEVICE9 pDevice);	//	�u���b�N�̒��_��񐶐�

//
//	�֐���`
//
//	�������֐�
void InitStage(LPDIRECT3DDEVICE9 pDevice) {
	//	���_�E�C���f�b�N�X�o�b�t�@�̐���
	MakeBlocksBuffer(pDevice);

	//	������
	ZeroMemory(&g_objBlocks, sizeof(Object)*NUM_MaxBlock);	//	�u���b�N�̏������ׂ�0�ŏ㏑���i�s��l�ǂݎ��h�~�j
	ZeroMemory(&g_flgBlocks, sizeof(bool)*NUM_MaxBlock);	//	�u���b�N�����������ׂ�0�ŏ㏑���i�s��l�ǂݎ��h�~�j

//	SetMouseMode(true);	//	�}�E�X����
}
//	�������[�J��
void UninitStage(void) {
	if (g_pVB_Blocks) {	//	���_�o�b�t�@�����
		g_pVB_Blocks->Release();
		g_pVB_Blocks = NULL;
	}
}

//	�X�V
void UpdateStage(void) {
}
//	�`��
void DrawStage(LPDIRECT3DDEVICE9 pDevice) {
	//	�ϐ��錾
	D3DXMATRIXA16 matWorld;

	//	���_�t�H�[�}�b�g��ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);	//	3D���_�t�H�[�}�b�g

	//	�e�N�X�`���[��ݒ�i�Ȃ��j
	pDevice->SetTexture(0,NULL);	//	�e�N�X�`�������F�e�N�X�`���̃|�C���^��NULL�ɐݒ�

	for (int Manager = 0; Manager < NUM_MaxBlock; Manager++) {
		if (g_flgBlocks[Manager] == false)continue;		//	�������Ȃ��u���b�N�𖳎�

		//	���[���h�s������Z�b�g�i�ЂƂO�̍s�����ێ������Ȃ����߁j
		D3DXMatrixIdentity(&matWorld);					//	�P�ʍs����i�[
		pDevice->SetTransform(D3DTS_WORLD, &matWorld);	//	�s���ݒ�

		//	���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(
			0,					//	�X�g���[�����w��
			g_pVB_Blocks,		//	���_�o�b�t�@�ւ̃|�C���^��n��
			0,					//	�I�t�Z�b�g��ݒ�
			sizeof(VERTEX_3D)	//	�傫����n��
		);

		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,
			Manager*NUM_VtxBlock,
			NUM_plgBlock
		);
	}
}

//	��񐧌�p�o�b�t�@����(������)
void MakeBlocksBuffer(LPDIRECT3DDEVICE9 pDevice) {

	//	���_��񐧌�p�̃o�b�t�@������
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D)*NUM_MaxBlock*NUM_VtxBlock,	//	�p�ӂ���o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,		//	���_�o�b�t�@�̎g�p�@���w��
		FVF_VERTEX_3D,			//	���̃t�H�[�}�b�g��ݒ�i3D�`��p�̒��_�t�H�[�}�b�g��ݒ�j
		D3DPOOL_MANAGED,		//	�o�b�t�@��ێ����郁�����N���X���w��
		&g_pVB_Blocks,			//	��������o�b�t�@�ւ̃|�C���^���w��
		NULL					//	(	�g��Ȃ����ߊ�{��NULL	)
	);
}

//	pos�ʒu��siz.x*siz.y*siz.z�̒����̂𐶐�
int CreateBlock(D3DXVECTOR3 pos,float rot, D3DXVECTOR3 siz) {
	//	�ϐ��錾
	int Manager = 0;	//	�Ǘ��ԍ�
	VERTEX_3D*pVtx;	//	���_��񐶐��p�̃|�C���^

	g_pVB_Blocks->Lock(0, 0, (void**)&pVtx, 0);	//	���_�o�b�t�@�͈̔͂����b�N

	//	�������Ă��Ȃ��u���b�N�v�f������
	for (Manager = 0; Manager < NUM_MaxBlock; Manager++, pVtx += NUM_VtxBlock)
		if (g_flgBlocks[Manager] == false)break;	//	�������Ă��Ȃ��u���b�N��������ƌJ��Ԃ����I��

	//	�g�p�t���O�𗧂Ă�
	g_flgBlocks[Manager] = true;

	//	�I�u�W�F�N�g�̃Q�[��������ݒ�
	g_objBlocks[Manager].pos = pos;	//	�ʒu��ݒ�
	g_objBlocks[Manager].siz = siz;	//	�傫����ݒ�

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
	pVtx[6].def =pVtx[14].def =pVtx[22].def =	//	�u���b�N�̐F�͔�
	pVtx[7].def =pVtx[15].def =pVtx[23].def =   D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	//	�e�N�X�`�����W�i�g��Ȃ��j��ݒ�
	pVtx[0].tex =pVtx[8].tex = pVtx[16].tex =
	pVtx[1].tex =pVtx[9].tex = pVtx[17].tex =
	pVtx[2].tex =pVtx[10].tex =pVtx[18].tex =
	pVtx[3].tex =pVtx[11].tex =pVtx[19].tex =
	pVtx[4].tex =pVtx[12].tex =pVtx[20].tex =
	pVtx[5].tex =pVtx[13].tex =pVtx[21].tex =
	pVtx[6].tex =pVtx[14].tex =pVtx[22].tex =
	pVtx[7].tex =pVtx[15].tex =pVtx[23].tex =   D3DXVECTOR2(0.0f,0.0f);


	g_pVB_Blocks->Unlock();	//	���_�o�b�t�@���A�����b�N

	return Manager;
}

//	�ڐG�����u���b�N��Ԃ��i��ڐG����1��Ԃ��j
int Get_TouchedBlock(D3DXVECTOR3 pos, D3DXVECTOR3 siz) {
	//	�ϐ��錾
	int	Manager = 0;	//	�u���b�N�̔ԍ�

	//	�ڐG����u���b�N�𔻒�i�{�b�N�X�R���C�_�[�j
	for (Manager = 0; Manager < NUM_MaxBlock; Manager++) {
		if (g_flgBlocks[Manager] == false)continue;	//	�������Ă��Ȃ��X�e�[�W�𖳎�
		if (pos.y + siz.y > g_objBlocks[Manager].pos.y - g_objBlocks[Manager].siz.y &&	//	��������
			pos.y - siz.y < g_objBlocks[Manager].pos.y + g_objBlocks[Manager].siz.y)
		if (pos.x + siz.x > g_objBlocks[Manager].pos.x - g_objBlocks[Manager].siz.x &&	//	x����
			pos.x - siz.x < g_objBlocks[Manager].pos.x + g_objBlocks[Manager].siz.x &&
			pos.z + siz.z > g_objBlocks[Manager].pos.z - g_objBlocks[Manager].siz.z &&	//	z����
			pos.z - siz.z < g_objBlocks[Manager].pos.z + g_objBlocks[Manager].siz.z){ 
			return Manager;
		}
	}

	return -1;	//	��ڐG����1��Ԃ�
}

//	�w�肳�ꂽ�u���b�N�̈ʒu���擾
D3DXVECTOR3 Get_PosBlock(int nManager) {
	return g_objBlocks[nManager].pos;
}
//	�w�肳�ꂽ�u���b�N�̑傫�����擾
D3DXVECTOR3 Get_SizBlock(int nManager) {
	return g_objBlocks[nManager].siz;
}
//	�w�肳�ꂽ�u���b�N�̎g�p�t���O���擾
bool Get_flgBlock(int nManager) {
	return g_flgBlocks[nManager];
}

void EraseBlock(int Index) {
	g_flgBlocks[Index] = false;
	g_desBlock[Index] = INIT_Vector3D;
	g_numFlagGoal[Index] = -1;
}

//	�S�[�����łɉ������u���b�N����
void EraseBlockByGoal(int nGoal){
	for (int Manager = 0; Manager < NUM_MaxBlock; Manager++) {
		//	nGoal�Ԗڂ̃S�[���ɑ΂�������Ώۂ𔻒�
		if (g_numFlagGoal[Manager] == nGoal) {
			EraseBlock(Manager);	//	����
			PlaySound(SOUND_cngPhase);
		}
	}
}

//	��������ɑΉ�����S�[���ԍ��̐ݒ�
void Set_numEraseByGoal(int nStage,int nGoal){
	g_numFlagGoal[nStage] = nGoal;
}
