/*
	ALICE SOFT SYSTEM 3 for Win32

	[ NACT ]
*/

#include <windows.h>
#include <windowsx.h>
#include "nact.h"
#include "ags.h"
#include "mako.h"
#include "dri.h"
#include "crc32.h"
#include "../fileio.h"
#if defined(_SYSTEM1)
#include "../res1/resource.h"
#elif defined(_SYSTEM2)
#include "../res2/resource.h"
#elif defined(_SYSTEM3)
#include "../res3/resource.h"
#endif

extern HWND g_hwnd;

// ������

NACT::NACT()
{
	memset(key, 0, sizeof(key));
	mouse_x = mouse_y = 0;

	// �f�o�b�O�R���\�[���N��
	initialize_console();

	// SYSTEM3 ������
	crc32_a = calc_crc32("ADISK.DAT");
	crc32_b = calc_crc32("BDISK.DAT");
	fatal_error = post_quit = false;

	// AG00.DAT�ǂݍ���
	FILEIO *fio = new FILEIO();

	if(fio->Fopen("AG00.DAT", FILEIO_READ_BINARY)) {
		int d0, d1, d2, d3;
		char string[MAX_CAPTION];
		fio->Fgets(string, MAX_CAPTION);
		sscanf_s(string, "%d,%d,%d,%d", &d0, &d1, &d2, &d3);
		for(int i = 0; i < d1; i++) {
			// �����̓ǂݍ���
			fio->Fgets(string, MAX_CAPTION);
			memcpy(caption_verb[i], string, sizeof(string));
		}
		for(int i = 0; i < d2; i++) {
			// �ړI��̓ǂݍ���
			fio->Fgets(string, MAX_CAPTION);
			memcpy(caption_obj[i], string, sizeof(string));
		}
		fio->Fclose();
	}
	delete fio;

	// ADISK.DAT
#if defined(_PROG_OMAKE)
	strcpy_s(adisk, 16, "AGAME.DAT");
#else
	strcpy_s(adisk, 16, "ADISK.DAT");
#endif

	// �V�i���I�Ǘ�
	scenario_data = NULL;
	load_scenario(0);
	scenario_page = 0;
	scenario_addr = 2;
	label_depth = page_depth = 0;

	// �ϐ�������
	memset(var, 0, sizeof(var));
	memset(var_stack, 0, sizeof(var_stack));
	memset(tvar, 0, sizeof(tvar));
	memset(tvar_stack, 0, sizeof(tvar_stack));

	// �R�}���h������
	column = true;		// ���W���[�h
	wait_keydown = true;	// �E�F�C�g���̃L�[��t
	seed = timeGetTime();	// �����̎�
	text_wait_time = 100;	// ���b�Z�[�W�\���̃E�F�C�g
	text_wait_enb = false;
	text_skip_enb = false;
	mouse_sence = 16;	// �}�E�X�ړ����m

	menu_window = text_window = 1;
	menu_index = 0;
	show_push = true;
	clear_text = true;

	verb_obj = false;
	set_palette = false;

	tvar_index = 0;

	pcm_index = 0;
	memset(pcm, 0, sizeof(pcm));

#if defined(_SYSTEM1)
	// SYETEM1 ������
	if(crc32_a == CRC32_DPS || crc32_a == CRC32_DPS_SG || crc32_a == CRC32_DPS_SG2 || crc32_a == CRC32_DPS_SG3) {
		text_refresh = false;
		sprintf_s(tvar[0], 22, "�J�X�^��");
		sprintf_s(tvar[1], 22, "���[�i�X");
		sprintf_s(tvar[2], 22, "����");
		sprintf_s(tvar[3], 22, "�R���q");
		sprintf_s(tvar[4], 22, "����");
		sprintf_s(tvar[5], 22, "�ЂƂ�");
		sprintf_s(tvar[6], 22, "�^���q");
	} else if(crc32_a == CRC32_INTRUDER) {
		paint_x = paint_y = map_page = 0;
	}
#endif

	// �e��N���X����
	ags = new AGS(this);
	mako = new MAKO(this);

	// ���͏�����
	joy_num = joyGetNumDevs();
	if(joy_num) {
		joyGetDevCaps(JOYSTICKID1, &joycaps, sizeof(JOYCAPS));
	}

	// ���C���X���b�h�N��
	params.nact = this;
	params.terminate = false;
	hThread = (HANDLE)_beginthread(thread, 0, &params);
	SetThreadPriority(hThread, THREAD_PRIORITY_NORMAL);
}

NACT::~NACT()
{
	// ���C���X���b�h�I��
	params.terminate = true;
	if(hThread) {
		WaitForSingleObject(hThread, INFINITE);
	}
	hThread = NULL;

	// �e��N���X�J��
	if(ags) {
		delete ags;
	}
	if(mako) {
		delete mako;
	}

	// �V�i���I�J��
	if(scenario_data) {
		free(scenario_data);
	}

	// �f�o�b�O�R���\�[���J��
	release_console();
}

void NACT::thread(PVOID pvoid)
{
	volatile PPARAMS pparams;
	pparams = (PPARAMS)pvoid;
	int sleep_cnt = 0;

	while(!pparams->terminate) {
		pparams->nact->execute();
		// 512�R�}���h���s����Sleep(10)
		if(!(sleep_cnt = (sleep_cnt + 1) & 0x1ff)) {
			Sleep(10);
		} else {
			Sleep(0);
		}
	}
	_endthread();
}

// �R�}���h�p�[�T

void NACT::execute()
{
	// �A�h���X�̊m�F
	if(scenario_addr < 2 || scenario_addr >= scenario_size) {
		fatal_error = true;
	}

	// �v���I�ȃG���[���� or ����I��
	if(fatal_error) {
		if(!post_quit) {
			PostMessage(g_hwnd, WM_CLOSE, 0, 0);
		}
		post_quit = true;
		return;
	}

#if defined(_SYSTEM1)
	if(scenario_page == 0 && scenario_addr == 2) {
		opening();
	}
#endif

	// �P�R�}���h���s
	prev_addr = scenario_addr;
	uint8 cmd = getd();

	if(set_palette && cmd != 'P') {
		// �p���b�g�ݒ肪�I�����
		ags->flush_screen(true);
		set_palette = false;
	}
	if(verb_obj && cmd != '[' && cmd != ':') {
		// ����-�ړI�ꃁ�j���[�̕\��
		scenario_addr--;
		cmd_open_verb();
		return;
	}

	switch(cmd) {
		case '!':
			cmd_calc();
			break;
		case '{':
			cmd_branch();
			break;
		case '}':
			break;
		case '@':
			cmd_label_jump();
			break;
		case '\\':
			cmd_label_call();
			break;
		case '&':
			cmd_page_jump();
			break;
		case '%':
			cmd_page_call();
			break;
		case '$':
			cmd_set_menu();
			break;
		case '[':
			cmd_set_verbobj();
			break;
		case ':':
			cmd_set_verbobj2();
			break;
		case ']':
			cmd_open_menu();
			break;
		case 'A':
			cmd_a();
			break;
		case 'B':
			cmd_b();
			break;
		case 'D':
			cmd_d();
			break;
		case 'E':
			cmd_e();
			break;
		case 'F':
			cmd_f();
			break;
		case 'G':
			cmd_g();
			break;
		case 'H':
			cmd_h();
			break;
		case 'I':
			cmd_i();
			break;
		case 'J':
			cmd_j();
			break;
		case 'K':
			cmd_k();
			break;
		case 'L':
			cmd_l();
			break;
		case 'M':
			cmd_m();
			break;
		case 'N':
			cmd_n();
			break;
		case 'O':
			cmd_o();
			break;
		case 'P':
			cmd_p();
			break;
		case 'Q':
			cmd_q();
			break;
		case 'R':
			cmd_r();
			break;
		case 'S':
			cmd_s();
			break;
		case 'T':
			cmd_t();
			break;
		case 'U':
			cmd_u();
			break;
		case 'V':
			cmd_v();
			break;
		case 'W':
			cmd_w();
			break;
		case 'X':
			cmd_x();
			break;
		case 'Y':
			cmd_y();
			break;
		case 'Z':
			cmd_z();
			break;
		default:
			if(cmd == 0x20 || (0xa1 <= cmd && cmd <= 0xdd)) {
				// message (1 byte)
				char string[3];
				string[0] = cmd;
				string[1] = '\0';
				ags->draw_text(string);
				
#if defined(_SYSTEM1)
				if(crc32_a == CRC32_DPS || crc32_a == CRC32_DPS_SG || crc32_a == CRC32_DPS_SG2 || crc32_a == CRC32_DPS_SG3) {
					if(!ags->draw_menu) {
						text_refresh = false;
					}
				}
#endif
				if(!ags->draw_menu && text_wait_enb && cmd != 0x20) {
					DWORD dwTime = timeGetTime() + text_wait_time;
					for(;;) {
						if(params.terminate) {
							return;
						}
						RND = get_key();
						if(RND && wait_keydown) {
							break;
						}
						if(dwTime <= timeGetTime()) {
							break;
						}
						Sleep(10);
					}
				}
				if(!ags->draw_hankaku) {
					uint16 code = ags->convert_zenkaku(cmd);
					string[0] = code >> 8;
					string[1] = code & 0xff;
					string[2] = '\0';
				}
				output_console(string);
			} else if((0x81 <= cmd && cmd <= 0x9f) || 0xe0 <= cmd) {
				// message (2 bytes)
				char string[3];
				string[0] = cmd;
				string[1] = getd();
				string[2] = '\0';
				ags->draw_text(string);
				
#if defined(_SYSTEM1)
				if(crc32_a == CRC32_DPS || crc32_a == CRC32_DPS_SG || crc32_a == CRC32_DPS_SG2 || crc32_a == CRC32_DPS_SG3) {
					if(!ags->draw_menu) {
						text_refresh = false;
					}
				}
#endif
				if(!ags->draw_menu && text_wait_enb) {
					DWORD dwTime = timeGetTime() + text_wait_time;
					for(;;) {
						if(params.terminate) {
							return;
						}
						RND = get_key();
						if(RND && wait_keydown) {
							break;
						}
						if(dwTime <= timeGetTime()) {
							break;
						}
						Sleep(10);
					}
				}
				output_console(string);
			} else {
				if(cmd >= 0x20 && cmd < 0x7f) {
					output_console("\nUnknown Command: '%c' at page = %d, addr = %d\n", cmd, scenario_page, prev_addr);
				} else {
					output_console("\nUnknown Command: %02x at page = %d, addr = %d\n", cmd, scenario_page, prev_addr);
				}
				fatal_error = true;
			}
			break;
	}
}

void NACT::load_scenario(int page)
{
	if(scenario_data) {
		free(scenario_data);
	}
	DRI *dri = new DRI();
	if((scenario_data = dri->load(adisk, page + 1, &scenario_size)) == NULL) {
		fatal_error = true;
	}
	delete dri;
}

// ���ʊ֐�

uint16 NACT::random(uint16 range)
{
	// xorshift32
	seed = seed ^ (seed << 13);
	seed = seed ^ (seed >> 17);
	seed = seed ^ (seed << 15);
	return (uint16)(((uint32)range * (seed & 0xffff)) >> 16) + 1;
}

void NACT::wait_after_open_menu()
{
	// �A�łɂ���N���b�N�h�~
	DWORD dwTime = timeGetTime();
	DWORD dwWait = dwTime + 400;

	while(dwTime < dwWait) {
		if(params.terminate) {
			return;
		}
/*
		if(get_key() == 16) {
			break;
		}
*/
		Sleep(10);
		dwTime = timeGetTime();
	}

	// �N���b�N���̊Ԃ͑ҋ@
	for(;;) {
		if(params.terminate) {
			return;
		}
		if(!get_key()) {
			break;
		}
		Sleep(10);
	}
}

// WinMain�Ƃ̃C���^�[�t�F�[�X

int NACT::get_screen_height()
{
	return ags->screen_height;
}

void NACT::update_screen(HDC hdc, int sx, int sy, int width, int height)
{
	ags->update_screen(hdc, sx, sy, width, height);
}

void NACT::notify_mci(int status)
{
	mako->notify_mci(status);
}

void NACT::key_down(int val)
{
	key[val] = 1;
}

void NACT::key_up(int val)
{
	if(val == -1) {
		memset(key, 0, sizeof(key));
	} else {
		key[val] = 0;
	}
}

void NACT::select_cursor()
{
	ags->select_cursor();
}

void NACT::select_sound(int dev)
{
	// �����I�ɉ�����ύX����
	int page = mako->current_music;
	int old_dev = (1 <= page && page <= 99 && mako->cd_track[page]) ? 1 : 0;

	for(int i = 1; i <= 99; i++) {
		mako->cd_track[i] = dev ? i : 0;
	}

	// �f�o�C�X���ύX���ꂽ�ꍇ�͍ĉ��t����
	if(dev != old_dev && page) {
		mako->stop_music();
		mako->play_music(page);
	}
}

// �f�o�b�O�R���\�[��

void NACT::initialize_console()
{
#if defined(_DEBUG_CONSOLE)
	AllocConsole();
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#if defined(_SYSTEM1)
	SetConsoleTitle(_T("SYSTEM1 NACT Tracer"));
#elif defined(_SYSTEM2)
	SetConsoleTitle(_T("SYSTEM2 NACT Tracer"));
#elif defined(_SYSTEM3)
	SetConsoleTitle(_T("SYSTEM3 NACT Tracer"));
#endif
#endif
}

void NACT::release_console()
{
#if defined(_DEBUG_CONSOLE)
	FreeConsole();
#endif
}

void NACT::output_console(const char *format, ...)
{
#if defined(_DEBUG_CONSOLE)
	va_list ap;
	char buffer[1024];
	
	va_start(ap, format);
	vsprintf_s(buffer, sizeof(buffer), format, ap);
	va_end(ap);
	
	DWORD dwWritten;
	WriteConsoleA(hConsole, buffer, strlen(buffer), &dwWritten, NULL);
#endif
}

BOOL CALLBACK NACT::TextDialogProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static NACT *nact;
	char string[64];
	int pnt, cnt;
	
	switch(msg) {
		case WM_CLOSE:
			EndDialog(hDlg, IDCANCEL);
			break;
		case WM_INITDIALOG:
			// get this pointer
			nact = (NACT *)lParam;
			// init dialog
			sprintf_s(string, 64, "���������͂��Ă��������i�ő�%d�����j", nact->tvar_maxlen);
			SetWindowTextA(GetDlgItem(hDlg, IDC_TEXT), string);
			SetWindowTextA(GetDlgItem(hDlg, IDC_EDITBOX), nact->tvar[nact->tvar_index - 1]);
			if(nact->tvar[nact->tvar_index - 1][0] == '\0') {
				EnableWindow(GetDlgItem(hDlg, IDOK), FALSE);
			} else {
				EnableWindow(GetDlgItem(hDlg, IDOK), TRUE);
			}
			break;
		
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case IDC_EDITBOX:
					GetDlgItemTextA(hDlg, IDC_EDITBOX, string, 32);
					pnt = cnt = 0;
					while(string[pnt] != '\0') {
						unsigned char dat = string[pnt];
						if((0x81 <= dat && dat <= 0x9f) || 0xe0 <= dat) {
							pnt++;
						}
						pnt++;
						cnt++;
					}
					if(cnt == 0 || cnt > nact->tvar_maxlen) {
						EnableWindow(GetDlgItem(hDlg, IDOK), FALSE);
					} else {
						EnableWindow(GetDlgItem(hDlg, IDOK), TRUE);
					}
					break;
				case IDOK:
					GetDlgItemTextA(hDlg, IDC_EDITBOX, string, 32);
					strcpy_s(nact->tvar[nact->tvar_index - 1], 22, string);
					EndDialog(hDlg, IDOK);
					break;
				default:
					return FALSE;
			}
			break;
		
		default:
			return FALSE;
	}
	return TRUE;
}

