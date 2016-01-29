#ifndef CSWIMAGESTATISTICSINFO_H
#define CSWIMAGESTATISTICSINFO_H
#include "SWObject.h"
#include "SWAutoLock.h"


struct STATISTICS_INFO;
/**
* ͼ��ͳ����Ϣ��
*/
class CSWImageStatisticsInfo : public CSWObject
{

CLASSINFO(CSWImageStatisticsInfo,CSWObject)

    enum{ MAX_IMAGEBLOCK_INFO = 16};
    // ͳ��ͼ����б�
    static BOOL *fpBlockList;
    // ͼ����Ƿ�Ϊ��ѡ��״̬
    static BOOL fIsAllNoSelect;
public:
    static BOOL InputData( PVOID pData , INT nWidth , INT nHeight );
    /**
     * @brief ��ʼ������
     * @return - �ɹ�����true,ʧ�ܷ���false
     */
    static BOOL Initialization( );
    /**
     * @brief ��ȡ����ͼƬ��ƽ������ֵ,ÿһ���Ӹ���һ��,��ֻ���û�����ʱ�Ż����
     * @return - ͼ���ƽ������ֵ,ʧ��-1
     */
    static INT GetYArgValue(  );

    /**
     * @brief ��ȡ����ͼƬ��ƽ������ֵ,ÿһ���Ӹ���һ��,��ֻ���û�����ʱ�Ż����
     * @return - ͼ���ƽ������ֵ,���ƽ������ֵû�и����򷵻�-1,
     * ע��:�÷���ֻΪAGC�ṩ����
     */
    static INT GetAGCYArgValue(  );
    /**
     * @brief ��ȡͼƬ�Ĳ���ƽ������ֵ���ֲ�λ�����ⲿָ��,ÿһ���Ӹ���һ��,��ֻ���û�����ʱ�Ÿ���
     * @return - ͼ��ľֲ�������ͼƬ��ƽ������ֵ,ʧ�ܷ���-1
     */
    static INT GetPartYArgValue( );
    /**
     * @brief ��ȡ���ͼ�����
     * @return - ��������ͳ�Ƶ�ͼ�����
     */
    static DWORD GetMaxImageBlock( );
    /**
     * @brief ������Ҫͳ��Yֵ��ͼ��飬ͼ��ͳ�Ʒ���Ĭ�϶���Ϊ��ͳ��״̬,
     * bIsStatistics = true��ʾ��Ҫͳ�ƣ�����Ϊ��ͳ�Ƹ�ͼ��Yֵ
     * @param [in] bIsStatistics : false����Ҫͳ�ƣ�true��Ҫͳ�Ƹ�ͼ���Yֵ
     * @param [in] nIndex : ��Ҫͳ�ƻ�ͳ�Ƶ�ͼ���λ��
     * @return - S_OK : �ɹ� - S_FALSE : ʧ��
     */
    static HRESULT SetImageListInfo( BOOL bIsStatistics ,  INT nIndex );

     /**
     * @brief ��ȡͼƬ����ͨ����ͳ��ֵ
     * @param [out] nRSum : Rͨ����ͳ����ֵ
     * @param [out] nGSum : Gͨ����ͳ����ֵ
     * @param [out] nBSum : Bͨ����ͳ����ֵ
     * @return - S_OK : �ɹ� - S_FALSE : ʧ��
     */
     static HRESULT GetRGBSum( INT& nRSum , INT& nGSum , INT& nBSum );

    /**
     * @brief ��ȡ����ͳ��ֵ
     * @return - Ϊ��Ƶ����������
     */

    static VOID GetVedioDiffStatistValue( FLOAT& fDiffFactor , FLOAT& fConsisIndex);
     /**
     * @brief ��ȡ����ͳ��ֵ
     * @return - Ϊץ�Ĳ�������
     */
    static VOID GetCaptureDiffStatistValue( FLOAT& fDiffFactor , BOOL &IsUpdate );
private:
    /**
     * @brief ͳ��ͼ���ƽ������ֵ
     * @param [in] pImageInfo : ��Ҫ�����ͼ����Ϣ
     * @param [in] pBlockList : ��Ҫͳ�Ƶ�ͼ����б�
     * @param [in] IsAll : �Ƿ���Ҫͳ������ͼ���Yֵ��Ĭ��Ϊ��Ҫͳ��
     * ���ֻ��Ҫͳ�Ʋ���ͼ����Yֵ��Ϣ�����ֵΪfalse
     * @return - ����ͼ���ƽ������ֵ�����ͳ��ʧ�ܣ��򷵻�-1
     */
    static INT CalculateArg( CAPTURE_IMAGE_INFO_C *pImageInfo
                              , const INT *pBlockList , BOOL IsAll = true );


    /**
     * @brief ����ͼ��Yͨ����Cͨ��������Ϣ�Ļص�����
     * @param [in] img_info_y : ��Ҫ�����ͼ��Yͨ����Ϣ
     * @param [in] img_info_c : ��Ҫ�����ͼ��Cͨ����Ϣ
     * @return - ͼ���ƽ������ֵ
     */
    static void ReceiverCallBack(CAPTURE_IMAGE_INFO_Y * img_info_y
                                 , CAPTURE_IMAGE_INFO_C * img_info_c,
                                 INT sum_y1, INT sum_y2, INT sum_y3 = 0, INT sum_y4 = 0,
                                 INT sum_y5 = 0, INT sum_y6 = 0, INT sum_y7 = 0, INT sum_y8 = 0);


private:
    // ͼ�����ݸ��»���
    static CSWRWMutex m_fSWRWMutex;
    // ������ͳ����Ϣ����
    static CSWRWMutex m_fDiffSWRWMutex;

    // Cͨ������
    static CAPTURE_IMAGE_INFO_C m_fImageInfoC;
    // Yͨ������
    static CAPTURE_IMAGE_INFO_Y m_fImageInfoY;
    // ABͨ���ȶ�����ЧӦ����
    static FLOAT m_fVedioConsisIndex;
    // ��Ƶ����������
    static FLOAT m_fVedioDiffFactor;
    // ץ������������
    static FLOAT m_fCaptureDiffFactor;
    static BOOL m_fCaptureUpdate ;

    // Yֵ���ݸ��±�׼
    static BOOL m_fImageAgrYUpdateFlag;

    // ��ǰ����
    // ����У��Ҫ����ͳ�ƶ�֡ȡƽ�������жϡ�
    static INT m_nCurrentCounters;
    static BOOL m_bIsOK;

    friend class STATISTICS_INFO;
};

#endif // CSWIMAGESTATISTICSINFO_H

