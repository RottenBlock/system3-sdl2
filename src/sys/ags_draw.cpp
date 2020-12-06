/*
	ALICE SOFT SYSTEM 3 for Win32

	[ AGS - draw ]
*/

#include "ags.h"
#include "dri.h"
#include "crc32.h"

void AGS::load_cg(int page, int transparent)
{
#if defined(_SYSTEM3)
	// ����݂���񕨌� �t���J���[���ʔ�
	if(strncmp(acg, "CGA000.BMP", 10) == 0) {
		char file_name[_MAX_PATH];
		sprintf_s(file_name, _MAX_PATH, "CGA%03d.BMP", page);
		load_bmp(file_name);
		return;
	} else if(strncmp(acg, "CGB000.BMP", 10) == 0) {
		char file_name[_MAX_PATH];
		sprintf_s(file_name, _MAX_PATH, "CGB%03d.BMP",  page);
		load_bmp(file_name);
		return;
	}
#endif
	DRI *dri = new DRI();
	int size;
	uint8 *data = dri->load(acg, page, &size);
	if(data) {
		#if defined(_SYSTEM1)
			if(nact->crc32_a == CRC32_BUNKASAI) {
				load_vsp(data, page, transparent);
			} else if(nact->crc32_a == CRC32_INTRUDER) {
//				load_gm3(data, page, transparent);
				load_vsp(data, page, transparent);	// �b��
			} else if(nact->crc32_a == CRC32_VAMPIRE) {
				load_vsp2l(data, page, transparent);
			} else {
				load_gl3(data, page, transparent);
			}
		#elif defined(_SYSTEM2)
			if(nact->crc32_a == CRC32_AYUMI_PROTO) {
				// ����݂���񕨌� PROTO
				load_gl3(data, page, transparent);
			} else if(nact->crc32_a == CRC32_AYUMI_FD || nact->crc32_a == CRC32_AYUMI_HINT) {
				// ����݂���񕨌�
				load_vsp(data, page, transparent);
			} else if(nact->crc32_a == CRC32_SDPS) {
				// Super D.P.S
				load_pms(data, page, transparent);
			} else {
				if(data[0x8] == 0) {
					load_vsp(data, page, transparent);
				} else {
					load_pms(data, page, transparent);
				}
			}
		#elif defined(_SYSTEM3)
			if(data[0x8] == 0) {
				load_vsp(data, page, transparent);
			} else {
				load_pms(data, page, transparent);
			}
		#endif
		free(data);
	}
	delete dri;
}

void AGS::copy(int sx, int sy, int ex, int ey, int dx, int dy)
{
#if 0
	int width = ex - sx + 1;
	int height = ey - sy + 1;

	uint32 tmp[640 * 480];
	memcpy(tmp, lpBmpScreen[src_screen], sizeof(tmp));

	for(int y = 0; y < height && y + sy < 480 && y + dy < 480; y++) {
		uint32 *src = &tmp[640 * (479 - (y + sy))];
		uint32 *dest = vram[dest_screen][y + dy];
		for(int x = 0; x < width && x + sx < 640 && x + dx < 640; x++) {
			dest[x + dx] = src[x + sx];
		}
	}
	if(dest_screen == 0) {
		draw_screen(dx, dy, width, height);
	}
#else
	BitBlt(hdcDibScreen[dest_screen], dx, dy, ex - sx + 1, ey - sy + 1, hdcDibScreen[src_screen], sx, sy, SRCCOPY);
	if(dest_screen == 0) {
		draw_screen(dx, dy, ex - sx + 1, ey - sy + 1);
	}
#endif
}

void AGS::gcopy(int gsc, int gde, int glx, int gly, int gsw)
{
	// N88-BASIC����̃R�s�[�R�}���h
	int src = (gsw == 0 || gsw == 2) ? 0 : 1;
	int dest = (gsw == 0 || gsw == 3) ? 0 : 1;
	int sx = (gsc % 80) * 8;
	int sy = gsc / 80;
	int dx = (gde % 80) * 8;
	int dy = gde / 80;

	BitBlt(hdcDibScreen[dest], dx, dy, glx * 8, gly, hdcDibScreen[src], sx, sy, SRCCOPY);
	if(dest == 0) {
		draw_screen(dx, dy, glx * 8, gly);
	}
}

void AGS::paint(int x, int y, uint8 color)
{
	HBRUSH hBrush = CreateSolidBrush(RGB(color, color, color));
	SelectObject(hdcDibScreen[0], hBrush);
	ExtFloodFill(hdcDibScreen[0], x, y, GetPixel(hdcDibScreen[0], x, y), FLOODFILLSURFACE);
	draw_screen(0, 0, 640, screen_height);
}

void AGS::draw_box(int index)
{
	if(index == 0) {
		// �S��ʏ���
		box_fill(dest_screen, 0, 0, 639, 479, 0);
		return;
	}

	int sx = box[index - 1].sx;
	int sy = box[index - 1].sy;
	int ex = box[index - 1].ex;
	int ey = box[index - 1].ey;
	uint8 color = box[index - 1].color;

	if(1 <= index && index <= 10) {
		box_fill(dest_screen, sx, sy, ex, ey, color);
	} else if(11 <= index && index <= 20) {
		box_line(dest_screen, sx, sy, ex, ey, color);
	}
}

void AGS::draw_mesh(int sx, int sy, int width, int height)
{
	// super d.p.s
	for(int y = sy, h = 0; h < height && y < 480; y += 2, h += 2) {
		for(int x = sx, w = 0; w < width && x < 640; x += 2, w += 2) {
			vram[0][y][x] = 255;
		}
		for(int x = sx + 1, w = 1; w < width && x < 640; x += 2, w += 2) {
			vram[0][y + 1][x] = 255;
		}
	}
	draw_screen(sx, sy, width, height);
}

void AGS::box_fill(int dest, int sx, int sy, int ex, int ey, uint8 color)
{
	for(int y = sy; y <= ey && y < 480; y++) {
		for(int x = sx; x <= ex && x < 640; x++) {
			vram[dest][y][x] = color;
		}
	}
	if(dest == 0) {
		draw_screen(sx, sy, ex - sx + 1, ey - sy + 1);
	}
}

void AGS::box_line(int dest, int sx, int sy, int ex, int ey, uint8 color)
{
	for(int x = sx; x <= ex && x < 640; x++) {
		vram[dest][sy][x] = color;
		vram[dest][ey][x] = color;
	}
	for(int y = sy; y <= ey && y < 480; y++) {
		vram[dest][y][sx] = color;
		vram[dest][y][ex] = color;
	}
	if(dest == 0) {
		draw_screen(sx, sy, ex - sx + 1, ey - sy + 1);
	}
}

