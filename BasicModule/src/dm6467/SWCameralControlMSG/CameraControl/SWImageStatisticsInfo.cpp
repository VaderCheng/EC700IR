
#include "swpa_mem.h"
#include "SWTask.h"
#include "SWImageStatisticsInfo.h"
#include "SWLog.h"

typedef struct STATISTICS_INFO{
    STATISTICS_INFO(){
        INT nSize = CSWImageStatisticsInfo::MAX_IMAGEBLOCK_INFO;
        for( int i = 0 ; i < nSize; i++)
        {
            CSWImageStatisticsInfo::fpBlockList[i] = true;
        }

        swpa_vpif_set_imginfo_callback( CSWImageStatisticsInfo::ReceiverCallBack);
    }
}STATISTICS_INFO;


BOOL *CSWImageStatisticsInfo::fpBlockList = new BOOL[ CSWImageStatisticsInfo::MAX_IMAGEBLOCK_INFO ];

// Cͨ������
CAPTURE_IMAGE_INFO_C CSWImageStatisticsInfo::m_fImageInfoC;
// Yͨ������
CAPTURE_IMAGE_INFO_Y CSWImageStatisticsInfo::m_fImageInfoY;
// ���ݶ�д����
CSWRWMutex CSWImageStatisticsInfo::m_fSWRWMutex;
// ������ͳ�ƻ���
CSWRWMutex CSWImageStatisticsInfo::m_fDiffSWRWMutex;


FLOAT CSWImageStatisticsInfo::m_fVedioDiffFactor = 1.0;
FLOAT CSWImageStatisticsInfo::m_fCaptureDiffFactor = 1.0;

BOOL CSWImageStatisticsInfo::m_fCaptureUpdate = false;

BOOL CSWImageStatisticsInfo::fIsAllNoSelect = false;

// ABͨ���ȶ�����ЧӦ����
FLOAT CSWImageStatisticsInfo::m_fVedioConsisIndex = 0;

// ����ֵ���±�־
BOOL CSWImageStatisticsInfo::m_fImageAgrYUpdateFlag = false;

INT CSWImageStatisticsInfo::m_nCurrentCounters = 0;

BOOL CSWImageStatisticsInfo::m_bIsOK = false;

/**
* ����ͼ�����ݽ���ͳ��,�÷�ʽ��������
*/
BOOL CSWImageStatisticsInfo::InputData( PVOID pData , INT nWidth , INT nHeight )
{
    void* pImageYUV422Data = pData;

    DWORD fYL , fYR ; fYL = fYR = 0 ;
    for( int i = 0 ; i < (nHeight - 1); i++)
    {
        BYTE *pYDataBuffer = (BYTE*)pImageYUV422Data + i*nWidth;

        fYL += (BYTE)(pYDataBuffer[nWidth / 2 - 1]);
        fYR += (BYTE)(pYDataBuffer[ nWidth / 2]);

        fYL += (BYTE)(pYDataBuffer[nWidth / 2 - 2]);
        fYR += (BYTE)(pYDataBuffer[nWidth / 2 + 1]);

        /*fYL += (BYTE)(pYDataBuffer[974 - 1]);
        fYR += (BYTE)(pYDataBuffer[ 974]);

        fYL += (BYTE)(pYDataBuffer[974 - 2]);
        fYR += (BYTE)(pYDataBuffer[974 + 1]);*/
    }

    CAPTURE_IMAGE_INFO_C stChannelInfo;

    INT *pAvgy = &stChannelInfo.avg_y_15;

    char *pAWBValueData = (char*)pData + nWidth*(2*nHeight - 1) + sizeof(INT)*25;
    INT *AWBR =  &stChannelInfo.sum_r;

    // ��ȡAWBͳ����Ϣ
    for( int k = 0 ; k < 6; k++)
    {
        /*UINT uVlaue = ((pAWBValueData[k*2*4 ]) << 24);
        uVlaue |= ((pAWBValueData[k*2*4 + 1]) << 16);
        uVlaue |= ((pAWBValueData[k*2*4 + 2]) << 8);
        uVlaue |= (pAWBValueData[k*2*4 + 3]);*/

        UINT uVlaue = (pAWBValueData[ k*2*4 ]);
        uVlaue |= ((pAWBValueData[ k*2*4 + 1]) << 8 );
        uVlaue |= ((pAWBValueData[ k*2*4 + 2]) << 16);
        uVlaue |= ((pAWBValueData[ k*2*4 + 3]) << 24);

        *AWBR = uVlaue; AWBR += 2;

    }

    char *pYValueData = (char*)pData + nWidth*(2*nHeight - 1) + sizeof(INT)*32;

    DWORD dwSum = 0 ;

    // ��ȡAGCͳ����Ϣ
    for( int j = 0 ; j < 16; j++)
    {
        UINT uValue = pYValueData[ j*2*4 ];
        *pAvgy = uValue; pAvgy += 2; dwSum += uValue;
    }

    char buffer[128];

    //
    ReceiverCallBack( NULL , &stChannelInfo , fYL , fYR);

}

/**
 * @brief ��ʼ������
 * @return - �ɹ�����true,ʧ�ܷ���false
 */
BOOL CSWImageStatisticsInfo::Initialization( )
{
    INT nSize = CSWImageStatisticsInfo::MAX_IMAGEBLOCK_INFO;
    for( int i = 0 ; i < nSize; i++)
    {
        CSWImageStatisticsInfo::fpBlockList[i] = true;
    }

    swpa_vpif_set_imginfo_callback( CSWImageStatisticsInfo::ReceiverCallBack);
}

/**
 * @brief ����ͼ��Yͨ����Cͨ��������Ϣ�Ļص�����
 * @param [in] img_info_y : ��Ҫ�����ͼ��Yͨ����Ϣ
 * @param [in] img_info_c : ��Ҫ�����ͼ��Cͨ����Ϣ
 * @return - ͼ���ƽ������ֵ
 */
void CSWImageStatisticsInfo::ReceiverCallBack(CAPTURE_IMAGE_INFO_Y * img_info_y
                                              , CAPTURE_IMAGE_INFO_C * img_info_c
                                              , INT sum_y1, INT sum_y2, INT sum_y3, INT sum_y4
                                              , INT sum_y5, INT sum_y6, INT sum_y7, INT sum_y8)
{


    CSWAutoLock objLock( &CSWImageStatisticsInfo::m_fSWRWMutex );

    static int siSumy3 = 0;
    static int siSumy4 = 0;
    static int siSumy5 = 0;
    static int siSumy6 = 0;
    static int siSumy7 = 0;

    if( img_info_y != NULL && img_info_y->capture_en == 0 )
    {
        if( img_info_y != NULL )
            swpa_memcpy( &CSWImageStatisticsInfo::m_fImageInfoY
                        , img_info_y ,sizeof( CAPTURE_IMAGE_INFO_Y));

        if( img_info_c != NULL ){
            swpa_memcpy( &CSWImageStatisticsInfo::m_fImageInfoC
                        , img_info_c ,sizeof( CAPTURE_IMAGE_INFO_C));

            CSWImageStatisticsInfo::m_fImageAgrYUpdateFlag = true;
        }
    }


    CSWAutoLock objDiffLock( &CSWImageStatisticsInfo::m_fDiffSWRWMutex );


    if( sum_y2 != 0 ){
        if( img_info_y != NULL && img_info_y->capture_en != 0 ){
            CSWImageStatisticsInfo::m_fCaptureDiffFactor = 1.0*sum_y1 / sum_y2;
            CSWImageStatisticsInfo::m_fCaptureUpdate = true;
        }
        else{


            // CSWImageStatisticsInfo::m_fVedioDiffFactor = 1.0*sum_y1 / sum_y2;
        	if( !CSWImageStatisticsInfo::m_bIsOK )
        	{
				siSumy3 += sum_y3;
				siSumy4 += sum_y4;
				siSumy5 += sum_y5;
				siSumy6 += sum_y6;
				siSumy7 += sum_y7;

				int iy1, iy2;
				if( sum_y4 > sum_y5 )
				{
				    iy1 = sum_y3;
				    iy2 = sum_y6;
				}
				else
				{
					iy1 = sum_y7;
					iy2 = sum_y8;
				}

				if( sum_y4 == 0 && sum_y5 == 0 )
				 {
					 iy1 = sum_y1;
					 iy2 = sum_y2;
				 }

				FLOAT fABChannelArg = 0.5*(iy1 + iy2) ;
				// ��ȡ�����Է���
				FLOAT dbMolecular = swpa_sqrt(swpa_pow( iy1 - fABChannelArg , 2)
										 + swpa_pow( iy2 - fABChannelArg , 2));
				// ABͨ���ȶ�����ЧӦ����
				CSWImageStatisticsInfo::m_fVedioConsisIndex += dbMolecular / fABChannelArg *100;



				CSWImageStatisticsInfo::m_fVedioDiffFactor += 1.0*iy1 / iy2;


			   if( CSWImageStatisticsInfo::m_nCurrentCounters++ > 9){
				   CSWImageStatisticsInfo::m_fVedioDiffFactor = CSWImageStatisticsInfo::m_fVedioDiffFactor / CSWImageStatisticsInfo::m_nCurrentCounters;

				   CSWImageStatisticsInfo::m_fVedioConsisIndex = CSWImageStatisticsInfo::m_fVedioConsisIndex / CSWImageStatisticsInfo::m_nCurrentCounters;

				   SW_TRACE_DEBUG("<diff info>f:%f, c:%f. max:%d; min:%d.",
						   CSWImageStatisticsInfo::m_fVedioDiffFactor, CSWImageStatisticsInfo::m_fVedioConsisIndex,

						   siSumy4 / CSWImageStatisticsInfo::m_nCurrentCounters,

						   siSumy5 / CSWImageStatisticsInfo::m_nCurrentCounters

						   );

				   siSumy3 = siSumy4 = siSumy5 = siSumy6 = siSumy7 = 0;

				   CSWImageStatisticsInfo::m_bIsOK = true; CSWImageStatisticsInfo::m_nCurrentCounters = 0;


				}

        	}
        }
    }
}

/**
 * @brief ��ȡ����ͳ��ֵ
 * @return - Ϊ��Ƶ����������
 */
VOID CSWImageStatisticsInfo::GetVedioDiffStatistValue( FLOAT& fDiffFactor , FLOAT& fConsisIndex )
{
	if( CSWImageStatisticsInfo::m_bIsOK == false ){
		fDiffFactor = 1.0; fConsisIndex  = 0.0; return;
	}


    CSWAutoLock objDiffLock( &CSWImageStatisticsInfo::m_fDiffSWRWMutex );
    fDiffFactor = CSWImageStatisticsInfo::m_fVedioDiffFactor;
    CSWImageStatisticsInfo::m_fVedioDiffFactor = 0.0; CSWImageStatisticsInfo::m_bIsOK  = false;
    // ABͨ���ȶ�����ЧӦ����
    fConsisIndex = CSWImageStatisticsInfo::m_fVedioConsisIndex;
    CSWImageStatisticsInfo::m_fVedioConsisIndex = 0.0;
}
 /**
 * @brief ��ȡ����ͳ��ֵ
 * @return - Ϊץ�Ĳ�������
 */
VOID CSWImageStatisticsInfo::GetCaptureDiffStatistValue( FLOAT& fDiffFactor , BOOL &IsUpdate  )
{
     CSWAutoLock objDiffLock( &CSWImageStatisticsInfo::m_fDiffSWRWMutex );
     fDiffFactor = CSWImageStatisticsInfo::m_fCaptureDiffFactor;
     IsUpdate = CSWImageStatisticsInfo::m_fCaptureUpdate;
     CSWImageStatisticsInfo::m_fCaptureUpdate = false;
}

/**
 * @brief ��ȡ����ͼƬ��ƽ������ֵ,ÿһ���Ӹ���һ��,��ֻ���û�����ʱ�Ż����
 * @return - ͼ���ƽ������ֵ
 */
INT CSWImageStatisticsInfo::GetYArgValue( )
{
    CSWAutoLock objLock( &CSWImageStatisticsInfo::m_fSWRWMutex );
    // ��ȡͼ���ƽ������
    return CSWImageStatisticsInfo::CalculateArg( &CSWImageStatisticsInfo::m_fImageInfoC
                                                , CSWImageStatisticsInfo::fpBlockList , true );
}
/**
 * @brief ��ȡ����ͼƬ��ƽ������ֵ,ÿһ���Ӹ���һ��,��ֻ���û�����ʱ�Ż����
 * @return - ͼ���ƽ������ֵ,���ƽ������ֵû�и����򷵻�-1,
 * ע��:�÷���ֻΪAGC�ṩ����
 */
INT CSWImageStatisticsInfo::GetAGCYArgValue( )
{
    CSWAutoLock objLock( &CSWImageStatisticsInfo::m_fSWRWMutex );
    // ��ȡͼ���ƽ������
    return CSWImageStatisticsInfo::CalculateArg( &CSWImageStatisticsInfo::m_fImageInfoC
                                                , fpBlockList , true );
}
/**
 * @brief ��ȡͼƬ�Ĳ���ƽ������ֵ���ֲ�λ�����ⲿָ��,ÿһ���Ӹ���һ��,��ֻ���û�����ʱ�Ÿ���
 * @return - ͼ��ľֲ�������ͼƬ��ƽ������ֵ
 */
INT CSWImageStatisticsInfo::GetPartYArgValue( )
{
    CSWAutoLock objLock( &CSWImageStatisticsInfo::m_fSWRWMutex );
     // ��ȡͼ���ƽ������
    return CSWImageStatisticsInfo::CalculateArg( &CSWImageStatisticsInfo::m_fImageInfoC
                                                , CSWImageStatisticsInfo::fpBlockList , false );

}
/**
 * @brief ��ȡ���ͼ�����
 * @return - ��������ͳ�Ƶ�ͼ�����
 */
DWORD CSWImageStatisticsInfo::GetMaxImageBlock( )
{
    return MAX_IMAGEBLOCK_INFO;
}
/**
 * @brief ������Ҫͳ��Yֵ��ͼ��飬ͼ��ͳ�Ʒ���Ĭ�϶���Ϊ��ͳ��״̬,
 * bIsStatistics = true��ʾ��Ҫͳ�ƣ�����Ϊ��ͳ�Ƹ�ͼ��Yֵ
 * @param [in] bIsStatistics : false����Ҫͳ�ƣ�true��Ҫͳ�Ƹ�ͼ���Yֵ
 * @param [in] nIndex : ��Ҫͳ�ƻ�ͳ�Ƶ�ͼ���λ��
 * @return - S_OK : �ɹ� - S_FALSE : ʧ��
 */
HRESULT CSWImageStatisticsInfo::SetImageListInfo( BOOL bIsStatistics ,  INT nIndex )
{
    if( nIndex >= CSWImageStatisticsInfo::MAX_IMAGEBLOCK_INFO) return S_FALSE;

    CSWImageStatisticsInfo::fpBlockList[nIndex] = bIsStatistics;

    CSWImageStatisticsInfo::fIsAllNoSelect = true;
    for( int i = 0 ; i < CSWImageStatisticsInfo::MAX_IMAGEBLOCK_INFO; i++)
    {
        if( CSWImageStatisticsInfo::fpBlockList[i] ){
            CSWImageStatisticsInfo::fIsAllNoSelect = false; break;
        }
    }

    return S_OK;
}

/**ImageInfo , name
 * @brief ͳ��ͼ���ƽ������ֵ
 * @param [in] pImageInfo : ��Ҫ�����ͼ����Ϣ
 * @param [in] pBlockList : ��Ҫͳ�Ƶ�ͼ����б�
 * @param [in] IsAll : �Ƿ���Ҫͳ������ͼ���Yֵ��Ĭ��Ϊ��Ҫͳ��
 * ���ֻ��Ҫͳ�Ʋ���ͼ����Yֵ��Ϣ�����ֵΪfalse
 * @return - �ɹ�����ͼ���ƽ������ֵ,ʧ�ܷ���-1
 */
INT CSWImageStatisticsInfo::CalculateArg( CAPTURE_IMAGE_INFO_C *pImageInfo
                              , const INT *pBlockList , BOOL IsAll )
{
    if( CSWImageStatisticsInfo::fIsAllNoSelect && !IsAll ) return -2;
    if( CSWImageStatisticsInfo::m_fImageAgrYUpdateFlag == false) return -1;

    CSWImageStatisticsInfo::m_fImageAgrYUpdateFlag = false;

    if( pImageInfo == NULL ||  pBlockList == NULL ) return -1;

    INT nImageYArg = 0 ; INT nCalculateBlockCounts = 0;

    for( int i = 0; i < CSWImageStatisticsInfo::MAX_IMAGEBLOCK_INFO; i++)
    {
        if( pBlockList[i] || IsAll){
            nImageYArg +=  *((INT*)(&pImageInfo->avg_y_0) - i*2); nCalculateBlockCounts++;
        }

    }

    return (nCalculateBlockCounts == 0 ? -1 : (0.5 + 1.0*nImageYArg / nCalculateBlockCounts));
}
 /**
 * @brief ��ȡͼƬ����ͨ����ͳ��ֵ
 * @param [out] nRSum : Rͨ����ͳ����ֵ
 * @param [out] nGSum : Gͨ����ͳ����ֵ
 * @param [out] nBSum : Bͨ����ͳ����ֵ
 * @return - S_OK : �ɹ� - S_FALSE : ʧ��
 */
 HRESULT CSWImageStatisticsInfo::GetRGBSum( INT& nRSum , INT& nGSum , INT& nBSum )
 {
    CSWAutoLock objLock( &CSWImageStatisticsInfo::m_fSWRWMutex );
    nRSum = CSWImageStatisticsInfo::m_fImageInfoC.sum_r;
    nGSum = CSWImageStatisticsInfo::m_fImageInfoC.sum_g;
    nBSum = CSWImageStatisticsInfo::m_fImageInfoC.sum_b;

    CSWImageStatisticsInfo::m_fImageInfoC.sum_r = CSWImageStatisticsInfo::m_fImageInfoC.sum_g = CSWImageStatisticsInfo::m_fImageInfoC.sum_b = 0;

    return S_OK;
 }



