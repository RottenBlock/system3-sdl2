/*
	ALICE SOFT SYSTEM 3 for Win32

	[ DRI ]
*/

#include "dri.h"
#include "crc32.h"
#include "../fileio.h"
#if defined(_SYSTEM1)
#include "../res1/resource.h"
#elif defined(_SYSTEM2)
#include "../res2/resource.h"
#else
#include "../res3/resource.h"
#endif

uint8 *DRI::load(const char *file_name, int page, int *size)
{
	char tmp_name[_MAX_PATH];
	strcpy_s(tmp_name, _MAX_PATH, file_name);

	FILEIO *fio = new FILEIO();

	// �y�[�W�̈ʒu���擾
	tmp_name[0] = 'A';
	if(!fio->Fopen(tmp_name, FILEIO_READ_BINARY)) {
		delete fio;
		return NULL;
	}

	int link_sector = fio->Fgetc();
	link_sector |= fio->Fgetc() << 8;
	int data_sector = fio->Fgetc();
	data_sector |= fio->Fgetc() << 8;

	if(page > (data_sector - link_sector) * 128 - 1) {
		// �y�[�W�ԍ��s��
		fio->Fclose();
		delete fio;
		return NULL;
	}

	fio->Fseek((link_sector - 1) * 256 + (page - 1) * 2, FILEIO_SEEK_SET);
	int disk_index = fio->Fgetc();
	int link_index = fio->Fgetc();

	if(disk_index == 0 || disk_index == 0x1a) {
		// ����
		fio->Fclose();
		delete fio;
		return NULL;
	}

	// A??.DAT�ȊO�Ƀ����N����Ă���ꍇ�̓t�@�C�����J������
	if(disk_index != 1) {
		tmp_name[0] = 'A' + disk_index - 1;
		fio->Fclose();
		if(!fio->Fopen(tmp_name, FILEIO_READ_BINARY)) {
			delete fio;
			return NULL;
		}
	}

	// �f�[�^�擾
	fio->Fseek(link_index * 2, FILEIO_SEEK_SET);
	int start_sector = fio->Fgetc();
	start_sector |= fio->Fgetc() << 8;
	int end_sector = fio->Fgetc();
	end_sector |= fio->Fgetc() << 8;

	if((*size = (end_sector - start_sector) * 256) == 0) {
		// �T�C�Y�s��
		fio->Fclose();
		delete fio;
		return NULL;
	}

	uint8 *buffer = (uint8 *)malloc(*size);
	fio->Fseek((start_sector - 1) * 256, FILEIO_SEEK_SET);
	fio->Fread(buffer, 256, end_sector - start_sector);

	fio->Fclose();
	delete fio;

	return buffer;
}

uint8 *DRI::load_mda(HINSTANCE hInst, uint32 crc32_a, uint32 crc32_b, int page, int *size)
{
	// �f�[�^�擾
	HGLOBAL hGlobal = NULL;

#if defined(_SYSTEM1)
	switch(crc32_a) {
		case CRC32_BUNKASAI:		// ���ԂȂ������ՑO��
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_AB ), _T("mda")));
			break;
		case CRC32_CRESCENT:		// �N���Z���g���[��������
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_CRS), _T("mda")));
			break;
		case CRC32_DPS:			// D.P.S. - Dream Program System
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_DPS), _T("mda")));
			break;
		case CRC32_DPS_SG:		// D.P.S. SG
//		case CRC32_DPS_SG2:		// D.P.S. SG set2
		case CRC32_DPS_SG3:		// D.P.S. SG set2
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_SG ), _T("mda")));
			break;
		case CRC32_FUKEI:		// �w�x����u�w
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_VX ), _T("mda")));
			break;
		case CRC32_INTRUDER:		// Intruder -�����~�̒T��-
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_INT), _T("mda")));
			break;
		case CRC32_TENGU:		// ���ԂȂ��Ă񂮓`��
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_AT ), _T("mda")));
			break;
		case CRC32_TOUSHIN_HINT:	// ���_�s�s �q���g�f�B�X�N
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_T1 ), _T("mda")));
			break;
		case CRC32_VAMPIRE:		// Little Vampire
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_LP2), _T("mda")));
			break;
		case CRC32_YAKATA:		// ALICE�̊�
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_AL1), _T("mda")));
			break;
	}
#elif defined(_SYSTEM2)
	switch(crc32_a) {
		case CRC32_AYUMI_FD:		// ����݂���񕨌� (FD)
		case CRC32_AYUMI_HINT:		// ����݂���񕨌� �q���g�f�B�X�N
		case CRC32_AYUMI_PROTO:		// ����݂���񕨌� PROTO
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_AYM), _T("mda")));
			break;
		case CRC32_DALK_HINT:		// DALK �q���g�f�B�X�N
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_DLK), _T("mda")));
			break;
		case CRC32_DRSTOP:		// Dr. STOP!
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_DRS), _T("mda")));
			break;
		case CRC32_PROSTUDENTG_FD:	// Prostudent -G- (FD)
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_PSG), _T("mda")));
			break;
		case CRC32_RANCE3_HINT:		// Rance3 �q���g�f�B�X�N
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_R3H), _T("mda")));
			break;
		case CRC32_SDPS:		// Super D.P.S
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_SDP), _T("mda")));
			break;
		case CRC32_YAKATA2:		// ALICE�̊�II
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_AL2), _T("mda")));
			break;
	}
#elif defined(_SYSTEM3)
	switch(crc32_a) {
		case CRC32_AMBIVALENZ_FD:	// AmbivalenZ (FD)
		case CRC32_AMBIVALENZ_CD:	// AmbivalenZ (CD)
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_AMB), _T("mda")));
			break;
		case CRC32_DPSALL:		// DPS�S��
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_ALL), _T("mda")));
			break;
		case CRC32_FUNNYBEE_CD:		// Funny Bee (CD)
//		case CRC32_FUNNYBEE_PATCH:	// Funny Bee (CD + Patch)
		case CRC32_FUNNYBEE_FD:		// Funny Bee (FD)
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_BEE), _T("mda")));
			break;
		case CRC32_ONLYYOU:		// Only You
		case CRC32_ONLYYOU_DEMO:	// Only You (DEMO)
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_OY ), _T("mda")));
			break;
		case CRC32_PROSTUDENTG_CD:	// Prostudent -G- (CD)
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_PSG), _T("mda")));
			break;
		case CRC32_RANCE41:		// Rance 4.1
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_R41), _T("mda")));
			break;
		case CRC32_RANCE42:		// Rance 4.2
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_R42), _T("mda")));
			break;
		case CRC32_AYUMI_CD:		// ����݂���񕨌� (CD)
		case CRC32_AYUMI_JISSHA_256:	// ����݂���񕨌� ���ʔ�
		case CRC32_AYUMI_JISSHA_FULL:	// ����݂���񕨌� �t���J���[���ʔ�
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_AYM), _T("mda")));
			break;
		case CRC32_YAKATA3_CD:		// �A���X�̊قR (CD)
		case CRC32_YAKATA3_FD:		// �A���X�̊قR (FD)
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_AL3), _T("mda")));
			break;
		case CRC32_HASHIRIONNA2:	// ���菗�Q (Rance 4.x �q���g�f�B�X�N)
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_RG2), _T("mda")));
			break;
		case CRC32_TOUSHIN2_GD:		// ���_�s�s�Q �O���t�B�b�N�f�B�X�N
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_T2 ), _T("mda")));
			break;
		case CRC32_OTOMESENKI:		// ������L
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_OTM), _T("mda")));
			break;
		case CRC32_MUGENHOUYOU:		// �����A�e
			hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_AMUS_MGN), _T("mda")));
			break;
	}
#endif

	if(hGlobal == NULL) {
		return NULL;
	}

	uint8 *data = (uint8 *)LockResource(hGlobal);

	// �y�[�W�̈ʒu���擾
	int link_sector = data[0] | (data[1] << 8);
	int data_sector = data[2] | (data[3] << 8);

	if(page > (data_sector - link_sector) * 128 - 1) {
		// �y�[�W�ԍ��s��
		return NULL;
	}

	int disk_index = data[(link_sector - 1) * 256 + (page - 1) * 2 + 0];
	int link_index = data[(link_sector - 1) * 256 + (page - 1) * 2 + 1];

	if(disk_index == 0 || disk_index == 0x1a) {
		// ����
		return NULL;
	}

	// AMUS.MDA�ȊO�Ƀ����N����Ă���ꍇ�̓��\�[�X���J������
#if defined(_SYSTEM1)
	if(disk_index == 2) {
		switch(crc32_b) {
			case CRC32_DPS_SG_FAHREN:	// D.P.S. SG - Fahren Fliegen
				hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_BMUS_FAH), _T("mda")));
				break;
			case CRC32_DPS_SG_KATEI:	// D.P.S. SG - �ƒ닳�t�̓X�e�L�Ȃ��d��
				hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_BMUS_KAT), _T("mda")));
				break;
			case CRC32_DPS_SG_NOBUNAGA:	// D.P.S. SG - �M���̈��d
				hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_BMUS_NOB), _T("mda")));
				break;
			case CRC32_DPS_SG2_ANTIQUE:	// D.P.S. SG set2 - ANTIQUE HOUSE
				hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_BMUS_ANT), _T("mda")));
				break;
			case CRC32_DPS_SG2_IKENAI:	// D.P.S. SG set2 - �����Ȃ����Ȍ��f�Ă�
				hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_BMUS_NAI), _T("mda")));
				break;
			case CRC32_DPS_SG2_AKAI:	// D.P.S. SG set2 - �邢��
				hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_BMUS_AKA), _T("mda")));
				break;
			case CRC32_DPS_SG3_RABBIT:	// D.P.S. SG set3 - Rabbit P4P
				hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_BMUS_RAB), _T("mda")));
				break;
			case CRC32_DPS_SG3_SHINKON:	// D.P.S. SG set3 - ���񂱂񂳂���̂�����
				hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_BMUS_SIN), _T("mda")));
				break;
			case CRC32_DPS_SG3_SOTSUGYOU:	// D.P.S. SG set3 - ����
				hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_BMUS_SOT), _T("mda")));
				break;
			default:
				hGlobal = NULL;
				break;
		}
		if(hGlobal == NULL) {
			return NULL;
		}
		data = (uint8 *)LockResource(hGlobal);
	} else if(disk_index == 3) {
		switch(crc32_a) {
			case CRC32_TOUSHIN_HINT:	// ���_�s�s �q���g�f�B�X�N
				hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_CMUS_T1 ), _T("mda")));
				break;
			default:
				hGlobal = NULL;
				break;
		}
		if(hGlobal == NULL) {
			return NULL;
		}
		data = (uint8 *)LockResource(hGlobal);
	} else
#elif defined(_SYSTEM2)
	if(disk_index == 2) {
		switch(crc32_b) {
			case CRC32_SDPS_MARIA:		// Super D.P.S - �}���A�ƃJ���p��
				hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_BMUS_MTK), _T("mda")));
				break;
			case CRC32_SDPS_TONO:		// Super D.P.S - ����̐X
				hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_BMUS_TNM), _T("mda")));
				break;
			case CRC32_SDPS_KAIZOKU:	// Super D.P.S - ���ꂵ���̂��C���ҋ�
				hGlobal = LoadResource(hInst, FindResource(hInst, MAKEINTRESOURCE(IDR_BMUS_KAM), _T("mda")));
				break;
			default:
				hGlobal = NULL;
				break;
		}
		if(hGlobal == NULL) {
			return NULL;
		}
		data = (uint8 *)LockResource(hGlobal);
	} else
#endif
	// AMUS.MDA�ȊO�Ƀ����N����Ă���ꍇ�͎��s
	if(disk_index != 1) {
		return NULL;
	}

	// �f�[�^�擾
	int start_sector = data[link_index * 2 + 0] | (data[link_index * 2 + 1] << 8);
	int end_sector = data[link_index * 2 + 2] | (data[link_index * 2 + 3] << 8);

	if((*size = (end_sector - start_sector) * 256) == 0) {
		// �T�C�Y�s��
		return NULL;
	}

	uint8 *buffer = (uint8 *)malloc(*size);
	memcpy(buffer, &data[(start_sector - 1) * 256], *size);

	return buffer;
}

