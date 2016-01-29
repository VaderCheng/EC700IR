#ifndef __RADAR_BASE_H__
#define __RADAR_BASE_H__
#include "hvutils.h"
#include "hvthreadbase.h"
#include "SerialBase.h"
/**
˵����һ���ֱ�������״�Ļ��࣬�����״����ͼ̳д��༴�ɡ�
*/
class CRadarBase : public CSerialBase, public CHvThreadBase
{
public:
    CRadarBase(){}
    virtual ~CRadarBase(){}
    virtual bool Initialize(void) = 0;
protected:
    virtual const char* GetName() = 0;
};
#endif
