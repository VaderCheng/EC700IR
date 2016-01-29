/**
* @file		HvDebug.h
* @version	1.0
* @brief	���ڶ��豸�����з����Ե�һЩ���ߺ�
* @author	Shaorg
* @date		2011-07-01
*/

#ifndef _HVDEBUG_H_
#define _HVDEBUG_H_

// ʹ�ܼ�ʱ����
#define ENABLE_HVDEBUG_TIMER

// ʹ�ܱ�Ǻ�
#define ENABLE_HVDEBUG_MARK

//------------------------------------------

/*
 * ��ʱ����ʹ��ʾ��1��
 * ...
 * tS(MyMark);
 * SomeFunc();
 * tE(MyMark);
 * ...
 * ������ʾ�����ɷ����ݵ�֪��SomeFunc���������ִ��ʱ�䡣
 *
 * ��ʱ����ʹ��ʾ��2��
 * ...
 * tSS(MyMark,10);
 * SomeFunc();
 * tEE(MyMark,10);
 * ...
 * ������ʾ�����ɷ����ݵ�֪��SomeFunc�������ִ��10�ε�ƽ��ʱ�䡣
 */

#define tS L_TIMER_RUN
#define tE L_TIMER_END

#define tSS L_TIMER_RUN_EX
#define tEE L_TIMER_END_EX

#ifdef ENABLE_HVDEBUG_TIMER

    #define L_TIMER_RUN(id) \
        \
        DWORD dwStart##id = GetSystemTick();

    #define L_TIMER_END(id) \
        \
        char szMsg##id[256];\
        sprintf(szMsg##id, "{%s} %6d ms\r\n", #id, (int)(GetSystemTick() - dwStart##id));\
        HV_Trace(5, "%s", szMsg##id);

#else

    #define L_TIMER_RUN(id)
    #define L_TIMER_END(id)

#endif

#ifdef ENABLE_HVDEBUG_TIMER

    #define L_TIMER_RUN_EX(id,T) \
        \
        DWORD dwStart##id##T = GetSystemTick();\
        static DWORD dwTimeMs##id##T = 0;\
        static DWORD dwCount##id##T = 0;

    #define L_TIMER_END_EX(id,T) \
        \
        char szMsg##id##T[256];\
        dwTimeMs##id##T += (GetSystemTick() - dwStart##id##T);\
        dwCount##id##T++;\
        if ( dwCount##id##T >= T )\
        {\
            sprintf(szMsg##id##T, "{%s,%s} %d ms\r\n", #id, #T, int((float)dwTimeMs##id##T / dwCount##id##T));\
            HV_Trace(5, "%s", szMsg##id##T);\
            dwTimeMs##id##T = 0;\
            dwCount##id##T = 0;\
        }

#else

    #define L_TIMER_RUN_EX(id,T)
    #define L_TIMER_END_EX(id,T)

#endif

//------------------------------------------

// ��Ǻ�ʹ��˵���� �ú������ڴ����д�ӡ��ǣ�ͨ����ӡ��Ϣ�鿴����ִ�е��δ���

#ifdef ENABLE_HVDEBUG_MARK
    #define HVDEBUG_MARK \
    HV_Trace(5, "HvDebugMark:[%s]-[%s]-[%d]\n", __FILE__, __FUNCTION__, __LINE__);
#else
    #define HVDEBUG_MARK
#endif

//------------------------------------------

#endif
