/*-----------------------------------------------------------------------------
	�g�`�`��v���O����

  �@�u�X�^�[�g�v�{�^�����������ƃs�N�`���[�{�b�N�X���ɃE�B���h�E���쐬���Csin�g, ��`�g��`��D
  �@�u�X�g�b�v�v�{�^����I�����邱�ƂŔg�`�̕`����~���A�u���X�^�[�g�v�{�^�����������ƍĂєg�`��`�悷��D

-----------------------------------------------------------------------------*/
#include <windows.h>		//Windows��
#include <windowsx.h>		//Windows��
#include <stdio.h>			//���o�͗p
#include <process.h>		//�X���b�h�p
#include <stdlib.h>

#pragma comment(lib,"winmm.lib")//�����x�^�C�}

#include "header.h"			//�w�b�_�[�t�@�C��
#include "resource.h"		//���\�[�X�t�@�C��

//�\����
typedef struct {
	HWND	hwnd;
	HWND	hEdit;
}SEND_POINTER_STRUCT;

typedef struct {
	double sin;
	double square;
}data;

//======================================
//��������_�C�A���O�o�[�W����

//���C���֐�(�_�C�A���O�o�[�W����)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	HANDLE hMutex;

	//���d�N������
	hMutex = CreateMutex(NULL, TRUE, DEF_MUTEX_NAME);		//�~���[�e�b�N�X�I�u�W�F�N�g�̐���
	if(GetLastError() == ERROR_ALREADY_EXISTS){				//2�d�N���̗L�����m�F
		MessageBox(NULL, TEXT("���ɋN������Ă��܂��D"), NULL, MB_OK|MB_ICONERROR);
		return 0;											//�I��
	}

	//�_�C�A���O�N��
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAINDIALOG), NULL, MainDlgProc);

	return FALSE;			//�I��
}


//���C���v���V�[�W���i�_�C�A���O�j
/********************************

�V�X�e����ʁi.rc�t�@�C���j�ɔz�u�����{�^�����������ꂽ�Ƃ���C
�������C�I�����ɂǂ̂悤�ȏ������s�����������ɋL�q����D

********************************/
BOOL CALLBACK MainDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	static HANDLE hThread;
	static UINT thID;
	static SEND_POINTER_STRUCT Sps;
	static HWND start; //�X�^�[�g�{�^���̃n���h�����l������ϐ�
	static HWND stop; //�X�g�b�v�{�^���̃n���h��

	switch(uMsg){
	case WM_INITDIALOG:		//�_�C�A���O������(exe���_�u���N���b�N������)


		/*******

		�V�X�e����ʁi.rc�t�@�C���j�Ɏ����Œǉ������{�^����Picture Control��
		�͕K��HWND�^��GetDlgItem�֐���p���ăn���h�����擾���܂��D

		��FhWnd = GetDlgItem(hDlg, ID);
		hWnd�F�擾�������n���h��
		hDlg�F�_�C�A���O�̃n���h���D�����͊�{�I�ɂ��̂܂�
		ID�F�擾�������n���h���̑ΏۂƂȂ�{�^����Picture Control��
		    ��ID�D.rc�t�@�C������m�F�ł���D�ڍׂ͔z�z�������Q�l

	   ********/

		Sps.hwnd = hDlg;
		start = GetDlgItem(hDlg, ID_START); //�J�n�{�^���̃n���h�����擾
		stop = GetDlgItem(hDlg, ID_STOP); //��~�{�^���̃n���h�����擾

		return TRUE;

	case WM_COMMAND:		//�{�^���������ꂽ��
		/*******
		case WM_COMMAND:�ł͊e�{�^���������ꂽ�Ƃ��̏������L�ڂ��܂��D
	   ********/
		switch(LOWORD(wParam)){
		case ID_START:			//�X�^�[�g�{�^��
			/***********************
			�X�^�[�g�{�^���������ꂽ�Ƃ��ɕ`����J�n���܂��D
			�`����J�n���邽�߂�PictureControl�̃n���h�����擾����.
			***********************/

			hPict[0] = GetDlgItem(hDlg, ID_PICTBOX1);
			hPict[1] = GetDlgItem(hDlg, ID_PICTBOX2);
			hPict[2] = GetDlgItem(hDlg, ID_PICTBOX3);
			hPict[3] = GetDlgItem(hDlg, ID_PICTBOX4);
			WinInitialize(NULL, hDlg, (HMENU)110, "CH1", hPict[0], WndProc, &hWnd[0]); //������
			WinInitialize(NULL, hDlg, (HMENU)110, "CH2", hPict[1], WndProc, &hWnd[1]);
			WinInitialize(NULL, hDlg, (HMENU)110, "CH3", hPict[2], WndProc, &hWnd[2]);
			WinInitialize(NULL, hDlg, (HMENU)110, "CH4", hPict[3], WndProc, &hWnd[3]);
			//WinInitialize�֐��ɂ���Ďq�E�B���h�E�v���V�[�W����hPict��hWnd�Ƃ��ăZ�b�g����܂����D

			//�X���b�h�N��
			hThread = (HANDLE)_beginthreadex(NULL, 0, TFunc, (PVOID)&Sps, 0, &thID);   //_beginthreadex���X���b�h�𗧂��グ��֐�
			EnableWindow(GetDlgItem(hDlg, ID_START), FALSE);						//�X�^�[�g�{�^���������@�@�@�@//EnableWindow�œ��͂𖳌��܂��͗L���ɂ���B


			return TRUE;

		case ID_STOP:		//�X�g�b�v�{�^��
							/*�@�T�X�y���h�J�E���^�@**************************
						�@�@�@���s��������܂ŃX���b�h�𓮂����Ȃ��B
						   �@�@ResumeThread�F�@�T�X�y���h�J�E���^��1���炷
							 �@SuspendThread�F�@�T�X�y���h�J�E���^��1���₷

							  0�̂Ƃ��͎��s�B����ȊO�͑ҋ@����B
							  **************************************************/


			if (ResumeThread(hThread) == 0) {					//��~�����𒲂ׂ�(�T�X�y���h�J�E���g���P���炷)
				SetDlgItemText(hDlg, ID_STOP, TEXT("���X�^�[�g"));	//���X�^�[�g�ɕύX�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@//SetDlgItemText�Ń_�C�A���O���̃e�L�X�g�Ȃǂ�ύX���邱�Ƃ��ł���
				SuspendThread(hThread);						//�X���b�h�̎��s���~(�T�X�y���h�J�E���g���P���₷)
			}
			else
				SetDlgItemText(hDlg, ID_STOP, TEXT("�X�g�b�v"));	//�X�g�b�v�ɕύX

			return TRUE;

		}
		break;

    case WM_CLOSE:
        EndDialog(hDlg, 0);			//�_�C�A���O�I��
        return TRUE;
    }

	//�I�[�i�[�`���ɍĕ`��
	if (uMsg==WM_PAINT){
		InvalidateRect(hWnd[0], NULL, TRUE );	//�ĕ`��
		InvalidateRect(hWnd[1], NULL, TRUE);
	}

  return FALSE;
}

/********************************

������PictureControl�̕`����s���܂��D

�q�E�B���h�E�v���V�[�W������hWnd��PictureControl�̃n���h���ł��D

********************************/
//�q�E�B���h�E�v���V�[�W��
HRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return (DefWindowProc(hWnd, uMsg, wParam,lParam));
	}

	return TRUE;
}



//-----------------------------------------------------------------------------
//�q�E�B���h�E������������
//�w�肵���E�B���h�E�n���h���̗̈�Ɏq�E�B���h�E�𐶐�����D
//----------------------------------------------------------
// hInst	: �����p�C���X�^���X�n���h��
// hPaWnd	: �e�E�B���h�E�̃n���h��
// chID		: �q�E�B���h�E��ID
// cWinName	: �q�E�B���h�E��
// PaintArea: �q�E�B���h�E�𐶐�����̈�̃f�o�C�X�n���h��
// WndProc	: �E�B���h�E�v���V�[�W��
// *hWnd	: �q�E�B���h�E�̃n���h���i�|�C���^�j
// �߂�l	: ������=true
//-----------------------------------------------------------------------------
BOOL WinInitialize( HINSTANCE hInst, HWND hPaWnd, HMENU chID,char *cWinName, HWND PaintArea, WNDPROC WndProc ,HWND *hWnd)
{
	WNDCLASS wc;			//�E�B���h�E�N���X
	WINDOWPLACEMENT	wplace;	//�q�E�B���h�E�����̈�v�Z�p�i��ʏ�̃E�B���h�E�̔z�u�����i�[����\���́j
	RECT WinRect;			//�q�E�B���h�E�����̈�
	ATOM atom;				//�A�g��

	//�E�B���h�E�N���X������
	wc.style		=CS_HREDRAW ^ WS_MAXIMIZEBOX | CS_VREDRAW;	//�E�C���h�E�X�^�C��
	wc.lpfnWndProc	=WndProc;									//�E�C���h�E�̃��b�Z�[�W����������R�[���o�b�N�֐��ւ̃|�C���^
	wc.cbClsExtra	=0;											//
	wc.cbWndExtra	=0;
	wc.hCursor		=NULL;										//�v���O�����̃n���h��
	wc.hIcon		=NULL;										//�A�C�R���̃n���h��
	wc.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);		//�E�C���h�E�w�i�F
	wc.hInstance	=hInst;										//�E�C���h�E�v���V�[�W��������C���X�^���X�n���h��
	wc.lpszMenuName	=NULL;										//���j���[��
	wc.lpszClassName=(LPCTSTR)cWinName;									//�E�C���h�E�N���X��

	if(!(atom=RegisterClass(&wc))){
		MessageBox(hPaWnd,TEXT("�E�B���h�E�N���X�̐����Ɏ��s���܂����D"),NULL,MB_OK|MB_ICONERROR);
		return false;
	}

	GetWindowPlacement(PaintArea,&wplace);	//�`��̈�n���h���̏����擾(�E�B���h�E�̕\����Ԃ��擾)
	WinRect=wplace.rcNormalPosition;		//�`��̈�̐ݒ�

	//�E�B���h�E����
	*hWnd=CreateWindow(
		(LPCTSTR)atom,
		(LPCTSTR)cWinName,
		WS_CHILD | WS_VISIBLE,//| WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME |WS_VISIBLE,
		WinRect.left,WinRect.top,
		WinRect.right-WinRect.left,WinRect.bottom-WinRect.top,
		hPaWnd,chID,hInst,NULL
	);

	if( *hWnd==NULL ){
		MessageBox(hPaWnd,TEXT("�E�B���h�E�̐����Ɏ��s���܂����D"),NULL,MB_OK|MB_ICONERROR);
		return false;
	}

	return true;
}

//�f�[�^�ǂݍ��ݗp�X���b�h
UINT WINAPI TFunc(LPVOID thParam)
{
	static SEND_POINTER_STRUCT* FU = (SEND_POINTER_STRUCT*)thParam;        //�\���̂̃|�C���^�擾

	FILE* fp;			//�t�@�C���|�C���^
	BOOL Flag = TRUE;		//���[�v�t���O
	RECT rect[4];
	HDC			hdc[4];				//�f�o�C�X�R���e�L�X�g�̃n���h��
	PAINTSTRUCT ps;					//(�\����)�N���C�A���g�̈�`�悷�邽�߂̏��
	HBRUSH		hBrush, hOldBrush[4];	//�u���V
	HPEN		hPen1,hPen2, hOldPen[4];		//�y��

	double data[5];	//�f�[�^
	double data_plot[4];		//�E�B���h�E�T�C�Y�ɍ��킹���f�[�^
	int width[4], height[4];	//�E�B���h�E�̃T�C�Y
	int pos_x[4], pos_y[4];	//1�T�O�̃v���b�g�_�̈ʒu

	DWORD DNum = 0, beforeTime;

	int time = 0, min = 0;

	//���݂̎����v�Z�i�������ԁj
	beforeTime = timeGetTime();

	//�F�̎w��
	color1 = RGB(255, 255, 255);	//�F�w��i���j
	color2 = RGB(0, 0, 0);	//�F�w��i���j
	color3 = RGB(0, 0, 255);	//�F�w��i�j

	//�y���E�u���V����
	hBrush = CreateSolidBrush(color2);				//�u���V����
	hPen1 = CreatePen(PS_SOLID, 2, color1);		//�y������
	hPen2 = CreatePen(PS_SOLID, 2, color3);

	for (int i = 0; i < 4; i++) {
	//�f�o�C�X�R���e�L�X�g�̃n���h���擾
		hdc[i] = BeginPaint(hWnd[i], &ps);

		//�N���C�A���g�T�C�Y�̎擾
		GetClientRect(hWnd[i], &rect[i]);
		width[i] = rect[i].right;	//��
		height[i] = rect[i].bottom;	//����

		//�ʒu�̏�����
		pos_x[i] = width[i] * 0.05;
		pos_y[i] = height[i] * 0.5;
	}

	//�w�i�A���A���x���̐ݒ�
	for (int i = 0; i < 4; i++) {
		//�y���ݒ�
		hOldBrush[i] = (HBRUSH)SelectObject(hdc[i], hBrush);	//�u���V�ݒ�
		hOldPen[i] = (HPEN)SelectObject(hdc[i], hPen1);

		//�w�i�����ɂ���
		Rectangle(hdc[i], 0, 0, width[i] + 1, height[i] + 1);

		//�c��
		MoveToEx(hdc[i], (int)(width[i] * 0.05), height[i] * 0.05, NULL);		  //���̎n�_������
		LineTo(hdc[i], (int)(width[i] * 0.05), height[i] * 0.95);				    //���̏I�_�����肵�A����`��

		//����
		MoveToEx(hdc[i], (int)(width[i] * 0.05), (int)(height[i] * 0.5), NULL);
		LineTo(hdc[i], width[i] * 0.95, (int)(height[i] * 0.5));

		//�����̃��x��
		SetBkColor(hdc[i], color2);
		SetTextColor(hdc[i], color1);
		TextOut(hdc[i], (int)(width[i] * 0.45), (int)(height[i] * 0.8), TEXT("Time [s]"), 8);		//�e�L�X�g�`��
	}

	//�t�@�C���I�[�v��
	if ((fp = fopen("data.txt", "r")) == NULL) {
		MessageBox(NULL, TEXT("Input File Open ERROR!"), NULL, MB_OK | MB_ICONERROR);
		return FALSE;
	}

	//�f�[�^�ǂݍ��݁E�\��
	while (Flag == TRUE) {
		DWORD nowTime, progress, idealTime;

		//���Ԃ̒���
		nowTime = timeGetTime();					//���݂̎����v�Z
		progress = nowTime - beforeTime;				//�������Ԃ��v�Z
		idealTime = (DWORD)(DNum * (1000.F / (double)DEF_DATAPERS));	//���z���Ԃ��v�Z
		if (idealTime > progress) {
			Sleep(idealTime - progress);			//���z���ԂɂȂ�܂őҋ@
		}
		//�f�[�^�̓ǂݍ���
		while (fgetc(fp) != '\n' && !feof(fp));
		if (fscanf(fp, "%lf %lf %lf %lf %lf", &data[0], &data[1], &data[2], &data[3], &data[4]) == EOF) {
			MessageBox(NULL, TEXT("�I��"), TEXT("INFORMATION"), MB_OK | MB_ICONEXCLAMATION);
			EnableWindow(GetDlgItem(FU->hwnd, ID_START), TRUE);		//�X�^�[�g�{�^���L��
			Flag = FALSE;												//���[�v�I���t���O
			return FALSE;
		}



		//�t�@�C������ǂݎ�����f�[�^���E�B���h�E�ɍ����悤����
		for (int i = 0; i < 4; i++){
			data_plot[i] = (data[i + 1] * height[i] * 2) + height[i] / 2;
		}

		//�g�`�̕`��
		if (pos_x[0] + 1 >= (int)(width[0] * 0.95)) {
			for (int i = 0; i < 4; i++) {
				//�y���ݒ�
				hOldBrush[i] = (HBRUSH)SelectObject(hdc[i], hBrush);	//�u���V�ݒ�
				hOldPen[i] = (HPEN)SelectObject(hdc[i], hPen1);

				//�w�i�����ɂ���
				Rectangle(hdc[i], 0, 0, width[i] + 1, height[i] + 1);

				//�c��
				MoveToEx(hdc[i], (int)(width[i] * 0.05), height[i] * 0.05, NULL);		  //���̎n�_������
				LineTo(hdc[i], (int)(width[i] * 0.05), height[i] * 0.95);				    //���̏I�_�����肵�A����`��

				//����
				MoveToEx(hdc[i], (int)(width[i] * 0.05), (int)(height[i] * 0.5), NULL);
				LineTo(hdc[i], width[i] * 0.95, (int)(height[i] * 0.5));

				//�����̃��x��
				SetBkColor(hdc[i], color2);
				SetTextColor(hdc[i], color1);
				TextOut(hdc[i], (int)(width[i] * 0.45), (int)(height[i] * 0.8), TEXT("Time [s]"), 8);		//�e�L�X�g�`��

				pos_x[i] = width[i] * 0.05;
				pos_y[i] = data_plot[i];
			}
		}
		else {
			for (int i = 0; i < 4; i++) {
				//�y���ݒ�
				hOldPen[i] = (HPEN)SelectObject(hdc[i], hPen2);

				//�v���b�g
				MoveToEx(hdc[i], pos_x[i], pos_y[i], NULL);
				LineTo(hdc[i], pos_x[i], data_plot[i]);

				//�ʒu�̍X�V
				pos_x[i] += 1;
				pos_y[i] = (int)(data_plot[i]);

			}
		}



		DNum++;

		//��b�o�ߎ�
		if (progress >= 1000.0) {
			beforeTime = nowTime;
			DNum = 0;
		}
	}

	//�y���C�u���V�p��
	for (int i = 0; i < 4; i++) {
		SelectObject(hdc[i], hOldBrush);
		DeleteObject(hBrush);
		SelectObject(hdc[i], hOldPen);
		DeleteObject(hPen1);
		DeleteObject(hPen2);
	}
	
	



	return 0;
}