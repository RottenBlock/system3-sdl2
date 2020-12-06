/*
	ALICE SOFT SYSTEM 3 for Win32

	[ NACT ]
*/

#ifndef _NACT_H_
#define _NACT_H_

#include <windows.h>
#include <process.h>
#if defined(_DEBUG_CONSOLE)
#include <wincon.h>
#endif
#include <mmsystem.h>
#include <tchar.h>
#include <stdio.h>
#include "../common.h"

#define RND var[ 0]
#define D01 var[ 1]
#define D02 var[ 2]
#define D03 var[ 3]
#define D04 var[ 4]
#define D05 var[ 5]
#define D06 var[ 6]
#define D07 var[ 7]
#define D08 var[ 8]
#define D09 var[ 9]
#define D10 var[10]
#define D11 var[11]
#define D12 var[12]
#define D13 var[13]
#define D14 var[14]
#define D15 var[15]
#define D16 var[16]
#define D17 var[17]
#define D18 var[18]
#define D19 var[19]
#define D20 var[20]
#define U01 var[21]
#define U02 var[22]
#define U03 var[23]
#define U04 var[24]
#define U05 var[25]
#define U06 var[26]
#define U07 var[27]
#define U08 var[28]
#define U09 var[29]
#define U10 var[30]
#define U11 var[31]
#define U12 var[32]
#define U13 var[33]
#define U14 var[34]
#define U15 var[35]
#define U16 var[36]
#define U17 var[37]
#define U18 var[38]
#define U19 var[39]
#define U20 var[40]
#define B01 var[41]
#define B02 var[42]
#define B03 var[43]
#define B04 var[44]
#define B05 var[45]
#define B06 var[46]
#define B07 var[47]
#define B08 var[48]
#define B09 var[49]
#define B10 var[50]
#define B11 var[51]
#define B12 var[52]
#define B13 var[53]
#define B14 var[54]
#define B15 var[55]
#define B16 var[56]
#define M_X var[57]
#define M_Y var[58]

#define MAX_STACK 100
#define MAX_MENU 64
#define MAX_VERB 128
#define MAX_OBJ 256
#define MAX_CAPTION 32
#define MAX_PCM 256

class AGS;
class MAKO;

class NACT
{
protected:
	AGS *ags;
	MAKO *mako;

private:
	// �R�}���h�p�[�T
	void execute();
	void load_scenario(int page);

	bool fatal_error;
	bool post_quit;

	// ADISK.DAT, ASLEEP.DAT
	char adisk[16];

	// �V�i���I�f�[�^
	uint8 *scenario_data;
	int scenario_size;

	// �A�h���X�A�y�[�W�Ǘ�
	int scenario_addr, prev_addr;
	int scenario_page;
	int label_depth;
	int label_stack[MAX_STACK];
	int page_depth;
	int page_stack[MAX_STACK];
	int addr_stack[MAX_STACK];

	// �ϐ�
	uint16 var[512];
	uint16 var_stack[30][20];
//	char tvar[10][22];
	char tvar_stack[30][10][22];
//	int tvar_index;

	// �R�}���h
	void cmd_calc();

	void cmd_branch();
	void cmd_label_jump();
	void cmd_label_call();
	void cmd_page_jump();
	void cmd_page_call();

	void cmd_set_menu();
	void cmd_open_menu();

	void cmd_set_verbobj();
	void cmd_set_verbobj2();
	void cmd_open_verb();
	void cmd_open_obj(int verb);

	void cmd_a();
	void cmd_b();
	void cmd_d();
	void cmd_e();
	void cmd_f();
	void cmd_g();
	void cmd_h();
	void cmd_i();
	void cmd_j();
	void cmd_k();
	void cmd_l();
	void cmd_m();
	void cmd_n();
	void cmd_o();
	void cmd_p();
	void cmd_q();
	void cmd_r();
	void cmd_s();
	void cmd_t();
	void cmd_u();
	void cmd_v();
	void cmd_w();
	void cmd_x();
	void cmd_y();
	void cmd_z();

#if defined(_SYSTEM1)
	void opening();
	// D.P.S. - Dream Program System
	bool text_refresh;
	// Intruder -�����~�̒T��-
	int paint_x;		// Intruder Z�R�}���h
	int paint_y;
	int map_page;
#endif

	bool column;		// ���W���[�h
	bool wait_keydown;	// �E�F�C�g���̃L�[��t
	int text_wait_time;	// �e�L�X�g�\���̃E�F�C�g
	int mouse_sence;	// �}�E�X���x

	int menu_window;	// ���j���[�E�B���h�E�ԍ�
	int text_window;	// ���b�Z�[�W�E�B���h�E�ԍ�
	bool show_push;		// Push�\��
	bool clear_text;	// ���j���[��̃��b�Z�[�W�E�B���h�E����

	int menu_index;			// ���j���[�o�^�̃C���f�b�N�X
	int menu_addr[64];		// �W�����v��̃A�h���X
	int menu_verb[64];		// �o�^���ꂽ����
	int menu_obj[64];		// �o�^���ꂽ�ړI��

	char caption_verb[MAX_VERB][MAX_CAPTION];
	char caption_obj[MAX_OBJ][MAX_CAPTION];

	bool verb_obj;	// ����-�`�e���^���j���[�̒�`��
	bool set_palette;

	int pcm_index;
	int pcm[MAX_PCM];

	// ���ʊ֐�
	uint16 cali();
	uint16 cali2();
	uint8 getd();
	uint16 getw();

	uint16 random(uint16 range);
	uint32 seed;

	void wait_after_open_menu();

	uint8 get_key();
	void get_cursor(int *x, int *y);
	void set_cursor(int x, int y);

	int joy_num;
	JOYCAPS joycaps;
	uint8 key[256];
	int mouse_x, mouse_y;

	uint32 calc_crc32(const char *file_name);

	// Y27 �_�C�A���O
	static BOOL CALLBACK TextDialogProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

	// ���C���X���b�h
	typedef struct {
		NACT *nact;
		bool terminate;
	} PARAMS, *PPARAMS;
	PARAMS params;

	static void thread(PVOID pvoid);
	HANDLE hThread;

	// �f�o�b�O���O
	void initialize_console();
	void release_console();
	HANDLE hConsole;

public:
	NACT();
	~NACT();

	int get_screen_height();
	void update_screen(HDC hdc, int sx, int sy, int width, int  height);
	void notify_mci(int status);

	void key_down(int val);
	void key_up(int val);
	void select_cursor();
	void select_sound(int dev);

	bool get_title(_TCHAR *title, int length);

	bool text_skip_enb;	// ���b�Z�[�W�X�L�b�v
	bool text_wait_enb;	// �e�L�X�g�\���̃E�F�C�g�L���^����
	uint32 crc32_a;		// ADISK
	uint32 crc32_b;		// BDISK for D.P.S -SG- and Super D.P.S

	// for Y27
	char tvar[10][33];
	int tvar_index, tvar_maxlen;

	// �f�o�b�O���O
	void output_console(const char *format, ...);
};

#endif

