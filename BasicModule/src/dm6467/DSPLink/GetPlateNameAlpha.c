// ���ļ����������WINDOWS-936��ʽ

#include "swplate.h"
#include <string.h>

int ProcessPlateNameAlpha(char* pszPlateName)
{
	char* pszBuff = pszPlateName;

	if (NULL == pszPlateName) return -1;

	if ( strncmp( pszBuff,"˫",2 ) == 0 || strncmp( pszBuff,"Ħ",2 ) == 0)
	{
		if ((strncmp(pszBuff+2,"��",2) == 0 ) ||
			(strncmp(pszBuff+2,"��",2) == 0 ) ||
			(strncmp(pszBuff+2,"��",2) == 0 ) ||
			(strncmp(pszBuff+2,"��",2) == 0 ) ||
			(strncmp(pszBuff+2,"��",2) == 0 ) ||
			(strncmp(pszBuff+2,"��",2) == 0 ) ||
			(strncmp(pszBuff+2,"��",2) == 0 ) ||
			(strncmp(pszBuff+2,"��",2) == 0 ) ||
			(strncmp(pszBuff+2,"��",2) == 0 ) ||
			(strncmp(pszBuff+2,"��",2) == 0 ) ||
			(strncmp(pszBuff+2,"��",2) == 0 ) ||
			(strncmp(pszBuff+2,"B",1) == 0 ) ||             //���¾���ʶ�𿪹غ��жϾ��Ƶĳ�����ɫ
			(strncmp(pszBuff+2,"C",1) == 0 ) ||
			(strncmp(pszBuff+2,"E",1) == 0 ) ||
			(strncmp(pszBuff+2,"G",1) == 0 ) ||
			(strncmp(pszBuff+2,"H",1) == 0 ) ||
			(strncmp(pszBuff+2,"J",1) == 0 ) ||
			(strncmp(pszBuff+2,"K",1) == 0 ) ||
			(strncmp(pszBuff+2,"L",1) == 0 ) ||
			(strncmp(pszBuff+2,"N",1) == 0 ) ||
			(strncmp(pszBuff+2,"S",1) == 0 ) ||
			(strncmp(pszBuff+2,"V",1) == 0 ) ||
			(strncmp(pszBuff+2,"Z",1) == 0 ))
		{
			memcpy(pszBuff,"��",2);
		}
		else
		{
			memcpy(pszBuff,"��",2);
		}
	}
	return 0;
}

#if !(defined(RELEASE_TO_MARKET) || defined(RELEASE_TO_IN))

const char *g_rgszPlateCharAlpha[] = {
	/*0*/ "#", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
	/*37*/ "��",
	/*38*/ "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "³", "ԥ", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��",
	/*69*/ "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "î", "��", "��", "��", "δ", "��", "��", "��", "��",
	/*91*/ "ʹ",
	/*92*/ "��", "��", "ͨ", "ɭ", "��", "��", "��",
	/*99*/ "��", "��", "ѧ", "��",
	/*103*/"��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��",
	/*114*/"��", "��"
};

/*�ַ����ϳ���*/
int g_cnCharsAlpha = sizeof(g_rgszPlateCharAlpha) / sizeof(g_rgszPlateCharAlpha[0]);

char NO_PLATE[] = "  �޳���";

/*����ֲ�ĺ���*/
int GetPlateNameAlpha(
	char *pszPlateName,
	const PLATE_TYPE nPlateType,
	const PLATE_COLOR nPlateColor,
	const unsigned char* pbContent
	)
{
	int i, nTemp, iCharNum = 7;
	char *pszTemp = pszPlateName;
	int fWhitePlate = 0;

	if (NULL == pbContent || NULL == pszPlateName)
	{
		return -1;
	}
	// ���Ƴ����
	if (0 == *pbContent)
	{
		strcpy( ( char* )pszPlateName, ( char* )NO_PLATE );
		return 0;
	}
	switch ( nPlateType )
	{
	case PLATE_NORMAL:		// �������ơ�����
		if (nPlateColor == PC_BLACK)
			strcpy( pszTemp, "��" );
		else
			strcpy( pszTemp, "��" );
		pszTemp += strlen(pszTemp);
		break;
	case PLATE_WJ:				// �侯����
	case PLATE_DOUBLE_WJ:       // ˫���侯����
	case PLATE_POLICE2:			// ��-A1234-��
	case PLATE_MILITARY:		// ����
		fWhitePlate = 1;
		strcpy( pszTemp, "��" );
		pszTemp += strlen(pszTemp);
		break;
	case PLATE_POLICE:			// �������е��л��ƣ�������Ƹ�ʽ��ͬ�ľ���
		if ( ( pbContent[6] == 37 ) ||
			( ( pbContent[0] >= 103 ) && ( pbContent[0] < 114 ) )
			|| ( nPlateColor == PC_WHITE ) )
		{
			fWhitePlate = 1;
			strcpy( pszTemp, "��" );
		}
		else if (nPlateColor == PC_LIGHTBLUE)
		{
			strcpy( pszTemp, "��" );
		}
		else
		{
			strcpy( pszTemp, "��" );
		}
		pszTemp += strlen(pszTemp);
		break;
	case PLATE_DOUBLE_YELLOW:	// ˫�л���
		strcpy( pszTemp, "˫" );
		pszTemp += strlen(pszTemp);
		break;
	case PLATE_DOUBLE_GREEN:
		strcpy( pszTemp, "��" );
		pszTemp += strlen(pszTemp);
		iCharNum = 8;
		break;
	case PLATE_DOUBLE_MOTO:	// ˫��Ħ����
		strcpy( pszTemp, "Ħ" );
		pszTemp += strlen(pszTemp);
		break;
	case PLATE_INDIVIDUAL:
		strcpy( pszTemp, "��" );
		pszTemp += strlen(pszTemp);
		break;
	case PLATE_SHI:
		strcpy( pszTemp, "��" );
		pszTemp += strlen(pszTemp);
		break;
	case PLATE_UNKNOWN:
		break;
    default:
        break;
	}

	if((nPlateType==PLATE_WJ) || (nPlateType==PLATE_DOUBLE_WJ))
	{
		*(pszTemp++)='W';
		*(pszTemp++)='J';
	}

	/*if (pbContent[0] >= 11 && pbContent[0] <= 36)
	{
		strcpy( pszTemp, "��" );
		pszTemp += strlen(pszTemp);
	}*/

	for (i=0; i<iCharNum; i++)
	{
		if((nPlateType==PLATE_DOUBLE_WJ || nPlateType==PLATE_WJ) && i == 0)
		{
			continue;
		}
		nTemp = pbContent[i];
		if (nTemp >= g_cnCharsAlpha)
		{
			nTemp = 0;
		}
		if (i == 6)
		{
			if ( (37 == nTemp && !fWhitePlate )		//�ǰ������һλ��Ϊ��������
				|| (pbContent[0] >= 103
				&& pbContent[0] <= 113 && nTemp > 36)	//�������һλ��Ϊ����
				)
			{
				nTemp = 1;
			}
			if (nPlateType==PLATE_POLICE2)			// ��-A1234-��
			{
				nTemp=37;
			}
		}
		strcpy(pszTemp, g_rgszPlateCharAlpha[nTemp]);
		pszTemp += strlen(pszTemp);
	}
	*pszTemp = '\0';

	ProcessPlateNameAlpha(pszPlateName);

	return 1;
}

#endif
