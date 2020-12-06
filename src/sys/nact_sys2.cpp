/*
	ALICE SOFT SYSTEM2 for Win32

	Super D.P.S. - Dream Program System

	[ NACT - command ]
*/

#include "nact.h"
#include "ags.h"
#include "mako.h"
#include "crc32.h"
#include "../fileio.h"
#include "../res2/resource.h"

extern HINSTANCE g_hinst;
extern HWND g_hwnd;

#define WAIT(tm) \
{ \
	DWORD dwTime = timeGetTime() + (tm); \
	for(;;) { \
		if(params.terminate) { \
			return; \
		} \
		if(dwTime <= timeGetTime()) { \
			break; \
		} \
		Sleep(10); \
	} \
}

#define WAIT_KEYQUIT(tm) \
{ \
	DWORD dwTime = timeGetTime() + (tm); \
	for(;;) { \
		if(params.terminate) { \
			return; \
		} \
		if(get_key()) { \
			for(;;) { \
				if(params.terminate) { \
					return; \
				} \
				if(!get_key()) { \
					break; \
				} \
				if(dwTime <= timeGetTime()) { \
					break; \
				} \
				Sleep(10); \
			} \
			break; \
		} \
		if(dwTime <= timeGetTime()) { \
			break; \
		} \
		Sleep(10); \
	} \
}

void NACT::cmd_calc()
{
	int index = getd();
	if(0x80 <= index && index <= 0xbf) {
		index &= 0x3f;
	} else {
		index = ((index & 0x3f) << 8) | getd();
	}
	var[index] = cali();

	output_console("\n!var[%d]:%d!", index, var[index]);
}

void NACT::cmd_branch()
{
	int condition = cali();
	int nest = 0;
	bool set_menu = false;

	if(!condition) {
		// ����'}'���߂܂ŃX�L�b�v����i�l�X�g���l������j
		for(;;) {
			prev_addr = scenario_addr;
			uint8 cmd = getd();

			if(cmd == '!') {
				int index = getd();
				if(!(0x80 <= index && index <= 0xbf)) {
					getd();
				}
				cali();
			} else if(cmd == '{') {
				cali();
				nest++;
			} else if(cmd == '}') {
				if(nest) {
					nest--;
				} else {
					break;
				}
			} else if(cmd == '@') {
				getw();
			} else if(cmd == '\\') {
				getw();
			} else if(cmd == '&') {
				cali();
			} else if(cmd == '%') {
				cali();
			} else if(cmd == '$') {
				if(!set_menu) {
					getw();
					set_menu = true;
				} else {
					set_menu = false;
				}
			} else if(cmd == '[') {
				getd();
				getd();
				getw();
			} else if(cmd == ':') {
				cali();
				getd();
				getd();
				getw();
			} else if(cmd == ']') {
				
			} else if(cmd == 'A') {
				
			} else if(cmd == 'B') {
				cali();
				cali();
				cali();
				cali();
				cali();
				cali();
				cali();
			} else if(cmd == 'D') {
				cali();
				cali();
				cali();
				cali();
				cali();
				cali();
				cali();
				cali();
			} else if(cmd == 'E') {
				cali();
				cali();
				cali();
			} else if(cmd == 'F') {
				
			} else if(cmd == 'G') {
				cali();
			} else if(cmd == 'H') {
				getd();
				cali();
			} else if(cmd == 'I') {
				cali();
				cali();
//				getd();
				cali();
			} else if(cmd == 'J') {
				cali();
				cali();
			} else if(cmd == 'K') {
				
			} else if(cmd == 'L') {
				getd();
			} else if(cmd == 'M') {
				for(;;) {
					uint8 val = getd();
					if(val == 0x20 || (0xa1 <= val && val <= 0xdd)) {
						// message (1 byte)
					} else if((0x81 <= val && val <= 0x9f) || 0xe0 <= val) {
						// message (2 bytes)
						getd();
					} else if(val == ':') {
						break;
					}
				}
			} else if(cmd == 'N') {
				cali();
				cali();
			} else if(cmd == 'O') {
				cali();
				cali();
				cali();
			} else if(cmd == 'P') {
				getd();
			} else if(cmd == 'Q') {
				getd();
			} else if(cmd == 'R') {
				
			} else if(cmd == 'S') {
				getd();
			} else if(cmd == 'T') {
				cali();
				cali();
				cali();
			} else if(cmd == 'U') {
				if(crc32_a == CRC32_YAKATA2) {
					cali();
					cali();
				} else {
					getd();
					getd();
				}
			} else if(cmd == 'V') {
#if 1
				cali();
				cali();
				cali();
				cali();
				cali();
				cali();
				cali();
				cali();
				cali();
				cali();
				cali();
				cali();
				cali();
				cali();
				cali();
				cali();
				cali();
				cali();
				cali();
				cali();
				cali();
				cali();
				cali();
				cali();
				cali();
				cali();
				cali();
				cali();
				cali();
#else
				cali();
				cali();
#endif
			} else if(cmd == 'W') {
				cali();
				cali();
				cali();
				cali();
			} else if(cmd == 'X') {
				getd();
			} else if(cmd == 'Y') {
				cali();
				cali();
			} else if(cmd == 'Z') {
				cali();
				cali();
			} else if(cmd == 0x20 || (0xa1 <= cmd && cmd <= 0xdd)) {
				// message (1 byte)
			} else if((0x81 <= cmd && cmd <= 0x9f) || 0xe0 <= cmd) {
				// message (2 bytes)
				getd();
			} else {
				if(cmd >= 0x20 && cmd < 0x7f) {
					output_console("\nUnknown Command: '%c' at page = %d, addr = %d\n", cmd, scenario_page, prev_addr);
				} else {
					output_console("\nUnknown Command: %02x at page = %d, addr = %d\n", cmd, scenario_page, prev_addr);
				}
				fatal_error = true;
				break;
			}
		}
	}

	output_console("\n{%d:", condition);
}

void NACT::cmd_label_jump()
{
	int next_addr = getw();
	scenario_addr = next_addr;

	output_console("\n@%x:", next_addr);
}

void NACT::cmd_label_call()
{
	int next_addr = getw();

	output_console("\n\\%x:", next_addr);

	if(next_addr == 0) {
		// ���^�[��
		if(label_depth == 0) {
//			fatal_error = true;
			return;
		}
		scenario_addr = label_stack[--label_depth];
	} else {
		label_stack[label_depth++] = scenario_addr;
		scenario_addr = next_addr;
	}
}

void NACT::cmd_page_jump()
{
	int next_page = cali();
	load_scenario(next_page);
	scenario_page = next_page;
	scenario_addr = 2;

	output_console("\n&%d:", next_page);
}

void NACT::cmd_page_call()
{
	int next_page = cali(), next_addr;

	output_console("\n%%%d:", next_page);

	if(next_page == 0) {
		// ���^�[��
		if(page_depth == 0) {
//			fatal_error = true;
			return;
		}
		next_page = page_stack[--page_depth];
		next_addr = addr_stack[page_depth];
	} else {
		page_stack[page_depth] = scenario_page;
		addr_stack[page_depth++] = scenario_addr;
		next_addr = 2;
	}
	load_scenario(next_page);
	scenario_page = next_page;
	scenario_addr = next_addr;
}

void NACT::cmd_set_menu()
{
	if(ags->draw_menu) {
		ags->menu_dest_x = 2;
		ags->menu_dest_y += ags->menu_font_size + 2;
		ags->draw_menu = false;

		output_console("$");
	} else {
		if(!menu_index) {
			ags->clear_menu_window();
			ags->menu_dest_y = 0;
		}
		menu_addr[menu_index++] = getw();
		ags->menu_dest_x = 2;
		ags->menu_dest_y += 2;
		ags->draw_menu = true;

		output_console("\n$%x,", menu_addr[menu_index - 1]);
	}
}

void NACT::cmd_open_menu()
{
	output_console("\n]");

	if(!menu_index) {
		scenario_addr = scenario_data[0] | (scenario_data[1] << 8);
		return;
	}

	// ���j���[�\��
	ags->open_menu_window(menu_window);

	// �}�E�X�ړ�
	int sx = ags->menu_w[menu_window - 1].sx;
	int sy = ags->menu_w[menu_window - 1].sy;
	int ex = ags->menu_w[menu_window - 1].ex;
	int mx = ex - 16;
	int my = sy + 10;
	int height = ags->menu_font_size + 4;
	int current_index = 0;

	set_cursor(mx, my);
	wait_after_open_menu();

	// ���j���[�I��
	for(bool selectable = true;;) {
		// ���͑ҋ@
		int val = 0, current_mx = mx, current_my = my;
		for(;;) {
			if(params.terminate) {
				return;
			}
			if((val = get_key()) != 0) {
				Sleep(100);
				break;
			}
			get_cursor(&current_mx, &current_my);
			if(abs(my - current_my) > 3) {
				break;
			}
			Sleep(10);
		}
		if(val) {
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

		if(val == 0) {
			// �}�E�X����
			mx = current_mx; my = current_my;
			int index = (my - sy) / height;
			if(sx <= mx && mx <= ex && 0 <= index && index < menu_index) {
				current_index = index;
				ags->redraw_menu_window(menu_window, current_index);
				selectable = true;
			} else {
				selectable = false;
			}
		} else if(val == 1 || val == 2 || val == 4 || val == 8) {
			if(val == 1) {
				current_index = current_index ? current_index - 1 : menu_index - 1;
			} else if(val == 2) {
				current_index = (current_index < menu_index - 1) ? current_index + 1 : 0;
			} else if(val == 4) {
				current_index = 0;
			} else if(val == 8) {
				current_index = menu_index - 1;
			}
			ags->redraw_menu_window(menu_window, current_index);
			selectable = true;
		} else if(val == 16 && selectable) {
			break;
		} else if(val == 32) {
			current_index = -1;
			break;
		}
	}

	// ��ʍX�V
	ags->close_menu_window(menu_window);
	if(clear_text) {
		ags->clear_text_window(text_window, true);
	}

	if(current_index != -1) {
		scenario_addr = menu_addr[current_index];
	}
	menu_index = 0;
}

void NACT::cmd_set_verbobj()
{
	int verb = getd();
	int obj = getd();
	int addr = getw();

	menu_addr[menu_index] = addr;
	menu_verb[menu_index] = verb;
	menu_obj[menu_index++] = obj;
	verb_obj = true;

	output_console("\n[%x,%s,%s:", addr, caption_verb[verb], caption_obj[obj]);
}

void NACT::cmd_set_verbobj2()
{
	int condition = cali();
	int verb = getd();
	int obj = getd();
	int addr = getw();

	if(condition) {
		menu_addr[menu_index] = addr;
		menu_verb[menu_index] = verb;
		menu_obj[menu_index++] = obj;
	}
	verb_obj = true;

	output_console("\n:%d,%x,%s,%s:", condition, addr, caption_verb[verb], caption_obj[obj]);
}

void NACT::cmd_open_verb()
{
	// �������j���[�̕\��
	output_console("\nopen verb-obj menu");

	verb_obj = false;

	// �\�����铮���̃`�F�b�N
	int chk[MAX_VERB], page = 0;
	
	memset(chk, 0, sizeof(chk));
	for(int i = 0; i < menu_index; i++) {
		chk[menu_verb[i]] = 1;
	}

	// ���j���[���ڂ̏���
	int id[32], index = 0;

	ags->clear_menu_window();
	ags->menu_dest_y = 0;
	ags->draw_menu = true;

	for(int i = 0; i < MAX_VERB; i++) {
		if(chk[i]) {
			ags->menu_dest_x = 2;
			ags->menu_dest_y += 2;
			ags->draw_text(caption_verb[i]);
			id[index++] = i;
			ags->menu_dest_y += ags->menu_font_size + 2;
		}
	}
	ags->draw_menu = false;

	// ���j���[�\��
	ags->open_menu_window(menu_window);

	// �}�E�X�ړ�
	int sx = ags->menu_w[menu_window - 1].sx;
	int sy = ags->menu_w[menu_window - 1].sy;
	int ex = ags->menu_w[menu_window - 1].ex;
	int mx = ex - 16;
	int my = sy + 10;
	int height = ags->menu_font_size + 4;
	int current_index = 0;

	set_cursor(mx, my);
	wait_after_open_menu();

	// ���j���[�I��
	for(bool selectable = true;;) {
		// ���͑ҋ@
		int val = 0, current_mx = mx, current_my = my;
		for(;;) {
			if(params.terminate) {
				return;
			}
			if((val = get_key()) != 0) {
				Sleep(100);
				break;
			}
			get_cursor(&current_mx, &current_my);
			if(abs(my - current_my) > 3) {
				break;
			}
			Sleep(10);
		}
		if(val) {
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

		if(val == 0) {
			// �}�E�X����
			mx = current_mx; my = current_my;
			int mindex = (my - sy) / height;
			if(sx <= mx && mx <= ex && 0 <= mindex && mindex < index) {
				current_index = mindex;
				ags->redraw_menu_window(menu_window, current_index);
				selectable = true;
			} else {
				selectable = false;
			}
		} else if(val == 1 || val == 2 || val == 4 || val == 8) {
			if(val == 1) {
				current_index = current_index ? current_index - 1 : index - 1;
			} else if(val == 2) {
				current_index = (current_index < index - 1) ? current_index + 1 : 0;
			} else if(val == 4) {
				current_index = 0;
			} else if(val == 8) {
				current_index = index - 1;
			}
			ags->redraw_menu_window(menu_window, current_index);
			selectable = true;
		} else if(val == 16 && selectable) {
			break;
		} else if(val == 32) {
			current_index = -1;
			break;
		}
	}

	// ��ʍX�V
	ags->close_menu_window(menu_window);
	if(clear_text) {
		ags->clear_text_window(text_window, true);
	}

	if(current_index == -1) {
		scenario_addr = scenario_data[0] | (scenario_data[1] << 8);
	} else {
		cmd_open_obj(id[current_index]);
	}
	menu_index = 0;
}

void NACT::cmd_open_obj(int verb)
{
	// �ړI�ꃁ�j���[�̕\��
	verb_obj = false;

	// �\������ړI��̃`�F�b�N
	int chk[MAX_OBJ], addr[MAX_OBJ], page = 0;
	
	memset(chk, 0, sizeof(chk));
	for(int i = 0; i < menu_index; i++) {
		if(menu_verb[i] == verb) {
			chk[menu_obj[i]] = 1;
			addr[menu_obj[i]] = menu_addr[i];
		}
	}
	// �ړI�ꂪ�Ȃ��ꍇ
	if(chk[0]) {
		scenario_addr = addr[0];
		return;
	}
	// �Ȍ�Aobj=0�͖߂�Ƃ��Ĉ���
	chk[0] = 0;
	addr[0] = scenario_data[0] | (scenario_data[1] << 8);

	// ���j���[���ڂ̏���
	int id[32], index = 0;

	ags->clear_menu_window();
	ags->menu_dest_y = 0;
	ags->draw_menu = true;

	for(int i = 0; i < MAX_OBJ; i++) {
		if(chk[i]) {
			ags->menu_dest_x = 2;
			ags->menu_dest_y += 2;
			ags->draw_text(caption_obj[i]);
			id[index++] = i;
			ags->menu_dest_y += ags->menu_font_size + 2;
		}
	}
	// �߂��ǉ�
	ags->menu_dest_x = 2;
	ags->menu_dest_y += 2;
	ags->draw_text("�߂�");
	id[index++] = 0;
	ags->menu_dest_y += ags->menu_font_size + 2;
	ags->draw_menu = false;

	// ���j���[�\��
	ags->open_menu_window(menu_window);

	// �}�E�X�ړ�
	int sx = ags->menu_w[menu_window - 1].sx;
	int sy = ags->menu_w[menu_window - 1].sy;
	int ex = ags->menu_w[menu_window - 1].ex;
	int mx = ex - 16;
	int my = sy + 10;
	int height = ags->menu_font_size + 4;
	int current_index = 0;

	set_cursor(mx, my);
	wait_after_open_menu();

	// ���j���[�I��
	for(bool selectable = true;;) {
		// ���͑ҋ@
		int val = 0, current_mx = mx, current_my = my;
		for(;;) {
			if(params.terminate) {
				return;
			}
			if((val = get_key()) != 0) {
				Sleep(100);
				break;
			}
			get_cursor(&current_mx, &current_my);
			if(abs(my - current_my) > 3) {
				break;
			}
			Sleep(10);
		}
		if(val) {
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

		if(val == 0) {
			// �}�E�X����
			mx = current_mx; my = current_my;
			int mindex = (my - sy) / height;
			if(sx <= mx && mx <= ex && 0 <= mindex && mindex < index) {
				current_index = mindex;
				ags->redraw_menu_window(menu_window, current_index);
				selectable = true;
			} else {
				selectable = false;
			}
		} else if(val == 1 || val == 2 || val == 4 || val == 8) {
			if(val == 1) {
				current_index = current_index ? current_index - 1 : index - 1;
			} else if(val == 2) {
				current_index = (current_index < index - 1) ? current_index + 1 : 0;
			} else if(val == 4) {
				current_index = 0;
			} else if(val == 8) {
				current_index = index - 1;
			}
			ags->redraw_menu_window(menu_window, current_index);
			selectable = true;
		} else if(val == 16 && selectable) {
			break;
		} else if(val == 32) {
			current_index = -1;
			break;
		}
	}

	// ��ʍX�V
	ags->close_menu_window(menu_window);
	if(clear_text) {
		ags->clear_text_window(text_window, true);
	}

	if(current_index == -1) {
		scenario_addr = scenario_data[0] | (scenario_data[1] << 8);
	} else {
		scenario_addr = addr[id[current_index]];
	}
}

void NACT::cmd_a()
{
	output_console("A\n");

	if(!text_skip_enb) {
		// Push�}�[�N�̕\��
		if(show_push) {
			ags->draw_push(text_window);
		}

		// �L�[��������ė������܂őҋ@
		for(;;) {
			if(params.terminate) {
				return;
			}
			if(get_key()) {
				break;
			}
			Sleep(10);
		}
		Sleep(100);
		for(;;) {
			if(params.terminate) {
				return;
			}
			if(!(get_key() & 0x18)) {
				break;
			}
			Sleep(10);
		}
	}

	// �E�B���h�E�X�V
	ags->clear_text_window(text_window, true);
}

void NACT::cmd_b()
{
	int cmd = cali();
	int index = cali();
	int p1 = cali();
	int p2 = cali();
	int p3 = cali();
	int p4 = cali();
	int p5 = cali();

	output_console("\nB %d,%d,%d,%d,%d,%d,%d:", cmd, index, p1, p2, p3, p4, p5);

	if(cmd == 1) {
		if(crc32_a == CRC32_AYUMI_FD || crc32_a == CRC32_AYUMI_HINT || crc32_a == CRC32_DRSTOP) {
			p5 = 1;
		}
		ags->menu_w[index - 1].sx = p1 * 8;
		ags->menu_w[index - 1].sy = p2;
		ags->menu_w[index - 1].ex = p3 * 8 - 1;
		ags->menu_w[index - 1].ey = p4;
		ags->menu_w[index - 1].push = p5 ? true : false;

		// �ޔ���ʂ̔j��
		if(ags->menu_w[index - 1].screen) {
			free(ags->menu_w[index - 1].screen);
			ags->menu_w[index - 1].screen = NULL;
		}
	} else if(cmd == 2) {
		if(crc32_a == CRC32_AYUMI_FD || crc32_a == CRC32_AYUMI_HINT || crc32_a == CRC32_DRSTOP) {
			p1 = 1;
		}
//		if(crc32_a == CRC32_PROSTUDENTG_FD) {
//			ags->menu_w[index - 1].frame = (index == 1 || index == 3) ? true : false;
//		} else
		ags->menu_w[index - 1].frame = p1 ? true : false;
		menu_window = index;
	} else if(cmd == 3) {
		ags->text_w[index - 1].sx = p1 * 8;
		ags->text_w[index - 1].sy = p2;
		ags->text_w[index - 1].ex = p3 * 8 - 1;
		ags->text_w[index - 1].ey = p4;
		ags->text_w[index - 1].push = p5 ? true : false;

		// �ޔ���ʂ̔j��
		if(ags->text_w[index - 1].screen) {
			free(ags->text_w[index - 1].screen);
			ags->text_w[index - 1].screen = NULL;
		}
		// �ޔ𑋂̔j��
		if(ags->text_w[index - 1].window) {
			free(ags->text_w[index - 1].window);
			ags->text_w[index - 1].window = NULL;
		}
	} else if(cmd == 4) {
		if(crc32_a == CRC32_AYUMI_FD || crc32_a == CRC32_AYUMI_HINT) {
			p1 = 1;
			//p5 ? 0 : 1; // �t�H
			p5 = 0;
		} else if(crc32_a == CRC32_DRSTOP) {
			p1 = 1;
		}
		if(p5 == 0) {
			// �E�B���h�E�ޔ�
//			if(crc32_a == CRC32_PROSTUDENTG_FD) {
//				ags->text_w[index - 1].frame = (index == 1 || index == 3) ? true : false;
//			} else
			ags->text_w[index - 1].frame = p1 ? true : false;
			ags->open_text_window(index, p4 ? false : true);
			text_window = index;
		} else {
			// �E�B���h�E���A
			ags->close_text_window(index, text_window == index ? true : false);
		}
	}
}

void NACT::cmd_d()
{
	int p1 = cali();
	int p2 = cali();
	int p3 = cali();
	int p4 = cali();
	int p5 = cali();
	int p6 = cali();
	int p7 = cali();
	int p8 = cali();

	output_console("\nD %d,%d,%d,%d,%d,%d,%d,%d:", p1, p2, p3, p4, p5, p6, p7, p8);
}

void NACT::cmd_e()
{
	int p1 = cali();
	int p2 = cali();
	int p3 = cali();

	output_console("\nE %d,%d,%d:", p1, p2, p3);
}

void NACT::cmd_f()
{
	output_console("\nF");

	scenario_addr = 2;
}

void NACT::cmd_g()
{
	int page = cali();

	output_console("\nG %d:", page);

	if(crc32_a == CRC32_SDPS && (crc32_b == CRC32_SDPS_TONO || crc32_b == CRC32_SDPS_KAIZOKU)) {
		if(20 <= page && page <= 100) {
			page--;
		}
	}
	ags->load_cg(page, -1);

	if(crc32_a == CRC32_DALK_HINT) {
		if(page == 3) {
			WAIT(2000)
		}
	} else if(crc32_a == CRC32_RANCE3_HINT) {
		if(page == 25) {
			WAIT(2000)
		}
	}
}

void NACT::cmd_h()
{
	int length = getd();
	int val = cali();

	output_console("\nH %d,%d:", length, val);

	// 8���̕�����𐶐�
	char tmp[8];
	tmp[0] = '0';
	tmp[1] = '0';
	tmp[2] = '0';
	for(int i = 3, ref = 10000; i < 8; i++) {
		tmp[i] = '0' + (int)(val / ref);
		val %= ref;
		ref /= 10;
	}
	for(int i = 0; i < 7; i++) {
		if(tmp[i] != '0')
			break;
		tmp[i] = ' ';
	}

	// �w�肵�����������o��
	char string[9];
	int p = 0;
	if(length == 0) {
		for(int i = 0; i < 8; i++) {
			if(tmp[i] != ' ') {
				string[p++] = tmp[i];
			}
		}
	} else {
		for(int i = 8 - length; i < 8; i++) {
			string[p++] = tmp[i];
		}
	}
	string[p++] = '\0';
	ags->draw_text(string);
}

void NACT::cmd_i()
{
	int p1 = cali();
	int p2 = cali();
//	int p3 = getd();
	int p3 = cali();

	output_console("\nI %d,%d,%d:", p1, p2, p3);
}

void NACT::cmd_j()
{
	int p1 = cali();
	int p2 = cali();

	output_console("\nJ %d,%d:", p1, p2);
}

void NACT::cmd_k()
{
	// ���g�p
	cmd_r();
}

void NACT::cmd_l()
{
	int index = getd();

	output_console("\nL %d:", index);

	if(1 <= index && index <= 26) {
		// ASLEEP_A.DAT - ASLEEP_Z.DAT
		char file_name[_MAX_PATH];
		sprintf_s(file_name, _MAX_PATH, "ASLEEP_%c.DAT", 'A' + index - 1);

		FILEIO *fio = new FILEIO();
		if(fio->Fopen(file_name, FILEIO_READ_BINARY)) {
			fio->Fseek(112, FILEIO_SEEK_SET);

			int next_page = fio->Fgetw() - 1;
			fio->Fgetw();
			fio->Fgetw();	// cg no?
			fio->Fgetw();
			int next_music = fio->Fgetw();
			fio->Fgetw();
			int next_addr = fio->Fgetw();
			fio->Fgetw();
			for(int i = 0; i < 512; i++) {
				var[i] = fio->Fgetw();
			}
			ags->menu_font_size = fio->Fgetw();
			ags->text_font_size = fio->Fgetw();
			ags->palette_bank = fio->Fgetw();
			if(!ags->palette_bank) {
				ags->palette_bank = -1;
			}
			ags->text_font_color = fio->Fgetw();
			ags->menu_font_color = fio->Fgetw();
			ags->menu_frame_color = fio->Fgetw();
			ags->menu_back_color = fio->Fgetw();
			ags->text_frame_color = fio->Fgetw();
			ags->text_back_color = fio->Fgetw();
			for(int i = 0; i < 10; i++) {
				ags->menu_w[i].sx = fio->Fgetw();
				ags->menu_w[i].sy = fio->Fgetw();
				ags->menu_w[i].ex = fio->Fgetw();
				ags->menu_w[i].ey = fio->Fgetw();
				ags->menu_w[i].push = fio->Fgetw() ? true : false;
				ags->menu_w[i].frame = fio->Fgetw() ? true : false;
				fio->Fgetw();
				fio->Fgetw();

				if(ags->menu_w[i].screen) {
					free(ags->menu_w[i].screen);
				}
				ags->menu_w[i].screen = NULL;
				if(ags->menu_w[i].window) {
					free(ags->menu_w[i].window);
				}
				ags->menu_w[i].window = NULL;
			}
			for(int i = 0; i < 10; i++) {
				ags->text_w[i].sx = fio->Fgetw();
				ags->text_w[i].sy = fio->Fgetw();
				ags->text_w[i].ex = fio->Fgetw();
				ags->text_w[i].ey = fio->Fgetw();
				ags->text_w[i].push = fio->Fgetw() ? true : false;
				ags->text_w[i].frame = fio->Fgetw() ? true : false;
				fio->Fgetw();
				fio->Fgetw();

				if(ags->text_w[i].screen) {
					free(ags->text_w[i].screen);
				}
				ags->text_w[i].screen = NULL;
				if(ags->text_w[i].window) {
					free(ags->text_w[i].window);
				}
				ags->text_w[i].window = NULL;
			}
			for(int i = 0; i < 10; i++) {
				fio->Fread(tvar[i], 22, 1);
			}
			for(int i = 0; i < 30; i++) {
				for(int j = 0; j < 10; j++) {
					fio->Fread(tvar_stack[i][j], 22, 1);
				}
			}
			for(int i = 0; i < 30; i++) {
				for(int j = 0; j < 20; j++) {
					var_stack[i][j] = fio->Fgetw();
				}
			}
			fio->Fclose();

			load_scenario(next_page);
			scenario_page = next_page;
			scenario_addr = next_addr;

//			ags->old_text_font_size = 0;
//			ags->old__menu_font_fize = 0;
			mako->play_music(next_music);
		}
		delete fio;
	}
}

void NACT::cmd_m()
{
	char string[33];
	int d, p = 0;

	while((d = getd()) != ':') {
		if((0x81 <= d && d <= 0x9f) || 0xe0 <= d) {
			// 2bytes
			string[p++] = d;
			string[p++] = getd();
		} else {
			string[p++] = d;
		}
	}
	string[p] = '\0';

	output_console("\nM %s:", string);

	if(1 <= tvar_index && tvar_index <= 10) {
		memcpy(tvar[tvar_index - 1], string, 33);
	}
}

void NACT::cmd_n()
{
	int p1 = cali();
	int p2 = cali();

	output_console("\nN %d,%d:", p1, p2);
}

void NACT::cmd_o()
{
	int st = cali();
	int width = cali();
	int height = cali();

	output_console("\nO %d,%d,%d:", st, width, height);

#if 0
	// white mesh
	int sx = (st % 80) * 8;
	int sy = (int)(st / 80);
	ags->draw_mesh(sx, sy, width, height);
#endif
}

void NACT::cmd_p()
{
	int param = getd();

	if(crc32_a != CRC32_YAKATA2 && crc32_a != CRC32_DALK_HINT && crc32_a != CRC32_RANCE3_HINT) {
		ags->text_font_color = (uint8)((param & 0x7) + 16);
	}

	output_console("\nP %d:", param);
}

#define FWRITE(data, size) { \
	memcpy(&buffer[p], data, size); \
	p += size; \
}
#define FPUTW(data) { \
	uint16 tmp = (data); \
	buffer[p++] = tmp & 0xff; \
	buffer[p++] = (tmp >> 8) & 0xff; \
}

void NACT::cmd_q()
{
	static char header[112] = {
		'T', 'h', 'i', 's', ' ', 'i', 's', ' ', 's', 'a', 'v', 'e', ' ', 'd', 'a', 't',
		'a', ' ', 'f', 'o', 'r', ' ', 'S', 'y', 's', 't', 'e', 'm', '2', ' ', ' ', 'U',
		'n', 'i', 'o', 'n', ' ', '[', 'W', 'i', 'n', '9', 'x', '/', 'N', 'T', '4', '0',
		'/', '2', '0', '0', '0', '/', 'X', 'P', '/', 'C', 'E', ']', ' ', 'F', 'o', 'r',
		' ', 'N', 'A', 'C', 'T', '/', 'A', 'D', 'V', ' ', 's', 'y', 's', 't', 'e', 'm',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '(', 'C', ')', '2', '0', '0',
		'4', ' ', 'A', 'L', 'I', 'C', 'E', '-', 'S', 'O', 'F', 'T', ' ', ' ', ' ', ' '
	};

	int index = getd();

	output_console("\nQ %d:", index);

	if(1 <= index && index <= 26) {
		// ASLEEP_A.DAT - ASLEEP_Z.DAT
		char file_name[_MAX_PATH];
		sprintf_s(file_name, _MAX_PATH, "ASLEEP_%c.DAT", 'A' + index - 1);

		FILEIO *fio = new FILEIO();
		if(fio->Fopen(file_name, FILEIO_WRITE_BINARY)) {
			uint8 buffer[9510];
			int p = 0;

			FWRITE(header, 112);
			FPUTW(scenario_page + 1);
			FPUTW(0);
			FPUTW(0);	// cg no?
			FPUTW(0);
			FPUTW(mako->current_music);
			FPUTW(0);
			FPUTW(scenario_addr);
			FPUTW(0);
			for(int i = 0; i < 512; i++) {
				FPUTW(var[i]);
			}
			FPUTW(ags->menu_font_size);
			FPUTW(ags->text_font_size);
			FPUTW(ags->palette_bank == -1 ? 0 : ags->palette_bank);
			FPUTW(ags->text_font_color);
			FPUTW(ags->menu_font_color);
			FPUTW(ags->menu_frame_color);
			FPUTW(ags->menu_back_color);
			FPUTW(ags->text_frame_color);
			FPUTW(ags->text_back_color);
			for(int i = 0; i < 10; i++) {
				FPUTW(ags->menu_w[i].sx);
				FPUTW(ags->menu_w[i].sy);
				FPUTW(ags->menu_w[i].ex);
				FPUTW(ags->menu_w[i].ey);
				FPUTW(ags->menu_w[i].push ? 1 : 0);
				FPUTW(ags->menu_w[i].frame ? 1 : 0);
				FPUTW(0);
				FPUTW(0);
			}
			for(int i = 0; i < 10; i++) {
				FPUTW(ags->text_w[i].sx);
				FPUTW(ags->text_w[i].sy);
				FPUTW(ags->text_w[i].ex);
				FPUTW(ags->text_w[i].ey);
				FPUTW(ags->text_w[i].push ? 1 : 0);
				FPUTW(ags->text_w[i].frame ? 1 : 0);
				FPUTW(0);
				FPUTW(0);
			}
			for(int i = 0; i < 10; i++) {
				FWRITE(tvar[i], 22);
			}
			for(int i = 0; i < 30; i++) {
				for(int j = 0; j < 10; j++) {
					FWRITE(tvar_stack[i][j], 22);
				}
			}
			for(int i = 0; i < 30; i++) {
				for(int j = 0; j < 20; j++) {
					FPUTW(var_stack[i][j]);
				}
			}

			fio->Fwrite(buffer, 9510, 1);
			fio->Fclose();
		}
		delete fio;
	}
}

void NACT::cmd_r()
{
	output_console("R\n");

	// �E�B���h�E�̕\���͈͊O�̏ꍇ�͉��y�[�W
	if(ags->return_text_line(text_window)) {
		cmd_a();
	}
}

void NACT::cmd_s()
{
	int page = getd();

	output_console("\nS %d:", page);

	if(page) {
		mako->play_music(page);
	} else {
		mako->stop_music();
	}
}

void NACT::cmd_t()
{
	int p1 = cali();
	int p2 = cali();
	int p3 = cali();

	output_console("\nT %d,%d,%d:", p1, p2, p3);
}

void NACT::cmd_u()
{
	int page, transparent;

	if(crc32_a == CRC32_YAKATA2) {
		page = cali();
		transparent = cali();
	} else {
		page = getd();
		transparent = getd();
	}

	output_console("\nU %d,%d:", page, transparent);

	ags->load_cg(page, transparent);
}

void NACT::cmd_v()
{
#if 1
	int p01 = cali();
	int p02 = cali();
	int p03 = cali();
	int p04 = cali();
	int p05 = cali();
	int p06 = cali();
	int p07 = cali();
	int p08 = cali();
	int p09 = cali();
	int p10 = cali();
	int p11 = cali();
	int p12 = cali();
	int p13 = cali();
	int p14 = cali();
	int p15 = cali();
	int p16 = cali();
	int p17 = cali();
	int p18 = cali();
	int p19 = cali();
	int p20 = cali();
	int p21 = cali();
	int p22 = cali();
	int p23 = cali();
	int p24 = cali();
	int p25 = cali();
	int p26 = cali();
	int p27 = cali();
	int p28 = cali();
	int p29 = cali();

	output_console("\nV %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d:",
	p01, p02, p03, p04, p05, p06, p07, p08, p09, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29);
#else
	int p1 = cali();
	int p2 = cali();

	output_console("\nV %d,%d:", p1, p2);
#endif
}

void NACT::cmd_w()
{
	int sx = cali();
	int sy = cali();
	int ex = cali();
	int ey = cali();

	output_console("\nW %d,%d,%d,%d:", sx, sy, ex, ey);

	ags->draw_mesh(sx, sy, ex - sx, ey - sy);
}

void NACT::cmd_x()
{
	int index = getd();

	output_console("\nX %d:", index);

	if(1 <= index && index <= 10) {
		ags->draw_text(tvar[index - 1]);
	}
}

void NACT::cmd_y()
{
	int cmd = cali();
	int param = cali();

	output_console("\nY %d,%d:", cmd, param);

	switch(cmd) {
		case 1:
			ags->clear_text_window(text_window, (param == 0) ? true : false);
			break;
		case 2:
			if(param == 0) {
				for(int i = 1; i <= 20; i++) {
					var[i] = 0;
				}
			} else if(param == 1) {
				for(int i = 21; i <= 40; i++) {
					var[i] = 0;
				}
			} else if(param == 2) {
				for(int i = 41; i <= 56; i++) {
					var[i] = 0;
				}
			}
			break;
		case 3:
			{
				DWORD dwTime = timeGetTime() + param * 1000 / 60;
				for(;;) {
					if(params.terminate) {
						return;
					}
//					RND = get_key();
//					if(RND && wait_keydown) {
//						break;
//					}
					if(dwTime <= timeGetTime()) {
						break;
					}
					Sleep(10);
				}
			}
			break;
		case 4:
			RND = (param == 0 || param == 1) ? 0 : random(param);
			break;
		case 7:
			if(!(crc32_a == CRC32_SDPS && crc32_b == CRC32_SDPS_MARIA)) {
				ags->draw_box(param);
			}
			break;
		case 8:
			tvar_index = param;
			break;
		case 14:
			RND = 1;
			break;
		case 25:
			ags->menu_font_size = (param == 1) ? 16 : (param == 2) ? 24 : (param == 3) ? 32 : (param == 4) ? 48 : (param == 5) ? 64 : 16;
			break;
		case 26:
			ags->text_font_size = (param == 1) ? 16 : (param == 2) ? 24 : (param == 3) ? 32 : (param == 4) ? 48 : (param == 5) ? 64 : 16;
			break;
		case 27:
			{
				int tmp = tvar_index;
				tvar_index = param;
				tvar_maxlen = 8;//param;
				DialogBoxParam(g_hinst, MAKEINTRESOURCE(IDD_DIALOG1), g_hwnd, TextDialogProc, (LPARAM)this);
				tvar_index = tmp;
			}
			break;
		case 40:
		case 42:
			if(ags->now_fade()) {
				DWORD dwStart = timeGetTime();
				for(int i = 0; i < 16; i++) {
					ags->fade_in(i);
					DWORD dwTime = dwStart + param * 1000 / 60 * i;
					for(;;) {
						if(params.terminate) {
							return;
						}
						if(dwTime <= timeGetTime()) {
							break;
						}
						Sleep(0);
					}
				}
				ags->fade_end();
			}
			break;
		case 41:
		case 43:
			if(!ags->now_fade()) {
				ags->fade_start();
				DWORD dwStart = timeGetTime();
				for(int i = 0; i < 16; i++) {
					ags->fade_out(i, (cmd == 41) ? false : true);
					DWORD dwTime = dwStart + param * 1000 / 60 * i;
					for(;;) {
						if(params.terminate) {
							return;
						}
						if(dwTime <= timeGetTime()) {
							break;
						}
						Sleep(0);
					}
				}
			}
			break;
		case 221:
		case 222:
		case 223:
		case 224:
		case 225:
		case 226:
		case 227:
		case 228:
		case 229:
			if(1 <= param && param <= 10) {
				char string[22];
				int len = cmd - 220, p = 0, q = 0;
				uint8 d;
				while((d = (uint8)tvar[param - 1][p]) != '\0') {
					if((0x81 <= d && d <= 0x9f) || 0xe0 <= d) {
						string[p++] = tvar[param - 1][p];
					}
					string[p++] = tvar[param - 1][p];
					q++;
				}
				for(int i = q; i < len; i++) {
					string[p++] = 0x20;
				}
				string[p] = '\0';
				ags->draw_text(string);
			}
			break;
		case 252:
			RND = 8;
			break;
		case 253:
			// �L�[��������ė������܂őҋ@
			for(;;) {
				if(params.terminate) {
					return;
				}
				if(get_key()) {
					break;
				}
				Sleep(10);
			}
			Sleep(100);
			for(;;) {
				if(params.terminate) {
					return;
				}
				if(!(get_key() & 0x18)) {
					break;
				}
				Sleep(10);
			}
			break;
		case 254:
			RND = 0;
			break;
		case 255:
			post_quit = (param == 1) ? true : false;
			fatal_error = true;
			break;
	}
// Y1
// Y2
// Y3
// Y7
// Y8
// Y11
// Y23
// Y25
// Y26
// Y27
// Y40
// Y50
// Y252
// Y254
// Y255
}

void NACT::cmd_z()
{
	int cmd = cali();
	int param = cali();

	output_console("\nZ %d,%d:", cmd, param);

// Z1
// Z2
// Z3
// Z4
// Z5
}


// ���ʊ֐�

uint16 NACT::cali()
{
	uint32 cali[256];
	int p = 1;
	fatal_error = true;

	while(p > 0) {
		uint8 dat = getd();

		if(0x80 <= dat && dat <= 0xbf) {
			cali[p++] = var[dat & 0x3f];
		} else if(0xc0 <= dat && dat <= 0xff) {
			cali[p++] = var[((dat & 0x3f) << 8) | getd()];
		} else if(0x00 <= dat && dat <= 0x3f) {
			cali[p++] = ((dat & 0x3f) << 8) | getd();
		} else if(0x40 <= dat && dat <= 0x76) {
			cali[p++] = dat & 0x3f;
		} else if(dat == 0x77) {
			cali[p - 2] = cali[p - 2] * cali[p - 1];
			if (cali[p - 2] > 65535) {
				cali[p - 2] = 65535;
			}
			p--;
		} else if(dat == 0x78) {
			if(cali[p - 1]) {
				cali[p - 2] = cali[p - 2] / cali[p - 1];
			} else {
				cali[p - 2] = 0;
			}
			p--;
		} else if(dat == 0x79) {
			cali[p - 2] = cali[p - 2] + cali[p - 1];
			if (cali[p - 2] > 65535) {
				cali[p - 2] = 65535;
			}
			p--;
		} else if(dat == 0x7a) {
			if(cali[p - 2] > cali[p - 1]) {
				cali[p - 2] = cali[p - 2] - cali[p - 1];
			} else {
				cali[p - 2] = 0;
			}
			p--;
		} else if(dat == 0x7b) {
			cali[p - 2] = (cali[p - 2] == cali[p - 1]) ? 1 : 0;
			p--;
		} else if(dat == 0x7c) {
			cali[p - 2] = (cali[p - 2] < cali[p - 1]) ? 1 : 0;
			p--;
		}else if(dat == 0x7d) {
			cali[p - 2] = (cali[p - 2] > cali[p - 1]) ? 1 : 0;
			p--;
		} else if(dat == 0x7e) {
			cali[p - 2] = (cali[p - 2] != cali[p - 1]) ? 1 : 0;
			p--;
		} else if(dat == 0x7f) {
			if(p == 2) {
				fatal_error = false;
			}
			p = 0;
		}
	}
	return (uint16)(cali[1] & 0xffff);
}

uint16 NACT::cali2()
{
	uint16 val = 0;
	uint16 dat = getd();

	if(0x80 <= dat && dat <= 0xbf) {
		val = dat & 0x3f;
	} else if(0xc0 <= dat && dat <= 0xff) {
		val = ((dat & 0x3f) << 8) | getd();
	} else {
		fatal_error = true;
	}
	if(getd() != 0x7f) {
		fatal_error = true;
	}
	return val;
}

uint8 NACT::getd()
{
	return scenario_data[scenario_addr++];
}

uint16 NACT::getw()
{
	uint16 val = scenario_data[scenario_addr] | (scenario_data[scenario_addr + 1] << 8);
	scenario_addr += 2;
	return val;
}

