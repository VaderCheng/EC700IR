#include "swplate.h"

/*	�ַ�����Ӧ��UNICODE�ַ� */
const WORD16 g_wszCharSet[] = {
	/*00*/0x0023,
	/*01*/0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039,
	/*11*/0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004a, 0x004b, 0x004c, 0x004d, 0x004e, 0x004f, 0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005a,
	/*37*/0x8b66,
	/*38*/0x4eac, 0x6caa, 0x6d25, 0x6e1d, 0x5180, 0x664b, 0x8499, 0x8fbd, 0x5409, 0x9ed1, 0x82cf, 0x6d59, 0x7696, 0x95fd, 0x8d63, 0x9c81, 0x8c6b, 0x9102, 0x6e58, 0x7ca4, 0x6842, 0x743c, 0x5ddd, 0x8d35, 0x4e91, 0x85cf, 0x9655, 0x7518, 0x9752, 0x5b81, 0x65b0,
	/*69*/0x7532, 0x4e59, 0x4e19, 0x4e01, 0x620a, 0x5df1, 0x5e9a, 0x8f9b, 0x58ec, 0x7678, 0x5b50, 0x4e11, 0x5bc5, 0x536f, 0x8fb0, 0x5df3, 0x5348, 0x672a, 0x7533, 0x9149, 0x620c, 0x4ea5,
	/*91*/0x4f7f,
	/*92*/0x6d88, 0x8fb9, 0x901a, 0x68ee, 0x91d1, 0x8b66, 0x7535,
	/*99*/0x8bd5, 0x6302, 0x5b66, 0x4e34,
	/*103*/0x6c88, 0x5317, 0x5170, 0x6d4e, 0x5357, 0x6210, 0x5e7f, 0x519b, 0x70ae, 0x7a7a, 0x6d77,
	/*114*/0x6e2f, 0x6fb3,0x6c11,0x822a,
};

/*
const WORD16 g_wszCharSet[] =
	L"#0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"							//0~36
	L"��"																//37
	L"�������弽�����ɼ�������������³ԥ�������������Ʋ��¸�������"	//38~68
	L"���ұ����켺�����ɹ��ӳ���î������δ�����纥"						//69~90
	L"ʹ"																//91
	L"����ͨɭ�𾯵�"													//92~98
	L"�Թ�ѧ��"															//99~102
	L"�������ϳɹ���ڿպ�"											//103~113
	L"�۰�";																//114~115
*/

/* �ַ����� */
//const int g_cnChars = sizeof(g_wszCharSet) / sizeof(g_wszCharSet[0]);

WORD16 NO_PLATE_UNICODE[] = {0x0020, 0x0020, 0x672a, 0x68c0, 0x6d4b, 0x0000}; // "  δ���"

//�ڲ�����ʹ��
HRESULT GetPlateNameUnicode(
				   WORD16 *pwszPlateName,
				   const PLATE_TYPE nPlateType,
				   const PLATE_COLOR nPlateColor,
				   const BYTE8 * pbContent
				   )
{
	if (NULL == pbContent || NULL == pwszPlateName)
	{
		return E_POINTER;
	}
	// ���Ƴ����
	if (0 == *pbContent)
	{
		for (unsigned int i = 0; i < sizeof(NO_PLATE_UNICODE) / sizeof(WORD16); i++)
		{
			*(pwszPlateName++) = NO_PLATE_UNICODE[i];
		}
		return S_OK;
	}

	WORD16 *pwszTemp = pwszPlateName;
	int iCharNum = 7;

	switch ( nPlateType )
	{
	case PLATE_MH:
	case PLATE_NORMAL:		// �������ơ�����
		if (nPlateColor == PC_BLACK)
			*(pwszTemp++) = 0x9ed1; // ��
		else
			*(pwszTemp++) = 0x84dd; // ��
		break;
	case PLATE_WJ:				// �侯����
	case PLATE_DOUBLE_WJ:		// ˫���侯
	case PLATE_POLICE2:			// ��-A1234-��
	case PLATE_MILITARY:		// ����
		*(pwszTemp++) = 0x767d; // ��
		break;
	case PLATE_POLICE:			// �������е��л��ƣ�������Ƹ�ʽ��ͬ�ľ���
		if ( ( pbContent[6] == 37 ) ||
			( ( pbContent[0] >= 103 ) && ( pbContent[0] < 114 ) )
			|| ( nPlateColor == PC_WHITE ) )
		{
			*(pwszTemp++) = 0x767d; // ��
		}
		else if (nPlateColor == PC_LIGHTBLUE)
		{
			*(pwszTemp++) = 0x84dd; // ��
		}
		else
		{
			*(pwszTemp++) = 0x9ec4; // ��
		}
		break;
	case PLATE_DOUBLE_YELLOW:	// ˫�л���
		*(pwszTemp++) = 0x53cc; // ˫
		break;
	case PLATE_DOUBLE_GREEN:
		*(pwszTemp++) = 0x7eff; // ��
		iCharNum = 8;
		break;
	case PLATE_DOUBLE_MOTO:		// ˫��Ħ����
		*(pwszTemp++) = 0x6469;	//Ħ
		break;
	case PLATE_INDIVIDUAL:
		*(pwszTemp++) = 0x767d; // ��
		break;
	case PLATE_SHI:
		*(pwszTemp++) = 0x9ed1; // ��
		break;
	case PLATE_UNKNOWN:
		break;
    default:
        break;
	}

	if(nPlateType==PLATE_WJ || nPlateType==PLATE_DOUBLE_WJ)
	{
		*(pwszTemp++) = L'W';
		*(pwszTemp++) = L'J';
	}

	//if (pbContent[0] >= 11 && pbContent[0] <= 36)
	//{
	//	*(pwszTemp++) = MapId2Char(38); // ��
	//}

	for (int i=0; i<iCharNum; i++)
	{
		if((nPlateType==PLATE_DOUBLE_WJ || nPlateType==PLATE_WJ) && i == 0)
		{
			continue;
		}
		int nTemp=pbContent[i];
		if (i==6 && nPlateType==PLATE_POLICE2)			// ��-A1234-��
		{
			nTemp=37;
		}
		*(pwszTemp++) = MapId2Char(nTemp);
	}
	*pwszTemp = L'\0';

	return S_OK;
}

