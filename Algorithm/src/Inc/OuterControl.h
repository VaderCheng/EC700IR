#ifndef _OUTERCONTROL_INCLUDED__
#define _OUTERCONTROL_INCLUDED__

#include "hvutils.h"
#include "trackerdef.h"
#include "hvinterface.h"
using namespace HiVideo;

// 附加信息类型
typedef enum
{
    ATTACH_INFO_NOTHING = 0,				// 没有附加信息
    ATTACH_INFO_SPEED,						// 附带车牌速度
    ATTACH_INFO_LAST_IMAGE,					// 附带Last场景图片
    ATTACH_INFO_WEIGHT						// 附带重量
} ATTACH_INFO_TYPE;

// 车牌和信号状态定义
typedef enum
{
    STATUS_INIT = 0,					    // 初始状态
    STATUS_ADD_IMAGE,					    // 主信号附加图片
    STATUS_OUTTIME,						    // 信息超过时间期限
    STATUS_MATCH						    // 信息已经匹配
}INFO_STATUS_TYPE;

// 匹配类型
typedef enum
{
    MATCH_PLATE = 0,				        // 有牌匹配
    MATCH_NO_PLATE			                // 无牌匹配
}MATCH_TYPE;

// 出牌方式定义
typedef enum
{
    PLATE_OUTPUT_AUTO = 0,					// 自动出牌方式，不考虑信号，无需匹配
    PLATE_OUTPUT_SIGNAL						// 出牌必须考虑信号
} OUTPUT_PLATE_MODE;

// 工作模式定义
typedef enum
{
    WORK_MODE_SINGLE = 0,					// 单车道方式
    WORK_MODE_MULTI							// 多车道方式
} WORK_MODE_TYPE;

// 车牌队列数据结构
typedef struct
{
    CARLEFT_INFO_STRUCT *pCarleftInfo;		// 车牌信息
    LPVOID pvUserData;						// 用户附加数据
    INFO_STATUS_TYPE nStatus;				// 车牌状态
    DWORD32 dwPlateTime;					// 出牌时标
    DWORD32 dwInputTime;					// 入队列的时标
    DWORD32 dwValue;					    // 数值(速度)
} PLATE_INFO_hxl;

// 信号数据结构
typedef struct
{
    int nType;								// 信号源类型
    INFO_STATUS_TYPE nStatus;				// 信号状态
    DWORD32 dwSignalTime;					// 信号时标
    DWORD32 dwInputTime;					// 入队列时标
    DWORD32 dwValue;						// 数值(速度)
    DWORD32 dwRoad;							// 车道编号(高16位为0x00FF),如果是0x00FF00FF,就是全覆盖
    DWORD32 dwFlag;							// 车道号高16位的值
    DWORD32 dwOutTime;						// 信号过期时间
    int iModifyRoad;						// 用于不同触发车道号一样时，标记是否需要修正车道号
    PROCESS_EVENT_STRUCT cProcessEvent;
    IReferenceComponentImage *pImage;		// 信号触发时的大图
    IReferenceComponentImage *pImageLast;	// 信号触发时的大图(last)
} SIGNAL_INFO;

// 信号源
typedef struct
{
    int nType;								// 信号类型, 由SIGNAL_INFO指定
    DWORD32 dwPrevTime;						// 信号前有效时间(毫秒)
    DWORD32 dwPostTime;						// 信号后有效时间(毫秒)
    int iRoad;								// 所属的车道编号,从0开始,如果车道数是3,则3表示全覆盖
} SIGNAL_SOURCE;

// 信号类型结构
typedef struct
{
    char strName[64];						// 类型描述
    BOOL fMainSignal;						// 是否主信号	true:主信号 false:非主信号
    ATTACH_INFO_TYPE atType;				// 附加信息类型
} SIGNAL_TYPE;

// 匹配分析信息结构
typedef struct
{
    int nSigSourceType;					    // 信号源编号
    HVPOSITION nSignalPos;				    // 匹配的信号位置
    HVPOSITION nSignalPos_2;			    // 匹配的第二个信号位置
    long lTimeDiff;				            // 匹配时间差异
    DWORD32 dwPosDiff;					    // 匹配距离差异
    DWORD32 dwValueDiff;					// 值的差异(速度等)
} MATCH_ANALYSIS;

// 匹配信息数量
typedef struct
{
    MATCH_TYPE nMatchType;			        // 匹配类型 0:有牌车 1:无牌车
    void *pInfo;						    // PlateInfo或者SignalInfo指针，取决于nMatchType
    DWORD32 dwSpeed;						// 速度
    DWORD32 dwWeight;					    // 重量
    IReferenceComponentImage *pLastImage;	// 附加的图像
    IReferenceComponentImage *pLastImage_2;	// 附加的第二张图像
    IReferenceComponentImage *pPanormicImage; //全景抓拍图
    PROCESS_EVENT_STRUCT cProcessEvent;
} MATCH_INFO;

//触发输出时间
typedef struct
{
    int iRoadNumber;			            //车牌所在车道
    DWORD32 dwTime;		                    //触发信号产生时间
    DWORD32 dwDelay;		                //触发信号延迟发送时间
} OUTPUT_SIGNAL;

//-----------------------------
//  外总控（信号匹配器）接口
//-----------------------------
class IOuterControler
{
public:
    virtual ~IOuterControler() {};

    // 为HvCore实现的Init函数
    STDMETHOD(InitOuterControler)() = 0;

    STDMETHOD(Run)() = 0;

    STDMETHOD(Stop)() = 0;

    STDMETHOD(CarArrive)(
        CARARRIVE_INFO_STRUCT *pCarArriveInfo,
        LPVOID pvUserData
    ) = 0;

    STDMETHOD(CarLeft)(
        CARLEFT_INFO_STRUCT *pCarLeftInfo,
        LPVOID pvUserData
    ) = 0;

    // 图形处理接口
    STDMETHOD(ProcessOneFrame)(
        int iVideoID,
        IReferenceComponentImage *pImage,
        IReferenceComponentImage *pCapImage,
        PVOID pvParam,
        PROCESS_EVENT_STRUCT* pProcessEvent
    ) = 0;

	STDMETHOD(GetTrafficLightStatus)(
		int* pbStatus
		)=0;
    // 增加信号
    STDMETHOD(AppendSignal)(SIGNAL_INFO *pSignalInfo) = 0;

    virtual bool ThreadIsOk() = 0;
};

#endif // _OUTERCONTROL_INCLUDED__
