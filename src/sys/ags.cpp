/*
	ALICE SOFT SYSTEM 3 for Win32

	[ AGS ]
*/

#include "ags.h"
#include "crc32.h"
#include "../fileio.h"

extern HWND g_hwnd;

#define SET_TEXT(n, x1, y1, x2, y2, f) { \
	text_w[n].sx = x1; \
	text_w[n].sy = y1; \
	text_w[n].ex = x2; \
	text_w[n].ey = y2; \
	text_w[n].frame = f; \
}
#define SET_MENU(n, x1, y1, x2, y2, f) { \
	menu_w[n].sx = x1; \
	menu_w[n].sy = y1; \
	menu_w[n].ex = x2; \
	menu_w[n].ey = y2; \
	menu_w[n].frame = f; \
}
#define SET_BOX(n, c, x1, y1, x2, y2) { \
	box[n].color = c; \
	box[n].sx = x1; \
	box[n].sy = y1; \
	box[n].ex = x2; \
	box[n].ey = y2; \
}

AGS::AGS(NACT *parent) : nact(parent)
{
	// GDI+������
	GdiplusStartup(&gdiToken, &gdiSI, NULL);

	HDC hdc = GetDC(g_hwnd);

	// DIBSection 8bpp * 3 (�\, ��, ���j���[)
	for(int i = 0; i < 3; i++) {
		lpBufScreen[i] = (LPBYTE)GlobalAlloc(GPTR, sizeof(BITMAPINFO));
		lpDibScreen[i] = (LPBITMAPINFO)lpBufScreen[i];
		lpDibScreen[i]->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		lpDibScreen[i]->bmiHeader.biWidth = 640;
		lpDibScreen[i]->bmiHeader.biHeight = 480;
		lpDibScreen[i]->bmiHeader.biPlanes = 1;
		lpDibScreen[i]->bmiHeader.biBitCount = 32;
		lpDibScreen[i]->bmiHeader.biCompression = BI_RGB;
		lpDibScreen[i]->bmiHeader.biSizeImage = 0;
		lpDibScreen[i]->bmiHeader.biXPelsPerMeter = 0;
		lpDibScreen[i]->bmiHeader.biYPelsPerMeter = 0;
		lpDibScreen[i]->bmiHeader.biClrUsed = 0;
		lpDibScreen[i]->bmiHeader.biClrImportant = 0;
		hBmpScreen[i] = CreateDIBSection(hdc, lpDibScreen[i], DIB_RGB_COLORS, (PVOID *)&lpBmpScreen[i], NULL, 0);
		hdcDibScreen[i] = CreateCompatibleDC(hdc);
		SelectObject(hdcDibScreen[i], hBmpScreen[i]);
		memset(lpBmpScreen[i], 0, 640 * 480 * sizeof(DWORD));

		// ���zVRAM�ւ̃|�C���^�擾
		for(int j = 0; j < 480; j++) {
			vram[i][j] = &lpBmpScreen[i][640 * (479 - j)];
		}
	}

	// DIBSection 24bpp * 1 (�ŏI�o�͐�)
	lpBufDest = (LPBYTE)GlobalAlloc(GPTR, sizeof(BITMAPINFO));
	lpDibDest = (LPBITMAPINFO)lpBufDest;
	lpDibDest->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpDibDest->bmiHeader.biWidth = 640;
	lpDibDest->bmiHeader.biHeight = 480;
	lpDibDest->bmiHeader.biPlanes = 1;
	lpDibDest->bmiHeader.biBitCount = 32;
	lpDibDest->bmiHeader.biCompression = BI_RGB;
	lpDibDest->bmiHeader.biSizeImage = 0;
	lpDibDest->bmiHeader.biXPelsPerMeter = 0;
	lpDibDest->bmiHeader.biYPelsPerMeter = 0;
	lpDibDest->bmiHeader.biClrUsed = 0;
	lpDibDest->bmiHeader.biClrImportant = 0;
	hBmpDest = CreateDIBSection(hdc, lpDibDest, DIB_RGB_COLORS, (PVOID *)&lpBmpDest, NULL, 0);
	hdcDibDest = CreateCompatibleDC(hdc);
	SelectObject(hdcDibDest, hBmpDest);
	memset(lpBmpDest, 0, 640 * 480 * sizeof(DWORD));

	// DIBSection 24bpp * 1 (�e�L�X�g�`��)
	lpBufText = (LPBYTE)GlobalAlloc(GPTR, sizeof(BITMAPINFO));
	lpDibText = (LPBITMAPINFO)lpBufText;
	lpDibText->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpDibText->bmiHeader.biWidth = 64;
	lpDibText->bmiHeader.biHeight = 64;
	lpDibText->bmiHeader.biPlanes = 1;
	lpDibText->bmiHeader.biBitCount = 32;
	lpDibText->bmiHeader.biCompression = BI_RGB;
	lpDibText->bmiHeader.biSizeImage = 0;
	lpDibText->bmiHeader.biXPelsPerMeter = 0;
	lpDibText->bmiHeader.biYPelsPerMeter = 0;
	lpDibText->bmiHeader.biClrUsed = 0;
	lpDibText->bmiHeader.biClrImportant = 0;
	hBmpText = CreateDIBSection(hdc, lpDibText, DIB_RGB_COLORS, (PVOID *)&lpBmpText, NULL, 0);
	hdcDibText = CreateCompatibleDC(hdc);
	SelectObject(hdcDibText, hBmpText);
//	memset(lpBmpText, 0, 64 * 64 * sizeof(DWORD));

	// DIBSection 24bpp * 1 (���j���[�`��)
	lpBufMenu = (LPBYTE)GlobalAlloc(GPTR, sizeof(BITMAPINFO));
	lpDibMenu = (LPBITMAPINFO)lpBufMenu;
	lpDibMenu->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpDibMenu->bmiHeader.biWidth = 64;
	lpDibMenu->bmiHeader.biHeight = 64;
	lpDibMenu->bmiHeader.biPlanes = 1;
	lpDibMenu->bmiHeader.biBitCount = 32;
	lpDibMenu->bmiHeader.biCompression = BI_RGB;
	lpDibMenu->bmiHeader.biSizeImage = 0;
	lpDibMenu->bmiHeader.biXPelsPerMeter = 0;
	lpDibMenu->bmiHeader.biYPelsPerMeter = 0;
	lpDibMenu->bmiHeader.biClrUsed = 0;
	lpDibMenu->bmiHeader.biClrImportant = 0;
	hBmpMenu = CreateDIBSection(hdc, lpDibMenu, DIB_RGB_COLORS, (PVOID *)&lpBmpMenu, NULL, 0);
	hdcDibMenu = CreateCompatibleDC(hdc);
	SelectObject(hdcDibMenu, hBmpMenu);
//	memset(lpBmpMenu, 0, 64 * 64 * sizeof(DWORD));

	ReleaseDC(g_hwnd, hdc);

	// �t�H���g
	LOGFONT logfont;
	logfont.lfEscapement = 0;
	logfont.lfOrientation = 0;
	logfont.lfWeight = FW_NORMAL;
	logfont.lfItalic = FALSE;
	logfont.lfUnderline = FALSE;
	logfont.lfStrikeOut = FALSE;
	logfont.lfCharSet = SHIFTJIS_CHARSET;
	logfont.lfOutPrecision = OUT_TT_PRECIS;
	logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logfont.lfQuality = NONANTIALIASED_QUALITY; //DEFAULT_QUALITY;
	logfont.lfPitchAndFamily = FIXED_PITCH | FF_DONTCARE;
	_tcscpy_s(logfont.lfFaceName, LF_FACESIZE, _T("�l�r �S�V�b�N"));

	logfont.lfHeight = 16;
	logfont.lfWidth = 8;
	hFont16 = CreateFontIndirect(&logfont);
	logfont.lfHeight = 24;
	logfont.lfWidth = 12;
	hFont24 = CreateFontIndirect(&logfont);
	logfont.lfHeight = 32;
	logfont.lfWidth = 16;
	hFont32 = CreateFontIndirect(&logfont);
	logfont.lfHeight = 48;
	logfont.lfWidth = 24;
	hFont48 = CreateFontIndirect(&logfont);
	logfont.lfHeight = 64;
	logfont.lfWidth = 32;
	hFont64 = CreateFontIndirect(&logfont);

	// �J�[�\��������
	for(int i = 0; i < 10; i++) {
		hCursor[i] = NULL;
	}
	hArrow = LoadCursor(NULL, IDC_ARROW);

	// GAIJI.DAT�ǂݍ���
	memset(gaiji, 0, sizeof(gaiji));

	FILEIO *fio = new FILEIO();

	if(fio->Fopen("GAIJI.DAT", FILEIO_READ_BINARY)) {
		int d1, d2;
		while((d1 = fio->Fgetc()) != EOF) {
			d2 = fio->Fgetc();
			// jis to sjis
			d1 += (d2 & 1) ? 0x1f : 0x7d;
			d1 += (d1 > 0x7f) ? 1 : 0;
			d2 = ((d2 - 0x21) >> 1) + 0x81;
			d2 += (d2 > 0x9f) ? 0x40 : 0;
			uint16 code = (d1 & 0xff) | ((d2 & 0xff) << 8);

			if(0xeb9f <= code && code <= 0xebfc) {
				for(int i = 0; i < 32; i++) {
					gaiji[code - 0xeb9f][i] = fio->Fgetc();
				}
			} else if(0xec40 <= code && code <= 0xec9e) {
				for(int i = 0; i < 32; i++) {
					gaiji[code - 0xec40 + 94][i] = fio->Fgetc();
				}
			} else {
				fio->Fseek(32, FILEIO_SEEK_CUR);
			}
		}
		fio->Fclose();
	}
	delete fio;

	// SYSTEM3 ������

	// ACG.DAT
	strcpy_s(acg, 16, "ACG.DAT");

	// �p���b�g
	memset(program_palette, 0, sizeof(program_palette));
	program_palette[0x00] = SETPALETTE16(0x0, 0x0, 0x0);
	program_palette[0x01] = SETPALETTE16(0x0, 0x0, 0xa);
	program_palette[0x02] = SETPALETTE16(0xa, 0x0, 0x0);
	program_palette[0x03] = SETPALETTE16(0xa, 0x0, 0xa);
	program_palette[0x04] = SETPALETTE16(0x0, 0x0, 0x0);
	program_palette[0x05] = SETPALETTE16(0x0, 0xa, 0xa);
	program_palette[0x06] = SETPALETTE16(0xa, 0xa, 0x0);
	program_palette[0x07] = SETPALETTE16(0xd, 0xd, 0xd);
	program_palette[0x08] = SETPALETTE16(0x7, 0x7, 0x7);
	program_palette[0x09] = SETPALETTE16(0x0, 0x0, 0xf);
	program_palette[0x0a] = SETPALETTE16(0xf, 0x0, 0x0);
	program_palette[0x0b] = SETPALETTE16(0xf, 0x0, 0xf);
	program_palette[0x0c] = SETPALETTE16(0x0, 0xf, 0x0);
	program_palette[0x0d] = SETPALETTE16(0x0, 0xf, 0xf);
	program_palette[0x0e] = SETPALETTE16(0xf, 0xf, 0x0);
	program_palette[0x0f] = SETPALETTE16(0xf, 0xf, 0xf);
#if defined(_SYSTEM1)
	program_palette[0x10] = SETPALETTE16(0x0, 0x0, 0x0);
	program_palette[0x11] = SETPALETTE16(0x0, 0x0, 0xf);
	program_palette[0x12] = SETPALETTE16(0xf, 0x0, 0x0);
	program_palette[0x13] = SETPALETTE16(0xf, 0x0, 0xf);
	program_palette[0x14] = SETPALETTE16(0x0, 0xf, 0x0);
	program_palette[0x15] = SETPALETTE16(0x0, 0xf, 0xf);
	program_palette[0x16] = SETPALETTE16(0xf, 0xf, 0x0);
	program_palette[0x17] = SETPALETTE16(0xf, 0xf, 0xf);
	program_palette[0x18] = SETPALETTE16(0x0, 0x0, 0x0);
	program_palette[0x19] = SETPALETTE16(0x0, 0x0, 0xf);
	program_palette[0x1a] = SETPALETTE16(0xf, 0x0, 0x0);
	program_palette[0x1b] = SETPALETTE16(0xf, 0x0, 0xf);
	program_palette[0x1c] = SETPALETTE16(0x0, 0xf, 0x0);
	program_palette[0x1d] = SETPALETTE16(0x0, 0xf, 0xf);
	program_palette[0x1e] = SETPALETTE16(0xf, 0xf, 0x0);
#endif
	program_palette[0x1f] = SETPALETTE16(0xf, 0xf, 0xf);
	program_palette[0x2f] = SETPALETTE16(0xf, 0xf, 0xf);
	program_palette[0x3f] = SETPALETTE16(0xf, 0xf, 0xf);
	program_palette[0x4f] = SETPALETTE16(0xf, 0xf, 0xf);
	program_palette[0x5f] = SETPALETTE16(0xf, 0xf, 0xf);
	program_palette[0x6f] = SETPALETTE16(0xf, 0xf, 0xf);
	program_palette[0x7f] = SETPALETTE16(0xf, 0xf, 0xf);
	program_palette[0x8f] = SETPALETTE16(0xf, 0xf, 0xf);
	program_palette[0x9f] = SETPALETTE16(0xf, 0xf, 0xf);
	program_palette[0xaf] = SETPALETTE16(0xf, 0xf, 0xf);
	program_palette[0xbf] = SETPALETTE16(0xf, 0xf, 0xf);
	program_palette[0xcf] = SETPALETTE16(0xf, 0xf, 0xf);
	program_palette[0xdf] = SETPALETTE16(0xf, 0xf, 0xf);
	program_palette[0xef] = SETPALETTE16(0xf, 0xf, 0xf);
	program_palette[0xff] = SETPALETTE16(0xf, 0xf, 0xf);
	memcpy(screen_palette, program_palette, sizeof(program_palette));

	// B�R�}���h
	for(int i = 0; i < 10; i++) {
		// �E�B���h�E�̏����ʒu�̓V�X�e���ɂ���ĈقȂ�
#if defined(_SYSTEM1)
		if(nact->crc32_a == CRC32_BUNKASAI) {
			SET_TEXT(i, 24, 304, 616, 384, false);
			SET_MENU(i, 440, 18, 620, 178, true);
		} else if(nact->crc32_a == CRC32_CRESCENT) {
			SET_TEXT(i, 24, 288, 616, 378, false);
			// �{���͉����j���[
			SET_MENU(i, 464, 50, 623, 240, true);
		} else if(nact->crc32_a == CRC32_DPS || nact->crc32_a == CRC32_DPS_SG || nact->crc32_a == CRC32_DPS_SG2 || nact->crc32_a == CRC32_DPS_SG3) {
			SET_TEXT(i, 48, 288, 594, 393, false);
			//SET_MENU(i, 48, 288, 584, 393, false);
			SET_MENU(i, 48, 288, 594, 393, false);
		} else if(nact->crc32_a == CRC32_FUKEI) {
			SET_TEXT(i, 44, 282, 593, 396, false);
			SET_MENU(i, 460, 14, 635, 214, false);
		} else if(nact->crc32_a == CRC32_INTRUDER) {
			SET_TEXT(i, 8, 280, 629, 393, false);
			SET_MENU(i, 448, 136, 623, 340, true);
		} else if(nact->crc32_a == CRC32_TENGU) {
			SET_TEXT(i, 44, 282, 593, 396, false);
			SET_MENU(i, 452, 14, 627, 214, false);
		} else if(nact->crc32_a == CRC32_TOUSHIN_HINT) {
			SET_TEXT(i, 8, 311, 623, 391, false);
			SET_MENU(i, 452, 14, 627, 214, true);
		} else if(nact->crc32_a == CRC32_VAMPIRE) {
			SET_TEXT(i, 8, 255, 615, 383, false);
			SET_MENU(i, 448, 11, 615, 224, false);
		} else if(nact->crc32_a == CRC32_YAKATA) {
			SET_TEXT(i, 48, 288, 594, 393, false);
			SET_MENU(i, 452, 14, 627, 214, false);
		} else
#elif defined(_SYSTEM2)
		if(nact->crc32_a == CRC32_DALK_HINT) {
			SET_TEXT(i, 24, 308, 376, 386, false);
			SET_MENU(i, 404, 28, 604, 244, true);
		} else if(nact->crc32_a == CRC32_RANCE3_HINT) {
			SET_TEXT(i, 104, 304, 615, 383, false);
			SET_MENU(i, 464, 24, 623, 200, true);
		} else if(nact->crc32_a == CRC32_YAKATA2) {
			SET_TEXT(i, 104, 304, 620, 382, false);
			SET_MENU(i, 420, 28, 620, 244, true);
		} else
#endif
		{
			SET_TEXT(i, 8, 311, 623, 391, true);
			SET_MENU(i, 464, 80, 623, 240, true);
		}
		text_w[i].push = false;
		text_w[i].screen = NULL;
		text_w[i].window = NULL;
		menu_w[i].push = true;
		menu_w[i].screen = NULL;
		menu_w[i].window = NULL;
	}

	// E�R�}���h
	for(int i = 0; i < 20; i++) {
//		SET_BOX(i, 0, 0, 0, 631, 399);
		SET_BOX(i, 0, 0, 0, 639, 399);
	}
#if defined(_SYSTEM2)
	if(nact->crc32_a == CRC32_SDPS && (nact->crc32_b == CRC32_SDPS_TONO || nact->crc32_b == CRC32_SDPS_KAIZOKU)) {
		SET_BOX(0, 0, 40, 8, 598, 271);
	} else if(nact->crc32_a == CRC32_PROSTUDENTG_FD) {
		SET_BOX(0, 0, 64, 13, 407, 289);
		SET_BOX(1, 0, 24, 298, 111, 390);
		SET_BOX(2, 0, 0, 0, 639, 307);
		SET_BOX(3, 0, 0, 0, 319, 399);
		SET_BOX(4, 0, (16*8), 310, ((77*8)-1), 390);
		SET_BOX(5, 0, (4*8), 310, ((76*8)-1), 390);
		SET_BOX(6, 0, (2*8), 317, ((56*8)-1), 389);
		SET_BOX(7, 15, 64, 13, 407, 289);
		SET_BOX(8, 0, 0, 0, 319, 399);
		SET_BOX(9, 0, 320, 0, 639, 40);
	}
#endif

	// ��ʑI��
	src_screen = dest_screen = 0;

	// ��ʃT�C�Y
	screen_height = 400;
	scroll = 400;

	// ���b�Z�[�W�\��
	text_dest_x = text_w[0].sx;
	text_dest_y = text_w[0].sy + 2;
	text_space = 2;
	text_font_size = 16;
	old_text_font_size = 0;
#if defined(_SYSTEM1)
	text_font_color = 15 + 16;
	text_frame_color = 15 + 16;
	text_back_color = 0 + 16;
#else
	text_font_color = 15;
	text_frame_color = 15;
	text_back_color = 0;
#endif

	// ���j���[�\��
	menu_dest_x = 2;
	menu_dest_y = 0;
	menu_font_size = 16;
	old_menu_font_size = 0;
#if defined(_SYSTEM1)
	menu_font_color = 15 + 16;
	menu_frame_color = 15 + 16;
	menu_back_color = 0 + 16;
#else
	menu_font_color = 15;
	menu_frame_color = 15;
	menu_back_color = 0;
#endif
	menu_fix = false;

	draw_hankaku = false;
	draw_menu = false;

	// CG�\��
	set_cg_dest = false;
	cg_dest_x = cg_dest_y = 0;
	for(int i = 0; i < MAX_CG; i++) {
		extract_palette_cg[i] = true;
	}
	get_palette = extract_palette = extract_cg = true;
	palette_bank = -1;

	// �}�E�X�J�[�\��
	cursor_color = 15;
	cursor_index = 0;

	// �t�F�[�h���
	fader = false;
	memset(fader_screen, 0, sizeof(fader_screen));
}

AGS::~AGS()
{
	// �ޔ�̈�̊J��
	for(int i = 0; i < 10; i++) {
		if(menu_w[i].screen) {
			free(menu_w[i].screen);
		}
		if(menu_w[i].window) {
			free(menu_w[i].window);
		}
		if(text_w[i].screen) {
			free(text_w[i].screen);
		}
		if(text_w[i].window) {
			free(text_w[i].window);
		}
	}

	// �J�[�\���J��
	for(int i = 0; i < 10; i++) {
		if(hCursor[i]) {
			DestroyCursor(hCursor[i]);
		}
	}

	// �t�H���g�J��
	DeleteObject(hFont16);
	DeleteObject(hFont24);
	DeleteObject(hFont32);
	DeleteObject(hFont48);
	DeleteObject(hFont64);

	// DIBSection�J��
	for(int i = 0; i < 3; i++) {
		DeleteDC(hdcDibScreen[i]);
		DeleteObject(hBmpScreen[i]);
		GlobalFree(lpBufScreen[i]);
	}

	DeleteDC(hdcDibDest);
	DeleteObject(hBmpDest);
	GlobalFree(lpBufDest);

	DeleteDC(hdcDibText);
	DeleteObject(hBmpText);
	GlobalFree(lpBufText);

	DeleteDC(hdcDibMenu);
	DeleteObject(hBmpMenu);
	GlobalFree(lpBufMenu);

	// GDI+�J��
	GdiplusShutdown(gdiToken);
}

void AGS::set_palette(int index, int r, int g, int b)
{
	if(index < 16) {
		screen_palette[index] = program_palette[index] = SETPALETTE16(r, g, b);
	} else {
		screen_palette[index] = program_palette[index] = SETPALETTE256(r, g, b);
	}
}

uint8 AGS::get_pixel(int dest, int x, int y)
{
	if(vram[dest][y][x] & 0x80000000) {
		return 0;
	} else {
		return (uint8)vram[dest][y][x];
	}
}

void AGS::set_pixel(int dest, int x, int y, uint8 color)
{
	vram[dest][y][x] = color;
}

void AGS::fade_start()
{
	memcpy(fader_screen, lpBmpDest, sizeof(fader_screen));
}

void AGS::fade_end()
{
	memcpy(lpBmpDest, fader_screen, sizeof(fader_screen));
}

void AGS::fade_out(int depth, bool white)
{
	// �ʏ��� �� �������
	int fx = fade_x[depth];
	int fy = fade_y[depth];
	fader = true;

	for(int y = 0; y < 480; y += 4) {
		uint32 *dest = &lpBmpDest[640 * (479 - y - fy) + fx];
		for(int x = 0; x < 640; x += 4) {
			dest[x] = white ? 0xffffff : 0;
		}
	}

	// ��ʍX�V
	invalidate_screen(0, 0, 640, screen_height);
}

void AGS::fade_in(int depth)
{
	// ������� �� �ʏ���
	int fx = fade_x[depth];
	int fy = fade_y[depth];
	fader = (depth == 15) ? false : true;

	for(int y = 0; y < 480; y += 4) {
		uint32 *src = &fader_screen[640 * (479 - y - fy) + fx];
		uint32 *dest = &lpBmpDest[640 * (479 - y - fy) + fx];
		for(int x = 0; x < 640; x += 4) {
			dest[x] = src[x];
		}
	}

	// ��ʍX�V
	invalidate_screen(0, 0, 640, screen_height);
}

void AGS::flush_screen(bool update)
{
	if(update) {
		for(int y = 0; y < screen_height; y++) {
			uint32 *src = vram[0][y];
			uint32 *dest = &lpBmpDest[640 * (479 - y)];
			for(int x = 0; x < 640; x++) {
#if defined(_SYSTEM3)
				// ����݂���񕨌� �t���J���[���ʔ�
				if(src[x] & 0x80000000) {
					dest[x] = src[x] & 0xffffff;
				} else
#endif
				dest[x] = screen_palette[src[x] & 0xff];
			}
		}
	}
	invalidate_screen(0, 0, 640, screen_height);
}

void AGS::draw_screen(int sx, int sy, int width, int height)
{
	if(fader) {
		for(int y = sy; y < (sy + height) && y < 480; y++) {
			uint32 *src = vram[0][y];
			uint32 *dest = &fader_screen[640 * (479 - y)];
			for(int x = sx; x < (sx + width) && x < 640; x++) {
				uint32 a=src[x];
#if defined(_SYSTEM3)
				// ����݂���񕨌� �t���J���[���ʔ�
				if(src[x] & 0x80000000) {
					dest[x] = src[x] & 0xffffff;
				} else
#endif
				dest[x] = screen_palette[src[x] & 0xff];
			}
		}
	} else {
		for(int y = sy; y < (sy + height) && y < 480; y++) {
			uint32 *src = vram[0][y];
			uint32 *dest = &lpBmpDest[640 * (479 - y)];
			for(int x = sx; x < (sx + width) && x < 640; x++) {
				uint32 a=src[x];
#if defined(_SYSTEM3)
				// ����݂���񕨌� �t���J���[���ʔ�
				if(src[x] & 0x80000000) {
					dest[x] = src[x] & 0xffffff;
				} else
#endif
				dest[x] = screen_palette[src[x] & 0xff];
			}
		}
		invalidate_screen(sx, sy, width, height);
	}
}

void AGS::invalidate_screen(int sx, int sy, int width, int height)
{
	RECT rect;
	rect.left = sx;
	rect.top = sy;
	rect.right = sx + width;
	rect.bottom = sy + height;

	InvalidateRect(g_hwnd, &rect, FALSE);
	UpdateWindow(g_hwnd);
}

void AGS::update_screen(HDC hdc, int sx, int sy, int width, int height)
{
	if(screen_height == 400) {
		BitBlt(hdc, sx, sy, width, height, hdcDibDest, sx, sy + (scroll - 400), SRCCOPY);
	} else {
		BitBlt(hdc, sx, sy, width, height, hdcDibDest, sx, sy, SRCCOPY);
	}
}

