/*
	ALICE SOFT SYSTEM 3 for Win32

	[ NACT - crc32 ]
*/

#include "nact.h"
#include "crc32.h"
#include "../fileio.h"

uint32 NACT::calc_crc32(const char *file_name)
{
	uint32 crc = 0;
	FILEIO *fio = new FILEIO();

	if(fio->Fopen(file_name, FILEIO_READ_BINARY)) {
		uint32 table[256];
		for(int i = 0; i < 256; i++) {
			uint32 c = i;
			for(int j = 0; j < 8; j++) {
				if(c & 1) {
					c = (c >> 1) ^ 0xedb88320;
				} else {
					c >>= 1;
				}
			}
			table[i] = c;
		}
		// ADISK.DAT�̐擪256bytes
		for(int i = 0; i < 256; i++) {
			int d = fio->Fgetc();
			uint32 c = ~crc;
			c = table[(c ^ d) & 0xff] ^ (c >> 8);
			crc = ~c;
		}
		fio->Fclose();
	}
	delete fio;
	return crc;
}

bool NACT::get_title(_TCHAR *title, int length)
{
#if defined(_SYSTEM1)
	switch(crc32_a) {
		case CRC32_BUNKASAI:
			_tcscpy_s(title, length, _T("���ԂȂ������ՑO��"));
			return true;
		case CRC32_CRESCENT:
			_tcscpy_s(title, length, _T("�N���Z���g���[��������"));
			return true;
		case CRC32_DPS:
			_tcscpy_s(title, length, _T("D.P.S - Dream Program System"));
			return true;
		case CRC32_DPS_SG:
//		case CRC32_DPS_SG2:
			switch(crc32_b) {
				case CRC32_DPS_SG_FAHREN:
					_tcscpy_s(title, length, _T("D.P.S SG - Fahren Fliegen"));
					return true;
				case CRC32_DPS_SG_KATEI:
					_tcscpy_s(title, length, _T("D.P.S SG - �ƒ닳�t�̓X�e�L�Ȃ��d��"));
					return true;
				case CRC32_DPS_SG_NOBUNAGA:
					_tcscpy_s(title, length, _T("D.P.S SG - �M���̈��d"));
					return true;
				case CRC32_DPS_SG2_ANTIQUE:
					_tcscpy_s(title, length, _T("D.P.S SG set2 - ANTIQUE HOUSE"));
					return true;
				case CRC32_DPS_SG2_IKENAI:
					_tcscpy_s(title, length, _T("D.P.S SG set2 - �����Ȃ����Ȍ��f�Ă�"));
					return true;
				case CRC32_DPS_SG2_AKAI:
					_tcscpy_s(title, length, _T("D.P.S SG set2 - �邢��"));
					return true;
			}
			_tcscpy_s(title, length, _T("D.P.S SG"));
			return true;
		case CRC32_DPS_SG3:
			switch(crc32_b) {
				case CRC32_DPS_SG3_RABBIT:
					_tcscpy_s(title, length, _T("D.P.S SG set3 - Rabbit P4P"));
					return true;
				case CRC32_DPS_SG3_SHINKON:
					_tcscpy_s(title, length, _T("D.P.S SG set3 - ���񂱂񂳂���̂�����"));
					return true;
				case CRC32_DPS_SG3_SOTSUGYOU:
					_tcscpy_s(title, length, _T("D.P.S SG set3 - ����"));
					return true;
			}
			_tcscpy_s(title, length, _T("D.P.S SG set3"));
			return true;
		case CRC32_FUKEI:
			_tcscpy_s(title, length, _T("�w�x����u�w"));
			return true;
		case CRC32_INTRUDER:
			_tcscpy_s(title, length, _T("Intruder -�����~�̒T��-"));
			return true;
		case CRC32_TENGU:
			_tcscpy_s(title, length, _T("���ԂȂ��Ă񂮓`��"));
			return true;
		case CRC32_TOUSHIN_HINT:
			_tcscpy_s(title, length, _T("���_�s�s �q���g�f�B�X�N"));
			return true;
		case CRC32_VAMPIRE:
			_tcscpy_s(title, length, _T("Little Vampire"));
			return true;
		case CRC32_YAKATA:
			_tcscpy_s(title, length, _T("ALICE�̊�"));
			return true;
	}
#elif defined(_SYSTEM2)
	switch(crc32_a) {
		case CRC32_AYUMI_FD:
			_tcscpy_s(title, length, _T("����݂���񕨌�"));
			return true;
		case CRC32_AYUMI_HINT:
			_tcscpy_s(title, length, _T("����݂���񕨌� �q���g�f�B�X�N"));
			return true;
		case CRC32_AYUMI_PROTO:
			_tcscpy_s(title, length, _T("����݂���񕨌� PROTO"));
			return true;
		case CRC32_DALK_HINT:
			_tcscpy_s(title, length, _T("DALK �q���g�f�B�X�N"));
			return true;
		case CRC32_DRSTOP:
			_tcscpy_s(title, length, _T("Dr. STOP!"));
			return true;
		case CRC32_PROSTUDENTG_FD:
			_tcscpy_s(title, length, _T("prostudent G"));
			return true;
		case CRC32_RANCE3_HINT:
			_tcscpy_s(title, length, _T("Rance3 �q���g�f�B�X�N"));
			return true;
		case CRC32_SDPS:
			switch(crc32_b) {
				case CRC32_SDPS_MARIA:
					_tcscpy_s(title, length, _T("Super D.P.S - �}���A�ƃJ���p��"));
					return true;
				case CRC32_SDPS_TONO:
					_tcscpy_s(title, length, _T("Super D.P.S - ����̐X"));
					return true;
				case CRC32_SDPS_KAIZOKU:
					_tcscpy_s(title, length, _T("Super D.P.S - ���ꂵ���̂��C���ҋ�"));
					return true;
			}
			_tcscpy_s(title, length, _T("Super D.P.S"));
			return true;
		case CRC32_YAKATA2:
			_tcscpy_s(title, length, _T("ALICE�̊�II"));
			return true;
	}
#elif defined(_SYSTEM3)
	switch(crc32_a) {
		case CRC32_AMBIVALENZ_FD:
		case CRC32_AMBIVALENZ_CD:
			_tcscpy_s(title, length, _T("AmbivalenZ �|�񗥔w���|"));
			return true;
		case CRC32_DPSALL:
			_tcscpy_s(title, length, _T("D.P.S. �S��"));
			return true;
		case CRC32_FUNNYBEE_CD:
//		case CRC32_FUNNYBEE_PATCH:
		case CRC32_FUNNYBEE_FD:
			_tcscpy_s(title, length, _T("�F�������t�@�j�[Bee"));
			return true;
		case CRC32_ONLYYOU:
			_tcscpy_s(title, length, _T("Only You �|���I���̃W�����G�b�g�B�|"));
			return true;
		case CRC32_ONLYYOU_DEMO:
			_tcscpy_s(title, length, _T("Only You �|���I���̃W�����G�b�g�B�| �f����"));
			return true;
		case CRC32_PROSTUDENTG_CD:
			_tcscpy_s(title, length, _T("prostudent G"));
			return true;
		case CRC32_RANCE41:
			_tcscpy_s(title, length, _T("�����X 4.1 �`����H����~���I�`"));
			return true;
		case CRC32_RANCE42:
			_tcscpy_s(title, length, _T("�����X 4.2 �`�G���W�F���g�`"));
			return true;
		case CRC32_AYUMI_CD:
			_tcscpy_s(title, length, _T("����݂���񕨌�"));
			return true;
		case CRC32_AYUMI_JISSHA_256:
			_tcscpy_s(title, length, _T("����݂���񕨌� ���ʔ�"));
			return true;
		case CRC32_AYUMI_JISSHA_FULL:
			_tcscpy_s(title, length, _T("����݂���񕨌� �t���J���[���ʔ�"));
			return true;
		case CRC32_YAKATA3_CD:
		case CRC32_YAKATA3_FD:
			_tcscpy_s(title, length, _T("�A���X�̊�3"));
			return true;
		case CRC32_HASHIRIONNA2:
			_tcscpy_s(title, length, _T("���菗2"));
			return true;
		case CRC32_TOUSHIN2_GD:
			_tcscpy_s(title, length, _T("���_�s�s2 �O���t�B�b�N�f�B�X�N"));
			return true;
		case CRC32_TOUSHIN2_SP:
			_tcscpy_s(title, length, _T("���_�s�s2 �����āA���ꂩ��c"));
			return true;
		case CRC32_OTOMESENKI:
			_tcscpy_s(title, length, _T("������L"));
			return true;
		case CRC32_NINGYO:
			_tcscpy_s(title, length, _T("�l�� -�f�q-"));
			return true;
		case CRC32_MUGENHOUYOU:
			_tcscpy_s(title, length, _T("�����A�e"));
			return true;
	}
#endif
	title[0] = _T('\0');
	return false;
}

