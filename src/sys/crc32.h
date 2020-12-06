/*
	ALICE SOFT SYSTEM 3 for Win32

	[ crc32 database ]
*/

#ifndef _CRC32_H_
#define _CRC32_H_

// ADISK.DAT�̐擪256bytes
#if defined(_SYSTEM1)

#define CRC32_BUNKASAI		0xc80f99b8	// ���ԂȂ������ՑO��
#define CRC32_CRESCENT		0x42351f2c	// �N���Z���g���[��������
#define CRC32_DPS		0x69ea4865	// D.P.S. - Dream Program System
#define CRC32_DPS_SG		0xab4cda48	// D.P.S. SG
#define CRC32_DPS_SG_FAHREN	0xe405d57c	//	D.P.S. SG - Fahren Fliegen
#define CRC32_DPS_SG_KATEI	 0x23e67d18	//	D.P.S. SG - �ƒ닳�t�̓X�e�L�Ȃ��d��
#define CRC32_DPS_SG_NOBUNAGA	0x2ec116f2	//	D.P.S. SG - �M���̈��d
#define CRC32_DPS_SG2		0xab4cda48	// D.P.S. SG set2
#define CRC32_DPS_SG2_ANTIQUE	0x41fe8b3d	//	D.P.S. SG set2 - ANTIQUE HOUSE
#define CRC32_DPS_SG2_IKENAI	0x6b562c09	//	D.P.S. SG set2 - �����Ȃ����Ȍ��f�Ă�
#define CRC32_DPS_SG2_AKAI	0x1098e78c	//	D.P.S. SG set2 - �邢��
#define CRC32_DPS_SG3		0xb77ae133	// D.P.S. SG SG set3
#define CRC32_DPS_SG3_RABBIT	0xa3228b6c	//	D.P.S. SG set3 - Rabbit P4P
#define CRC32_DPS_SG3_SHINKON	0x09b4448a	//	D.P.S. SG set3 - ���񂱂񂳂���̂�����
#define CRC32_DPS_SG3_SOTSUGYOU	0xbc4525d8	//	D.P.S. SG set3 - ����
#define CRC32_FUKEI		0x026de326	// �w�x����u�w
#define CRC32_INTRUDER		0xa7520fb2	// Intruder -�����~�̒T��-
#define CRC32_TENGU		0xc942ff58	// ���ԂȂ��Ă񂮓`��
#define CRC32_TOUSHIN_HINT	0xac337537	// ���_�s�s �q���g�f�B�X�N
#define CRC32_VAMPIRE		0x957bcfbf	// Little Vampire
#define CRC32_YAKATA		0x8cef6fa6	// ALICE�̊�

#elif defined(_SYSTEM2)

#define CRC32_AYUMI_FD		0x4e2fed2a	// ����݂���񕨌� (FD)
#define CRC32_AYUMI_HINT	0xf6bd963a	// ����݂���񕨌� �q���g�f�B�X�N
#define CRC32_AYUMI_PROTO	0x4e2f5678	// ����݂���񕨌� PROTO
#define CRC32_DALK_HINT		0x4793b843	// DALK �q���g�f�B�X�N
#define CRC32_DRSTOP		0x73fa86c4	// Dr. STOP!
#define CRC32_PROSTUDENTG_FD	0x5ffbfee7	// Prostudent -G- (FD)
#define CRC32_RANCE3_HINT	0x8d5ec610	// Rance3 �q���g�f�B�X�N
#define CRC32_SDPS		0xc7a20cdf	// Super D.P.S
#define CRC32_SDPS_MARIA	0x80d4eaca	//	Super D.P.S - �}���A�ƃJ���p��
#define CRC32_SDPS_TONO		0xbb1edff1	//	Super D.P.S - ����̐X
#define CRC32_SDPS_KAIZOKU	0xf81829e3	//	Super D.P.S - ���ꂵ���̂��C���ҋ�
#define CRC32_YAKATA2		0x2df591ff	// ALICE�̊�II

#elif defined(_SYSTEM3)

#define CRC32_AMBIVALENZ_FD	0xa6b48dfe	// AmbivalenZ (FD)
#define CRC32_AMBIVALENZ_CD	0x4b10db69	// AmbivalenZ (CD)
#define CRC32_DPSALL		0xd48b4ec6	// DPS�S��
#define CRC32_FUNNYBEE_CD	0xe14e3971	// Funny Bee (CD)
#define CRC32_FUNNYBEE_PATCH	0xe14e3971	// Funny Bee (CD + Patch)
#define CRC32_FUNNYBEE_FD	0x731267fa	// Funny Bee (FD)
#define CRC32_ONLYYOU		0x832aeb97	// Only You
#define CRC32_ONLYYOU_DEMO	0xc1d13e44	// Only You (DEMO)
#define CRC32_PROSTUDENTG_CD	0xfb0e4a63	// Prostudent -G- (CD)
#define CRC32_RANCE41		0xa43fb4b6	// Rance 4.1
#define CRC32_RANCE42		0x04d24d1e	// Rance 4.2
#define CRC32_AYUMI_CD		0xd2bed9ee	// ����݂���񕨌� (CD)
#define CRC32_AYUMI_JISSHA_256	0x00d15a2b	// ����݂���񕨌� ���ʔ�
#define CRC32_AYUMI_JISSHA_FULL	0x5f66ff1d	// ����݂���񕨌� �t���J���[���ʔ�
#define CRC32_YAKATA3_CD	0x7f8f5e2a	// �A���X�̊قR (CD)
#define CRC32_YAKATA3_FD	0x58ebcc99	// �A���X�̊قR (FD)
#define CRC32_HASHIRIONNA2	0x09f47cbd	// ���菗�Q (Rance 4.x �q���g�f�B�X�N)
#define CRC32_TOUSHIN2_GD	0xb5eba798	// ���_�s�s�Q �O���t�B�b�N�f�B�X�N
#define CRC32_TOUSHIN2_SP	0x2172c7b2	// ���_�s�s�Q �����āA���ꂩ��c
#define CRC32_OTOMESENKI	0x49a4db15	// ������L
#define CRC32_NINGYO		0xd491e7ab	// �l�� -�f�q-
#define CRC32_MUGENHOUYOU	0xbb27d1ba	// �����A�e

#endif

#endif
