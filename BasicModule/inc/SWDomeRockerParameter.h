#ifndef _SWDOMEROCKERPARAMETER_H_
#define _SWDOMEROCKERPARAMETER_H_

typedef struct _tagDomeRockerParam
{
    BOOL fRockerEnable; // ʹ�ܱ�־
	INT iBaudrate;      // ������
	INT iProtocol;      // Э��(Ŀǰֻ֧��pelco-d)

    _tagDomeRockerParam()
    {
        fRockerEnable = FALSE;
		iBaudrate = 9600;
        iProtocol = 0;
    }
}DOMEROCKER_PARAM;

#endif // _SWDOMEROCKERPARAMETER_H_
