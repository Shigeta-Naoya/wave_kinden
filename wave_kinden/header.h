//-------------------------------------------------------------------------
//	main.cpp�t�@�C���̃w�b�_�[�t�@�C��
//	
//	�֐��C�ϐ��錾�p
//-------------------------------------------------------------------------

//�֐��錾
BOOL CALLBACK MainDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );	//���C���_�C�A���O�v���V�[�W��
BOOL WinInitialize( HINSTANCE hInst, HWND hPaWnd, HMENU chID, char *cWinName, HWND PaintArea, WNDPROC WndProc ,HWND *hDC);//�q�E�B���h�E�𐶐�
HRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );		//�q�E�B���h�E�v���V�[�W��

UINT WINAPI TFunc(LPVOID thParam);												//�f�[�^�ǂݍ��ݗp�X���b�h

//�ϐ��錾
#define DEF_APP_NAME	TEXT("Waveform Test")
#define DEF_MUTEX_NAME	DEF_APP_NAME			//�~���[�e�b�N�X��
#define DEF_DATAPERS 1000	//1�b�Ԃɉ��f�[�^���o�͂��邩
#define DATA_MAX 3010 //�f�[�^�̍ő��
#define N 5 //�f�[�^��ǂݍ��݃t�@�C����1�s�̕�����

static COLORREF	color1, color2, color3;	//�F
static HWND hWnd[4];	//�q�E�B���h�E�̃n���h��
static HWND hPict[4];		//�E�B���h�E�n���h���iPictureBox�j