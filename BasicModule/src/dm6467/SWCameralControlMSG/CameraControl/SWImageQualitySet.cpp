#include "SWImageQualitySet.h"
#include "swpa_camera.h"

CSWImageQualitySet::CSWImageQualitySet()
: m_nMode( MODE_CLOSE )
, m_nSharpenEnMode( MODE_CLOSE )
, m_nHazeMode( MODE_CLOSE )
, m_nContrast( 0 )
, m_nSaturation( 0 )
, m_nTemperature( 0 )
, m_nHue( 0 )
, m_nHazeThreshold( 8 )
, m_nSharpenThreshold( 7 )
{
    //ctor
}

CSWImageQualitySet::~CSWImageQualitySet()
{
    //dtor
}

/**
* @brief ����ɫ�ʲ���\n
*
* @param [in] mode ����ģʽ MODE_CLOSE-�ر�ɫ�ʵ������ܣ�MODE_CAPTURE-������ץ��֡��
* MODE_VIDEO_CAPUTRUE-����������֡
* @note .
* @see .
*/
INT CSWImageQualitySet::SetColorParamMode( INT nMode )
{
    return swpa_camera_imgproc_set_color_param( (m_nMode = nMode), m_nContrast , m_nSaturation
                                                , m_nTemperature , m_nHue);
}
/**
* @brief ����ɫ�ʲ���\n
*
* @param [in] contrast_value �Աȶ�ֵ����Χ��[-100~100]��
* Ĭ��ֵ��0(��ֵ���Ӧ�ò㣬����õ���������FPGA�Ĵ�����Ĭ��ֵ)
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
INT CSWImageQualitySet::SetImageContrast( INT nContrast)
{
    return swpa_camera_imgproc_set_color_param( m_nMode, (m_nContrast = nContrast) , m_nSaturation
                                                , m_nTemperature , m_nHue);
}
/**
* @brief ����ɫ�ʲ���\n
*
* @param [in] saturation_value ���Ͷ�ֵ����Χ��[-100~100]��
* Ĭ��ֵ��0(��ֵ���Ӧ�ò㣬����õ���r/g/b������FPGA�Ĵ�����Ĭ��ֵ)
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
 INT CSWImageQualitySet::SetImageSaturation( INT nSaturation)
 {
    return swpa_camera_imgproc_set_color_param( m_nMode, m_nContrast , (m_nSaturation = nSaturation)
                                                , m_nTemperature , m_nHue);
 }
/**
* @brief ����ɫ�ʲ���\n
*
* @param [in] color_temperature_value ɫ��ֵ
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
 INT CSWImageQualitySet::SetImageTemperature( INT nTemperature)
 {
    return swpa_camera_imgproc_set_color_param( m_nMode, m_nContrast ,m_nSaturation
                                                , (m_nTemperature = nTemperature) , m_nHue);
 }
/**
* @brief ����ɫ�ʲ���\n
*
* @param [in] hue_value ɫ��ֵ��ɫ�ʡ�ɫ�ࣩ
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note .
* @see .
*/
 INT CSWImageQualitySet::SetImageHue( INT nHue)
 {
    return swpa_camera_imgproc_set_color_param( m_nMode, m_nContrast ,m_nSaturation
                                                , m_nTemperature , (m_nHue = nHue));
 }

 // ��ȡ��ز���
INT CSWImageQualitySet::GetColorParam( INT &nMode , INT &nContrast
                   ,  INT &nSaturation , INT &nTemperature , INT &nHue )
{
    nMode = m_nMode;
    nContrast = m_nContrast;
    nSaturation = m_nSaturation;
    nTemperature = m_nTemperature;
    nHue = m_nHue;

    return S_OK;
}

/**
* @brief ��������ز���\n
*
*
* @param [in] mode ����ģʽ 1-�ر��񻯹��ܣ�2-������ץ��֡��3-����������֡
* @param [in] threshold ��ֵ ��Χ0-127
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note
* @see .
*/
INT CSWImageQualitySet::SetSharpenParam( INT threshold )
{
    return swpa_camera_imgproc_set_sharpen_param( m_nSharpenEnMode , (m_nSharpenThreshold = threshold) );
}
INT CSWImageQualitySet::SetSharpenMode( INT nMode )
{
    return swpa_camera_imgproc_set_sharpen_param((m_nSharpenEnMode = nMode) , m_nSharpenThreshold );

}
/**
* @brief ��ȡ����ز���\n
*
*
* @param [out] mode ����ģʽ 1-�ر��񻯹��ܣ�2-������ץ��֡��3-����������֡
* @param [out] threshold ��ֵ ��Χ0-127
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note
* @see .
*/
INT CSWImageQualitySet::GetSharpenParam( INT &nMode , INT &threshold )
{
    return swpa_camera_imgproc_get_sharpen_param( &nMode , &threshold );
}

VOID CSWImageQualitySet::SetGamma( INT *pData )
{
    swpa_memcpy(m_arrGamma,pData,GAMMA_ROW*GAMMA_COL*sizeof(INT));
    swpa_camera_imgproc_set_gamma( m_arrGamma );
}
/**
* @brief ���ó�����ز���\n
*
*
* @param [in] mode ����ģʽ MODE_CLOSE-�ر��񻯵������ܣ�MODE_CAPTURE-������ץ��֡��
* MODE_VIDEO_CAPUTRUE-����������֡
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note
* @see .
*/
INT CSWImageQualitySet::SetHazeRemovalMode( INT nMode )
{
    return swpa_camera_imgproc_set_defog_param((m_nHazeMode = nMode) , m_nHazeThreshold);
}
/**
* @brief ���ó�����ز���\n
*
* @param [in] threshold ��ֵ ��Χ0-10
*/
INT CSWImageQualitySet::SetHazeRemovalParam( INT threshold )
{
    return swpa_camera_imgproc_set_defog_param( m_nHazeMode
                                               , (m_nHazeThreshold = threshold) );
}



