//	�A�C�e���I�u�W�F�N�g(:Item,�ȉ�=�A�C�e��)�̐���

//	�t�@�C������
#include "../Heads/item.h"
#include "../Heads/camera.h"	//	�r���{�[�h�̕`��Ɏg�p
#include "../Heads/goal.h"

//	�O���[�o���ϐ��錾
Object	g_objItems[NUM_MaxItem];	//	�A�C�e���̏��ϐ�
bool	g_flgItems[NUM_MaxItem];	//	�A�C�e���̌������
LPDIRECT3DVERTEXBUFFER9	g_pVB_Items = NULL;		//	���_(���)�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pTex_Items = NULL;	//	�e�N�X�`���ւ̃|�C���^

//	�v���g�^�C�v�錾
void MakeItemsBuffer(LPDIRECT3DDEVICE9 pDevice);

//
//	�֐���`
//
//	�������֐�
void InitItems(LPDIRECT3DDEVICE9 pDevice) {

	ZeroMemory(g_objItems,sizeof(g_objItems));	//	�Ǘ��������ׂ�0�ŏ������i�s��l�ǂݎ��h�~�j
	ZeroMemory(g_flgItems,sizeof(g_flgItems));	//	�Ǘ��������ׂ�0�ŏ������i�s��l�ǂݎ��h�~�j
	
	MakeItemsBuffer(pDevice);	//	���_�o�b�t�@�̐���

	D3DXCreateTextureFromFile(	//	�e�N�X�`���̐���
		pDevice,
		ADR_TextureFruit00,
		&g_pTex_Items
	);
}
void UninitItems(void) {
	if (g_pVB_Items) {	//	���_�o�b�t�@�����
		g_pVB_Items->Release();
		g_pVB_Items = NULL;
	}
	if (g_pTex_Items) {	//	�e�N�X�`���̉��
		g_pTex_Items->Release();
		g_pTex_Items = NULL;
	}
}

void UpdateItems(void) {
}
void DrawItems(LPDIRECT3DDEVICE9 pDevice) {
	//	�ϐ��錾
	D3DXMATRIX matInvCamera;		//	�J�����̋t�s��
	D3DXMATRIX matBillboard;		//	�r���{�[�h�`��Ɏg�p����s��
	D3DXMATRIX matTranslate;		//	�r���{�[�h�̕`��Ɏg�p����s��
	camera Camera = Get_Camera();	//	�J�����̏����擾

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);				//	���C�e�B���O������
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		//	���e�X�g��L����
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	D3DXMatrixIdentity(&matBillboard);	//	������

	//	�t�s���ݒ�
	pDevice->GetTransform(D3DTS_VIEW,&matInvCamera);
	D3DXMatrixInverse(&matInvCamera,NULL,&matInvCamera);
	matInvCamera._41=
	matInvCamera._42=
	matInvCamera._43=0.0f;

	//	�e�N�X�`����ݒ�
	pDevice->SetTexture(0,g_pTex_Items);

	//	�o�b�t�@�̎Q�Ɣ͈͂�ݒ�
	pDevice->SetStreamSource(0,g_pVB_Items,0,sizeof(VERTEX_3D));

	//	�`��
	for (int Manager = 0; Manager < NUM_MaxItem; Manager++) {
		if (g_flgItems[Manager] == false)continue;	//	�������Ă��Ȃ��A�C�e����`�悵�Ȃ�

		D3DXMatrixIdentity(&matBillboard);	//	������

		D3DXMatrixTranslation(&matTranslate,g_objItems[Manager].pos.x, g_objItems[Manager].pos.y, g_objItems[Manager].pos.z);
		D3DXMatrixMultiply(&matBillboard, &matBillboard, &matInvCamera);
		D3DXMatrixMultiply(&matBillboard, &matBillboard, &matTranslate);
		pDevice->SetTransform(D3DTS_WORLD,&matBillboard);

		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,
			Manager*NUM_VertexPolygon2D,
			NUM_Polygon2D
		);
	}

	//	���C�e�B���O��L��
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		//	���e�X�g�𖳌���
}

void MakeItemsBuffer(LPDIRECT3DDEVICE9 pDevice) {
	//	�A�C�e���̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D)*NUM_VertexPolygon2D*NUM_MaxItem,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVB_Items,
		NULL
	);
}

//	�A�C�e���𐶐�
int CreateItem(D3DXVECTOR3 pos, float siz) {
	//	�ϐ��錾
	int Manager;	//	�Ǘ��ԍ�
	VERTEX_3D*pVtx;	//	���_��񐶐��p�̃|�C���^
	
	g_pVB_Items->Lock(0,0,(void**)&pVtx,0);	//	���_�o�b�t�@�͈̔͂����b�N

	//	�������Ă��Ȃ��ϐ��i�Ɣԍ��j������
	for (Manager = 0; Manager < NUM_MaxItem; Manager++, pVtx += NUM_VertexPolygon2D)
		if (g_flgItems[Manager] == false)break;	//	�g���Ă��Ȃ��ϐ���������ƌJ��Ԃ����甲����
	if (Manager >= NUM_MaxItem) {
		g_pVB_Items->Unlock();
		return -1;
	}

	g_flgItems[Manager] = true;	//	�g�p�t���O�𗧂Ă�

	//	���_�̏�����
	//	-	�傫����ݒ�
	pVtx[0].pos = D3DXVECTOR3(-siz, +siz, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+siz, +siz, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-siz, -siz, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+siz, -siz, 0.0f);
	//	-	�@���x�N�g����ݒ�
	pVtx[0].vec = 
	pVtx[1].vec =
	pVtx[2].vec =
	pVtx[3].vec = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	//	-	���_�J���[��ݒ�
	pVtx[0].def = 
	pVtx[1].def =
	pVtx[2].def =
	pVtx[3].def = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	//	-	�e�N�X�`�����W��ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//	�A�C�e���̊Ǘ�����ݒ�
	g_objItems[Manager].pos = pos;
	g_objItems[Manager].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	g_objItems[Manager].rot = D3DXVECTOR2(0.0f,0.0f);
	g_objItems[Manager].siz = D3DXVECTOR3(siz,siz,siz);

	g_pVB_Items->Unlock();	//	���_�o�b�t�@�͈̔͂��A�����b�N
	
	return Manager;
}

//	�ڐG�����A�C�e���̔ԍ���Ԃ��i��ڐG����1��Ԃ��j
int Get_TouchedItem(D3DXVECTOR3 pos, D3DXVECTOR3 siz) {
	//	�ϐ��錾
	int	Manager = 0;	//	�A�C�e���̔ԍ�

						//	�ڐG����A�C�e���𔻒�i�{�b�N�X�R���C�_�[�j
	for (Manager = 0; Manager < NUM_MaxItem; Manager++) {
		if (g_flgItems[Manager] == false)continue;	//	�������Ă��Ȃ��A�C�e���𖳎�
		if (pos.y + siz.y > g_objItems[Manager].pos.y - g_objItems[Manager].siz.y &&	//	��������
			pos.y - siz.y < g_objItems[Manager].pos.y + g_objItems[Manager].siz.y)
			if (pos.x + siz.x > g_objItems[Manager].pos.x - g_objItems[Manager].siz.x &&	//	x����
				pos.x - siz.x < g_objItems[Manager].pos.x + g_objItems[Manager].siz.x &&
				pos.z + siz.z > g_objItems[Manager].pos.z - g_objItems[Manager].siz.z &&	//	z����
				pos.z - siz.z < g_objItems[Manager].pos.z + g_objItems[Manager].siz.z)
				return Manager;
	}

	return -1;	//	��ڐG����1��Ԃ�
}

//	�w�肳�ꂽ�A�C�e���𖳌���
void EraseItem(int Manager) {
	g_flgItems[Manager] = false;
	EraseItemFLG(Manager);
}